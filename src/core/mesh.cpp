#include "core/mesh.hpp"
#include "core/qt_glm_utils.hpp"
#include "geometry/halfedge/HalfEdgeMesh.h"
#include "Common.h"
#include <QDebug>
#include <algorithm>
#include <glm/glm.hpp>
#include <cmath>
#include "core/mesh_elements.hpp"

namespace rude {

Mesh::Mesh() 
    : m_VAO(0), m_VBO(0), m_EBO(0), m_uploaded(false)
{
}

Mesh::~Mesh() 
{
    cleanupGL();
}

void Mesh::extrudeFace(int faceIndex, float distance)
{
    if (faceIndex < 0 || faceIndex >= static_cast<int>(m_indices.size() / 3)) {
        qWarning() << "Invalid face index for extrusion:" << faceIndex;
        return;
    }

    // This is a stub implementation - in a real application, this would:
    // 1. Convert the mesh to a half-edge mesh
    // 2. Perform the extrusion operation using HalfEdgeUtils::extrudeFaces
    // 3. Convert back to a regular mesh
    
    qDebug() << "Extruding face" << faceIndex << "with distance" << distance;
    
    // Get face normal
    unsigned int i0 = m_indices[faceIndex * 3];
    unsigned int i1 = m_indices[faceIndex * 3 + 1];
    unsigned int i2 = m_indices[faceIndex * 3 + 2];
    
    glm::vec3 v0 = m_vertices[i0]->position;
    glm::vec3 v1 = m_vertices[i1]->position;
    glm::vec3 v2 = m_vertices[i2]->position;
    
    glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);
    
    // For now, just move the vertices along the normal
    m_vertices[i0]->position += normal * distance;
    m_vertices[i1]->position += normal * distance;
    m_vertices[i2]->position += normal * distance;
    
    updateNormals();
    m_uploaded = false;
}

void Mesh::bevelEdge(int edgeIndex, float width, float depth)
{
    if (edgeIndex < 0 || edgeIndex >= static_cast<int>(m_indices.size())) {
        qWarning() << "Invalid edge index for beveling:" << edgeIndex;
        return;
    }
    
    qDebug() << "Beveling edge" << edgeIndex << "with width" << width << "and depth" << depth;
    
    // Stub implementation: In a full implementation, you would:
    // 1. Convert to half-edge mesh
    // 2. Find the edge to bevel
    // 3. Apply beveling using HalfEdgeUtils::bevelEdge
    // 4. Convert back to a regular mesh
    
    // For now, we'll just log it and avoid changing the mesh
    qDebug() << "Bevel edge operation not fully implemented yet";
    
    m_uploaded = false;
}

void Mesh::subdivideFace(int faceIndex, int divisions)
{
    if (faceIndex < 0 || faceIndex >= static_cast<int>(m_indices.size() / 3)) {
        qWarning() << "Invalid face index for subdivision:" << faceIndex;
        return;
    }
    
    qDebug() << "Subdividing face" << faceIndex << "with" << divisions << "divisions";
    
    // Stub implementation: In a full implementation, you would:
    // 1. Convert to half-edge mesh
    // 2. Find the face to subdivide
    // 3. Apply subdivision using a subdivision algorithm
    // 4. Convert back to a regular mesh
    
    // For now, we'll just log it and avoid changing the mesh
    qDebug() << "Subdivide face operation not fully implemented yet";
    
    m_uploaded = false;
}

void Mesh::updateNormals()
{
    // Reset all normals to zero
    for (auto& vertex : m_vertices) {
        vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    
    // Calculate face normals and accumulate on vertices
    for (size_t i = 0; i < m_indices.size(); i += 3) {
        if (i + 2 >= m_indices.size()) continue;
        
        unsigned int i0 = m_indices[i];
        unsigned int i1 = m_indices[i + 1];
        unsigned int i2 = m_indices[i + 2];
        
        if (i0 >= m_vertices.size() || i1 >= m_vertices.size() || i2 >= m_vertices.size()) {
            continue;
        }
        
        glm::vec3 v0 = m_vertices[i0].position;
        glm::vec3 v1 = m_vertices[i1].position;
        glm::vec3 v2 = m_vertices[i2].position;
        
        glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);
        
        // Add to each vertex normal (will be normalized later)
        m_vertices[i0].normal += normal;
        m_vertices[i1].normal += normal;
        m_vertices[i2].normal += normal;
    }
    
    // Normalize all vertex normals
    for (auto& vertex : m_vertices) {
        float lengthSq = glm::dot(vertex.normal, vertex.normal);
        if (lengthSq > 0.0001f) {
            vertex.normal = glm::normalize(vertex.normal);
        } else {
            // If normal is too small, set to default up vector
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        }
    }
    
    m_uploaded = false;
}

void Mesh::setData(const std::vector<::Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    m_vertices = vertices;
    m_indices = indices;
    
    if (vertices.empty()) {
        qWarning() << "Setting empty vertex data";
    }
    
    if (indices.empty()) {
        qWarning() << "Setting empty index data";
    }
    
    // Calculate normals if they're not provided
    bool hasNormals = false;
    for (const auto& vertex : vertices) {
        float lengthSq = glm::dot(vertex.normal, vertex.normal);
        if (lengthSq > 0.0001f) {
            hasNormals = true;
            break;
        }
    }
    
    if (!hasNormals && !vertices.empty()) {
        updateNormals();
    }
    
    m_uploaded = false;
}

void Mesh::uploadToGPU()
{
    // This would be implemented to upload data to GPU
    // Implementation would depend on the OpenGL context
    m_uploaded = true;
}

void Mesh::bind()
{
    if (!m_uploaded) {
        uploadToGPU();
    }
    // Bind the VAO
}

void Mesh::unbind()
{
    // Unbind the VAO
}

void Mesh::render()
{
    if (!m_uploaded) {
        uploadToGPU();
    }
    // Render the mesh
}

glm::vec3 Mesh::getBoundingBoxMin() const
{
    if (m_vertices.empty()) {
        return glm::vec3(0, 0, 0);
    }
    
    glm::vec3 min = m_vertices[0].position;
    for (const auto& vertex : m_vertices) {
        min.x = std::min(min.x, vertex.position.x);
        min.y = std::min(min.y, vertex.position.y);
        min.z = std::min(min.z, vertex.position.z);
    }
    
    return min;
}

glm::vec3 Mesh::getBoundingBoxMax() const
{
    if (m_vertices.empty()) {
        return glm::vec3(0, 0, 0);
    }
    
    glm::vec3 max = m_vertices[0].position;
    for (const auto& vertex : m_vertices) {
        max.x = std::max(max.x, vertex.position.x);
        max.y = std::max(max.y, vertex.position.y);
        max.z = std::max(max.z, vertex.position.z);
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
        float distSq = glm::dot(vertex.position - center, vertex.position - center);
        maxDistSq = std::max(maxDistSq, distSq);
    }
    
    return std::sqrt(maxDistSq);
}

std::shared_ptr<HalfEdgeMesh> Mesh::toHalfEdgeMesh() const
{
    // Stub implementation - this would convert a simple mesh to a half-edge mesh
    return std::make_shared<HalfEdgeMesh>();
}

Mesh Mesh::fromHalfEdgeMesh(const std::shared_ptr<HalfEdgeMesh>& halfEdgeMesh)
{
    // Stub implementation - this would convert a half-edge mesh to a simple mesh
    Mesh mesh;
    // Conversion code would go here
    return mesh;
}

void Mesh::initializeGL()
{
    // This would initialize OpenGL objects (VAO, VBO, EBO)
}

void Mesh::cleanupGL()
{
    // This would clean up OpenGL objects
    m_uploaded = false;
}

} // namespace rude
