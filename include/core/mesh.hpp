#pragma once

#include "Common.h"
#include "core/half_edge_mesh.hpp"
#include "core/mesh_elements.hpp"
#include <QOpenGLFunctions_3_3_Core>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

// Forward declaration
class Entity;

namespace rude {

/**
 * @brief A renderable mesh class
 * 
 * This class represents a renderable mesh with vertex and index data for OpenGL rendering.
 * It's different from HalfEdgeMesh which is used for topology operations.
 */
class Mesh {
public:
    Mesh();
    ~Mesh();

    // Mesh data access
    std::vector<rude::Vertex>& getVertices() { return m_vertices; }
    const std::vector<rude::Vertex>& getVertices() const { return m_vertices; }
    
    std::vector<unsigned int>& getIndices() { return m_indices; }
    const std::vector<unsigned int>& getIndices() const { return m_indices; }
    
    // Mesh operations
    void extrudeFace(int faceIndex, float distance);
    void bevelEdge(int edgeIndex, float width, float depth);
    void subdivideFace(int faceIndex, int divisions);
    void updateNormals();
    
    // Data setting
    void setData(const std::vector<rude::Vertex>& vertices, const std::vector<unsigned int>& indices);
    
    // OpenGL operations
    void uploadToGPU();
    void bind();
    void unbind();
    void render();
    
    // Utility
    glm::vec3 getBoundingBoxMin() const;
    glm::vec3 getBoundingBoxMax() const;
    glm::vec3 getBoundingBoxCenter() const;
    float getBoundingRadius() const;
    
    // Conversion
    std::shared_ptr<HalfEdgeMesh> toHalfEdgeMesh() const;
    static Mesh fromHalfEdgeMesh(const std::shared_ptr<HalfEdgeMesh>& halfEdgeMesh);
    
    // Property queries
    bool isEmpty() const { return m_vertices.empty(); }
    size_t getVertexCount() const { return m_vertices.size(); }
    size_t getTriangleCount() const { return m_indices.size() / 3; }

private:
    std::vector<rude::Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    
    // OpenGL objects
    unsigned int m_VAO;  // Vertex Array Object
    unsigned int m_VBO;  // Vertex Buffer Object
    unsigned int m_EBO;  // Element Buffer Object
    
    bool m_uploaded;
    
    void initializeGL();
    void cleanupGL();
};

} // namespace rude
