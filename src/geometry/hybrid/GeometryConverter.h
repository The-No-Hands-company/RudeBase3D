#pragma once

#include "Common.h"
#include "HalfEdgeMesh.h"
#include "Mesh.h"
#include "GeometryTypes.h"
#include <memory>
#include <unordered_map>
#include <set>

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
    static HalfEdgeMeshPtr toHalfEdge(MeshPtr faceVertexMesh);
    static MeshPtr toFaceVertex(HalfEdgeMeshPtr halfEdgeMesh);
    
    // NURBS conversions
    static HalfEdgeMeshPtr toHalfEdge(NURBSSurfacePtr nurbs, int resolutionU = 32, int resolutionV = 32);
    static MeshPtr toFaceVertex(NURBSSurfacePtr nurbs, int resolutionU = 32, int resolutionV = 32);
    static NURBSSurfacePtr toNURBS(HalfEdgeMeshPtr halfEdge, int degreeU = 3, int degreeV = 3);
    
    // Subdivision surface conversions
    static SubdivisionMeshPtr toSubdivision(HalfEdgeMeshPtr baseMesh);
    static SubdivisionMeshPtr toSubdivision(MeshPtr baseMesh);
    static HalfEdgeMeshPtr toHalfEdge(SubdivisionMeshPtr subdivision, int level = -1);
    static MeshPtr toFaceVertex(SubdivisionMeshPtr subdivision, int level = -1);
    
    // Voxel conversions
    static VoxelGridPtr toVoxels(MeshPtr mesh, float voxelSize, float bandwidth = 2.0f);
    static VoxelGridPtr toVoxels(HalfEdgeMeshPtr halfEdge, float voxelSize, float bandwidth = 2.0f);
    static MeshPtr toFaceVertex(VoxelGridPtr voxels, float isoValue = 0.0f);
    static HalfEdgeMeshPtr toHalfEdge(VoxelGridPtr voxels, float isoValue = 0.0f);
    
    // Point cloud conversions
    static PointCloudPtr toPointCloud(MeshPtr mesh, int samplesPerFace = 10);
    static PointCloudPtr toPointCloud(HalfEdgeMeshPtr halfEdge, int samplesPerFace = 10);
    static MeshPtr toFaceVertex(PointCloudPtr pointCloud, int reconstructionDepth = 8);
    static HalfEdgeMeshPtr toHalfEdge(PointCloudPtr pointCloud, int reconstructionDepth = 8);
    
    // Implicit surface conversions
    static ImplicitSurfacePtr toImplicitSurface(MeshPtr mesh, float bandwidth = 2.0f);
    static ImplicitSurfacePtr toImplicitSurface(VoxelGridPtr voxels);
    static MeshPtr toFaceVertex(ImplicitSurfacePtr implicit, const QVector3D& bounds, float resolution);
    static VoxelGridPtr toVoxels(ImplicitSurfacePtr implicit, const QVector3D& bounds, float resolution);
    
    // Advanced conversions
    static HalfEdgeMeshPtr fromVerticesAndFaces(
        const std::vector<QVector3D>& positions,
        const std::vector<std::vector<unsigned int>>& faces);
    
    static void toVerticesAndFaces(
        HalfEdgeMeshPtr halfEdgeMesh,
        std::vector<QVector3D>& positions,
        std::vector<QVector3D>& normals,
        std::vector<QVector2D>& texCoords,
        std::vector<unsigned int>& indices);

    // Validation and repair
    static bool validateConversion(MeshPtr original, HalfEdgeMeshPtr converted);
    static bool validateConversion(HalfEdgeMeshPtr original, MeshPtr converted);
    static bool validateTopology(HalfEdgeMeshPtr mesh);
    static void repairMesh(HalfEdgeMeshPtr mesh);
    
    // Utility functions
    static void copyVertexData(HalfEdgeVertexPtr src, Vertex& dst);
    static void copyVertexData(const Vertex& src, HalfEdgeVertexPtr dst);
    
    // Optimization utilities
    static void optimizeMeshForRendering(MeshPtr mesh);
    static void optimizeHalfEdgeForEditing(HalfEdgeMeshPtr mesh);
    static BVHTreePtr createBVH(MeshPtr mesh);
    
private:
    // Helper functions
    static void buildFaceVertexConnectivity(HalfEdgeMeshPtr mesh);
    static void optimizeVertexOrder(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    
    // NURBS fitting helpers
    static void fitNURBSToMesh(HalfEdgeMeshPtr mesh, NURBSSurfacePtr nurbs, int degreeU, int degreeV);
    static std::vector<float> computeKnotVector(int numControlPoints, int degree, bool clamped = true);
    
    // Voxelization helpers
    static void voxelizeTriangle(VoxelGridPtr grid, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, float bandwidth);
    static float computeSignedDistance(const QVector3D& point, MeshPtr mesh);
    
    // Point sampling helpers
    static void sampleTriangle(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, 
                              const QVector3D& n0, const QVector3D& n1, const QVector3D& n2,
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
    HalfEdgeMeshPtr getHalfEdge(MeshPtr mesh);
    MeshPtr getFaceVertex(HalfEdgeMeshPtr mesh);
    
    void invalidate(MeshPtr mesh);
    void invalidate(HalfEdgeMeshPtr mesh);
    
    void clear();
    size_t getCacheSize() const;

private:
    MeshConverterCache() = default;
    
    std::unordered_map<MeshPtr, HalfEdgeMeshPtr> m_faceVertexToHalfEdge;
    std::unordered_map<HalfEdgeMeshPtr, MeshPtr> m_halfEdgeToFaceVertex;
    
    // Weak references to track lifetime (using std::set since weak_ptr cannot be hashed)
    std::set<std::weak_ptr<Mesh>, std::owner_less<std::weak_ptr<Mesh>>> m_trackedFaceVertex;
    std::set<std::weak_ptr<HalfEdgeMesh>, std::owner_less<std::weak_ptr<HalfEdgeMesh>>> m_trackedHalfEdge;
    
    void cleanupExpiredEntries();
};
