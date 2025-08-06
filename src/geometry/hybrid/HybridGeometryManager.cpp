
#include "HybridGeometryManager.h"
#include <chrono>
#include <algorithm>
#include <stdexcept>
#include "geometry/core/GeometryTypes.h"
#include "geometry/subdivision/SubdivisionMesh.h"
#include "core/mesh.hpp"
#include "core/mesh_elements.hpp"

// ===============================
// HybridGeometry Implementation
// ===============================

HybridGeometry::HybridGeometry(GeometryVariant primary) 
    : m_primary(primary), m_primaryType(getRepresentationType(primary)) {}

rude::MeshPtr HybridGeometry::getFaceVertexMesh(bool forceUpdate) {
    if (m_primaryType == GeometryRepresentation::FaceVertex && !forceUpdate) {
        return getPrimaryAs<rude::Mesh>();
    }
    return getCachedAs<rude::Mesh>(GeometryRepresentation::FaceVertex, forceUpdate);
}

rude::HalfEdgeMeshPtr HybridGeometry::getHalfEdgeMesh(bool forceUpdate) {
    if (m_primaryType == GeometryRepresentation::HalfEdge && !forceUpdate) {
        return getPrimaryAs<rude::HalfEdgeMesh>();
    }
    return getCachedAs<rude::HalfEdgeMesh>(GeometryRepresentation::HalfEdge, forceUpdate);
}

NURBSSurfacePtr HybridGeometry::getNURBSSurface(bool forceUpdate) {
    if (m_primaryType == GeometryRepresentation::NURBS && !forceUpdate) {
        return getPrimaryAs<NURBSSurface>();
    }
    return getCachedAs<NURBSSurface>(GeometryRepresentation::NURBS, forceUpdate);
}

SubdivisionMeshPtr HybridGeometry::getSubdivisionMesh(bool forceUpdate) {
    if (m_primaryType == GeometryRepresentation::Subdivision && !forceUpdate) {
        return getPrimaryAs<SubdivisionMesh>();
    }
    return getCachedAs<SubdivisionMesh>(GeometryRepresentation::Subdivision, forceUpdate);
}

VoxelGridPtr HybridGeometry::getVoxelGrid(float voxelSize, bool forceUpdate) {
    if (m_primaryType == GeometryRepresentation::Voxel && !forceUpdate) {
        return getPrimaryAs<VoxelGrid>();
    }
    
    // Check cache first
    if (!forceUpdate && isCacheValid(GeometryRepresentation::Voxel)) {
        auto cached = getCachedAs<VoxelGrid>(GeometryRepresentation::Voxel, false);
        if (cached && std::abs(cached->getVoxelSize() - voxelSize) < EPSILON) {
            return cached;
        }
    }
    
    // Convert to voxel grid
    VoxelGridPtr voxelGrid;
    
    switch (m_primaryType) {
        case GeometryRepresentation::FaceVertex: {
            auto mesh = getPrimaryAs<rude::Mesh>();
            voxelGrid = GeometryConverter::toVoxels(mesh, voxelSize);
            break;
        }
        case GeometryRepresentation::HalfEdge: {
            auto halfEdge = getPrimaryAs<rude::HalfEdgeMesh>();
            voxelGrid = GeometryConverter::toVoxels(halfEdge, voxelSize);
            break;
        }
        case GeometryRepresentation::NURBS: {
            auto nurbs = getPrimaryAs<NURBSSurface>();
            auto mesh = GeometryConverter::toFaceVertex(nurbs);
            voxelGrid = GeometryConverter::toVoxels(mesh, voxelSize);
            break;
        }
        case GeometryRepresentation::Subdivision: {
            auto subdivision = getPrimaryAs<SubdivisionMesh>();
            auto mesh = GeometryConverter::toFaceVertex(subdivision);
            voxelGrid = GeometryConverter::toVoxels(mesh, voxelSize);
            break;
        }
        case GeometryRepresentation::PointCloud: {
            auto pointCloud = getPrimaryAs<PointCloud>();
            auto mesh = GeometryConverter::toFaceVertex(pointCloud);
            voxelGrid = GeometryConverter::toVoxels(mesh, voxelSize);
            break;
        }
        case GeometryRepresentation::Implicit: {
            // TODO: Implement implicit to voxel conversion
            break;
        }
    }
    
    if (voxelGrid) {
        setCacheEntry(GeometryRepresentation::Voxel, voxelGrid);
    }
    
    return voxelGrid;
}

PointCloudPtr HybridGeometry::getPointCloud(int samplesPerFace, bool forceUpdate) {
    if (m_primaryType == GeometryRepresentation::PointCloud && !forceUpdate) {
        return getPrimaryAs<PointCloud>();
    }
    return getCachedAs<PointCloud>(GeometryRepresentation::PointCloud, forceUpdate);
}

ImplicitSurfacePtr HybridGeometry::getImplicitSurface(float bandwidth, bool forceUpdate) {
    if (m_primaryType == GeometryRepresentation::Implicit && !forceUpdate) {
        return getPrimaryAs<ImplicitSurface>();
    }
    return getCachedAs<ImplicitSurface>(GeometryRepresentation::Implicit, forceUpdate);
}

rude::MeshPtr HybridGeometry::getRenderMesh(int subdivisionLevel) {
    if (m_primaryType == GeometryRepresentation::Subdivision) {
        auto subdivision = getPrimaryAs<SubdivisionMesh>();
        return subdivision->generateRenderMesh(subdivisionLevel);
    }
    return getFaceVertexMesh();
}

BVHTreePtr HybridGeometry::getBVH(bool forceUpdate) {
    if (!forceUpdate && m_bvhValid && m_bvhCache) {
        return m_bvhCache;
    }
    
    auto mesh = getFaceVertexMesh();
    if (mesh) {
        m_bvhCache = GeometryConverter::createBVH(mesh);
        m_bvhValid = true;
    }
    
    return m_bvhCache;
}

void HybridGeometry::updatePrimary(GeometryVariant newPrimary) {
    m_primary = newPrimary;
    m_primaryType = getRepresentationType(newPrimary);
    invalidateCache();
}

void HybridGeometry::convertPrimaryTo(GeometryRepresentation targetType) {
    if (m_primaryType == targetType) {
        return;
    }
    
    GeometryVariant newPrimary;
    
    switch (targetType) {
        case GeometryRepresentation::FaceVertex:
            newPrimary = getFaceVertexMesh(true);
            break;
        case GeometryRepresentation::HalfEdge:
            newPrimary = getHalfEdgeMesh(true);
            break;
        case GeometryRepresentation::NURBS:
            newPrimary = getNURBSSurface(true);
            break;
        case GeometryRepresentation::Subdivision:
            newPrimary = getSubdivisionMesh(true);
            break;
        case GeometryRepresentation::Voxel:
            newPrimary = getVoxelGrid(0.1f, true);
            break;
        case GeometryRepresentation::PointCloud:
            newPrimary = getPointCloud(10, true);
            break;
        case GeometryRepresentation::Implicit:
            newPrimary = getImplicitSurface(2.0f, true);
            break;
    }
    
    updatePrimary(newPrimary);
}

void HybridGeometry::invalidateCache() {
    m_cache.clear();
    m_bvhValid = false;
    m_bvhCache.reset();
}

void HybridGeometry::invalidateSpecificCache(GeometryRepresentation type) {
    auto it = m_cache.find(type);
    if (it != m_cache.end()) {
        it->second.valid = false;
    }
    
    if (type == GeometryRepresentation::FaceVertex) {
        m_bvhValid = false;
        m_bvhCache.reset();
    }
}

bool HybridGeometry::hasCache(GeometryRepresentation type) const {
    auto it = m_cache.find(type);
    return it != m_cache.end() && it->second.valid;
}

void HybridGeometry::setMetadata(const std::string& key, const std::string& value) {
    m_metadata[key] = value;
}

std::string HybridGeometry::getMetadata(const std::string& key) const {
    auto it = m_metadata.find(key);
    return (it != m_metadata.end()) ? it->second : std::string();
}

uint64_t HybridGeometry::getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

GeometryRepresentation HybridGeometry::getRepresentationType(const GeometryVariant& variant) {
    return std::visit([](const auto& ptr) -> GeometryRepresentation {
        using T = std::decay_t<decltype(*ptr)>;
        if constexpr (std::is_same_v<T, rude::Mesh>) {
            return GeometryRepresentation::FaceVertex;
        } else if constexpr (std::is_same_v<T, rude::HalfEdgeMesh>) {
            return GeometryRepresentation::HalfEdge;
        } else if constexpr (std::is_same_v<T, NURBSSurface>) {
            return GeometryRepresentation::NURBS;
        } else if constexpr (std::is_same_v<T, SubdivisionMesh>) {
            return GeometryRepresentation::Subdivision;
        } else if constexpr (std::is_same_v<T, VoxelGrid>) {
            return GeometryRepresentation::Voxel;
        } else if constexpr (std::is_same_v<T, PointCloud>) {
            return GeometryRepresentation::PointCloud;
        } else if constexpr (std::is_same_v<T, ImplicitSurface>) {
            return GeometryRepresentation::Implicit;
        }
        return GeometryRepresentation::FaceVertex; // fallback
    }, variant);
}

template<typename T>
std::shared_ptr<T> HybridGeometry::getCachedAs(GeometryRepresentation type, bool forceUpdate) const {
    if (!forceUpdate && isCacheValid(type)) {
        auto it = m_cache.find(type);
        if (it != m_cache.end()) {
            if (auto ptr = std::get_if<std::shared_ptr<T>>(&it->second.geometry)) {
                return *ptr;
            }
        }
    }
    
    // Convert from primary using GeometryConverter
    std::shared_ptr<T> converted;
    
    switch (type) {
        case GeometryRepresentation::FaceVertex: {
            if constexpr (std::is_same_v<T, rude::Mesh>) {
                switch (m_primaryType) {
                    case GeometryRepresentation::HalfEdge:
                        converted = GeometryConverter::toFaceVertex(getPrimaryAs<rude::HalfEdgeMesh>());
                        break;
                    // Add other conversions as they become available
                    default:
                        break;
                }
            }
            break;
        }
        case GeometryRepresentation::HalfEdge: {
            if constexpr (std::is_same_v<T, rude::HalfEdgeMesh>) {
                switch (m_primaryType) {
                    case GeometryRepresentation::FaceVertex:
                        converted = GeometryConverter::toHalfEdge(getPrimaryAs<rude::Mesh>());
                        break;
                    // Add other conversions as they become available
                    default:
                        break;
                }
            }
            break;
        }
        // Add other target types as they become available
        default:
            break;
    }
    
    if (converted) {
        setCacheEntry(type, converted);
    }
    
    return converted;
}

void HybridGeometry::setCacheEntry(GeometryRepresentation type, GeometryVariant geometry) const {
    m_cache[type] = CacheEntry(geometry);
}

bool HybridGeometry::isCacheValid(GeometryRepresentation type) const {
    auto it = m_cache.find(type);
    return it != m_cache.end() && it->second.valid;
}

size_t HybridGeometry::getCacheMemoryUsage() const {
    size_t totalSize = 0;
    
    // Estimate memory usage of cached representations
    for (const auto& [type, entry] : m_cache) {
        if (entry.valid) {
            // Rough estimate - in practice, this would be more sophisticated
            totalSize += 1024 * 1024; // 1MB per cache entry (placeholder)
        }
    }
    
    return totalSize;
}

void HybridGeometry::clearUnusedCache(size_t maxAge) {
    uint64_t currentTime = getCurrentTime();
    
    auto it = m_cache.begin();
    while (it != m_cache.end()) {
        if (currentTime - it->second.timestamp > maxAge) {
            it = m_cache.erase(it);
        } else {
            ++it;
        }
    }
}

// ===============================
// HybridGeometryManager Implementation
// ===============================

std::shared_ptr<HybridGeometry> HybridGeometryManager::createGeometry(GeometryVariant geometry) {
    return std::make_shared<HybridGeometry>(geometry);
}

std::shared_ptr<HybridGeometry> HybridGeometryManager::createFromMesh(rude::MeshPtr mesh) {
    return createGeometry(mesh);
}

std::shared_ptr<HybridGeometry> HybridGeometryManager::createFromHalfEdge(rude::HalfEdgeMeshPtr halfEdge) {
    return createGeometry(halfEdge);
}

std::shared_ptr<HybridGeometry> HybridGeometryManager::createFromNURBS(NURBSSurfacePtr nurbs) {
    return createGeometry(nurbs);
}

std::shared_ptr<HybridGeometry> HybridGeometryManager::createFromPointCloud(PointCloudPtr pointCloud) {
    return createGeometry(pointCloud);
}

void HybridGeometryManager::registerGeometry(const std::string& id, std::shared_ptr<HybridGeometry> geometry) {
    m_geometries[id] = geometry;
}

std::shared_ptr<HybridGeometry> HybridGeometryManager::getGeometry(const std::string& id) {
    auto it = m_geometries.find(id);
    return (it != m_geometries.end()) ? it->second : nullptr;
}

void HybridGeometryManager::removeGeometry(const std::string& id) {
    m_geometries.erase(id);
}

void HybridGeometryManager::invalidateAllCaches() {
    for (auto& [id, geometry] : m_geometries) {
        geometry->invalidateCache();
    }
}

void HybridGeometryManager::cleanupUnusedCaches(size_t maxAge) {
    for (auto& [id, geometry] : m_geometries) {
        geometry->clearUnusedCache(maxAge);
    }
}

HybridGeometryManager::Statistics HybridGeometryManager::getStatistics() const {
    Statistics stats;
    stats.totalGeometries = m_geometries.size();
    
    for (const auto& [id, geometry] : m_geometries) {
        stats.memoryUsage += geometry->getCacheMemoryUsage();
        
        // Count representation types
        auto primaryType = geometry->getPrimaryType();
        stats.representationCounts[primaryType]++;
        
        // Count cache entries
        for (int i = 0; i < 7; ++i) { // 7 geometry representation types
            auto type = static_cast<GeometryRepresentation>(i);
            if (geometry->hasCache(type)) {
                stats.totalCacheEntries++;
            }
        }
    }
    
    return stats;
}

// ===============================
// GeometryProcessingPipeline Implementation
// ===============================

GeometryProcessingPipeline::GeometryProcessingPipeline(HybridGeometryManager* manager) 
    : m_manager(manager) {}

std::shared_ptr<HybridGeometry> GeometryProcessingPipeline::unionGeometry(
    std::shared_ptr<HybridGeometry> a, 
    std::shared_ptr<HybridGeometry> b,
    GeometryRepresentation outputType) {
    
    // Try voxel-based boolean first for robustness
    return performVoxelBoolean(a, b, [](VoxelGridPtr grid, const VoxelGridPtr& other) {
        grid->unionWith(*other);
    }, outputType);
}

std::shared_ptr<HybridGeometry> GeometryProcessingPipeline::subtractGeometry(
    std::shared_ptr<HybridGeometry> a, 
    std::shared_ptr<HybridGeometry> b,
    GeometryRepresentation outputType) {
    
    return performVoxelBoolean(a, b, [](VoxelGridPtr grid, const VoxelGridPtr& other) {
        grid->subtractFrom(*other);
    }, outputType);
}

std::shared_ptr<HybridGeometry> GeometryProcessingPipeline::intersectGeometry(
    std::shared_ptr<HybridGeometry> a, 
    std::shared_ptr<HybridGeometry> b,
    GeometryRepresentation outputType) {
    
    return performVoxelBoolean(a, b, [](VoxelGridPtr grid, const VoxelGridPtr& other) {
        grid->intersectWith(*other);
    }, outputType);
}

std::shared_ptr<HybridGeometry> GeometryProcessingPipeline::subdivide(
    std::shared_ptr<HybridGeometry> geometry,
    int levels,
    bool useSubdivisionSurface) {
    
    if (useSubdivisionSurface) {
        auto subdivisionMesh = geometry->getSubdivisionMesh();
        if (!subdivisionMesh) {
            auto halfEdge = geometry->getHalfEdgeMesh();
            subdivisionMesh = GeometryConverter::toSubdivision(halfEdge);
        }
        
        subdivisionMesh->subdivide(levels);
        return m_manager->createGeometry(subdivisionMesh);
    } else {
        // Use simple subdivision on half-edge mesh
        auto halfEdge = geometry->getHalfEdgeMesh();
        // TODO: Implement simple subdivision
        return geometry; // placeholder
    }
}

std::shared_ptr<HybridGeometry> GeometryProcessingPipeline::convertForRendering(std::shared_ptr<HybridGeometry> geometry) {
    // Ensure we have an optimized face-vertex mesh
    auto mesh = geometry->getFaceVertexMesh();
    GeometryConverter::optimizeMeshForRendering(mesh);
    return m_manager->createFromMesh(mesh);
}

std::shared_ptr<HybridGeometry> GeometryProcessingPipeline::convertForEditing(std::shared_ptr<HybridGeometry> geometry) {
    // Ensure we have a half-edge mesh optimized for editing
    auto halfEdge = geometry->getHalfEdgeMesh();
    GeometryConverter::optimizeHalfEdgeForEditing(halfEdge);
    return m_manager->createFromHalfEdge(halfEdge);
}

std::shared_ptr<HybridGeometry> GeometryProcessingPipeline::convertForSculpting(std::shared_ptr<HybridGeometry> geometry, float voxelSize) {
    // Convert to voxel grid for sculpting operations
    auto voxelGrid = geometry->getVoxelGrid(voxelSize);
    return m_manager->createGeometry(voxelGrid);
}

GeometryProcessingPipeline::GeometryAnalysis GeometryProcessingPipeline::analyzeGeometry(std::shared_ptr<HybridGeometry> geometry) {
    GeometryAnalysis analysis;
    
    auto mesh = geometry->getFaceVertexMesh();
    if (!mesh) {
        return analysis;
    }
    
    const auto& vertices = mesh->getVertices();
    const auto& indices = mesh->getIndices();
    
    analysis.vertexCount = vertices.size();
    analysis.faceCount = indices.size() / 3;
    
    // Compute bounding box and centroid
    if (!vertices.empty()) {
        auto firstPos = vertices[0].position;
        analysis.boundingBoxMin = analysis.boundingBoxMax = firstPos;
        analysis.centroid = glm::vec3(0.0f);
        
        for (const auto& vertex : vertices) {
            const glm::vec3& pos = vertex.position;
            analysis.boundingBoxMin.x = std::min(analysis.boundingBoxMin.x, pos.x);
            analysis.boundingBoxMin.y = std::min(analysis.boundingBoxMin.y, pos.y);
            analysis.boundingBoxMin.z = std::min(analysis.boundingBoxMin.z, pos.z);
            analysis.boundingBoxMax.x = std::max(analysis.boundingBoxMax.x, pos.x);
            analysis.boundingBoxMax.y = std::max(analysis.boundingBoxMax.y, pos.y);
            analysis.boundingBoxMax.z = std::max(analysis.boundingBoxMax.z, pos.z);
            analysis.centroid += pos;
        }
        analysis.centroid /= static_cast<float>(vertices.size());
    }
    
    // Compute surface area
    for (size_t i = 0; i < indices.size(); i += 3) {
        const glm::vec3& v0 = vertices[indices[i]].position;
        const glm::vec3& v1 = vertices[indices[i + 1]].position;
        const glm::vec3& v2 = vertices[indices[i + 2]].position;
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        float triangleArea = glm::length(glm::cross(edge1, edge2)) * 0.5f;
        analysis.surfaceArea += triangleArea;
    }
    
    // TODO: Implement volume calculation, manifold checking, etc.
    
    return analysis;
}

std::shared_ptr<HybridGeometry> GeometryProcessingPipeline::performVoxelBoolean(
    std::shared_ptr<HybridGeometry> a,
    std::shared_ptr<HybridGeometry> b,
    const std::function<void(VoxelGridPtr, const VoxelGridPtr&)>& operation,
    GeometryRepresentation outputType) {
    
    // Convert both geometries to voxel grids with same resolution
    float voxelSize = m_manager->getDefaultVoxelSize();
    auto voxelA = a->getVoxelGrid(voxelSize);
    auto voxelB = b->getVoxelGrid(voxelSize);
    
    if (!voxelA || !voxelB) {
        return nullptr;
    }
    
    // Perform boolean operation
    operation(voxelA, voxelB);
    
    // Convert result to desired output type
    auto result = m_manager->createGeometry(voxelA);
    result->convertPrimaryTo(outputType);
    
    return result;
}
