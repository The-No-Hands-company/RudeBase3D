#include "InsetTool.h"
#include "HalfEdgeMesh.h"
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

InsetTool::InsetTool() {
    clearResults();
}

bool InsetTool::insetFaces(const std::vector<rude::HalfEdgeFacePtr>& faces, float insetAmount) {
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

bool InsetTool::insetFace(rude::HalfEdgeFacePtr face, float insetAmount) {
    std::vector<rude::HalfEdgeFacePtr> faces = {face};
    return insetFaces(faces, insetAmount);
}

bool InsetTool::canInset() const {
    return m_mesh != nullptr;
}

rude::HalfEdgeFacePtr InsetTool::insetFaceIndividual(rude::HalfEdgeFacePtr face, float insetAmount) {
    if (!face || !m_mesh) {
        return nullptr;
    }
    
    // Get face vertices in order
    auto originalVertices = face->getVertices();
    if (originalVertices.size() < 3) {
        qWarning() << "InsetTool: Face has less than 3 vertices";
        return nullptr;
    }
    
    // Calculate face center and normal
    glm::vec3 faceCenter = calculateFaceCenter(face);
    glm::vec3 faceNormal = face->computeNormal();
    
    // Create inset vertices
    std::vector<rude::HalfEdgeVertexPtr> insetVertices;
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

std::vector<rude::HalfEdgeFacePtr> InsetTool::insetRegion(const std::vector<rude::HalfEdgeFacePtr>& faces, float insetAmount) {
    std::vector<rude::HalfEdgeFacePtr> resultFaces;
    
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

glm::vec3 InsetTool::calculateFaceCenter(rude::HalfEdgeFacePtr face) const {
    if (!face) return glm::vec3();
    
    auto vertices = face->getVertices();
    if (vertices.empty()) return glm::vec3();
    
    glm::vec3 center(0, 0, 0);
    for (auto vertex : vertices) {
        center += vertex->getPosition();
    }
    
    return center / static_cast<float>(vertices.size());
}

glm::vec3 InsetTool::calculateInsetPosition(rude::HalfEdgeVertexPtr vertex, rude::HalfEdgeFacePtr face, float insetAmount) const {
    if (!vertex || !face) return glm::vec3();
    
    glm::vec3 originalPos = vertex->getPosition();
    glm::vec3 faceCenter = calculateFaceCenter(face);
    
    if (m_scaleEvenThickness) {
        // Calculate inset direction based on vertex normal within the face
        auto adjacentFaces = vertex->getAdjacentFaces();
        glm::vec3 vertexNormal(0, 0, 0);
        
        for (auto adjFace : adjacentFaces) {
            vertexNormal += adjFace->computeNormal();
        }
        
        if (adjacentFaces.size() > 0) {
            vertexNormal = glm::normalize(vertexNormal);
        }
        
        // Calculate inset direction perpendicular to vertex normal in face plane
        glm::vec3 faceNormal = face->computeNormal();
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

std::vector<rude::HalfEdgeVertexPtr> InsetTool::createInsetVertices(rude::HalfEdgeFacePtr face, float insetAmount) {
    std::vector<rude::HalfEdgeVertexPtr> insetVertices;
    
    auto originalVertices = face->getVertices();
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

void InsetTool::createBridgeFaces(const std::vector<rude::HalfEdgeVertexPtr>& originalVertices, 
                                  const std::vector<rude::HalfEdgeVertexPtr>& insetVertices) {
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

rude::HalfEdgeVertexPtr InsetTool::duplicateVertex(rude::HalfEdgeVertexPtr vertex, const glm::vec3& newPosition) {
    if (!vertex || !m_mesh) return nullptr;
    
    auto newVertex = m_mesh->addVertex(newPosition);
    if (newVertex) {
        // Copy vertex properties
        newVertex->setNormal(vertex->getNormal());
        newVertex->setTexCoord(vertex->getTexCoord());
    }
    
    return newVertex;
}

rude::HalfEdgeFacePtr InsetTool::createQuadFace(rude::HalfEdgeVertexPtr v1, rude::HalfEdgeVertexPtr v2,
                                          rude::HalfEdgeVertexPtr v3, rude::HalfEdgeVertexPtr v4) {
    if (!v1 || !v2 || !v3 || !v4 || !m_mesh) return nullptr;
    
    std::vector<rude::HalfEdgeVertexPtr> vertices = {v1, v2, v3, v4};
    return m_mesh->addFace(vertices);
}

void InsetTool::clearResults() {
    m_createdFaces.clear();
    m_createdVertices.clear();
}
