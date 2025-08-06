#pragma once

#include "core/selection_manager.hpp"
#include "core/camera.hpp"
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

namespace rude {

class SelectionRenderer {
public:
    SelectionRenderer();
    ~SelectionRenderer();
    
    // Initialize OpenGL resources
    void initialize(QOpenGLFunctions_3_3_Core* gl);
    
    // Cleanup OpenGL resources
    void cleanup();
    
    // Set references to required objects
    void setSelectionManager(SelectionManager* manager) { m_selectionManager = manager; }
    
    // Render selection highlights
    void renderSelection(const glm::mat4& view, const glm::mat4& projection,
                        QOpenGLFunctions_3_3_Core* gl);
    
    // Configuration
    void setSelectionColor(const glm::vec3& color) { m_selectionColor = color; }
    void setHoverColor(const glm::vec3& color) { m_hoverColor = color; }
    void setLineWidth(float width) { m_lineWidth = width; }
    void setPointSize(float size) { m_pointSize = size; }
    
private:
    SelectionManager* m_selectionManager = nullptr;
    QOpenGLFunctions_3_3_Core* m_gl = nullptr;
    
    // OpenGL resources
    unsigned int m_shaderProgram = 0;
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    
    // Rendering configuration
    glm::vec3 m_selectionColor = glm::vec3(1.0f, 0.5f, 0.0f); // Orange
    glm::vec3 m_hoverColor = glm::vec3(0.0f, 1.0f, 1.0f);     // Cyan
    float m_lineWidth = 3.0f;
    float m_pointSize = 8.0f;
    
    // Shader compilation
    bool compileShaders();
    unsigned int createShader(unsigned int type, const char* source);
    
    // Rendering methods
    void renderSingleSelection(const SelectionData& selection, const glm::mat4& view, 
                             const glm::mat4& projection, QOpenGLFunctions_3_3_Core* gl,
                             int mvpLocation, int colorLocation, const glm::vec3& color);
    void renderVertexSelection(const SelectionData& selection, QOpenGLFunctions_3_3_Core* gl);
    void renderEdgeSelection(const SelectionData& selection, QOpenGLFunctions_3_3_Core* gl);
    void renderFaceSelection(const SelectionData& selection, QOpenGLFunctions_3_3_Core* gl);
};

} // namespace rude
