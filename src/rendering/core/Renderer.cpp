#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include <QOpenGLContext>
#include <QDebug>

Renderer::Renderer()
    : m_currentShader(nullptr)
    , m_lightDirection(0.0f, -1.0f, -1.0f)
    , m_lightColor(1.0f, 1.0f, 1.0f, 1.0f)
    , m_lineVAO(0)
    , m_lineVBO(0)
{
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::initialize()
{
    initializeOpenGLFunctions();
    
    // Load default shaders
    if (!loadShaders()) {
        qDebug() << "Failed to load shaders";
        return false;
    }
    
    // Initialize line renderer
    initializeLineRenderer();
    
    // Set default OpenGL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    setClearColor(QVector4D(0.2f, 0.2f, 0.2f, 1.0f));
    
    return true;
}

void Renderer::cleanup()
{
    if (QOpenGLContext::currentContext()) {
        if (m_lineVAO != 0) {
            glDeleteVertexArrays(1, &m_lineVAO);
            m_lineVAO = 0;
        }
        
        if (m_lineVBO != 0) {
            glDeleteBuffers(1, &m_lineVBO);
            m_lineVBO = 0;
        }
    }
    
    m_shaderPrograms.clear();
}

bool Renderer::loadShaders()
{
    // Load default solid shader
    if (!createShaderProgram("default", getDefaultVertexShader(), getDefaultFragmentShader())) {
        return false;
    }
    
    // Load wireframe shader
    if (!createShaderProgram("wireframe", getWireframeVertexShader(), getWireframeFragmentShader())) {
        return false;
    }
    
    // Load line shader
    if (!createShaderProgram("line", getLineVertexShader(), getLineFragmentShader())) {
        return false;
    }
    
    return true;
}

void Renderer::useShaderProgram(std::string_view name)
{
    QString qname = QString::fromUtf8(name.data(), name.size());
    auto it = m_shaderPrograms.find(qname);
    if (it != m_shaderPrograms.end()) {
        m_currentShader = it->second.get();
        m_currentShader->program->bind();
    }
}

void Renderer::beginFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame()
{
    // Nothing to do for now
}

void Renderer::setViewMatrix(const QMatrix4x4& view)
{
    m_viewMatrix = view;
}

void Renderer::setProjectionMatrix(const QMatrix4x4& projection)
{
    m_projectionMatrix = projection;
}

void Renderer::setModelMatrix(const QMatrix4x4& model)
{
    m_modelMatrix = model;
}

void Renderer::setMaterial(MaterialPtr material)
{
    if (!m_currentShader || !material) {
        return;
    }
    
    auto& shader = *m_currentShader;
    
    // Control blending based on material alpha
    float alpha = material->getDiffuseColor().w();
    if (alpha < 0.99f) {
        // Enable blending for transparent materials
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        // Disable blending for opaque materials to prevent transparency issues
        glDisable(GL_BLEND);
    }
    
    if (shader.diffuseColorLoc >= 0) {
        shader.program->setUniformValue(shader.diffuseColorLoc, material->getDiffuseColor());
    }
    
    if (shader.specularColorLoc >= 0) {
        shader.program->setUniformValue(shader.specularColorLoc, material->getSpecularColor());
    }
    
    if (shader.ambientColorLoc >= 0) {
        shader.program->setUniformValue(shader.ambientColorLoc, material->getAmbientColor());
    }
    
    if (shader.shininessLoc >= 0) {
        shader.program->setUniformValue(shader.shininessLoc, material->getShininess());
    }
}

void Renderer::setLighting(const QVector3D& lightDir, const QVector4D& lightColor)
{
    m_lightDirection = lightDir;
    m_lightColor = lightColor;
    
    if (!m_currentShader) {
        return;
    }
    
    auto& shader = *m_currentShader;
    
    if (shader.lightDirectionLoc >= 0) {
        shader.program->setUniformValue(shader.lightDirectionLoc, m_lightDirection);
    }
    
    if (shader.lightColorLoc >= 0) {
        shader.program->setUniformValue(shader.lightColorLoc, m_lightColor);
    }
}

void Renderer::renderMesh(MeshPtr mesh, RenderMode mode)
{
    if (!mesh) {
        return;
    }
    
    switch (mode) {
        case RenderMode::Wireframe:
            useShaderProgram("wireframe");
            updateUniforms();
            mesh->renderWireframe();
            break;
            
        case RenderMode::Solid:
            useShaderProgram("default");
            updateUniforms();
            mesh->render();
            break;
            
        case RenderMode::SolidWireframe:
            // Render solid first
            useShaderProgram("default");
            updateUniforms();
            mesh->render();
            
            // Then render wireframe on top
            glPolygonOffset(1.0f, 1.0f);
            glEnable(GL_POLYGON_OFFSET_FILL);
            useShaderProgram("wireframe");
            updateUniforms();
            mesh->renderWireframe();
            glDisable(GL_POLYGON_OFFSET_FILL);
            break;
    }
}

void Renderer::renderLine(const QVector3D& start, const QVector3D& end, const QVector4D& color)
{
    qDebug() << "Renderer::renderLine() called - Start:" << start << "End:" << end << "Color:" << color;
    
    useShaderProgram("line");
    
    if (!m_currentShader) {
        qDebug() << "ERROR: No line shader available!";
        return;
    }
    
    qDebug() << "Using line shader, updating uniforms...";
    
    // Update uniforms
    updateUniforms();
    
    if (m_currentShader->colorLoc >= 0) {
        m_currentShader->program->setUniformValue(m_currentShader->colorLoc, color);
        qDebug() << "Color uniform set successfully";
    } else {
        qDebug() << "WARNING: Color uniform location not found!";
    }
    
    // Upload line data
    float vertices[] = {
        start.x(), start.y(), start.z(),
        end.x(), end.y(), end.z()
    };
    
    qDebug() << "Uploading vertex data and rendering line...";
    
    glBindVertexArray(m_lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_LINES, 0, 2);
    
    glBindVertexArray(0);
    
    qDebug() << "Renderer::renderLine() completed";
}

void Renderer::renderAABB(const QVector3D& min, const QVector3D& max, const QVector4D& color)
{
    // Draw the 12 edges of the bounding box
    QVector3D corners[8] = {
        QVector3D(min.x(), min.y(), min.z()), // 0
        QVector3D(max.x(), min.y(), min.z()), // 1
        QVector3D(max.x(), max.y(), min.z()), // 2
        QVector3D(min.x(), max.y(), min.z()), // 3
        QVector3D(min.x(), min.y(), max.z()), // 4
        QVector3D(max.x(), min.y(), max.z()), // 5
        QVector3D(max.x(), max.y(), max.z()), // 6
        QVector3D(min.x(), max.y(), max.z())  // 7
    };
    
    // Bottom face
    renderLine(corners[0], corners[1], color);
    renderLine(corners[1], corners[2], color);
    renderLine(corners[2], corners[3], color);
    renderLine(corners[3], corners[0], color);
    
    // Top face
    renderLine(corners[4], corners[5], color);
    renderLine(corners[5], corners[6], color);
    renderLine(corners[6], corners[7], color);
    renderLine(corners[7], corners[4], color);
    
    // Vertical edges
    renderLine(corners[0], corners[4], color);
    renderLine(corners[1], corners[5], color);
    renderLine(corners[2], corners[6], color);
    renderLine(corners[3], corners[7], color);
}

void Renderer::enableDepthTest(bool enable)
{
    if (enable) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::enableBlending(bool enable)
{
    if (enable) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }
}

void Renderer::setLineWidth(float width)
{
    glLineWidth(width);
}

void Renderer::setClearColor(const QVector4D& color)
{
    glClearColor(color.x(), color.y(), color.z(), color.w());
}

bool Renderer::createShaderProgram(const QString& name, const QString& vertexSource, const QString& fragmentSource)
{
    auto shaderProgram = std::make_unique<ShaderProgram>();
    shaderProgram->program = std::make_unique<QOpenGLShaderProgram>();
    
    // Compile vertex shader
    if (!shaderProgram->program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSource)) {
        qDebug() << "Failed to compile vertex shader for" << name;
        qDebug() << shaderProgram->program->log();
        return false;
    }
    
    // Compile fragment shader
    if (!shaderProgram->program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSource)) {
        qDebug() << "Failed to compile fragment shader for" << name;
        qDebug() << shaderProgram->program->log();
        return false;
    }
    
    // Link program
    if (!shaderProgram->program->link()) {
        qDebug() << "Failed to link shader program for" << name;
        qDebug() << shaderProgram->program->log();
        return false;
    }
    
    // Get uniform locations
    shaderProgram->mvpMatrixLoc = shaderProgram->program->uniformLocation("mvpMatrix");
    shaderProgram->modelMatrixLoc = shaderProgram->program->uniformLocation("modelMatrix");
    shaderProgram->viewMatrixLoc = shaderProgram->program->uniformLocation("viewMatrix");
    shaderProgram->projectionMatrixLoc = shaderProgram->program->uniformLocation("projectionMatrix");
    shaderProgram->normalMatrixLoc = shaderProgram->program->uniformLocation("normalMatrix");
    
    shaderProgram->diffuseColorLoc = shaderProgram->program->uniformLocation("material.diffuseColor");
    shaderProgram->specularColorLoc = shaderProgram->program->uniformLocation("material.specularColor");
    shaderProgram->ambientColorLoc = shaderProgram->program->uniformLocation("material.ambientColor");
    shaderProgram->shininessLoc = shaderProgram->program->uniformLocation("material.shininess");
    
    shaderProgram->lightDirectionLoc = shaderProgram->program->uniformLocation("lightDirection");
    shaderProgram->lightColorLoc = shaderProgram->program->uniformLocation("lightColor");
    shaderProgram->viewPosLoc = shaderProgram->program->uniformLocation("viewPos");
    
    shaderProgram->colorLoc = shaderProgram->program->uniformLocation("color");
    
    m_shaderPrograms[name] = std::move(shaderProgram);
    return true;
}

void Renderer::updateUniforms()
{
    if (!m_currentShader) {
        return;
    }
    
    auto& shader = *m_currentShader;
    QMatrix4x4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
    QMatrix3x3 normalMatrix = m_modelMatrix.normalMatrix();
    
    if (shader.mvpMatrixLoc >= 0) {
        shader.program->setUniformValue(shader.mvpMatrixLoc, mvpMatrix);
    }
    
    if (shader.modelMatrixLoc >= 0) {
        shader.program->setUniformValue(shader.modelMatrixLoc, m_modelMatrix);
    }
    
    if (shader.viewMatrixLoc >= 0) {
        shader.program->setUniformValue(shader.viewMatrixLoc, m_viewMatrix);
    }
    
    if (shader.projectionMatrixLoc >= 0) {
        shader.program->setUniformValue(shader.projectionMatrixLoc, m_projectionMatrix);
    }
    
    if (shader.normalMatrixLoc >= 0) {
        shader.program->setUniformValue(shader.normalMatrixLoc, normalMatrix);
    }
    
    if (shader.lightDirectionLoc >= 0) {
        shader.program->setUniformValue(shader.lightDirectionLoc, m_lightDirection);
    }
    
    if (shader.lightColorLoc >= 0) {
        shader.program->setUniformValue(shader.lightColorLoc, m_lightColor);
    }
    
    if (shader.viewPosLoc >= 0) {
        shader.program->setUniformValue(shader.viewPosLoc, m_viewPosition);
    }
}

void Renderer::initializeLineRenderer()
{
    glGenVertexArrays(1, &m_lineVAO);
    glGenBuffers(1, &m_lineVBO);
}

const QString& Renderer::getDefaultVertexShader()
{
    static QString shader = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    Normal = normalMatrix * aNormal;
    TexCoord = aTexCoord;
    
    gl_Position = mvpMatrix * vec4(aPos, 1.0);
}
)";
    return shader;
}

const QString& Renderer::getDefaultFragmentShader()
{
    static QString shader = R"(
#version 330 core

struct Material {
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 ambientColor;
    float shininess;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform Material material;
uniform vec3 lightDirection;
uniform vec4 lightColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // Normalize vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-lightDirection);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Ambient
    vec3 ambient = material.ambientColor.rgb * lightColor.rgb;
    
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuseColor.rgb * lightColor.rgb;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specularColor.rgb * lightColor.rgb;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, material.diffuseColor.a);
}
)";
    return shader;
}

const QString& Renderer::getWireframeVertexShader()
{
    static QString shader = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 mvpMatrix;

void main()
{
    gl_Position = mvpMatrix * vec4(aPos, 1.0);
}
)";
    return shader;
}

const QString& Renderer::getWireframeFragmentShader()
{
    static QString shader = R"(
#version 330 core

uniform vec4 color;

out vec4 FragColor;

void main()
{
    FragColor = color;
}
)";
    return shader;
}

const QString& Renderer::getLineVertexShader()
{
    return getWireframeVertexShader();
}

const QString& Renderer::getLineFragmentShader()
{
    return getWireframeFragmentShader();
}

void Renderer::setViewPosition(const QVector3D& viewPos)
{
    m_viewPosition = viewPos;
    
    if (!m_currentShader) {
        return;
    }
    
    auto& shader = *m_currentShader;
    
    if (shader.viewPosLoc >= 0) {
        shader.program->setUniformValue(shader.viewPosLoc, m_viewPosition);
    }
}
