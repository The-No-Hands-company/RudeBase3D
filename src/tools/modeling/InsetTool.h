#pragma once

#include "Common.h"
#include "HalfEdgeMesh.h"
#include <QVector3D>
#include <memory>
#include <vector>

/**
 * @brief Tool for insetting faces in half-edge meshes
 * 
 * The InsetTool creates smaller faces inside selected faces, maintaining
 * proper topology and creating connecting faces between the original and
 * inset face boundaries.
 */
class InsetTool {
public:
    enum class InsetMode {
        Individual,  // Inset each face individually
        Region      // Inset selected faces as a region
    };

    InsetTool();
    ~InsetTool() = default;

    // Tool state
    void setMesh(HalfEdgeMeshPtr mesh) { m_mesh = mesh; }
    HalfEdgeMeshPtr getMesh() const { return m_mesh; }

    // Inset operations
    bool insetFaces(const std::vector<HalfEdgeFacePtr>& faces, float insetAmount);
    bool insetFace(HalfEdgeFacePtr face, float insetAmount);
    
    // Tool settings
    void setInsetMode(InsetMode mode) { m_insetMode = mode; }
    InsetMode getInsetMode() const { return m_insetMode; }
    
    void setInsetDepth(float depth) { m_insetDepth = depth; }
    float getInsetDepth() const { return m_insetDepth; }
    
    void setScaleEvenThickness(bool enabled) { m_scaleEvenThickness = enabled; }
    bool getScaleEvenThickness() const { return m_scaleEvenThickness; }

    // Validation
    bool canInset() const;
    
    // Result information
    const std::vector<HalfEdgeFacePtr>& getCreatedFaces() const { return m_createdFaces; }
    const std::vector<HalfEdgeVertexPtr>& getCreatedVertices() const { return m_createdVertices; }

private:
    // Core mesh
    HalfEdgeMeshPtr m_mesh;
    
    // Tool settings
    InsetMode m_insetMode = InsetMode::Individual;
    float m_insetDepth = 0.0f;
    bool m_scaleEvenThickness = false;
    
    // Result tracking
    std::vector<HalfEdgeFacePtr> m_createdFaces;
    std::vector<HalfEdgeVertexPtr> m_createdVertices;
    
    // Internal operations
    HalfEdgeFacePtr insetFaceIndividual(HalfEdgeFacePtr face, float insetAmount);
    std::vector<HalfEdgeFacePtr> insetRegion(const std::vector<HalfEdgeFacePtr>& faces, float insetAmount);
    
    // Helper methods
    QVector3D calculateFaceCenter(HalfEdgeFacePtr face) const;
    QVector3D calculateInsetPosition(HalfEdgeVertexPtr vertex, HalfEdgeFacePtr face, float insetAmount) const;
    std::vector<HalfEdgeVertexPtr> createInsetVertices(HalfEdgeFacePtr face, float insetAmount);
    void createBridgeFaces(const std::vector<HalfEdgeVertexPtr>& originalVertices, 
                          const std::vector<HalfEdgeVertexPtr>& insetVertices);
    
    // Topology helpers
    HalfEdgeVertexPtr duplicateVertex(HalfEdgeVertexPtr vertex, const QVector3D& newPosition);
    HalfEdgeFacePtr createQuadFace(HalfEdgeVertexPtr v1, HalfEdgeVertexPtr v2,
                                   HalfEdgeVertexPtr v3, HalfEdgeVertexPtr v4);
    
    // Cleanup
    void clearResults();
};
