#include "Mesh.h"

#include <QOpenGLFunctions>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <algorithm>

#include "core/mesh_elements.hpp"

Mesh::Mesh()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_uploaded(false)
    , m_halfEdgeMesh(std::make_unique<rude::HalfEdgeMesh>())
{
}

Mesh::~Mesh()
{
    cleanupGL();
}

void Mesh::setVertices(const std::vector<rude::VertexPtr>& vertices)
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
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(rude::VertexPtr), 
                    m_vertices.data(), GL_STATIC_DRAW);
    
    // Upload index data
    if (!m_indices.empty() && m_EBO != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), 
                        m_indices.data(), GL_STATIC_DRAW);
    }
    
    // Set vertex attributes
    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(rude::Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(rude::Vertex), 
                            (void*)offsetof(rude::Vertex, normal));
    glEnableVertexAttribArray(1);
    
    // Texture coordinate attribute (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(rude::Vertex), 
                            (void*)offsetof(rude::Vertex, texCoord));
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
        vertex->normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    
    // Calculate face normals and accumulate vertex normals
    if (!m_indices.empty()) {
        for (size_t i = 0; i < m_indices.size(); i += 3) {
            if (i + 2 < m_indices.size()) {
                const glm::vec3& v0 = m_vertices[m_indices[i]]->position;
                const glm::vec3& v1 = m_vertices[m_indices[i + 1]]->position;
                const glm::vec3& v2 = m_vertices[m_indices[i + 2]]->position;
                
                glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
                
                m_vertices[m_indices[i]]->normal += normal;
                m_vertices[m_indices[i + 1]]->normal += normal;
                m_vertices[m_indices[i + 2]]->normal += normal;
            }
        }
    } else {
        for (size_t i = 0; i < m_vertices.size(); i += 3) {
            if (i + 2 < m_vertices.size()) {
                const glm::vec3& v0 = m_vertices[i]->position;
                const glm::vec3& v1 = m_vertices[i + 1]->position;
                const glm::vec3& v2 = m_vertices[i + 2]->position;
                
                glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
                
                m_vertices[i]->normal += normal;
                m_vertices[i + 1]->normal += normal;
                m_vertices[i + 2]->normal += normal;
            }
        }
    }
    
    // Normalize all vertex normals
    for (auto& vertex : m_vertices) {
        vertex->normal = glm::normalize(vertex->normal);
    }
    
    m_uploaded = false;
}

glm::vec3 Mesh::getBoundingBoxMin() const
{
    if (m_vertices.empty()) {
        return glm::vec3(0, 0, 0);
    }
    glm::vec3 min = m_vertices[0]->position;
    for (const auto& vertex : m_vertices) {
        glm::vec3 pos = vertex->position;
        min.x = std::min(min.x, pos.x);
        min.y = std::min(min.y, pos.y);
        min.z = std::min(min.z, pos.z);
    }
    return min;
}

glm::vec3 Mesh::getBoundingBoxMax() const
{
    if (m_vertices.empty()) {
        return glm::vec3(0, 0, 0);
    }
    glm::vec3 max = m_vertices[0]->position;
    for (const auto& vertex : m_vertices) {
        glm::vec3 pos = vertex->position;
        max.x = std::max(max.x, pos.x);
        max.y = std::max(max.y, pos.y);
        max.z = std::max(max.z, pos.z);
    }
    return max;
}

glm::vec3 Mesh::getBoundingBoxCenter() const
{
    return (getBoundingBoxMin() + getBoundingBoxMax()) * 0.5f;
}

float Mesh::getBoundingRadius() const
{
    glm::vec3 center = getBoundingBoxCenter();
    float maxDistSq = 0.0f;
    for (const auto& vertex : m_vertices) {
        glm::vec3 pos = vertex->position;
        float distSq = glm::dot(pos - center, pos - center);
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

// Half-edge mesh interface
rude::HalfEdgeMesh& Mesh::getHalfEdgeMesh()
{
    return *m_halfEdgeMesh;
}

const rude::HalfEdgeMesh& Mesh::getHalfEdgeMesh() const
{
    return *m_halfEdgeMesh;
}

// Data management
void Mesh::setData(const std::vector<rude::VertexPtr>& vertices, const std::vector<unsigned int>& indices)
{
    setVertices(vertices);
    setIndices(indices);
}

void Mesh::setData(const std::vector<glm::vec3>& positions, const std::vector<unsigned int>& indices, 
                   const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords)
{
    // Clear existing vertices
    m_vertices.clear();
    m_vertices.reserve(positions.size());
    
    // Create rude::Vertex objects from GLM data
    for (size_t i = 0; i < positions.size(); ++i) {
        auto vertex = std::make_shared<rude::Vertex>(positions[i]);
        
        if (i < normals.size()) {
            vertex->normal = normals[i];
        } else {
            vertex->normal = glm::vec3(0.0f, 1.0f, 0.0f); // Default up normal
        }
        
        if (i < texCoords.size()) {
            vertex->texCoord = texCoords[i];
        } else {
            vertex->texCoord = glm::vec2(0.0f, 0.0f); // Default UV
        }
        
        m_vertices.push_back(vertex);
    }
    
    setIndices(indices);
}



void Mesh::updateNormals()
{
    calculateNormals();
    
    // Update GPU data if already uploaded
    if (m_uploaded) {
        uploadToGPU();
    }
}

// Mesh operations
bool Mesh::extrudeFace(const rude::FacePtr& face, float distance)
{
    if (!face || !m_halfEdgeMesh) {
        return false;
    }
    
    // TODO: Implement face extrusion using half-edge mesh operations
    // This is a stub implementation
    return true;
}

bool Mesh::bevelEdge(const rude::EdgePtr& edge, float amount)
{
    if (!edge || !m_halfEdgeMesh) {
        return false;
    }
    
    // TODO: Implement edge beveling using half-edge mesh operations
    // This is a stub implementation
    return true;
}

bool Mesh::subdivideFace(const rude::FacePtr& face, int subdivisions)
{
    if (!face || subdivisions <= 0 || !m_halfEdgeMesh) {
        return false;
    }
    
    // TODO: Implement face subdivision using half-edge mesh operations
    // This is a stub implementation
    return true;
}
