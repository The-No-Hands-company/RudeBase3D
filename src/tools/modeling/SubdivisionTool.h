#pragma once

#include "Common.h"
#include "HalfEdgeMesh.h"
#include <QVector3D>
#include <memory>
#include <vector>
#include <unordered_map>

/**
 * @brief Tool for applying subdivision operations to half-edge meshes
 * 
 * The SubdivisionTool implements various subdivision algorithms including
 * Catmull-Clark for quad meshes and Loop subdivision for triangle meshes.
 * Creates smooth, organic surfaces from base polygon meshes.
 */
class SubdivisionTool {
public:
    enum class SubdivisionType {
        CatmullClark,   // Catmull-Clark subdivision (quads)
        Loop,           // Loop subdivision (triangles)
        DooSabin,       // Doo-Sabin subdivision
        ModifiedButterfly // Modified Butterfly subdivision
    };
    
    enum class BoundaryRule {
        Sharp,          // Keep boundary edges sharp
        Smooth,         // Smooth boundary edges
        CreaseAngle     // Use crease angle threshold
    };

    SubdivisionTool();
    ~SubdivisionTool() = default;

    // Tool state
    void setMesh(HalfEdgeMeshPtr mesh) { m_mesh = mesh; }
    HalfEdgeMeshPtr getMesh() const { return m_mesh; }

    // Subdivision operations
    HalfEdgeMeshPtr subdivide(int levels = 1);
    HalfEdgeMeshPtr subdivideAdaptive(float errorThreshold = 0.1f);
    HalfEdgeMeshPtr subdivideRegion(const std::vector<HalfEdgeFacePtr>& faces, int levels = 1);
    
    // Tool settings
    void setSubdivisionType(SubdivisionType type) { m_subdivisionType = type; }
    SubdivisionType getSubdivisionType() const { return m_subdivisionType; }
    
    void setBoundaryRule(BoundaryRule rule) { m_boundaryRule = rule; }
    BoundaryRule getBoundaryRule() const { return m_boundaryRule; }
    
    void setCreaseAngle(float angleInDegrees) { m_creaseAngle = angleInDegrees; }
    float getCreaseAngle() const { return m_creaseAngle; }
    
    void setPreserveBoundary(bool preserve) { m_preserveBoundary = preserve; }
    bool getPreserveBoundary() const { return m_preserveBoundary; }
    
    void setUseQuadraticErrorMetric(bool use) { m_useQEM = use; }
    bool getUseQuadraticErrorMetric() const { return m_useQEM; }

    // Validation
    bool canSubdivide() const;
    SubdivisionType getRecommendedType() const;
    
    // Analysis
    bool isQuadMesh() const;
    bool isTriangleMesh() const;
    float calculateMeshComplexity() const;

private:
    // Core mesh
    HalfEdgeMeshPtr m_mesh;
    
    // Tool settings
    SubdivisionType m_subdivisionType = SubdivisionType::CatmullClark;
    BoundaryRule m_boundaryRule = BoundaryRule::Sharp;
    float m_creaseAngle = 30.0f;
    bool m_preserveBoundary = true;
    bool m_useQEM = false;
    
    // Catmull-Clark subdivision
    HalfEdgeMeshPtr applyCatmullClark(HalfEdgeMeshPtr mesh) const;
    void computeFacePoints(HalfEdgeMeshPtr mesh, 
                          std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints) const;
    void computeEdgePoints(HalfEdgeMeshPtr mesh,
                          const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                          std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints) const;
    void computeVertexPoints(HalfEdgeMeshPtr mesh,
                            const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                            const std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints,
                            std::unordered_map<HalfEdgeVertexPtr, QVector3D>& vertexPoints) const;
    
    // Loop subdivision  
    HalfEdgeMeshPtr applyLoop(HalfEdgeMeshPtr mesh) const;
    QVector3D computeLoopVertexPoint(HalfEdgeVertexPtr vertex) const;
    QVector3D computeLoopEdgePoint(HalfEdgeEdgePtr edge) const;
    
    // Doo-Sabin subdivision
    HalfEdgeMeshPtr applyDooSabin(HalfEdgeMeshPtr mesh) const;
    
    // Helper methods
    bool isCreaseEdge(HalfEdgeEdgePtr edge) const;
    bool isBoundaryVertex(HalfEdgeVertexPtr vertex) const;
    bool isBoundaryEdge(HalfEdgeEdgePtr edge) const;
    float calculateDihedralAngle(HalfEdgeEdgePtr edge) const;
    
    // Mesh construction helpers
    HalfEdgeMeshPtr buildSubdividedMesh(
        const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
        const std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints,
        const std::unordered_map<HalfEdgeVertexPtr, QVector3D>& vertexPoints) const;
    
    void createSubdividedFaces(HalfEdgeMeshPtr newMesh,
                              const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                              const std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints,
                              const std::unordered_map<HalfEdgeVertexPtr, QVector3D>& vertexPoints) const;
    
    // Adaptive subdivision helpers
    float calculateSubdivisionError(HalfEdgeFacePtr face) const;
    bool needsSubdivision(HalfEdgeFacePtr face, float threshold) const;
    
    // Quality metrics
    float calculateAspectRatio(HalfEdgeFacePtr face) const;
    float calculateTriangleQuality(HalfEdgeFacePtr face) const;
};
