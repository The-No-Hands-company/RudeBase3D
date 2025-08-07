#pragma once

#include "core/mesh_iterator.hpp"
#include "core/mesh_forward.hpp"
#include "core/mesh_elements.hpp"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace rude {

class HalfEdgeMesh {
public:
    // Constructor
    HalfEdgeMesh();

    // Iterator types
    using VertexIterator = rude::VertexIterator;
    using EdgeIterator = rude::EdgeIterator;
    using FaceIterator = rude::FaceIterator;
    using HalfEdgeIterator = rude::HalfEdgeIterator;

    // Iterator methods
    VertexIterator vertices_begin() const;
    VertexIterator vertices_end() const;
    EdgeIterator edges_begin() const;
    EdgeIterator edges_end() const;
    FaceIterator faces_begin() const;
    FaceIterator faces_end() const;
    HalfEdgeIterator half_edges_begin() const;
    HalfEdgeIterator half_edges_end() const;

    // Mesh operations
    void clear();  // Clear all mesh data
    bool isEmpty() const; // Check if mesh is empty
    bool isValid() const;  // Check if the mesh structure is valid
    
    // Element access
    ::std::vector<VertexPtr> getVertices() const;
    ::std::vector<EdgePtr> getEdges() const;
    ::std::vector<FacePtr> getFaces() const;
    
    // Geometry operations
    void updateNormals();
    Vec3 computeCentroid() const;
    bool isManifold() const;
    VertexPtr createVertex(const Vec3& position = Vec3(0.0f));
    VertexPtr addVertex(const Vec3& position = Vec3(0.0f));
    EdgePtr createEdge(VertexPtr v1, VertexPtr v2);
    EdgePtr addEdge(VertexPtr v1, VertexPtr v2);
    FacePtr createFace(const ::std::vector<VertexPtr>& vertices);
    FacePtr addFace(const ::std::vector<VertexPtr>& vertices);
    void deleteVertex(VertexPtr vertex);
    void removeVertex(VertexPtr vertex);
    void deleteEdge(EdgePtr edge);
    void removeEdge(EdgePtr edge);
    void deleteFace(FacePtr face);
    void removeFace(FacePtr face);

    // Topology queries
    ::std::vector<VertexPtr> getAdjacentVertices(VertexPtr vertex) const;
    ::std::vector<EdgePtr> getAdjacentEdges(VertexPtr vertex) const;
    ::std::vector<FacePtr> getAdjacentFaces(VertexPtr vertex) const;
    ::std::vector<HalfEdgePtr> getAdjacentHalfEdges(VertexPtr vertex) const;

    // Advanced mesh operations
    void splitEdge(EdgePtr edge, const Vec3& position);
    void mergeVertices(VertexPtr v1, VertexPtr v2);
    void flipEdge(EdgePtr edge);
    bool isValidMesh() const;

    // Additional utility methods
    EdgePtr findEdge(VertexPtr v1, VertexPtr v2) const;
    bool areVerticesConnected(VertexPtr v1, VertexPtr v2) const;
    void updateVertexNormals();

private:
    ::std::vector<VertexPtr> m_vertices;
    ::std::vector<EdgePtr> m_edges;
    ::std::vector<FacePtr> m_faces;
    ::std::vector<HalfEdgePtr> m_halfEdges;
};

} // namespace rude