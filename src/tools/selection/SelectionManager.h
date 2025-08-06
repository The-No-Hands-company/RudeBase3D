#pragma once

#include "Common.h"

#include <glm/glm.hpp>
#include <QRect>
#include <unordered_set>
#include <vector>
#include <memory>

/**
 * @brief Manages mesh element selection for interactive editing
 * 
 * The SelectionManager handles vertex, edge, and face selection in HEDS meshes,
 * providing tools for single selection, multi-selection, and box selection.
 */
class SelectionManager {
public:
    SelectionManager();
    ~SelectionManager() = default;

    // Selection mode
    void setSelectionType(SelectionType type) { m_selectionType = type; }
    SelectionType getSelectionType() const { return m_selectionType; }

    // Current mesh
    void setMesh(HalfEdgeMeshPtr mesh);
    HalfEdgeMeshPtr getMesh() const { return m_mesh; }

    // Selection operations
    void clearSelection();
    void selectAll();
    void invertSelection();

    // Point selection (from mouse picking)
    bool selectAtPoint(const glm::vec3& worldPos, bool addToSelection = false);
    
    // Box selection
    void beginBoxSelection(const glm::vec3& startPos);
    void updateBoxSelection(const glm::vec3& currentPos);
    void endBoxSelection(bool addToSelection = false);
    bool isBoxSelecting() const { return m_boxSelecting; }
    std::pair<glm::vec2, glm::vec2> getSelectionBox() const { return m_selectionBox; }

    // Selection queries
    bool hasSelection() const;
    size_t getSelectionCount() const;
    
    // Get selected elements
    std::vector<HalfEdgeVertexPtr> getSelectedVertices() const;
    std::vector<HalfEdgeEdgePtr> getSelectedEdges() const;
    std::vector<HalfEdgeFacePtr> getSelectedFaces() const;

    // Selection state of individual elements
    bool isSelected(HalfEdgeVertexPtr vertex) const;
    bool isSelected(HalfEdgeEdgePtr edge) const;
    bool isSelected(HalfEdgeFacePtr face) const;

    // Manual selection of elements
    void selectVertex(HalfEdgeVertexPtr vertex, bool selected = true);
    void selectEdge(HalfEdgeEdgePtr edge, bool selected = true);
    void selectFace(HalfEdgeFacePtr face, bool selected = true);

    // Selection conversion (e.g., convert vertex selection to adjacent faces)
    void convertSelection(SelectionType toType);

    // Ray-mesh intersection for picking
    struct RayHit {
        bool hit = false;
        glm::vec3 point;
        float distance = 0.0f;
        HalfEdgeVertexPtr vertex;
        HalfEdgeEdgePtr edge;
        HalfEdgeFacePtr face;
    };
    
    RayHit raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const;

private:
    SelectionType m_selectionType = SelectionType::Vertex;
    HalfEdgeMeshPtr m_mesh;
    
    // Box selection state
    bool m_boxSelecting = false;
    glm::vec3 m_boxStart;
    glm::vec3 m_boxEnd;
    std::pair<glm::vec2, glm::vec2> m_selectionBox;
    
    // Selection sets for performance
    std::unordered_set<HalfEdgeVertexPtr> m_selectedVertices;
    std::unordered_set<HalfEdgeEdgePtr> m_selectedEdges;
    std::unordered_set<HalfEdgeFacePtr> m_selectedFaces;
    
    // Helper methods
    void updateSelectionVisualization();
    HalfEdgeVertexPtr findClosestVertex(const glm::vec3& point, float maxDistance = 0.1f) const;
    HalfEdgeEdgePtr findClosestEdge(const glm::vec3& point, float maxDistance = 0.1f) const;
    HalfEdgeFacePtr findClosestFace(const glm::vec3& point) const;
    
    // Ray-triangle intersection
    bool rayTriangleIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                              const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                              float& t, glm::vec3& hitPoint) const;
};
