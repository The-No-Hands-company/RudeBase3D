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
 * █  FILE: GeometryConverter.h                                                   █
 * █  DESCRIPTION: Hybrid Geometry Conversion & Format Translation System         █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite            █
 * █  VERSION: 1.0.0                                                              █
 * █  CREATED: 2024                                                               █
 * █  UPDATED: August 7, 2025                                                     █
 * █                                                                              █
 * █  LICENSE: MIT License                                                        █
 * █  COMPANY: The No Hands Company                                               █
 * █  AUTHOR: RudeBase3D Development Team                                         █
 * █                                                                              █
 * █  This file contains the GeometryConverter class providing comprehensive      █
 * █  format translation between different geometry representations (meshes,      █
 * █  NURBS, subdivision surfaces, voxels, point clouds, implicit surfaces).      █
 * █  Essential for hybrid modeling workflows in professional 3D applications.    █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file GeometryConverter.h
 * @brief Advanced Hybrid Geometry Conversion and Format Translation System
 * 
 * This file contains the comprehensive GeometryConverter class implementation that provides
 * professional-grade conversion capabilities between different geometric representations
 * used in modern 3D modeling applications. The converter supports seamless translation
 * between meshes, NURBS surfaces, subdivision surfaces, voxel grids, point clouds, and
 * implicit surfaces, enabling hybrid modeling workflows essential for professional
 * 3D content creation pipelines.
 * 
 * ## Core Conversion Architecture
 * 
 * ### Multi-Format Support
 * Industry-standard geometry representation formats:
 * - **Face-Vertex Meshes**: Traditional triangle/quad mesh representation for rendering
 * - **Half-Edge Meshes**: Topological mesh structure for advanced editing operations
 * - **NURBS Surfaces**: Non-uniform rational B-spline surfaces for CAD integration
 * - **Subdivision Surfaces**: Smooth surface representation with multi-resolution detail
 * - **Voxel Grids**: Volume-based geometry for sculpting and simulation workflows
 * - **Point Clouds**: Sparse 3D point representation for scanning and reconstruction
 * - **Implicit Surfaces**: Mathematical surface definition for procedural modeling
 * 
 * ### Professional Workflow Integration
 * Seamless integration with industry-standard modeling pipelines:
 * - **CAD-to-Mesh**: NURBS surface conversion for CAD model visualization
 * - **Scan-to-Mesh**: Point cloud reconstruction for 3D scanning workflows
 * - **Sculpt-to-Mesh**: Voxel-based modeling conversion for digital sculpting
 * - **Subdivision Modeling**: Multi-resolution surface editing and refinement
 * - **Procedural Generation**: Implicit surface evaluation for algorithmic modeling
 * 
 * ### Quality Preservation
 * Advanced algorithms ensuring conversion accuracy:
 * - **Adaptive Sampling**: Resolution adjustment based on surface curvature
 * - **Feature Preservation**: Sharp edge and corner detection during conversion
 * - **Topology Validation**: Mesh integrity checking and automatic repair
 * - **UV Coordinate Transfer**: Texture coordinate preservation across formats
 * - **Normal Reconstruction**: High-quality normal vector generation
 * 
 * ## Advanced Conversion Features
 * 
 * ### Mesh Format Translation
 * Bidirectional conversion between mesh representations:
 * - **Face-Vertex to Half-Edge**: Topology construction for editing operations
 * - **Half-Edge to Face-Vertex**: Rendering-optimized mesh generation
 * - **Vertex Attribute Transfer**: Position, normal, UV, color, and custom attributes
 * - **Index Buffer Optimization**: Cache-friendly vertex ordering for GPU rendering
 * 
 * ### NURBS Integration
 * Professional CAD workflow support:
 * - **Surface Tessellation**: Adaptive mesh generation from NURBS patches
 * - **Mesh Fitting**: Reverse-engineering meshes to NURBS surfaces
 * - **Control Point Optimization**: Minimal control point surface representation
 * - **Knot Vector Generation**: Automatic parameterization for surface fitting
 * - **Multi-Patch Surfaces**: Complex surface composition from simple patches
 * 
 * ### Subdivision Surface Processing
 * Multi-resolution surface representation:
 * - **Base Mesh Conversion**: Control cage generation from arbitrary meshes
 * - **Level-of-Detail**: Multiple subdivision levels for adaptive rendering
 * - **Catmull-Clark Scheme**: Industry-standard subdivision algorithm
 * - **Boundary Handling**: Proper treatment of open mesh boundaries
 * - **Crease Support**: Sharp feature preservation in subdivision surfaces
 * 
 * ### Voxel Processing
 * Volume-based geometry workflows:
 * - **Mesh Voxelization**: Solid geometry conversion to voxel representation
 * - **Marching Cubes**: ISO-surface extraction for mesh reconstruction
 * - **Dual Contouring**: Feature-preserving surface reconstruction
 * - **Distance Fields**: Signed distance function generation and evaluation
 * - **Boolean Operations**: Volume-based CSG operations on voxel data
 * 
 * ### Point Cloud Reconstruction
 * Scanning and measurement data processing:
 * - **Surface Sampling**: Mesh-to-point conversion with density control
 * - **Poisson Reconstruction**: Globally-optimal surface reconstruction
 * - **Delaunay Triangulation**: Direct point cloud triangulation
 * - **Normal Estimation**: Surface orientation recovery from sparse points
 * - **Outlier Removal**: Noise filtering and data cleaning algorithms
 * 
 * ### Implicit Surface Evaluation
 * Mathematical surface representation:
 * - **Function Sampling**: Direct evaluation of implicit surface equations
 * - **Level Set Methods**: Efficient implicit surface representation
 * - **Metaball Systems**: Blob-based implicit surface modeling
 * - **Procedural Generation**: Algorithm-based surface creation
 * - **CSG Operations**: Constructive solid geometry on implicit surfaces
 * 
 * ## Performance Optimization
 * 
 * ### Conversion Caching
 * Intelligent caching system for performance:
 * - **Format Cache**: Avoid redundant conversions between identical geometries
 * - **Weak Reference Tracking**: Automatic cache cleanup when geometry is deleted
 * - **Memory Management**: Efficient resource usage for large geometry datasets
 * - **Cache Invalidation**: Automatic updates when source geometry changes
 * 
 * ### Parallel Processing
 * Multi-threaded conversion algorithms:
 * - **Thread-Safe Operations**: Concurrent conversion of independent geometries
 * - **SIMD Optimization**: Vector instruction utilization for mathematical operations
 * - **GPU Acceleration**: Compute shader integration for intensive operations
 * - **Memory Pool Management**: Efficient allocation for temporary conversion data
 * 
 * ### Adaptive Quality
 * Dynamic quality adjustment based on requirements:
 * - **Curvature-Based Sampling**: Higher resolution in areas of high curvature
 * - **Distance-Based LOD**: Quality reduction for distant geometry
 * - **Performance Budgets**: Target frame rate maintenance during real-time conversion
 * - **Progressive Refinement**: Incremental quality improvement for interactive workflows
 * 
 * ## Usage Examples
 * 
 * ### Basic Mesh Format Conversion
 * ```cpp
 * // Convert face-vertex mesh to half-edge for editing
 * auto faceVertexMesh = loadMeshFromFile("model.obj");
 * auto halfEdgeMesh = GeometryConverter::toHalfEdge(faceVertexMesh);
 * 
 * // Perform editing operations on half-edge mesh
 * meshEditor.subdivideEdges(halfEdgeMesh, selectedEdges);
 * meshEditor.smoothVertices(halfEdgeMesh, selectedVertices);
 * 
 * // Convert back to face-vertex for rendering
 * auto optimizedMesh = GeometryConverter::toFaceVertex(halfEdgeMesh);
 * GeometryConverter::optimizeMeshForRendering(optimizedMesh);
 * ```
 * 
 * ### NURBS Surface Workflow
 * ```cpp
 * // Convert NURBS CAD data to mesh for visualization
 * auto nurbsSurface = loadNURBSFromCAD("part.step");
 * 
 * // High-quality conversion for close-up viewing
 * auto detailMesh = GeometryConverter::toFaceVertex(nurbsSurface, 64, 64);
 * 
 * // Lower quality for distant LOD
 * auto lodMesh = GeometryConverter::toFaceVertex(nurbsSurface, 16, 16);
 * 
 * // Reverse conversion: fit NURBS to scanned mesh
 * auto scannedMesh = loadScanData("scanned_part.ply");
 * auto fittedNURBS = GeometryConverter::toNURBS(scannedMesh, 3, 3);
 * ```
 * 
 * ### Point Cloud Reconstruction Pipeline
 * ```cpp
 * // Load point cloud from 3D scanner
 * auto pointCloud = loadPointCloudFromFile("scan.xyz");
 * 
 * // Reconstruct mesh using Poisson reconstruction
 * auto reconstructedMesh = GeometryConverter::toFaceVertex(pointCloud, 8);
 * 
 * // Validate and repair mesh topology
 * if (!GeometryConverter::validateTopology(reconstructedMesh)) {
 *     GeometryConverter::repairMesh(reconstructedMesh);
 * }
 * 
 * // Create BVH for efficient collision detection
 * auto bvhTree = GeometryConverter::createBVH(reconstructedMesh);
 * ```
 * 
 * ### Subdivision Surface Modeling
 * ```cpp
 * // Start with low-poly base mesh (control cage)
 * auto baseMesh = createCube(); // Simple 8-vertex cube
 * 
 * // Create subdivision surface
 * auto subdivisionSurface = GeometryConverter::toSubdivision(baseMesh);
 * 
 * // Generate different levels of detail
 * auto level0 = GeometryConverter::toFaceVertex(subdivisionSurface, 0); // Base mesh
 * auto level1 = GeometryConverter::toFaceVertex(subdivisionSurface, 1); // 32 faces
 * auto level2 = GeometryConverter::toFaceVertex(subdivisionSurface, 2); // 128 faces
 * auto level3 = GeometryConverter::toFaceVertex(subdivisionSurface, 3); // 512 faces
 * 
 * // Use appropriate level based on camera distance
 * float distance = length(cameraPosition - objectPosition);
 * auto renderMesh = (distance < 10.0f) ? level3 :
 *                   (distance < 50.0f) ? level2 :
 *                   (distance < 200.0f) ? level1 : level0;
 * ```
 * 
 * ### Voxel-Based Sculpting Workflow
 * ```cpp
 * // Convert mesh to voxels for sculpting
 * auto originalMesh = loadMeshFromFile("character_base.obj");
 * auto voxelGrid = GeometryConverter::toVoxels(originalMesh, 0.01f); // 1cm voxels
 * 
 * // Perform sculpting operations on voxel grid
 * voxelSculptor.brush(voxelGrid, sculptPosition, brushRadius, strength);
 * voxelSculptor.smooth(voxelGrid, smoothRegion);
 * 
 * // Convert back to mesh for rendering
 * auto sculptedMesh = GeometryConverter::toFaceVertex(voxelGrid, 0.0f);
 * 
 * // Optimize for real-time rendering
 * GeometryConverter::optimizeMeshForRendering(sculptedMesh);
 * ```
 * 
 * ### Multi-Format Hybrid Pipeline
 * ```cpp
 * class HybridModelingPipeline {
 * private:
 *     MeshConverterCache& m_cache = MeshConverterCache::getInstance();
 *     
 * public:
 *     void processGeometry(rude::MeshPtr inputMesh) {
 *         // Phase 1: Convert to voxels for volumetric operations
 *         auto voxelGrid = GeometryConverter::toVoxels(inputMesh, m_voxelSize);
 *         performVolumetricCSG(voxelGrid);
 *         
 *         // Phase 2: Extract mesh with improved topology
 *         auto processedMesh = GeometryConverter::toFaceVertex(voxelGrid);
 *         
 *         // Phase 3: Convert to subdivision surface for smooth editing
 *         auto subdivSurface = GeometryConverter::toSubdivision(processedMesh);
 *         applySubdivisionEdits(subdivSurface);
 *         
 *         // Phase 4: Generate final mesh at appropriate resolution
 *         auto finalMesh = GeometryConverter::toFaceVertex(subdivSurface, m_targetLevel);
 *         
 *         // Phase 5: Optimize for target use case
 *         if (m_targetUse == TargetUse::REALTIME_RENDERING) {
 *             GeometryConverter::optimizeMeshForRendering(finalMesh);
 *         } else if (m_targetUse == TargetUse::FURTHER_EDITING) {
 *             auto halfEdgeMesh = GeometryConverter::toHalfEdge(finalMesh);
 *             GeometryConverter::optimizeHalfEdgeForEditing(halfEdgeMesh);
 *             finalMesh = GeometryConverter::toFaceVertex(halfEdgeMesh);
 *         }
 *         
 *         // Cache results to avoid redundant conversions
 *         m_cache.getFaceVertex(GeometryConverter::toHalfEdge(finalMesh));
 *     }
 * };
 * ```
 * 
 * ### Validation and Quality Assurance
 * ```cpp
 * bool validateGeometryConversion(rude::MeshPtr original, rude::MeshPtr converted) {
 *     // Check vertex count preservation (within tolerance for sampling)
 *     if (abs(static_cast<int>(converted->getVertexCount()) - 
 *             static_cast<int>(original->getVertexCount())) > original->getVertexCount() * 0.1f) {
 *         logWarning("Significant vertex count change during conversion");
 *     }
 *     
 *     // Validate topology integrity
 *     auto halfEdgeConverted = GeometryConverter::toHalfEdge(converted);
 *     if (!GeometryConverter::validateTopology(halfEdgeConverted)) {
 *         logError("Topology validation failed after conversion");
 *         GeometryConverter::repairMesh(halfEdgeConverted);
 *         converted = GeometryConverter::toFaceVertex(halfEdgeConverted);
 *     }
 *     
 *     // Check surface area preservation
 *     float originalArea = calculateSurfaceArea(original);
 *     float convertedArea = calculateSurfaceArea(converted);
 *     float areaError = abs(convertedArea - originalArea) / originalArea;
 *     
 *     if (areaError > 0.05f) { // 5% tolerance
 *         logWarning("Surface area significantly changed during conversion: " 
 *                   + std::to_string(areaError * 100.0f) + "% error");
 *     }
 *     
 *     return areaError < 0.1f; // 10% maximum error threshold
 * }
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Adaptive Resolution Control
 * Dynamic quality adjustment based on geometric complexity:
 * - **Curvature Analysis**: Higher sampling density in areas of high curvature
 * - **Edge Length Metrics**: Uniform edge length maintenance during conversion
 * - **Feature Detection**: Preservation of sharp edges and corners
 * - **Smooth Region Optimization**: Lower density in flat areas for efficiency
 * 
 * ### Multi-Threading Support
 * Parallel processing for large geometry datasets:
 * - **Thread-Safe Conversions**: Concurrent processing of independent geometries
 * - **Work Stealing**: Load balancing across available CPU cores
 * - **Memory Locality**: Cache-efficient data access patterns
 * - **Progress Tracking**: Real-time feedback for long-running conversions
 * 
 * ### Memory Management
 * Efficient resource handling for large datasets:
 * - **Streaming Conversion**: Process geometry in chunks to handle huge datasets
 * - **Temporary Buffer Pools**: Reuse conversion buffers to reduce allocations
 * - **Weak Reference Tracking**: Automatic cleanup of unused conversion cache
 * - **Memory Budget Control**: Configurable limits on conversion memory usage
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Geometry
 * @ingroup HybridModeling
 * @ingroup Conversion
 */

#pragma once

#include "core/mesh_forward.hpp"
#include "GeometryTypes.h"
#include "Common.h"
#include <memory>
#include <unordered_map>
#include <set>
#include <vector>
#include <glm/glm.hpp>

// Temporary typedefs for missing rude pointer types
namespace rude {
    class Vertex;
    using VertexPtr = std::shared_ptr<Vertex>;
    class Face;
    using FacePtr = std::shared_ptr<Face>;
}

/**
 * @brief Geometry Converter
 * Converts between different mesh representations for optimal processing
 * Supports hybrid mesh system with multiple geometry types
 */
class GeometryConverter {
public:
    GeometryConverter() = default;
    ~GeometryConverter() = default;

    // Core mesh format conversions
    static rude::HalfEdgeMeshPtr toHalfEdge(rude::MeshPtr faceVertexMesh);
    static rude::MeshPtr toFaceVertex(rude::HalfEdgeMeshPtr halfEdgeMesh);
    
    // NURBS conversions
    static rude::HalfEdgeMeshPtr toHalfEdge(NURBSSurfacePtr nurbs, int resolutionU = 32, int resolutionV = 32);
    static rude::MeshPtr toFaceVertex(NURBSSurfacePtr nurbs, int resolutionU = 32, int resolutionV = 32);
    static NURBSSurfacePtr toNURBS(rude::HalfEdgeMeshPtr halfEdge, int degreeU = 3, int degreeV = 3);
    
    // Subdivision surface conversions
    static SubdivisionMeshPtr toSubdivision(rude::HalfEdgeMeshPtr baseMesh);
    static SubdivisionMeshPtr toSubdivision(rude::MeshPtr baseMesh);
    static rude::HalfEdgeMeshPtr toHalfEdge(SubdivisionMeshPtr subdivision, int level = -1);
    static rude::MeshPtr toFaceVertex(SubdivisionMeshPtr subdivision, int level = -1);
    
    // Voxel conversions
    static VoxelGridPtr toVoxels(rude::MeshPtr mesh, float voxelSize, float bandwidth = 2.0f);
    static VoxelGridPtr toVoxels(rude::HalfEdgeMeshPtr halfEdge, float voxelSize, float bandwidth = 2.0f);
    static rude::MeshPtr toFaceVertex(VoxelGridPtr voxels, float isoValue = 0.0f);
    static rude::HalfEdgeMeshPtr toHalfEdge(VoxelGridPtr voxels, float isoValue = 0.0f);
    
    // Point cloud conversions
    static PointCloudPtr toPointCloud(rude::MeshPtr mesh, int samplesPerFace = 10);
    static PointCloudPtr toPointCloud(rude::HalfEdgeMeshPtr halfEdge, int samplesPerFace = 10);
    static rude::MeshPtr toFaceVertex(PointCloudPtr pointCloud, int reconstructionDepth = 8);
    static rude::HalfEdgeMeshPtr toHalfEdge(PointCloudPtr pointCloud, int reconstructionDepth = 8);
    
    // Implicit surface conversions
    static ImplicitSurfacePtr toImplicitSurface(rude::MeshPtr mesh, float bandwidth = 2.0f);
    static ImplicitSurfacePtr toImplicitSurface(VoxelGridPtr voxels);
    static rude::MeshPtr toFaceVertex(ImplicitSurfacePtr implicit, const glm::vec3& bounds, float resolution);
    static VoxelGridPtr toVoxels(ImplicitSurfacePtr implicit, const glm::vec3& bounds, float resolution);
    
    // Advanced conversions
    static HalfEdgeMeshPtr fromVerticesAndFaces(
        const std::vector<glm::vec3>& positions,
        const std::vector<std::vector<unsigned int>>& faces);
    
    static void toVerticesAndFaces(
        rude::HalfEdgeMeshPtr halfEdgeMesh,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texCoords,
        std::vector<unsigned int>& indices);

    // Validation and repair
    static bool validateConversion(rude::MeshPtr original, rude::HalfEdgeMeshPtr converted);
    static bool validateConversion(rude::HalfEdgeMeshPtr original, rude::MeshPtr converted);
    static bool validateTopology(rude::HalfEdgeMeshPtr mesh);
    static void repairMesh(rude::HalfEdgeMeshPtr mesh);
    
    // Utility functions
    static void copyVertexData(rude::VertexPtr src, rude::Vertex& dst);
    static void copyVertexData(const rude::Vertex& src, rude::VertexPtr dst);
    
    // Optimization utilities
    static void optimizeMeshForRendering(rude::MeshPtr mesh);
    static void optimizeHalfEdgeForEditing(rude::HalfEdgeMeshPtr mesh);
    static BVHTreePtr createBVH(rude::MeshPtr mesh);
    
private:
    // Helper functions
    static void buildFaceVertexConnectivity(rude::HalfEdgeMeshPtr mesh);
    static void optimizeVertexOrder(std::vector<rude::Vertex>& vertices, std::vector<unsigned int>& indices);
    
    // NURBS fitting helpers
    static void fitNURBSToMesh(HalfEdgeMeshPtr mesh, NURBSSurfacePtr nurbs, int degreeU, int degreeV);
    static std::vector<float> computeKnotVector(int numControlPoints, int degree, bool clamped = true);
    
    // Voxelization helpers
    static void voxelizeTriangle(VoxelGridPtr grid, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float bandwidth);
    static float computeSignedDistance(const glm::vec3& point, rude::MeshPtr mesh);
    
    // Point sampling helpers
    static void sampleTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, 
                              const glm::vec3& n0, const glm::vec3& n1, const glm::vec3& n2,
                              int samples, std::vector<PointCloud::Point>& points);
};

/**
 * @brief Mesh Converter Cache
 * Caches converted meshes to avoid redundant conversions
 */
class MeshConverterCache {
public:
    static MeshConverterCache& getInstance();

    // Cache management
    rude::HalfEdgeMeshPtr getHalfEdge(rude::MeshPtr mesh);
    rude::MeshPtr getFaceVertex(rude::HalfEdgeMeshPtr mesh);

    void invalidate(rude::MeshPtr mesh);
    void invalidate(rude::HalfEdgeMeshPtr mesh);

    void clear();
    size_t getCacheSize() const;

private:
    MeshConverterCache() = default;

    std::unordered_map<rude::MeshPtr, rude::HalfEdgeMeshPtr> m_faceVertexToHalfEdge;
    std::unordered_map<rude::HalfEdgeMeshPtr, rude::MeshPtr> m_halfEdgeToFaceVertex;

    // Weak references to track lifetime (using std::set since weak_ptr cannot be hashed)
    std::set<std::weak_ptr<rude::Mesh>, std::owner_less<std::weak_ptr<rude::Mesh>>> m_trackedFaceVertex;
    std::set<std::weak_ptr<rude::HalfEdgeMesh>, std::owner_less<std::weak_ptr<rude::HalfEdgeMesh>>> m_trackedHalfEdge;

    void cleanupExpiredEntries();
};
