#pragma once

#include "core/mesh_forward.hpp"
#include "Common.h"
// Removed Qt includes
#include <memory>
#include <unordered_map>
#include <string_view>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Renderer {
public:
    Renderer();
    ~Renderer();

    // Initialization
    bool initialize();
    void cleanup();
    
    // Shader management
    bool loadShaders();
    void useShaderProgram(std::string_view name);
    
    // Rendering
    void beginFrame();
    void endFrame();
    void setViewMatrix(const glm::mat4& view);
    void setProjectionMatrix(const glm::mat4& projection);
    void setModelMatrix(const glm::mat4& model);
    
    // Material and lighting
    void setMaterial(MaterialPtr material);
    void setLighting(const glm::vec3& lightDir, const glm::vec4& lightColor);
    void setViewPosition(const glm::vec3& viewPos);
    
    // Render mesh
    void renderMesh(rude::MeshPtr mesh, RenderMode mode = RenderMode::Solid);
    
    // Utility rendering
    void renderLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
    void renderAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color);
    
    // State management
    void enableDepthTest(bool enable);
    void enableBlending(bool enable);
    void setLineWidth(float width);
    void setClearColor(const glm::vec4& color);
    
    // Default shaders
    const std::string& getDefaultVertexShader();
    const std::string& getDefaultFragmentShader();
    const std::string& getWireframeVertexShader();
    const std::string& getWireframeFragmentShader();
    const std::string& getLineVertexShader();
    const std::string& getLineFragmentShader();

private:
    struct ShaderProgram {
        unsigned int programID;
        // Uniform locations
        int mvpMatrixLoc;
        int modelMatrixLoc;
        int viewMatrixLoc;
        int projectionMatrixLoc;
        int normalMatrixLoc;
        int diffuseColorLoc;
        int specularColorLoc;
        int ambientColorLoc;
        int shininessLoc;
        int lightDirectionLoc;
        int lightColorLoc;
        int viewPosLoc;
        int colorLoc;
    };
    
    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> m_shaderPrograms;
    ShaderProgram* m_currentShader;
    
    // Matrices
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_modelMatrix;
    
    // Lighting
    glm::vec3 m_lightDirection;
    glm::vec4 m_lightColor;
    glm::vec3 m_viewPosition;
    
    // Line rendering
    unsigned int m_lineVAO;
    unsigned int m_lineVBO;
    
    // Helper methods
    bool createShaderProgram(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
    void updateUniforms();
    void initializeLineRenderer();
};
