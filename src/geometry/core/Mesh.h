#pragma once

#include "Common.h"
#include "core/mesh_forward.hpp"
#include <QtOpenGL/QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class Mesh : protected QOpenGLFunctions_3_3_Core {
public:
    Mesh();
    ~Mesh();

    // Mesh data
    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    
    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<unsigned int>& getIndices() const { return m_indices; }
    
    // OpenGL buffer management
    void uploadToGPU();
    void bind();
    void unbind();
    void render();
    void renderWireframe();
    
    // Mesh manipulation
    void calculateNormals();
    void calculateTangents();
    QVector3D getBoundingBoxMin() const;
    QVector3D getBoundingBoxMax() const;
    QVector3D getBoundingBoxCenter() const;
    float getBoundingRadius() const;
    
    // Half-edge mesh interface
    HalfEdgeMesh& getHalfEdgeMesh();
    const HalfEdgeMesh& getHalfEdgeMesh() const;
    
    // Data management
    void setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void setData(const std::vector<glm::vec3>& positions, const std::vector<unsigned int>& indices, 
                 const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords);
    
    // Mesh operations
    bool extrudeFace(const rude::FacePtr& face, float distance = 1.0f);
    bool bevelEdge(const rude::EdgePtr& edge, float amount = 0.1f);
    bool subdivideFace(const rude::FacePtr& face, int subdivisions = 1);
    void updateNormals();
    
    // OpenGL management
    void initializeGL();
    void cleanupGL();
    
    // Utility
    void clear();
    bool isEmpty() const { return m_vertices.empty(); }
    size_t getVertexCount() const { return m_vertices.size(); }
    size_t getTriangleCount() const { return m_indices.size() / 3; }

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    
    // OpenGL objects
    unsigned int m_VAO;  // Vertex Array Object
    unsigned int m_VBO;  // Vertex Buffer Object
    unsigned int m_EBO;  // Element Buffer Object
    
    bool m_uploaded;
    
    // Half-edge mesh representation
    std::unique_ptr<HalfEdgeMesh> m_halfEdgeMesh;
};
