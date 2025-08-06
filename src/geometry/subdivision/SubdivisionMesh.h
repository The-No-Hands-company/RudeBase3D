#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "core/mesh_forward.hpp"


class SubdivisionMesh {
public:
    struct SubdivisionLevel {
        rude::HalfEdgeMeshPtr mesh;
        int level;
        bool isQuadMesh;
    };

    SubdivisionMesh(rude::HalfEdgeMeshPtr baseMesh);
    void subdivide(int levels);
    rude::HalfEdgeMeshPtr getLevel(int level) const;
    int getNumLevels() const { return static_cast<int>(m_levels.size()); }
    rude::HalfEdgeMeshPtr getFinestLevel() const { return m_levels.empty() ? nullptr : m_levels.back().mesh; }
    rude::MeshPtr generateRenderMesh(int level = -1) const;
    rude::HalfEdgeMeshPtr applyCatmullClark(rude::HalfEdgeMeshPtr mesh);
    void computeEdgePoints(rude::HalfEdgeMeshPtr mesh,
                          const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
                          std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints);
    void computeVertexPoints(rude::HalfEdgeMeshPtr mesh,
                            const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
                            const std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints,
                            std::unordered_map<rude::VertexPtr, glm::vec3>& vertexPoints);
private:
    std::vector<SubdivisionLevel> m_levels;
};
