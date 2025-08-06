#include "InsetTool.h"

#include "core/mesh_elements.hpp"
#include "core/half_edge_mesh.hpp"
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

InsetTool::InsetTool() {
    clearResults();
}

bool InsetTool::insetFaces(const std::vector<HalfEdgeFacePtr>& faces, float insetAmount) {
    if (!m_mesh || faces.empty() || insetAmount <= 0.0f) {
        qWarning() << "InsetTool: Invalid input parameters";
        return false;
    }
    
    clearResults();
    
    switch (m_insetMode) {
        case InsetMode::Individual:
            for (auto face : faces) {
                if (face) {
                    auto insetFace = insetFaceIndividual(face, insetAmount);
                    if (insetFace) {
                        m_createdFaces.push_back(insetFace);
                    }
                }
            }
            break;
            
        case InsetMode::Region:
            auto regionFaces = insetRegion(faces, insetAmount);
            m_createdFaces.insert(m_createdFaces.end(), regionFaces.begin(), regionFaces.end());
            break;
    }
    
    return !m_createdFaces.empty();
}

bool InsetTool::insetFace(HalfEdgeFacePtr face, float insetAmount) {
    std::vector<HalfEdgeFacePtr> faces = {face};
    return insetFaces(faces, insetAmount);
}

bool InsetTool::canInset() const {
    return m_mesh != nullptr;
}

HalfEdgeFacePtr InsetTool::insetFaceIndividual(HalfEdgeFacePtr face, float insetAmount) {
    if (!face || !m_mesh) {
        return nullptr;
    }
    
    // Get face vertices in order by traversing half-edge structure
    std::vector<HalfEdgeVertexPtr> originalVertices;
    if (face->halfEdge) {
        auto startHE = face->halfEdge;
        auto currentHE = startHE;
        do {
            if (currentHE->vertex) {
                originalVertices.push_back(currentHE->vertex);
            }
            currentHE = currentHE->next;
        } while (currentHE && currentHE != startHE);
    }
    
    if (originalVertices.size() < 3) {
        qWarning() << "InsetTool: Face has less than 3 vertices";
        return nullptr;
    }
    
    // Calculate face center and normal
    glm::vec3 faceCenter = calculateFaceCenter(face);
    // Calculate face normal manually since computeNormal() doesn't exist
    glm::vec3 faceNormal(0.0f, 1.0f, 0.0f); // Default normal
    if (originalVertices.size() >= 3) {
        glm::vec3 v1 = originalVertices[1]->position - originalVertices[0]->position;
        glm::vec3 v2 = originalVertices[2]->position - originalVertices[0]->position;
        faceNormal = glm::normalize(glm::cross(v1, v2));
    }
    
    // Create inset vertices
    std::vector<HalfEdgeVertexPtr> insetVertices;
    insetVertices.reserve(originalVertices.size());
    
    for (auto originalVertex : originalVertices) {
        glm::vec3 insetPosition = calculateInsetPosition(originalVertex, face, insetAmount);
        
        // Apply depth if specified
        if (m_insetDepth != 0.0f) {
            insetPosition += faceNormal * m_insetDepth;
        }
        
        auto insetVertex = duplicateVertex(originalVertex, insetPosition);
        if (insetVertex) {
            insetVertices.push_back(insetVertex);
            m_createdVertices.push_back(insetVertex);
        }
    }
    
    if (insetVertices.size() != originalVertices.size()) {
        qWarning() << "InsetTool: Failed to create all inset vertices";
        return nullptr;
    }
    
    // Create the inset face
    auto insetFace = m_mesh->addFace(insetVertices);
    if (!insetFace) {
        qWarning() << "InsetTool: Failed to create inset face";
        return nullptr;
    }
    
    // Create bridge faces between original and inset vertices
    createBridgeFaces(originalVertices, insetVertices);
    
    // Remove the original face (it's now replaced by the inset face and bridges)
    m_mesh->removeFace(face);
    
    return insetFace;
}

std::vector<HalfEdgeFacePtr> InsetTool::insetRegion(const std::vector<HalfEdgeFacePtr>& faces, float insetAmount) {
    std::vector<HalfEdgeFacePtr> resultFaces;
    
    // For region inset, we need to find boundary edges and inset as a group
    // This is more complex and would require boundary detection
    // For now, fall back to individual inset
    qDebug() << "InsetTool: Region inset mode not yet fully implemented, using individual mode";
    
    for (auto face : faces) {
        auto insetFace = insetFaceIndividual(face, insetAmount);
        if (insetFace) {
            resultFaces.push_back(insetFace);
        }
    }
    
    return resultFaces;
}

glm::vec3 InsetTool::calculateFaceCenter(HalfEdgeFacePtr face) const {
    if (!face) return glm::vec3();
    
    // Traverse half-edge structure to get vertices
    std::vector<HalfEdgeVertexPtr> vertices;
    if (face->halfEdge) {
        auto startHE = face->halfEdge;
        auto currentHE = startHE;
        do {
            if (currentHE->vertex) {
                vertices.push_back(currentHE->vertex);
            }
            currentHE = currentHE->next;
        } while (currentHE && currentHE != startHE);
    }
    
    if (vertices.empty()) return glm::vec3();
    
    glm::vec3 center(0, 0, 0);
    for (auto vertex : vertices) {
        center += vertex->position; // Direct member access instead of getPosition()
    }
    
    return center / static_cast<float>(vertices.size());
}

glm::vec3 InsetTool::calculateInsetPosition(HalfEdgeVertexPtr vertex, HalfEdgeFacePtr face, float insetAmount) const {
    if (!vertex || !face) return glm::vec3();
    
    glm::vec3 originalPos = vertex->position;
    glm::vec3 faceCenter = calculateFaceCenter(face);
    
    if (m_scaleEvenThickness) {
        // Calculate inset direction based on vertex normal within the face
        // Note: rude::Vertex doesn't have getAdjacentFaces() method
        // Using simplified approach for now
        glm::vec3 vertexNormal(0, 1, 0); // Default normal
        
        // Calculate inset direction perpendicular to vertex normal in face plane
        // Calculate face normal manually
        glm::vec3 faceNormal(0.0f, 1.0f, 0.0f);
        // Get face vertices to calculate normal
        std::vector<HalfEdgeVertexPtr> faceVertices;
        if (face->halfEdge) {
            auto startHE = face->halfEdge;
            auto currentHE = startHE;
            do {
                if (currentHE->vertex) {
                    faceVertices.push_back(currentHE->vertex);
                }
                currentHE = currentHE->next;
            } while (currentHE && currentHE != startHE);
        }
        if (faceVertices.size() >= 3) {
            glm::vec3 v1 = faceVertices[1]->position - faceVertices[0]->position;
            glm::vec3 v2 = faceVertices[2]->position - faceVertices[0]->position;
            faceNormal = glm::normalize(glm::cross(v1, v2));
        }
        
        glm::vec3 toCenter = glm::normalize(faceCenter - originalPos);
        glm::vec3 insetDirection = glm::normalize(glm::cross(glm::cross(faceNormal, toCenter), faceNormal));
        
        return originalPos + insetDirection * insetAmount;
    } else {
        // Simple linear interpolation toward face center
        glm::vec3 toCenter = faceCenter - originalPos;
        float centerDistance = glm::length(toCenter);
        
        if (centerDistance > insetAmount) {
            return originalPos + glm::normalize(toCenter) * insetAmount;
        } else {
            return faceCenter;
        }
    }
}

std::vector<HalfEdgeVertexPtr> InsetTool::createInsetVertices(HalfEdgeFacePtr face, float insetAmount) {
    std::vector<HalfEdgeVertexPtr> insetVertices;
    
    // Traverse half-edge structure to get vertices
    std::vector<HalfEdgeVertexPtr> originalVertices;
    if (face->halfEdge) {
        auto startHE = face->halfEdge;
        auto currentHE = startHE;
        do {
            if (currentHE->vertex) {
                originalVertices.push_back(currentHE->vertex);
            }
            currentHE = currentHE->next;
        } while (currentHE && currentHE != startHE);
    }
    
    for (auto vertex : originalVertices) {
        glm::vec3 insetPos = calculateInsetPosition(vertex, face, insetAmount);
        auto insetVertex = duplicateVertex(vertex, insetPos);
        if (insetVertex) {
            insetVertices.push_back(insetVertex);
            m_createdVertices.push_back(insetVertex);
        }
    }
    
    return insetVertices;
}

void InsetTool::createBridgeFaces(const std::vector<HalfEdgeVertexPtr>& originalVertices, 
                                  const std::vector<HalfEdgeVertexPtr>& insetVertices) {
    if (originalVertices.size() != insetVertices.size() || originalVertices.size() < 3) {
        return;
    }
    
    size_t numVertices = originalVertices.size();
    
    // Create quad faces bridging between original and inset vertices
    for (size_t i = 0; i < numVertices; ++i) {
        size_t nextI = (i + 1) % numVertices;
        
        // Create quad: original[i] -> original[next] -> inset[next] -> inset[i]
        auto bridgeFace = createQuadFace(
            originalVertices[i],
            originalVertices[nextI],
            insetVertices[nextI],
            insetVertices[i]
        );
        
        if (bridgeFace) {
            m_createdFaces.push_back(bridgeFace);
        }
    }
}

HalfEdgeVertexPtr InsetTool::duplicateVertex(HalfEdgeVertexPtr vertex, const glm::vec3& newPosition) {
    if (!vertex || !m_mesh) return nullptr;
    
    auto newVertex = m_mesh->addVertex(newPosition);
    if (newVertex) {
        // Copy vertex properties using direct member access
        newVertex->normal = vertex->normal;
        newVertex->texCoord = vertex->texCoord;
    }
    
    return newVertex;
}

HalfEdgeFacePtr InsetTool::createQuadFace(HalfEdgeVertexPtr v1, HalfEdgeVertexPtr v2,
                                          HalfEdgeVertexPtr v3, HalfEdgeVertexPtr v4) {
    if (!v1 || !v2 || !v3 || !v4 || !m_mesh) return nullptr;
    
    std::vector<HalfEdgeVertexPtr> vertices = {v1, v2, v3, v4};
    return m_mesh->addFace(vertices);
}

void InsetTool::clearResults() {
    m_createdFaces.clear();
    m_createdVertices.clear();
}
