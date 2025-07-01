#include "InsetTool.h"
#include "HalfEdgeMesh.h"
#include <QDebug>
#include <algorithm>
#include <cmath>

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
    
    // Get face vertices in order
    auto originalVertices = face->getVertices();
    if (originalVertices.size() < 3) {
        qWarning() << "InsetTool: Face has less than 3 vertices";
        return nullptr;
    }
    
    // Calculate face center and normal
    QVector3D faceCenter = calculateFaceCenter(face);
    QVector3D faceNormal = face->computeNormal();
    
    // Create inset vertices
    std::vector<HalfEdgeVertexPtr> insetVertices;
    insetVertices.reserve(originalVertices.size());
    
    for (auto originalVertex : originalVertices) {
        QVector3D insetPosition = calculateInsetPosition(originalVertex, face, insetAmount);
        
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

QVector3D InsetTool::calculateFaceCenter(HalfEdgeFacePtr face) const {
    if (!face) return QVector3D();
    
    auto vertices = face->getVertices();
    if (vertices.empty()) return QVector3D();
    
    QVector3D center(0, 0, 0);
    for (auto vertex : vertices) {
        center += vertex->getPosition();
    }
    
    return center / static_cast<float>(vertices.size());
}

QVector3D InsetTool::calculateInsetPosition(HalfEdgeVertexPtr vertex, HalfEdgeFacePtr face, float insetAmount) const {
    if (!vertex || !face) return QVector3D();
    
    QVector3D originalPos = vertex->getPosition();
    QVector3D faceCenter = calculateFaceCenter(face);
    
    if (m_scaleEvenThickness) {
        // Calculate inset direction based on vertex normal within the face
        auto adjacentFaces = vertex->getAdjacentFaces();
        QVector3D vertexNormal(0, 0, 0);
        
        for (auto adjFace : adjacentFaces) {
            vertexNormal += adjFace->computeNormal();
        }
        
        if (adjacentFaces.size() > 0) {
            vertexNormal /= static_cast<float>(adjacentFaces.size());
            vertexNormal = vertexNormal.normalized();
        }
        
        // Calculate inset direction perpendicular to vertex normal in face plane
        QVector3D faceNormal = face->computeNormal();
        QVector3D toCenter = (faceCenter - originalPos).normalized();
        QVector3D insetDirection = QVector3D::crossProduct(
            QVector3D::crossProduct(faceNormal, toCenter), faceNormal).normalized();
        
        return originalPos + insetDirection * insetAmount;
    } else {
        // Simple linear interpolation toward face center
        QVector3D toCenter = faceCenter - originalPos;
        float centerDistance = toCenter.length();
        
        if (centerDistance > insetAmount) {
            return originalPos + toCenter.normalized() * insetAmount;
        } else {
            return faceCenter;
        }
    }
}

std::vector<HalfEdgeVertexPtr> InsetTool::createInsetVertices(HalfEdgeFacePtr face, float insetAmount) {
    std::vector<HalfEdgeVertexPtr> insetVertices;
    
    auto originalVertices = face->getVertices();
    for (auto vertex : originalVertices) {
        QVector3D insetPos = calculateInsetPosition(vertex, face, insetAmount);
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

HalfEdgeVertexPtr InsetTool::duplicateVertex(HalfEdgeVertexPtr vertex, const QVector3D& newPosition) {
    if (!vertex || !m_mesh) return nullptr;
    
    auto newVertex = m_mesh->addVertex(newPosition);
    if (newVertex) {
        // Copy vertex properties
        newVertex->setNormal(vertex->getNormal());
        newVertex->setTexCoord(vertex->getTexCoord());
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
