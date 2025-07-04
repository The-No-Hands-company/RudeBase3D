#include "core/selection_renderer.hpp"
#include "core/entity.hpp"
#include "mesh.hpp"
#include "core/math/Transform.h"
#include <QOpenGLFunctions_3_3_Core>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace rude {

namespace {
    // Vertex shader for selection rendering
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        
        uniform mat4 uMVP;
        
        void main() {
            gl_Position = uMVP * vec4(aPos, 1.0);
        }
    )";
    
    // Fragment shader for selection rendering
    const char* fragmentShaderSource = R"(
        #version 330 core
        uniform vec3 uColor;
        out vec4 FragColor;
        
        void main() {
            FragColor = vec4(uColor, 1.0);
        }
    )";
}

SelectionRenderer::SelectionRenderer() {
}

SelectionRenderer::~SelectionRenderer() {
    cleanup();
}

void SelectionRenderer::initialize(QOpenGLFunctions_3_3_Core* gl) {
    m_gl = gl;
    
    // Compile shaders
    if (!compileShaders()) {
        return;
    }
    
    // Generate VAO and VBO
    m_gl->glGenVertexArrays(1, &m_VAO);
    m_gl->glGenBuffers(1, &m_VBO);
    
    // Setup VAO
    m_gl->glBindVertexArray(m_VAO);
    m_gl->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    // Configure vertex attributes
    m_gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_gl->glEnableVertexAttribArray(0);
    
    m_gl->glBindVertexArray(0);
}

void SelectionRenderer::cleanup() {
    if (m_gl) {
        if (m_VAO) {
            m_gl->glDeleteVertexArrays(1, &m_VAO);
            m_VAO = 0;
        }
        if (m_VBO) {
            m_gl->glDeleteBuffers(1, &m_VBO);
            m_VBO = 0;
        }
        if (m_shaderProgram) {
            m_gl->glDeleteProgram(m_shaderProgram);
            m_shaderProgram = 0;
        }
    }
}

void SelectionRenderer::renderSelection(const glm::mat4& view, const glm::mat4& projection,
                                       QOpenGLFunctions_3_3_Core* gl) {
    if (!m_selectionManager || !gl || !m_shaderProgram) return;
    
    // Save current OpenGL state
    GLint oldLineWidth;
    GLfloat oldPointSize;
    gl->glGetIntegerv(GL_LINE_WIDTH, &oldLineWidth);
    gl->glGetFloatv(GL_POINT_SIZE, &oldPointSize);
    
    // Enable blending for better visibility
    gl->glEnable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Use selection shader
    gl->glUseProgram(m_shaderProgram);
    gl->glBindVertexArray(m_VAO);
    
    // Get MVP matrix uniform location
    int mvpLocation = gl->glGetUniformLocation(m_shaderProgram, "uMVP");
    int colorLocation = gl->glGetUniformLocation(m_shaderProgram, "uColor");
    
    // Render all current selections
    const auto& currentSelection = m_selectionManager->getSelection();
    for (const auto& selection : currentSelection) {
        if (selection.isValid()) {
            renderSingleSelection(selection, view, projection, gl, mvpLocation, colorLocation, m_selectionColor);
        }
    }
    
    // For now, no hover support - can be added later
    // const auto& hoveredSelection = m_selectionManager->getHoveredSelection();
    // if (hoveredSelection.isValid()) {
    //     renderSingleSelection(hoveredSelection, view, projection, gl, mvpLocation, colorLocation, m_hoverColor);
    // }
    
    // Restore OpenGL state
    gl->glLineWidth(oldLineWidth);
    gl->glPointSize(oldPointSize);
    gl->glDisable(GL_BLEND);
    gl->glBindVertexArray(0);
    gl->glUseProgram(0);
}

void SelectionRenderer::renderSingleSelection(const SelectionData& selection, 
                                            const glm::mat4& view, const glm::mat4& projection,
                                            QOpenGLFunctions_3_3_Core* gl,
                                            int mvpLocation, int colorLocation, 
                                            const glm::vec3& color) {
    if (!selection.entity) return;
    
    auto mesh = selection.entity->getMesh();
    if (!mesh) return;
    
    Transform& transform = selection.entity->getTransform();
    glm::mat4 model = transform.getModelMatrix();
    glm::mat4 mvp = projection * view * model;
    
    // Set uniforms
    gl->glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    gl->glUniform3fv(colorLocation, 1, glm::value_ptr(color));
    
    switch (selection.type) {
        case ComponentType::Vertex:
            renderVertexSelection(selection, gl);
            break;
        case ComponentType::Edge:
            renderEdgeSelection(selection, gl);
            break;
        case ComponentType::Face:
            renderFaceSelection(selection, gl);
            break;
        default:
            break;
    }
}

void SelectionRenderer::renderVertexSelection(const SelectionData& selection, QOpenGLFunctions_3_3_Core* gl) {
    if (!selection.vertex) return;
    
    // Set point size
    gl->glPointSize(m_pointSize);
    
    // Upload vertex position
    glm::vec3 pos = selection.vertex->position;
    gl->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &pos, GL_DYNAMIC_DRAW);
    
    // Draw point
    gl->glDrawArrays(GL_POINTS, 0, 1);
}

void SelectionRenderer::renderEdgeSelection(const SelectionData& selection, QOpenGLFunctions_3_3_Core* gl) {
    if (!selection.edge) return;
    
    // Set line width
    gl->glLineWidth(m_lineWidth);
    
    // Get edge half-edges
    auto halfEdge1 = selection.edge->getHalfEdge();
    if (!halfEdge1 || !halfEdge1->getTwin()) return;
    
    auto v1 = halfEdge1->getVertex();
    auto v2 = halfEdge1->getTwin()->getVertex();
    
    if (!v1 || !v2) return;
    
    // Upload edge positions
    glm::vec3 positions[2] = { v1->position, v2->position };
    gl->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    gl->glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), positions, GL_DYNAMIC_DRAW);
    
    // Draw line
    gl->glDrawArrays(GL_LINES, 0, 2);
}

void SelectionRenderer::renderFaceSelection(const SelectionData& selection, QOpenGLFunctions_3_3_Core* gl) {
    if (!selection.face || !selection.face->getHalfEdge()) return;
    
    // Set line width for face outline
    gl->glLineWidth(m_lineWidth);
    
    // Collect face vertices
    std::vector<glm::vec3> vertices;
    auto halfEdge = selection.face->getHalfEdge();
    auto current = halfEdge;
    
    do {
        if (current->getVertex()) {
            vertices.push_back(current->getVertex()->position);
        }
        current = current->getNext();
    } while (current && current != halfEdge);
    
    if (vertices.size() < 3) return;
    
    // Upload vertices
    gl->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    gl->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), 
                     vertices.data(), GL_DYNAMIC_DRAW);
    
    // Draw face outline as line loop
    gl->glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
}

bool SelectionRenderer::compileShaders() {
    // Create vertex shader
    unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    if (!vertexShader) return false;
    
    // Create fragment shader
    unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (!fragmentShader) {
        m_gl->glDeleteShader(vertexShader);
        return false;
    }
    
    // Create program
    m_shaderProgram = m_gl->glCreateProgram();
    m_gl->glAttachShader(m_shaderProgram, vertexShader);
    m_gl->glAttachShader(m_shaderProgram, fragmentShader);
    m_gl->glLinkProgram(m_shaderProgram);
    
    // Check linking
    int success;
    m_gl->glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        m_gl->glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        // Log error
        m_gl->glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
    
    // Clean up shaders
    m_gl->glDeleteShader(vertexShader);
    m_gl->glDeleteShader(fragmentShader);
    
    return success;
}

unsigned int SelectionRenderer::createShader(unsigned int type, const char* source) {
    unsigned int shader = m_gl->glCreateShader(type);
    m_gl->glShaderSource(shader, 1, &source, nullptr);
    m_gl->glCompileShader(shader);
    
    // Check compilation
    int success;
    m_gl->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        m_gl->glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        // Log error
        m_gl->glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

} // namespace rude
