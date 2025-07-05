#include "LoopCutTool.h"
#include "HalfEdgeMesh.h"
#include "core/mesh_elements.hpp"
#include "core/half_edge_mesh.hpp"
#include <QDebug>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <glm/glm.hpp>

LoopCutTool::LoopCutTool() {
    clearResults();
}

bool LoopCutTool::createLoopCut(HalfEdgeEdgePtr startEdge, int numCuts) {
    return createLoopCutAtPosition(startEdge, m_cutPosition, numCuts);
}

bool LoopCutTool::createLoopCutAtPosition(HalfEdgeEdgePtr startEdge, float position, int numCuts) {
    if (!canCreateLoopCut(startEdge) || numCuts <= 0) {
        qWarning() << "LoopCutTool: Invalid parameters for loop cut";
        return false;
    }
    
    clearResults();
    
    // Find the complete edge loop starting from the given edge
    auto edgeLoop = findEdgeLoop(startEdge);
    if (edgeLoop.empty()) {
        qWarning() << "LoopCutTool: Could not find valid edge loop";
        return false;
    }
    
    // Create multiple cuts if requested
    bool success = true;
    for (int cut = 0; cut < numCuts; ++cut) {
        float cutPos = position;
        
        if (numCuts > 1 && m_evenSpacing) {
            // Distribute cuts evenly
            cutPos = position + (static_cast<float>(cut) / static_cast<float>(numCuts - 1)) * 0.2f - 0.1f;
            cutPos = qBound(0.1f, cutPos, 0.9f);
        }
        
        if (!cutEdgeLoop(edgeLoop, cutPos)) {
            success = false;
            qWarning() << "LoopCutTool: Failed to create cut" << cut + 1 << "of" << numCuts;
        }
    }
    
    return success;
}

bool LoopCutTool::createParallelLoops(HalfEdgeEdgePtr startEdge, int numLoops, float spacing) {
    if (!canCreateLoopCut(startEdge) || numLoops <= 0) {
        return false;
    }
    
    bool success = true;
    for (int i = 0; i < numLoops; ++i) {
        float position = 0.5f + (static_cast<float>(i) - static_cast<float>(numLoops - 1) / 2.0f) * spacing;
        position = qBound(0.1f, position, 0.9f);
        
        if (!createLoopCutAtPosition(startEdge, position, 1)) {
            success = false;
        }
    }
    
    return success;
}

bool LoopCutTool::canCreateLoopCut(HalfEdgeEdgePtr startEdge) const {
    if (!m_mesh || !startEdge) {
        return false;
    }
    
    // Check if edge is manifold (has exactly one or two adjacent faces)
    if (!isManifoldEdge(startEdge)) {
        return false;
    }
    
    // Check if we can find a valid loop
    auto edgeLoop = findEdgeLoop(startEdge);
    return !edgeLoop.empty() && edgeLoop.size() >= 3;
}

std::vector<HalfEdgeEdgePtr> LoopCutTool::findEdgeLoop(HalfEdgeEdgePtr startEdge) const {
    std::vector<HalfEdgeEdgePtr> loop;
    if (!startEdge) return loop;
    
    std::unordered_set<HalfEdgeEdgePtr> visited;
    auto currentEdge = startEdge;
    
    do {
        if (visited.find(currentEdge) != visited.end()) {
            // We've encountered this edge before - loop detected
            break;
        }
        
        visited.insert(currentEdge);
        loop.push_back(currentEdge);
        
        // Find the next edge in the loop
        // We need to traverse across faces to find parallel edges
        auto nextEdge = findNextLoopEdge(currentEdge, nullptr);
        if (!nextEdge || nextEdge == startEdge) {
            if (nextEdge == startEdge && loop.size() > 2) {
                // Valid closed loop
                break;
            } else if (!nextEdge) {
                // Dead end - not a valid loop
                if (loop.size() < 4) {
                    loop.clear();
                }
                break;
            }
        }
        
        currentEdge = nextEdge;
        
    } while (currentEdge && loop.size() < 1000); // Safety limit
    
    return loop;
}

HalfEdgeEdgePtr LoopCutTool::findNextLoopEdge(HalfEdgeEdgePtr currentEdge, HalfEdgeFacePtr throughFace) const {
    if (!currentEdge) return nullptr;
    
    // Get the face on one side of the current edge
    auto face = currentEdge->halfEdge ? currentEdge->halfEdge->face : nullptr;
    if (!face && currentEdge->halfEdge && currentEdge->halfEdge->twin) {
        face = currentEdge->halfEdge->twin->face;
    }
    
    if (!face) return nullptr;
    
    // Find the edge opposite to the current edge in the face
    // Since rude::Face doesn't have getEdges(), we need to traverse the half-edge loop
    std::vector<rude::EdgePtr> faceEdges;
    if (face->halfEdge) {
        auto startHE = face->halfEdge;
        auto currentHE = startHE;
        do {
            if (currentHE->edge) {
                faceEdges.push_back(currentHE->edge);
            }
            currentHE = currentHE->next;
        } while (currentHE && currentHE != startHE);
    }
    
    if (faceEdges.size() != 4) {
        // For now, only handle quad faces properly
        // TODO: Handle n-gons and triangles
        return nullptr;
    }
    
    // In a quad, the opposite edge is 2 positions away
    auto it = std::find(faceEdges.begin(), faceEdges.end(), currentEdge);
    if (it == faceEdges.end()) {
        // Try with twin edge
        if (currentEdge->halfEdge && currentEdge->halfEdge->twin && currentEdge->halfEdge->twin->edge) {
            auto twin = currentEdge->halfEdge->twin->edge;
            it = std::find(faceEdges.begin(), faceEdges.end(), twin);
        }
    }
    
    if (it != faceEdges.end()) {
        size_t index = std::distance(faceEdges.begin(), it);
        size_t oppositeIndex = (index + 2) % faceEdges.size();
        return faceEdges[oppositeIndex];
    }
    
    return nullptr;
}

std::vector<HalfEdgeFacePtr> LoopCutTool::findAffectedFaces(const std::vector<HalfEdgeEdgePtr>& edgeLoop) const {
    std::unordered_set<HalfEdgeFacePtr> affectedFaces;
    
    for (auto edge : edgeLoop) {
        if (edge->halfEdge && edge->halfEdge->face) {
            affectedFaces.insert(edge->halfEdge->face);
        }
        if (edge->halfEdge && edge->halfEdge->twin && edge->halfEdge->twin->face) {
            affectedFaces.insert(edge->halfEdge->twin->face);
        }
    }
    
    return std::vector<HalfEdgeFacePtr>(affectedFaces.begin(), affectedFaces.end());
}

bool LoopCutTool::cutEdgeLoop(const std::vector<HalfEdgeEdgePtr>& edgeLoop, float position) {
    if (edgeLoop.empty()) return false;
    
    std::vector<HalfEdgeEdgePtr> newLoopEdges;
    std::vector<HalfEdgeVertexPtr> newVertices;
    
    // First pass: split all edges in the loop and create new vertices
    for (auto edge : edgeLoop) {
        auto splitResult = splitEdge(edge, position);
        if (splitResult.first && splitResult.second) {
            newVertices.push_back(splitResult.first); // The new vertex on the edge
            m_createdVertices.push_back(splitResult.first);
        } else {
            qWarning() << "LoopCutTool: Failed to split edge in loop";
            return false;
        }
    }
    
    // Second pass: create new edges connecting the new vertices
    if (newVertices.size() >= 3) {
        for (size_t i = 0; i < newVertices.size(); ++i) {
            size_t nextIndex = (i + 1) % newVertices.size();
            auto newEdge = createEdgeBetweenVertices(newVertices[i], newVertices[nextIndex]);
            if (newEdge) {
                newLoopEdges.push_back(newEdge);
                m_createdEdges.push_back(newEdge);
            }
        }
        
        m_createdLoops.push_back(newLoopEdges);
    }
    
    // Third pass: update face connectivity for affected faces
    auto affectedFaces = findAffectedFaces(edgeLoop);
    for (auto face : affectedFaces) {
        updateFaceConnectivity(face);
    }
    
    return !newLoopEdges.empty();
}

std::pair<HalfEdgeVertexPtr, HalfEdgeEdgePtr> LoopCutTool::splitEdge(HalfEdgeEdgePtr edge, float position) {
    if (!edge || !m_mesh) {
        return std::make_pair(nullptr, nullptr);
    }
    
    auto originVertex = edge->halfEdge ? edge->halfEdge->vertex : nullptr;
    auto targetVertex = (edge->halfEdge && edge->halfEdge->twin) ? edge->halfEdge->twin->vertex : nullptr;
    
    if (!originVertex || !targetVertex) {
        return std::make_pair(nullptr, nullptr);
    }
    
    // Create new vertex at the split position
    glm::vec3 newPosition = originVertex->position + 
                           (targetVertex->position - originVertex->position) * position;
    
    auto newVertex = m_mesh->addVertex(newPosition);
    if (!newVertex) {
        return std::make_pair(nullptr, nullptr);
    }
    
    // Interpolate vertex properties
    glm::vec3 newNormal = originVertex->normal + 
                         (targetVertex->normal - originVertex->normal) * position;
    newVertex->normal = newNormal;
    
    glm::vec2 newTexCoord = originVertex->texCoord + 
                           (targetVertex->texCoord - originVertex->texCoord) * position;
    newVertex->texCoord = newTexCoord;
    
    // Create new edge from new vertex to target
    auto newEdge = m_mesh->addEdge(newVertex, targetVertex);
    if (newEdge) {
        // TODO: Update original edge connectivity properly
        // The rude::Edge doesn't have setTargetVertex() method
        // This would require more complex topology updates
        m_createdEdges.push_back(newEdge);
    }
    
    return std::make_pair(newVertex, newEdge);
}

HalfEdgeVertexPtr LoopCutTool::createVertexOnEdge(HalfEdgeEdgePtr edge, float position) {
    if (!edge) return nullptr;
    
    auto splitResult = splitEdge(edge, position);
    return splitResult.first;
}

bool LoopCutTool::splitFaceAtVertices(HalfEdgeFacePtr face, HalfEdgeVertexPtr vertex1, HalfEdgeVertexPtr vertex2) {
    if (!face || !vertex1 || !vertex2 || vertex1 == vertex2) {
        return false;
    }
    
    // This is a simplified implementation
    // A full implementation would need to handle proper face splitting with correct topology
    qDebug() << "LoopCutTool: Face splitting not yet fully implemented";
    return true;
}

HalfEdgeEdgePtr LoopCutTool::createEdgeBetweenVertices(HalfEdgeVertexPtr v1, HalfEdgeVertexPtr v2) {
    if (!v1 || !v2 || !m_mesh) return nullptr;
    
    return m_mesh->addEdge(v1, v2);
}

void LoopCutTool::updateFaceConnectivity(HalfEdgeFacePtr face) {
    if (!face) return;
    
    // Update face connectivity after loop cut
    // This would involve complex topology updates
    // For now, just ensure the face is still valid by counting edges manually
    int edgeCount = 0;
    if (face->halfEdge) {
        auto startHE = face->halfEdge;
        auto currentHE = startHE;
        do {
            edgeCount++;
            currentHE = currentHE->next;
            if (edgeCount > 100) break; // Safety limit
        } while (currentHE && currentHE != startHE);
    }
    
    if (edgeCount < 3) {
        qWarning() << "LoopCutTool: Face has invalid edge count after cut";
    }
}

bool LoopCutTool::isManifoldEdge(HalfEdgeEdgePtr edge) const {
    if (!edge) return false;
    
    int faceCount = 0;
    if (edge->halfEdge && edge->halfEdge->face) faceCount++;
    if (edge->halfEdge && edge->halfEdge->twin && edge->halfEdge->twin->face) faceCount++;
    
    return faceCount > 0 && faceCount <= 2;
}

bool LoopCutTool::wouldCreateInvalidTopology(HalfEdgeEdgePtr edge, float position) const {
    // Check if cutting at this position would create degenerate geometry
    if (position <= 0.01f || position >= 0.99f) {
        return true; // Too close to existing vertices
    }
    
    return false;
}

bool LoopCutTool::isValidLoopContinuation(HalfEdgeEdgePtr currentEdge, HalfEdgeEdgePtr nextEdge) const {
    if (!currentEdge || !nextEdge) return false;
    
    // Calculate edge vectors manually
    glm::vec3 dir1(0.0f);
    glm::vec3 dir2(0.0f);
    
    // Get vertices for currentEdge
    if (currentEdge->halfEdge && currentEdge->halfEdge->vertex && 
        currentEdge->halfEdge->twin && currentEdge->halfEdge->twin->vertex) {
        auto v1 = currentEdge->halfEdge->vertex;
        auto v2 = currentEdge->halfEdge->twin->vertex;
        dir1 = v2->position - v1->position;
    }
    
    // Get vertices for nextEdge
    if (nextEdge->halfEdge && nextEdge->halfEdge->vertex && 
        nextEdge->halfEdge->twin && nextEdge->halfEdge->twin->vertex) {
        auto v1 = nextEdge->halfEdge->vertex;
        auto v2 = nextEdge->halfEdge->twin->vertex;
        dir2 = v2->position - v1->position;
    }
    
    if (glm::length(dir1) < 1e-6f || glm::length(dir2) < 1e-6f) {
        return false;
    }
    
    // Check if edges are roughly parallel (simplified check)
    dir1 = glm::normalize(dir1);
    dir2 = glm::normalize(dir2);
    float dot = glm::dot(dir1, dir2);
    return std::abs(dot) > 0.5f; // Roughly parallel
}

glm::vec3 LoopCutTool::calculateLoopDirection(HalfEdgeEdgePtr startEdge) const {
    if (!startEdge) return glm::vec3(1, 0, 0);
    
    switch (m_loopDirection) {
        case LoopDirection::Custom:
            return m_customDirection;
            
        case LoopDirection::Automatic:
        default:
            // Calculate edge vector manually
            if (startEdge->halfEdge && startEdge->halfEdge->vertex && 
                startEdge->halfEdge->twin && startEdge->halfEdge->twin->vertex) {
                auto v1 = startEdge->halfEdge->vertex;
                auto v2 = startEdge->halfEdge->twin->vertex;
                glm::vec3 dir = v2->position - v1->position;
                if (glm::length(dir) > 1e-6f) {
                    return glm::normalize(dir);
                }
            }
            return glm::vec3(1, 0, 0);
    }
}

void LoopCutTool::clearResults() {
    m_createdLoops.clear();
    m_createdVertices.clear();
    m_createdEdges.clear();
}
