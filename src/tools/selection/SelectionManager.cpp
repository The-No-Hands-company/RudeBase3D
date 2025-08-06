
#include "SelectionManager.h"

#include "core/mesh_elements.hpp"
#include "core/half_edge_mesh.hpp"
#include <algorithm>
#include <cmath>
#include <spdlog/spdlog.h>

SelectionManager::SelectionManager()
    : m_selectionType(SelectionType::Vertex)
    , m_boxSelecting(false)
{
}

void SelectionManager::clearSelection()
{
    if (!m_mesh) return;
    
    // Clear selection on all vertices
    // Note: rude:: mesh elements don't have setSelected() method, selection is managed externally
    // TODO: Implement external selection tracking if needed
    
    // Clear internal selection sets
    m_selectedVertices.clear();
    m_selectedEdges.clear(); 
    m_selectedFaces.clear();
    
    spdlog::debug("Selection updated: {} elements selected", getSelectionCount());
    
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
                    // Check if vertex is currently selected
                    bool isCurrentlySelected = (m_selectedVertices.find(vertex) != m_selectedVertices.end());
                    selectVertex(vertex, !isCurrentlySelected);
                }
            }
            break;
            
        case SelectionType::Edge:
            for (auto edge : m_mesh->getEdges()) {
                if (edge) {
                    // Check if edge is currently selected
                    bool isCurrentlySelected = (m_selectedEdges.find(edge) != m_selectedEdges.end());
                    selectEdge(edge, !isCurrentlySelected);
                }
            }
            break;
            
        case SelectionType::Face:
            for (auto face : m_mesh->getFaces()) {
                if (face) {
                    // Check if face is currently selected
                    bool isCurrentlySelected = (m_selectedFaces.find(face) != m_selectedFaces.end());
                    selectFace(face, !isCurrentlySelected);
                }
            }
            break;
            
        default:
            break;
    }
    
    updateSelectionVisualization();
}

bool SelectionManager::selectAtPoint(const glm::vec3& worldPos, bool addToSelection)
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

void SelectionManager::beginBoxSelection(const glm::vec3& startPos)
{
    m_boxSelecting = true;
    m_boxStart = startPos;
    m_boxEnd = startPos;
    m_selectionBox = { glm::vec2(startPos.x, startPos.y), glm::vec2(startPos.x, startPos.y) };
}

void SelectionManager::updateBoxSelection(const glm::vec3& currentPos)
{
    if (!m_boxSelecting) return;
    m_boxEnd = currentPos;
    float minX = std::min(m_boxStart.x, m_boxEnd.x);
    float maxX = std::max(m_boxStart.x, m_boxEnd.x);
    float minY = std::min(m_boxStart.y, m_boxEnd.y);
    float maxY = std::max(m_boxStart.y, m_boxEnd.y);
    m_selectionBox = { glm::vec2(minX, minY), glm::vec2(maxX, maxY) };
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
    float minX = std::min(m_boxStart.x, m_boxEnd.x);
    float maxX = std::max(m_boxStart.x, m_boxEnd.x);
    float minY = std::min(m_boxStart.y, m_boxEnd.y);
    float maxY = std::max(m_boxStart.y, m_boxEnd.y);
    float minZ = std::min(m_boxStart.z, m_boxEnd.z);
    float maxZ = std::max(m_boxStart.z, m_boxEnd.z);
    
    switch (m_selectionType) {
        case SelectionType::Vertex:
            for (auto vertex : m_mesh->getVertices()) {
                if (vertex) {
                    // Convert glm::vec3 to compare with QVector3D bounds
                    auto pos_glm = vertex->position;
                    if (pos_glm.x >= minX && pos_glm.x <= maxX &&
                        pos_glm.y >= minY && pos_glm.y <= maxY &&
                        pos_glm.z >= minZ && pos_glm.z <= maxZ) {
                        selectVertex(vertex, true);
                    }
                }
            }
            break;
            
        case SelectionType::Edge:
            for (auto edge : m_mesh->getEdges()) {
                if (edge && edge->halfEdge && edge->halfEdge->vertex) {
                    auto halfEdge1 = edge->halfEdge;
                    auto halfEdge2 = edge->halfEdge->twin;
                    if (halfEdge1 && halfEdge2 && halfEdge1->vertex && halfEdge2->vertex) {
                        auto pos1 = halfEdge1->vertex->position;
                        auto pos2 = halfEdge2->vertex->position;
                        if ((pos1.x >= minX && pos1.x <= maxX &&
                             pos1.y >= minY && pos1.y <= maxY &&
                             pos1.z >= minZ && pos1.z <= maxZ) ||
                            (pos2.x >= minX && pos2.x <= maxX &&
                             pos2.y >= minY && pos2.y <= maxY &&
                             pos2.z >= minZ && pos2.z <= maxZ)) {
                            selectEdge(edge, true);
                        }
                    }
                }
            }
            break;
            
        case SelectionType::Face:
            for (auto face : m_mesh->getFaces()) {
                if (face) {
                    // Calculate face center manually
                    auto vertices = face->getVertices();
                    if (!vertices.empty()) {
                        glm::vec3 center(0.0f);
                        for (auto vertex : vertices) {
                            if (vertex) {
                                center += vertex->position;
                            }
                        }
                        center /= static_cast<float>(vertices.size());
                        
                        // Check if face center is in selection box
                        if (center.x >= minX && center.x <= maxX &&
                            center.y >= minY && center.y <= maxY &&
                            center.z >= minZ && center.z <= maxZ) {
                            selectFace(face, true);
                        }
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
    
    // Return vertices from our internal selection tracking
    for (auto vertex : m_selectedVertices) {
        if (vertex) {
            selected.push_back(vertex);
        }
    }
    return selected;
}

std::vector<HalfEdgeEdgePtr> SelectionManager::getSelectedEdges() const
{
    std::vector<HalfEdgeEdgePtr> selected;
    if (!m_mesh) return selected;
    
    // Return edges from our internal selection tracking
    for (auto edge : m_selectedEdges) {
        if (edge) {
            selected.push_back(edge);
        }
    }
    return selected;
}

std::vector<HalfEdgeFacePtr> SelectionManager::getSelectedFaces() const
{
    std::vector<HalfEdgeFacePtr> selected;
    if (!m_mesh) return selected;
    
    // Return faces from our internal selection tracking
    for (auto face : m_selectedFaces) {
        if (face) {
            selected.push_back(face);
        }
    }
    return selected;
}

bool SelectionManager::isSelected(HalfEdgeVertexPtr vertex) const
{
    return vertex && (m_selectedVertices.find(vertex) != m_selectedVertices.end());
}

bool SelectionManager::isSelected(HalfEdgeEdgePtr edge) const
{
    return edge && (m_selectedEdges.find(edge) != m_selectedEdges.end());
}

bool SelectionManager::isSelected(HalfEdgeFacePtr face) const
{
    return face && (m_selectedFaces.find(face) != m_selectedFaces.end());
}

void SelectionManager::selectVertex(HalfEdgeVertexPtr vertex, bool selected)
{
    if (!vertex) return;
    
    // Note: rude:: mesh elements don't have setSelected() method
    // vertex->setSelected(selected);
    
    if (selected) {
        m_selectedVertices.insert(vertex);
    } else {
        m_selectedVertices.erase(vertex);
    }
}

void SelectionManager::selectEdge(HalfEdgeEdgePtr edge, bool selected)
{
    if (!edge) return;
    
    // Note: rude:: mesh elements don't have setSelected() method
    // edge->setSelected(selected);
    
    if (selected) {
        m_selectedEdges.insert(edge);
    } else {
        m_selectedEdges.erase(edge);
    }
}

void SelectionManager::selectFace(HalfEdgeFacePtr face, bool selected)
{
    if (!face) return;
    
    // Note: rude:: mesh elements don't have setSelected() method
    // face->setSelected(selected);
    
    if (selected) {
        m_selectedFaces.insert(face);
    } else {
        m_selectedFaces.erase(face);
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

SelectionManager::RayHit SelectionManager::raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const
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
            glm::vec3 hitPoint;
            
            // Use glm::vec3 directly
            auto pos0 = vertices[0]->position;
            auto pos1 = vertices[i]->position;
            auto pos2 = vertices[i + 1]->position;
            
            if (rayTriangleIntersect(rayOrigin, rayDirection, pos0, pos1, pos2, t, hitPoint)) {
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
    spdlog::debug("Selection updated: {} elements selected", getSelectionCount());
}

HalfEdgeVertexPtr SelectionManager::findClosestVertex(const glm::vec3& point, float maxDistance) const
{
    if (!m_mesh) return nullptr;
    HalfEdgeVertexPtr closest = nullptr;
    float closestDistSq = maxDistance * maxDistance;
    for (auto vertex : m_mesh->getVertices()) {
        if (!vertex) continue;
        auto pos = vertex->position;
        float distSq = glm::dot(pos - point, pos - point);
        if (distSq < closestDistSq) {
            closestDistSq = distSq;
            closest = vertex;
        }
    }
    return closest;
}

HalfEdgeEdgePtr SelectionManager::findClosestEdge(const glm::vec3& point, float maxDistance) const
{
    if (!m_mesh) return nullptr;
    HalfEdgeEdgePtr closest = nullptr;
    float closestDistSq = maxDistance * maxDistance;
    for (auto edge : m_mesh->getEdges()) {
        if (!edge || !edge->halfEdge) continue;
        auto halfEdge1 = edge->halfEdge;
        auto halfEdge2 = edge->halfEdge->twin;
        if (!halfEdge1 || !halfEdge2 || !halfEdge1->vertex || !halfEdge2->vertex) continue;
        auto p1 = halfEdge1->vertex->position;
        auto p2 = halfEdge2->vertex->position;
        glm::vec3 edgeVec = p2 - p1;
        glm::vec3 pointVec = point - p1;
        float edgeLengthSq = glm::dot(edgeVec, edgeVec);
        if (edgeLengthSq < EPSILON) continue;
        float t = glm::dot(pointVec, edgeVec) / edgeLengthSq;
        t = std::clamp(t, 0.0f, 1.0f);
        glm::vec3 closestOnEdge = p1 + t * edgeVec;
        float distSq = glm::dot(point - closestOnEdge, point - closestOnEdge);
        if (distSq < closestDistSq) {
            closestDistSq = distSq;
            closest = edge;
        }
    }
    return closest;
}

HalfEdgeFacePtr SelectionManager::findClosestFace(const glm::vec3& point) const
{
    if (!m_mesh) return nullptr;
    glm::vec3 rayDirection(0, -1, 0);
    auto hit = raycast(point + glm::vec3(0, 100, 0), rayDirection);
    return hit.hit ? hit.face : nullptr;
}

bool SelectionManager::rayTriangleIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                                           const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                                           float& t, glm::vec3& hitPoint) const
{
    // Möller-Trumbore ray-triangle intersection algorithm
    const float RAY_EPSILON = 1e-8f;
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(rayDirection, edge2);
    float a = glm::dot(edge1, h);
    if (a > -RAY_EPSILON && a < RAY_EPSILON) {
        return false; // Ray is parallel to triangle
    }
    float f = 1.0f / a;
    glm::vec3 s = rayOrigin - v0;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) {
        return false;
    }
    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(rayDirection, q);
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }
    t = f * glm::dot(edge2, q);
    if (t > RAY_EPSILON) {
        hitPoint = rayOrigin + rayDirection * t;
        return true;
    }
    return false;
}
