#pragma once

#include "Common.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <memory>
#include <unordered_map>
#include <string_view>

class Renderer : protected QOpenGLFunctions_3_3_Core {
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
    
    void setViewMatrix(const QMatrix4x4& view);
    void setProjectionMatrix(const QMatrix4x4& projection);
    void setModelMatrix(const QMatrix4x4& model);
    
    // Material and lighting
    void setMaterial(MaterialPtr material);
    void setLighting(const QVector3D& lightDir, const QVector4D& lightColor);
    void setViewPosition(const QVector3D& viewPos);
    
    // Render mesh
    void renderMesh(MeshPtr mesh, RenderMode mode = RenderMode::Solid);
    
    // Utility rendering
    void renderLine(const QVector3D& start, const QVector3D& end, const QVector4D& color);
    void renderAABB(const QVector3D& min, const QVector3D& max, const QVector4D& color);
    
    // State management
    void enableDepthTest(bool enable);
    void enableBlending(bool enable);
    void setLineWidth(float width);
    void setClearColor(const QVector4D& color);

private:
    struct ShaderProgram {
        std::unique_ptr<QOpenGLShaderProgram> program;
        
        // Common uniforms
        int mvpMatrixLoc;
        int modelMatrixLoc;
        int viewMatrixLoc;
        int projectionMatrixLoc;
        int normalMatrixLoc;
        
        // Material uniforms
        int diffuseColorLoc;
        int specularColorLoc;
        int ambientColorLoc;
        int shininessLoc;
        
        // Lighting uniforms
        int lightDirectionLoc;
        int lightColorLoc;
        int viewPosLoc;
        
        // Other uniforms
        int colorLoc;
    };
    
    std::unordered_map<QString, std::unique_ptr<ShaderProgram>> m_shaderPrograms;
    ShaderProgram* m_currentShader;
    
    // Matrices
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelMatrix;
    
    // Lighting
    QVector3D m_lightDirection;
    QVector4D m_lightColor;
    QVector3D m_viewPosition;
    
    // Line rendering
    unsigned int m_lineVAO;
    unsigned int m_lineVBO;
    
    // Helper methods
    bool createShaderProgram(const QString& name, const QString& vertexSource, const QString& fragmentSource);
    void updateUniforms();
    void initializeLineRenderer();
    
    // Default shaders
    static const QString& getDefaultVertexShader();
    static const QString& getDefaultFragmentShader();
    static const QString& getWireframeVertexShader();
    static const QString& getWireframeFragmentShader();
    static const QString& getLineVertexShader();
    static const QString& getLineFragmentShader();
};
