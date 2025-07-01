#pragma once

#include "Common.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

// Forward declarations for all geometry types
class HalfEdgeMesh;
class NURBSSurface;
class SubdivisionMesh;
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
    struct ControlPoint {
        QVector3D position;
        float weight = 1.0f;
        
        ControlPoint() = default;
        ControlPoint(const QVector3D& pos, float w = 1.0f) : position(pos), weight(w) {}
    };

    NURBSSurface(int degreeU, int degreeV, int controlPointsU, int controlPointsV);
    ~NURBSSurface() = default;

    // Control point access
    void setControlPoint(int u, int v, const ControlPoint& point);
    const ControlPoint& getControlPoint(int u, int v) const;
    
    // Knot vector management
    void setKnotU(int index, float value);
    void setKnotV(int index, float value);
    const std::vector<float>& getKnotsU() const { return m_knotsU; }
    const std::vector<float>& getKnotsV() const { return m_knotsV; }
    
    // Surface evaluation
    QVector3D evaluate(float u, float v) const;
    QVector3D evaluateDerivativeU(float u, float v) const;
    QVector3D evaluateDerivativeV(float u, float v) const;
    QVector3D evaluateNormal(float u, float v) const;
    
    // Conversion to mesh
    std::shared_ptr<Mesh> tessellate(int resolutionU = 32, int resolutionV = 32) const;
    
    // Properties
    int getDegreeU() const { return m_degreeU; }
    int getDegreeV() const { return m_degreeV; }
    int getControlPointsU() const { return m_controlPointsU; }
    int getControlPointsV() const { return m_controlPointsV; }

private:
    int m_degreeU, m_degreeV;
    int m_controlPointsU, m_controlPointsV;
    std::vector<ControlPoint> m_controlPoints;
    std::vector<float> m_knotsU, m_knotsV;
    
    // NURBS basis function evaluation
    float basisFunction(int i, int degree, float t, const std::vector<float>& knots) const;
    float basisFunctionDerivative(int i, int degree, float t, const std::vector<float>& knots) const;
};

/**
 * @brief Subdivision Mesh
 * Catmull-Clark subdivision surface for smooth organic modeling
 */
class SubdivisionMesh {
public:
    struct SubdivisionLevel {
        std::shared_ptr<HalfEdgeMesh> mesh;
        int level;
        bool isQuadMesh; // true if all faces are quads
    };

    explicit SubdivisionMesh(std::shared_ptr<HalfEdgeMesh> baseMesh);
    ~SubdivisionMesh() = default;

    // Subdivision operations
    void subdivide(int levels = 1);
    void subdivideAdaptive(float threshold = 0.1f);
    
    // Level access
    std::shared_ptr<HalfEdgeMesh> getLevel(int level) const;
    std::shared_ptr<HalfEdgeMesh> getBaseLevel() const { return getLevel(0); }
    std::shared_ptr<HalfEdgeMesh> getFinestLevel() const { return getLevel(static_cast<int>(m_levels.size()) - 1); }
    int getNumLevels() const { return static_cast<int>(m_levels.size()); }
    
    // Conversion to mesh for rendering
    std::shared_ptr<Mesh> generateRenderMesh(int level = -1) const; // -1 = finest level
    
    // Subdivision settings
    void setBoundaryRule(bool sharp) { m_sharpBoundaries = sharp; }
    bool getBoundaryRule() const { return m_sharpBoundaries; }
    
    void setCreaseThreshold(float threshold) { m_creaseThreshold = threshold; }
    float getCreaseThreshold() const { return m_creaseThreshold; }

private:
    std::vector<SubdivisionLevel> m_levels;
    bool m_sharpBoundaries = false;
    float m_creaseThreshold = 30.0f; // degrees
    
    // Catmull-Clark subdivision implementation
    std::shared_ptr<HalfEdgeMesh> applyCatmullClark(std::shared_ptr<HalfEdgeMesh> mesh);
    void computeFacePoints(std::shared_ptr<HalfEdgeMesh> mesh, std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints);
    void computeEdgePoints(std::shared_ptr<HalfEdgeMesh> mesh, 
                          const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                          std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints);
    void computeVertexPoints(std::shared_ptr<HalfEdgeMesh> mesh,
                            const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                            const std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints,
                            std::unordered_map<HalfEdgeVertexPtr, QVector3D>& vertexPoints);
};

/**
 * @brief Voxel Grid
 * 3D voxel representation for sculpting and boolean operations
 */
class VoxelGrid {
public:
    struct Voxel {
        float density = 0.0f;  // Signed distance field value
        QVector3D gradient;    // Surface gradient for normals
        unsigned char material = 0; // Material ID
        
        bool isSolid() const { return density > 0.0f; }
        bool isEmpty() const { return density <= -1.0f; }
        bool isSurface() const { return std::abs(density) <= 1.0f; }
    };

    VoxelGrid(const QVector3D& origin, const QVector3D& size, int resolution);
    VoxelGrid(const QVector3D& origin, float voxelSize, int resX, int resY, int resZ);
    ~VoxelGrid() = default;

    // Voxel access
    Voxel& getVoxel(int x, int y, int z);
    const Voxel& getVoxel(int x, int y, int z) const;
    void setVoxel(int x, int y, int z, const Voxel& voxel);
    Voxel& getVoxelAt(const QVector3D& worldPos);
    const Voxel& getVoxelAt(const QVector3D& worldPos) const;
    
    // Conversion helpers
    QVector3D indexToWorld(int x, int y, int z) const;
    QVector3D worldToIndex(const QVector3D& worldPos) const;
    bool isValidIndex(int x, int y, int z) const;
    
    // Mesh conversion using Marching Cubes
    std::shared_ptr<Mesh> extractSurface(float isoValue = 0.0f) const;
    void fromMesh(std::shared_ptr<Mesh> mesh, float bandwidth = 2.0f);
    
    // Boolean operations
    void unionWith(const VoxelGrid& other);
    void subtractFrom(const VoxelGrid& other);
    void intersectWith(const VoxelGrid& other);
    
    // Properties
    const QVector3D& getOrigin() const { return m_origin; }
    const QVector3D& getSize() const { return m_size; }
    int getResolutionX() const { return m_resX; }
    int getResolutionY() const { return m_resY; }
    int getResolutionZ() const { return m_resZ; }
    float getVoxelSize() const { return m_voxelSize; }

private:
    QVector3D m_origin;
    QVector3D m_size;
    int m_resX, m_resY, m_resZ;
    float m_voxelSize;
    std::vector<Voxel> m_voxels;
    
    // Helper functions
    int getIndex(int x, int y, int z) const { return x + y * m_resX + z * m_resX * m_resY; }
    void computeGradients();
    void voxelizeTriangle(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, float bandwidth);
    float computeDistanceToTriangle(const QVector3D& point, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2);
    
    // Marching Cubes implementation
    struct MCVertex {
        QVector3D position;
        QVector3D normal;
    };
    void marchingCubes(std::vector<MCVertex>& vertices, std::vector<unsigned int>& indices, float isoValue) const;
    QVector3D interpolateVertex(const QVector3D& p1, const QVector3D& p2, float v1, float v2, float iso) const;
};

/**
 * @brief Point Cloud
 * Unstructured 3D point data for scanning input and processing
 */
class PointCloud {
public:
    struct Point {
        QVector3D position;
        QVector3D normal;
        QVector3D color = QVector3D(1.0f, 1.0f, 1.0f);
        float intensity = 1.0f;
        
        Point() = default;
        Point(const QVector3D& pos) : position(pos), normal(0, 1, 0) {}
        Point(const QVector3D& pos, const QVector3D& norm) : position(pos), normal(norm) {}
    };

    PointCloud() = default;
    explicit PointCloud(const std::vector<Point>& points) : m_points(points) {}
    ~PointCloud() = default;

    // Point access
    void addPoint(const Point& point) { m_points.push_back(point); }
    void addPoint(const QVector3D& position, const QVector3D& normal = QVector3D(0, 1, 0));
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
    std::shared_ptr<Mesh> poissonReconstruction(int depth = 8, float samplesPerNode = 1.5f) const;
    std::shared_ptr<Mesh> ballPivoting(float ballRadius = 0.1f) const;
    
    // Spatial queries
    std::vector<size_t> findKNearestNeighbors(const QVector3D& queryPoint, int k) const;
    std::vector<size_t> findRadiusNeighbors(const QVector3D& queryPoint, float radius) const;
    
    // Bounding box
    void computeBoundingBox(QVector3D& minBounds, QVector3D& maxBounds) const;

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
    void knnSearch(const KDNode* node, const QVector3D& query, int k, 
                   std::vector<std::pair<float, size_t>>& results) const;
};

/**
 * @brief Implicit Surface
 * Signed Distance Field representation for advanced operations
 */
class ImplicitSurface {
public:
    using SDFFunction = std::function<float(const QVector3D&)>;
    using GradientFunction = std::function<QVector3D(const QVector3D&)>;

    ImplicitSurface(SDFFunction sdf, GradientFunction gradient = nullptr);
    ~ImplicitSurface() = default;

    // SDF evaluation
    float evaluate(const QVector3D& point) const;
    QVector3D evaluateGradient(const QVector3D& point) const;
    QVector3D evaluateNormal(const QVector3D& point) const;
    
    // Surface extraction
    std::shared_ptr<Mesh> extractSurface(const QVector3D& bounds, float resolution, float isoValue = 0.0f) const;
    std::shared_ptr<VoxelGrid> toVoxelGrid(const QVector3D& bounds, float resolution) const;
    
    // Boolean operations (create new implicit surfaces)
    static ImplicitSurface unionOp(const ImplicitSurface& a, const ImplicitSurface& b);
    static ImplicitSurface intersectionOp(const ImplicitSurface& a, const ImplicitSurface& b);
    static ImplicitSurface differenceOp(const ImplicitSurface& a, const ImplicitSurface& b);
    
    // Primitive SDFs
    static ImplicitSurface sphere(const QVector3D& center, float radius);
    static ImplicitSurface box(const QVector3D& center, const QVector3D& size);
    static ImplicitSurface cylinder(const QVector3D& center, float radius, float height);
    static ImplicitSurface torus(const QVector3D& center, float majorRadius, float minorRadius);

private:
    SDFFunction m_sdf;
    GradientFunction m_gradient;
    
    // Numerical gradient computation if not provided
    QVector3D computeNumericalGradient(const QVector3D& point, float epsilon = 1e-4f) const;
};

/**
 * @brief BVH Tree
 * Bounding Volume Hierarchy for fast spatial queries and ray casting
 */
class BVHTree {
public:
    struct BoundingBox {
        QVector3D min, max;
        
        BoundingBox() : min(FLT_MAX, FLT_MAX, FLT_MAX), max(-FLT_MAX, -FLT_MAX, -FLT_MAX) {}
        BoundingBox(const QVector3D& minBounds, const QVector3D& maxBounds) : min(minBounds), max(maxBounds) {}
        
        void expand(const QVector3D& point);
        void expand(const BoundingBox& box);
        bool intersects(const BoundingBox& other) const;
        bool contains(const QVector3D& point) const;
        QVector3D getCenter() const { return (min + max) * 0.5f; }
        QVector3D getSize() const { return max - min; }
        float getSurfaceArea() const;
    };
    
    struct Ray {
        QVector3D origin;
        QVector3D direction;
        float tMin = 0.0f;
        float tMax = FLT_MAX;
        
        Ray(const QVector3D& o, const QVector3D& d) : origin(o), direction(d) {}
        QVector3D pointAt(float t) const { return origin + direction * t; }
    };
    
    struct RayHit {
        bool hit = false;
        float distance = FLT_MAX;
        QVector3D point;
        QVector3D normal;
        size_t primitiveIndex = SIZE_MAX;
    };

    explicit BVHTree(std::shared_ptr<Mesh> mesh);
    ~BVHTree() = default;
    
    // Ray casting
    RayHit raycast(const Ray& ray) const;
    std::vector<RayHit> raycastAll(const Ray& ray) const;
    
    // Spatial queries
    std::vector<size_t> query(const BoundingBox& bounds) const;
    std::vector<size_t> query(const QVector3D& point, float radius) const;
    
    // Closest point queries
    std::pair<QVector3D, float> closestPoint(const QVector3D& query) const;

private:
    struct BVHNode {
        BoundingBox bounds;
        std::unique_ptr<BVHNode> left, right;
        std::vector<size_t> primitiveIndices;
        bool isLeaf() const { return !left && !right; }
    };
    
    std::shared_ptr<Mesh> m_mesh;
    std::unique_ptr<BVHNode> m_root;
    std::vector<BoundingBox> m_primitiveBounds;
    
    void buildTree();
    std::unique_ptr<BVHNode> buildRecursive(std::vector<size_t>& primitives, int depth);
    BoundingBox computePrimitiveBounds(size_t primitiveIndex) const;
    bool rayTriangleIntersect(const Ray& ray, size_t triangleIndex, float& t, QVector3D& normal) const;
};

// Smart pointer types for all geometry classes
using NURBSSurfacePtr = std::shared_ptr<NURBSSurface>;
using SubdivisionMeshPtr = std::shared_ptr<SubdivisionMesh>;
using VoxelGridPtr = std::shared_ptr<VoxelGrid>;
using PointCloudPtr = std::shared_ptr<PointCloud>;
using ImplicitSurfacePtr = std::shared_ptr<ImplicitSurface>;
using BVHTreePtr = std::shared_ptr<BVHTree>;
