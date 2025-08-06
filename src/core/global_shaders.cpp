#include "core/global_shaders.hpp"
#include <QOpenGLContext>
#include <spdlog/spdlog.h>
#include <mutex>

namespace {
    // Mesh vertex shader
    const char* meshVertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        out vec3 FragPos;
        out vec3 Normal;

        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";

    // Mesh fragment shader
    const char* meshFragmentShader = R"(
        #version 330 core
        struct Material {
            vec3 diffuse;
            vec3 specular;
            float shininess;
        };

        in vec3 FragPos;
        in vec3 Normal;

        uniform Material material;

        out vec4 FragColor;

        void main() {
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // Simple directional light
            
            // Ambient
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * material.diffuse;
            
            // Diffuse
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * material.diffuse;
            
            // Specular
            vec3 viewDir = normalize(-FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            vec3 specular = material.specular * spec;
            
            vec3 result = ambient + diffuse + specular;
            FragColor = vec4(result, 1.0);
        }
    )";
    
    static std::mutex s_singletonMutex;
}

GlobalShaders& GlobalShaders::getInstance() {
    std::lock_guard<std::mutex> lock(s_singletonMutex);
    static GlobalShaders instance;
    return instance;
}

GlobalShaders::GlobalShaders() : meshShaderProgram(0), initialized(false) {}

GlobalShaders::~GlobalShaders() {
    // Only cleanup if we have a valid OpenGL context
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (context && context->isValid() && meshShaderProgram != 0) {
        if (this->initializeOpenGLFunctions()) {
            this->glDeleteProgram(meshShaderProgram);
        }
    }
    meshShaderProgram = 0;
    initialized = false;
}

GLuint GlobalShaders::getMeshShader() const {
    std::lock_guard<std::mutex> lock(s_singletonMutex);
    
    // Verify the shader program is still valid
    if (meshShaderProgram == 0) {
        spdlog::warn("[GlobalShaders::getMeshShader] Shader program is 0 (not initialized)");
        return 0;
    }
    if (!initialized) {
        spdlog::warn("[GlobalShaders::getMeshShader] GlobalShaders not initialized");
        return 0;
    }
    // Check if we have a valid OpenGL context
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context || !context->isValid()) {
        spdlog::warn("[GlobalShaders::getMeshShader] No valid OpenGL context");
        return 0;
    }
    // Verify the program is still valid in OpenGL
    if (const_cast<GlobalShaders*>(this)->initializeOpenGLFunctions()) {
        if (!const_cast<GlobalShaders*>(this)->glIsProgram(meshShaderProgram)) {
            spdlog::warn("[GlobalShaders::getMeshShader] Shader program {} is no longer valid in OpenGL", meshShaderProgram);
            return 0;
        }
    }
    
    return meshShaderProgram;
}

bool GlobalShaders::initialize() {
    std::lock_guard<std::mutex> lock(s_singletonMutex);
    
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context) {
        spdlog::warn("[GlobalShaders::initialize] No OpenGL context available for shader initialization");
        return false;
    }
    if (!context->isValid()) {
        spdlog::warn("[GlobalShaders::initialize] OpenGL context is not valid");
        return false;
    }
    if (!this->initializeOpenGLFunctions()) {
        spdlog::warn("[GlobalShaders::initialize] Failed to initialize OpenGL functions");
        return false;
    }

    // Clean up existing program if any
    if (meshShaderProgram != 0) {
        spdlog::info("[GlobalShaders::initialize] Cleaning up existing shader program {}", meshShaderProgram);
        this->glDeleteProgram(meshShaderProgram);
        meshShaderProgram = 0;
    }

    // Create vertex shader
    GLuint vertexShader = this->glCreateShader(GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        spdlog::warn("[GlobalShaders::initialize] Failed to create vertex shader");
        return false;
    }
    
    this->glShaderSource(vertexShader, 1, &meshVertexShader, nullptr);
    this->glCompileShader(vertexShader);

    // Check vertex shader compilation
    GLint success = 0;
    GLchar infoLog[512];
    this->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        this->glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        spdlog::warn("[GlobalShaders::initialize] Vertex shader compilation failed: {}", infoLog);
        this->glDeleteShader(vertexShader);
        return false;
    }

    // Create fragment shader
    GLuint fragmentShader = this->glCreateShader(GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        spdlog::warn("[GlobalShaders::initialize] Failed to create fragment shader");
        this->glDeleteShader(vertexShader);
        return false;
    }
    
    this->glShaderSource(fragmentShader, 1, &meshFragmentShader, nullptr);
    this->glCompileShader(fragmentShader);

    // Check fragment shader compilation
    this->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        this->glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        spdlog::warn("[GlobalShaders::initialize] Fragment shader compilation failed: {}", infoLog);
        this->glDeleteShader(vertexShader);
        this->glDeleteShader(fragmentShader);
        return false;
    }

    // Create shader program
    meshShaderProgram = this->glCreateProgram();
    if (meshShaderProgram == 0) {
        spdlog::warn("[GlobalShaders::initialize] Failed to create shader program");
        this->glDeleteShader(vertexShader);
        this->glDeleteShader(fragmentShader);
        return false;
    }
    
    this->glAttachShader(meshShaderProgram, vertexShader);
    this->glAttachShader(meshShaderProgram, fragmentShader);
    this->glLinkProgram(meshShaderProgram);

    // Check for linking errors
    this->glGetProgramiv(meshShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        this->glGetProgramInfoLog(meshShaderProgram, 512, nullptr, infoLog);
        spdlog::warn("[GlobalShaders::initialize] Shader program linking failed: {}", infoLog);
        this->glDeleteShader(vertexShader);
        this->glDeleteShader(fragmentShader);
        this->glDeleteProgram(meshShaderProgram);
        meshShaderProgram = 0;
        return false;
    }

    // Clean up shaders after linking
    this->glDeleteShader(vertexShader);
    this->glDeleteShader(fragmentShader);

    // Verify the program is valid
    if (!this->glIsProgram(meshShaderProgram)) {
        spdlog::warn("[GlobalShaders::initialize] Created program is not valid");
        this->glDeleteProgram(meshShaderProgram);
        meshShaderProgram = 0;
        return false;
    }

    initialized = true;
    spdlog::info("[GlobalShaders::initialize] Mesh shader program initialized successfully with ID: {}", meshShaderProgram);
    return true;
}