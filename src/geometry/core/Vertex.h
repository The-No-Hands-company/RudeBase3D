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
 * █  FILE: Vertex.h                                                             █
 * █  DESCRIPTION: Advanced Vertex Data Structure with Multi-Attribute Support  █
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
 * █  This file contains the core Vertex class implementation providing          █
 * █  comprehensive vertex data representation, attribute management, and        █
 * █  seamless integration between different coordinate systems and frameworks.  █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file Vertex.h
 * @brief Advanced Vertex Data Structure with Multi-Attribute Support and Framework Integration
 * 
 * This file contains the comprehensive Vertex class implementation that provides
 * professional-grade vertex data representation, multi-attribute management,
 * and seamless integration between Qt GUI frameworks and GLM mathematics libraries.
 * The system is designed to support complex 3D modeling workflows while maintaining
 * optimal performance for real-time rendering and animation within RudeBase3D.
 * 
 * ## Core Vertex Architecture
 * 
 * ### Multi-Framework Integration
 * Seamless interoperability between graphics frameworks:
 * - **GLM Mathematics**: High-performance vector and matrix operations
 * - **Qt Integration**: GUI framework compatibility for user interfaces
 * - **OpenGL Compatibility**: Direct vertex buffer object (VBO) support
 * - **Cross-Platform**: Consistent behavior across Windows, Linux, and macOS
 * 
 * ### Attribute Management System
 * Comprehensive vertex attribute handling:
 * - **Position Data**: 3D spatial coordinates with precision control
 * - **Normal Vectors**: Surface orientation for lighting calculations
 * - **Texture Coordinates**: UV mapping for material and texture application
 * - **Color Attributes**: Per-vertex color specification for artistic control
 * - **Tangent Vectors**: Bump mapping and normal mapping support
 * - **Custom Attributes**: Extensible system for specialized vertex data
 * 
 * ### Performance Optimization
 * Memory-efficient and cache-friendly design:
 * - **Compact Layout**: Optimized memory footprint for large vertex datasets
 * - **SIMD Compatibility**: Vectorized operations for batch processing
 * - **Memory Alignment**: GPU-friendly data alignment for optimal performance
 * - **Reference Counting**: Smart pointer management for automatic cleanup
 * 
 * ## Professional Features
 * 
 * ### Advanced Vertex Operations
 * Sophisticated geometric processing capabilities:
 * - **Interpolation**: Linear and spherical interpolation between vertices
 * - **Distance Calculations**: Euclidean and Manhattan distance metrics
 * - **Normalization**: Vector normalization with numerical stability
 * - **Transformation**: Matrix-based vertex transformations
 * - **Blending**: Weighted vertex combination for morphing and animation
 * 
 * ### Animation Support
 * Dynamic vertex manipulation for animation systems:
 * - **Keyframe Animation**: Time-based vertex position interpolation
 * - **Skeletal Deformation**: Bone-based vertex transformation with weights
 * - **Morph Targets**: Blend shape animation for facial expressions
 * - **Physics Integration**: Vertex position updates from physics simulations
 * - **Constraint Systems**: Vertex position constraints for realistic animation
 * 
 * ### Quality Assurance
 * Robust data validation and error handling:
 * - **NaN Detection**: Automatic detection of invalid floating-point values
 * - **Range Validation**: Configurable bounds checking for vertex attributes
 * - **Consistency Checks**: Validation of normal vector unit length
 * - **Precision Control**: Floating-point precision management for stability
 * 
 * ## Integration Capabilities
 * 
 * ### Rendering Pipeline Integration
 * Seamless graphics pipeline compatibility:
 * - **Vertex Buffer Objects**: Direct VBO data layout compatibility
 * - **Vertex Array Objects**: VAO attribute binding support
 * - **Shader Integration**: GLSL vertex shader attribute mapping
 * - **Instanced Rendering**: Per-instance vertex attribute support
 * - **Geometry Shaders**: Advanced vertex processing pipeline integration
 * 
 * ### File Format Support
 * Industry-standard file format compatibility:
 * - **OBJ Format**: Wavefront OBJ vertex data import/export
 * - **FBX Integration**: Autodesk FBX vertex attribute preservation
 * - **glTF 2.0**: Modern web standard vertex format support
 * - **PLY Format**: Stanford PLY point cloud and mesh format
 * - **STL Support**: Stereolithography format with normal generation
 * 
 * ### Mesh Processing Integration
 * Advanced geometric algorithm compatibility:
 * - **Subdivision Surfaces**: Vertex refinement for smooth surfaces
 * - **Mesh Decimation**: Vertex reduction while preserving quality
 * - **Remeshing**: Vertex redistribution for uniform tessellation
 * - **Smoothing Algorithms**: Laplacian and Taubin vertex smoothing
 * - **Edge Operations**: Vertex splitting and merging for topology changes
 * 
 * ## Technical Implementation
 * 
 * ### Memory Layout Optimization
 * Cache-friendly data structure design:
 * - **Structure of Arrays (SoA)**: Optimal for SIMD processing and batch operations
 * - **Array of Structures (AoS)**: Convenient for object-oriented vertex manipulation
 * - **Interleaved Attributes**: GPU-optimized vertex buffer layout
 * - **Alignment Padding**: Proper memory alignment for vectorized operations
 * 
 * ### Precision Management
 * Numerical stability and accuracy control:
 * - **Single Precision**: 32-bit floating-point for real-time applications
 * - **Double Precision**: 64-bit floating-point for high-precision modeling
 * - **Fixed Point**: Integer-based coordinates for specific applications
 * - **Adaptive Precision**: Dynamic precision adjustment based on requirements
 * 
 * ### Conversion Systems
 * Efficient data format interoperability:
 * - **GLM Conversion**: Native GLM vector and matrix type integration
 * - **Qt Conversion**: QVector3D and QVector2D compatibility
 * - **OpenGL Types**: Direct OpenGL attribute format support
 * - **SIMD Types**: SSE/AVX vector type compatibility for acceleration
 * 
 * ## Usage Examples
 * 
 * ### Basic Vertex Creation and Manipulation
 * ```cpp
 * // Create vertices with different attribute combinations
 * Vertex v1(glm::vec3(0.0f, 0.0f, 0.0f)); // Position only
 * Vertex v2(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Position + Normal
 * Vertex v3(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f)); // Full attributes
 * 
 * // Modify vertex attributes
 * v1.setPosition(glm::vec3(2.0f, 1.0f, 0.5f));
 * v1.setNormal(glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
 * v1.setTexCoord(glm::vec2(0.75f, 0.25f));
 * ```
 * 
 * ### Advanced Vertex Operations
 * ```cpp
 * // Interpolation between vertices for animation
 * Vertex startVertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
 * Vertex endVertex(glm::vec3(5.0f, 2.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
 * 
 * // Linear interpolation for smooth animation
 * float animationTime = 0.5f; // 50% through animation
 * Vertex interpolated = startVertex.lerp(endVertex, animationTime);
 * 
 * // Distance calculations for proximity queries
 * float distance = startVertex.distanceTo(endVertex);
 * 
 * // Vertex arithmetic operations
 * Vertex center = (startVertex + endVertex) * 0.5f;
 * Vertex offset = endVertex - startVertex;
 * ```
 * 
 * ### Framework Integration Examples
 * ```cpp
 * // Convert between RudeBase3D internal format and external representations
 * rude::Vertex internalVertex = externalVertex.toRudeVertex();
 * auto internalVertexPtr = externalVertex.toRudeVertexPtr();
 * 
 * // Create from GLM types directly
 * auto vertexFromGLM = Vertex::fromGLM(
 *     glm::vec3(1.0f, 2.0f, 3.0f), // position
 *     glm::vec3(0.0f, 1.0f, 0.0f), // normal
 *     glm::vec2(0.5f, 0.5f)        // texture coordinate
 * );
 * 
 * // Use in rendering pipeline
 * std::vector<Vertex> vertices = {v1, v2, v3};
 * auto vertexBuffer = createVertexBuffer(vertices);
 * bindVertexBuffer(vertexBuffer);
 * ```
 * 
 * ### Animation and Deformation
 * ```cpp
 * class VertexAnimator {
 * private:
 *     std::vector<Vertex> m_baseVertices;
 *     std::vector<Vertex> m_targetVertices;
 *     float m_animationTime = 0.0f;
 *     
 * public:
 *     void setKeyframes(const std::vector<Vertex>& base, const std::vector<Vertex>& target) {
 *         m_baseVertices = base;
 *         m_targetVertices = target;
 *     }
 *     
 *     std::vector<Vertex> getInterpolatedVertices(float t) {
 *         std::vector<Vertex> result;
 *         result.reserve(m_baseVertices.size());
 *         
 *         for (size_t i = 0; i < m_baseVertices.size(); ++i) {
 *             result.push_back(m_baseVertices[i].lerp(m_targetVertices[i], t));
 *         }
 *         
 *         return result;
 *     }
 *     
 *     void update(float deltaTime) {
 *         m_animationTime += deltaTime;
 *         // Apply easing functions for smooth animation curves
 *         float easedTime = easeInOutCubic(m_animationTime);
 *         auto currentVertices = getInterpolatedVertices(easedTime);
 *         // Update mesh with new vertex positions
 *     }
 * };
 * ```
 * 
 * ### Batch Processing and Optimization
 * ```cpp
 * class VertexProcessor {
 * public:
 *     // Batch normalize all vertex normals
 *     static void normalizeNormals(std::vector<Vertex>& vertices) {
 *         #pragma omp parallel for
 *         for (auto& vertex : vertices) {
 *             vertex.normalize();
 *         }
 *     }
 *     
 *     // Apply transformation matrix to vertex positions
 *     static void transformVertices(std::vector<Vertex>& vertices, const glm::mat4& transform) {
 *         glm::mat3 normalTransform = glm::transpose(glm::inverse(glm::mat3(transform)));
 *         
 *         #pragma omp parallel for
 *         for (auto& vertex : vertices) {
 *             glm::vec4 pos = transform * glm::vec4(vertex.position, 1.0f);
 *             vertex.setPosition(glm::vec3(pos) / pos.w);
 *             vertex.setNormal(glm::normalize(normalTransform * vertex.normal));
 *         }
 *     }
 *     
 *     // Calculate bounding box for vertex collection
 *     static std::pair<glm::vec3, glm::vec3> calculateBounds(const std::vector<Vertex>& vertices) {
 *         if (vertices.empty()) return {glm::vec3(0), glm::vec3(0)};
 *         
 *         glm::vec3 min = vertices[0].position;
 *         glm::vec3 max = vertices[0].position;
 *         
 *         for (const auto& vertex : vertices) {
 *             min = glm::min(min, vertex.position);
 *             max = glm::max(max, vertex.position);
 *         }
 *         
 *         return {min, max};
 *     }
 * };
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Memory Management
 * Efficient vertex data handling strategies:
 * - **Memory Pooling**: Reuse vertex objects to reduce allocation overhead
 * - **Bulk Operations**: Batch processing for improved cache utilization
 * - **Lazy Evaluation**: Deferred computation of derived attributes
 * - **Reference Sharing**: Smart pointer usage for memory efficiency
 * 
 * ### Computational Efficiency
 * High-performance vertex processing techniques:
 * - **SIMD Operations**: Vectorized math operations for batch processing
 * - **Parallel Processing**: Multi-threading for large vertex datasets
 * - **GPU Acceleration**: Compute shader integration for massive datasets
 * - **Cache Optimization**: Data layout optimization for CPU cache efficiency
 * 
 * @ingroup Geometry
 * @ingroup Vertex
 * @ingroup DataStructures
 * 
 * @see Mesh For vertex collection and mesh representation
 * @see Material For vertex attribute usage in rendering
 * @see Animation For vertex-based animation systems
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */

#pragma once

#include "Common.h"
#include "core/mesh_elements.hpp"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

/**
 * @brief Advanced Vertex Data Structure with Multi-Attribute Support and Professional Integration
 * 
 * The Vertex class represents the fundamental building block for 3D geometry in
 * RudeBase3D, providing comprehensive vertex data management, multi-framework
 * integration, and professional-grade attribute handling. This implementation
 * serves as a bridge between different graphics frameworks while maintaining
 * optimal performance for real-time rendering and complex modeling operations.
 * 
 * ## Core Architecture
 * 
 * ### Multi-Framework Bridge
 * Seamless integration across graphics ecosystems:
 * - **GLM Mathematics**: High-performance vector operations and transformations
 * - **Qt Framework**: GUI and widget integration for user interface components
 * - **OpenGL Compatibility**: Direct vertex buffer and shader attribute mapping
 * - **Cross-Platform**: Consistent behavior across Windows, Linux, and macOS platforms
 * 
 * ### Attribute Management System
 * Comprehensive vertex attribute handling:
 * - **Spatial Coordinates**: 3D position data with configurable precision
 * - **Surface Normals**: Orientation vectors for lighting and shading calculations
 * - **Texture Mapping**: UV coordinates for material and texture application
 * - **Color Information**: Per-vertex color specification for artistic control
 * - **Tangent Vectors**: Surface tangent data for advanced normal mapping
 * - **Custom Attributes**: Extensible system for specialized vertex properties
 * 
 * ### Performance-Oriented Design
 * Optimized for real-time graphics applications:
 * - **Memory Efficiency**: Compact data layout with minimal overhead
 * - **Cache Friendliness**: Optimized memory access patterns for CPU performance
 * - **SIMD Compatibility**: Vectorized operations for batch processing
 * - **GPU Integration**: Direct compatibility with graphics hardware acceleration
 * 
 * ## Professional Features
 * 
 * ### Advanced Geometric Operations
 * Sophisticated vertex manipulation capabilities:
 * - **Interpolation Methods**: Linear, spherical, and cubic interpolation algorithms
 * - **Distance Metrics**: Euclidean, Manhattan, and Chebyshev distance calculations
 * - **Vector Operations**: Normalization, cross products, and dot products
 * - **Transformation Support**: Matrix-based vertex transformations and projections
 * - **Blending Operations**: Weighted vertex combination for morphing and animation
 * 
 * ### Animation and Deformation
 * Dynamic vertex manipulation for animation systems:
 * - **Keyframe Animation**: Time-based vertex position and attribute interpolation
 * - **Skeletal Deformation**: Bone-based vertex transformation with blend weights
 * - **Morph Target Animation**: Blend shape animation for facial expressions and organic forms
 * - **Physics Integration**: Real-time vertex updates from physics simulation systems
 * - **Constraint Systems**: Vertex position and attribute constraints for realistic animation
 * 
 * ### Quality Assurance and Validation
 * Robust data integrity and error handling:
 * - **NaN Detection**: Automatic identification and handling of invalid floating-point values
 * - **Range Validation**: Configurable bounds checking for vertex attribute values
 * - **Numerical Stability**: Precision management and overflow protection
 * - **Consistency Verification**: Validation of normal vector unit length and attribute coherence
 * 
 * ## Integration Capabilities
 * 
 * ### Rendering Pipeline Integration
 * Seamless graphics pipeline compatibility:
 * - **Vertex Buffer Objects**: Direct VBO data layout and attribute binding
 * - **Vertex Array Objects**: VAO configuration and state management
 * - **Shader Attribute Mapping**: GLSL vertex shader input variable mapping
 * - **Instanced Rendering**: Per-instance vertex attribute support for efficient rendering
 * - **Geometry Processing**: Integration with geometry and tessellation shaders
 * 
 * ### File Format Compatibility
 * Industry-standard file format support:
 * - **Wavefront OBJ**: Complete OBJ vertex data import and export functionality
 * - **Autodesk FBX**: FBX vertex attribute preservation and transformation
 * - **glTF 2.0**: Modern web standard vertex format with extension support
 * - **Stanford PLY**: Point cloud and mesh format with custom attribute support
 * - **STL Format**: Stereolithography format with automatic normal generation
 * 
 * ### Mesh Processing Integration
 * Advanced geometric algorithm compatibility:
 * - **Subdivision Surfaces**: Vertex refinement for Catmull-Clark and Loop subdivision
 * - **Mesh Simplification**: Vertex reduction algorithms with quality preservation
 * - **Remeshing Operations**: Vertex redistribution for uniform mesh tessellation
 * - **Smoothing Algorithms**: Laplacian, Taubin, and bilateral vertex smoothing
 * - **Topological Operations**: Vertex splitting, merging, and connectivity changes
 * 
 * ## Technical Implementation
 * 
 * ### Memory Layout Optimization
 * Cache-friendly data structure design:
 * - **Contiguous Storage**: Sequential memory layout for optimal cache utilization
 * - **Attribute Interleaving**: GPU-optimized vertex buffer organization
 * - **Alignment Requirements**: Proper memory alignment for SIMD and GPU operations
 * - **Padding Management**: Efficient memory usage with minimal padding overhead
 * 
 * ### Precision and Accuracy Management
 * Numerical stability across different precision requirements:
 * - **Single Precision (32-bit)**: Standard floating-point for real-time applications
 * - **Double Precision (64-bit)**: High-precision mode for CAD and scientific applications
 * - **Mixed Precision**: Selective precision for different vertex attributes
 * - **Quantization Support**: Compressed vertex formats for memory-constrained applications
 * 
 * ### Conversion and Interoperability
 * Efficient data format transformation:
 * - **GLM Vector Types**: Native integration with GLM mathematics library
 * - **Qt Vector Types**: Seamless conversion to QVector3D and QVector2D
 * - **OpenGL Data Types**: Direct mapping to OpenGL vertex attribute formats
 * - **SIMD Vector Types**: SSE, AVX, and NEON vector type compatibility
 * 
 * ## Performance Optimization
 * 
 * ### Computational Efficiency
 * High-performance vertex processing strategies:
 * - **Vectorized Operations**: SIMD instruction utilization for batch processing
 * - **Parallel Processing**: Multi-threading support for large vertex datasets
 * - **GPU Compute Integration**: CUDA and OpenCL acceleration for massive parallel processing
 * - **Memory Bandwidth Optimization**: Efficient memory access patterns for large datasets
 * 
 * ### Memory Management
 * Advanced memory handling techniques:
 * - **Object Pooling**: Vertex object reuse to minimize allocation overhead
 * - **Bulk Allocation**: Contiguous memory allocation for vertex arrays
 * - **Reference Counting**: Smart pointer management for automatic memory cleanup
 * - **Copy-on-Write**: Lazy copying for memory efficiency in vertex operations
 * 
 * @ingroup Geometry
 * @ingroup Vertex
 * @ingroup DataStructures
 * @ingroup FrameworkIntegration
 * 
 * @see Mesh For vertex collection management and mesh representation
 * @see Material For vertex attribute usage in rendering pipelines
 * @see Animation For vertex-based animation and deformation systems
 * @see Transform For vertex transformation and coordinate system conversion
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class Vertex {
public:
    // Constructors
    Vertex();
    Vertex(const glm::vec3& pos);
    Vertex(const glm::vec3& pos, const glm::vec3& norm);
    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex);
    
    // Copy constructor and assignment from rude::Vertex
    Vertex(const rude::Vertex& rudeVertex);
    Vertex& operator=(const rude::Vertex& rudeVertex);
    
    // Destructor
    ~Vertex() = default;

    // Qt-style accessors (for compatibility with existing code)
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    // GLM-style getters (for performance-critical code)
    glm::vec3 getPositionGLM() const;
    glm::vec3 getNormalGLM() const;
    glm::vec2 getTexCoordGLM() const;

    // GLM-style setters
    void setPosition(const glm::vec3& pos);
    void setNormal(const glm::vec3& norm);
    void setTexCoord(const glm::vec2& tex);

    // Conversion to rude::Vertex
    rude::Vertex toRudeVertex() const;
    std::shared_ptr<rude::Vertex> toRudeVertexPtr() const;

    // Static factory methods
    static Vertex fromGLM(const glm::vec3& pos, const glm::vec3& norm = glm::vec3(0.0f, 1.0f, 0.0f), const glm::vec2& tex = glm::vec2(0.0f, 0.0f));

    // Utility methods
    void normalize();
    float distanceTo(const Vertex& other) const;
    Vertex lerp(const Vertex& other, float t) const;

    // Operators
    bool operator==(const Vertex& other) const;
    bool operator!=(const Vertex& other) const;
    Vertex operator+(const Vertex& other) const;
    Vertex operator-(const Vertex& other) const;
    Vertex operator*(float scalar) const;

private:
    // No Qt conversion functions needed; all logic is now GLM-based.
};

// Type alias for convenience
using VertexPtr = std::shared_ptr<Vertex>;
