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
 * █  FILE: HybridGeometryManager.h                                              █
 * █  DESCRIPTION: Advanced Hybrid Geometry Management & Processing System       █
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
 * █  This file contains the HybridGeometryManager system providing advanced     █
 * █  multi-representation geometry management, intelligent caching, and         █
 * █  cross-format processing pipelines for professional 3D modeling workflows. █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file HybridGeometryManager.h
 * @brief Advanced Hybrid Geometry Management and Multi-Representation Processing System
 * 
 * This file contains the comprehensive HybridGeometryManager implementation that provides
 * professional-grade management of multiple geometric representations within a unified
 * framework. The system enables seamless workflow transitions between different geometry
 * formats (meshes, NURBS, subdivision surfaces, voxels, point clouds, implicit surfaces)
 * while maintaining performance through intelligent caching and memory management.
 * 
 * ## Core Hybrid Geometry Architecture
 * 
 * ### Multi-Representation Framework
 * Unified geometry management across diverse formats:
 * - **Face-Vertex Meshes**: Optimized triangle/quad meshes for real-time rendering
 * - **Half-Edge Meshes**: Topological mesh structures for advanced editing operations
 * - **NURBS Surfaces**: CAD-precision non-uniform rational B-spline surfaces
 * - **Subdivision Surfaces**: Smooth multi-resolution surface representation
 * - **Voxel Grids**: Volume-based geometry for sculpting and boolean operations
 * - **Point Clouds**: Sparse 3D point data for scanning and reconstruction workflows
 * - **Implicit Surfaces**: Mathematical surface definitions for procedural modeling
 * 
 * ### Intelligent Caching System
 * Performance optimization through smart representation caching:
 * - **Lazy Conversion**: On-demand format conversion with automatic caching
 * - **Memory Management**: Intelligent cache eviction based on usage patterns
 * - **Temporal Tracking**: Time-based cache validity and automatic cleanup
 * - **Reference Counting**: Safe memory management with shared ownership
 * - **Cache Invalidation**: Automatic cache updates when geometry changes
 * 
 * ### Professional Workflow Integration
 * Seamless support for industry-standard modeling pipelines:
 * - **CAD Integration**: High-precision NURBS workflow support
 * - **Animation Pipeline**: Efficient subdivision surface handling
 * - **Game Development**: Optimized mesh generation for real-time rendering
 * - **Digital Sculpting**: Voxel-based modeling with high-resolution detail
 * - **3D Scanning**: Point cloud processing and reconstruction workflows
 * 
 * ## Advanced Management Features
 * 
 * ### Unified Geometry Interface
 * Single interface for managing diverse geometry types:
 * - **Primary Representation**: Native format preservation for optimal workflows
 * - **Automatic Conversion**: Transparent format translation between representations
 * - **Type Safety**: Compile-time type checking with runtime flexibility
 * - **Metadata Management**: Custom property storage for workflow integration
 * - **Version Control**: Change tracking and undo/redo support preparation
 * 
 * ### Dynamic Format Selection
 * Optimal representation selection based on operation requirements:
 * - **Rendering Optimization**: Automatic mesh format selection for GPU efficiency
 * - **Editing Operations**: Half-edge conversion for topological modifications
 * - **Boolean Operations**: Voxel or implicit surface selection for CSG operations
 * - **Smooth Modeling**: Subdivision surface activation for organic modeling
 * - **Precision Work**: NURBS representation for CAD-level accuracy requirements
 * 
 * ### Memory-Aware Processing
 * Intelligent resource management for large geometry datasets:
 * - **Streaming Processing**: Chunk-based handling of massive geometry datasets
 * - **Memory Budgets**: Configurable limits on cache and processing memory usage
 * - **Garbage Collection**: Automatic cleanup of unused geometry representations
 * - **Priority Systems**: LRU and frequency-based cache replacement strategies
 * - **Memory Profiling**: Real-time memory usage monitoring and optimization
 * 
 * ## Processing Pipeline Architecture
 * 
 * ### Cross-Format Operations
 * Advanced operations that work transparently across geometry types:
 * - **Boolean Operations**: CSG operations (union, intersection, subtraction)
 * - **Smoothing Algorithms**: Subdivision and mesh smoothing across formats
 * - **Mesh Repair**: Topology validation and automatic repair procedures
 * - **Simplification**: Level-of-detail generation and polygon reduction
 * - **Remeshing**: Uniform mesh generation with target edge lengths
 * 
 * ### Adaptive Processing
 * Intelligent algorithm selection based on geometry characteristics:
 * - **Format-Specific Optimization**: Best algorithm choice per representation type
 * - **Quality vs. Performance**: Configurable trade-offs for different use cases
 * - **Progressive Processing**: Incremental operations for interactive workflows
 * - **Error Metrics**: Quality assessment and validation throughout processing
 * - **Fallback Strategies**: Robust handling of edge cases and processing failures
 * 
 * ### Parallel Processing Support
 * Multi-threaded operations for performance-critical workflows:
 * - **Thread-Safe Operations**: Concurrent processing of independent geometries
 * - **NUMA Awareness**: Memory locality optimization for large-scale processing
 * - **GPU Acceleration**: Compute shader integration for intensive operations
 * - **Task Scheduling**: Work-stealing and load balancing across available cores
 * 
 * ## Professional Workflow Examples
 * 
 * ### CAD-to-Animation Pipeline
 * Professional workflow from engineering to animation:
 * - **NURBS Import**: Precise CAD surface representation preservation
 * - **Mesh Conversion**: Adaptive tessellation for animation requirements
 * - **Subdivision Setup**: Smooth surface conversion for character modeling
 * - **Animation Preparation**: Topology optimization for deformation quality
 * 
 * ### Scanning-to-Model Workflow
 * Complete 3D scanning reconstruction pipeline:
 * - **Point Cloud Input**: Raw scanner data processing and filtering
 * - **Surface Reconstruction**: Poisson or Delaunay mesh generation
 * - **Mesh Optimization**: Cleanup, repair, and topology improvement
 * - **Format Preparation**: Output optimization for target application
 * 
 * ### Hybrid Modeling Approach
 * Mixed-representation modeling for maximum flexibility:
 * - **Base Shape**: NURBS or subdivision surface for primary form
 * - **Detail Addition**: Voxel sculpting for fine surface details
 * - **Feature Integration**: Boolean operations for mechanical features
 * - **Final Optimization**: Mesh conversion and rendering preparation
 * 
 * ## Usage Examples
 * 
 * ### Basic Hybrid Geometry Creation and Management
 * ```cpp
 * // Create hybrid geometry manager
 * auto geometryManager = std::make_unique<HybridGeometryManager>();
 * geometryManager->setDefaultVoxelSize(0.01f); // 1cm precision
 * geometryManager->setMaxCacheMemory(2048 * 1024 * 1024); // 2GB cache limit
 * 
 * // Load mesh from file and create hybrid geometry
 * auto originalMesh = loadMeshFromFile("character_base.obj");
 * auto hybridGeometry = geometryManager->createFromMesh(originalMesh);
 * geometryManager->registerGeometry("character_base", hybridGeometry);
 * 
 * // Access different representations as needed
 * auto editingMesh = hybridGeometry->getHalfEdgeMesh(); // For topology editing
 * auto renderingMesh = hybridGeometry->getFaceVertexMesh(); // For GPU rendering
 * auto smoothSurface = hybridGeometry->getSubdivisionMesh(); // For smooth modeling
 * ```
 * 
 * ### CAD Integration Workflow
 * ```cpp
 * // Load precision NURBS surface from CAD system
 * auto nurbsSurface = loadNURBSFromSTEP("mechanical_part.step");
 * auto cadGeometry = geometryManager->createFromNURBS(nurbsSurface);
 * 
 * // Convert to mesh for visualization with adaptive resolution
 * auto visualizationMesh = cadGeometry->getFaceVertexMesh();
 * 
 * // High-resolution conversion for manufacturing
 * cadGeometry->convertPrimaryTo(GeometryRepresentation::FaceVertex);
 * auto manufacturingMesh = cadGeometry->getRenderMesh(0); // Highest resolution
 * 
 * // Low-resolution for real-time preview
 * auto previewMesh = cadGeometry->getRenderMesh(2); // 2 subdivision levels down
 * ```
 * 
 * ### Digital Sculpting Pipeline
 * ```cpp
 * // Start with base mesh and convert to voxels for sculpting
 * auto baseMesh = createSphereMesh(1.0f, 32);
 * auto sculptGeometry = geometryManager->createFromMesh(baseMesh);
 * 
 * // Convert to high-resolution voxel grid for detailed sculpting
 * auto voxelGrid = sculptGeometry->getVoxelGrid(0.005f); // 5mm voxel resolution
 * 
 * // Perform sculpting operations on voxel grid
 * voxelSculptor.brush(voxelGrid, sculptPosition, brushRadius, intensity);
 * voxelSculptor.smooth(voxelGrid, smoothRegion, smoothStrength);
 * 
 * // Convert back to mesh for rendering and further processing
 * sculptGeometry->updatePrimary(voxelGrid);
 * auto sculptedMesh = sculptGeometry->getFaceVertexMesh();
 * ```
 * 
 * ### Boolean Operations with Automatic Format Selection
 * ```cpp
 * // Create geometry processing pipeline
 * GeometryProcessingPipeline pipeline(geometryManager.get());
 * 
 * // Load two geometries for boolean operations
 * auto geometry1 = geometryManager->getGeometry("base_shape");
 * auto geometry2 = geometryManager->getGeometry("cutting_tool");
 * 
 * // Perform boolean subtraction (automatically uses optimal representation)
 * auto cutResult = pipeline.subtractGeometry(geometry1, geometry2, 
 *                                          GeometryRepresentation::HalfEdge);
 * 
 * // Boolean union with implicit surface representation for smooth blending
 * auto unionResult = pipeline.unionGeometry(geometry1, geometry2,
 *                                         GeometryRepresentation::Implicit);
 * 
 * // Convert final result to mesh for rendering
 * auto finalMesh = unionResult->getFaceVertexMesh();
 * ```
 * 
 * ### Adaptive Level-of-Detail System
 * ```cpp
 * class AdaptiveLODRenderer {
 * private:
 *     HybridGeometryManager* m_geometryManager;
 *     std::unordered_map<std::string, std::shared_ptr<HybridGeometry>> m_geometries;
 *     
 * public:
 *     void render(const Camera& camera) {
 *         for (auto& [id, geometry] : m_geometries) {
 *             float distance = length(camera.getPosition() - geometry->getBounds().center);
 *             
 *             // Select appropriate representation based on distance
 *             rude::MeshPtr renderMesh;
 *             if (distance < 10.0f) {
 *                 // Close: Use high-resolution subdivision surface
 *                 renderMesh = geometry->getRenderMesh(0); // Full resolution
 *             } else if (distance < 100.0f) {
 *                 // Medium: Use moderate subdivision level
 *                 renderMesh = geometry->getRenderMesh(2); // 2 levels down
 *             } else {
 *                 // Far: Use base mesh only
 *                 renderMesh = geometry->getRenderMesh(4); // Minimal detail
 *             }
 *             
 *             // Render with appropriate material and shaders
 *             renderMeshWithLOD(renderMesh, distance);
 *         }
 *     }
 * };
 * ```
 * 
 * ### Memory Management and Performance Monitoring
 * ```cpp
 * class GeometryMemoryManager {
 * private:
 *     HybridGeometryManager* m_manager;
 *     size_t m_memoryBudget;
 *     
 * public:
 *     void optimizeMemoryUsage() {
 *         auto stats = m_manager->getStatistics();
 *         
 *         if (stats.memoryUsage > m_memoryBudget * 0.8f) {
 *             // Clean up old caches first
 *             m_manager->cleanupUnusedCaches(5000); // 5 second threshold
 *             
 *             // If still over budget, invalidate least recently used caches
 *             if (stats.memoryUsage > m_memoryBudget * 0.9f) {
 *                 m_manager->invalidateAllCaches();
 *             }
 *         }
 *         
 *         // Log memory usage statistics
 *         logInfo("Geometry Memory Usage: " + std::to_string(stats.memoryUsage / 1024 / 1024) + " MB");
 *         logInfo("Total Geometries: " + std::to_string(stats.totalGeometries));
 *         logInfo("Cache Entries: " + std::to_string(stats.totalCacheEntries));
 *         
 *         // Performance optimization based on usage patterns
 *         for (auto& [type, count] : stats.representationCounts) {
 *             if (count > stats.totalGeometries * 0.5f) {
 *                 // If most geometries use this representation, optimize for it
 *                 optimizeForRepresentation(type);
 *             }
 *         }
 *     }
 * };
 * ```
 * 
 * ### Advanced Processing Pipeline
 * ```cpp
 * class AdvancedModelingPipeline {
 * private:
 *     std::unique_ptr<HybridGeometryManager> m_geometryManager;
 *     std::unique_ptr<GeometryProcessingPipeline> m_pipeline;
 *     
 * public:
 *     std::shared_ptr<HybridGeometry> processModel(
 *         const std::vector<GeometryVariant>& inputGeometries,
 *         const ProcessingParameters& params) {
 *         
 *         // Phase 1: Unify all input geometries
 *         std::shared_ptr<HybridGeometry> result = nullptr;
 *         for (const auto& geom : inputGeometries) {
 *             auto hybridGeom = m_geometryManager->createGeometry(geom);
 *             
 *             if (!result) {
 *                 result = hybridGeom;
 *             } else {
 *                 // Combine with previous result using optimal representation
 *                 result = m_pipeline->unionGeometry(result, hybridGeom,
 *                                                  params.preferredRepresentation);
 *             }
 *         }
 *         
 *         // Phase 2: Apply processing operations
 *         if (params.enableSmoothing) {
 *             result = m_pipeline->smooth(result, params.smoothFactor, params.smoothIterations);
 *         }
 *         
 *         if (params.enableSimplification) {
 *             result = m_pipeline->simplifyMesh(result, params.simplificationRatio);
 *         }
 *         
 *         if (params.enableRemeshing) {
 *             result = m_pipeline->remeshUniform(result, params.targetEdgeLength);
 *         }
 *         
 *         // Phase 3: Repair and validation
 *         result = m_pipeline->repairMesh(result);
 *         
 *         // Phase 4: Optimize for target use case
 *         switch (params.targetUse) {
 *             case TargetUse::REALTIME_RENDERING:
 *                 result = m_pipeline->convertForRendering(result);
 *                 break;
 *             case TargetUse::FURTHER_EDITING:
 *                 result = m_pipeline->convertForEditing(result);
 *                 break;
 *             case TargetUse::CAD_EXPORT:
 *                 result = m_pipeline->convertForCAD(result);
 *                 break;
 *             case TargetUse::DIGITAL_SCULPTING:
 *                 result = m_pipeline->convertForSculpting(result, params.voxelSize);
 *                 break;
 *         }
 *         
 *         // Phase 5: Analysis and quality metrics
 *         auto analysis = m_pipeline->analyzeGeometry(result);
 *         validateProcessingQuality(analysis, params.qualityThresholds);
 *         
 *         return result;
 *     }
 * };
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Cache Management
 * Intelligent caching strategies for optimal performance:
 * - **Adaptive Cache Size**: Dynamic cache sizing based on available memory
 * - **Usage Pattern Analysis**: Cache retention based on access frequency
 * - **Predictive Caching**: Pre-generate likely needed representations
 * - **Memory Pool Allocation**: Efficient memory management for cache entries
 * 
 * ### Conversion Optimization
 * Efficient format translation algorithms:
 * - **Incremental Updates**: Partial conversion when geometry changes minimally
 * - **Quality Scaling**: Adaptive quality based on performance requirements
 * - **Parallel Conversion**: Multi-threaded format translation for large datasets
 * - **GPU Acceleration**: Compute shader utilization for conversion algorithms
 * 
 * ### Memory Efficiency
 * Optimized memory usage for large-scale modeling:
 * - **Shared Vertex Data**: Reference sharing for common vertices across formats
 * - **Compressed Storage**: Efficient encoding of geometry data in memory
 * - **Streaming I/O**: Disk-based caching for extremely large geometry datasets
 * - **Reference Counting**: Automatic cleanup of unused geometry representations
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Geometry
 * @ingroup HybridModeling
 * @ingroup MemoryManagement
 * @ingroup PerformanceOptimization
 */

#pragma once

#include "core/mesh_forward.hpp"
#include "GeometryTypes.h"
#include "GeometryConverter.h"
#include <memory>
#include <variant>
#include <unordered_map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

/**
 * @brief Geometry Representation Type
 * Identifies the primary representation type of geometry
 */
enum class GeometryRepresentation {
    FaceVertex,      // Traditional face-vertex mesh (rendering optimized)
    HalfEdge,        // Half-edge mesh (editing optimized)
    NURBS,           // NURBS surface (CAD precision)
    Subdivision,     // Subdivision surface (smooth modeling)
    Voxel,           // Voxel grid (sculpting/booleans)
    PointCloud,      // Point cloud (scanning input)
    Implicit         // Implicit surface/SDF (advanced operations)
};

/**
 * @brief Geometry Variant
 * Holds any supported geometry type
 */
using GeometryVariant = std::variant<
    rude::MeshPtr,
    rude::HalfEdgeMeshPtr,
    NURBSSurfacePtr,
    SubdivisionMeshPtr,
    VoxelGridPtr,
    PointCloudPtr,
    ImplicitSurfacePtr
>;

/**
 * @brief Hybrid Geometry Object
 * Manages multiple representations of the same geometry for optimal workflow support
 */
class HybridGeometry {
public:
    explicit HybridGeometry(GeometryVariant primary);
    ~HybridGeometry() = default;

    // Primary representation access
    GeometryRepresentation getPrimaryType() const { return m_primaryType; }
    const GeometryVariant& getPrimary() const { return m_primary; }
    GeometryVariant& getPrimary() { return m_primary; }
    
    // Typed access to primary representation
    template<typename T>
    std::shared_ptr<T> getPrimaryAs() const {
        if (auto ptr = std::get_if<std::shared_ptr<T>>(&m_primary)) {
            return *ptr;
        }
        return nullptr;
    }
    
    // Cache management for different representations
    rude::MeshPtr getFaceVertexMesh(bool forceUpdate = false);
    rude::HalfEdgeMeshPtr getHalfEdgeMesh(bool forceUpdate = false);
    NURBSSurfacePtr getNURBSSurface(bool forceUpdate = false);
    SubdivisionMeshPtr getSubdivisionMesh(bool forceUpdate = false);
    VoxelGridPtr getVoxelGrid(float voxelSize = 0.1f, bool forceUpdate = false);
    PointCloudPtr getPointCloud(int samplesPerFace = 10, bool forceUpdate = false);
    ImplicitSurfacePtr getImplicitSurface(float bandwidth = 2.0f, bool forceUpdate = false);

    // Rendering optimization
    rude::MeshPtr getRenderMesh(int subdivisionLevel = -1);
    BVHTreePtr getBVH(bool forceUpdate = false);
    
    // Update primary representation
    void updatePrimary(GeometryVariant newPrimary);
    void convertPrimaryTo(GeometryRepresentation targetType);
    
    // Cache invalidation
    void invalidateCache();
    void invalidateSpecificCache(GeometryRepresentation type);
    
    // Properties
    bool hasCache(GeometryRepresentation type) const;
    size_t getCacheMemoryUsage() const;
    void clearUnusedCache(size_t maxAge = 1000); // age in milliseconds
    
    // Metadata
    void setMetadata(const std::string& key, const std::string& value);
    std::string getMetadata(const std::string& key) const;
    const std::unordered_map<std::string, std::string>& getAllMetadata() const { return m_metadata; }

private:
    GeometryVariant m_primary;
    GeometryRepresentation m_primaryType;
    
    // Cached representations
    struct CacheEntry {
        GeometryVariant geometry;
        uint64_t timestamp;
        bool valid;
        
        CacheEntry() : timestamp(0), valid(false) {}
        CacheEntry(GeometryVariant geom) : geometry(geom), timestamp(getCurrentTime()), valid(true) {}
    };
    
    mutable std::unordered_map<GeometryRepresentation, CacheEntry> m_cache;
    mutable BVHTreePtr m_bvhCache;
    mutable bool m_bvhValid = false;
    
    // Metadata storage
    std::unordered_map<std::string, std::string> m_metadata;
    
    // Helper functions
    static uint64_t getCurrentTime();
    static GeometryRepresentation getRepresentationType(const GeometryVariant& variant);
    
    // Conversion helpers
    template<typename T>
    std::shared_ptr<T> getCachedAs(GeometryRepresentation type, bool forceUpdate) const;
    
    // Cache management helpers
    void setCacheEntry(GeometryRepresentation type, GeometryVariant geometry) const;
    bool isCacheValid(GeometryRepresentation type) const;
};

/**
 * @brief Hybrid Geometry Manager
 * Centralized management of hybrid geometry objects with optimization and memory management
 */
class HybridGeometryManager {
public:
    HybridGeometryManager() = default;
    ~HybridGeometryManager() = default;

    // Geometry creation
    std::shared_ptr<HybridGeometry> createGeometry(GeometryVariant geometry);
    std::shared_ptr<HybridGeometry> createFromMesh(rude::MeshPtr mesh);
    std::shared_ptr<HybridGeometry> createFromHalfEdge(rude::HalfEdgeMeshPtr halfEdge);
    std::shared_ptr<HybridGeometry> createFromNURBS(NURBSSurfacePtr nurbs);
    std::shared_ptr<HybridGeometry> createFromPointCloud(PointCloudPtr pointCloud);
    
    // Geometry management
    void registerGeometry(const std::string& id, std::shared_ptr<HybridGeometry> geometry);
    std::shared_ptr<HybridGeometry> getGeometry(const std::string& id);
    void removeGeometry(const std::string& id);
    
    // Global operations
    void invalidateAllCaches();
    void cleanupUnusedCaches(size_t maxAge = 5000);
    size_t getTotalMemoryUsage() const;
    
    // Statistics
    struct Statistics {
        size_t totalGeometries = 0;
        size_t totalCacheEntries = 0;
        size_t memoryUsage = 0;
        std::unordered_map<GeometryRepresentation, size_t> representationCounts;
    };
    
    Statistics getStatistics() const;
    
    // Configuration
    void setDefaultVoxelSize(float size) { m_defaultVoxelSize = size; }
    void setDefaultSubdivisionLevel(int level) { m_defaultSubdivisionLevel = level; }
    void setMaxCacheAge(size_t maxAge) { m_maxCacheAge = maxAge; }
    void setMaxCacheMemory(size_t maxMemory) { m_maxCacheMemory = maxMemory; }
    
    float getDefaultVoxelSize() const { return m_defaultVoxelSize; }
    int getDefaultSubdivisionLevel() const { return m_defaultSubdivisionLevel; }

private:
    std::unordered_map<std::string, std::shared_ptr<HybridGeometry>> m_geometries;
    
    // Configuration
    float m_defaultVoxelSize = 0.1f;
    int m_defaultSubdivisionLevel = 2;
    size_t m_maxCacheAge = 10000; // milliseconds
    size_t m_maxCacheMemory = 1024 * 1024 * 1024; // 1GB
    
    // Memory management
    void enforceMemoryLimits();
    void cleanupOldestCaches();
};

/**
 * @brief Geometry Processing Pipeline
 * High-level operations that work across different geometry representations
 */
class GeometryProcessingPipeline {
public:
    explicit GeometryProcessingPipeline(HybridGeometryManager* manager);
    ~GeometryProcessingPipeline() = default;

    // Boolean operations (works with any representation)
    std::shared_ptr<HybridGeometry> unionGeometry(
        std::shared_ptr<HybridGeometry> a, 
        std::shared_ptr<HybridGeometry> b,
        GeometryRepresentation outputType = GeometryRepresentation::HalfEdge);
    
    std::shared_ptr<HybridGeometry> subtractGeometry(
        std::shared_ptr<HybridGeometry> a, 
        std::shared_ptr<HybridGeometry> b,
        GeometryRepresentation outputType = GeometryRepresentation::HalfEdge);
    
    std::shared_ptr<HybridGeometry> intersectGeometry(
        std::shared_ptr<HybridGeometry> a, 
        std::shared_ptr<HybridGeometry> b,
        GeometryRepresentation outputType = GeometryRepresentation::HalfEdge);
    
    // Smoothing operations
    std::shared_ptr<HybridGeometry> subdivide(
        std::shared_ptr<HybridGeometry> geometry,
        int levels = 1,
        bool useSubdivisionSurface = true);
    
    std::shared_ptr<HybridGeometry> smooth(
        std::shared_ptr<HybridGeometry> geometry,
        float factor = 0.5f,
        int iterations = 1);
    
    // Mesh repair and optimization
    std::shared_ptr<HybridGeometry> repairMesh(std::shared_ptr<HybridGeometry> geometry);
    std::shared_ptr<HybridGeometry> simplifyMesh(std::shared_ptr<HybridGeometry> geometry, float ratio = 0.5f);
    std::shared_ptr<HybridGeometry> remeshUniform(std::shared_ptr<HybridGeometry> geometry, float targetEdgeLength);
    
    // Format conversions with optimization
    std::shared_ptr<HybridGeometry> convertForRendering(std::shared_ptr<HybridGeometry> geometry);
    std::shared_ptr<HybridGeometry> convertForEditing(std::shared_ptr<HybridGeometry> geometry);
    std::shared_ptr<HybridGeometry> convertForCAD(std::shared_ptr<HybridGeometry> geometry);
    std::shared_ptr<HybridGeometry> convertForSculpting(std::shared_ptr<HybridGeometry> geometry, float voxelSize = 0.05f);
    
    // Analysis operations
    struct GeometryAnalysis {
        float volume = 0.0f;
        float surfaceArea = 0.0f;
        glm::vec3 centroid = glm::vec3(0.0f);
        glm::vec3 boundingBoxMin = glm::vec3(0.0f), boundingBoxMax = glm::vec3(0.0f);
        bool isManifold = false;
        bool isWatertight = false;
        int genusApproximation = 0;
        size_t vertexCount = 0;
        size_t faceCount = 0;
        size_t edgeCount = 0;
    };
    
    GeometryAnalysis analyzeGeometry(std::shared_ptr<HybridGeometry> geometry);

private:
    HybridGeometryManager* m_manager;
    
    // Boolean operation helpers
    std::shared_ptr<HybridGeometry> performVoxelBoolean(
        std::shared_ptr<HybridGeometry> a,
        std::shared_ptr<HybridGeometry> b,
        const std::function<void(VoxelGridPtr, const VoxelGridPtr&)>& operation,
        GeometryRepresentation outputType);
    
    std::shared_ptr<HybridGeometry> performImplicitBoolean(
        std::shared_ptr<HybridGeometry> a,
        std::shared_ptr<HybridGeometry> b,
        const std::function<ImplicitSurface(const ImplicitSurface&, const ImplicitSurface&)>& operation,
        GeometryRepresentation outputType);
};
