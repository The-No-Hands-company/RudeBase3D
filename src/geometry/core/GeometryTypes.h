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

// Forward declarations for all geometry types
// class HalfEdgeMesh; // No longer needed, use rude::HalfEdgeMeshPtr
struct ControlPoint;
class NURBSSurface;
class VoxelGrid;
class PointCloud;
class ImplicitSurface;
class BVHTree;

/**
 * @brief NURBS Surface
 * Non-Uniform Rational B-Spline surface for precise CAD-like modeling
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
 * @brief Point Cloud
 * Unstructured 3D point data for scanning input and processing
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
 * @brief Implicit Surface
 * Signed Distance Field representation for advanced operations
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
 * @brief BVH Tree
 * Bounding Volume Hierarchy for fast spatial queries and ray casting
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
