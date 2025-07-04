#pragma once

#include "core/mesh_elements.hpp"
#include "core/mesh_forward.hpp"
#include <vector>
#include <memory>

namespace rude {

/**
 * @brief A half-edge data structure for mesh topology operations
 * 
 * This class implements a half-edge data structure for efficient
 * mesh topology operations. It's different from Mesh which is used
 * primarily for rendering.
 */
class HalfEdgeMesh {
public:
    // Constructor/destructor
    HalfEdgeMesh();
    ~HalfEdgeMesh() = default;
    
    // Geometry operations
    void clear();
    bool isEmpty() const;
    
    // Element access
    std::vector<VertexPtr> getVertices() const;
    std::vector<EdgePtr> getEdges() const;
    std::vector<FacePtr> getFaces() const;
    
    // Mesh operations
    void updateNormals();
    
    // Utility methods
    Vec3 computeCentroid() const;
    bool isManifold() const;

    // Iterators
    class vertex_iterator {
    public:
        vertex_iterator(std::vector<VertexPtr>::iterator it) : m_it(it) {}
        VertexPtr operator*() { return *m_it; }
        vertex_iterator& operator++() { ++m_it; return *this; }
        bool operator!=(const vertex_iterator& other) const { return m_it != other.m_it; }
    private:
        std::vector<VertexPtr>::iterator m_it;
    };

    class edge_iterator {
    public:
        edge_iterator(std::vector<EdgePtr>::iterator it) : m_it(it) {}
        EdgePtr operator*() { return *m_it; }
        edge_iterator& operator++() { ++m_it; return *this; }
        bool operator!=(const edge_iterator& other) const { return m_it != other.m_it; }
    private:
        std::vector<EdgePtr>::iterator m_it;
    };

    class face_iterator {
    public:
        face_iterator(std::vector<FacePtr>::iterator it) : m_it(it) {}
        FacePtr operator*() { return *m_it; }
        face_iterator& operator++() { ++m_it; return *this; }
        bool operator!=(const face_iterator& other) const { return m_it != other.m_it; }
    private:
        std::vector<FacePtr>::iterator m_it;
    };

    vertex_iterator vertices_begin() { return vertex_iterator(m_vertices.begin()); }
    vertex_iterator vertices_end() { return vertex_iterator(m_vertices.end()); }
    edge_iterator edges_begin() { return edge_iterator(m_edges.begin()); }
    edge_iterator edges_end() { return edge_iterator(m_edges.end()); }
    face_iterator faces_begin() { return face_iterator(m_faces.begin()); }
    face_iterator faces_end() { return face_iterator(m_faces.end()); }

private:
    std::vector<VertexPtr> m_vertices;
    std::vector<EdgePtr> m_edges;
    std::vector<FacePtr> m_faces;
};

} // namespace rude
