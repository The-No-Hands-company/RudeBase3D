#include <GL/glew.h>
#include "Renderer.h"
#include "Mesh.h"
#include "core/mesh.hpp"
#include "Material.h"
#include "core/shader_utils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
// Removed Qt includes

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
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        spdlog::error("Failed to initialize GLEW");
        return false;
    }
    
    // Load default shaders
    if (!loadShaders()) {
        // Log error using spdlog or std::cerr
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
    
    setClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    
    return true;
}

void Renderer::cleanup()
{
    // TODO: Temporarily commented out Qt OpenGL context check due to missing Qt OpenGL includes
    // if (QOpenGLContext::currentContext()) {
        if (m_lineVAO != 0) {
            glDeleteVertexArrays(1, &m_lineVAO);
            m_lineVAO = 0;
        }
        
        if (m_lineVBO != 0) {
            glDeleteBuffers(1, &m_lineVBO);
            m_lineVBO = 0;
        }
    // }
    
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
    auto it = m_shaderPrograms.find(std::string(name));
    if (it != m_shaderPrograms.end()) {
        m_currentShader = it->second.get();
        glUseProgram(m_currentShader->programID);
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

void Renderer::setViewMatrix(const glm::mat4& view)
{
    m_viewMatrix = view;
}

void Renderer::setProjectionMatrix(const glm::mat4& projection)
{
    m_projectionMatrix = projection;
}

void Renderer::setModelMatrix(const glm::mat4& model)
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
    float alpha = material->getDiffuseColor().w;
    if (alpha < 0.99f) {
        // Enable blending for transparent materials
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        // Disable blending for opaque materials to prevent transparency issues
        glDisable(GL_BLEND);
    }
    
    if (shader.diffuseColorLoc >= 0) {
        glUniform4fv(shader.diffuseColorLoc, 1, glm::value_ptr(material->getDiffuseColor()));
    }
    
    if (shader.specularColorLoc >= 0) {
        glUniform4fv(shader.specularColorLoc, 1, glm::value_ptr(material->getSpecularColor()));
    }
    
    if (shader.ambientColorLoc >= 0) {
        glUniform4fv(shader.ambientColorLoc, 1, glm::value_ptr(material->getAmbientColor()));
    }
    
    if (shader.shininessLoc >= 0) {
        glUniform1f(shader.shininessLoc, material->getShininess());
    }
}

void Renderer::setLighting(const glm::vec3& lightDir, const glm::vec4& lightColor)
{
    m_lightDirection = lightDir;
    m_lightColor = lightColor;
    
    if (!m_currentShader) {
        return;
    }
    auto& shader = *m_currentShader;
    if (shader.lightDirectionLoc >= 0) {
        glUniform3fv(shader.lightDirectionLoc, 1, glm::value_ptr(m_lightDirection));
    }
    if (shader.lightColorLoc >= 0) {
        glUniform4fv(shader.lightColorLoc, 1, glm::value_ptr(m_lightColor));
    }
}

void Renderer::renderMesh(rude::MeshPtr mesh, RenderMode mode)
{
    if (!mesh) {
        return;
    }
    
    switch (mode) {
        case RenderMode::Wireframe:
            useShaderProgram("wireframe");
            updateUniforms();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            mesh->render();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            mesh->render();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDisable(GL_POLYGON_OFFSET_FILL);
            break;
    }
}

void Renderer::renderLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
{
    // TODO: Temporarily commented out due to shader system architectural issues
    // Need to resolve Qt OpenGL vs raw OpenGL approach and fix ShaderProgram member access
    spdlog::debug("Renderer::renderLine() called but temporarily disabled - Start: ({}, {}, {}) End: ({}, {}, {}) Color: ({}, {}, {}, {})", 
                  start.x, start.y, start.z, end.x, end.y, end.z, color.x, color.y, color.z, color.w);
    /*
    useShaderProgram("line");
    if (!m_currentShader) {
        spdlog::error("No line shader available!");
        return;
    }
    spdlog::debug("Using line shader, updating uniforms...");
    // Update uniforms
    updateUniforms();
    if (m_currentShader->colorLoc >= 0) {
        m_currentShader->program->setUniformValue(m_currentShader->colorLoc, glm::value_ptr(color));
        spdlog::debug("Color uniform set successfully");
    } else {
        spdlog::warn("Color uniform location not found!");
    }
    // Upload line data
    float vertices[] = {
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };
    spdlog::debug("Uploading vertex data and rendering line...");
    glBindVertexArray(m_lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
    spdlog::debug("Renderer::renderLine() completed");
    */
}

void Renderer::renderAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color)
{
    // Draw the 12 edges of the bounding box
    glm::vec3 corners[8] = {
        glm::vec3(min.x, min.y, min.z), // 0
        glm::vec3(max.x, min.y, min.z), // 1
        glm::vec3(max.x, max.y, min.z), // 2
        glm::vec3(min.x, max.y, min.z), // 3
        glm::vec3(min.x, min.y, max.z), // 4
        glm::vec3(max.x, min.y, max.z), // 5
        glm::vec3(max.x, max.y, max.z), // 6
        glm::vec3(min.x, max.y, max.z)  // 7
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

void Renderer::setClearColor(const glm::vec4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}

bool Renderer::createShaderProgram(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
{
    // Create a simple OpenGL shader compilation
    
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSrc = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);
    
    // Check vertex shader compilation
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        spdlog::error("Vertex shader compilation failed for {}: {}", name, infoLog);
        glDeleteShader(vertexShader);
        return false;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSrc = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);
    
    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        spdlog::error("Fragment shader compilation failed for {}: {}", name, infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check program linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        spdlog::error("Shader program linking failed for {}: {}", name, infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        return false;
    }
    
    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Create and store shader program
    auto shader = std::make_unique<ShaderProgram>();
    shader->programID = shaderProgram;
    
    // Get uniform locations
    shader->mvpMatrixLoc = glGetUniformLocation(shaderProgram, "mvpMatrix");
    shader->modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
    shader->viewMatrixLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
    shader->projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
    shader->normalMatrixLoc = glGetUniformLocation(shaderProgram, "normalMatrix");
    shader->colorLoc = glGetUniformLocation(shaderProgram, "color");
    shader->diffuseColorLoc = glGetUniformLocation(shaderProgram, "diffuseColor");
    shader->lightDirectionLoc = glGetUniformLocation(shaderProgram, "lightDirection");
    shader->lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    shader->viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    
    m_shaderPrograms[name] = std::move(shader);
    
    spdlog::info("Shader program '{}' compiled and linked successfully", name);
    return true;
}

void Renderer::updateUniforms()
{
    // TODO: Temporarily commented out due to shader system architectural issues
    // Need to resolve Qt vs glm matrix types and Qt OpenGL vs raw OpenGL approach
    if (!m_currentShader) {
        return;
    }
    /*
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
    */
}

void Renderer::initializeLineRenderer()
{
    glGenVertexArrays(1, &m_lineVAO);
    glGenBuffers(1, &m_lineVBO);
}

const std::string& Renderer::getDefaultVertexShader()
{
    static std::string shader = 
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"\n"
"uniform mat4 mvpMatrix;\n"
"uniform mat4 modelMatrix;\n"
"uniform mat4 viewMatrix;\n"
"uniform mat4 projectionMatrix;\n"
"uniform mat3 normalMatrix;\n"
"\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec2 TexCoord;\n"
"\n"
"void main()\n"
"{\n"
"    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));\n"
"    Normal = normalMatrix * aNormal;\n"
"    TexCoord = aTexCoord;\n"
"    gl_Position = mvpMatrix * vec4(aPos, 1.0);\n"
"}\n";
    return shader;
}

const std::string& Renderer::getDefaultFragmentShader()
{
    static std::string shader = 
"#version 330 core\n"
"\n"
"struct Material {\n"
"    vec4 diffuseColor;\n"
"    vec4 specularColor;\n"
"    vec4 ambientColor;\n"
"    float shininess;\n"
"};\n"
"\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec2 TexCoord;\n"
"\n"
"uniform Material material;\n"
"uniform vec3 lightDirection;\n"
"uniform vec4 lightColor;\n"
"uniform vec3 viewPos;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    // Normalize vectors\n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 lightDir = normalize(-lightDirection);\n"
"    vec3 viewDir = normalize(viewPos - FragPos);\n"
"    \n"
"    // Ambient\n"
"    vec3 ambient = material.ambientColor.rgb * lightColor.rgb;\n"
"    \n"
"    // Diffuse\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = diff * material.diffuseColor.rgb * lightColor.rgb;\n"
"    \n"
"    // Specular\n"
"    vec3 reflectDir = reflect(-lightDir, norm);\n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
"    vec3 specular = spec * material.specularColor.rgb * lightColor.rgb;\n"
"    \n"
"    vec3 result = ambient + diffuse + specular;\n"
"    FragColor = vec4(result, material.diffuseColor.a);\n"
"}\n";
    return shader;
}

const std::string& Renderer::getWireframeVertexShader()
{
    static std::string shader = 
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"uniform mat4 mvpMatrix;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = mvpMatrix * vec4(aPos, 1.0);\n"
"}\n";
    return shader;
}

const std::string& Renderer::getWireframeFragmentShader()
{
    static std::string shader = 
"#version 330 core\n"
"\n"
"uniform vec4 color;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = color;\n"
"}\n";
    return shader;
}

const std::string& Renderer::getLineVertexShader()
{
    return getWireframeVertexShader();
}

const std::string& Renderer::getLineFragmentShader()
{
    return getWireframeFragmentShader();
}

void Renderer::setViewPosition(const glm::vec3& viewPos)
{
    m_viewPosition = viewPos;
    if (!m_currentShader) {
        return;
    }
    // TODO: Temporarily commented out due to shader system architectural issues
    /*
    auto& shader = *m_currentShader;
    if (shader.viewPosLoc >= 0) {
        shader.program->setUniformValue(shader.viewPosLoc, glm::value_ptr(m_viewPosition));
    }
    */
}
