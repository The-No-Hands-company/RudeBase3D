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
