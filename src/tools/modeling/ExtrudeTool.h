#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "core/mesh_forward.hpp"

namespace rude {
class Face;
class Vertex;
class Edge;
class HalfEdgeMesh;
using FacePtr = std::shared_ptr<Face>;
using VertexPtr = std::shared_ptr<Vertex>;
using EdgePtr = std::shared_ptr<Edge>;
using HalfEdgeMeshPtr = std::shared_ptr<HalfEdgeMesh>;
}

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
    void setMesh(rude::HalfEdgeMeshPtr mesh);
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);
    
    // Extrude operations
    bool beginExtrude();
    void updateExtrude(float distance);
    void updateExtrude(const glm::vec3& direction, float distance);
    void confirmExtrude();
    void cancelExtrude();
    
    // Tool settings
    void setExtrudeMode(ExtrudeMode mode) { m_extrudeMode = mode; }
    ExtrudeMode getExtrudeMode() const { return m_extrudeMode; }
    
    void setExtrudeDirection(const glm::vec3& direction) { m_extrudeDirection = direction; }
    glm::vec3 getExtrudeDirection() const { return m_extrudeDirection; }
    
    // Tool state queries
    bool isExtruding() const { return m_isExtruding; }
    bool canExtrude() const;
    
    // Get preview data for rendering
    struct ExtrudePreview {
        std::vector<glm::vec3> previewVertices;
        std::vector<unsigned int> previewIndices;
        std::vector<glm::vec3> extrudeVectors;
    };
    
    ExtrudePreview getPreview() const { return m_preview; }

private:
    rude::HalfEdgeMeshPtr m_mesh;
    std::shared_ptr<SelectionManager> m_selectionManager;
    
    ExtrudeMode m_extrudeMode = ExtrudeMode::Normal;
    glm::vec3 m_extrudeDirection = glm::vec3(0, 1, 0);
    
    bool m_isExtruding = false;
    float m_currentDistance = 0.0f;
    
    // Backup data for cancellation
    struct OriginalData {
        std::vector<glm::vec3> originalPositions;
        std::vector<unsigned int> originalVertexIds;
        std::vector<unsigned int> newVertexIds;
        std::vector<unsigned int> newFaceIds;
    };
    OriginalData m_originalData;
    
    ExtrudePreview m_preview;
    
    // Internal operations
    bool extrudeFaces(const std::vector<rude::FacePtr>& faces);
    bool extrudeEdges(const std::vector<rude::EdgePtr>& edges);
    bool extrudeVertices(const std::vector<rude::VertexPtr>& vertices);
    
    // Helper methods
    glm::vec3 calculateFaceNormal(rude::FacePtr face) const;
    glm::vec3 calculateEdgeNormal(rude::EdgePtr edge) const;
    glm::vec3 calculateVertexNormal(rude::VertexPtr vertex) const;
    
    void createExtrudedGeometry();
    void updatePreview();
    void restoreOriginalGeometry();
    
    // HEDS topology operations
    rude::VertexPtr duplicateVertex(rude::VertexPtr vertex);
    rude::FacePtr createQuadFace(rude::VertexPtr v1, rude::VertexPtr v2, 
                                   rude::VertexPtr v3, rude::VertexPtr v4);
    void bridgeEdges(rude::EdgePtr edge1, rude::EdgePtr edge2);
};
