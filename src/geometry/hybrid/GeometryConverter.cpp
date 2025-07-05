#include "GeometryConverter.h"
#include "core/mesh_elements.hpp"
#include "core/half_edge_mesh.hpp"
#include <QDebug>
#include <unordered_map>
#include <algorithm>

HalfEdgeMeshPtr GeometryConverter::toHalfEdge(MeshPtr faceVertexMesh) {
    if (!faceVertexMesh || faceVertexMesh->isEmpty()) {
        return nullptr;
    }

    auto halfEdgeMesh = std::make_shared<rude::HalfEdgeMesh>();
    const auto& vertices = faceVertexMesh->getVertices();
    const auto& indices = faceVertexMesh->getIndices();

    if (indices.size() % 3 != 0) {
        qWarning() << "Face-vertex mesh must be triangulated for conversion";
        return nullptr;
    }

    // Create vertices
    std::vector<HalfEdgeVertexPtr> halfEdgeVertices;
    halfEdgeVertices.reserve(vertices.size());
    
    for (const auto& vertex : vertices) {
        auto halfEdgeVertex = halfEdgeMesh->addVertex(qtToGlm(vertex.position));
        halfEdgeVertex->normal = qtToGlm(vertex.normal);
        halfEdgeVertex->texCoord = qtToGlm(vertex.texCoord);
        halfEdgeVertices.push_back(halfEdgeVertex);
    }

    // Create faces (triangles)
    for (size_t i = 0; i < indices.size(); i += 3) {
        std::vector<HalfEdgeVertexPtr> faceVertices = {
            halfEdgeVertices[indices[i]],
            halfEdgeVertices[indices[i + 1]],
            halfEdgeVertices[indices[i + 2]]
        };
        
        halfEdgeMesh->addFace(faceVertices);
    }

    // Update connectivity and normals
    buildFaceVertexConnectivity(halfEdgeMesh);
    halfEdgeMesh->updateNormals();

    return halfEdgeMesh;
}

MeshPtr GeometryConverter::toFaceVertex(HalfEdgeMeshPtr halfEdgeMesh) {
    if (!halfEdgeMesh || halfEdgeMesh->isEmpty()) {
        return nullptr;
    }

    auto faceVertexMesh = std::make_shared<Mesh>();
    
    std::vector<QVector3D> positions;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> texCoords;
    std::vector<unsigned int> indices;
    
    toVerticesAndFaces(halfEdgeMesh, positions, normals, texCoords, indices);
    
    // Convert to Vertex format
    std::vector<Vertex> vertices;
    vertices.reserve(positions.size());
    
    for (size_t i = 0; i < positions.size(); ++i) {
        Vertex vertex;
        vertex.position = positions[i];
        vertex.normal = (i < normals.size()) ? normals[i] : QVector3D(0, 1, 0);
        vertex.texCoord = (i < texCoords.size()) ? texCoords[i] : QVector2D(0, 0);
        vertices.push_back(vertex);
    }
    
    faceVertexMesh->setVertices(vertices);
    faceVertexMesh->setIndices(indices);
    
    return faceVertexMesh;
}

HalfEdgeMeshPtr GeometryConverter::fromVerticesAndFaces(
    const std::vector<QVector3D>& positions,
    const std::vector<std::vector<unsigned int>>& faces) {
    
    auto halfEdgeMesh = std::make_shared<HalfEdgeMesh>();
    
    // Create vertices
    std::vector<HalfEdgeVertexPtr> vertices;
    vertices.reserve(positions.size());
    
    for (const auto& position : positions) {
        vertices.push_back(halfEdgeMesh->addVertex(position));
    }
    
    // Create faces
    for (const auto& face : faces) {
        if (face.size() < 3) {
            qWarning() << "Skipping face with less than 3 vertices";
            continue;
        }
        
        std::vector<HalfEdgeVertexPtr> faceVertices;
        faceVertices.reserve(face.size());
        
        for (unsigned int vertexIndex : face) {
            if (vertexIndex < vertices.size()) {
                faceVertices.push_back(vertices[vertexIndex]);
            } else {
                qWarning() << "Invalid vertex index:" << vertexIndex;
                break;
            }
        }
        
        if (faceVertices.size() == face.size()) {
            halfEdgeMesh->addFace(faceVertices);
        }
    }
    
    halfEdgeMesh->updateNormals();
    return halfEdgeMesh;
}

void GeometryConverter::toVerticesAndFaces(
    HalfEdgeMeshPtr halfEdgeMesh,
    std::vector<QVector3D>& positions,
    std::vector<QVector3D>& normals,
    std::vector<QVector2D>& texCoords,
    std::vector<unsigned int>& indices) {
    
    if (!halfEdgeMesh) {
        return;
    }
    
    const auto& vertices = halfEdgeMesh->getVertices();
    const auto& faces = halfEdgeMesh->getFaces();
    
    // Create vertex index mapping
    std::unordered_map<HalfEdgeVertexPtr, unsigned int> vertexIndexMap;
    
    positions.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();
    
    positions.reserve(vertices.size());
    normals.reserve(vertices.size());
    texCoords.reserve(vertices.size());
    
    // Extract vertex data
    for (size_t i = 0; i < vertices.size(); ++i) {
        const auto& vertex = vertices[i];
        vertexIndexMap[vertex] = static_cast<unsigned int>(i);
        
        positions.push_back(vertex->getPosition());
        normals.push_back(vertex->getNormal());
        texCoords.push_back(vertex->getTexCoord());
    }
    
    // Extract face indices
    indices.reserve(faces.size() * 3); // Assume triangulation for now
    
    for (const auto& face : faces) {
        auto faceVertices = face->getVertices();
        
        if (faceVertices.size() == 3) {
            // Triangle - add directly
            for (const auto& vertex : faceVertices) {
                auto it = vertexIndexMap.find(vertex);
                if (it != vertexIndexMap.end()) {
                    indices.push_back(it->second);
                }
            }
        } else if (faceVertices.size() > 3) {
            // Polygon - triangulate using fan triangulation
            if (faceVertices.size() >= 3) {
                for (size_t i = 1; i < faceVertices.size() - 1; ++i) {
                    // Create triangle: vertex[0], vertex[i], vertex[i+1]
                    auto it0 = vertexIndexMap.find(faceVertices[0]);
                    auto it1 = vertexIndexMap.find(faceVertices[i]);
                    auto it2 = vertexIndexMap.find(faceVertices[i + 1]);
                    
                    if (it0 != vertexIndexMap.end() && 
                        it1 != vertexIndexMap.end() && 
                        it2 != vertexIndexMap.end()) {
                        indices.push_back(it0->second);
                        indices.push_back(it1->second);
                        indices.push_back(it2->second);
                    }
                }
            }
        }
    }
}

bool GeometryConverter::validateConversion(MeshPtr original, HalfEdgeMeshPtr converted) {
    if (!original || !converted) {
        return false;
    }
    
    // Check vertex count
    if (original->getVertexCount() != converted->getVertexCount()) {
        qWarning() << "Vertex count mismatch in conversion";
        return false;
    }
    
    // Check face count (approximate, since polygons may be triangulated)
    size_t originalTriangleCount = original->getTriangleCount();
    size_t convertedFaceCount = converted->getFaceCount();
    
    if (originalTriangleCount != convertedFaceCount) {
        qWarning() << "Face count mismatch: original" << originalTriangleCount 
                   << "converted" << convertedFaceCount;
        // This might be acceptable for polygon meshes
    }
    
    // Validate mesh integrity
    return converted->validate();
}

bool GeometryConverter::validateConversion(HalfEdgeMeshPtr original, MeshPtr converted) {
    if (!original || !converted) {
        return false;
    }
    
    // Check vertex count
    if (original->getVertexCount() != converted->getVertexCount()) {
        qWarning() << "Vertex count mismatch in conversion";
        return false;
    }
    
    // Check that converted mesh is valid
    if (converted->isEmpty()) {
        qWarning() << "Converted mesh is empty";
        return false;
    }
    
    return true;
}

void GeometryConverter::copyVertexData(HalfEdgeVertexPtr src, Vertex& dst) {
    if (!src) return;
    
    dst.position = src->getPosition();
    dst.normal = src->getNormal();
    dst.texCoord = src->getTexCoord();
}

void GeometryConverter::copyVertexData(const Vertex& src, HalfEdgeVertexPtr dst) {
    if (!dst) return;
    
    dst->setPosition(src.position);
    dst->setNormal(src.normal);
    dst->setTexCoord(src.texCoord);
}

void GeometryConverter::buildFaceVertexConnectivity(HalfEdgeMeshPtr mesh) {
    if (!mesh) return;
    
    // Update target vertices for edges
    for (const auto& edge : mesh->getEdges()) {
        if (auto next = edge->getNext()) {
            edge->setTargetVertex(next->getOriginVertex());
        }
    }
    
    // Ensure all vertices have outgoing edges
    for (const auto& vertex : mesh->getVertices()) {
        if (!vertex->getOutgoingEdge()) {
            // Find an edge originating from this vertex
            for (const auto& edge : mesh->getEdges()) {
                if (edge->getOriginVertex() == vertex) {
                    vertex->setOutgoingEdge(edge);
                    break;
                }
            }
        }
    }
}

void GeometryConverter::optimizeVertexOrder(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    // Simple optimization: reorder vertices to improve cache locality
    // This is a basic implementation - more sophisticated algorithms exist
    
    std::vector<bool> used(vertices.size(), false);
    std::vector<Vertex> optimizedVertices;
    std::vector<unsigned int> oldToNewIndex(vertices.size());
    
    optimizedVertices.reserve(vertices.size());
    
    // Process vertices in the order they appear in indices
    for (unsigned int index : indices) {
        if (index < vertices.size() && !used[index]) {
            oldToNewIndex[index] = static_cast<unsigned int>(optimizedVertices.size());
            optimizedVertices.push_back(vertices[index]);
            used[index] = true;
        }
    }
    
    // Add any remaining vertices
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (!used[i]) {
            oldToNewIndex[i] = static_cast<unsigned int>(optimizedVertices.size());
            optimizedVertices.push_back(vertices[i]);
        }
    }
    
    // Update indices
    for (unsigned int& index : indices) {
        if (index < oldToNewIndex.size()) {
            index = oldToNewIndex[index];
        }
    }
    
    vertices = std::move(optimizedVertices);
}

// ===============================
// Additional Conversion Methods
// ===============================

// NURBS conversions (stubs)
HalfEdgeMeshPtr GeometryConverter::toHalfEdge(NURBSSurfacePtr nurbs, int resolutionU, int resolutionV) {
    if (!nurbs) return nullptr;
    auto mesh = nurbs->tessellate(resolutionU, resolutionV);
    return toHalfEdge(mesh);
}

MeshPtr GeometryConverter::toFaceVertex(NURBSSurfacePtr nurbs, int resolutionU, int resolutionV) {
    if (!nurbs) return nullptr;
    return nurbs->tessellate(resolutionU, resolutionV);
}

NURBSSurfacePtr GeometryConverter::toNURBS(HalfEdgeMeshPtr halfEdge, int degreeU, int degreeV) {
    // Stub implementation - would need NURBS fitting algorithm
    if (!halfEdge) return nullptr;
    
    // Parameters will be used in full implementation
    Q_UNUSED(degreeU);
    Q_UNUSED(degreeV);
    
    auto nurbs = std::make_shared<NURBSSurface>(3, 3, 5, 5); // Use default degrees for now
    // TODO: Implement mesh to NURBS fitting
    return nurbs;
}

// Subdivision surface conversions
SubdivisionMeshPtr GeometryConverter::toSubdivision(HalfEdgeMeshPtr baseMesh) {
    if (!baseMesh) return nullptr;
    return std::make_shared<SubdivisionMesh>(baseMesh);
}

SubdivisionMeshPtr GeometryConverter::toSubdivision(MeshPtr baseMesh) {
    auto halfEdge = toHalfEdge(baseMesh);
    return toSubdivision(halfEdge);
}

HalfEdgeMeshPtr GeometryConverter::toHalfEdge(SubdivisionMeshPtr subdivision, int level) {
    if (!subdivision) return nullptr;
    return subdivision->getLevel(level == -1 ? subdivision->getNumLevels() - 1 : level);
}

MeshPtr GeometryConverter::toFaceVertex(SubdivisionMeshPtr subdivision, int level) {
    if (!subdivision) return nullptr;
    return subdivision->generateRenderMesh(level);
}

// Voxel conversions (stubs)
VoxelGridPtr GeometryConverter::toVoxels(MeshPtr mesh, float voxelSize, float bandwidth) {
    if (!mesh) return nullptr;
    
    // Compute mesh bounds
    const auto& vertices = mesh->getVertices();
    if (vertices.empty()) return nullptr;
    
    QVector3D minBounds = vertices[0].position;
    QVector3D maxBounds = vertices[0].position;
    
    for (const auto& vertex : vertices) {
        minBounds.setX(std::min(minBounds.x(), vertex.position.x()));
        minBounds.setY(std::min(minBounds.y(), vertex.position.y()));
        minBounds.setZ(std::min(minBounds.z(), vertex.position.z()));
        
        maxBounds.setX(std::max(maxBounds.x(), vertex.position.x()));
        maxBounds.setY(std::max(maxBounds.y(), vertex.position.y()));
        maxBounds.setZ(std::max(maxBounds.z(), vertex.position.z()));
    }
    
    // Expand bounds by bandwidth
    QVector3D expansion(bandwidth * voxelSize, bandwidth * voxelSize, bandwidth * voxelSize);
    minBounds -= expansion;
    maxBounds += expansion;
    
    QVector3D size = maxBounds - minBounds;
    int resX = static_cast<int>(std::ceil(size.x() / voxelSize));
    int resY = static_cast<int>(std::ceil(size.y() / voxelSize));
    int resZ = static_cast<int>(std::ceil(size.z() / voxelSize));
    
    auto voxelGrid = std::make_shared<VoxelGrid>(minBounds, voxelSize, resX, resY, resZ);
    voxelGrid->fromMesh(mesh, bandwidth);
    
    return voxelGrid;
}

VoxelGridPtr GeometryConverter::toVoxels(HalfEdgeMeshPtr halfEdge, float voxelSize, float bandwidth) {
    auto mesh = toFaceVertex(halfEdge);
    return toVoxels(mesh, voxelSize, bandwidth);
}

MeshPtr GeometryConverter::toFaceVertex(VoxelGridPtr voxels, float isoValue) {
    if (!voxels) return nullptr;
    return voxels->extractSurface(isoValue);
}

HalfEdgeMeshPtr GeometryConverter::toHalfEdge(VoxelGridPtr voxels, float isoValue) {
    auto mesh = toFaceVertex(voxels, isoValue);
    return toHalfEdge(mesh);
}

// Point cloud conversions (stubs)
PointCloudPtr GeometryConverter::toPointCloud(MeshPtr mesh, int samplesPerFace) {
    if (!mesh) return nullptr;
    
    auto pointCloud = std::make_shared<PointCloud>();
    const auto& vertices = mesh->getVertices();
    const auto& indices = mesh->getIndices();
    
    // Simple sampling - just copy vertices for now
    for (const auto& vertex : vertices) {
        pointCloud->addPoint(vertex.position, vertex.normal);
    }
    
    // TODO: Implement proper face sampling
    return pointCloud;
}

PointCloudPtr GeometryConverter::toPointCloud(HalfEdgeMeshPtr halfEdge, int samplesPerFace) {
    auto mesh = toFaceVertex(halfEdge);
    return toPointCloud(mesh, samplesPerFace);
}

MeshPtr GeometryConverter::toFaceVertex(PointCloudPtr pointCloud, int reconstructionDepth) {
    if (!pointCloud) return nullptr;
    return pointCloud->poissonReconstruction(reconstructionDepth);
}

HalfEdgeMeshPtr GeometryConverter::toHalfEdge(PointCloudPtr pointCloud, int reconstructionDepth) {
    auto mesh = toFaceVertex(pointCloud, reconstructionDepth);
    return toHalfEdge(mesh);
}

// Implicit surface conversions (stubs)
ImplicitSurfacePtr GeometryConverter::toImplicitSurface(MeshPtr mesh, float bandwidth) {
    // Stub implementation
    return nullptr;
}

ImplicitSurfacePtr GeometryConverter::toImplicitSurface(VoxelGridPtr voxels) {
    // Stub implementation
    return nullptr;
}

MeshPtr GeometryConverter::toFaceVertex(ImplicitSurfacePtr implicit, const QVector3D& bounds, float resolution) {
    if (!implicit) return nullptr;
    return implicit->extractSurface(bounds, resolution);
}

VoxelGridPtr GeometryConverter::toVoxels(ImplicitSurfacePtr implicit, const QVector3D& bounds, float resolution) {
    if (!implicit) return nullptr;
    return implicit->toVoxelGrid(bounds, resolution);
}

// Optimization utilities
void GeometryConverter::optimizeMeshForRendering(MeshPtr mesh) {
    if (!mesh) return;
    // TODO: Implement mesh optimization (vertex cache optimization, etc.)
}

void GeometryConverter::optimizeHalfEdgeForEditing(HalfEdgeMeshPtr mesh) {
    if (!mesh) return;
    // TODO: Implement half-edge optimization
}

BVHTreePtr GeometryConverter::createBVH(MeshPtr mesh) {
    if (!mesh) return nullptr;
    return std::make_shared<BVHTree>(mesh);
}

// Topology validation
bool GeometryConverter::validateTopology(HalfEdgeMeshPtr mesh) {
    if (!mesh) return false;
    // TODO: Implement topology validation
    return true;
}

void GeometryConverter::repairMesh(HalfEdgeMeshPtr mesh) {
    if (!mesh) return;
    // TODO: Implement mesh repair algorithms
}

// MeshConverterCache Implementation
MeshConverterCache& MeshConverterCache::getInstance() {
    static MeshConverterCache instance;
    return instance;
}

HalfEdgeMeshPtr MeshConverterCache::getHalfEdge(MeshPtr mesh) {
    if (!mesh) return nullptr;
    
    cleanupExpiredEntries();
    
    auto it = m_faceVertexToHalfEdge.find(mesh);
    if (it != m_faceVertexToHalfEdge.end()) {
        return it->second;
    }
    
    // Convert and cache
    auto halfEdgeMesh = GeometryConverter::toHalfEdge(mesh);
    if (halfEdgeMesh) {
        m_faceVertexToHalfEdge[mesh] = halfEdgeMesh;
        m_halfEdgeToFaceVertex[halfEdgeMesh] = mesh;
        m_trackedFaceVertex.insert(mesh);
        m_trackedHalfEdge.insert(halfEdgeMesh);
    }
    
    return halfEdgeMesh;
}

MeshPtr MeshConverterCache::getFaceVertex(HalfEdgeMeshPtr mesh) {
    if (!mesh) return nullptr;
    
    cleanupExpiredEntries();
    
    auto it = m_halfEdgeToFaceVertex.find(mesh);
    if (it != m_halfEdgeToFaceVertex.end()) {
        return it->second;
    }
    
    // Convert and cache
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    if (faceVertexMesh) {
        m_halfEdgeToFaceVertex[mesh] = faceVertexMesh;
        m_faceVertexToHalfEdge[faceVertexMesh] = mesh;
        m_trackedHalfEdge.insert(mesh);
        m_trackedFaceVertex.insert(faceVertexMesh);
    }
    
    return faceVertexMesh;
}

void MeshConverterCache::invalidate(MeshPtr mesh) {
    if (!mesh) return;
    
    auto it = m_faceVertexToHalfEdge.find(mesh);
    if (it != m_faceVertexToHalfEdge.end()) {
        auto halfEdgeMesh = it->second;
        m_halfEdgeToFaceVertex.erase(halfEdgeMesh);
        m_faceVertexToHalfEdge.erase(it);
    }
}

void MeshConverterCache::invalidate(HalfEdgeMeshPtr mesh) {
    if (!mesh) return;
    
    auto it = m_halfEdgeToFaceVertex.find(mesh);
    if (it != m_halfEdgeToFaceVertex.end()) {
        auto faceVertexMesh = it->second;
        m_faceVertexToHalfEdge.erase(faceVertexMesh);
        m_halfEdgeToFaceVertex.erase(it);
    }
}

void MeshConverterCache::clear() {
    m_faceVertexToHalfEdge.clear();
    m_halfEdgeToFaceVertex.clear();
    m_trackedFaceVertex.clear();
    m_trackedHalfEdge.clear();
}

size_t MeshConverterCache::getCacheSize() const {
    return m_faceVertexToHalfEdge.size() + m_halfEdgeToFaceVertex.size();
}

void MeshConverterCache::cleanupExpiredEntries() {
    // Remove expired weak references and their corresponding cache entries
    auto fvIt = m_trackedFaceVertex.begin();
    while (fvIt != m_trackedFaceVertex.end()) {
        if (fvIt->expired()) {
            fvIt = m_trackedFaceVertex.erase(fvIt);
        } else {
            ++fvIt;
        }
    }
    
    auto heIt = m_trackedHalfEdge.begin();
    while (heIt != m_trackedHalfEdge.end()) {
        if (heIt->expired()) {
            heIt = m_trackedHalfEdge.erase(heIt);
        } else {
            ++heIt;
        }
    }
    
    // Clean up cache entries for expired objects
    auto fvCacheIt = m_faceVertexToHalfEdge.begin();
    while (fvCacheIt != m_faceVertexToHalfEdge.end()) {
        if (fvCacheIt->first.use_count() <= 1) { // Only held by cache
            m_halfEdgeToFaceVertex.erase(fvCacheIt->second);
            fvCacheIt = m_faceVertexToHalfEdge.erase(fvCacheIt);
        } else {
            ++fvCacheIt;
        }
    }
    
    auto heCacheIt = m_halfEdgeToFaceVertex.begin();
    while (heCacheIt != m_halfEdgeToFaceVertex.end()) {
        if (heCacheIt->first.use_count() <= 1) { // Only held by cache
            m_faceVertexToHalfEdge.erase(heCacheIt->second);
            heCacheIt = m_halfEdgeToFaceVertex.erase(heCacheIt);
        } else {
            ++heCacheIt;
        }
    }
}
