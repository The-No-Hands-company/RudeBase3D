#include "SelectionManager.h"
#include "HalfEdgeMesh.h"
#include <QDebug>
#include <algorithm>
#include <cmath>

SelectionManager::SelectionManager()
    : m_selectionType(SelectionType::Vertex)
    , m_boxSelecting(false)
{
}

void SelectionManager::setMesh(HalfEdgeMeshPtr mesh)
{
    // Clear previous selection when changing mesh
    if (m_mesh) {
        clearSelection();
    }
    
    m_mesh = mesh;
}

void SelectionManager::clearSelection()
{
    if (!m_mesh) return;
    
    // Clear selection on all vertices
    for (auto vertex : m_mesh->getVertices()) {
        if (vertex) {
            vertex->setSelected(false);
        }
    }
    
    // Clear selection on all edges
    for (auto edge : m_mesh->getEdges()) {
        if (edge) {
            edge->setSelected(false);
        }
    }
    
    // Clear selection on all faces
    for (auto face : m_mesh->getFaces()) {
        if (face) {
            face->setSelected(false);
        }
    }
    
    // Clear internal selection sets
    m_selectedVertices.clear();
    m_selectedEdges.clear();
    m_selectedFaces.clear();
    
    updateSelectionVisualization();
}

void SelectionManager::selectAll()
{
    if (!m_mesh) return;
    
    switch (m_selectionType) {
        case SelectionType::Vertex:
            for (auto vertex : m_mesh->getVertices()) {
                if (vertex) {
                    selectVertex(vertex, true);
                }
            }
            break;
            
        case SelectionType::Edge:
            for (auto edge : m_mesh->getEdges()) {
                if (edge) {
                    selectEdge(edge, true);
                }
            }
            break;
            
        case SelectionType::Face:
            for (auto face : m_mesh->getFaces()) {
                if (face) {
                    selectFace(face, true);
                }
            }
            break;
            
        default:
            break;
    }
    
    updateSelectionVisualization();
}

void SelectionManager::invertSelection()
{
    if (!m_mesh) return;
    
    switch (m_selectionType) {
        case SelectionType::Vertex:
            for (auto vertex : m_mesh->getVertices()) {
                if (vertex) {
                    selectVertex(vertex, !vertex->isSelected());
                }
            }
            break;
            
        case SelectionType::Edge:
            for (auto edge : m_mesh->getEdges()) {
                if (edge) {
                    selectEdge(edge, !edge->isSelected());
                }
            }
            break;
            
        case SelectionType::Face:
            for (auto face : m_mesh->getFaces()) {
                if (face) {
                    selectFace(face, !face->isSelected());
                }
            }
            break;
            
        default:
            break;
    }
    
    updateSelectionVisualization();
}

bool SelectionManager::selectAtPoint(const QVector3D& worldPos, bool addToSelection)
{
    if (!m_mesh) return false;
    
    bool selectionChanged = false;
    
    if (!addToSelection) {
        clearSelection();
    }
    
    switch (m_selectionType) {
        case SelectionType::Vertex: {
            auto vertex = findClosestVertex(worldPos);
            if (vertex) {
                selectVertex(vertex, true);
                selectionChanged = true;
            }
            break;
        }
        
        case SelectionType::Edge: {
            auto edge = findClosestEdge(worldPos);
            if (edge) {
                selectEdge(edge, true);
                selectionChanged = true;
            }
            break;
        }
        
        case SelectionType::Face: {
            auto face = findClosestFace(worldPos);
            if (face) {
                selectFace(face, true);
                selectionChanged = true;
            }
            break;
        }
        
        default:
            break;
    }
    
    if (selectionChanged) {
        updateSelectionVisualization();
    }
    
    return selectionChanged;
}

void SelectionManager::beginBoxSelection(const QVector3D& startPos)
{
    m_boxSelecting = true;
    m_boxStart = startPos;
    m_boxEnd = startPos;
    m_selectionBox = QRect();
}

void SelectionManager::updateBoxSelection(const QVector3D& currentPos)
{
    if (!m_boxSelecting) return;
    
    m_boxEnd = currentPos;
    
    // Update 2D selection box for UI visualization
    // This would be converted from 3D world coordinates to screen coordinates
    // For now, we'll create a simple bounding box
    float minX = std::min(m_boxStart.x(), m_boxEnd.x());
    float maxX = std::max(m_boxStart.x(), m_boxEnd.x());
    float minY = std::min(m_boxStart.y(), m_boxEnd.y());
    float maxY = std::max(m_boxStart.y(), m_boxEnd.y());
    
    m_selectionBox = QRect(
        static_cast<int>(minX), 
        static_cast<int>(minY),
        static_cast<int>(maxX - minX), 
        static_cast<int>(maxY - minY)
    );
}

void SelectionManager::endBoxSelection(bool addToSelection)
{
    if (!m_boxSelecting || !m_mesh) {
        m_boxSelecting = false;
        return;
    }
    
    if (!addToSelection) {
        clearSelection();
    }
    
    // Define selection volume (simple box for now)
    float minX = std::min(m_boxStart.x(), m_boxEnd.x());
    float maxX = std::max(m_boxStart.x(), m_boxEnd.x());
    float minY = std::min(m_boxStart.y(), m_boxEnd.y());
    float maxY = std::max(m_boxStart.y(), m_boxEnd.y());
    float minZ = std::min(m_boxStart.z(), m_boxEnd.z());
    float maxZ = std::max(m_boxStart.z(), m_boxEnd.z());
    
    switch (m_selectionType) {
        case SelectionType::Vertex:
            for (auto vertex : m_mesh->getVertices()) {
                if (vertex) {
                    const auto& pos = vertex->getPosition();
                    if (pos.x() >= minX && pos.x() <= maxX &&
                        pos.y() >= minY && pos.y() <= maxY &&
                        pos.z() >= minZ && pos.z() <= maxZ) {
                        selectVertex(vertex, true);
                    }
                }
            }
            break;
            
        case SelectionType::Edge:
            for (auto edge : m_mesh->getEdges()) {
                if (edge && edge->getOriginVertex() && edge->getTargetVertex()) {
                    const auto& pos1 = edge->getOriginVertex()->getPosition();
                    const auto& pos2 = edge->getTargetVertex()->getPosition();
                    // Select edge if both endpoints are in box
                    if ((pos1.x() >= minX && pos1.x() <= maxX &&
                         pos1.y() >= minY && pos1.y() <= maxY &&
                         pos1.z() >= minZ && pos1.z() <= maxZ) ||
                        (pos2.x() >= minX && pos2.x() <= maxX &&
                         pos2.y() >= minY && pos2.y() <= maxY &&
                         pos2.z() >= minZ && pos2.z() <= maxZ)) {
                        selectEdge(edge, true);
                    }
                }
            }
            break;
            
        case SelectionType::Face:
            for (auto face : m_mesh->getFaces()) {
                if (face) {
                    // Check if face center is in selection box
                    auto center = face->getCentroid();
                    if (center.x() >= minX && center.x() <= maxX &&
                        center.y() >= minY && center.y() <= maxY &&
                        center.z() >= minZ && center.z() <= maxZ) {
                        selectFace(face, true);
                    }
                }
            }
            break;
            
        default:
            break;
    }
    
    m_boxSelecting = false;
    updateSelectionVisualization();
}

bool SelectionManager::hasSelection() const
{
    return !m_selectedVertices.empty() || !m_selectedEdges.empty() || !m_selectedFaces.empty();
}

size_t SelectionManager::getSelectionCount() const
{
    switch (m_selectionType) {
        case SelectionType::Vertex: return m_selectedVertices.size();
        case SelectionType::Edge: return m_selectedEdges.size();
        case SelectionType::Face: return m_selectedFaces.size();
        default: return 0;
    }
}

std::vector<HalfEdgeVertexPtr> SelectionManager::getSelectedVertices() const
{
    std::vector<HalfEdgeVertexPtr> selected;
    if (!m_mesh) return selected;
    
    for (auto vertex : m_mesh->getVertices()) {
        if (vertex && vertex->isSelected()) {
            selected.push_back(vertex);
        }
    }
    return selected;
}

std::vector<HalfEdgeEdgePtr> SelectionManager::getSelectedEdges() const
{
    std::vector<HalfEdgeEdgePtr> selected;
    if (!m_mesh) return selected;
    
    for (auto edge : m_mesh->getEdges()) {
        if (edge && edge->isSelected()) {
            selected.push_back(edge);
        }
    }
    return selected;
}

std::vector<HalfEdgeFacePtr> SelectionManager::getSelectedFaces() const
{
    std::vector<HalfEdgeFacePtr> selected;
    if (!m_mesh) return selected;
    
    for (auto face : m_mesh->getFaces()) {
        if (face && face->isSelected()) {
            selected.push_back(face);
        }
    }
    return selected;
}

bool SelectionManager::isSelected(HalfEdgeVertexPtr vertex) const
{
    return vertex && vertex->isSelected();
}

bool SelectionManager::isSelected(HalfEdgeEdgePtr edge) const
{
    return edge && edge->isSelected();
}

bool SelectionManager::isSelected(HalfEdgeFacePtr face) const
{
    return face && face->isSelected();
}

void SelectionManager::selectVertex(HalfEdgeVertexPtr vertex, bool selected)
{
    if (!vertex) return;
    
    vertex->setSelected(selected);
    
    if (selected) {
        m_selectedVertices.insert(vertex->getId());
    } else {
        m_selectedVertices.erase(vertex->getId());
    }
}

void SelectionManager::selectEdge(HalfEdgeEdgePtr edge, bool selected)
{
    if (!edge) return;
    
    edge->setSelected(selected);
    
    if (selected) {
        m_selectedEdges.insert(edge->getId());
    } else {
        m_selectedEdges.erase(edge->getId());
    }
}

void SelectionManager::selectFace(HalfEdgeFacePtr face, bool selected)
{
    if (!face) return;
    
    face->setSelected(selected);
    
    if (selected) {
        m_selectedFaces.insert(face->getId());
    } else {
        m_selectedFaces.erase(face->getId());
    }
}

void SelectionManager::convertSelection(SelectionType toType)
{
    if (!m_mesh || toType == m_selectionType) return;
    
    // Store current selection
    auto selectedVertices = getSelectedVertices();
    auto selectedEdges = getSelectedEdges();
    auto selectedFaces = getSelectedFaces();
    
    // Clear current selection
    clearSelection();
    
    // Convert based on source and target types
    if (m_selectionType == SelectionType::Vertex && toType == SelectionType::Face) {
        // Convert vertex selection to adjacent faces
        for (auto vertex : selectedVertices) {
            auto adjacentFaces = vertex->getAdjacentFaces();
            for (auto face : adjacentFaces) {
                selectFace(face, true);
            }
        }
    } else if (m_selectionType == SelectionType::Face && toType == SelectionType::Vertex) {
        // Convert face selection to vertices
        for (auto face : selectedFaces) {
            auto vertices = face->getVertices();
            for (auto vertex : vertices) {
                selectVertex(vertex, true);
            }
        }
    }
    // Add more conversion logic as needed
    
    m_selectionType = toType;
    updateSelectionVisualization();
}

SelectionManager::RayHit SelectionManager::raycast(const QVector3D& rayOrigin, const QVector3D& rayDirection) const
{
    RayHit hit;
    if (!m_mesh) return hit;
    
    float closestDistance = std::numeric_limits<float>::max();
    
    // Test against all faces
    for (auto face : m_mesh->getFaces()) {
        if (!face) continue;
        
        auto vertices = face->getVertices();
        if (vertices.size() < 3) continue;
        
        // For triangulated faces
        for (size_t i = 1; i < vertices.size() - 1; ++i) {
            float t;
            QVector3D hitPoint;
            
            if (rayTriangleIntersect(rayOrigin, rayDirection,
                                   vertices[0]->getPosition(),
                                   vertices[i]->getPosition(),
                                   vertices[i + 1]->getPosition(),
                                   t, hitPoint)) {
                if (t < closestDistance && t > 0) {
                    closestDistance = t;
                    hit.hit = true;
                    hit.point = hitPoint;
                    hit.distance = t;
                    hit.face = face;
                }
            }
        }
    }
    
    return hit;
}

void SelectionManager::updateSelectionVisualization()
{
    // This would trigger a viewport update to show selection highlighting
    // For now, just log the selection count
    qDebug() << "Selection updated:" << getSelectionCount() << "elements selected";
}

HalfEdgeVertexPtr SelectionManager::findClosestVertex(const QVector3D& point, float maxDistance) const
{
    if (!m_mesh) return nullptr;
    
    HalfEdgeVertexPtr closest = nullptr;
    float closestDistSq = maxDistance * maxDistance;
    
    for (auto vertex : m_mesh->getVertices()) {
        if (!vertex) continue;
        
        float distSq = (vertex->getPosition() - point).lengthSquared();
        if (distSq < closestDistSq) {
            closestDistSq = distSq;
            closest = vertex;
        }
    }
    
    return closest;
}

HalfEdgeEdgePtr SelectionManager::findClosestEdge(const QVector3D& point, float maxDistance) const
{
    if (!m_mesh) return nullptr;
    
    HalfEdgeEdgePtr closest = nullptr;
    float closestDistSq = maxDistance * maxDistance;
    
    for (auto edge : m_mesh->getEdges()) {
        if (!edge || !edge->getOriginVertex() || !edge->getTargetVertex()) continue;
        
        // Calculate distance from point to edge (line segment)
        const auto& p1 = edge->getOriginVertex()->getPosition();
        const auto& p2 = edge->getTargetVertex()->getPosition();
        
        QVector3D edgeVec = p2 - p1;
        QVector3D pointVec = point - p1;
        
        float edgeLengthSq = edgeVec.lengthSquared();
        if (edgeLengthSq < EPSILON) continue;
        
        float t = QVector3D::dotProduct(pointVec, edgeVec) / edgeLengthSq;
        t = std::clamp(t, 0.0f, 1.0f);
        
        QVector3D closestOnEdge = p1 + t * edgeVec;
        float distSq = (point - closestOnEdge).lengthSquared();
        
        if (distSq < closestDistSq) {
            closestDistSq = distSq;
            closest = edge;
        }
    }
    
    return closest;
}

HalfEdgeFacePtr SelectionManager::findClosestFace(const QVector3D& point) const
{
    if (!m_mesh) return nullptr;
    
    // Use raycast to find the face under the point
    // For simplicity, cast a ray downward from the point
    QVector3D rayDirection(0, -1, 0);
    auto hit = raycast(point + QVector3D(0, 100, 0), rayDirection);
    
    return hit.hit ? hit.face : nullptr;
}

bool SelectionManager::rayTriangleIntersect(const QVector3D& rayOrigin, const QVector3D& rayDirection,
                                          const QVector3D& v0, const QVector3D& v1, const QVector3D& v2,
                                          float& t, QVector3D& hitPoint) const
{
    // Möller-Trumbore ray-triangle intersection algorithm
    const float RAY_EPSILON = 1e-8f;
    
    QVector3D edge1 = v1 - v0;
    QVector3D edge2 = v2 - v0;
    QVector3D h = QVector3D::crossProduct(rayDirection, edge2);
    float a = QVector3D::dotProduct(edge1, h);
    
    if (a > -RAY_EPSILON && a < RAY_EPSILON) {
        return false; // Ray is parallel to triangle
    }
    
    float f = 1.0f / a;
    QVector3D s = rayOrigin - v0;
    float u = f * QVector3D::dotProduct(s, h);
    
    if (u < 0.0f || u > 1.0f) {
        return false;
    }
    
    QVector3D q = QVector3D::crossProduct(s, edge1);
    float v = f * QVector3D::dotProduct(rayDirection, q);
    
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }
    
    t = f * QVector3D::dotProduct(edge2, q);
    
    if (t > RAY_EPSILON) {
        hitPoint = rayOrigin + rayDirection * t;
        return true;
    }
    
    return false;
}
