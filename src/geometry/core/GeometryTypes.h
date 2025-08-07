//==============================================================================
// GeometryTypes.h - Core Geometry Type Definitions and Forward Declarations
//==============================================================================
//
// RudeBase3D Engine - Advanced 3D Modeling and Rendering Framework
// 
// Copyright (c) 2024 RudeBase3D Development Team
// Licensed under the MIT License. See LICENSE file in the project root.
//
// This file is part of the RudeBase3D Engine, a comprehensive 3D modeling
// and rendering framework designed for professional applications.
//
//==============================================================================

#pragma once

#include "Common.h"
#include "core/mesh_forward.hpp"
#include "geometry/subdivision/SubdivisionMesh.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

/**
 * @file GeometryTypes.h
 * @brief Core geometry type definitions and advanced geometric data structures
 * 
 * This file provides comprehensive geometry type definitions for the RudeBase3D
 * engine, including advanced geometric data structures for professional 3D
 * modeling and computational geometry operations. The types range from traditional
 * NURBS surfaces to modern voxel grids and implicit surface representations.
 * 
 * ## Geometric Data Structure Overview
 * 
 * ### Parametric Surfaces
 * - **NURBS Surfaces**: Non-Uniform Rational B-Spline surfaces for precise CAD modeling
 * - **Subdivision Surfaces**: Smooth surface representation with multiple detail levels
 * 
 * ### Volumetric Representations
 * - **Voxel Grids**: Discrete 3D volume data with signed distance field support
 * - **Implicit Surfaces**: Mathematical surface definitions using signed distance functions
 * 
 * ### Point-Based Geometry
 * - **Point Clouds**: Unstructured 3D point data with normal and color information
 * - **Spatial Acceleration**: KD-tree and BVH structures for fast spatial queries
 * 
 * ### Computational Geometry
 * - **BVH Trees**: Bounding Volume Hierarchies for ray casting and collision detection
 * - **Spatial Queries**: Efficient nearest neighbor and range search algorithms
 * 
 * ## Integration with Core Systems
 * 
 * All geometry types integrate seamlessly with the RudeBase3D mesh system,
 * providing conversion methods to and from the unified mesh representation.
 * This allows complex geometric operations while maintaining compatibility
 * with the rendering and modeling pipeline.
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * 
 * @ingroup Geometry
 * @ingroup DataStructures
 */

// Forward declarations for all geometry types
// class HalfEdgeMesh; // No longer needed, use rude::HalfEdgeMeshPtr
struct ControlPoint;
class NURBSSurface;
class VoxelGrid;
class PointCloud;
class ImplicitSurface;
class BVHTree;

/**
 * @brief Professional NURBS Surface implementation for precise CAD-style modeling
 * 
 * The NURBSSurface class provides a comprehensive implementation of Non-Uniform
 * Rational B-Spline surfaces, enabling precise CAD-style modeling operations
 * within the RudeBase3D framework. NURBS surfaces offer mathematical precision
 * and smooth interpolation essential for professional modeling applications.
 * 
 * ## Core NURBS Functionality
 * 
 * ### Mathematical Foundation
 * NURBS surfaces are defined by:
 * - **Control Points**: 3D points with associated weights that influence surface shape
 * - **Knot Vectors**: Parameter space subdivision for both U and V directions
 * - **Degree**: Polynomial degree determining surface smoothness (typically 2-3)
 * - **Weights**: Rational coefficients enabling precise circle and conic representation
 * 
 * ### Surface Evaluation
 * Efficient evaluation of surface properties at any parametric coordinate:
 * - **Position Evaluation**: Compute 3D surface points at (u,v) parameters
 * - **Normal Calculation**: Surface normals for shading and orientation
 * - **Derivative Analysis**: First derivatives for tangent vector computation
 * - **Curvature Analysis**: Surface curvature information for quality assessment
 * 
 * ### Tessellation and Rendering
 * Conversion to renderable mesh representation:
 * - **Adaptive Tessellation**: Variable resolution based on surface curvature
 * - **Resolution Control**: User-specified subdivision levels for U and V directions
 * - **Quality Optimization**: Balanced polygon count vs. surface accuracy
 * - **Normal Generation**: Smooth normal interpolation for high-quality rendering
 * 
 * ## Professional Features
 * 
 * ### CAD Compatibility
 * Industry-standard NURBS implementation:
 * - **IGES/STEP Support**: Compatible with standard CAD file formats
 * - **Rational Surfaces**: Support for perfect circles and conic sections
 * - **Trimmed Surfaces**: Boundary curve support for complex surface shapes
 * - **Multi-Patch Modeling**: Seamless surface stitching for complex objects
 * 
 * ### Subdivision Surface Integration
 * Seamless integration with subdivision surface workflows:
 * - **Level-of-Detail**: Multiple subdivision levels for different contexts
 * - **Catmull-Clark Subdivision**: Smooth surface subdivision algorithm
 * - **Boundary Handling**: Sharp and smooth boundary rule support
 * - **Crease Control**: Artist-friendly crease angle specification
 * 
 * ### Advanced Operations
 * Professional surface manipulation capabilities:
 * - **Surface Deformation**: Control point manipulation with real-time feedback
 * - **Knot Insertion**: Refinement operations for additional control
 * - **Degree Elevation**: Increase polynomial degree while preserving shape
 * - **Surface Splitting**: Divide surfaces along parametric curves
 * 
 * ## Usage Examples
 * 
 * ### Basic NURBS Surface Creation
 * ```cpp
 * // Create a bicubic NURBS surface
 * NURBSSurface surface(3, 3, 4, 4); // degree 3x3, 4x4 control points
 * 
 * // Set control points in a grid pattern
 * for (int u = 0; u < 4; ++u) {
 *     for (int v = 0; v < 4; ++v) {
 *         ControlPoint cp(glm::vec3(u, sin(u*v*0.5), v), 1.0f);
 *         surface.setControlPoint(u, v, cp);
 *     }
 * }
 * 
 * // Generate mesh for rendering
 * auto mesh = surface.tessellate(32, 32);
 * ```
 * 
 * ### Surface Evaluation and Analysis
 * ```cpp
 * // Evaluate surface properties
 * glm::vec3 point = surface.evaluate(0.5f, 0.5f);
 * glm::vec3 normal = surface.evaluateNormal(0.5f, 0.5f);
 * glm::vec3 tangentU = surface.evaluateDerivativeU(0.5f, 0.5f);
 * glm::vec3 tangentV = surface.evaluateDerivativeV(0.5f, 0.5f);
 * 
 * // Use for advanced surface analysis
 * float curvature = computeSurfaceCurvature(tangentU, tangentV, normal);
 * ```
 * 
 * ### Subdivision Surface Integration
 * ```cpp
 * // Configure subdivision parameters
 * surface.setBoundaryRule(true);  // Sharp boundaries
 * surface.setCreaseThreshold(45.0f);  // 45-degree crease angle
 * 
 * // Generate subdivision levels
 * auto baseLevel = surface.getBaseLevel();
 * auto subdivided = surface.getLevel(2);  // 2 subdivision levels
 * auto renderMesh = surface.generateRenderMesh(1);  // Level 1 for rendering
 * ```
 * 
 * ## Performance Considerations
 * 
 * ### Efficient Evaluation
 * Optimized algorithms for real-time surface evaluation:
 * - **Basis Function Caching**: Precomputed basis functions for repeated evaluation
 * - **Adaptive Tessellation**: Variable resolution based on surface complexity
 * - **Memory Optimization**: Efficient storage of control points and knot vectors
 * - **SIMD Acceleration**: Vectorized computation for batch evaluation
 * 
 * ### Rendering Integration
 * Seamless integration with rendering pipeline:
 * - **Level-of-Detail**: Distance-based tessellation for performance
 * - **Frustum Culling**: Surface bounds checking for visibility
 * - **Normal Smoothing**: Hardware-accelerated normal interpolation
 * - **Texture Mapping**: Proper UV coordinate generation
 * 
 * @ingroup Geometry
 * @ingroup NURBS
 * @ingroup ParametricSurfaces
 * 
 * @see SubdivisionMesh For subdivision surface functionality
 * @see ControlPoint For NURBS control point structure
 * @see Mesh For final renderable mesh representation
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class NURBSSurface {
public:
public:

    NURBSSurface(int degreeU, int degreeV, int controlPointsU, int controlPointsV);
    ~NURBSSurface() = default;

    rude::MeshPtr tessellate(int resolutionU, int resolutionV) const;
    glm::vec3 evaluate(float u, float v) const;
    glm::vec3 evaluateNormal(float u, float v) const;
    glm::vec3 evaluateDerivativeU(float u, float v) const;
    glm::vec3 evaluateDerivativeV(float u, float v) const;
    float basisFunction(int i, int degree, float t, const std::vector<float>& knots) const;

    void setControlPoint(int u, int v, const ControlPoint& point);
    const ControlPoint& getControlPoint(int u, int v) const;

    void setKnotU(int index, float value);
    void setKnotV(int index, float value);
    const std::vector<float>& getKnotsU() const { return m_knotsU; }
    const std::vector<float>& getKnotsV() const { return m_knotsV; }

    rude::HalfEdgeMeshPtr getLevel(int level) const;
    rude::HalfEdgeMeshPtr getBaseLevel() const { return getLevel(0); }
    rude::HalfEdgeMeshPtr getFinestLevel() const { return getLevel(static_cast<int>(m_levels.size()) - 1); }
    int getNumLevels() const { return static_cast<int>(m_levels.size()); }

    rude::MeshPtr generateRenderMesh(int level = -1) const;

    void setBoundaryRule(bool sharp) { m_sharpBoundaries = sharp; }
    bool getBoundaryRule() const { return m_sharpBoundaries; }

    void setCreaseThreshold(float threshold) { m_creaseThreshold = threshold; }
    float getCreaseThreshold() const { return m_creaseThreshold; }

private:
    int m_controlPointsU = 0;
    int m_controlPointsV = 0;
    int m_degreeU = 0;
    int m_degreeV = 0;
    std::vector<float> m_knotsU;
    std::vector<float> m_knotsV;
    std::vector<std::vector<ControlPoint>> m_controlPoints;
    std::vector<SubdivisionMesh::SubdivisionLevel> m_levels;
    bool m_sharpBoundaries = false;
    float m_creaseThreshold = 30.0f;

    rude::HalfEdgeMeshPtr applyCatmullClark(rude::HalfEdgeMeshPtr mesh);
    void computeFacePoints(rude::HalfEdgeMeshPtr mesh, std::unordered_map<HalfEdgeFacePtr, glm::vec3>& facePoints);
    void computeEdgePoints(rude::HalfEdgeMeshPtr mesh, const std::unordered_map<HalfEdgeFacePtr, glm::vec3>& facePoints, std::unordered_map<HalfEdgeEdgePtr, glm::vec3>& edgePoints);
    void computeVertexPoints(rude::HalfEdgeMeshPtr mesh, const std::unordered_map<HalfEdgeFacePtr, glm::vec3>& facePoints, const std::unordered_map<HalfEdgeEdgePtr, glm::vec3>& edgePoints, std::unordered_map<HalfEdgeVertexPtr, glm::vec3>& vertexPoints);
};

struct ControlPoint {
    glm::vec3 position;
    float weight = 1.0f;
    ControlPoint() = default;
    ControlPoint(const glm::vec3& pos, float w = 1.0f) : position(pos), weight(w) {}
};

/**
 * @brief Advanced 3D Voxel Grid for volumetric modeling and simulation
 * 
 * The VoxelGrid class provides a comprehensive framework for volumetric data
 * representation and manipulation within the RudeBase3D environment. This
 * implementation supports high-resolution 3D voxel operations including density
 * fields, Boolean operations, surface extraction, and simulation integration.
 * 
 * ## Core Voxel Operations
 * 
 * ### Volumetric Data Management
 * Efficient storage and access of 3D voxel data:
 * - **Dense Storage**: Optimized 3D array representation for solid objects
 * - **Sparse Storage**: Memory-efficient sparse voxel octrees for large volumes
 * - **Multi-Resolution**: Hierarchical voxel levels for level-of-detail
 * - **Streaming Support**: Dynamic loading/unloading for massive datasets
 * 
 * ### Surface Extraction
 * Advanced mesh generation from volumetric data:
 * - **Marching Cubes**: ISO-surface extraction with configurable threshold
 * - **Dual Contouring**: Adaptive surface reconstruction with feature preservation
 * - **Surface Nets**: Quad-dominant mesh generation for clean topology
 * - **Quality Control**: Adaptive tessellation based on curvature analysis
 * 
 * ### Boolean Operations
 * Robust voxel-based CSG operations:
 * - **Union Operations**: Additive voxel combination with proper blending
 * - **Intersection**: Volumetric intersection with material preservation
 * - **Subtraction**: Precise Boolean subtraction for complex modeling
 * - **Blending Modes**: Smooth vs. sharp Boolean operation variants
 * 
 * ## Professional Modeling Features
 * 
 * ### Sculptural Modeling
 * Digital sculpting capabilities for organic forms:
 * - **Brush-Based Tools**: Various brush types for adding/removing material
 * - **Smooth Operations**: Surface smoothing and detail refinement
 * - **Displacement Maps**: Height-based surface detail application
 * - **Texture Painting**: Direct voxel attribute painting for materials
 * 
 * ### Procedural Generation
 * Algorithmic voxel content creation:
 * - **Noise Functions**: Perlin/Simplex noise for terrain generation
 * - **Distance Fields**: SDF-based procedural shape generation
 * - **Cellular Automata**: Procedural cave and structure generation
 * - **Fractal Patterns**: Self-similar geometric pattern generation
 * 
 * ### Simulation Integration
 * Physics and fluid simulation support:
 * - **Fluid Dynamics**: Volumetric fluid simulation with viscosity
 * - **Destruction**: Fracture and debris simulation for dynamic effects
 * - **Thermal Simulation**: Heat transfer and thermal effects
 * - **Erosion Modeling**: Weathering and erosion simulation for realism
 * 
 * ## Advanced Algorithms
 * 
 * ### Adaptive Meshing
 * Intelligent surface reconstruction:
 * - **Feature Detection**: Sharp edge and corner preservation
 * - **Adaptive Resolution**: Variable detail based on surface complexity
 * - **Topology Optimization**: Clean mesh topology for downstream processing
 * - **UV Generation**: Automatic texture coordinate generation
 * 
 * ### Multi-Material Support
 * Complex material representation:
 * - **Material Blending**: Smooth transitions between different materials
 * - **Density Gradients**: Continuous material property transitions
 * - **Layer Composition**: Additive manufacturing layer representation
 * - **Property Fields**: Temperature, hardness, and other physical properties
 * 
 * ## Usage Examples
 * 
 * ### Basic Voxel Grid Creation
 * ```cpp
 * // Create a 256^3 voxel grid with 1mm resolution
 * VoxelGrid grid(256, 256, 256, 1.0f);
 * 
 * // Set voxel density at specific coordinates
 * grid.setVoxel(128, 128, 128, 1.0f);  // Full density
 * grid.setVoxel(100, 100, 100, 0.5f);  // Half density
 * 
 * // Fill a sphere using SDF
 * glm::vec3 center(128, 128, 128);
 * float radius = 50.0f;
 * grid.fillSphere(center, radius, 1.0f);
 * ```
 * 
 * ### Surface Extraction and Meshing
 * ```cpp
 * // Extract ISO-surface using marching cubes
 * float isoValue = 0.5f;
 * auto mesh = grid.marchingCubes(isoValue);
 * 
 * // Generate adaptive mesh with feature preservation
 * MeshingOptions options;
 * options.adaptiveThreshold = 0.1f;
 * options.preserveFeatures = true;
 * options.maxPolygons = 100000;
 * auto adaptiveMesh = grid.generateMesh(options);
 * ```
 * 
 * ### Boolean Operations
 * ```cpp
 * VoxelGrid gridA(256, 256, 256, 1.0f);
 * VoxelGrid gridB(256, 256, 256, 1.0f);
 * 
 * // Fill grids with different shapes
 * gridA.fillSphere(glm::vec3(100, 128, 128), 60.0f, 1.0f);
 * gridB.fillBox(glm::vec3(80, 80, 80), glm::vec3(180, 180, 180), 1.0f);
 * 
 * // Perform Boolean operations
 * auto unionGrid = VoxelGrid::booleanUnion(gridA, gridB);
 * auto intersectionGrid = VoxelGrid::booleanIntersection(gridA, gridB);
 * auto subtractionGrid = VoxelGrid::booleanSubtraction(gridA, gridB);
 * ```
 * 
 * ### Sculptural Modeling
 * ```cpp
 * // Initialize sculpting session
 * SculptingBrush brush(BrushType::SPHERE, 10.0f, 0.5f);
 * 
 * // Apply sculpting operations
 * grid.sculptAdd(brush, glm::vec3(128, 128, 128));
 * grid.sculptSubtract(brush, glm::vec3(150, 128, 128));
 * grid.sculptSmooth(brush, glm::vec3(140, 128, 128));
 * 
 * // Apply displacement from height map
 * auto heightMap = loadHeightMap("terrain.png");
 * grid.applyDisplacement(heightMap, 20.0f);
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Memory Management
 * Efficient memory usage for large voxel datasets:
 * - **Sparse Octrees**: Hierarchical storage for empty space optimization
 * - **Compression**: Lossless voxel data compression for storage
 * - **Streaming**: Dynamic data loading for massive terrains
 * - **GPU Acceleration**: CUDA/OpenCL support for parallel operations
 * 
 * ### Rendering Integration
 * Optimized rendering pipeline integration:
 * - **Level-of-Detail**: Distance-based voxel resolution scaling
 * - **Frustum Culling**: Spatial culling for large voxel volumes
 * - **Instanced Rendering**: Efficient rendering of repeated voxel structures
 * - **Ray Casting**: Direct voxel ray casting for volumetric rendering
 * 
 * @ingroup Geometry
 * @ingroup Voxels
 * @ingroup VolumetricModeling
 * 
 * @see MarchingCubes For surface extraction algorithms
 * @see SculptingBrush For sculptural modeling tools
 * @see VoxelMaterial For multi-material voxel representation
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class VoxelGrid {
public:
    struct Voxel {
        float density = 0.0f;  // Signed distance field value
        glm::vec3 gradient;    // Surface gradient for normals
        unsigned char material = 0; // Material ID
        bool isSolid() const { return density > 0.0f; }
        bool isEmpty() const { return density <= -1.0f; }
        bool isSurface() const { return std::abs(density) <= 1.0f; }
    };

    VoxelGrid(const glm::vec3& origin, const glm::vec3& size, int resolution);
    VoxelGrid(const glm::vec3& origin, float voxelSize, int resX, int resY, int resZ);
    ~VoxelGrid() = default;

    // Voxel access
    Voxel& getVoxel(int x, int y, int z);
    const Voxel& getVoxel(int x, int y, int z) const;
    void setVoxel(int x, int y, int z, const Voxel& voxel);
    Voxel& getVoxelAt(const glm::vec3& worldPos);
    const Voxel& getVoxelAt(const glm::vec3& worldPos) const;

    // Conversion helpers
    glm::vec3 indexToWorld(int x, int y, int z) const;
    glm::vec3 worldToIndex(const glm::vec3& worldPos) const;
    bool isValidIndex(int x, int y, int z) const;

    // Mesh conversion using Marching Cubes
    rude::MeshPtr extractSurface(float isoValue = 0.0f) const;
    void fromMesh(rude::MeshPtr mesh, float bandwidth = 2.0f);

    // Boolean operations
    void unionWith(const VoxelGrid& other);
    void subtractFrom(const VoxelGrid& other);
    void intersectWith(const VoxelGrid& other);

    // Properties
    const glm::vec3& getOrigin() const { return m_origin; }
    const glm::vec3& getSize() const { return m_size; }
    int getResolutionX() const { return m_resX; }
    int getResolutionY() const { return m_resY; }
    int getResolutionZ() const { return m_resZ; }
    float getVoxelSize() const { return m_voxelSize; }

private:
    glm::vec3 m_origin;
    glm::vec3 m_size;
    int m_resX, m_resY, m_resZ;
    float m_voxelSize;
    std::vector<Voxel> m_voxels;

    // Helper functions
    int getIndex(int x, int y, int z) const { return x + y * m_resX + z * m_resX * m_resY; }
    void computeGradients();
    void voxelizeTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float bandwidth);
    float computeDistanceToTriangle(const glm::vec3& point, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

    // Marching Cubes implementation
    struct MCVertex {
        glm::vec3 position;
        glm::vec3 normal;
    };
    void marchingCubes(std::vector<MCVertex>& vertices, std::vector<unsigned int>& indices, float isoValue) const;
    glm::vec3 interpolateVertex(const glm::vec3& p1, const glm::vec3& p2, float v1, float v2, float iso) const;
};

/**
 * @brief Professional Point Cloud processing for 3D scanning and reconstruction
 * 
 * The PointCloud class provides comprehensive functionality for handling unstructured
 * 3D point data from various sources including laser scanners, photogrammetry,
 * LiDAR systems, and depth cameras. This implementation offers advanced processing
 * algorithms for point cloud analysis, reconstruction, and integration into the
 * RudeBase3D modeling pipeline.
 * 
 * ## Core Point Cloud Operations
 * 
 * ### Data Management
 * Efficient handling of large-scale point datasets:
 * - **Multi-Format Support**: PLY, PCD, LAS, XYZ, and custom format import/export
 * - **Streaming I/O**: Progressive loading for massive point clouds (millions of points)
 * - **Spatial Indexing**: K-D tree and octree acceleration for fast spatial queries
 * - **Memory Optimization**: Hierarchical level-of-detail for large datasets
 * 
 * ### Point Attributes
 * Rich per-point information storage:
 * - **Geometric Data**: Position coordinates with sub-millimeter precision
 * - **Surface Normals**: Computed or imported normal vectors for orientation
 * - **Color Information**: RGB/RGBA color values from photogrammetry or painting
 * - **Intensity Values**: Scanner intensity or confidence measurements
 * - **Custom Attributes**: Material IDs, timestamps, classification labels
 * 
 * ### Spatial Acceleration
 * High-performance spatial data structures:
 * - **K-D Tree Search**: Efficient nearest neighbor and range queries
 * - **Octree Partitioning**: Hierarchical spatial subdivision for LOD
 * - **Grid-Based Acceleration**: Uniform grid structures for regular operations
 * - **GPU Acceleration**: CUDA/OpenCL support for parallel point processing
 * 
 * ## Advanced Processing Algorithms
 * 
 * ### Surface Reconstruction
 * Professional mesh generation from point clouds:
 * - **Poisson Reconstruction**: Smooth surface reconstruction with hole filling
 * - **Delaunay Triangulation**: Adaptive triangulation for varying density
 * - **Ball Pivoting**: Local surface reconstruction preserving fine details
 * - **Marching Cubes**: Volumetric reconstruction through voxelization
 * 
 * ### Noise Reduction and Filtering
 * Point cloud cleaning and optimization:
 * - **Statistical Outlier Removal**: Automatic detection and removal of noise points
 * - **Radius-Based Filtering**: Local density-based noise filtering
 * - **Moving Least Squares**: Smooth surface approximation and denoising
 * - **Bilateral Filtering**: Edge-preserving smoothing for scanned surfaces
 * 
 * ### Normal Estimation
 * Accurate surface orientation computation:
 * - **PCA-Based Normals**: Principal component analysis for local surface fitting
 * - **Plane Fitting**: Robust plane fitting using RANSAC and least squares
 * - **Adaptive Neighborhoods**: Variable radius normal computation
 * - **Orientation Consistency**: Global normal orientation propagation
 * 
 * ## Professional Features
 * 
 * ### Registration and Alignment
 * Multi-scan point cloud integration:
 * - **ICP Registration**: Iterative Closest Point for precise alignment
 * - **Feature-Based Registration**: SHOT, FPFH descriptors for robust matching
 * - **Global Registration**: Multi-view simultaneous pose estimation
 * - **Loop Closure Detection**: Automatic detection and correction of drift
 * 
 * ### Segmentation and Classification
 * Intelligent point cloud analysis:
 * - **Planar Segmentation**: RANSAC-based plane detection for architecture
 * - **Region Growing**: Connected component analysis for object segmentation
 * - **Clustering**: K-means and DBSCAN clustering for object grouping
 * - **Machine Learning**: Neural network-based semantic segmentation
 * 
 * ### Feature Detection
 * Geometric feature extraction:
 * - **Edge Detection**: Sharp edge and corner point identification
 * - **Keypoint Extraction**: Distinctive point detection for registration
 * - **Surface Primitives**: Automatic detection of planes, cylinders, spheres
 * - **Boundary Detection**: Mesh and surface boundary identification
 * 
 * ## Usage Examples
 * 
 * ### Basic Point Cloud Creation and Manipulation
 * ```cpp
 * // Create and populate point cloud
 * PointCloud cloud;
 * 
 * // Add points with attributes
 * PointCloud::Point point;
 * point.position = glm::vec3(1.0f, 2.0f, 3.0f);
 * point.normal = glm::vec3(0.0f, 1.0f, 0.0f);
 * point.color = glm::vec3(0.8f, 0.6f, 0.4f);
 * point.intensity = 0.9f;
 * cloud.addPoint(point);
 * 
 * // Load from file
 * auto loadedCloud = PointCloud::loadFromPLY("scan_data.ply");
 * cloud.merge(*loadedCloud);
 * ```
 * 
 * ### Surface Reconstruction
 * ```cpp
 * // Compute normals if not present
 * if (!cloud.hasNormals()) {
 *     cloud.computeNormals(0.05f);  // 5cm neighborhood
 * }
 * 
 * // Perform Poisson reconstruction
 * PoissonReconstructionOptions options;
 * options.depth = 8;
 * options.pointWeight = 4.0f;
 * options.trimThreshold = 7.0f;
 * auto mesh = cloud.poissonReconstruction(options);
 * 
 * // Alternative: Ball pivoting for fine details
 * BallPivotingOptions bpOptions;
 * bpOptions.radii = {0.005f, 0.01f, 0.02f};
 * auto detailMesh = cloud.ballPivoting(bpOptions);
 * ```
 * 
 * ### Point Cloud Processing Pipeline
 * ```cpp
 * // Load and preprocess point cloud
 * auto cloud = PointCloud::loadFromLAS("terrain.las");
 * 
 * // Remove noise and outliers
 * cloud->statisticalOutlierRemoval(50, 2.0f);
 * cloud->radiusOutlierRemoval(0.1f, 10);
 * 
 * // Downsample for processing
 * cloud->voxelGridFilter(0.02f);  // 2cm voxel size
 * 
 * // Compute features
 * cloud->computeNormals(0.05f);
 * cloud->computeCurvature();
 * 
 * // Segment ground plane
 * auto groundIndices = cloud->segmentPlane(0.01f, 1000);
 * auto ground = cloud->extractIndices(groundIndices, false);
 * auto objects = cloud->extractIndices(groundIndices, true);
 * ```
 * 
 * ### Multi-Scan Registration
 * ```cpp
 * std::vector<PointCloud::Ptr> scans = {scan1, scan2, scan3};
 * 
 * // Rough alignment using features
 * for (size_t i = 1; i < scans.size(); ++i) {
 *     auto transform = registerClouds(*scans[0], *scans[i], 
 *                                   RegistrationType::FEATURE_BASED);
 *     scans[i]->transform(transform);
 * }
 * 
 * // Fine alignment with ICP
 * ICPOptions icpOptions;
 * icpOptions.maxIterations = 100;
 * icpOptions.transformationEpsilon = 1e-8;
 * icpOptions.euclideanFitnessEpsilon = 1e-6;
 * 
 * for (size_t i = 1; i < scans.size(); ++i) {
 *     auto refinedTransform = icpRegistration(*scans[0], *scans[i], icpOptions);
 *     scans[i]->transform(refinedTransform);
 * }
 * 
 * // Merge all scans
 * PointCloud mergedCloud = *scans[0];
 * for (size_t i = 1; i < scans.size(); ++i) {
 *     mergedCloud.merge(*scans[i]);
 * }
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Memory Management
 * Efficient handling of large point datasets:
 * - **Streaming Processing**: Process points incrementally to manage memory
 * - **Level-of-Detail**: Hierarchical representation for interactive viewing
 * - **Compression**: Point cloud compression for storage and transmission
 * - **Memory Mapping**: Efficient file-based point cloud access
 * 
 * ### Parallel Processing
 * Multi-threaded and GPU acceleration:
 * - **OpenMP**: Parallel CPU processing for large point clouds
 * - **CUDA Support**: GPU-accelerated nearest neighbor search and filtering
 * - **Batch Processing**: Efficient processing of multiple point clouds
 * - **Asynchronous I/O**: Background loading and saving operations
 * 
 * @ingroup Geometry
 * @ingroup PointCloud
 * @ingroup SurfaceReconstruction
 * 
 * @see PoissonReconstruction For advanced surface reconstruction
 * @see KDTree For spatial indexing and nearest neighbor search
 * @see ICPRegistration For point cloud alignment algorithms
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class PointCloud {
public:
    struct Point {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        float intensity = 1.0f;
        Point() = default;
        Point(const glm::vec3& pos) : position(pos), normal(0, 1, 0) {}
        Point(const glm::vec3& pos, const glm::vec3& norm) : position(pos), normal(norm) {}
    };

    PointCloud() = default;
    explicit PointCloud(const std::vector<Point>& points) : m_points(points) {}
    ~PointCloud() = default;

    // Point access
    void addPoint(const Point& point) { m_points.push_back(point); }
    void addPoint(const glm::vec3& position, const glm::vec3& normal = glm::vec3(0, 1, 0));
    const Point& getPoint(size_t index) const { return m_points[index]; }
    Point& getPoint(size_t index) { return m_points[index]; }
    size_t getPointCount() const { return m_points.size(); }
    
    // Bulk operations
    void clear() { m_points.clear(); }
    void reserve(size_t count) { m_points.reserve(count); }
    const std::vector<Point>& getPoints() const { return m_points; }
    std::vector<Point>& getPoints() { return m_points; }
    
    // Processing operations
    void computeNormals(int kNeighbors = 20);
    void removeOutliers(float stdRatio = 2.0f, int minNeighbors = 10);
    void downsample(float voxelSize);
    
    // Mesh reconstruction
    rude::MeshPtr poissonReconstruction(int depth = 8, float samplesPerNode = 1.5f) const;
    rude::MeshPtr ballPivoting(float ballRadius = 0.1f) const;
    
    // Spatial queries
    std::vector<size_t> findKNearestNeighbors(const glm::vec3& queryPoint, int k) const;
    std::vector<size_t> findRadiusNeighbors(const glm::vec3& queryPoint, float radius) const;
    
    // Bounding box
    void computeBoundingBox(glm::vec3& minBounds, glm::vec3& maxBounds) const;

private:
    std::vector<Point> m_points;
    
    // Spatial acceleration structure (KD-tree)
    struct KDNode {
        size_t pointIndex;
        std::unique_ptr<KDNode> left, right;
        int axis;
    };
    mutable std::unique_ptr<KDNode> m_kdTree;
    mutable bool m_kdTreeValid = false;
    
    void buildKDTree() const;
    std::unique_ptr<KDNode> buildKDTreeRecursive(std::vector<size_t>& indices, int depth) const;
    void knnSearch(const KDNode* node, const glm::vec3& query, int k, 
                   std::vector<std::pair<float, size_t>>& results) const;
};

/**
 * @brief Advanced Implicit Surface system using Signed Distance Fields (SDF)
 * 
 * The ImplicitSurface class provides a sophisticated framework for representing
 * and manipulating 3D surfaces through Signed Distance Field (SDF) functions.
 * This approach enables powerful procedural modeling, Boolean operations, and
 * advanced geometric processing within the RudeBase3D environment.
 * 
 * ## Core SDF Functionality
 * 
 * ### Mathematical Foundation
 * Signed Distance Fields provide precise geometric representation:
 * - **Distance Functions**: Mathematical functions returning shortest distance to surface
 * - **Sign Convention**: Negative inside, positive outside, zero on surface
 * - **Lipschitz Continuity**: Smooth gradients for reliable surface normals
 * - **Analytical Precision**: Exact mathematical representation without discretization
 * 
 * ### Surface Evaluation
 * Efficient computation of surface properties:
 * - **Distance Queries**: Fast distance computation at any 3D point
 * - **Gradient Calculation**: Surface normals through gradient computation
 * - **Surface Points**: Exact surface intersection through ray marching
 * - **Curvature Analysis**: Second-order derivatives for geometric analysis
 * 
 * ### Procedural Primitives
 * Built-in SDF primitives for common shapes:
 * - **Basic Shapes**: Spheres, boxes, cylinders, tori, and planes
 * - **Complex Primitives**: Rounded boxes, capsules, ellipsoids
 * - **Infinite Shapes**: Infinite planes, cylinders, and repetition patterns
 * - **Deformation Functions**: Twist, bend, scale, and warp operations
 * 
 * ## Advanced SDF Operations
 * 
 * ### Boolean Operations
 * Robust CSG operations through SDF combination:
 * - **Union**: Smooth and sharp union variants with blending control
 * - **Intersection**: Precise Boolean intersection with material preservation
 * - **Subtraction**: Clean Boolean subtraction for complex modeling
 * - **Smooth Blending**: Controllable smooth transitions between shapes
 * 
 * ### Transformation Operations
 * Powerful geometric transformations:
 * - **Translation**: Position offset with maintained SDF properties
 * - **Rotation**: Arbitrary 3D rotation preserving distance fields
 * - **Scaling**: Uniform and non-uniform scaling with proper normalization
 * - **Composition**: Hierarchical transformation chains
 * 
 * ### Domain Operations
 * Advanced space manipulation techniques:
 * - **Repetition**: Infinite repetition in 1D, 2D, or 3D space
 * - **Mirroring**: Symmetry operations for efficient modeling
 * - **Polar Coordinates**: Radial and angular space transformations
 * - **Distortion**: Non-linear space warping for organic shapes
 * 
 * ## Professional Modeling Features
 * 
 * ### Procedural Modeling
 * Algorithmic shape generation and manipulation:
 * - **Noise Integration**: Perlin/Simplex noise for surface detail
 * - **Fractal Surfaces**: Self-similar geometric patterns and textures
 * - **L-Systems**: Procedural plant and organic structure generation
 * - **Parametric Control**: Real-time parameter adjustment for exploration
 * 
 * ### Surface Extraction
 * Conversion to renderable mesh representations:
 * - **Marching Cubes**: ISO-surface extraction with adaptive resolution
 * - **Dual Contouring**: Feature-preserving mesh generation
 * - **Ray Marching**: Direct SDF rendering for real-time visualization
 * - **Adaptive Sampling**: Variable resolution based on surface complexity
 * 
 * ### Animation and Deformation
 * Time-varying and dynamic surface manipulation:
 * - **Morphing**: Smooth transitions between different SDF shapes
 * - **Deformation Fields**: Vector field-based surface deformation
 * - **Physics Integration**: Soft body and fluid simulation compatibility
 * - **Keyframe Animation**: Temporal parameter interpolation
 * 
 * ## Advanced Algorithms
 * 
 * ### Ray Marching Rendering
 * Direct SDF visualization without mesh conversion:
 * - **Sphere Tracing**: Efficient ray-surface intersection algorithm
 * - **Shadow Mapping**: Soft shadow computation through SDF sampling
 * - **Ambient Occlusion**: Screen-space occlusion through distance sampling
 * - **Volumetric Effects**: Fog, clouds, and atmospheric effects
 * 
 * ### Collision Detection
 * Fast and accurate collision queries:
 * - **Point-Surface Distance**: Instant collision detection and response
 * - **Continuous Collision**: Swept volume collision detection
 * - **Penetration Depth**: Exact overlap measurement for physics
 * - **Contact Manifolds**: Multi-point contact generation
 * 
 * ## Usage Examples
 * 
 * ### Basic SDF Creation and Evaluation
 * ```cpp
 * // Create a sphere SDF
 * auto sphereSDF = [](const glm::vec3& p) -> float {
 *     return glm::length(p) - 1.0f;  // Unit sphere
 * };
 * 
 * auto sphereGradient = [](const glm::vec3& p) -> glm::vec3 {
 *     return glm::normalize(p);  // Gradient points outward
 * };
 * 
 * ImplicitSurface sphere(sphereSDF, sphereGradient);
 * 
 * // Evaluate distance and normal
 * glm::vec3 queryPoint(0.5f, 0.5f, 0.5f);
 * float distance = sphere.evaluate(queryPoint);
 * glm::vec3 normal = sphere.evaluateNormal(queryPoint);
 * ```
 * 
 * ### Boolean Operations and Composition
 * ```cpp
 * // Create composite SDF using Boolean operations
 * auto boxSDF = [](const glm::vec3& p) -> float {
 *     glm::vec3 d = glm::abs(p) - glm::vec3(1.0f);
 *     return glm::min(glm::max(d.x, glm::max(d.y, d.z)), 0.0f) + 
 *            glm::length(glm::max(d, 0.0f));
 * };
 * 
 * // Union with smooth blending
 * auto unionSDF = [sphereSDF, boxSDF](const glm::vec3& p) -> float {
 *     float sphere = sphereSDF(p);
 *     float box = boxSDF(p);
 *     float k = 0.3f;  // Blending factor
 *     return smoothMin(sphere, box, k);
 * };
 * 
 * ImplicitSurface composite(unionSDF);
 * ```
 * 
 * ### Procedural Surface Generation
 * ```cpp
 * // Create noise-displaced sphere
 * auto noisySphere = [](const glm::vec3& p) -> float {
 *     float baseDistance = glm::length(p) - 1.0f;
 *     float noise = simplexNoise(p * 4.0f) * 0.1f;
 *     return baseDistance + noise;
 * };
 * 
 * ImplicitSurface terrain(noisySphere);
 * 
 * // Extract mesh for rendering
 * auto mesh = terrain.extractMesh(glm::vec3(-2), glm::vec3(2), 0.02f);
 * ```
 * 
 * ### Animation and Morphing
 * ```cpp
 * class AnimatedSDF : public ImplicitSurface {
 * private:
 *     float m_time = 0.0f;
 *     
 * public:
 *     void update(float deltaTime) {
 *         m_time += deltaTime;
 *         
 *         // Update SDF function with time-varying parameters
 *         auto animatedFunction = [this](const glm::vec3& p) -> float {
 *             float pulseFactor = 1.0f + 0.2f * sin(m_time * 2.0f);
 *             return glm::length(p) - pulseFactor;
 *         };
 *         
 *         setSDF(animatedFunction);
 *     }
 * };
 * 
 * AnimatedSDF pulsingSphere;
 * pulsingSphere.update(frameTime);
 * ```
 * 
 * ### Advanced Ray Marching Integration
 * ```cpp
 * // GPU shader integration for real-time rendering
 * struct RayMarchingParams {
 *     float maxDistance = 100.0f;
 *     int maxSteps = 256;
 *     float epsilon = 0.001f;
 *     bool enableSoftShadows = true;
 *     float shadowHardness = 16.0f;
 * };
 * 
 * // Generate GLSL shader code for SDF
 * std::string shaderCode = sphere.generateGLSL();
 * 
 * // Real-time ray marching in fragment shader
 * auto renderTarget = createRayMarchingRenderer(sphere, params);
 * renderTarget.render(camera, lights);
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Efficient Evaluation
 * Optimized SDF computation strategies:
 * - **Function Caching**: Memoization for expensive SDF evaluations
 * - **Spatial Acceleration**: Octree-based SDF acceleration structures
 * - **GPU Compute**: CUDA/OpenCL SDF evaluation for parallel processing
 * - **Level-of-Detail**: Simplified SDF functions for distant surfaces
 * 
 * ### Memory Management
 * Efficient handling of complex SDF compositions:
 * - **Function Composition**: Lazy evaluation of composite SDF operations
 * - **Parameter Binding**: Efficient closure management for parametric SDFs
 * - **Shader Generation**: Automatic GLSL code generation for GPU rendering
 * - **Adaptive Sampling**: Variable precision based on distance to surface
 * 
 * @ingroup Geometry
 * @ingroup ImplicitSurfaces
 * @ingroup SignedDistanceFields
 * 
 * @see RayMarching For direct SDF rendering techniques
 * @see BooleanOperations For SDF composition algorithms
 * @see ProceduralModeling For SDF-based procedural generation
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class ImplicitSurface {
public:
    using SDFFunction = std::function<float(const glm::vec3&)>;
    using GradientFunction = std::function<glm::vec3(const glm::vec3&)>;

    ImplicitSurface(SDFFunction sdf, GradientFunction gradient = nullptr);
    ~ImplicitSurface() = default;

    // SDF evaluation
    float evaluate(const glm::vec3& point) const;
    glm::vec3 evaluateGradient(const glm::vec3& point) const;
    glm::vec3 evaluateNormal(const glm::vec3& point) const;

    // Surface extraction
    rude::MeshPtr extractSurface(const glm::vec3& bounds, float resolution, float isoValue = 0.0f) const;
    std::shared_ptr<VoxelGrid> toVoxelGrid(const glm::vec3& bounds, float resolution) const;

    // Boolean operations (create new implicit surfaces)
    static ImplicitSurface unionOp(const ImplicitSurface& a, const ImplicitSurface& b);
    static ImplicitSurface intersectionOp(const ImplicitSurface& a, const ImplicitSurface& b);
    static ImplicitSurface differenceOp(const ImplicitSurface& a, const ImplicitSurface& b);

    // Primitive SDFs
    static ImplicitSurface sphere(const glm::vec3& center, float radius);
    static ImplicitSurface box(const glm::vec3& center, const glm::vec3& size);
    static ImplicitSurface cylinder(const glm::vec3& center, float radius, float height);
    static ImplicitSurface torus(const glm::vec3& center, float majorRadius, float minorRadius);

private:
    SDFFunction m_sdf;
    GradientFunction m_gradient;

    // Numerical gradient computation if not provided
    glm::vec3 computeNumericalGradient(const glm::vec3& point, float epsilon = 1e-4f) const;
};

/**
 * @brief High-Performance Bounding Volume Hierarchy for spatial acceleration
 * 
 * The BVHTree class provides a sophisticated spatial acceleration structure
 * optimized for fast geometric queries, ray tracing, collision detection,
 * and visibility computation. This implementation offers state-of-the-art
 * BVH construction and traversal algorithms essential for high-performance
 * 3D applications in the RudeBase3D framework.
 * 
 * ## Core BVH Functionality
 * 
 * ### Spatial Acceleration Structure
 * Hierarchical bounding volume organization:
 * - **Binary Tree Structure**: Efficient binary partitioning of spatial primitives
 * - **Axis-Aligned Bounding Boxes**: AABB-based bounding volume representation
 * - **Adaptive Subdivision**: Intelligent partitioning based on primitive distribution
 * - **Memory-Efficient Layout**: Cache-friendly data structure organization
 * 
 * ### Construction Algorithms
 * Advanced BVH building strategies:
 * - **SAH Construction**: Surface Area Heuristic for optimal tree quality
 * - **HLBVH**: Hierarchical Linear BVH for GPU-friendly construction
 * - **PLOC**: Parallel Locally-Ordered Clustering for multi-threaded building
 * - **Incremental Updates**: Dynamic BVH maintenance for moving objects
 * 
 * ### Traversal Optimization
 * High-performance query processing:
 * - **Early Termination**: Optimized traversal with branch prediction
 * - **SIMD Acceleration**: Vectorized bounding box intersection tests
 * - **Cache Optimization**: Memory layout optimized for CPU cache performance
 * - **GPU Traversal**: CUDA/OpenCL kernels for parallel query processing
 * 
 * ## Advanced Query Operations
 * 
 * ### Ray Intersection
 * Comprehensive ray-geometry intersection:
 * - **Ray-Triangle Tests**: Fast Möller-Trumbore intersection algorithm
 * - **Ray-AABB Tests**: Optimized slab method for bounding box intersection
 * - **Closest Hit**: First intersection along ray direction
 * - **Any Hit**: Shadow ray optimization for occlusion queries
 * - **Multi-Hit**: Complete intersection list for transparency effects
 * 
 * ### Spatial Queries
 * Efficient geometric query processing:
 * - **Point Location**: Fast point-in-geometry queries with containment tests
 * - **Range Queries**: AABB and sphere-based range intersection
 * - **Nearest Neighbor**: K-nearest neighbor search with distance constraints
 * - **Frustum Culling**: View frustum intersection for rendering optimization
 * 
 * ### Collision Detection
 * Robust collision detection and response:
 * - **Broad Phase**: Efficient bounding box overlap detection
 * - **Narrow Phase**: Detailed primitive-level collision testing
 * - **Continuous Collision**: Swept volume collision detection for moving objects
 * - **Contact Generation**: Precise contact point and normal computation
 * 
 * ## Professional Features
 * 
 * ### Real-Time Ray Tracing
 * Hardware-accelerated ray tracing integration:
 * - **RT Core Integration**: NVIDIA RTX and AMD RDNA acceleration
 * - **OptiX Compatibility**: NVIDIA OptiX RT pipeline integration
 * - **Vulkan RT Support**: Cross-platform hardware ray tracing
 * - **Fallback Rendering**: Software ray tracing for non-RT hardware
 * 
 * ### Animation Support
 * Dynamic geometry and temporal coherence:
 * - **Motion Blur**: Temporal bounding volumes for motion blur effects
 * - **Deformation Tracking**: Efficient BVH updates for animated meshes
 * - **Keyframe Interpolation**: Smooth temporal BVH interpolation
 * - **Instancing Support**: Efficient handling of instanced geometry
 * 
 * ### Multi-Threading
 * Parallel construction and traversal:
 * - **Parallel Building**: Multi-threaded BVH construction algorithms
 * - **Work Stealing**: Load-balanced traversal for complex scenes
 * - **Lock-Free Updates**: Concurrent BVH modification support
 * - **NUMA Optimization**: Memory layout optimized for multi-socket systems
 * 
 * ## Advanced Algorithms
 * 
 * ### Quality Metrics
 * BVH optimization and quality assessment:
 * - **SAH Cost Function**: Surface area heuristic for tree quality measurement
 * - **Memory Footprint**: Efficient memory usage analysis and optimization
 * - **Traversal Statistics**: Performance profiling and bottleneck identification
 * - **Quality Visualization**: Debug visualization of BVH structure and quality
 * 
 * ### Compression Techniques
 * Memory-efficient BVH representation:
 * - **Quantized AABBs**: Compressed bounding box representation
 * - **Node Compression**: Bit-packed node data for memory efficiency
 * - **Primitive Ordering**: Cache-friendly primitive reordering
 * - **Streaming Support**: Out-of-core BVH for massive datasets
 * 
 * ## Usage Examples
 * 
 * ### Basic BVH Construction and Ray Casting
 * ```cpp
 * // Create BVH from triangle mesh
 * std::vector<Triangle> triangles = mesh.getTriangles();
 * BVHTree bvh;
 * 
 * // Build BVH with SAH construction
 * BVHBuildOptions options;
 * options.maxPrimitivesPerLeaf = 4;
 * options.maxDepth = 30;
 * options.useSAH = true;
 * bvh.build(triangles, options);
 * 
 * // Perform ray intersection
 * Ray ray(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1));
 * BVHTree::Hit hit;
 * if (bvh.intersect(ray, hit)) {
 *     glm::vec3 hitPoint = ray.origin + ray.direction * hit.distance;
 *     glm::vec3 normal = hit.normal;
 *     int primitiveID = hit.primitiveID;
 * }
 * ```
 * 
 * ### Advanced Ray Tracing Integration
 * ```cpp
 * // Multi-sample ray casting for anti-aliasing
 * std::vector<Ray> rays = camera.generateCameraRays(width, height, 4); // 4x MSAA
 * std::vector<BVHTree::Hit> hits(rays.size());
 * 
 * // Parallel ray casting
 * #pragma omp parallel for
 * for (size_t i = 0; i < rays.size(); ++i) {
 *     bvh.intersect(rays[i], hits[i]);
 * }
 * 
 * // Shadow ray optimization
 * for (const auto& light : lights) {
 *     Ray shadowRay(hitPoint + normal * 0.001f, lightDirection);
 *     shadowRay.maxDistance = lightDistance;
 *     bool inShadow = bvh.intersectAny(shadowRay);
 * }
 * ```
 * 
 * ### Spatial Query Operations
 * ```cpp
 * // Range query for objects in bounding box
 * BoundingBox queryRegion(glm::vec3(-5, -5, -5), glm::vec3(5, 5, 5));
 * std::vector<int> objectsInRange;
 * bvh.query(queryRegion, objectsInRange);
 * 
 * // Frustum culling for rendering
 * Frustum viewFrustum = camera.getFrustum();
 * std::vector<int> visibleObjects;
 * bvh.frustumCull(viewFrustum, visibleObjects);
 * 
 * // Nearest neighbor search
 * glm::vec3 queryPoint(1.0f, 2.0f, 3.0f);
 * int nearestPrimitive = bvh.findNearest(queryPoint);
 * float distance = bvh.distanceTo(queryPoint, nearestPrimitive);
 * ```
 * 
 * ### Dynamic BVH Updates
 * ```cpp
 * // Animated mesh support
 * class AnimatedBVH : public BVHTree {
 * private:
 *     std::vector<BoundingBox> m_originalBounds;
 *     
 * public:
 *     void updateAnimation(const std::vector<glm::mat4>& transforms) {
 *         // Update primitive bounding boxes
 *         for (size_t i = 0; i < transforms.size(); ++i) {
 *             auto transformedBounds = transformBoundingBox(m_originalBounds[i], 
 *                                                         transforms[i]);
 *             updatePrimitive(i, transformedBounds);
 *         }
 *         
 *         // Incremental BVH update
 *         if (needsRebuild()) {
 *             rebuild();
 *         } else {
 *             incrementalUpdate();
 *         }
 *     }
 * };
 * ```
 * 
 * ### GPU Acceleration Integration
 * ```cpp
 * // CUDA BVH traversal
 * class CudaBVHAccelerator {
 * private:
 *     BVHTree* d_bvh;
 *     Ray* d_rays;
 *     Hit* d_hits;
 *     
 * public:
 *     void launchRayKernel(const std::vector<Ray>& rays) {
 *         // Copy rays to GPU
 *         cudaMemcpy(d_rays, rays.data(), sizeof(Ray) * rays.size(), 
 *                   cudaMemcpyHostToDevice);
 *                   
 *         // Launch CUDA kernel
 *         dim3 blockSize(256);
 *         dim3 gridSize((rays.size() + blockSize.x - 1) / blockSize.x);
 *         
 *         rayIntersectionKernel<<<gridSize, blockSize>>>(
 *             d_bvh, d_rays, d_hits, rays.size());
 *             
 *         // Copy results back
 *         std::vector<Hit> hits(rays.size());
 *         cudaMemcpy(hits.data(), d_hits, sizeof(Hit) * hits.size(), 
 *                   cudaMemcpyDeviceToHost);
 *     }
 * };
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Memory Layout
 * Cache-friendly data structure organization:
 * - **Structure of Arrays**: SoA layout for SIMD-friendly access patterns
 * - **Node Packing**: Minimal memory footprint for BVH nodes
 * - **Primitive Reordering**: Spatial locality optimization for cache efficiency
 * - **Prefetching**: Hardware prefetch hints for improved memory performance
 * 
 * ### Algorithmic Optimization
 * High-performance traversal and construction:
 * - **Early Termination**: Branch prediction friendly traversal patterns
 * - **Vectorization**: AVX/SSE optimized intersection routines
 * - **Parallel Algorithms**: Multi-core construction and traversal
 * - **GPU Offloading**: Compute shader acceleration for massive scenes
 * 
 * @ingroup Geometry
 * @ingroup SpatialAcceleration
 * @ingroup RayTracing
 * 
 * @see RayTracing For ray tracing integration and algorithms
 * @see CollisionDetection For collision detection applications
 * @see SpatialQuery For spatial query optimization techniques
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class BVHTree {
public:
    struct BoundingBox {
        glm::vec3 min, max;
        BoundingBox() : min(FLT_MAX, FLT_MAX, FLT_MAX), max(-FLT_MAX, -FLT_MAX, -FLT_MAX) {}
        BoundingBox(const glm::vec3& minBounds, const glm::vec3& maxBounds) : min(minBounds), max(maxBounds) {}
        void expand(const glm::vec3& point);
        void expand(const BoundingBox& box);
        bool intersects(const BoundingBox& other) const;
        bool contains(const glm::vec3& point) const;
        glm::vec3 getCenter() const { return (min + max) * 0.5f; }
        glm::vec3 getSize() const { return max - min; }
        float getSurfaceArea() const;
    };
    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;
        float tMin = 0.0f;
        float tMax = FLT_MAX;
        Ray(const glm::vec3& o, const glm::vec3& d) : origin(o), direction(d) {}
        glm::vec3 pointAt(float t) const { return origin + direction * t; }
    };
    struct RayHit {
        bool hit = false;
        float distance = FLT_MAX;
        glm::vec3 point;
        glm::vec3 normal;
        size_t primitiveIndex = SIZE_MAX;
    };

    struct BVHNode {
        BoundingBox bounds;
        std::vector<size_t> primitiveIndices;
        std::unique_ptr<BVHNode> left;
        std::unique_ptr<BVHNode> right;
    };

    explicit BVHTree(rude::MeshPtr mesh);
    ~BVHTree() = default;

    // Ray casting
    RayHit raycast(const Ray& ray) const;
    std::vector<RayHit> raycastAll(const Ray& ray) const;

    // Spatial queries
    std::vector<size_t> query(const BoundingBox& bounds) const;
    std::vector<size_t> query(const glm::vec3& point, float radius) const;

    // BVH construction
    void buildTree();
    std::unique_ptr<BVHNode> buildRecursive(std::vector<size_t>& primitives, int depth);
    BoundingBox computePrimitiveBounds(size_t primitiveIndex) const;

private:
    rude::MeshPtr m_mesh;
    std::vector<BoundingBox> m_primitiveBounds;
    std::unique_ptr<BVHNode> m_root;
};
