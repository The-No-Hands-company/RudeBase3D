#include "core/half_edge_mesh.hpp"
#include "core/qt_glm_utils.hpp"
#include <QVector3D>

namespace rude {

HalfEdgeMesh::HalfEdgeMesh() {
}

void HalfEdgeMesh::clear() {
    m_vertices.clear();
    m_edges.clear();
    m_faces.clear();
}

bool HalfEdgeMesh::isEmpty() const {
    return m_vertices.empty() && m_edges.empty() && m_faces.empty();
}

std::vector<std::shared_ptr<Vertex>> HalfEdgeMesh::getVertices() const {
    return m_vertices;
}

std::vector<std::shared_ptr<Edge>> HalfEdgeMesh::getEdges() const {
    return m_edges;
}

std::vector<std::shared_ptr<Face>> HalfEdgeMesh::getFaces() const {
    return m_faces;
}

void HalfEdgeMesh::updateNormals() {
    // Update face normals
    for (auto& face : m_faces) {
        if (face && face->vertices.size() >= 3) {
            QVector3D v1 = rude::glmToQVector(face->vertices[1]->position - face->vertices[0]->position);
            QVector3D v2 = rude::glmToQVector(face->vertices[2]->position - face->vertices[0]->position);
            face->normal = QVector3D::crossProduct(v1, v2).normalized();
        }
    }
    
    // Update vertex normals
    for (auto& vertex : m_vertices) {
        if (vertex) {
            vertex->normal = Vec3(0, 0, 0);
            // TODO: Calculate vertex normal from adjacent faces
        }
    }
}

Vec3 HalfEdgeMesh::computeCentroid() const {
    if (m_vertices.empty()) {
        return Vec3(0, 0, 0);
    }
    
    Vec3 centroid(0, 0, 0);
    for (const auto& vertex : m_vertices) {
        if (vertex) {
            centroid += vertex->position;
        }
    }
    
    return centroid / static_cast<float>(m_vertices.size());
}

bool HalfEdgeMesh::isManifold() const {
    // TODO: Implement manifold check
    return true;
}

} // namespace rude
