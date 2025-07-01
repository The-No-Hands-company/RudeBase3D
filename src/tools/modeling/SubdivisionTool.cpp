#include "SubdivisionTool.h"
#include "HalfEdgeMesh.h"
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <unordered_set>

SubdivisionTool::SubdivisionTool() {
}

HalfEdgeMeshPtr SubdivisionTool::subdivide(int levels) {
    if (!canSubdivide() || levels <= 0) {
        qWarning() << "SubdivisionTool: Cannot subdivide - invalid parameters";
        return nullptr;
    }
    
    auto currentMesh = m_mesh;
    
    for (int level = 0; level < levels; ++level) {
        HalfEdgeMeshPtr subdividedMesh = nullptr;
        
        switch (m_subdivisionType) {
            case SubdivisionType::CatmullClark:
                subdividedMesh = applyCatmullClark(currentMesh);
                break;
                
            case SubdivisionType::Loop:
                subdividedMesh = applyLoop(currentMesh);
                break;
                
            case SubdivisionType::DooSabin:
                subdividedMesh = applyDooSabin(currentMesh);
                break;
                
            case SubdivisionType::ModifiedButterfly:
                qWarning() << "SubdivisionTool: Modified Butterfly subdivision not yet implemented";
                return currentMesh;
        }
        
        if (!subdividedMesh) {
            qWarning() << "SubdivisionTool: Failed to subdivide at level" << level + 1;
            return currentMesh;
        }
        
        currentMesh = subdividedMesh;
    }
    
    return currentMesh;
}

HalfEdgeMeshPtr SubdivisionTool::subdivideAdaptive(float errorThreshold) {
    if (!canSubdivide()) {
        return nullptr;
    }
    
    auto currentMesh = m_mesh;
    bool needsMoreSubdivision = true;
    int maxIterations = 5; // Safety limit
    int iteration = 0;
    
    while (needsMoreSubdivision && iteration < maxIterations) {
        needsMoreSubdivision = false;
        
        // Check each face to see if it needs subdivision
        auto faces = currentMesh->getFaces();
        for (auto face : faces) {
            if (needsSubdivision(face, errorThreshold)) {
                needsMoreSubdivision = true;
                break;
            }
        }
        
        if (needsMoreSubdivision) {
            auto subdividedMesh = subdivide(1);
            if (subdividedMesh) {
                currentMesh = subdividedMesh;
            } else {
                break;
            }
        }
        
        iteration++;
    }
    
    return currentMesh;
}

HalfEdgeMeshPtr SubdivisionTool::subdivideRegion(const std::vector<HalfEdgeFacePtr>& faces, int levels) {
    // Regional subdivision is complex and would require boundary handling
    qDebug() << "SubdivisionTool: Regional subdivision not yet fully implemented";
    return subdivide(levels);
}

bool SubdivisionTool::canSubdivide() const {
    if (!m_mesh || m_mesh->isEmpty()) {
        return false;
    }
    
    // Check if mesh has valid topology
    auto faces = m_mesh->getFaces();
    if (faces.empty()) {
        return false;
    }
    
    // For Catmull-Clark, prefer quad meshes
    if (m_subdivisionType == SubdivisionType::CatmullClark) {
        return true; // Can handle mixed topology
    }
    
    // For Loop subdivision, need triangle meshes
    if (m_subdivisionType == SubdivisionType::Loop) {
        return isTriangleMesh();
    }
    
    return true;
}

SubdivisionTool::SubdivisionType SubdivisionTool::getRecommendedType() const {
    if (isTriangleMesh()) {
        return SubdivisionType::Loop;
    } else if (isQuadMesh()) {
        return SubdivisionType::CatmullClark;
    } else {
        // Mixed topology - Catmull-Clark can handle it better
        return SubdivisionType::CatmullClark;
    }
}

bool SubdivisionTool::isQuadMesh() const {
    if (!m_mesh) return false;
    
    auto faces = m_mesh->getFaces();
    for (auto face : faces) {
        if (face->getVertices().size() != 4) {
            return false;
        }
    }
    
    return !faces.empty();
}

bool SubdivisionTool::isTriangleMesh() const {
    if (!m_mesh) return false;
    
    auto faces = m_mesh->getFaces();
    for (auto face : faces) {
        if (face->getVertices().size() != 3) {
            return false;
        }
    }
    
    return !faces.empty();
}

float SubdivisionTool::calculateMeshComplexity() const {
    if (!m_mesh) return 0.0f;
    
    size_t vertexCount = m_mesh->getVertices().size();
    size_t faceCount = m_mesh->getFaces().size();
    
    return static_cast<float>(vertexCount + faceCount);
}

HalfEdgeMeshPtr SubdivisionTool::applyCatmullClark(HalfEdgeMeshPtr mesh) const {
    if (!mesh) return nullptr;
    
    std::unordered_map<HalfEdgeFacePtr, QVector3D> facePoints;
    std::unordered_map<HalfEdgeEdgePtr, QVector3D> edgePoints;
    std::unordered_map<HalfEdgeVertexPtr, QVector3D> vertexPoints;
    
    // Step 1: Compute face points (centroid of each face)
    computeFacePoints(mesh, facePoints);
    
    // Step 2: Compute edge points
    computeEdgePoints(mesh, facePoints, edgePoints);
    
    // Step 3: Compute new vertex positions
    computeVertexPoints(mesh, facePoints, edgePoints, vertexPoints);
    
    // Step 4: Build new subdivided mesh
    return buildSubdividedMesh(facePoints, edgePoints, vertexPoints);
}

void SubdivisionTool::computeFacePoints(HalfEdgeMeshPtr mesh, 
                                       std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints) const {
    auto faces = mesh->getFaces();
    
    for (auto face : faces) {
        auto vertices = face->getVertices();
        if (vertices.empty()) continue;
        
        QVector3D centroid(0, 0, 0);
        for (auto vertex : vertices) {
            centroid += vertex->getPosition();
        }
        
        centroid /= static_cast<float>(vertices.size());
        facePoints[face] = centroid;
    }
}

void SubdivisionTool::computeEdgePoints(HalfEdgeMeshPtr mesh,
                                       const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                                       std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints) const {
    auto edges = mesh->getEdges();
    std::unordered_set<HalfEdgeEdgePtr> processedEdges;
    
    for (auto edge : edges) {
        // Skip if we've already processed this edge or its twin
        if (processedEdges.find(edge) != processedEdges.end()) {
            continue;
        }
        
        auto twin = edge->getTwin();
        if (twin && processedEdges.find(twin) != processedEdges.end()) {
            continue;
        }
        
        QVector3D edgePoint;
        auto originVertex = edge->getOriginVertex();
        auto targetVertex = edge->getTargetVertex();
        
        if (!originVertex || !targetVertex) continue;
        
        if (isBoundaryEdge(edge)) {
            // For boundary edges, just use midpoint
            edgePoint = (originVertex->getPosition() + targetVertex->getPosition()) * 0.5f;
        } else {
            // For interior edges, use average of edge endpoints and adjacent face points
            auto face1 = edge->getFace();
            auto face2 = twin ? twin->getFace() : nullptr;
            
            edgePoint = (originVertex->getPosition() + targetVertex->getPosition()) * 0.5f;
            
            if (face1 && facePoints.find(face1) != facePoints.end()) {
                edgePoint = (edgePoint + facePoints.at(face1)) * 0.5f;
            }
            
            if (face2 && facePoints.find(face2) != facePoints.end()) {
                edgePoint = (edgePoint + facePoints.at(face2)) * 0.5f;
            }
        }
        
        edgePoints[edge] = edgePoint;
        if (twin) {
            edgePoints[twin] = edgePoint;
        }
        
        processedEdges.insert(edge);
        if (twin) {
            processedEdges.insert(twin);
        }
    }
}

void SubdivisionTool::computeVertexPoints(HalfEdgeMeshPtr mesh,
                                         const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                                         const std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints,
                                         std::unordered_map<HalfEdgeVertexPtr, QVector3D>& vertexPoints) const {
    auto vertices = mesh->getVertices();
    
    for (auto vertex : vertices) {
        if (isBoundaryVertex(vertex)) {
            // For boundary vertices, use different rule
            auto adjacentEdges = vertex->getOutgoingEdges();
            QVector3D newPos = vertex->getPosition();
            
            // Find boundary edges
            std::vector<HalfEdgeEdgePtr> boundaryEdges;
            for (auto edge : adjacentEdges) {
                if (isBoundaryEdge(edge)) {
                    boundaryEdges.push_back(edge);
                }
            }
            
            if (boundaryEdges.size() == 2) {
                // Standard boundary vertex
                QVector3D edgeSum(0, 0, 0);
                for (auto edge : boundaryEdges) {
                    auto it = edgePoints.find(edge);
                    if (it != edgePoints.end()) {
                        edgeSum += it->second;
                    }
                }
                
                if (m_boundaryRule == BoundaryRule::Sharp) {
                    newPos = vertex->getPosition() * 0.5f + edgeSum * 0.25f;
                } else {
                    newPos = vertex->getPosition() * 0.75f + edgeSum * 0.125f;
                }
            }
            
            vertexPoints[vertex] = newPos;
        } else {
            // Interior vertex - use full Catmull-Clark rule
            auto adjacentFaces = vertex->getAdjacentFaces();
            auto adjacentEdges = vertex->getOutgoingEdges();
            
            int n = static_cast<int>(adjacentFaces.size());
            if (n == 0) {
                vertexPoints[vertex] = vertex->getPosition();
                continue;
            }
            
            // Average of adjacent face points
            QVector3D faceAvg(0, 0, 0);
            for (auto face : adjacentFaces) {
                auto it = facePoints.find(face);
                if (it != facePoints.end()) {
                    faceAvg += it->second;
                }
            }
            faceAvg /= static_cast<float>(n);
            
            // Average of adjacent edge midpoints
            QVector3D edgeAvg(0, 0, 0);
            for (auto edge : adjacentEdges) {
                auto targetVertex = edge->getTargetVertex();
                if (targetVertex) {
                    edgeAvg += (vertex->getPosition() + targetVertex->getPosition()) * 0.5f;
                }
            }
            edgeAvg /= static_cast<float>(adjacentEdges.size());
            
            // Catmull-Clark vertex rule
            QVector3D originalPos = vertex->getPosition();
            QVector3D newPos = (faceAvg + edgeAvg * 2.0f + originalPos * static_cast<float>(n - 3)) / static_cast<float>(n);
            
            vertexPoints[vertex] = newPos;
        }
    }
}

HalfEdgeMeshPtr SubdivisionTool::buildSubdividedMesh(
    const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
    const std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints,
    const std::unordered_map<HalfEdgeVertexPtr, QVector3D>& vertexPoints) const {
    
    auto newMesh = std::make_shared<HalfEdgeMesh>();
    
    // Create vertex mapping for the new mesh
    std::unordered_map<HalfEdgeVertexPtr, HalfEdgeVertexPtr> oldToNewVertices;
    std::unordered_map<HalfEdgeFacePtr, HalfEdgeVertexPtr> faceToNewVertex;
    std::unordered_map<HalfEdgeEdgePtr, HalfEdgeVertexPtr> edgeToNewVertex;
    
    // Add original vertices with new positions
    for (const auto& pair : vertexPoints) {
        auto newVertex = newMesh->addVertex(pair.second);
        if (newVertex) {
            newVertex->setNormal(pair.first->getNormal());
            newVertex->setTexCoord(pair.first->getTexCoord());
            oldToNewVertices[pair.first] = newVertex;
        }
    }
    
    // Add face points as vertices
    for (const auto& pair : facePoints) {
        auto newVertex = newMesh->addVertex(pair.second);
        if (newVertex) {
            faceToNewVertex[pair.first] = newVertex;
        }
    }
    
    // Add edge points as vertices
    std::unordered_set<HalfEdgeEdgePtr> processedEdges;
    for (const auto& pair : edgePoints) {
        if (processedEdges.find(pair.first) != processedEdges.end()) {
            continue;
        }
        
        auto newVertex = newMesh->addVertex(pair.second);
        if (newVertex) {
            edgeToNewVertex[pair.first] = newVertex;
            auto twin = pair.first->getTwin();
            if (twin) {
                edgeToNewVertex[twin] = newVertex;
                processedEdges.insert(twin);
            }
        }
        processedEdges.insert(pair.first);
    }
    
    // Create new faces
    createSubdividedFaces(newMesh, facePoints, edgePoints, vertexPoints);
    
    // Update normals
    newMesh->updateNormals();
    
    return newMesh;
}

void SubdivisionTool::createSubdividedFaces(HalfEdgeMeshPtr newMesh,
                                           const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                                           const std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints,
                                           const std::unordered_map<HalfEdgeVertexPtr, QVector3D>& vertexPoints) const {
    // This is a simplified implementation
    // A complete implementation would properly construct all the subdivision faces
    qDebug() << "SubdivisionTool: Face creation in subdivision needs full implementation";
}

HalfEdgeMeshPtr SubdivisionTool::applyLoop(HalfEdgeMeshPtr mesh) const {
    qDebug() << "SubdivisionTool: Loop subdivision not yet fully implemented";
    return nullptr;
}

HalfEdgeMeshPtr SubdivisionTool::applyDooSabin(HalfEdgeMeshPtr mesh) const {
    qDebug() << "SubdivisionTool: Doo-Sabin subdivision not yet implemented";
    return nullptr;
}

bool SubdivisionTool::isCreaseEdge(HalfEdgeEdgePtr edge) const {
    if (!edge || m_boundaryRule != BoundaryRule::CreaseAngle) {
        return false;
    }
    
    float angle = calculateDihedralAngle(edge);
    return angle > m_creaseAngle;
}

bool SubdivisionTool::isBoundaryVertex(HalfEdgeVertexPtr vertex) const {
    if (!vertex) return false;
    return vertex->isBoundary();
}

bool SubdivisionTool::isBoundaryEdge(HalfEdgeEdgePtr edge) const {
    if (!edge) return false;
    return edge->isBoundary();
}

float SubdivisionTool::calculateDihedralAngle(HalfEdgeEdgePtr edge) const {
    if (!edge || !edge->getTwin()) return 0.0f;
    
    auto face1 = edge->getFace();
    auto face2 = edge->getTwin()->getFace();
    
    if (!face1 || !face2) return 0.0f;
    
    QVector3D normal1 = face1->computeNormal();
    QVector3D normal2 = face2->computeNormal();
    
    float dot = QVector3D::dotProduct(normal1, normal2);
    dot = qBound(-1.0f, dot, 1.0f);
    
    return std::acos(dot) * 180.0f / static_cast<float>(M_PI);
}

bool SubdivisionTool::needsSubdivision(HalfEdgeFacePtr face, float threshold) const {
    if (!face) return false;
    
    float error = calculateSubdivisionError(face);
    return error > threshold;
}

float SubdivisionTool::calculateSubdivisionError(HalfEdgeFacePtr face) const {
    if (!face) return 0.0f;
    
    // Use face area as a simple error metric
    return face->getArea();
}

float SubdivisionTool::calculateAspectRatio(HalfEdgeFacePtr face) const {
    if (!face) return 1.0f;
    
    auto edges = face->getEdges();
    if (edges.empty()) return 1.0f;
    
    float minLength = std::numeric_limits<float>::max();
    float maxLength = 0.0f;
    
    for (auto edge : edges) {
        float length = edge->getLength();
        minLength = std::min(minLength, length);
        maxLength = std::max(maxLength, length);
    }
    
    return (minLength > 0.0f) ? (maxLength / minLength) : 1.0f;
}

float SubdivisionTool::calculateTriangleQuality(HalfEdgeFacePtr face) const {
    if (!face) return 0.0f;
    
    auto vertices = face->getVertices();
    if (vertices.size() != 3) return 0.0f;
    
    // Calculate triangle quality based on angles
    // Higher values indicate better quality
    QVector3D v0 = vertices[0]->getPosition();
    QVector3D v1 = vertices[1]->getPosition();
    QVector3D v2 = vertices[2]->getPosition();
    
    QVector3D e0 = (v1 - v0).normalized();
    QVector3D e1 = (v2 - v1).normalized();
    QVector3D e2 = (v0 - v2).normalized();
    
    float angle0 = std::acos(qBound(-1.0f, QVector3D::dotProduct(-e2, e0), 1.0f));
    float angle1 = std::acos(qBound(-1.0f, QVector3D::dotProduct(-e0, e1), 1.0f));
    float angle2 = std::acos(qBound(-1.0f, QVector3D::dotProduct(-e1, e2), 1.0f));
    
    // Quality based on how close angles are to 60 degrees (optimal triangle)
    float optimalAngle = static_cast<float>(M_PI) / 3.0f;
    float deviation = std::abs(angle0 - optimalAngle) + 
                     std::abs(angle1 - optimalAngle) + 
                     std::abs(angle2 - optimalAngle);
    
    return 1.0f / (1.0f + deviation);
}
