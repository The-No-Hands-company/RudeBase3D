#include "Mesh.h"
#include <QOpenGLFunctions>
#include <algorithm>

Mesh::Mesh()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_uploaded(false)
{
}

Mesh::~Mesh()
{
    cleanupGL();
}

void Mesh::setVertices(const std::vector<Vertex>& vertices)
{
    m_vertices = vertices;
    m_uploaded = false;
}

void Mesh::setIndices(const std::vector<unsigned int>& indices)
{
    m_indices = indices;
    m_uploaded = false;
}

void Mesh::uploadToGPU()
{
    if (m_vertices.empty()) {
        return;
    }
    
    initializeOpenGLFunctions();
    
    // Generate buffers if not created
    if (m_VAO == 0) {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        if (!m_indices.empty()) {
            glGenBuffers(1, &m_EBO);
        }
    }
    
    // Bind VAO
    glBindVertexArray(m_VAO);
    
    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), 
                    m_vertices.data(), GL_STATIC_DRAW);
    
    // Upload index data
    if (!m_indices.empty() && m_EBO != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), 
                        m_indices.data(), GL_STATIC_DRAW);
    }
    
    // Set vertex attributes
    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                            (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    // Texture coordinate attribute (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                            (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);
    
    // Unbind VAO
    glBindVertexArray(0);
    
    m_uploaded = true;
}

void Mesh::bind()
{
    if (!m_uploaded) {
        uploadToGPU();
    }
    
    glBindVertexArray(m_VAO);
}

void Mesh::unbind()
{
    glBindVertexArray(0);
}

void Mesh::render()
{
    if (m_vertices.empty()) {
        return;
    }
    
    bind();
    
    if (!m_indices.empty()) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
    }
    
    unbind();
}

void Mesh::renderWireframe()
{
    if (m_vertices.empty()) {
        return;
    }
    
    bind();
    
    // Save current polygon mode
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);
    
    // Set wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    if (!m_indices.empty()) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
    }
    
    // Restore polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);
    
    unbind();
}

void Mesh::calculateNormals()
{
    if (m_vertices.empty()) {
        return;
    }
    
    // Initialize all normals to zero
    for (auto& vertex : m_vertices) {
        vertex.normal = QVector3D(0, 0, 0);
    }
    
    // Calculate face normals and accumulate vertex normals
    if (!m_indices.empty()) {
        for (size_t i = 0; i < m_indices.size(); i += 3) {
            if (i + 2 < m_indices.size()) {
                const QVector3D& v0 = m_vertices[m_indices[i]].position;
                const QVector3D& v1 = m_vertices[m_indices[i + 1]].position;
                const QVector3D& v2 = m_vertices[m_indices[i + 2]].position;
                
                QVector3D normal = QVector3D::crossProduct(v1 - v0, v2 - v0).normalized();
                
                m_vertices[m_indices[i]].normal += normal;
                m_vertices[m_indices[i + 1]].normal += normal;
                m_vertices[m_indices[i + 2]].normal += normal;
            }
        }
    } else {
        for (size_t i = 0; i < m_vertices.size(); i += 3) {
            if (i + 2 < m_vertices.size()) {
                const QVector3D& v0 = m_vertices[i].position;
                const QVector3D& v1 = m_vertices[i + 1].position;
                const QVector3D& v2 = m_vertices[i + 2].position;
                
                QVector3D normal = QVector3D::crossProduct(v1 - v0, v2 - v0).normalized();
                
                m_vertices[i].normal += normal;
                m_vertices[i + 1].normal += normal;
                m_vertices[i + 2].normal += normal;
            }
        }
    }
    
    // Normalize all vertex normals
    for (auto& vertex : m_vertices) {
        vertex.normal.normalize();
    }
    
    m_uploaded = false;
}

QVector3D Mesh::getBoundingBoxMin() const
{
    if (m_vertices.empty()) {
        return QVector3D(0, 0, 0);
    }
    
    QVector3D min = m_vertices[0].position;
    for (const auto& vertex : m_vertices) {
        min.setX(std::min(min.x(), vertex.position.x()));
        min.setY(std::min(min.y(), vertex.position.y()));
        min.setZ(std::min(min.z(), vertex.position.z()));
    }
    return min;
}

QVector3D Mesh::getBoundingBoxMax() const
{
    if (m_vertices.empty()) {
        return QVector3D(0, 0, 0);
    }
    
    QVector3D max = m_vertices[0].position;
    for (const auto& vertex : m_vertices) {
        max.setX(std::max(max.x(), vertex.position.x()));
        max.setY(std::max(max.y(), vertex.position.y()));
        max.setZ(std::max(max.z(), vertex.position.z()));
    }
    return max;
}

QVector3D Mesh::getBoundingBoxCenter() const
{
    return (getBoundingBoxMin() + getBoundingBoxMax()) * 0.5f;
}

float Mesh::getBoundingRadius() const
{
    QVector3D center = getBoundingBoxCenter();
    float maxDistSq = 0.0f;
    
    for (const auto& vertex : m_vertices) {
        float distSq = (vertex.position - center).lengthSquared();
        maxDistSq = std::max(maxDistSq, distSq);
    }
    
    return sqrt(maxDistSq);
}

void Mesh::clear()
{
    m_vertices.clear();
    m_indices.clear();
    cleanupGL();
}

void Mesh::initializeGL()
{
    // OpenGL context should be current when this is called
}

void Mesh::cleanupGL()
{
    if (m_VAO != 0 || m_VBO != 0 || m_EBO != 0) {
        QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
        if (f) {
            if (m_VAO != 0) {
                glDeleteVertexArrays(1, &m_VAO);
                m_VAO = 0;
            }
            if (m_VBO != 0) {
                glDeleteBuffers(1, &m_VBO);
                m_VBO = 0;
            }
            if (m_EBO != 0) {
                glDeleteBuffers(1, &m_EBO);
                m_EBO = 0;
            }
        }
    }
    m_uploaded = false;
}
