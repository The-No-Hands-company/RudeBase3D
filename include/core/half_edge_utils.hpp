#pragma once

#include "core/half_edge_mesh.hpp"
#include "core/mesh_elements.hpp"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace rude {
namespace HalfEdgeUtils {
    // Edge operations
    std::pair<EdgePtr, EdgePtr> splitEdge(std::shared_ptr<HalfEdgeMesh> mesh, EdgePtr edge, const glm::vec3& position);
    bool collapseEdge(std::shared_ptr<HalfEdgeMesh> mesh, EdgePtr edge);
    bool flipEdge(EdgePtr edge);

    // Face operations
    std::vector<FacePtr> extrudeFaces(std::shared_ptr<HalfEdgeMesh> mesh, const std::vector<FacePtr>& faces, float distance);
    std::vector<FacePtr> insetFaces(std::shared_ptr<HalfEdgeMesh> mesh, const std::vector<FacePtr>& faces, float inset);
    std::vector<EdgePtr> bevelEdges(std::shared_ptr<HalfEdgeMesh> mesh, const std::vector<EdgePtr>& edges, float amount);

    // Loop operations
    std::vector<EdgePtr> getEdgeLoop(EdgePtr startEdge);
    std::vector<EdgePtr> getEdgeRing(EdgePtr startEdge);

    // Subdivision
    std::shared_ptr<HalfEdgeMesh> catmullClarkSubdivide(std::shared_ptr<HalfEdgeMesh> mesh);
    std::shared_ptr<HalfEdgeMesh> loopSubdivide(std::shared_ptr<HalfEdgeMesh> mesh);

    // Mesh analysis
    std::vector<std::vector<VertexPtr>> findConnectedComponents(std::shared_ptr<HalfEdgeMesh> mesh);
    std::vector<EdgePtr> findNonManifoldEdges(std::shared_ptr<HalfEdgeMesh> mesh);
    std::vector<VertexPtr> findNonManifoldVertices(std::shared_ptr<HalfEdgeMesh> mesh);
} // namespace HalfEdgeUtils
} // namespace rude 