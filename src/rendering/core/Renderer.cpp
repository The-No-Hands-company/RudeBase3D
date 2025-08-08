#include <GL/glew.h>
#include "Renderer.h"
#include "Mesh.h"
#include "core/mesh.hpp"
#include "Material.h"
#include "core/shader_utils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>
#include <vector>
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
    // Temporarily disable face culling to debug cube visibility
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
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
    // Load default solid shader from files
    std::string vertSource = readShaderFile("assets/shaders/basic.vert");
    std::string fragSource = readShaderFile("assets/shaders/basic.frag");
    
    if (vertSource.empty() || fragSource.empty()) {
        spdlog::error("Failed to load basic shader files, falling back to hardcoded shaders");
        // Fallback to hardcoded shaders
        if (!createShaderProgram("default", getDefaultVertexShader(), getDefaultFragmentShader())) {
            return false;
        }
    } else {
        if (!createShaderProgram("default", vertSource, fragSource)) {
            return false;
        }
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
        spdlog::warn("renderMesh called with null mesh");
        return;
    }
    
    // DEBUG: Log mesh data info
    spdlog::info("renderMesh: mode={}, mesh has data={}", 
                 (mode == RenderMode::Solid ? "Solid" : mode == RenderMode::Wireframe ? "Wireframe" : "SolidWireframe"),
                 (mesh ? "yes" : "no"));
    
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
    // PROFESSIONAL LINE RENDERING with proper thickness control
    
    // Save current OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    // Professional line rendering settings
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);           // Smooth antialiased lines
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Best quality
    
    // Professional line width - thin and crisp like Maya/Blender
    glLineWidth(1.5f);
    
    // Set up matrices properly for immediate mode
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(m_projectionMatrix));
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glm::mat4 modelView = m_viewMatrix * m_modelMatrix;
    glLoadMatrixf(glm::value_ptr(modelView));
    
    // Render the line with immediate mode OpenGL
    glBegin(GL_LINES);
    glColor4f(color.x, color.y, color.z, color.w);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
    
    // Restore matrices
    glPopMatrix(); // modelview
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // projection
    glMatrixMode(GL_MODELVIEW);
    
    // Restore OpenGL state
    glPopAttrib();
    
    /*
    // TODO: Original shader-based implementation (currently disabled due to architectural issues)
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
    auto shaderProgram_ptr = std::make_unique<ShaderProgram>();
    shaderProgram_ptr->programID = shaderProgram;
    
    // Uniform name constants - updated to match basic.vert/basic.frag
    constexpr const char* MVP_MATRIX_UNIFORM = "mvpMatrix";  // Legacy, may not be used
    constexpr const char* MODEL_MATRIX_UNIFORM = "model";
    constexpr const char* VIEW_MATRIX_UNIFORM = "view";
    constexpr const char* PROJECTION_MATRIX_UNIFORM = "projection";
    constexpr const char* NORMAL_MATRIX_UNIFORM = "normalMatrix";
    constexpr const char* COLOR_UNIFORM = "color";
    constexpr const char* DIFFUSE_COLOR_UNIFORM = "diffuseColor";
    constexpr const char* LIGHT_DIRECTION_UNIFORM = "lightDirection";
    constexpr const char* LIGHT_COLOR_UNIFORM = "lightColor";
    constexpr const char* VIEW_POS_UNIFORM = "viewPos";
    constexpr const char* OBJECT_COLOR_UNIFORM = "objectColor";  // Added for basic.frag
    constexpr const char* LIGHT_POS_UNIFORM = "lightPos";  // Added for basic.frag
    
    // Get uniform locations and check for missing uniforms
    shaderProgram_ptr->mvpMatrixLoc = glGetUniformLocation(shaderProgram, MVP_MATRIX_UNIFORM);
    if (shaderProgram_ptr->mvpMatrixLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", MVP_MATRIX_UNIFORM, name);
    }
    shaderProgram_ptr->modelMatrixLoc = glGetUniformLocation(shaderProgram, MODEL_MATRIX_UNIFORM);
    if (shaderProgram_ptr->modelMatrixLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", MODEL_MATRIX_UNIFORM, name);
    }
    shaderProgram_ptr->viewMatrixLoc = glGetUniformLocation(shaderProgram, VIEW_MATRIX_UNIFORM);
    if (shaderProgram_ptr->viewMatrixLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", VIEW_MATRIX_UNIFORM, name);
    }
    shaderProgram_ptr->projectionMatrixLoc = glGetUniformLocation(shaderProgram, PROJECTION_MATRIX_UNIFORM);
    if (shaderProgram_ptr->projectionMatrixLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", PROJECTION_MATRIX_UNIFORM, name);
    }
    shaderProgram_ptr->normalMatrixLoc = glGetUniformLocation(shaderProgram, NORMAL_MATRIX_UNIFORM);
    if (shaderProgram_ptr->normalMatrixLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", NORMAL_MATRIX_UNIFORM, name);
    }
    shaderProgram_ptr->colorLoc = glGetUniformLocation(shaderProgram, COLOR_UNIFORM);
    if (shaderProgram_ptr->colorLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", COLOR_UNIFORM, name);
    }
    shaderProgram_ptr->diffuseColorLoc = glGetUniformLocation(shaderProgram, DIFFUSE_COLOR_UNIFORM);
    if (shaderProgram_ptr->diffuseColorLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", DIFFUSE_COLOR_UNIFORM, name);
    }
    shaderProgram_ptr->lightDirectionLoc = glGetUniformLocation(shaderProgram, LIGHT_DIRECTION_UNIFORM);
    if (shaderProgram_ptr->lightDirectionLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", LIGHT_DIRECTION_UNIFORM, name);
    }
    shaderProgram_ptr->lightColorLoc = glGetUniformLocation(shaderProgram, LIGHT_COLOR_UNIFORM);
    if (shaderProgram_ptr->lightColorLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", LIGHT_COLOR_UNIFORM, name);
    }
    shaderProgram_ptr->viewPosLoc = glGetUniformLocation(shaderProgram, VIEW_POS_UNIFORM);
    if (shaderProgram_ptr->viewPosLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", VIEW_POS_UNIFORM, name);
    }
    shaderProgram_ptr->objectColorLoc = glGetUniformLocation(shaderProgram, OBJECT_COLOR_UNIFORM);
    if (shaderProgram_ptr->objectColorLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", OBJECT_COLOR_UNIFORM, name);
    }
    shaderProgram_ptr->lightPosLoc = glGetUniformLocation(shaderProgram, LIGHT_POS_UNIFORM);
    if (shaderProgram_ptr->lightPosLoc == -1) {
        spdlog::warn("Uniform '{}' not found in shader '{}'", LIGHT_POS_UNIFORM, name);
    }
    
    m_shaderPrograms[name] = std::move(shaderProgram_ptr);
    
    spdlog::info("Shader program '{}' compiled and linked successfully", name);
    return true;
}

void Renderer::updateUniforms()
{
    if (!m_currentShader) {
        return;
    }
    
    // Use raw OpenGL calls to set uniforms with correct names matching the shaders
    GLuint program = m_currentShader->programID;
    glUseProgram(program);
    
    // Set transformation matrices with names that match the shaders
    GLint modelLoc = glGetUniformLocation(program, "model");
    if (modelLoc >= 0) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
    }
    
    GLint viewLoc = glGetUniformLocation(program, "view");
    if (viewLoc >= 0) {
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
    }
    
    GLint projectionLoc = glGetUniformLocation(program, "projection");
    if (projectionLoc >= 0) {
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
    }
    
    // Set lighting uniforms for the fragment shader
    GLint lightPosLoc = glGetUniformLocation(program, "lightPos");
    if (lightPosLoc >= 0) {
        // Set a default light position (above and to the right of origin)
        glm::vec3 lightPos(10.0f, 10.0f, 10.0f);
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    }
    
    GLint lightColorLoc = glGetUniformLocation(program, "lightColor");
    if (lightColorLoc >= 0) {
        // Set a bright white light
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    }
    
    GLint objectColorLoc = glGetUniformLocation(program, "objectColor");
    if (objectColorLoc >= 0) {
        // Set a nice orange color for the cube
        glm::vec3 objectColor(1.0f, 0.5f, 0.2f);
        glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));
    }
    
    GLint viewPosLoc = glGetUniformLocation(program, "viewPos");
    if (viewPosLoc >= 0) {
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(m_viewPosition));
    }
}

std::string Renderer::readShaderFile(const std::string& filePath)
{
    // Try multiple possible paths for the shader files
    std::vector<std::string> possiblePaths = {
        filePath,  // Original path
        std::string("../") + filePath,  // One level up (if running from build dir)
        std::string("../../") + filePath,  // Two levels up
        std::string("d:/dev/rudebase3d/") + filePath  // Absolute path as fallback
    };
    
    for (const auto& path : possiblePaths) {
        std::ifstream file(path);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();
            
            std::string content = buffer.str();
            if (content.empty()) {
                spdlog::warn("Shader file is empty: {}", path);
            } else {
                spdlog::info("Successfully loaded shader file: {} ({} bytes)", path, content.size());
                return content;
            }
        }
    }
    
    spdlog::error("Failed to open shader file at any of the attempted paths for: {}", filePath);
    return "";
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
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec2 TexCoord;\n"
"\n"
"void main()\n"
"{\n"
"    FragPos = vec3(model * vec4(aPos, 1.0));\n"
"    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"    TexCoord = aTexCoord;\n"
"    \n"
"    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";
    return shader;
}

const std::string& Renderer::getDefaultFragmentShader()
{
    static std::string shader = 
"#version 330 core\n"
"\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec2 TexCoord;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"uniform vec3 lightPos;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 objectColor;\n"
"uniform vec3 viewPos;\n"
"\n"
"void main()\n"
"{\n"
"    // Ambient\n"
"    float ambientStrength = 0.1;\n"
"    vec3 ambient = ambientStrength * lightColor;\n"
"    \n"
"    // Diffuse\n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 lightDir = normalize(lightPos - FragPos);\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = diff * lightColor;\n"
"    \n"
"    // Specular\n"
"    float specularStrength = 0.5;\n"
"    vec3 viewDir = normalize(viewPos - FragPos);\n"
"    vec3 reflectDir = reflect(-lightDir, norm);\n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"    vec3 specular = specularStrength * spec * lightColor;\n"
"    \n"
"    vec3 result = (ambient + diffuse + specular) * objectColor;\n"
"    FragColor = vec4(result, 1.0);\n"
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
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\n";
    return shader;
}

const std::string& Renderer::getWireframeFragmentShader()
{
    static std::string shader = 
"#version 330 core\n"
"\n"
"uniform vec3 objectColor;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(objectColor, 1.0);\n"
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
