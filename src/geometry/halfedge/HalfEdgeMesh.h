#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include "core/mesh_forward.hpp"

// Forward declarations
class HalfEdgeMesh;

// Smart pointer types
// Removed HalfEdgeMeshPtr typedef to avoid conflict with Common.h
// using HalfEdgeMeshPtr = std::shared_ptr<HalfEdgeMesh>;

// Weak pointer types for avoiding circular references
// Removed VertexWeak, EdgeWeak, FaceWeak, HalfEdgeWeak typedefs.

/**
 * @brief Half-Edge Mesh
 * Main container for the half-edge data structure
 */
class HalfEdgeMesh {
public:
    HalfEdgeMesh();
    ~HalfEdgeMesh() = default;

    // Basic operations
    void clear();
    bool isEmpty() const;
    
    // Element creation
    rude::VertexPtr addVertex(const glm::vec3& position);
    rude::EdgePtr addEdge(rude::VertexPtr origin, rude::VertexPtr target);
    rude::FacePtr addFace(const std::vector<rude::VertexPtr>& vertices);
    
    // Element removal
    bool removeVertex(rude::VertexPtr vertex);
    bool removeEdge(rude::EdgePtr edge);
    bool removeFace(rude::FacePtr face);
    
    // Element access
    const std::vector<rude::VertexPtr>& getVertices() const { return m_vertices; }
    const std::vector<rude::EdgePtr>& getEdges() const { return m_edges; }
    const std::vector<rude::FacePtr>& getFaces() const { return m_faces; }
    
    rude::VertexPtr getVertex(unsigned int id) const;
    rude::EdgePtr getEdge(unsigned int id) const;
    rude::FacePtr getFace(unsigned int id) const;
    
    // Counts
    size_t getVertexCount() const { return m_vertices.size(); }
    size_t getEdgeCount() const { return m_edges.size(); }
    size_t getFaceCount() const { return m_faces.size(); }
    
    // Selection
    std::vector<rude::VertexPtr> getSelectedVertices() const;
    std::vector<rude::EdgePtr> getSelectedEdges() const;
    std::vector<rude::FacePtr> getSelectedFaces() const;
    
    void clearSelection();
    void selectAll();
    void invertSelection();
    
    // Validation and repair
    bool validate() const;
    bool repair();
    
    // Mesh properties
    glm::vec3 getBoundingBoxMin() const;
    glm::vec3 getBoundingBoxMax() const;
    glm::vec3 getCentroid() const;
    
    // Conversion utilities
    void updateNormals();
    void updateTextureCoordinates();
    
    // Topology queries
    bool isManifold() const;
    bool isClosed() const;
    int getEulerCharacteristic() const; // V - E + F
    std::vector<rude::VertexPtr> getBoundaryVertices() const;
    std::vector<rude::EdgePtr> getBoundaryEdges() const;

private:
    // Element storage
    std::vector<rude::VertexPtr> m_vertices;
    std::vector<rude::EdgePtr> m_edges;
    std::vector<rude::FacePtr> m_faces;
    
    // ID maps for fast lookup
    std::unordered_map<unsigned int, rude::VertexPtr> m_vertexMap;
    std::unordered_map<unsigned int, rude::EdgePtr> m_edgeMap;
    std::unordered_map<unsigned int, rude::FacePtr> m_faceMap;
    
    // ID generators
    unsigned int m_nextVertexId = 1;
    unsigned int m_nextEdgeId = 1;
    unsigned int m_nextFaceId = 1;
    
    // Helper methods
    void connectEdges(rude::EdgePtr edge1, rude::EdgePtr edge2);
    rude::EdgePtr findEdge(rude::VertexPtr origin, rude::VertexPtr target) const;
    void updateConnectivity();
};

// Utility functions for working with half-edge meshes
namespace HalfEdgeUtils {
    // Edge operations
    std::pair<rude::EdgePtr, rude::EdgePtr> splitEdge(rude::HalfEdgeMeshPtr mesh, rude::EdgePtr edge, const glm::vec3& position);
    bool collapseEdge(rude::HalfEdgeMeshPtr mesh, rude::EdgePtr edge);
    bool flipEdge(rude::EdgePtr edge);
    
    // Face operations
    std::vector<rude::FacePtr> extrudeFaces(rude::HalfEdgeMeshPtr mesh, const std::vector<rude::FacePtr>& faces, float distance);
    std::vector<rude::FacePtr> insetFaces(rude::HalfEdgeMeshPtr mesh, const std::vector<rude::FacePtr>& faces, float inset);
    std::vector<rude::EdgePtr> bevelEdges(rude::HalfEdgeMeshPtr mesh, const std::vector<rude::EdgePtr>& edges, float amount);
    
    // Loop operations
    std::vector<rude::EdgePtr> getEdgeLoop(rude::EdgePtr startEdge);
    std::vector<rude::EdgePtr> getEdgeRing(rude::EdgePtr startEdge);
    
    // Subdivision
    rude::HalfEdgeMeshPtr catmullClarkSubdivide(rude::HalfEdgeMeshPtr mesh);
    rude::HalfEdgeMeshPtr loopSubdivide(rude::HalfEdgeMeshPtr mesh);
    
    // Mesh analysis
    std::vector<std::vector<rude::VertexPtr>> findConnectedComponents(rude::HalfEdgeMeshPtr mesh);
    std::vector<rude::EdgePtr> findNonManifoldEdges(rude::HalfEdgeMeshPtr mesh);
    std::vector<rude::VertexPtr> findNonManifoldVertices(rude::HalfEdgeMeshPtr mesh);
}
