#pragma once

#include "Common.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>

// Forward declarations
class HalfEdgeVertex;
class HalfEdgeEdge;
class HalfEdgeFace;
class HalfEdgeMesh;

// Smart pointer types
using HalfEdgeVertexPtr = std::shared_ptr<HalfEdgeVertex>;
using HalfEdgeEdgePtr = std::shared_ptr<HalfEdgeEdge>;
using HalfEdgeFacePtr = std::shared_ptr<HalfEdgeFace>;
using HalfEdgeMeshPtr = std::shared_ptr<HalfEdgeMesh>;

// Weak pointer types for avoiding circular references
using HalfEdgeVertexWeak = std::weak_ptr<HalfEdgeVertex>;
using HalfEdgeEdgeWeak = std::weak_ptr<HalfEdgeEdge>;
using HalfEdgeFaceWeak = std::weak_ptr<HalfEdgeFace>;

/**
 * @brief Half-Edge Vertex
 * Represents a vertex in the half-edge data structure
 */
class HalfEdgeVertex {
public:
    explicit HalfEdgeVertex(const QVector3D& position = QVector3D(), unsigned int id = 0);
    ~HalfEdgeVertex() = default;

    // Position and properties
    void setPosition(const QVector3D& position) { m_position = position; }
    const QVector3D& getPosition() const { return m_position; }
    
    void setNormal(const QVector3D& normal) { m_normal = normal; }
    const QVector3D& getNormal() const { return m_normal; }
    
    void setTexCoord(const QVector2D& texCoord) { m_texCoord = texCoord; }
    const QVector2D& getTexCoord() const { return m_texCoord; }

    // Identification
    unsigned int getId() const { return m_id; }
    void setId(unsigned int id) { m_id = id; }

    // Half-edge connectivity
    void setOutgoingEdge(HalfEdgeEdgePtr edge) { m_outgoingEdge = edge; }
    HalfEdgeEdgePtr getOutgoingEdge() const { return m_outgoingEdge.lock(); }

    // Selection and properties
    void setSelected(bool selected) { m_selected = selected; }
    bool isSelected() const { return m_selected; }
    
    void setMarked(bool marked) { m_marked = marked; }
    bool isMarked() const { return m_marked; }

    // Topology queries
    std::vector<HalfEdgeEdgePtr> getIncomingEdges() const;
    std::vector<HalfEdgeEdgePtr> getOutgoingEdges() const;
    std::vector<HalfEdgeVertexPtr> getAdjacentVertices() const;
    std::vector<HalfEdgeFacePtr> getAdjacentFaces() const;
    int getValence() const; // Number of adjacent edges
    bool isBoundary() const; // Check if vertex is on mesh boundary

private:
    QVector3D m_position;
    QVector3D m_normal;
    QVector2D m_texCoord;
    unsigned int m_id;
    HalfEdgeEdgeWeak m_outgoingEdge; // One outgoing half-edge
    bool m_selected = false;
    bool m_marked = false;
};

/**
 * @brief Half-Edge Edge
 * Represents a directed edge in the half-edge data structure
 */
class HalfEdgeEdge {
public:
    explicit HalfEdgeEdge(unsigned int id = 0);
    ~HalfEdgeEdge() = default;

    // Identification
    unsigned int getId() const { return m_id; }
    void setId(unsigned int id) { m_id = id; }

    // Half-edge connectivity
    void setOriginVertex(HalfEdgeVertexPtr vertex) { m_originVertex = vertex; }
    HalfEdgeVertexPtr getOriginVertex() const { return m_originVertex.lock(); }
    
    void setTargetVertex(HalfEdgeVertexPtr vertex) { m_targetVertex = vertex; }
    HalfEdgeVertexPtr getTargetVertex() const { return m_targetVertex.lock(); }

    void setTwin(HalfEdgeEdgePtr twin) { m_twin = twin; }
    HalfEdgeEdgePtr getTwin() const { return m_twin.lock(); }

    void setNext(HalfEdgeEdgePtr next) { m_next = next; }
    HalfEdgeEdgePtr getNext() const { return m_next.lock(); }

    void setPrev(HalfEdgeEdgePtr prev) { m_prev = prev; }
    HalfEdgeEdgePtr getPrev() const { return m_prev.lock(); }

    void setFace(HalfEdgeFacePtr face) { m_face = face; }
    HalfEdgeFacePtr getFace() const { return m_face.lock(); }

    // Selection and properties
    void setSelected(bool selected) { m_selected = selected; }
    bool isSelected() const { return m_selected; }
    
    void setMarked(bool marked) { m_marked = marked; }
    bool isMarked() const { return m_marked; }

    // Topology queries
    bool isBoundary() const; // Check if edge is on mesh boundary
    QVector3D getVector() const; // Vector from origin to target
    float getLength() const;
    QVector3D getMidpoint() const;

private:
    unsigned int m_id;
    HalfEdgeVertexWeak m_originVertex; // Start vertex
    HalfEdgeVertexWeak m_targetVertex; // End vertex (for convenience, derived from next->origin)
    HalfEdgeEdgeWeak m_twin;           // Opposite half-edge
    HalfEdgeEdgeWeak m_next;           // Next half-edge in face loop
    HalfEdgeEdgeWeak m_prev;           // Previous half-edge in face loop
    HalfEdgeFaceWeak m_face;           // Adjacent face (null for boundary edges)
    bool m_selected = false;
    bool m_marked = false;
};

/**
 * @brief Half-Edge Face
 * Represents a face in the half-edge data structure
 */
class HalfEdgeFace {
public:
    explicit HalfEdgeFace(unsigned int id = 0);
    ~HalfEdgeFace() = default;

    // Identification
    unsigned int getId() const { return m_id; }
    void setId(unsigned int id) { m_id = id; }

    // Half-edge connectivity
    void setOuterEdge(HalfEdgeEdgePtr edge) { m_outerEdge = edge; }
    HalfEdgeEdgePtr getOuterEdge() const { return m_outerEdge.lock(); }

    // Properties
    void setNormal(const QVector3D& normal) { m_normal = normal; }
    const QVector3D& getNormal() const { return m_normal; }

    // Selection and properties
    void setSelected(bool selected) { m_selected = selected; }
    bool isSelected() const { return m_selected; }
    
    void setMarked(bool marked) { m_marked = marked; }
    bool isMarked() const { return m_marked; }

    // Topology queries
    std::vector<HalfEdgeEdgePtr> getEdges() const;
    std::vector<HalfEdgeVertexPtr> getVertices() const;
    std::vector<HalfEdgeFacePtr> getAdjacentFaces() const;
    int getVertexCount() const;
    QVector3D computeNormal() const;
    QVector3D getCentroid() const;
    float getArea() const;

private:
    unsigned int m_id;
    HalfEdgeEdgeWeak m_outerEdge; // One edge of the face boundary
    QVector3D m_normal;
    bool m_selected = false;
    bool m_marked = false;
};

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
    HalfEdgeVertexPtr addVertex(const QVector3D& position);
    HalfEdgeEdgePtr addEdge(HalfEdgeVertexPtr origin, HalfEdgeVertexPtr target);
    HalfEdgeFacePtr addFace(const std::vector<HalfEdgeVertexPtr>& vertices);
    
    // Element removal
    bool removeVertex(HalfEdgeVertexPtr vertex);
    bool removeEdge(HalfEdgeEdgePtr edge);
    bool removeFace(HalfEdgeFacePtr face);
    
    // Element access
    const std::vector<HalfEdgeVertexPtr>& getVertices() const { return m_vertices; }
    const std::vector<HalfEdgeEdgePtr>& getEdges() const { return m_edges; }
    const std::vector<HalfEdgeFacePtr>& getFaces() const { return m_faces; }
    
    HalfEdgeVertexPtr getVertex(unsigned int id) const;
    HalfEdgeEdgePtr getEdge(unsigned int id) const;
    HalfEdgeFacePtr getFace(unsigned int id) const;
    
    // Counts
    size_t getVertexCount() const { return m_vertices.size(); }
    size_t getEdgeCount() const { return m_edges.size(); }
    size_t getFaceCount() const { return m_faces.size(); }
    
    // Selection
    std::vector<HalfEdgeVertexPtr> getSelectedVertices() const;
    std::vector<HalfEdgeEdgePtr> getSelectedEdges() const;
    std::vector<HalfEdgeFacePtr> getSelectedFaces() const;
    
    void clearSelection();
    void selectAll();
    void invertSelection();
    
    // Validation and repair
    bool validate() const;
    bool repair();
    
    // Mesh properties
    QVector3D getBoundingBoxMin() const;
    QVector3D getBoundingBoxMax() const;
    QVector3D getCentroid() const;
    
    // Conversion utilities
    void updateNormals();
    void updateTextureCoordinates();
    
    // Topology queries
    bool isManifold() const;
    bool isClosed() const;
    int getEulerCharacteristic() const; // V - E + F
    std::vector<HalfEdgeVertexPtr> getBoundaryVertices() const;
    std::vector<HalfEdgeEdgePtr> getBoundaryEdges() const;

private:
    // Element storage
    std::vector<HalfEdgeVertexPtr> m_vertices;
    std::vector<HalfEdgeEdgePtr> m_edges;
    std::vector<HalfEdgeFacePtr> m_faces;
    
    // ID maps for fast lookup
    std::unordered_map<unsigned int, HalfEdgeVertexPtr> m_vertexMap;
    std::unordered_map<unsigned int, HalfEdgeEdgePtr> m_edgeMap;
    std::unordered_map<unsigned int, HalfEdgeFacePtr> m_faceMap;
    
    // ID generators
    unsigned int m_nextVertexId = 1;
    unsigned int m_nextEdgeId = 1;
    unsigned int m_nextFaceId = 1;
    
    // Helper methods
    void connectEdges(HalfEdgeEdgePtr edge1, HalfEdgeEdgePtr edge2);
    HalfEdgeEdgePtr findEdge(HalfEdgeVertexPtr origin, HalfEdgeVertexPtr target) const;
    void updateConnectivity();
};

// Utility functions for working with half-edge meshes
namespace HalfEdgeUtils {
    // Edge operations
    std::pair<HalfEdgeEdgePtr, HalfEdgeEdgePtr> splitEdge(HalfEdgeMeshPtr mesh, HalfEdgeEdgePtr edge, const QVector3D& position);
    bool collapseEdge(HalfEdgeMeshPtr mesh, HalfEdgeEdgePtr edge);
    bool flipEdge(HalfEdgeEdgePtr edge);
    
    // Face operations
    std::vector<HalfEdgeFacePtr> extrudeFaces(HalfEdgeMeshPtr mesh, const std::vector<HalfEdgeFacePtr>& faces, float distance);
    std::vector<HalfEdgeFacePtr> insetFaces(HalfEdgeMeshPtr mesh, const std::vector<HalfEdgeFacePtr>& faces, float inset);
    std::vector<HalfEdgeEdgePtr> bevelEdges(HalfEdgeMeshPtr mesh, const std::vector<HalfEdgeEdgePtr>& edges, float amount);
    
    // Loop operations
    std::vector<HalfEdgeEdgePtr> getEdgeLoop(HalfEdgeEdgePtr startEdge);
    std::vector<HalfEdgeEdgePtr> getEdgeRing(HalfEdgeEdgePtr startEdge);
    
    // Subdivision
    HalfEdgeMeshPtr catmullClarkSubdivide(HalfEdgeMeshPtr mesh);
    HalfEdgeMeshPtr loopSubdivide(HalfEdgeMeshPtr mesh);
    
    // Mesh analysis
    std::vector<std::vector<HalfEdgeVertexPtr>> findConnectedComponents(HalfEdgeMeshPtr mesh);
    std::vector<HalfEdgeEdgePtr> findNonManifoldEdges(HalfEdgeMeshPtr mesh);
    std::vector<HalfEdgeVertexPtr> findNonManifoldVertices(HalfEdgeMeshPtr mesh);
}
