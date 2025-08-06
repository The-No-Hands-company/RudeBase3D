#include "SubdivisionTool.h"

#include "core/half_edge_mesh.hpp"
#include "core/mesh_elements.hpp"
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <glm/glm.hpp>

SubdivisionTool::SubdivisionTool() {
}

rude::HalfEdgeMeshPtr SubdivisionTool::subdivide(int levels) {
    if (!canSubdivide() || levels <= 0) {
        qWarning() << "SubdivisionTool: Cannot subdivide - invalid parameters";
        return nullptr;
    }
    
    // TODO: Implement subdivision using rude:: API
    qDebug() << "SubdivisionTool: Subdivision not yet fully implemented for rude:: API";
    return nullptr;
    
    // NOTE: All subdivision code temporarily commented out due to API changes
    /*
    auto currentMesh = m_mesh;
    
    for (int level = 0; level < levels; ++level) {
        auto subdividedMesh = std::shared_ptr<rude::HalfEdgeMesh>();
        
        switch (m_subdivisionType) {
            case SubdivisionType::CatmullClark:
                // subdividedMesh = applyCatmullClark(currentMesh);
                break;
                
            case SubdivisionType::Loop:
                // subdividedMesh = applyLoop(currentMesh);
                break;
                
            case SubdivisionType::DooSabin:
                // subdividedMesh = applyDooSabin(currentMesh);
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
    */
}

rude::HalfEdgeMeshPtr SubdivisionTool::subdivideAdaptive(float errorThreshold) {
    if (!canSubdivide()) {
        return nullptr;
    }
    
    // TODO: Implement adaptive subdivision using rude:: API
    qDebug() << "SubdivisionTool: Adaptive subdivision not yet implemented for rude:: API";
    Q_UNUSED(errorThreshold);
    return nullptr;
}

rude::HalfEdgeMeshPtr SubdivisionTool::subdivideRegion(const std::vector<rude::FacePtr>& faces, int levels) {
    // Regional subdivision is complex and would require boundary handling
    qDebug() << "SubdivisionTool: Regional subdivision not yet fully implemented for rude:: API";
    Q_UNUSED(faces);
    Q_UNUSED(levels);
    return nullptr;
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

rude::HalfEdgeMeshPtr SubdivisionTool::applyCatmullClark(rude::HalfEdgeMeshPtr mesh) const {
    if (!mesh) return nullptr;
    
    std::unordered_map<rude::FacePtr, glm::vec3> facePoints;
    std::unordered_map<rude::EdgePtr, glm::vec3> edgePoints;
    std::unordered_map<rude::VertexPtr, glm::vec3> vertexPoints;
    
    // Step 1: Compute face points (centroid of each face)
    computeFacePoints(mesh, facePoints);
    
    // Step 2: Compute edge points
    computeEdgePoints(mesh, facePoints, edgePoints);
    
    // Step 3: Compute new vertex positions
    computeVertexPoints(mesh, facePoints, edgePoints, vertexPoints);
    
    // Step 4: Build new subdivided mesh
    return buildSubdividedMesh(facePoints, edgePoints, vertexPoints);
}

void SubdivisionTool::computeFacePoints(rude::HalfEdgeMeshPtr mesh, 
                                       std::unordered_map<rude::FacePtr, glm::vec3>& facePoints) const {
    auto faces = mesh->getFaces();
    
    for (auto face : faces) {
        auto vertices = face->getVertices();
        if (vertices.empty()) continue;
        
        glm::vec3 centroid(0, 0, 0);
        for (auto vertex : vertices) {
            centroid += vertex->position; // Direct member access, not getPosition()
        }
        centroid /= static_cast<float>(vertices.size());
        facePoints[face] = centroid;
    }
}

void SubdivisionTool::computeEdgePoints(rude::HalfEdgeMeshPtr mesh,
                                       const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
                                       std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints) const {
    
    // TODO: Implement edge points computation using rude:: API
    // This function uses legacy API methods that don't exist in the new rude:: mesh system
    // Commenting out until proper implementation with direct half-edge traversal
    
    Q_UNUSED(mesh);
    Q_UNUSED(facePoints);
    Q_UNUSED(edgePoints);
    
    /*
    auto edges = mesh->getEdges();
    std::unordered_set<rude::EdgePtr> processedEdges;
    
    for (auto edge : edges) {
        // Legacy implementation using methods that don't exist in rude:: API
        // getTwin(), getOriginVertex(), getTargetVertex(), getFace() etc.
        // Need to be replaced with direct half-edge member access
    }
    */
}

void SubdivisionTool::computeVertexPoints(rude::HalfEdgeMeshPtr mesh,
                                         const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
                                         const std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints,
                                         std::unordered_map<rude::VertexPtr, glm::vec3>& vertexPoints) const {
    // TODO: Implement vertex point computation for rude:: API
    // Legacy implementation temporarily commented out due to API changes
    Q_UNUSED(mesh);
    Q_UNUSED(facePoints);
    Q_UNUSED(edgePoints);
    Q_UNUSED(vertexPoints);
}

rude::HalfEdgeMeshPtr SubdivisionTool::buildSubdividedMesh(
    const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
    const std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints,
    const std::unordered_map<rude::VertexPtr, glm::vec3>& vertexPoints) const {
    
    // TODO: Implement subdivision mesh building with rude::HalfEdgeMesh
    // Legacy implementation temporarily commented out due to API changes
    Q_UNUSED(facePoints);
    Q_UNUSED(edgePoints);
    Q_UNUSED(vertexPoints);
    
    qDebug() << "SubdivisionTool: buildSubdividedMesh not yet implemented for rude:: API";
    return nullptr;
}

void SubdivisionTool::createSubdividedFaces(rude::HalfEdgeMeshPtr newMesh,
                                           const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
                                           const std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints,
                                           const std::unordered_map<rude::VertexPtr, glm::vec3>& vertexPoints) const {
    // TODO: Implement subdivision face creation for rude:: API
    // Legacy implementation temporarily commented out due to API changes
    Q_UNUSED(newMesh);
    Q_UNUSED(facePoints);
    Q_UNUSED(edgePoints);
    Q_UNUSED(vertexPoints);
    
    qDebug() << "SubdivisionTool: createSubdividedFaces not yet implemented for rude:: API";
}

rude::HalfEdgeMeshPtr SubdivisionTool::applyLoop(rude::HalfEdgeMeshPtr mesh) const {
    // TODO: Implement Loop subdivision for rude:: API
    Q_UNUSED(mesh);
    qDebug() << "SubdivisionTool: Loop subdivision not yet fully implemented";
    return nullptr;
}

rude::HalfEdgeMeshPtr SubdivisionTool::applyDooSabin(rude::HalfEdgeMeshPtr mesh) const {
    // TODO: Implement Doo-Sabin subdivision for rude:: API
    Q_UNUSED(mesh);
    qDebug() << "SubdivisionTool: Doo-Sabin subdivision not yet implemented";
    return nullptr;
}

bool SubdivisionTool::isCreaseEdge(rude::EdgePtr edge) const {
    if (!edge || m_boundaryRule != BoundaryRule::CreaseAngle) {
        return false;
    }
    
    float angle = calculateDihedralAngle(edge);
    return angle > m_creaseAngle;
}

bool SubdivisionTool::isBoundaryVertex(rude::VertexPtr vertex) const {
    if (!vertex) return false;
    return vertex->isBoundary();
}

bool SubdivisionTool::isBoundaryEdge(rude::EdgePtr edge) const {
    if (!edge) return false;
    return edge->isBoundary();
}

float SubdivisionTool::calculateDihedralAngle(rude::EdgePtr edge) const {
    // TODO: Implement dihedral angle calculation with rude:: API
    // The original implementation used getTwin() and getFace() methods which don't exist
    Q_UNUSED(edge);
    qDebug() << "SubdivisionTool: calculateDihedralAngle not yet implemented for rude:: API";
    return 0.0f;
}

bool SubdivisionTool::needsSubdivision(rude::FacePtr face, float threshold) const {
    if (!face) return false;
    
    float error = calculateSubdivisionError(face);
    return error > threshold;
}

float SubdivisionTool::calculateSubdivisionError(rude::FacePtr face) const {
    if (!face) return 0.0f;
    
    // TODO: Implement area calculation with rude:: API
    // The original implementation used getArea() method which doesn't exist
    Q_UNUSED(face);
    qDebug() << "SubdivisionTool: calculateSubdivisionError not yet implemented for rude:: API";
    return 0.0f;
}

float SubdivisionTool::calculateAspectRatio(rude::FacePtr face) const {
    if (!face) return 1.0f;
    
    auto edges = face->getEdges();
    if (edges.empty()) return 1.0f;
    
    // TODO: Implement edge length calculation with rude:: API
    // The original implementation used getLength() method which doesn't exist
    Q_UNUSED(edges);
    qDebug() << "SubdivisionTool: calculateAspectRatio not yet implemented for rude:: API";
    return 1.0f;
}

float SubdivisionTool::calculateTriangleQuality(rude::FacePtr face) const {
    if (!face) return 0.0f;
    
    auto vertices = face->getVertices();
    if (vertices.size() != 3) return 0.0f;
    
    // Calculate triangle quality based on angles
    // Higher values indicate better quality
    glm::vec3 v0 = vertices[0]->position;
    glm::vec3 v1 = vertices[1]->position;
    glm::vec3 v2 = vertices[2]->position;
    
    glm::vec3 e0 = glm::normalize(v1 - v0);
    glm::vec3 e1 = glm::normalize(v2 - v1);
    glm::vec3 e2 = glm::normalize(v0 - v2);
    
    float angle0 = std::acos(qBound(-1.0f, glm::dot(-e2, e0), 1.0f));
    float angle1 = std::acos(qBound(-1.0f, glm::dot(-e0, e1), 1.0f));
    float angle2 = std::acos(qBound(-1.0f, glm::dot(-e1, e2), 1.0f));
    
    // Quality based on how close angles are to 60 degrees (optimal triangle)
    float optimalAngle = static_cast<float>(M_PI) / 3.0f;
    float deviation = std::abs(angle0 - optimalAngle) + 
                     std::abs(angle1 - optimalAngle) + 
                     std::abs(angle2 - optimalAngle);
    
    return 1.0f / (1.0f + deviation);
}
