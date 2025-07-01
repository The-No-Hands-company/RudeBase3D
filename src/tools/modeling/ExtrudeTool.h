#pragma once

#include "Common.h"
#include "HalfEdgeMesh.h"
#include "SelectionManager.h"
#include <QVector3D>
#include <memory>
#include <vector>

/**
 * @brief Tool for extruding faces, edges, and vertices in HEDS meshes
 * 
 * The ExtrudeTool provides interactive mesh editing by extruding selected
 * elements along their normals or a specified direction.
 */
class ExtrudeTool {
public:
    enum class ExtrudeMode {
        Normal,     // Extrude along element normals
        Direction,  // Extrude in a specific direction
        Individual  // Extrude each element individually
    };

    ExtrudeTool();
    ~ExtrudeTool() = default;

    // Tool state
    void setMesh(HalfEdgeMeshPtr mesh);
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);
    
    // Extrude operations
    bool beginExtrude();
    void updateExtrude(float distance);
    void updateExtrude(const QVector3D& direction, float distance);
    void confirmExtrude();
    void cancelExtrude();
    
    // Tool settings
    void setExtrudeMode(ExtrudeMode mode) { m_extrudeMode = mode; }
    ExtrudeMode getExtrudeMode() const { return m_extrudeMode; }
    
    void setExtrudeDirection(const QVector3D& direction) { m_extrudeDirection = direction; }
    QVector3D getExtrudeDirection() const { return m_extrudeDirection; }
    
    // Tool state queries
    bool isExtruding() const { return m_isExtruding; }
    bool canExtrude() const;
    
    // Get preview data for rendering
    struct ExtrudePreview {
        std::vector<QVector3D> previewVertices;
        std::vector<unsigned int> previewIndices;
        std::vector<QVector3D> extrudeVectors;
    };
    
    ExtrudePreview getPreview() const { return m_preview; }

private:
    HalfEdgeMeshPtr m_mesh;
    std::shared_ptr<SelectionManager> m_selectionManager;
    
    ExtrudeMode m_extrudeMode = ExtrudeMode::Normal;
    QVector3D m_extrudeDirection = QVector3D(0, 1, 0);
    
    bool m_isExtruding = false;
    float m_currentDistance = 0.0f;
    
    // Backup data for cancellation
    struct OriginalData {
        std::vector<QVector3D> originalPositions;
        std::vector<unsigned int> originalVertexIds;
        std::vector<unsigned int> newVertexIds;
        std::vector<unsigned int> newFaceIds;
    };
    OriginalData m_originalData;
    
    ExtrudePreview m_preview;
    
    // Internal operations
    bool extrudeFaces(const std::vector<HalfEdgeFacePtr>& faces);
    bool extrudeEdges(const std::vector<HalfEdgeEdgePtr>& edges);
    bool extrudeVertices(const std::vector<HalfEdgeVertexPtr>& vertices);
    
    // Helper methods
    QVector3D calculateFaceNormal(HalfEdgeFacePtr face) const;
    QVector3D calculateEdgeNormal(HalfEdgeEdgePtr edge) const;
    QVector3D calculateVertexNormal(HalfEdgeVertexPtr vertex) const;
    
    void createExtrudedGeometry();
    void updatePreview();
    void restoreOriginalGeometry();
    
    // HEDS topology operations
    HalfEdgeVertexPtr duplicateVertex(HalfEdgeVertexPtr vertex);
    HalfEdgeFacePtr createQuadFace(HalfEdgeVertexPtr v1, HalfEdgeVertexPtr v2, 
                                   HalfEdgeVertexPtr v3, HalfEdgeVertexPtr v4);
    void bridgeEdges(HalfEdgeEdgePtr edge1, HalfEdgeEdgePtr edge2);
};
