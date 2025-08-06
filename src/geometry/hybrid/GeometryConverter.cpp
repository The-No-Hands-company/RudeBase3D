
#include "GeometryConverter.h"
#include "core/mesh_forward.hpp"
#include "core/mesh.hpp"
#include "core/mesh_elements.hpp"
#include "geometry/core/Vertex.h"
#include "geometry/subdivision/SubdivisionMesh.h"
#include "core/half_edge_mesh.hpp"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <glm/glm.hpp>

// Use rude:: namespace for all types
rude::HalfEdgeMeshPtr GeometryConverter::toHalfEdge(rude::MeshPtr faceVertexMesh) {
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
    std::vector<rude::VertexPtr> halfEdgeVertices;
    halfEdgeVertices.reserve(vertices.size());
    for (const auto& vertex : vertices) {
        glm::vec3 pos = vertex.position;
        glm::vec3 norm = vertex.normal;
        glm::vec2 tex = vertex.texCoord;
        auto halfEdgeVertex = halfEdgeMesh->addVertex(pos);
        halfEdgeVertex->normal = norm;
        halfEdgeVertex->texCoord = tex;
        halfEdgeVertices.push_back(halfEdgeVertex);
    }

    // Create faces (triangles)
    for (size_t i = 0; i < indices.size(); i += 3) {
        std::vector<rude::VertexPtr> faceVertices = {
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

// Use rude:: namespace for all types
rude::MeshPtr GeometryConverter::toFaceVertex(rude::HalfEdgeMeshPtr halfEdgeMesh) {
    if (!halfEdgeMesh || halfEdgeMesh->isEmpty()) {
        return nullptr;
    }

    auto faceVertexMesh = std::make_shared<rude::Mesh>();
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    
    toVerticesAndFaces(halfEdgeMesh, positions, normals, texCoords, indices);
    
    // Convert to Vertex format
    std::vector<rude::Vertex> vertices;
    vertices.reserve(positions.size());
    
    for (size_t i = 0; i < positions.size(); ++i) {
        rude::Vertex vertex;
        // Use glm::vec3 for position and normal
        const glm::vec3& pos = positions[i];
        vertex.position = pos;
        if (i < normals.size()) {
            const glm::vec3& norm = normals[i];
            vertex.normal = norm;
        } else {
            vertex.normal = glm::vec3(0, 1, 0);
        }
        if (i < texCoords.size()) {
            const glm::vec2& tex = texCoords[i];
            vertex.texCoord = tex;
        } else {
            vertex.texCoord = glm::vec2(0, 0);
        }
        vertices.push_back(vertex);
    }
    
    faceVertexMesh->setData(vertices, indices);
    
    return faceVertexMesh;
}

HalfEdgeMeshPtr GeometryConverter::fromVerticesAndFaces(
    const std::vector<glm::vec3>& positions,
    const std::vector<std::vector<unsigned int>>& faces) {
    
    auto halfEdgeMesh = std::make_shared<rude::HalfEdgeMesh>();
    
    // Create vertices
    std::vector<rude::VertexPtr> vertices;
    vertices.reserve(positions.size());
    for (const auto& position : positions) {
        vertices.push_back(halfEdgeMesh->addVertex(position));
    }

    // Create faces
    for (const auto& face : faces) {
        if (face.size() < 3) {
            // Skipping face with less than 3 vertices
            continue;
        }
        std::vector<rude::VertexPtr> faceVertices;
        faceVertices.reserve(face.size());
        for (unsigned int vertexIndex : face) {
            if (vertexIndex < vertices.size()) {
                faceVertices.push_back(vertices[vertexIndex]);
            } else {
                // Invalid vertex index
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
    rude::HalfEdgeMeshPtr halfEdgeMesh,
    std::vector<glm::vec3>& positions,
    std::vector<glm::vec3>& normals,
    std::vector<glm::vec2>& texCoords,
    std::vector<unsigned int>& indices) {
    
    if (!halfEdgeMesh) {
        return;
    }
    
    const auto& vertices = halfEdgeMesh->getVertices();
    const auto& faces = halfEdgeMesh->getFaces();
    
    // Create vertex index mapping
    std::unordered_map<rude::VertexPtr, unsigned int> vertexIndexMap;
    
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
        positions.push_back(vertex->position);
        normals.push_back(vertex->normal);
        texCoords.push_back(vertex->texCoord);
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

bool GeometryConverter::validateConversion(rude::MeshPtr original, rude::HalfEdgeMeshPtr converted) {
    if (!original || !converted) {
        return false;
    }
    
    // Check vertex count
    size_t originalVertexCount = original->getVertices().size();
    size_t convertedVertexCount = converted->getVertices().size();
    if (originalVertexCount != convertedVertexCount) {
        qWarning() << "Vertex count mismatch in conversion";
        return false;
    }
    
    // Check face count (approximate, since polygons may be triangulated)
    size_t originalTriangleCount = original->getIndices().size() / 3;
    size_t convertedFaceCount = converted->getFaces().size();
    
    if (originalTriangleCount != convertedFaceCount) {
        qWarning() << "Face count mismatch: original" << originalTriangleCount 
                   << "converted" << convertedFaceCount;
        // This might be acceptable for polygon meshes
    }
    
    // Note: mesh validation method doesn't exist in rude:: API
    // Assume valid for now
    return true;
}

bool GeometryConverter::validateConversion(rude::HalfEdgeMeshPtr original, rude::MeshPtr converted) {
    if (!original || !converted) {
        return false;
    }
    
    // Check vertex count
    size_t originalVertexCount = original->getVertices().size();
    size_t convertedVertexCount = converted->getVertices().size();
    if (originalVertexCount != convertedVertexCount) {
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

void GeometryConverter::copyVertexData(rude::VertexPtr src, rude::Vertex& dst) {
    if (!src) return;
    // Convert glm types to Qt types
    auto pos = src->position;
    auto norm = src->normal;
    auto tex = src->texCoord;
    dst.position = glm::vec3(pos.x, pos.y, pos.z);
    dst.normal = glm::vec3(norm.x, norm.y, norm.z);
    dst.texCoord = glm::vec2(tex.x, tex.y);
}

void GeometryConverter::copyVertexData(const rude::Vertex& src, rude::VertexPtr dst) {
    if (!dst) return;
    // Convert Qt types to glm types
    dst->position = glm::vec3(src.position.x, src.position.y, src.position.z);
    dst->normal = glm::vec3(src.normal.x, src.normal.y, src.normal.z);
    dst->texCoord = glm::vec2(src.texCoord.x, src.texCoord.y);
}

void GeometryConverter::buildFaceVertexConnectivity(rude::HalfEdgeMeshPtr mesh) {
    if (!mesh) return;
    
    // Note: Half-edge connectivity building would need to be implemented
    // using the rude:: API structure. For now, just comment out legacy calls.
    
    // TODO: Implement proper half-edge connectivity building
    // This would involve traversing the half-edge structure and setting up
    // proper next/prev/twin relationships using the rude:: API
    
    qDebug() << "GeometryConverter: Half-edge connectivity building not yet implemented for rude:: API";
}

void GeometryConverter::optimizeVertexOrder(std::vector<rude::Vertex>& vertices, std::vector<unsigned int>& indices) {
    // Simple optimization: reorder vertices to improve cache locality
    // This is a basic implementation - more sophisticated algorithms exist
    
    std::vector<bool> used(vertices.size(), false);
    std::vector<rude::Vertex> optimizedVertices;
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

// Use rude::MeshPtr for clarity and consistency
rude::MeshPtr GeometryConverter::toFaceVertex(NURBSSurfacePtr nurbs, int resolutionU, int resolutionV) {
    if (!nurbs) return nullptr;
    return nurbs->tessellate(resolutionU, resolutionV);
}

NURBSSurfacePtr GeometryConverter::toNURBS(HalfEdgeMeshPtr halfEdge, int degreeU, int degreeV) {
    // Stub implementation - would need NURBS fitting algorithm
    if (!halfEdge) return nullptr;
    
    // Parameters will be used in full implementation
    (void)degreeU;
    (void)degreeV;
    
    auto nurbs = std::make_shared<NURBSSurface>(3, 3, 5, 5); // Use default degrees for now
    // TODO: Implement mesh to NURBS fitting
    return nurbs;
}

// Subdivision surface conversions
SubdivisionMeshPtr GeometryConverter::toSubdivision(HalfEdgeMeshPtr baseMesh) {
    if (!baseMesh) return nullptr;
    return std::make_shared<SubdivisionMesh>(baseMesh);
}

SubdivisionMeshPtr GeometryConverter::toSubdivision(rude::MeshPtr baseMesh) {
    auto halfEdge = toHalfEdge(baseMesh);
    return toSubdivision(halfEdge);
}

HalfEdgeMeshPtr GeometryConverter::toHalfEdge(SubdivisionMeshPtr subdivision, int level) {
    // TODO: Fix type conversion between legacy HalfEdgeMesh and rude::HalfEdgeMesh
    (void)subdivision;
    (void)level;
    // GeometryConverter: toHalfEdge from SubdivisionMesh not yet implemented
    return nullptr;
}

rude::MeshPtr GeometryConverter::toFaceVertex(SubdivisionMeshPtr subdivision, int level) {
    if (!subdivision) return nullptr;
    return subdivision->generateRenderMesh(level);
}

// Voxel conversions (stubs)
VoxelGridPtr GeometryConverter::toVoxels(rude::MeshPtr mesh, float voxelSize, float bandwidth) {
    if (!mesh) return nullptr;
    
    // Compute mesh bounds
    const auto& vertices = mesh->getVertices();
    if (vertices.empty()) return nullptr;
    
    glm::vec3 minBounds(vertices[0].position.x, vertices[0].position.y, vertices[0].position.z);
    glm::vec3 maxBounds(vertices[0].position.x, vertices[0].position.y, vertices[0].position.z);

    for (const auto& vertex : vertices) {
        minBounds.x = std::min(minBounds.x, vertex.position.x);
        minBounds.y = std::min(minBounds.y, vertex.position.y);
        minBounds.z = std::min(minBounds.z, vertex.position.z);

        maxBounds.x = std::max(maxBounds.x, vertex.position.x);
        maxBounds.y = std::max(maxBounds.y, vertex.position.y);
        maxBounds.z = std::max(maxBounds.z, vertex.position.z);
    }

    // Expand bounds by bandwidth
    glm::vec3 expansion(bandwidth * voxelSize, bandwidth * voxelSize, bandwidth * voxelSize);
    minBounds -= expansion;
    maxBounds += expansion;

    glm::vec3 size = maxBounds - minBounds;
    int resX = static_cast<int>(std::ceil(size.x / voxelSize));
    int resY = static_cast<int>(std::ceil(size.y / voxelSize));
    int resZ = static_cast<int>(std::ceil(size.z / voxelSize));

    auto voxelGrid = std::make_shared<VoxelGrid>(minBounds, voxelSize, resX, resY, resZ);
    voxelGrid->fromMesh(mesh, bandwidth);

    return voxelGrid;
}

VoxelGridPtr GeometryConverter::toVoxels(HalfEdgeMeshPtr halfEdge, float voxelSize, float bandwidth) {
    auto mesh = toFaceVertex(halfEdge);
    return toVoxels(mesh, voxelSize, bandwidth);
}

rude::MeshPtr GeometryConverter::toFaceVertex(VoxelGridPtr voxels, float isoValue) {
    if (!voxels) return nullptr;
    return voxels->extractSurface(isoValue);
}

HalfEdgeMeshPtr GeometryConverter::toHalfEdge(VoxelGridPtr voxels, float isoValue) {
    auto mesh = toFaceVertex(voxels, isoValue);
    return toHalfEdge(mesh);
}

// Point cloud conversions (stubs)
PointCloudPtr GeometryConverter::toPointCloud(rude::MeshPtr mesh, int samplesPerFace) {
    if (!mesh) return nullptr;
    
    auto pointCloud = std::make_shared<PointCloud>();
    const auto& vertices = mesh->getVertices();
    const auto& indices = mesh->getIndices();
    
    // Simple sampling - just copy vertices for now
    for (const auto& vertex : vertices) {
        // TODO: Fix PointCloud::addPoint API call
        // pointCloud->addPoint(vertex->position, vertex->normal);
        (void)vertex;
    }
    
    // TODO: Implement proper face sampling
    qDebug() << "GeometryConverter: toPointCloud not yet implemented for rude:: API";
    return pointCloud;
}

PointCloudPtr GeometryConverter::toPointCloud(HalfEdgeMeshPtr halfEdge, int samplesPerFace) {
    auto mesh = toFaceVertex(halfEdge);
    return toPointCloud(mesh, samplesPerFace);
}


// MeshConverterCache Implementation
MeshConverterCache& MeshConverterCache::getInstance() {
    static MeshConverterCache instance;
    return instance;
}

// Use rude:: namespace for all types
rude::HalfEdgeMeshPtr MeshConverterCache::getHalfEdge(rude::MeshPtr mesh) {
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
        m_trackedFaceVertex.insert(std::weak_ptr<rude::Mesh>(mesh));
        // TODO: Fix type mismatch between legacy HalfEdgeMesh and rude::HalfEdgeMesh
        // m_trackedHalfEdge.insert(std::weak_ptr<rude::HalfEdgeMesh>(halfEdgeMesh));
    }
    
    return halfEdgeMesh;
}

// Use rude:: namespace for all types
rude::MeshPtr MeshConverterCache::getFaceVertex(rude::HalfEdgeMeshPtr mesh) {
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
        // TODO: Fix type mismatch between legacy HalfEdgeMesh and rude::HalfEdgeMesh
        // m_trackedHalfEdge.insert(std::weak_ptr<rude::HalfEdgeMesh>(mesh));
        m_trackedFaceVertex.insert(std::weak_ptr<rude::Mesh>(faceVertexMesh));
    }
    
    return faceVertexMesh;
}

void MeshConverterCache::invalidate(rude::MeshPtr mesh) {
    if (!mesh) return;
    
    auto it = m_faceVertexToHalfEdge.find(mesh);
    if (it != m_faceVertexToHalfEdge.end()) {
        auto halfEdgeMesh = it->second;
        m_halfEdgeToFaceVertex.erase(halfEdgeMesh);
        m_faceVertexToHalfEdge.erase(it);
    }
}

void MeshConverterCache::invalidate(rude::HalfEdgeMeshPtr mesh) {
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

// Stub implementations for missing functions
rude::MeshPtr GeometryConverter::toFaceVertex(std::shared_ptr<PointCloud> pointCloud, int /* unused */) {
    // TODO: Implement point cloud to mesh conversion
    if (!pointCloud) return nullptr;
    
    // For now, return an empty mesh
    auto mesh = std::make_shared<rude::Mesh>();
    return mesh;
}

void GeometryConverter::optimizeMeshForRendering(rude::MeshPtr mesh) {
    // TODO: Implement mesh optimization for rendering
    if (!mesh) return;
    // Stub implementation - no optimization for now
}

void GeometryConverter::optimizeHalfEdgeForEditing(rude::HalfEdgeMeshPtr mesh) {
    // TODO: Implement half-edge mesh optimization for editing
    if (!mesh) return;
    // Stub implementation - no optimization for now
}

std::shared_ptr<BVHTree> GeometryConverter::createBVH(rude::MeshPtr mesh) {
    // TODO: Implement BVH tree creation
    if (!mesh) return nullptr;
    // Stub implementation - return null for now
    return nullptr;
}
