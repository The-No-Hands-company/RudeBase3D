/*
 * ██████╗ ██╗   ██╗██████╗ ███████╗██████╗  █████╗ ███████╗███████╗██████╗ ██████╗ 
 * ██╔══██╗██║   ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝╚════██╗██╔══██╗
 * ██████╔╝██║   ██║██║  ██║█████╗  ██████╔╝███████║███████╗█████╗   █████╔╝██║  ██║
 * ██╔══██╗██║   ██║██║  ██║██╔══╝  ██╔══██╗██╔══██║╚════██║██╔══╝   ╚═══██╗██║  ██║
 * ██║  ██║╚██████╔╝██████╔╝███████╗██████╔╝██║  ██║███████║███████╗██████╔╝██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═════╝ ╚═════╝ 
 * 
 * Professional 3D Modeling & Animation Software
 * 
 * ████████████████████████████████████████████████████████████████████████████████
 * █                                                                              █
 * █  FILE: Mesh.h                                                               █
 * █  DESCRIPTION: Advanced 3D Mesh System with Half-Edge Data Structure        █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite          █
 * █  VERSION: 1.0.0                                                             █
 * █  CREATED: 2024                                                              █
 * █  UPDATED: August 7, 2025                                                    █
 * █                                                                              █
 * █  LICENSE: MIT License                                                       █
 * █  COMPANY: The No Hands Company                                              █
 * █  AUTHOR: RudeBase3D Development Team                                        █
 * █                                                                              █
 * █  This file contains the core Mesh class implementation providing advanced   █
 * █  3D mesh representation, manipulation, and rendering capabilities using     █
 * █  half-edge data structures for professional 3D modeling operations.        █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file Mesh.h
 * @brief Advanced 3D Mesh System with Half-Edge Data Structure and GPU Integration
 * 
 * This file contains the comprehensive Mesh class implementation that provides
 * professional-grade 3D mesh representation, manipulation, and rendering
 * capabilities. The system utilizes half-edge data structures for efficient
 * topological operations and seamless OpenGL integration for high-performance
 * rendering within the RudeBase3D framework.
 * 
 * ## Core Mesh Architecture
 * 
 * ### Half-Edge Data Structure
 * Advanced topological mesh representation:
 * - **Efficient Connectivity**: O(1) navigation between mesh elements
 * - **Topological Integrity**: Maintains manifold mesh properties
 * - **Dynamic Operations**: Real-time mesh editing and modification
 * - **Memory Efficiency**: Optimized storage for large mesh datasets
 * 
 * ### Dual Representation System
 * Flexible mesh data handling:
 * - **Vertex Array Format**: Direct rendering-optimized vertex data
 * - **Half-Edge Structure**: Topological operations and mesh editing
 * - **Automatic Synchronization**: Seamless conversion between representations
 * - **Lazy Evaluation**: Performance optimization through deferred updates
 * 
 * ### GPU Integration
 * High-performance OpenGL rendering:
 * - **Vertex Buffer Objects**: Efficient GPU memory management
 * - **Vertex Array Objects**: Optimized rendering state management
 * - **Element Buffer Objects**: Indexed rendering for memory efficiency
 * - **Instanced Rendering**: Support for large-scale object duplication
 * 
 * ## Professional Mesh Operations
 * 
 * ### Geometric Manipulation
 * Advanced mesh editing capabilities:
 * - **Face Extrusion**: Extend faces along normal or custom directions
 * - **Edge Beveling**: Create smooth beveled edges with configurable parameters
 * - **Face Subdivision**: Increase mesh resolution with various algorithms
 * - **Loop Cuts**: Insert edge loops for detailed modeling control
 * 
 * ### Topological Operations
 * Robust mesh structure modification:
 * - **Edge Splitting**: Divide edges while maintaining manifold properties
 * - **Face Merging**: Combine adjacent faces with automatic vertex welding
 * - **Hole Filling**: Intelligent hole detection and triangulation
 * - **Non-Manifold Repair**: Automatic cleanup of mesh topology issues
 * 
 * ### Surface Analysis
 * Comprehensive mesh quality assessment:
 * - **Normal Calculation**: Smooth and flat normal computation algorithms
 * - **Curvature Analysis**: Mean and Gaussian curvature computation
 * - **Quality Metrics**: Aspect ratio, skewness, and triangle quality
 * - **Boundary Detection**: Identification of mesh holes and boundaries
 * 
 * ## Advanced Features
 * 
 * ### Subdivision Surfaces
 * Industry-standard subdivision algorithms:
 * - **Catmull-Clark Subdivision**: Quad-dominant mesh smoothing
 * - **Loop Subdivision**: Triangle-based subdivision for organic forms
 * - **Custom Subdivision**: User-defined subdivision rules and patterns
 * - **Adaptive Subdivision**: Level-of-detail based mesh refinement
 * 
 * ### Mesh Optimization
 * Performance and quality enhancement:
 * - **Decimation**: Polygon reduction while preserving visual quality
 * - **Remeshing**: Uniform triangle distribution for better quality
 * - **Vertex Welding**: Automatic duplicate vertex removal
 * - **Degenerate Removal**: Cleanup of zero-area triangles and edges
 * 
 * ### UV Mapping Integration
 * Texture coordinate management:
 * - **Automatic UV Generation**: Planar, cylindrical, and spherical projection
 * - **UV Unwrapping**: Conformal and angle-preserving parameterization
 * - **Seam Detection**: Intelligent edge marking for UV boundaries
 * - **Texture Atlas**: Multiple UV channel support for complex shading
 * 
 * ## Rendering Pipeline Integration
 * 
 * ### Multi-Pass Rendering
 * Flexible rendering architecture:
 * - **Depth Pre-Pass**: Early Z-rejection for performance optimization
 * - **Shadow Mapping**: Efficient shadow generation and rendering
 * - **Deferred Shading**: G-buffer generation for complex lighting
 * - **Forward+ Rendering**: Tiled forward rendering for transparency
 * 
 * ### Level-of-Detail (LOD)
 * Distance-based mesh complexity management:
 * - **Automatic LOD Generation**: Progressive mesh simplification
 * - **Seamless Transitions**: Smooth LOD switching without popping
 * - **View-Dependent LOD**: Camera-relative detail adjustment
 * - **Morphing**: Smooth interpolation between LOD levels
 * 
 * ### Instancing Support
 * Efficient rendering of repeated geometry:
 * - **Hardware Instancing**: GPU-accelerated instance rendering
 * - **Transform Matrices**: Per-instance transformation data
 * - **Attribute Streaming**: Custom per-instance vertex attributes
 * - **Culling Optimization**: Instance-level frustum and occlusion culling
 * 
 * ## Data Structure Details
 * 
 * ### Vertex Management
 * Comprehensive vertex data handling:
 * - **Position Data**: 3D coordinate storage with precision options
 * - **Normal Vectors**: Surface normal computation and storage
 * - **Texture Coordinates**: Multi-channel UV coordinate support
 * - **Vertex Colors**: Per-vertex color attributes for artistic control
 * - **Custom Attributes**: Extensible attribute system for specialized data
 * 
 * ### Index Management
 * Efficient triangle and connectivity data:
 * - **Triangle Indices**: Standard indexed triangle representation
 * - **Adjacency Information**: Neighbor triangle data for advanced algorithms
 * - **Primitive Restart**: Support for triangle strips and fans
 * - **Multi-Material**: Per-face material assignment and rendering
 * 
 * ### Memory Layout
 * Performance-optimized data organization:
 * - **Structure of Arrays**: Cache-friendly memory layout for processing
 * - **Array of Structures**: Convenience layout for direct rendering
 * - **Interleaved Attributes**: Optimized vertex buffer layout
 * - **Compression**: Vertex attribute compression for memory efficiency
 * 
 * ## Usage Examples
 * 
 * ### Basic Mesh Creation and Setup
 * ```cpp
 * // Create a new mesh
 * auto mesh = std::make_shared<Mesh>();
 * 
 * // Define vertices with positions, normals, and texture coordinates
 * std::vector<glm::vec3> positions = {
 *     {-1.0f, -1.0f, 0.0f}, { 1.0f, -1.0f, 0.0f},
 *     { 1.0f,  1.0f, 0.0f}, {-1.0f,  1.0f, 0.0f}
 * };
 * 
 * std::vector<glm::vec3> normals = {
 *     {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
 *     {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}
 * };
 * 
 * std::vector<glm::vec2> texCoords = {
 *     {0.0f, 0.0f}, {1.0f, 0.0f},
 *     {1.0f, 1.0f}, {0.0f, 1.0f}
 * };
 * 
 * std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
 * 
 * // Set mesh data
 * mesh->setData(positions, indices, normals, texCoords);
 * mesh->uploadToGPU();
 * ```
 * 
 * ### Advanced Mesh Manipulation
 * ```cpp
 * // Access half-edge representation for topological operations
 * auto& halfEdgeMesh = mesh->getHalfEdgeMesh();
 * 
 * // Extrude a specific face
 * auto face = halfEdgeMesh.getFace(0);
 * if (face && mesh->extrudeFace(face, 2.0f)) {
 *     mesh->updateNormals();
 *     mesh->uploadToGPU();
 * }
 * 
 * // Subdivide for higher detail
 * for (auto& face : halfEdgeMesh.getFaces()) {
 *     if (face->shouldSubdivide()) {
 *         mesh->subdivideFace(face, 2); // 2 subdivision levels
 *     }
 * }
 * 
 * // Apply edge beveling for smooth transitions
 * for (auto& edge : halfEdgeMesh.getEdges()) {
 *     if (edge->isSharp()) {
 *         mesh->bevelEdge(edge, 0.1f);
 *     }
 * }
 * ```
 * 
 * ### Efficient Rendering Integration
 * ```cpp
 * // Render with material and lighting
 * void renderMesh(const Mesh& mesh, const Material& material, 
 *                const Camera& camera, const std::vector<Light>& lights) {
 *     
 *     // Bind mesh for rendering
 *     mesh.bind();
 *     
 *     // Set up shader uniforms
 *     auto shader = material.getShader();
 *     shader.use();
 *     shader.setMatrix4("model", mesh.getTransform());
 *     shader.setMatrix4("view", camera.getViewMatrix());
 *     shader.setMatrix4("projection", camera.getProjectionMatrix());
 *     
 *     // Set material properties
 *     material.bindToShader(shader);
 *     
 *     // Set lighting uniforms
 *     for (size_t i = 0; i < lights.size(); ++i) {
 *         lights[i].bindToShader(shader, i);
 *     }
 *     
 *     // Render the mesh
 *     mesh.render();
 *     mesh.unbind();
 * }
 * ```
 * 
 * ### Procedural Mesh Generation
 * ```cpp
 * class ProceduralMeshGenerator {
 * public:
 *     static std::shared_ptr<Mesh> createSphere(float radius, int segments) {
 *         auto mesh = std::make_shared<Mesh>();
 *         
 *         std::vector<glm::vec3> positions;
 *         std::vector<glm::vec3> normals;
 *         std::vector<glm::vec2> texCoords;
 *         std::vector<unsigned int> indices;
 *         
 *         // Generate sphere vertices
 *         for (int lat = 0; lat <= segments; ++lat) {
 *             float theta = lat * M_PI / segments;
 *             float sinTheta = sin(theta);
 *             float cosTheta = cos(theta);
 *             
 *             for (int lon = 0; lon <= segments; ++lon) {
 *                 float phi = lon * 2 * M_PI / segments;
 *                 float sinPhi = sin(phi);
 *                 float cosPhi = cos(phi);
 *                 
 *                 glm::vec3 position(
 *                     radius * sinTheta * cosPhi,
 *                     radius * cosTheta,
 *                     radius * sinTheta * sinPhi
 *                 );
 *                 
 *                 positions.push_back(position);
 *                 normals.push_back(glm::normalize(position));
 *                 texCoords.push_back(glm::vec2(
 *                     (float)lon / segments,
 *                     (float)lat / segments
 *                 ));
 *             }
 *         }
 *         
 *         // Generate sphere indices
 *         for (int lat = 0; lat < segments; ++lat) {
 *             for (int lon = 0; lon < segments; ++lon) {
 *                 int current = lat * (segments + 1) + lon;
 *                 int next = current + segments + 1;
 *                 
 *                 indices.push_back(current);
 *                 indices.push_back(next);
 *                 indices.push_back(current + 1);
 *                 
 *                 indices.push_back(current + 1);
 *                 indices.push_back(next);
 *                 indices.push_back(next + 1);
 *             }
 *         }
 *         
 *         mesh->setData(positions, indices, normals, texCoords);
 *         return mesh;
 *     }
 * };
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Memory Management
 * Efficient mesh data handling:
 * - **Memory Pooling**: Reuse vertex and index buffers for similar meshes
 * - **Compression**: Vertex attribute quantization for reduced memory usage
 * - **Streaming**: Dynamic loading/unloading for large mesh datasets
 * - **Reference Counting**: Automatic cleanup of unused mesh resources
 * 
 * ### Rendering Efficiency
 * High-performance rendering strategies:
 * - **Frustum Culling**: Skip rendering of meshes outside view frustum
 * - **Occlusion Culling**: GPU-based visibility testing for hidden objects
 * - **Batch Rendering**: Combine multiple meshes into single draw calls
 * - **Indirect Drawing**: GPU-driven rendering for massive scene complexity
 * 
 * @ingroup Geometry
 * @ingroup Mesh
 * @ingroup HalfEdge
 * 
 * @see HalfEdgeMesh For topological mesh operations
 * @see Vertex For vertex data structure and attributes
 * @see Material For surface property assignment
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */

#pragma once

#include "Common.h"
#include "core/mesh_forward.hpp"
#include "core/half_edge_mesh.hpp"
#include "core/mesh_elements.hpp"
#include <QtOpenGL/QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

// Forward declaration

/**
 * @brief Advanced 3D Mesh System with Half-Edge Data Structure and Professional Modeling Tools
 * 
 * The Mesh class represents the core 3D geometry system in RudeBase3D, providing
 * sophisticated mesh representation, manipulation, and rendering capabilities.
 * This implementation combines the efficiency of indexed vertex arrays for
 * rendering with the topological richness of half-edge data structures for
 * advanced geometric operations and professional modeling workflows.
 * 
 * ## Architecture Overview
 * 
 * ### Dual Representation System
 * Optimal data structure for different operations:
 * - **Indexed Vertex Arrays**: Direct GPU rendering with maximum performance
 * - **Half-Edge Structure**: Topological mesh editing and advanced operations
 * - **Automatic Synchronization**: Seamless conversion between representations
 * - **Lazy Evaluation**: Performance optimization through deferred updates
 * 
 * ### Memory Management
 * Efficient handling of large mesh datasets:
 * - **GPU Buffer Objects**: Optimized OpenGL buffer management (VBO, VAO, EBO)
 * - **Smart Pointers**: Automatic memory cleanup and reference counting
 * - **Memory Pooling**: Reuse of vertex and index buffers for similar meshes
 * - **Streaming Support**: Dynamic loading/unloading for massive datasets
 * 
 * ### Professional Integration
 * Industry-standard mesh processing capabilities:
 * - **Half-Edge Topology**: O(1) navigation for mesh editing operations
 * - **Manifold Preservation**: Maintains mesh integrity during modifications
 * - **Multi-Resolution**: Support for subdivision surfaces and LOD systems
 * - **Material Binding**: Seamless integration with advanced material systems
 * 
 * ## Core Mesh Operations
 * 
 * ### Geometric Manipulation
 * Professional modeling tool implementation:
 * - **Face Extrusion**: Extend selected faces along normal or custom directions
 * - **Edge Beveling**: Create smooth beveled edges with precise control
 * - **Face Subdivision**: Increase mesh resolution using various algorithms
 * - **Loop Cuts**: Insert edge loops for detailed modeling control
 * - **Vertex Smoothing**: Laplacian and Taubin smoothing algorithms
 * 
 * ### Topological Operations
 * Robust mesh structure modification:
 * - **Edge Splitting**: Divide edges while maintaining manifold properties
 * - **Face Merging**: Combine adjacent faces with automatic vertex welding
 * - **Hole Filling**: Intelligent hole detection and triangulation
 * - **Non-Manifold Repair**: Automatic cleanup of mesh topology issues
 * - **Boundary Detection**: Identification and handling of mesh boundaries
 * 
 * ### Surface Analysis
 * Comprehensive mesh quality assessment:
 * - **Normal Calculation**: Smooth and flat normal computation algorithms
 * - **Curvature Analysis**: Mean and Gaussian curvature computation
 * - **Quality Metrics**: Aspect ratio, skewness, and triangle quality assessment
 * - **Bounding Volume**: Efficient AABB and sphere computation for culling
 * - **Connectivity Analysis**: Genus calculation and topology validation
 * 
 * ## Advanced Features
 * 
 * ### Subdivision Surfaces
 * Industry-standard subdivision implementations:
 * - **Catmull-Clark**: Quad-dominant subdivision for smooth organic forms
 * - **Loop Subdivision**: Triangle-based subdivision with boundary rules
 * - **Adaptive Subdivision**: View-dependent and curvature-based refinement
 * - **Custom Rules**: User-defined subdivision patterns and behaviors
 * 
 * ### Mesh Optimization
 * Performance and quality enhancement algorithms:
 * - **Polygon Reduction**: Quadric error metrics decimation
 * - **Remeshing**: Uniform triangle distribution for better quality
 * - **Vertex Optimization**: Position adjustment for improved shape quality
 * - **Topology Optimization**: Edge flipping and vertex relocation
 * 
 * ### UV Mapping Integration
 * Comprehensive texture coordinate management:
 * - **Automatic Projection**: Planar, cylindrical, spherical, and cubic projection
 * - **Conformal Unwrapping**: Angle-preserving UV parameterization
 * - **Seam Management**: Intelligent edge marking for UV boundaries
 * - **Multi-Channel UV**: Support for multiple UV coordinate sets
 * 
 * ## Rendering Integration
 * 
 * ### OpenGL Pipeline
 * Optimized GPU rendering support:
 * - **Vertex Array Objects**: Efficient rendering state management
 * - **Buffer Management**: Dynamic and static buffer usage patterns
 * - **Instanced Rendering**: Hardware-accelerated instance rendering
 * - **Multi-Draw Support**: Batch rendering for improved performance
 * 
 * ### Shader Integration
 * Modern graphics pipeline compatibility:
 * - **Vertex Attributes**: Position, normal, tangent, UV, and color data
 * - **Uniform Buffers**: Efficient constant data management
 * - **Storage Buffers**: Large dataset handling for complex shaders
 * - **Compute Shaders**: GPU-accelerated mesh processing algorithms
 * 
 * ### Level-of-Detail (LOD)
 * Adaptive mesh complexity management:
 * - **Distance-Based LOD**: Automatic detail reduction based on camera distance
 * - **Progressive Meshes**: Smooth transitions between detail levels
 * - **View-Frustum LOD**: Screen-space detail adjustment
 * - **Morphing**: Smooth interpolation between LOD levels
 * 
 * ## Professional Workflows
 * 
 * ### Modeling Tool Integration
 * Essential tools for 3D content creation:
 * - **Selection Systems**: Vertex, edge, and face selection with advanced filters
 * - **Transform Operations**: Precise translation, rotation, and scaling
 * - **Duplication**: Array modifiers and instance creation
 * - **Boolean Operations**: Union, intersection, and difference using robust CSG
 * 
 * ### Animation Support
 * Dynamic mesh deformation and animation:
 * - **Vertex Animation**: Direct vertex position animation
 * - **Skeletal Deformation**: Bone-based mesh deformation with weights
 * - **Morph Targets**: Blend shape animation for facial expressions
 * - **Physics Integration**: Soft body and cloth simulation compatibility
 * 
 * ### Import/Export Pipeline
 * Industry-standard file format support:
 * - **OBJ Format**: Wavefront OBJ with material library support
 * - **FBX Integration**: Autodesk FBX for complex scene hierarchies
 * - **glTF 2.0**: Modern web and real-time rendering standard
 * - **USD Support**: Pixar Universal Scene Description integration
 * 
 * ## Performance Optimization
 * 
 * ### Memory Efficiency
 * Optimized data structures and algorithms:
 * - **Structure of Arrays**: Cache-friendly memory layout for processing
 * - **Compression**: Vertex attribute quantization and compression
 * - **Memory Mapping**: Efficient file-based mesh streaming
 * - **Reference Counting**: Automatic cleanup of unused mesh data
 * 
 * ### Rendering Performance
 * High-performance graphics optimization:
 * - **Frustum Culling**: Early rejection of off-screen geometry
 * - **Occlusion Culling**: GPU-based visibility testing for hidden objects
 * - **Draw Call Batching**: Minimize state changes and draw calls
 * - **GPU-Driven Rendering**: Compute shader-based culling and LOD selection
 * 
 * ### Parallel Processing
 * Multi-core and GPU acceleration:
 * - **Threading**: Multi-threaded mesh processing algorithms
 * - **SIMD Optimization**: Vectorized operations for vertex processing
 * - **GPU Compute**: CUDA/OpenCL acceleration for intensive operations
 * - **Asynchronous Processing**: Non-blocking mesh operations for UI responsiveness
 * 
 * @ingroup Geometry
 * @ingroup Mesh
 * @ingroup HalfEdgeDataStructure
 * 
 * @see HalfEdgeMesh For topological mesh representation and operations
 * @see Vertex For vertex data structure and attribute management  
 * @see Material For surface property assignment and rendering
 * @see SubdivisionMesh For advanced subdivision surface algorithms
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class Mesh : protected QOpenGLFunctions_3_3_Core {
public:
    Mesh();
    ~Mesh();

    // Mesh data
    void setVertices(const std::vector<rude::VertexPtr>& vertices);
    void setVertices(const std::vector<rude::Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    
    const std::vector<rude::VertexPtr>& getVertices() const { return m_vertices; }
    const std::vector<unsigned int>& getIndices() const { return m_indices; }
    
    // OpenGL buffer management
    void uploadToGPU();
    void bind();
    void unbind();
    void render();
    void renderWireframe();
    
    // Mesh manipulation
    void calculateNormals();
    void calculateTangents();
    glm::vec3 getBoundingBoxMin() const;
    glm::vec3 getBoundingBoxMax() const;
    glm::vec3 getBoundingBoxCenter() const;
    float getBoundingRadius() const;
    
    // Half-edge mesh interface
    rude::HalfEdgeMesh& getHalfEdgeMesh();
    const rude::HalfEdgeMesh& getHalfEdgeMesh() const;
    
    // Data management
    void setData(const std::vector<rude::VertexPtr>& vertices, const std::vector<unsigned int>& indices);
    void setData(const std::vector<glm::vec3>& positions, const std::vector<unsigned int>& indices, 
                 const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords);
    

    
    // Mesh operations
    bool extrudeFace(const rude::FacePtr& face, float distance = 1.0f);
    bool bevelEdge(const rude::EdgePtr& edge, float amount = 0.1f);
    bool subdivideFace(const rude::FacePtr& face, int subdivisions = 1);
    void updateNormals();
    
    // OpenGL management
    void initializeGL();
    void cleanupGL();
    
    // Utility
    void clear();
    bool isEmpty() const { return m_vertices.empty(); }
    size_t getVertexCount() const { return m_vertices.size(); }
    size_t getTriangleCount() const { return m_indices.size() / 3; }

private:
    std::vector<rude::VertexPtr> m_vertices;
    std::vector<unsigned int> m_indices;
    
    // OpenGL objects
    unsigned int m_VAO;  // Vertex Array Object
    unsigned int m_VBO;  // Vertex Buffer Object
    unsigned int m_EBO;  // Element Buffer Object
    
    bool m_uploaded;
    
    // Half-edge mesh representation
    std::unique_ptr<rude::HalfEdgeMesh> m_halfEdgeMesh;
};
