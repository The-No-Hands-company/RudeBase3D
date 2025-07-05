#include "ExtrudeTool.h"
#include "HalfEdgeMesh.h"
#include "SelectionManager.h"
#include "core/mesh_elements.hpp"
#include "core/half_edge_mesh.hpp"
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

ExtrudeTool::ExtrudeTool()
    : m_extrudeMode(ExtrudeMode::Normal)
    , m_extrudeDirection(0, 1, 0)
    , m_isExtruding(false)
    , m_currentDistance(0.0f)
{
}

void ExtrudeTool::setMesh(HalfEdgeMeshPtr mesh)
{
    if (m_isExtruding) {
        cancelExtrude();
    }
    m_mesh = mesh;
}

void ExtrudeTool::setSelectionManager(std::shared_ptr<SelectionManager> selectionManager)
{
    m_selectionManager = selectionManager;
}

bool ExtrudeTool::beginExtrude()
{
    if (!canExtrude()) {
        qDebug() << "Cannot extrude: no valid selection or mesh";
        return false;
    }
    
    if (m_isExtruding) {
        confirmExtrude(); // Finish previous extrude
    }
    
    m_isExtruding = true;
    m_currentDistance = 0.0f;
    
    // Store original geometry for potential cancellation
    m_originalData = OriginalData();
    
    // Create extruded geometry based on current selection
    createExtrudedGeometry();
    
    qDebug() << "Extrude operation started";
    return true;
}

void ExtrudeTool::updateExtrude(float distance)
{
    if (!m_isExtruding) return;
    
    m_currentDistance = distance;
    
    // Update vertex positions based on extrude direction and distance
    switch (m_selectionManager->getSelectionType()) {
        case SelectionType::Face: {
            auto selectedFaces = m_selectionManager->getSelectedFaces();
            for (auto face : selectedFaces) {
                if (!face) continue;
                
                glm::vec3 normal = (m_extrudeMode == ExtrudeMode::Normal) 
                    ? calculateFaceNormal(face) 
                    : m_extrudeDirection;
                
                // Update positions of vertices in the extruded face
                // Get vertices by traversing half-edges of the face
                std::vector<rude::VertexPtr> vertices;
                if (face->halfEdge) {
                    auto currentHE = face->halfEdge;
                    do {
                        if (currentHE->vertex) {
                            vertices.push_back(currentHE->vertex);
                        }
                        currentHE = currentHE->next;
                    } while (currentHE && currentHE != face->halfEdge);
                }
                
                for (auto vertex : vertices) {
                    if (vertex) {
                        // Find corresponding new vertex and update position
                        glm::vec3 newPos = vertex->position + normal * distance;
                        vertex->position = newPos;
                    }
                }
            }
            break;
        }
        
        case SelectionType::Edge: {
            auto selectedEdges = m_selectionManager->getSelectedEdges();
            for (auto edge : selectedEdges) {
                if (!edge) continue;
                
                glm::vec3 normal = (m_extrudeMode == ExtrudeMode::Normal) 
                    ? calculateEdgeNormal(edge) 
                    : m_extrudeDirection;
                
                // Update positions of edge vertices
                auto originVertex = edge->halfEdge ? edge->halfEdge->vertex : nullptr;
                if (originVertex) {
                    glm::vec3 newPos = originVertex->position + normal * distance;
                    originVertex->position = newPos;
                }
                auto targetVertex = (edge->halfEdge && edge->halfEdge->twin) ? edge->halfEdge->twin->vertex : nullptr;
                if (targetVertex) {
                    glm::vec3 newPos = targetVertex->position + normal * distance;
                    targetVertex->position = newPos;
                }
            }
            break;
        }
        
        case SelectionType::Vertex: {
            auto selectedVertices = m_selectionManager->getSelectedVertices();
            for (auto vertex : selectedVertices) {
                if (!vertex) continue;
                
                glm::vec3 normal = (m_extrudeMode == ExtrudeMode::Normal) 
                    ? calculateVertexNormal(vertex) 
                    : m_extrudeDirection;
                
                glm::vec3 newPos = vertex->position + normal * distance;
                vertex->position = newPos;
            }
            break;
        }
        
        default:
            break;
    }
    
    updatePreview();
}

void ExtrudeTool::updateExtrude(const glm::vec3& direction, float distance)
{
    m_extrudeDirection = direction;
    updateExtrude(distance);
}

void ExtrudeTool::confirmExtrude()
{
    if (!m_isExtruding) return;
    
    m_isExtruding = false;
    
    // Clear original data as we're committing the change
    m_originalData = OriginalData();
    m_preview = ExtrudePreview();
    
    qDebug() << "Extrude operation confirmed with distance:" << m_currentDistance;
}

void ExtrudeTool::cancelExtrude()
{
    if (!m_isExtruding) return;
    
    // Restore original geometry
    restoreOriginalGeometry();
    
    m_isExtruding = false;
    m_currentDistance = 0.0f;
    m_originalData = OriginalData();
    m_preview = ExtrudePreview();
    
    qDebug() << "Extrude operation cancelled";
}

bool ExtrudeTool::canExtrude() const
{
    if (!m_mesh || !m_selectionManager) {
        return false;
    }
    
    // Check if we have a valid selection
    switch (m_selectionManager->getSelectionType()) {
        case SelectionType::Face:
            return !m_selectionManager->getSelectedFaces().empty();
        case SelectionType::Edge:
            return !m_selectionManager->getSelectedEdges().empty();
        case SelectionType::Vertex:
            return !m_selectionManager->getSelectedVertices().empty();
        default:
            return false;
    }
}

void ExtrudeTool::createExtrudedGeometry()
{
    if (!m_mesh || !m_selectionManager) return;
    
    switch (m_selectionManager->getSelectionType()) {
        case SelectionType::Face:
            extrudeFaces(m_selectionManager->getSelectedFaces());
            break;
        case SelectionType::Edge:
            extrudeEdges(m_selectionManager->getSelectedEdges());
            break;
        case SelectionType::Vertex:
            extrudeVertices(m_selectionManager->getSelectedVertices());
            break;
        default:
            break;
    }
}

bool ExtrudeTool::extrudeFaces(const std::vector<HalfEdgeFacePtr>& faces)
{
    if (faces.empty()) return false;
    
    // For each selected face, create new vertices and connect them
    for (auto face : faces) {
        if (!face) continue;
        
        auto vertices = face->getVertices();
        if (vertices.size() < 3) continue;
        
        // Store original positions
        for (auto vertex : vertices) {
            if (vertex) {
                m_originalData.originalPositions.push_back(vertex->position);
                // Note: rude::Vertex doesn't have getId() method
                // m_originalData.originalVertexIds.push_back(vertex->getId());
            }
        }
        
        // Create new vertices for the extruded face
        std::vector<HalfEdgeVertexPtr> newVertices;
        for (auto vertex : vertices) {
            if (vertex) {
                auto newVertex = duplicateVertex(vertex);
                newVertices.push_back(newVertex);
                m_originalData.newVertexIds.push_back(newVertex->getId());
            }
        }
        
        // Create side faces to connect original and new vertices
        for (size_t i = 0; i < vertices.size(); ++i) {
            size_t next = (i + 1) % vertices.size();
            
            if (vertices[i] && vertices[next] && newVertices[i] && newVertices[next]) {
                auto sideFace = createQuadFace(vertices[i], vertices[next], 
                                             newVertices[next], newVertices[i]);
                if (sideFace) {
                    m_originalData.newFaceIds.push_back(sideFace->getId());
                }
            }
        }
        
        // Update the original face to use new vertices
        // This is a simplified approach - in a full implementation, 
        // we would need to properly update the HEDS topology
    }
    
    return true;
}

bool ExtrudeTool::extrudeEdges(const std::vector<HalfEdgeEdgePtr>& edges)
{
    if (edges.empty()) return false;
    
    // For edge extrusion, create faces from the edges
    for (auto edge : edges) {
        auto originVertex = edge->halfEdge ? edge->halfEdge->vertex : nullptr;
        auto targetVertex = (edge->halfEdge && edge->halfEdge->twin) ? edge->halfEdge->twin->vertex : nullptr;
        if (!edge || !originVertex || !targetVertex) continue;
        
        // Store original positions
        m_originalData.originalPositions.push_back(originVertex->position);
        m_originalData.originalPositions.push_back(targetVertex->position);
        // Note: rude::Vertex doesn't have getId() method
        // m_originalData.originalVertexIds.push_back(originVertex->getId());
        // m_originalData.originalVertexIds.push_back(targetVertex->getId());
        
        // Create new vertices
        auto newVertex1 = duplicateVertex(originVertex);
        auto newVertex2 = duplicateVertex(targetVertex);
        
        // Note: rude::Vertex doesn't have getId() method
        // m_originalData.newVertexIds.push_back(newVertex1->getId());
        // m_originalData.newVertexIds.push_back(newVertex2->getId());
        
        // Create a face connecting the original edge with the new edge
        auto extrudeFace = createQuadFace(edge->getOriginVertex(), edge->getTargetVertex(),
                                        newVertex2, newVertex1);
        if (extrudeFace) {
            m_originalData.newFaceIds.push_back(extrudeFace->getId());
        }
    }
    
    return true;
}

bool ExtrudeTool::extrudeVertices(const std::vector<HalfEdgeVertexPtr>& vertices)
{
    if (vertices.empty()) return false;
    
    // For vertex extrusion, create edges from the vertices
    for (auto vertex : vertices) {
        if (!vertex) continue;
        
        // Store original position
        m_originalData.originalPositions.push_back(vertex->position);
        // Note: rude::Vertex doesn't have getId() method
        // m_originalData.originalVertexIds.push_back(vertex->getId());
        
        // Create new vertex
        auto newVertex = duplicateVertex(vertex);
        m_originalData.newVertexIds.push_back(newVertex->getId());
        
        // Create edge connecting original and new vertex
        // In a full implementation, this would create proper HEDS edge topology
    }
    
    return true;
}

glm::vec3 ExtrudeTool::calculateFaceNormal(HalfEdgeFacePtr face) const
{
    if (!face) return glm::vec3(0, 1, 0);
    
    auto vertices = face->getVertices();
    if (vertices.size() < 3) return glm::vec3(0, 1, 0);
    
    // Calculate normal using cross product of two edges
    glm::vec3 v1 = vertices[1]->position - vertices[0]->position;
    glm::vec3 v2 = vertices[2]->position - vertices[0]->position;
    
    glm::vec3 normal = glm::cross(v1, v2);
    return glm::normalize(normal);
}

glm::vec3 ExtrudeTool::calculateEdgeNormal(HalfEdgeEdgePtr edge) const
{
    auto originVertex = edge->halfEdge ? edge->halfEdge->vertex : nullptr;
    auto targetVertex = (edge->halfEdge && edge->halfEdge->twin) ? edge->halfEdge->twin->vertex : nullptr;
    if (!edge || !originVertex || !targetVertex) {
        return glm::vec3(0, 1, 0);
    }
    
    // For edge normal, we need to find adjacent faces and average their normals
    // Simplified approach: use perpendicular to edge direction
    glm::vec3 edgeDir = targetVertex->position - originVertex->position;
    glm::vec3 up = glm::vec3(0, 1, 0);
    
    glm::vec3 normal = glm::cross(edgeDir, up);
    return glm::normalize(normal);
}

glm::vec3 ExtrudeTool::calculateVertexNormal(HalfEdgeVertexPtr vertex) const
{
    if (!vertex) return glm::vec3(0, 1, 0);
    
    // Calculate vertex normal by averaging adjacent face normals
    auto adjacentFaces = vertex->getAdjacentFaces();
    if (adjacentFaces.empty()) return glm::vec3(0, 1, 0);
    
    glm::vec3 normalSum(0, 0, 0);
    for (auto face : adjacentFaces) {
        normalSum += calculateFaceNormal(face);
    }
    
    glm::vec3 normal = glm::normalize(normalSum);
    return glm::normalize(normal);
}

void ExtrudeTool::updatePreview()
{
    // Update preview data for rendering
    m_preview = ExtrudePreview();
    
    // This would populate preview geometry for rendering
    // For now, just clear the preview
}

void ExtrudeTool::restoreOriginalGeometry()
{
    // Restore original vertex positions
    if (m_originalData.originalPositions.size() == m_originalData.originalVertexIds.size()) {
        for (size_t i = 0; i < m_originalData.originalVertexIds.size(); ++i) {
            // Find vertex by ID and restore position
            // This is simplified - in practice we'd need proper vertex lookup
        }
    }
    
    // Remove newly created vertices and faces
    // This would require proper HEDS cleanup operations
}

HalfEdgeVertexPtr ExtrudeTool::duplicateVertex(HalfEdgeVertexPtr vertex)
{
    if (!vertex || !m_mesh) return nullptr;
    
    // Create a new vertex with the same position and properties
    auto newVertex = m_mesh->addVertex(vertex->position);
    if (newVertex) {
        newVertex->normal = vertex->normal;
        newVertex->texCoord = vertex->texCoord;
    }
    
    return newVertex;
}

HalfEdgeFacePtr ExtrudeTool::createQuadFace(HalfEdgeVertexPtr v1, HalfEdgeVertexPtr v2, 
                                          HalfEdgeVertexPtr v3, HalfEdgeVertexPtr v4)
{
    if (!v1 || !v2 || !v3 || !v4 || !m_mesh) return nullptr;
    
    // Create a new face with the four vertices
    std::vector<HalfEdgeVertexPtr> vertices = {v1, v2, v3, v4};
    auto newFace = m_mesh->addFace(vertices);
    
    return newFace;
}

void ExtrudeTool::bridgeEdges(HalfEdgeEdgePtr edge1, HalfEdgeEdgePtr edge2)
{
    // Suppress unused parameter warnings
    Q_UNUSED(edge1);
    Q_UNUSED(edge2);
    
    // Create connections between two edges
    // This would involve creating proper half-edge topology
    // TODO: Implement proper edge bridging functionality
}
