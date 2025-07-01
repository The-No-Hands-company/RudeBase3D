#pragma once

#include "Common.h"
#include <QOpenGLFunctions_3_3_Core>
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
    
    void initializeGL();
    void cleanupGL();
};
