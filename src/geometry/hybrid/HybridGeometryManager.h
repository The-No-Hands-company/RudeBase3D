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
