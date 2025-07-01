#include "GeometryTypes.h"
#include "Mesh.h"
#include "HalfEdgeMesh.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <random>

// ===============================
// NURBS Surface Implementation
// ===============================

NURBSSurface::NURBSSurface(int degreeU, int degreeV, int controlPointsU, int controlPointsV)
    : m_degreeU(degreeU), m_degreeV(degreeV)
    , m_controlPointsU(controlPointsU), m_controlPointsV(controlPointsV)
{
    // Initialize control points
    m_controlPoints.resize(controlPointsU * controlPointsV);
    
    // Initialize knot vectors (uniform for now)
    int knotCountU = controlPointsU + degreeU + 1;
    int knotCountV = controlPointsV + degreeV + 1;
    
    m_knotsU.resize(knotCountU);
    m_knotsV.resize(knotCountV);
    
    // Create uniform knot vectors
    for (int i = 0; i < knotCountU; ++i) {
        m_knotsU[i] = static_cast<float>(i) / (knotCountU - 1);
    }
    for (int i = 0; i < knotCountV; ++i) {
        m_knotsV[i] = static_cast<float>(i) / (knotCountV - 1);
    }
}

void NURBSSurface::setControlPoint(int u, int v, const ControlPoint& point) {
    if (u >= 0 && u < m_controlPointsU && v >= 0 && v < m_controlPointsV) {
        m_controlPoints[v * m_controlPointsU + u] = point;
    }
}

const NURBSSurface::ControlPoint& NURBSSurface::getControlPoint(int u, int v) const {
    static ControlPoint defaultPoint;
    if (u >= 0 && u < m_controlPointsU && v >= 0 && v < m_controlPointsV) {
        return m_controlPoints[v * m_controlPointsU + u];
    }
    return defaultPoint;
}

QVector3D NURBSSurface::evaluate(float u, float v) const {
    QVector3D result(0, 0, 0);
    float totalWeight = 0.0f;
    
    for (int i = 0; i < m_controlPointsU; ++i) {
        for (int j = 0; j < m_controlPointsV; ++j) {
            float basisU = basisFunction(i, m_degreeU, u, m_knotsU);
            float basisV = basisFunction(j, m_degreeV, v, m_knotsV);
            const ControlPoint& cp = getControlPoint(i, j);
            
            float weight = basisU * basisV * cp.weight;
            result += cp.position * weight;
            totalWeight += weight;
        }
    }
    
    if (totalWeight > EPSILON) {
        result /= totalWeight;
    }
    
    return result;
}

QVector3D NURBSSurface::evaluateNormal(float u, float v) const {
    QVector3D du = evaluateDerivativeU(u, v);
    QVector3D dv = evaluateDerivativeV(u, v);
    return QVector3D::crossProduct(du, dv).normalized();
}

std::shared_ptr<Mesh> NURBSSurface::tessellate(int resolutionU, int resolutionV) const {
    auto mesh = std::make_shared<Mesh>();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Generate vertices
    for (int v = 0; v < resolutionV; ++v) {
        for (int u = 0; u < resolutionU; ++u) {
            float paramU = static_cast<float>(u) / (resolutionU - 1);
            float paramV = static_cast<float>(v) / (resolutionV - 1);
            
            QVector3D position = evaluate(paramU, paramV);
            QVector3D normal = evaluateNormal(paramU, paramV);
            QVector2D texCoord(paramU, paramV);
            
            vertices.emplace_back(position, normal, texCoord);
        }
    }
    
    // Generate indices for triangulated quads
    for (int v = 0; v < resolutionV - 1; ++v) {
        for (int u = 0; u < resolutionU - 1; ++u) {
            int i0 = v * resolutionU + u;
            int i1 = v * resolutionU + (u + 1);
            int i2 = (v + 1) * resolutionU + (u + 1);
            int i3 = (v + 1) * resolutionU + u;
            
            // Triangle 1
            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);
            
            // Triangle 2
            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    return mesh;
}

float NURBSSurface::basisFunction(int i, int degree, float t, const std::vector<float>& knots) const {
    // Cox-de Boor recursion formula (simplified implementation)
    if (degree == 0) {
        return (t >= knots[i] && t < knots[i + 1]) ? 1.0f : 0.0f;
    }
    
    float left = 0.0f, right = 0.0f;
    
    if (knots[i + degree] - knots[i] > EPSILON) {
        left = (t - knots[i]) / (knots[i + degree] - knots[i]) * basisFunction(i, degree - 1, t, knots);
    }
    
    if (knots[i + degree + 1] - knots[i + 1] > EPSILON) {
        right = (knots[i + degree + 1] - t) / (knots[i + degree + 1] - knots[i + 1]) * basisFunction(i + 1, degree - 1, t, knots);
    }
    
    return left + right;
}

QVector3D NURBSSurface::evaluateDerivativeU(float u, float v) const {
    // Numerical derivative for now (should implement analytical version)
    const float h = 1e-4f;
    QVector3D p1 = evaluate(u - h, v);
    QVector3D p2 = evaluate(u + h, v);
    return (p2 - p1) / (2.0f * h);
}

QVector3D NURBSSurface::evaluateDerivativeV(float u, float v) const {
    const float h = 1e-4f;
    QVector3D p1 = evaluate(u, v - h);
    QVector3D p2 = evaluate(u, v + h);
    return (p2 - p1) / (2.0f * h);
}

// ===============================
// Subdivision Mesh Implementation
// ===============================

SubdivisionMesh::SubdivisionMesh(std::shared_ptr<HalfEdgeMesh> baseMesh) {
    SubdivisionLevel level0;
    level0.mesh = baseMesh;
    level0.level = 0;
    level0.isQuadMesh = true; // TODO: Implement proper quad detection
    m_levels.push_back(level0);
}

void SubdivisionMesh::subdivide(int levels) {
    for (int i = 0; i < levels; ++i) {
        auto currentMesh = getFinestLevel();
        auto subdividedMesh = applyCatmullClark(currentMesh);
        
        SubdivisionLevel newLevel;
        newLevel.mesh = subdividedMesh;
        newLevel.level = static_cast<int>(m_levels.size());
        newLevel.isQuadMesh = true; // Catmull-Clark produces quads
        m_levels.push_back(newLevel);
    }
}

std::shared_ptr<HalfEdgeMesh> SubdivisionMesh::getLevel(int level) const {
    if (level >= 0 && level < static_cast<int>(m_levels.size())) {
        return m_levels[level].mesh;
    }
    return nullptr;
}

std::shared_ptr<Mesh> SubdivisionMesh::generateRenderMesh(int level) const {
    int targetLevel = (level == -1) ? getNumLevels() - 1 : level;
    auto halfEdgeMesh = getLevel(targetLevel);
    
    if (!halfEdgeMesh) {
        return nullptr;
    }
    
    // Convert half-edge mesh to face-vertex mesh
    // This is a placeholder - should use GeometryConverter
    auto mesh = std::make_shared<Mesh>();
    // TODO: Implement conversion using GeometryConverter::toFaceVertex
    return mesh;
}

std::shared_ptr<HalfEdgeMesh> SubdivisionMesh::applyCatmullClark(std::shared_ptr<HalfEdgeMesh> mesh) {
    // Catmull-Clark subdivision algorithm
    // This is a simplified implementation stub
    std::unordered_map<HalfEdgeFacePtr, QVector3D> facePoints;
    std::unordered_map<HalfEdgeEdgePtr, QVector3D> edgePoints;
    std::unordered_map<HalfEdgeVertexPtr, QVector3D> vertexPoints;
    
    computeFacePoints(mesh, facePoints);
    computeEdgePoints(mesh, facePoints, edgePoints);
    computeVertexPoints(mesh, facePoints, edgePoints, vertexPoints);
    
    // Create new subdivided mesh (placeholder)
    auto subdividedMesh = std::make_shared<HalfEdgeMesh>();
    // TODO: Implement actual subdivision mesh construction
    
    return subdividedMesh;
}

void SubdivisionMesh::computeFacePoints(std::shared_ptr<HalfEdgeMesh> mesh, 
                                       std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints) {
    // Compute face centroid for each face
    for (auto face : mesh->getFaces()) {
        QVector3D centroid(0, 0, 0);
        int vertexCount = 0;
        
        // Walk around face to get all vertices
        auto startEdge = face->getOuterEdge();
        auto currentEdge = startEdge;
        
        do {
            centroid += currentEdge->getOriginVertex()->getPosition();
            vertexCount++;
            currentEdge = currentEdge->getNext();
        } while (currentEdge && currentEdge != startEdge);
        
        if (vertexCount > 0) {
            centroid /= vertexCount;
        }
        
        facePoints[face] = centroid;
    }
}

void SubdivisionMesh::computeEdgePoints(std::shared_ptr<HalfEdgeMesh> mesh,
                                       const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                                       std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints) {
    // For each edge, compute the average of the two face points and two edge vertices
    for (auto edge : mesh->getEdges()) {
        QVector3D edgePoint(0, 0, 0);
        
        QVector3D v1 = edge->getOriginVertex()->getPosition();
        QVector3D v2 = edge->getTwin() ? edge->getTwin()->getOriginVertex()->getPosition() : v1;
        
        edgePoint = (v1 + v2) * 0.5f;
        
        // If edge has adjacent faces, include their face points
        if (edge->getFace() && edge->getTwin() && edge->getTwin()->getFace()) {
            auto it1 = facePoints.find(edge->getFace());
            auto it2 = facePoints.find(edge->getTwin()->getFace());
            
            if (it1 != facePoints.end() && it2 != facePoints.end()) {
                edgePoint = (v1 + v2 + it1->second + it2->second) * 0.25f;
            }
        }
        
        edgePoints[edge] = edgePoint;
    }
}

void SubdivisionMesh::computeVertexPoints(std::shared_ptr<HalfEdgeMesh> mesh,
                                         const std::unordered_map<HalfEdgeFacePtr, QVector3D>& facePoints,
                                         const std::unordered_map<HalfEdgeEdgePtr, QVector3D>& edgePoints,
                                         std::unordered_map<HalfEdgeVertexPtr, QVector3D>& vertexPoints) {
    // Catmull-Clark vertex update formula
    for (auto vertex : mesh->getVertices()) {
        QVector3D originalPos = vertex->getPosition();
        QVector3D avgFacePoint(0, 0, 0);
        QVector3D avgEdgePoint(0, 0, 0);
        
        int faceCount = 0;
        int edgeCount = 0;
        
        // Get all incident faces and edges
        auto outgoingEdges = vertex->getOutgoingEdges();
        
        for (auto edge : outgoingEdges) {
            if (edge->getFace()) {
                auto it = facePoints.find(edge->getFace());
                if (it != facePoints.end()) {
                    avgFacePoint += it->second;
                    faceCount++;
                }
            }
            
            auto it = edgePoints.find(edge);
            if (it != edgePoints.end()) {
                avgEdgePoint += it->second;
                edgeCount++;
            }
        }
        
        if (faceCount > 0) avgFacePoint /= faceCount;
        if (edgeCount > 0) avgEdgePoint /= edgeCount;
        
        // Catmull-Clark formula: (F + 2R + (n-3)P) / n
        // where F = avg face point, R = avg edge point, P = original vertex, n = valence
        int n = static_cast<int>(outgoingEdges.size());
        if (n > 0) {
            QVector3D newPos = (avgFacePoint + avgEdgePoint * 2.0f + originalPos * (n - 3)) / n;
            vertexPoints[vertex] = newPos;
        } else {
            vertexPoints[vertex] = originalPos;
        }
    }
}

// ===============================
// Voxel Grid Implementation
// ===============================

VoxelGrid::VoxelGrid(const QVector3D& origin, const QVector3D& size, int resolution)
    : m_origin(origin), m_size(size)
    , m_resX(resolution), m_resY(resolution), m_resZ(resolution)
    , m_voxelSize(std::max({size.x(), size.y(), size.z()}) / resolution)
{
    m_voxels.resize(m_resX * m_resY * m_resZ);
}

VoxelGrid::VoxelGrid(const QVector3D& origin, float voxelSize, int resX, int resY, int resZ)
    : m_origin(origin), m_size(resX * voxelSize, resY * voxelSize, resZ * voxelSize)
    , m_resX(resX), m_resY(resY), m_resZ(resZ), m_voxelSize(voxelSize)
{
    m_voxels.resize(m_resX * m_resY * m_resZ);
}

VoxelGrid::Voxel& VoxelGrid::getVoxel(int x, int y, int z) {
    static Voxel defaultVoxel;
    if (isValidIndex(x, y, z)) {
        return m_voxels[getIndex(x, y, z)];
    }
    return defaultVoxel;
}

const VoxelGrid::Voxel& VoxelGrid::getVoxel(int x, int y, int z) const {
    static Voxel defaultVoxel;
    if (isValidIndex(x, y, z)) {
        return m_voxels[getIndex(x, y, z)];
    }
    return defaultVoxel;
}

void VoxelGrid::setVoxel(int x, int y, int z, const Voxel& voxel) {
    if (isValidIndex(x, y, z)) {
        m_voxels[getIndex(x, y, z)] = voxel;
    }
}

QVector3D VoxelGrid::indexToWorld(int x, int y, int z) const {
    return m_origin + QVector3D(x * m_voxelSize, y * m_voxelSize, z * m_voxelSize);
}

QVector3D VoxelGrid::worldToIndex(const QVector3D& worldPos) const {
    QVector3D relative = worldPos - m_origin;
    return QVector3D(relative.x() / m_voxelSize, relative.y() / m_voxelSize, relative.z() / m_voxelSize);
}

bool VoxelGrid::isValidIndex(int x, int y, int z) const {
    return x >= 0 && x < m_resX && y >= 0 && y < m_resY && z >= 0 && z < m_resZ;
}

std::shared_ptr<Mesh> VoxelGrid::extractSurface(float isoValue) const {
    auto mesh = std::make_shared<Mesh>();
    std::vector<MCVertex> vertices;
    std::vector<unsigned int> indices;
    
    marchingCubes(vertices, indices, isoValue);
    
    // Convert to Mesh format
    std::vector<Vertex> meshVertices;
    for (const auto& mcVertex : vertices) {
        meshVertices.emplace_back(mcVertex.position, mcVertex.normal);
    }
    
    mesh->setVertices(meshVertices);
    mesh->setIndices(indices);
    return mesh;
}

void VoxelGrid::marchingCubes(std::vector<MCVertex>& vertices, std::vector<unsigned int>& indices, float isoValue) const {
    // Simplified marching cubes implementation
    // This is a placeholder - full implementation would be quite extensive
    
    for (int z = 0; z < m_resZ - 1; ++z) {
        for (int y = 0; y < m_resY - 1; ++y) {
            for (int x = 0; x < m_resX - 1; ++x) {
                // Get 8 corner values of the cube
                float values[8] = {
                    getVoxel(x, y, z).density,
                    getVoxel(x + 1, y, z).density,
                    getVoxel(x + 1, y + 1, z).density,
                    getVoxel(x, y + 1, z).density,
                    getVoxel(x, y, z + 1).density,
                    getVoxel(x + 1, y, z + 1).density,
                    getVoxel(x + 1, y + 1, z + 1).density,
                    getVoxel(x, y + 1, z + 1).density
                };
                
                // Determine cube configuration
                int cubeIndex = 0;
                for (int i = 0; i < 8; ++i) {
                    if (values[i] > isoValue) {
                        cubeIndex |= (1 << i);
                    }
                }
                
                // Skip if cube is entirely inside or outside
                if (cubeIndex == 0 || cubeIndex == 255) {
                    continue;
                }
                
                // TODO: Implement full marching cubes lookup table and triangulation
                // For now, just create a simple triangle in the center
                // Note: cubeIndex would be used in full implementation for lookup table
                Q_UNUSED(cubeIndex);
                
                QVector3D center = indexToWorld(x, y, z) + QVector3D(m_voxelSize * 0.5f, m_voxelSize * 0.5f, m_voxelSize * 0.5f);
                
                MCVertex vertex;
                vertex.position = center;
                vertex.normal = QVector3D(0, 1, 0); // TODO: Compute proper normal
                
                vertices.push_back(vertex);
            }
        }
    }
}

void VoxelGrid::fromMesh(std::shared_ptr<Mesh> mesh, float bandwidth) {
    if (!mesh) return;
    
    // Compute bounding box of mesh
    QVector3D minBounds = mesh->getBoundingBoxMin();
    QVector3D maxBounds = mesh->getBoundingBoxMax();
    
    // Expand bounds by bandwidth
    QVector3D padding(bandwidth * m_voxelSize, bandwidth * m_voxelSize, bandwidth * m_voxelSize);
    m_origin = minBounds - padding;
    m_size = maxBounds - minBounds + padding * 2.0f;
    
    // Recalculate grid dimensions
    m_resX = static_cast<int>(std::ceil(m_size.x() / m_voxelSize));
    m_resY = static_cast<int>(std::ceil(m_size.y() / m_voxelSize));
    m_resZ = static_cast<int>(std::ceil(m_size.z() / m_voxelSize));
    
    // Resize voxel array
    m_voxels.clear();
    m_voxels.resize(m_resX * m_resY * m_resZ);
    
    // Voxelize mesh triangles
    const auto& vertices = mesh->getVertices();
    const auto& indices = mesh->getIndices();
    
    for (size_t i = 0; i < indices.size(); i += 3) {
        QVector3D v0 = vertices[indices[i]].position;
        QVector3D v1 = vertices[indices[i + 1]].position;
        QVector3D v2 = vertices[indices[i + 2]].position;
        
        // Simple voxelization - just mark voxels containing triangle vertices
        voxelizeTriangle(v0, v1, v2, bandwidth);
    }
}

void VoxelGrid::voxelizeTriangle(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, float bandwidth) {
    // Currently bandwidth is not used in this simplified implementation
    Q_UNUSED(bandwidth);
    
    // Find bounding box of triangle
    QVector3D minBounds = QVector3D(
        std::min({v0.x(), v1.x(), v2.x()}),
        std::min({v0.y(), v1.y(), v2.y()}),
        std::min({v0.z(), v1.z(), v2.z()})
    );
    QVector3D maxBounds = QVector3D(
        std::max({v0.x(), v1.x(), v2.x()}),
        std::max({v0.y(), v1.y(), v2.y()}),
        std::max({v0.z(), v1.z(), v2.z()})
    );
    
    // Convert to grid coordinates
    QVector3D minGrid = worldToIndex(minBounds);
    QVector3D maxGrid = worldToIndex(maxBounds);
    
    // Iterate through voxels in bounding box
    int minX = std::max(0, static_cast<int>(std::floor(minGrid.x())));
    int maxX = std::min(m_resX - 1, static_cast<int>(std::ceil(maxGrid.x())));
    int minY = std::max(0, static_cast<int>(std::floor(minGrid.y())));
    int maxY = std::min(m_resY - 1, static_cast<int>(std::ceil(maxGrid.y())));
    int minZ = std::max(0, static_cast<int>(std::floor(minGrid.z())));
    int maxZ = std::min(m_resZ - 1, static_cast<int>(std::ceil(maxGrid.z())));
    
    for (int z = minZ; z <= maxZ; ++z) {
        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                QVector3D voxelCenter = indexToWorld(x, y, z) + QVector3D(m_voxelSize * 0.5f, m_voxelSize * 0.5f, m_voxelSize * 0.5f);
                
                // Compute signed distance to triangle (simplified)
                float distance = computeDistanceToTriangle(voxelCenter, v0, v1, v2);
                
                setVoxel(x, y, z, {distance, QVector3D(0, 1, 0)});
            }
        }
    }
}

float VoxelGrid::computeDistanceToTriangle(const QVector3D& point, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2) {
    // Simplified distance computation - just use distance to closest vertex
    float d0 = (point - v0).length();
    float d1 = (point - v1).length();
    float d2 = (point - v2).length();
    return std::min({d0, d1, d2});
}

void VoxelGrid::unionWith(const VoxelGrid& other) {
    // Simple union - take minimum signed distance
    if (m_resX != other.m_resX || m_resY != other.m_resY || m_resZ != other.m_resZ) {
        return; // Grids must have same resolution
    }
    
    for (int i = 0; i < static_cast<int>(m_voxels.size()); ++i) {
        m_voxels[i].density = std::min(m_voxels[i].density, other.m_voxels[i].density);
    }
}

void VoxelGrid::subtractFrom(const VoxelGrid& other) {
    // Subtraction: max(-a, b)
    if (m_resX != other.m_resX || m_resY != other.m_resY || m_resZ != other.m_resZ) {
        return; // Grids must have same resolution
    }
    
    for (int i = 0; i < static_cast<int>(m_voxels.size()); ++i) {
        m_voxels[i].density = std::max(-m_voxels[i].density, other.m_voxels[i].density);
    }
}

void VoxelGrid::intersectWith(const VoxelGrid& other) {
    // Intersection - take maximum signed distance
    if (m_resX != other.m_resX || m_resY != other.m_resY || m_resZ != other.m_resZ) {
        return; // Grids must have same resolution
    }
    
    for (int i = 0; i < static_cast<int>(m_voxels.size()); ++i) {
        m_voxels[i].density = std::max(m_voxels[i].density, other.m_voxels[i].density);
    }
}

// ===============================
// Point Cloud Implementation
// ===============================

void PointCloud::addPoint(const QVector3D& position, const QVector3D& normal) {
    m_points.emplace_back(position, normal);
    m_kdTreeValid = false;
}

void PointCloud::computeNormals(int kNeighbors) {
    if (!m_kdTreeValid) {
        buildKDTree();
    }
    
    for (auto& point : m_points) {
        auto neighbors = findKNearestNeighbors(point.position, kNeighbors);
        
        if (neighbors.size() >= 3) {
            // Compute covariance matrix and find smallest eigenvector
            // This is a simplified implementation
            QVector3D centroid(0, 0, 0);
            for (size_t idx : neighbors) {
                centroid += m_points[idx].position;
            }
            centroid /= neighbors.size();
            
            // For now, use simplified normal estimation
            // TODO: Implement proper PCA-based normal estimation
            point.normal = QVector3D(0, 1, 0);
        }
    }
}

void PointCloud::computeBoundingBox(QVector3D& minBounds, QVector3D& maxBounds) const {
    if (m_points.empty()) {
        minBounds = maxBounds = QVector3D(0, 0, 0);
        return;
    }
    
    minBounds = maxBounds = m_points[0].position;
    
    for (const auto& point : m_points) {
        minBounds.setX(std::min(minBounds.x(), point.position.x()));
        minBounds.setY(std::min(minBounds.y(), point.position.y()));
        minBounds.setZ(std::min(minBounds.z(), point.position.z()));
        
        maxBounds.setX(std::max(maxBounds.x(), point.position.x()));
        maxBounds.setY(std::max(maxBounds.y(), point.position.y()));
        maxBounds.setZ(std::max(maxBounds.z(), point.position.z()));
    }
}

std::vector<size_t> PointCloud::findKNearestNeighbors(const QVector3D& queryPoint, int k) const {
    if (!m_kdTreeValid) {
        buildKDTree();
    }
    
    std::vector<std::pair<float, size_t>> results;
    results.reserve(k);
    
    if (m_kdTree) {
        knnSearch(m_kdTree.get(), queryPoint, k, results);
    }
    
    std::vector<size_t> indices;
    indices.reserve(results.size());
    for (const auto& result : results) {
        indices.push_back(result.second);
    }
    
    return indices;
}

void PointCloud::buildKDTree() const {
    if (m_points.empty()) {
        m_kdTree.reset();
        m_kdTreeValid = true;
        return;
    }
    
    std::vector<size_t> indices(m_points.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    m_kdTree = buildKDTreeRecursive(indices, 0);
    m_kdTreeValid = true;
}

std::unique_ptr<PointCloud::KDNode> PointCloud::buildKDTreeRecursive(std::vector<size_t>& indices, int depth) const {
    if (indices.empty()) {
        return nullptr;
    }
    
    auto node = std::make_unique<KDNode>();
    node->axis = depth % 3;
    
    if (indices.size() == 1) {
        node->pointIndex = indices[0];
        return node;
    }
    
    // Sort points along the current axis
    std::sort(indices.begin(), indices.end(), [this, axis = node->axis](size_t a, size_t b) {
        const QVector3D& posA = m_points[a].position;
        const QVector3D& posB = m_points[b].position;
        
        switch (axis) {
            case 0: return posA.x() < posB.x();
            case 1: return posA.y() < posB.y();
            case 2: return posA.z() < posB.z();
            default: return false;
        }
    });
    
    size_t median = indices.size() / 2;
    node->pointIndex = indices[median];
    
    std::vector<size_t> leftIndices(indices.begin(), indices.begin() + median);
    std::vector<size_t> rightIndices(indices.begin() + median + 1, indices.end());
    
    node->left = buildKDTreeRecursive(leftIndices, depth + 1);
    node->right = buildKDTreeRecursive(rightIndices, depth + 1);
    
    return node;
}

void PointCloud::knnSearch(const KDNode* node, const QVector3D& query, int k,
                          std::vector<std::pair<float, size_t>>& results) const {
    if (!node) return;
    
    const QVector3D& nodePos = m_points[node->pointIndex].position;
    float distance = (nodePos - query).lengthSquared();
    
    // Add this point to results if we have room or it's better than the worst
    if (results.size() < k) {
        results.emplace_back(distance, node->pointIndex);
        std::push_heap(results.begin(), results.end());
    } else if (distance < results.front().first) {
        std::pop_heap(results.begin(), results.end());
        results.back() = {distance, node->pointIndex};
        std::push_heap(results.begin(), results.end());
    }
    
    // Recursively search children
    float axisDiff = 0.0f;
    switch (node->axis) {
        case 0: axisDiff = query.x() - nodePos.x(); break;
        case 1: axisDiff = query.y() - nodePos.y(); break;
        case 2: axisDiff = query.z() - nodePos.z(); break;
    }
    
    KDNode* nearChild = (axisDiff < 0) ? node->left.get() : node->right.get();
    KDNode* farChild = (axisDiff < 0) ? node->right.get() : node->left.get();
    
    knnSearch(nearChild, query, k, results);
    
    // Check if we need to search the far side
    if (results.size() < k || axisDiff * axisDiff < results.front().first) {
        knnSearch(farChild, query, k, results);
    }
}

std::shared_ptr<Mesh> PointCloud::poissonReconstruction(int depth, float samplesPerNode) const {
    // This is a placeholder implementation
    // A real Poisson reconstruction would require a complex algorithm
    // For now, return a simple triangulated mesh from the point cloud
    
    auto mesh = std::make_shared<Mesh>();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Convert points to vertices
    for (const auto& point : m_points) {
        vertices.emplace_back(point.position, point.normal);
    }
    
    // Create simple triangulation (this is a very basic implementation)
    // In practice, this would use the Poisson surface reconstruction algorithm
    if (vertices.size() >= 3) {
        for (size_t i = 0; i < vertices.size() - 2; i += 3) {
            indices.push_back(static_cast<unsigned int>(i));
            indices.push_back(static_cast<unsigned int>(i + 1));
            indices.push_back(static_cast<unsigned int>(i + 2));
        }
    }
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    return mesh;
}

std::shared_ptr<Mesh> PointCloud::ballPivoting(float ballRadius) const {
    // Placeholder implementation for ball pivoting algorithm
    // This would normally implement the Ball-Pivoting surface reconstruction
    return poissonReconstruction(8, 1.5f); // Fall back to Poisson for now
}

// ===============================
// Implicit Surface Implementation
// ===============================

ImplicitSurface::ImplicitSurface(SDFFunction sdf, GradientFunction gradient)
    : m_sdf(sdf), m_gradient(gradient) {}

float ImplicitSurface::evaluate(const QVector3D& point) const {
    return m_sdf(point);
}

QVector3D ImplicitSurface::evaluateGradient(const QVector3D& point) const {
    if (m_gradient) {
        return m_gradient(point);
    }
    return computeNumericalGradient(point);
}

QVector3D ImplicitSurface::evaluateNormal(const QVector3D& point) const {
    return evaluateGradient(point).normalized();
}

QVector3D ImplicitSurface::computeNumericalGradient(const QVector3D& point, float epsilon) const {
    QVector3D gradient;
    
    gradient.setX((evaluate(point + QVector3D(epsilon, 0, 0)) - evaluate(point - QVector3D(epsilon, 0, 0))) / (2.0f * epsilon));
    gradient.setY((evaluate(point + QVector3D(0, epsilon, 0)) - evaluate(point - QVector3D(0, epsilon, 0))) / (2.0f * epsilon));
    gradient.setZ((evaluate(point + QVector3D(0, 0, epsilon)) - evaluate(point - QVector3D(0, 0, epsilon))) / (2.0f * epsilon));
    
    return gradient;
}

ImplicitSurface ImplicitSurface::sphere(const QVector3D& center, float radius) {
    return ImplicitSurface([center, radius](const QVector3D& p) {
        return (p - center).length() - radius;
    });
}

ImplicitSurface ImplicitSurface::box(const QVector3D& center, const QVector3D& size) {
    return ImplicitSurface([center, size](const QVector3D& p) {
        QVector3D d = QVector3D(std::abs(p.x() - center.x()), std::abs(p.y() - center.y()), std::abs(p.z() - center.z())) - size * 0.5f;
        return std::max({d.x(), d.y(), d.z(), 0.0f}) + std::min(std::max({d.x(), d.y(), d.z()}), 0.0f);
    });
}

std::shared_ptr<Mesh> ImplicitSurface::extractSurface(const QVector3D& bounds, float resolution, float isoValue) const {
    // Create a voxel grid and extract surface using marching cubes
    auto voxelGrid = toVoxelGrid(bounds, resolution);
    return voxelGrid->extractSurface(isoValue);
}

std::shared_ptr<VoxelGrid> ImplicitSurface::toVoxelGrid(const QVector3D& bounds, float resolution) const {
    // Create voxel grid from implicit surface
    int resX = static_cast<int>(std::ceil(bounds.x() / resolution));
    int resY = static_cast<int>(std::ceil(bounds.y() / resolution));
    int resZ = static_cast<int>(std::ceil(bounds.z() / resolution));
    
    auto voxelGrid = std::make_shared<VoxelGrid>(QVector3D(-bounds.x() * 0.5f, -bounds.y() * 0.5f, -bounds.z() * 0.5f), bounds, resolution);
    
    // Sample the implicit surface at each voxel
    for (int z = 0; z < resZ; ++z) {
        for (int y = 0; y < resY; ++y) {
            for (int x = 0; x < resX; ++x) {
                QVector3D worldPos = voxelGrid->indexToWorld(x, y, z);
                float value = evaluate(worldPos);
                QVector3D normal = evaluateNormal(worldPos);
                
                voxelGrid->setVoxel(x, y, z, {value, normal});
            }
        }
    }
    
    return voxelGrid;
}

// ===============================
// BVH Tree Implementation
// ===============================

void BVHTree::BoundingBox::expand(const QVector3D& point) {
    min.setX(std::min(min.x(), point.x()));
    min.setY(std::min(min.y(), point.y()));
    min.setZ(std::min(min.z(), point.z()));
    
    max.setX(std::max(max.x(), point.x()));
    max.setY(std::max(max.y(), point.y()));
    max.setZ(std::max(max.z(), point.z()));
}

void BVHTree::BoundingBox::expand(const BoundingBox& box) {
    expand(box.min);
    expand(box.max);
}

bool BVHTree::BoundingBox::intersects(const BoundingBox& other) const {
    return (min.x() <= other.max.x() && max.x() >= other.min.x()) &&
           (min.y() <= other.max.y() && max.y() >= other.min.y()) &&
           (min.z() <= other.max.z() && max.z() >= other.min.z());
}

bool BVHTree::BoundingBox::contains(const QVector3D& point) const {
    return point.x() >= min.x() && point.x() <= max.x() &&
           point.y() >= min.y() && point.y() <= max.y() &&
           point.z() >= min.z() && point.z() <= max.z();
}

float BVHTree::BoundingBox::getSurfaceArea() const {
    QVector3D size = max - min;
    return 2.0f * (size.x() * size.y() + size.y() * size.z() + size.z() * size.x());
}

BVHTree::BVHTree(std::shared_ptr<Mesh> mesh) : m_mesh(mesh) {
    buildTree();
}

void BVHTree::buildTree() {
    if (!m_mesh) return;
    
    const auto& indices = m_mesh->getIndices();
    size_t triangleCount = indices.size() / 3;
    
    // Compute bounding box for each triangle
    m_primitiveBounds.reserve(triangleCount);
    std::vector<size_t> primitives(triangleCount);
    
    for (size_t i = 0; i < triangleCount; ++i) {
        m_primitiveBounds.push_back(computePrimitiveBounds(i));
        primitives[i] = i;
    }
    
    m_root = buildRecursive(primitives, 0);
}

std::unique_ptr<BVHTree::BVHNode> BVHTree::buildRecursive(std::vector<size_t>& primitives, int depth) {
    auto node = std::make_unique<BVHNode>();
    
    // Compute bounding box for all primitives
    for (size_t primitiveIndex : primitives) {
        node->bounds.expand(m_primitiveBounds[primitiveIndex]);
    }
    
    // Leaf node criteria
    if (primitives.size() <= 4 || depth > 20) {
        node->primitiveIndices = primitives;
        return node;
    }
    
    // Choose split axis (longest axis)
    QVector3D size = node->bounds.getSize();
    int axis = 0;
    if (size.y() > size.x()) axis = 1;
    if (size.z() > ((axis == 0) ? size.x() : size.y())) axis = 2;
    
    // Sort primitives along chosen axis
    std::sort(primitives.begin(), primitives.end(), [this, axis](size_t a, size_t b) {
        QVector3D centerA = m_primitiveBounds[a].getCenter();
        QVector3D centerB = m_primitiveBounds[b].getCenter();
        
        switch (axis) {
            case 0: return centerA.x() < centerB.x();
            case 1: return centerA.y() < centerB.y();
            case 2: return centerA.z() < centerB.z();
            default: return false;
        }
    });
    
    // Split in the middle
    size_t mid = primitives.size() / 2;
    std::vector<size_t> leftPrimitives(primitives.begin(), primitives.begin() + mid);
    std::vector<size_t> rightPrimitives(primitives.begin() + mid, primitives.end());
    
    node->left = buildRecursive(leftPrimitives, depth + 1);
    node->right = buildRecursive(rightPrimitives, depth + 1);
    
    return node;
}

BVHTree::BoundingBox BVHTree::computePrimitiveBounds(size_t primitiveIndex) const {
    const auto& vertices = m_mesh->getVertices();
    const auto& indices = m_mesh->getIndices();
    
    size_t baseIndex = primitiveIndex * 3;
    BoundingBox bounds;
    
    for (int i = 0; i < 3; ++i) {
        const QVector3D& pos = vertices[indices[baseIndex + i]].position;
        bounds.expand(pos);
    }
    
    return bounds;
}

BVHTree::RayHit BVHTree::raycast(const Ray& ray) const {
    RayHit hit;
    
    if (!m_root) {
        return hit;
    }
    
    // TODO: Implement BVH ray traversal
    // This is a placeholder implementation
    
    return hit;
}
