//==============================================================================
// RudeBase3D Engine - mesh.hpp
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

#pragma once

#include "Common.h"
#include "core/half_edge_mesh.hpp"
#include "core/mesh_elements.hpp"
#include <QOpenGLFunctions_3_3_Core>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

// Forward declaration
class Entity;

/**
 * @addtogroup Geometry
 * @{
 */

namespace rude {

/**
 * @brief Renderable mesh representation for 3D geometry
 * 
 * The Mesh class provides a high-performance, GPU-friendly representation
 * of 3D geometry optimized for rendering. It stores vertex data in an
 * indexed format suitable for OpenGL vertex buffer objects.
 * 
 * ## Design Philosophy
 * This class is designed for rendering performance, with vertex data
 * stored in contiguous arrays optimized for GPU upload. For topological
 * operations and mesh editing, use HalfEdgeMesh instead.
 * 
 * ## Key Features
 * - Indexed vertex representation for memory efficiency
 * - Direct OpenGL buffer object integration
 * - Automatic normal calculation and updates
 * - Bounding box and geometric queries
 * - Conversion to/from half-edge representation
 * - Basic modeling operations (extrude, bevel, subdivide)
 * 
 * ## Usage Pattern
 * ```cpp
 * auto mesh = std::make_shared<rude::Mesh>();
 * mesh->setData(vertices, indices);
 * mesh->updateNormals();
 * mesh->uploadToGPU();
 * 
 * // In render loop:
 * mesh->bind();
 * mesh->render();
 * mesh->unbind();
 * ```
 * 
 * ## Performance Notes
 * - Vertex data is stored in structure-of-arrays format for cache efficiency
 * - GPU uploads are batched for optimal performance
 * - Geometric queries use cached bounding volumes when possible
 * 
 * @see HalfEdgeMesh for topological operations
 * @see Vertex for vertex data structure
 * @see Material for rendering properties
 * 
 * @ingroup Geometry
 */
class Mesh {
public:
    /**
     * @brief Default constructor
     * 
     * Creates an empty mesh with no vertices or indices.
     * The mesh must be populated with setData() before use.
     */
    Mesh();
    
    /**
     * @brief Destructor
     * 
     * Cleans up GPU resources and vertex data.
     * Automatically unbinds from OpenGL context if needed.
     */
    ~Mesh();

    // ========================================================================
    // Data Access
    // ========================================================================
    
    /**
     * @brief Get mutable reference to vertex array
     * 
     * Direct access to vertex data for modification. After modifying
     * vertices, call uploadToGPU() to sync changes to the GPU.
     * 
     * @return Mutable reference to vertex vector
     * 
     * @warning Modifying vertices requires calling uploadToGPU() and
     *          potentially updateNormals() for changes to take effect
     */
    ::std::vector<rude::Vertex>& getVertices() { return m_vertices; }
    
    /**
     * @brief Get const reference to vertex array
     * 
     * @return Const reference to vertex vector for read-only access
     */
    const ::std::vector<rude::Vertex>& getVertices() const { return m_vertices; }
    
    /**
     * @brief Get mutable reference to index array
     * 
     * Direct access to index data for modification. Indices define
     * triangular faces using vertex array indices.
     * 
     * @return Mutable reference to index vector
     * 
     * @note Indices must be valid vertex array indices (< vertex count)
     */
    ::std::vector<unsigned int>& getIndices() { return m_indices; }
    
    /**
     * @brief Get const reference to index array
     * 
     * @return Const reference to index vector for read-only access
     */
    const ::std::vector<unsigned int>& getIndices() const { return m_indices; }
    
    // ========================================================================
    // Mesh Operations
    // ========================================================================
    
    /**
     * @brief Extrude a face along its normal
     * 
     * Creates new geometry by extruding the specified face along its normal
     * vector by the given distance. This operation adds new vertices and faces.
     * 
     * @param faceIndex Index of the face to extrude (must be valid)
     * @param distance Distance to extrude (positive = outward, negative = inward)
     * 
     * @pre faceIndex must be a valid face index (0 <= faceIndex < face count)
     * @post New vertices and faces are added to the mesh
     * 
     * @note Normals are automatically recalculated after the operation
     * @see updateNormals()
     */
    void extrudeFace(int faceIndex, float distance);
    
    /**
     * @brief Bevel an edge with specified width and depth
     * 
     * Creates a beveled edge by replacing the sharp edge with a small
     * chamfered surface. This operation smooths hard edges.
     * 
     * @param edgeIndex Index of the edge to bevel
     * @param width Width of the bevel (perpendicular to edge)
     * @param depth Depth of the bevel (along edge direction)
     * 
     * @pre edgeIndex must be a valid edge index
     * @post Edge is replaced with beveled geometry
     */
    void bevelEdge(int edgeIndex, float width, float depth);
    
    /**
     * @brief Subdivide a face into smaller faces
     * 
     * Divides the specified face into multiple smaller faces using
     * the specified number of divisions in each direction.
     * 
     * @param faceIndex Index of the face to subdivide
     * @param divisions Number of divisions (must be >= 1)
     * 
     * @pre faceIndex must be valid, divisions must be positive
     * @post Face is replaced with (divisions^2) smaller faces
     */
    void subdivideFace(int faceIndex, int divisions);
    
    /**
     * @brief Recalculate vertex normals
     * 
     * Computes smooth vertex normals by averaging face normals
     * of adjacent faces. Call this after modifying vertex positions
     * or face topology.
     * 
     * @post All vertex normals are updated with new values
     * @note This is an expensive operation for large meshes
     */
    void updateNormals();
    
    // ========================================================================
    // Data Management
    // ========================================================================
    
    /**
     * @brief Set mesh data from vertex and index arrays
     * 
     * Replaces the current mesh data with new vertices and indices.
     * This is the primary way to populate an empty mesh.
     * 
     * @param vertices Vector of vertex data
     * @param indices Vector of triangle indices (must be multiples of 3)
     * 
     * @pre indices.size() must be divisible by 3
     * @pre All index values must be < vertices.size()
     * 
     * @post Mesh contains the new geometry data
     * @note GPU data is not automatically updated - call uploadToGPU()
     */
    void setData(const ::std::vector<rude::Vertex>& vertices, const ::std::vector<unsigned int>& indices);
    
    // OpenGL operations
    void uploadToGPU();
    void bind();
    void unbind();
    void render();
    
    // Utility
    glm::vec3 getBoundingBoxMin() const;
    glm::vec3 getBoundingBoxMax() const;
    glm::vec3 getBoundingBoxCenter() const;
    float getBoundingRadius() const;
    
    // Conversion
    ::std::shared_ptr<HalfEdgeMesh> toHalfEdgeMesh() const;
    static Mesh fromHalfEdgeMesh(const ::std::shared_ptr<HalfEdgeMesh>& halfEdgeMesh);
    
    // Property queries
    bool isEmpty() const { return m_vertices.empty(); }
    size_t getVertexCount() const { return m_vertices.size(); }
    size_t getTriangleCount() const { return m_indices.size() / 3; }

private:
    ::std::vector<rude::Vertex> m_vertices;
    ::std::vector<unsigned int> m_indices;
    
    // OpenGL objects
    unsigned int m_VAO;  // Vertex Array Object
    unsigned int m_VBO;  // Vertex Buffer Object
    unsigned int m_EBO;  // Element Buffer Object
    
    bool m_uploaded;
    
    void initializeGL();
    void cleanupGL();
};

} // namespace rude
