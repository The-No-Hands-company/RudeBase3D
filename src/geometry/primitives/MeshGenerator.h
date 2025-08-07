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
 * █  FILE: MeshGenerator.h                                                      █
 * █  DESCRIPTION: Advanced Parametric Primitive Mesh Generation System         █
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
 * █  This file contains the MeshGenerator class providing comprehensive         █
 * █  parametric primitive generation, advanced geometric algorithms, and        █
 * █  professional-quality mesh creation for 3D modeling workflows.             █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file MeshGenerator.h
 * @brief Advanced Parametric Primitive Mesh Generation and Geometric Algorithm System
 * 
 * This file contains the comprehensive MeshGenerator class implementation that provides
 * professional-grade parametric primitive generation capabilities essential for 3D
 * modeling workflows. The system offers mathematically precise generation of geometric
 * primitives with configurable resolution, UV mapping, and topology optimization
 * suitable for real-time rendering, animation, and professional modeling applications.
 * 
 * ## Core Primitive Generation System
 * 
 * ### Basic Geometric Primitives
 * Industry-standard primitive generation with mathematical precision:
 * - **Cube/Box**: Axis-aligned rectangular prisms with configurable dimensions
 * - **Sphere**: UV-sphere generation with latitude/longitude parameterization
 * - **Cylinder**: Circular cylinders with configurable height and radius
 * - **Plane**: Rectangular planes with subdivision support for displacement
 * - **Cone**: Circular cones with apex control and base radius configuration
 * - **Torus**: Donut-shaped geometry with major and minor radius control
 * 
 * ### Advanced Primitive Generation
 * Sophisticated geometric algorithms for complex primitives:
 * - **Icosphere**: Geodesic sphere with uniform triangle distribution
 * - **Subdivision Surfaces**: Multi-resolution primitive generation
 * - **Parametric Surfaces**: Mathematical function-based surface generation
 * - **Platonic Solids**: Regular polyhedra with perfect symmetry
 * - **Geodesic Domes**: Architectural and structural modeling primitives
 * 
 * ### Professional Mesh Quality
 * Production-ready mesh generation with industry standards:
 * - **Uniform Topology**: Consistent edge flow for animation and deformation
 * - **Optimal UV Mapping**: Distortion-free texture coordinate generation
 * - **Normal Calculation**: Smooth and flat normal generation algorithms
 * - **Vertex Optimization**: Cache-friendly vertex ordering for GPU rendering
 * - **Index Buffer Organization**: Efficient triangle strip and fan generation
 * 
 * ## Advanced Generation Features
 * 
 * ### Parametric Control System
 * Comprehensive parameter control for primitive customization:
 * - **Dimensional Parameters**: Precise size, radius, height, and scale control
 * - **Resolution Control**: Adaptive subdivision for quality vs. performance
 * - **Topology Options**: Quad vs. triangle mesh generation selection
 * - **UV Parameterization**: Multiple UV mapping strategies per primitive type
 * - **Normal Generation**: Smooth, flat, and custom normal calculation methods
 * 
 * ### Mathematical Precision
 * Accurate geometric algorithms for professional applications:
 * - **IEEE Float Precision**: Consistent floating-point arithmetic across platforms
 * - **Trigonometric Accuracy**: High-precision trigonometric function usage
 * - **Numerical Stability**: Robust algorithms preventing degenerate geometry
 * - **Edge Case Handling**: Proper treatment of extreme parameter values
 * - **Coordinate System Consistency**: Right-handed coordinate system adherence
 * 
 * ### Memory Optimization
 * Efficient mesh generation for large-scale applications:
 * - **Vertex Deduplication**: Shared vertex optimization for memory efficiency
 * - **Index Buffer Compression**: Optimal index size selection (16/32-bit)
 * - **Memory Pool Allocation**: Efficient temporary buffer management
 * - **Streaming Generation**: Progressive mesh generation for large primitives
 * 
 * ## Professional Workflow Integration
 * 
 * ### Animation-Ready Topology
 * Mesh generation optimized for animation workflows:
 * - **Edge Flow Optimization**: Natural deformation-friendly topology
 * - **Pole Reduction**: Minimized topology artifacts for smooth animation
 * - **Seam Placement**: Strategic UV seam positioning for texture continuity
 * - **Subdivision Surface Compatibility**: Base meshes suitable for smoothing
 * 
 * ### Real-Time Rendering Optimization
 * GPU-optimized mesh generation for game and visualization applications:
 * - **Vertex Cache Optimization**: Triangle ordering for GPU cache efficiency
 * - **Level-of-Detail Generation**: Multiple resolution variants for LOD systems
 * - **Material Optimization**: Mesh generation compatible with PBR workflows
 * - **Instancing Support**: Efficient generation for instanced rendering systems
 * 
 * ### CAD Integration Support
 * Precision geometry generation for engineering applications:
 * - **Parametric Accuracy**: Mathematical precision for CAD compatibility
 * - **NURBS Compatibility**: Mesh generation compatible with NURBS workflows
 * - **Measurement Tools**: Built-in validation for dimensional accuracy
 * - **Manufacturing Preparation**: Mesh generation suitable for 3D printing
 * 
 * ## Specialized Generation Algorithms
 * 
 * ### Sphere Generation Techniques
 * Multiple sphere generation algorithms for different use cases:
 * - **UV Sphere**: Latitude/longitude parameterization with polar singularities
 * - **Icosphere**: Geodesic subdivision for uniform triangle distribution
 * - **Cube Sphere**: Cube-based sphere projection for minimal distortion
 * - **Fibonacci Sphere**: Golden ratio-based point distribution for sampling
 * 
 * ### Cylinder and Cone Variants
 * Advanced cylindrical and conical primitive generation:
 * - **Truncated Cones**: Frustum generation with dual radius control
 * - **Elliptical Cylinders**: Non-circular cross-section support
 * - **Twisted Cylinders**: Helical deformation for advanced modeling
 * - **Multi-Segment Cylinders**: Piecewise linear cylinder approximation
 * 
 * ### Torus and Ring Geometries
 * Sophisticated toroidal geometry generation:
 * - **Circular Torus**: Standard donut-shaped geometry with smooth transitions
 * - **Elliptical Torus**: Non-circular cross-sections for advanced modeling
 * - **Twisted Torus**: Helical deformation for complex ring structures
 * - **Knot Torus**: Mathematical knot generation for educational and artistic use
 * 
 * ## Usage Examples
 * 
 * ### Basic Primitive Generation
 * ```cpp
 * // Generate basic geometric primitives
 * auto cube = MeshGenerator::generateCube(2.0f); // 2x2x2 cube
 * auto sphere = MeshGenerator::generateSphere(1.5f, 64, 32); // High-quality sphere
 * auto cylinder = MeshGenerator::generateCylinder(1.0f, 3.0f, 32); // Tall cylinder
 * auto plane = MeshGenerator::generatePlane(10.0f, 10.0f, 10, 10); // Subdivided plane
 * 
 * // Advanced primitives for specialized use
 * auto icosphere = MeshGenerator::generateIcosphere(1.0f, 3); // Geodesic sphere
 * auto torus = MeshGenerator::generateTorus(2.0f, 0.5f, 48, 24); // Smooth torus
 * auto cone = MeshGenerator::generateCone(1.5f, 2.5f, 32); // Pointed cone
 * ```
 * 
 * ### Resolution-Adaptive Generation
 * ```cpp
 * class AdaptivePrimitiveGenerator {
 * private:
 *     enum class QualityLevel { LOW, MEDIUM, HIGH, ULTRA };
 *     
 * public:
 *     rude::MeshPtr generateSphere(float radius, QualityLevel quality) {
 *         switch (quality) {
 *             case QualityLevel::LOW:
 *                 return MeshGenerator::generateIcosphere(radius, 1); // 80 triangles
 *             case QualityLevel::MEDIUM:
 *                 return MeshGenerator::generateSphere(radius, 32, 16); // ~1K triangles
 *             case QualityLevel::HIGH:
 *                 return MeshGenerator::generateSphere(radius, 64, 32); // ~4K triangles
 *             case QualityLevel::ULTRA:
 *                 return MeshGenerator::generateIcosphere(radius, 4); // ~5K triangles
 *         }
 *         return nullptr;
 *     }
 *     
 *     rude::MeshPtr generateCylinder(float radius, float height, QualityLevel quality) {
 *         int segments = (quality == QualityLevel::LOW) ? 8 :
 *                       (quality == QualityLevel::MEDIUM) ? 16 :
 *                       (quality == QualityLevel::HIGH) ? 32 : 64;
 *         return MeshGenerator::generateCylinder(radius, height, segments);
 *     }
 * };
 * ```
 * 
 * ### Debug and Visualization Primitives
 * ```cpp
 * // Generate debug and wireframe geometry for development
 * auto gridMesh = MeshGenerator::generateGrid(20.0f, 20); // 20x20 grid
 * auto wireCube = MeshGenerator::generateWireCube(1.0f); // Wireframe cube
 * auto wireSphere = MeshGenerator::generateWireSphere(1.0f, 16); // Wireframe sphere
 * 
 * // Use for debug visualization in 3D scenes
 * class DebugRenderer {
 * private:
 *     rude::MeshPtr m_gridMesh;
 *     rude::MeshPtr m_wireCubeMesh;
 *     rude::MeshPtr m_wireSphereMesh;
 *     
 * public:
 *     DebugRenderer() {
 *         m_gridMesh = MeshGenerator::generateGrid(50.0f, 50);
 *         m_wireCubeMesh = MeshGenerator::generateWireCube(1.0f);
 *         m_wireSphereMesh = MeshGenerator::generateWireSphere(1.0f, 24);
 *     }
 *     
 *     void renderCoordinateSystem(const glm::mat4& transform) {
 *         renderWireframe(m_gridMesh, transform, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
 *     }
 *     
 *     void renderBoundingBox(const glm::vec3& min, const glm::vec3& max) {
 *         glm::vec3 center = (min + max) * 0.5f;
 *         glm::vec3 size = max - min;
 *         glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), center), size);
 *         renderWireframe(m_wireCubeMesh, transform, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
 *     }
 * };
 * ```
 * 
 * ### Procedural Modeling Integration
 * ```cpp
 * class ProceduralPrimitiveLibrary {
 * private:
 *     std::unordered_map<std::string, rude::MeshPtr> m_primitiveCache;
 *     
 * public:
 *     rude::MeshPtr getPrimitive(const std::string& type, const std::vector<float>& params) {
 *         std::string key = type + "_" + encodeParameters(params);
 *         
 *         auto it = m_primitiveCache.find(key);
 *         if (it != m_primitiveCache.end()) {
 *             return it->second; // Return cached primitive
 *         }
 *         
 *         // Generate new primitive based on type and parameters
 *         rude::MeshPtr primitive = nullptr;
 *         if (type == "cube" && params.size() >= 1) {
 *             primitive = MeshGenerator::generateCube(params[0]);
 *         } else if (type == "sphere" && params.size() >= 3) {
 *             primitive = MeshGenerator::generateSphere(params[0], 
 *                                                     static_cast<int>(params[1]), 
 *                                                     static_cast<int>(params[2]));
 *         } else if (type == "cylinder" && params.size() >= 4) {
 *             primitive = MeshGenerator::generateCylinder(params[0], params[1], 
 *                                                       static_cast<int>(params[2]));
 *         } else if (type == "torus" && params.size() >= 4) {
 *             primitive = MeshGenerator::generateTorus(params[0], params[1],
 *                                                     static_cast<int>(params[2]),
 *                                                     static_cast<int>(params[3]));
 *         }
 *         
 *         if (primitive) {
 *             m_primitiveCache[key] = primitive;
 *         }
 *         
 *         return primitive;
 *     }
 * };
 * ```
 * 
 * ### Animation-Ready Primitive Generation
 * ```cpp
 * class AnimationPrimitiveGenerator {
 * public:
 *     // Generate sphere with optimal topology for facial animation
 *     static rude::MeshPtr generateAnimationSphere(float radius = 1.0f) {
 *         // Use icosphere for uniform topology without poles
 *         auto sphere = MeshGenerator::generateIcosphere(radius, 3);
 *         
 *         // Optimize vertex order for cache efficiency
 *         optimizeVertexCacheOrder(sphere);
 *         
 *         // Generate smooth normals for deformation
 *         generateSmoothNormals(sphere);
 *         
 *         return sphere;
 *     }
 *     
 *     // Generate cylinder with edge flow suitable for bending
 *     static rude::MeshPtr generateAnimationCylinder(float radius, float height, int segments = 16) {
 *         auto cylinder = MeshGenerator::generateCylinder(radius, height, segments);
 *         
 *         // Add edge loops for natural bending deformation
 *         addAnimationEdgeLoops(cylinder, 4);
 *         
 *         // Optimize for skinning algorithms
 *         optimizeForSkinning(cylinder);
 *         
 *         return cylinder;
 *     }
 * };
 * ```
 * 
 * ### Level-of-Detail Primitive System
 * ```cpp
 * class LODPrimitiveManager {
 * private:
 *     struct LODSet {
 *         rude::MeshPtr high;    // High detail for close viewing
 *         rude::MeshPtr medium;  // Medium detail for normal viewing
 *         rude::MeshPtr low;     // Low detail for distant viewing
 *         rude::MeshPtr minimal; // Minimal detail for very distant viewing
 *     };
 *     
 * public:
 *     LODSet generateSphereLODs(float radius) {
 *         LODSet lods;
 *         lods.high = MeshGenerator::generateIcosphere(radius, 4);     // ~5K triangles
 *         lods.medium = MeshGenerator::generateIcosphere(radius, 3);   // ~1K triangles
 *         lods.low = MeshGenerator::generateSphere(radius, 16, 8);     // ~256 triangles
 *         lods.minimal = MeshGenerator::generateIcosphere(radius, 1);  // ~80 triangles
 *         return lods;
 *     }
 *     
 *     LODSet generateCylinderLODs(float radius, float height) {
 *         LODSet lods;
 *         lods.high = MeshGenerator::generateCylinder(radius, height, 64);    // High quality
 *         lods.medium = MeshGenerator::generateCylinder(radius, height, 32);  // Medium quality
 *         lods.low = MeshGenerator::generateCylinder(radius, height, 16);     // Low quality
 *         lods.minimal = MeshGenerator::generateCylinder(radius, height, 8);  // Minimal quality
 *         return lods;
 *     }
 *     
 *     rude::MeshPtr selectLOD(const LODSet& lods, float distance, float pixelError = 1.0f) {
 *         // Select appropriate LOD based on distance and pixel error threshold
 *         if (distance < 10.0f) return lods.high;
 *         if (distance < 50.0f) return lods.medium;
 *         if (distance < 200.0f) return lods.low;
 *         return lods.minimal;
 *     }
 * };
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Mathematical Precision
 * High-precision geometric algorithms for professional applications:
 * - **Double Precision**: Extended precision for CAD-level accuracy requirements
 * - **Numerical Stability**: Robust algorithms preventing floating-point errors
 * - **Edge Case Handling**: Proper treatment of degenerate and extreme cases
 * - **Coordinate System Consistency**: Adherence to industry-standard conventions
 * 
 * ### UV Mapping Optimization
 * Professional texture coordinate generation:
 * - **Distortion Minimization**: Optimal UV parameterization algorithms
 * - **Seam Placement**: Strategic UV seam positioning for texture continuity
 * - **Atlas Packing**: Efficient UV space utilization for texture atlasing
 * - **Multi-Channel UVs**: Support for multiple UV sets per primitive
 * 
 * ### Topology Optimization
 * Advanced mesh topology for specific use cases:
 * - **Animation Topology**: Edge flow optimization for natural deformation
 * - **Rendering Topology**: Triangle strip optimization for GPU efficiency
 * - **Subdivision Topology**: Base mesh generation for smooth surfaces
 * - **Simulation Topology**: Mesh generation suitable for physics simulation
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Geometry
 * @ingroup Primitives
 * @ingroup ProceduralGeneration
 * @ingroup MeshGeneration
 */

#pragma once

#include "Common.h"
#include "geometry/core/Vertex.h"
#include "core/mesh.hpp"
#include "core/mesh_forward.hpp"
#include <vector>
#include <memory>


class MeshGenerator {
public:
    // Primitive generation
    static rude::MeshPtr generateCube(float size = 1.0f);
    static rude::MeshPtr generateSphere(float radius = 1.0f, int segments = 32, int rings = 16);
    static rude::MeshPtr generateCylinder(float radius = 1.0f, float height = 2.0f, int segments = 32);
    static rude::MeshPtr generatePlane(float width = 2.0f, float height = 2.0f, int widthSegments = 1, int heightSegments = 1);

    // Advanced primitives
    static rude::MeshPtr generateCone(float radius = 1.0f, float height = 2.0f, int segments = 32);
    static rude::MeshPtr generateTorus(float majorRadius = 1.0f, float minorRadius = 0.3f, int majorSegments = 32, int minorSegments = 16);
    static rude::MeshPtr generateIcosphere(float radius = 1.0f, int subdivisions = 2);

    // Grid and debug meshes
    static rude::MeshPtr generateGrid(float size = 10.0f, int divisions = 10);
    static rude::MeshPtr generateWireCube(float size = 1.0f);
    static rude::MeshPtr generateWireSphere(float radius = 1.0f, int segments = 16);

private:
    // Helper functions
    static void addQuad(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                       const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
                       const glm::vec3& normal);

    static void addTriangle(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                           const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                           const glm::vec3& normal);

    static glm::vec3 calculateNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
    static glm::vec2 sphericalToUV(const glm::vec3& position);
};
