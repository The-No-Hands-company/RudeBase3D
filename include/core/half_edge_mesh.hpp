//==============================================================================
// RudeBase3D Engine - half_edge_mesh.hpp
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

/**
 * @file half_edge_mesh.hpp
 * @brief Half-edge mesh data structure for topological operations
 * 
 * This file contains the HalfEdgeMesh class, which provides a sophisticated
 * half-edge data structure for representing and manipulating 3D mesh topology.
 * This representation is particularly suited for modeling operations that
 * require efficient adjacency queries and topological modifications.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup Geometry
 */

#pragma once

#include "core/mesh_iterator.hpp"
#include "core/mesh_forward.hpp"
#include "core/mesh_elements.hpp"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

/**
 * @addtogroup Geometry
 * @{
 */

namespace rude {

/**
 * @brief Half-edge mesh data structure for topological mesh operations
 * 
 * The HalfEdgeMesh class implements the half-edge data structure, which is
 * the gold standard for representing mesh topology in professional 3D modeling
 * applications. This representation excels at topological operations like
 * subdivision, extrusion, loop cuts, and other advanced modeling operations.
 * 
 * ## Half-Edge Data Structure
 * The half-edge representation stores mesh topology using four main components:
 * - **Vertices**: Store position and reference to one outgoing half-edge
 * - **Half-Edges**: Store next, previous, opposite, vertex, and face references
 * - **Faces**: Store reference to one bounding half-edge
 * - **Edges**: Implicit - represented by half-edge pairs
 * 
 * ## Key Advantages
 * - **O(1) adjacency queries**: Fast neighbor lookups for modeling operations
 * - **Manifold enforcement**: Ensures valid mesh topology
 * - **Iterator support**: Efficient traversal of mesh elements
 * - **Topological operations**: Direct support for advanced modeling tools
 * 
 * ## Design Philosophy
 * This class is optimized for topological operations rather than rendering.
 * For rendering, convert to the simpler Mesh class using toMesh().
 * 
 * ## Usage Patterns
 * ```cpp
 * // Create and populate half-edge mesh
 * auto hemesh = std::make_shared<rude::HalfEdgeMesh>();
 * hemesh->fromMesh(renderMesh);
 * 
 * // Perform topological operations
 * auto face = hemesh->faces().begin();
 * hemesh->extrudeFace(face, 1.0f);
 * 
 * // Convert back for rendering
 * auto renderMesh = hemesh->toMesh();
 * ```
 * 
 * ## Performance Characteristics
 * - **Memory**: Higher overhead than indexed mesh (3-4x memory usage)
 * - **Adjacency**: O(1) neighbor queries vs O(n) for indexed mesh
 * - **Operations**: Optimized for topological modifications
 * - **Conversion**: Efficient bidirectional conversion with Mesh class
 * 
 * ## Thread Safety
 * This class is not thread-safe. External synchronization required for
 * concurrent access. Consider using separate instances per thread for
 * modeling operations.
 * 
 * @see Mesh for rendering-optimized representation
 * @see Vertex for vertex data structure
 * @see Face for face representation
 * @see HalfEdge for edge topology
 * 
 * @ingroup Geometry
 */
class HalfEdgeMesh {
public:
    // ========================================================================
    // Type Definitions
    // ========================================================================
    
    /** @brief Iterator for traversing vertices */
    using VertexIterator = rude::VertexIterator;
    
    /** @brief Iterator for traversing edges */
    using EdgeIterator = rude::EdgeIterator;
    
    /** @brief Iterator for traversing faces */
    using FaceIterator = rude::FaceIterator;
    
    /** @brief Iterator for traversing half-edges */
    using HalfEdgeIterator = rude::HalfEdgeIterator;

    // ========================================================================
    // Construction and Destruction
    // ========================================================================
    
    /**
     * @brief Default constructor
     * 
     * Creates an empty half-edge mesh with no vertices, edges, or faces.
     * Use addVertex(), addFace(), or fromMesh() to populate the mesh.
     */
    HalfEdgeMesh();

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