/*
 * ██████╗ ██╗   ██╗██████╗ ███████╗██████╗  █████╗ ███████╗███████╗██████╗ ██████╗ 
 * ██╔══██╗██║   ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝╚════██╗██╔══██╗
 * ██████╔╝██║   ██║██║  ██║█████╗  ██████╔╝███████║███████╗█████╗   █████╔╝██║  ██║
 * ██╔══██╗██║   ██║██║  ██║██╔══╝  ██╔══██╗██╔══██║╚════██║██╔══╝   ╚═══██╗██║  ██║
 * ██║  ██║╚██████╔╝██████╔╝███████╗██████╔╝██║  ██║███████║███████╗██████╔╝██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═════╝ ╚═════╝ 
 * 
 * Professional 3D Modeling & Animation Software
 * 
 * ████████████████████████████████████████████████████████████████████████████████
 * █                                                                              █
 * █  FILE: SubdivisionTool.h                                                     █
 * █  DESCRIPTION: Advanced Subdivision Surface Tool & Mesh Refinement          █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite            █
 * █  VERSION: 1.0.0                                                              █
 * █  CREATED: 2024                                                               █
 * █  UPDATED: August 7, 2025                                                     █
 * █                                                                              █
 * █  LICENSE: MIT License                                                        █
 * █  COMPANY: The No Hands Company                                               █
 * █  AUTHOR: RudeBase3D Development Team                                         █
 * █                                                                              █
 * █  This file contains the SubdivisionTool class providing advanced            █
 * █  subdivision surface algorithms and mesh refinement capabilities            █
 * █  for professional 3D modeling workflows.                                    █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <unordered_map>
#include "core/mesh_forward.hpp"

namespace rude {
class Face;
class Vertex;
class Edge;
class HalfEdgeMesh;
using FacePtr = std::shared_ptr<Face>;
using VertexPtr = std::shared_ptr<Vertex>;
using EdgePtr = std::shared_ptr<Edge>;
using HalfEdgeMeshPtr = std::shared_ptr<HalfEdgeMesh>;
}

/**
 * @brief Tool for applying subdivision operations to half-edge meshes
 * 
 * The SubdivisionTool implements various subdivision algorithms including
 * Catmull-Clark for quad meshes and Loop subdivision for triangle meshes.
 * Creates smooth, organic surfaces from base polygon meshes.
 */
class SubdivisionTool {
public:
    enum class SubdivisionType {
        CatmullClark,   // Catmull-Clark subdivision (quads)
        Loop,           // Loop subdivision (triangles)
        DooSabin,       // Doo-Sabin subdivision
        ModifiedButterfly // Modified Butterfly subdivision
    };
    
    enum class BoundaryRule {
        Sharp,          // Keep boundary edges sharp
        Smooth,         // Smooth boundary edges
        CreaseAngle     // Use crease angle threshold
    };

    SubdivisionTool();
    ~SubdivisionTool() = default;

    // Tool state
    void setMesh(rude::HalfEdgeMeshPtr mesh) { m_mesh = mesh; }
    rude::HalfEdgeMeshPtr getMesh() const { return m_mesh; }

    // Subdivision operations
    rude::HalfEdgeMeshPtr subdivide(int levels = 1);
    rude::HalfEdgeMeshPtr subdivideAdaptive(float errorThreshold = 0.1f);
    rude::HalfEdgeMeshPtr subdivideRegion(const std::vector<rude::FacePtr>& faces, int levels = 1);
    
    // Tool settings
    void setSubdivisionType(SubdivisionType type) { m_subdivisionType = type; }
    SubdivisionType getSubdivisionType() const { return m_subdivisionType; }
    
    void setBoundaryRule(BoundaryRule rule) { m_boundaryRule = rule; }
    BoundaryRule getBoundaryRule() const { return m_boundaryRule; }
    
    void setCreaseAngle(float angleInDegrees) { m_creaseAngle = angleInDegrees; }
    float getCreaseAngle() const { return m_creaseAngle; }
    
    void setPreserveBoundary(bool preserve) { m_preserveBoundary = preserve; }
    bool getPreserveBoundary() const { return m_preserveBoundary; }
    
    void setUseQuadraticErrorMetric(bool use) { m_useQEM = use; }
    bool getUseQuadraticErrorMetric() const { return m_useQEM; }

    // Validation
    bool canSubdivide() const;
    SubdivisionType getRecommendedType() const;
    
    // Analysis
    bool isQuadMesh() const;
    bool isTriangleMesh() const;
    float calculateMeshComplexity() const;

private:
    // Core mesh
    rude::HalfEdgeMeshPtr m_mesh;
    
    // Tool settings
    SubdivisionType m_subdivisionType = SubdivisionType::CatmullClark;
    BoundaryRule m_boundaryRule = BoundaryRule::Sharp;
    float m_creaseAngle = 30.0f;
    bool m_preserveBoundary = true;
    bool m_useQEM = false;
    
    // Catmull-Clark subdivision
    rude::HalfEdgeMeshPtr applyCatmullClark(rude::HalfEdgeMeshPtr mesh) const;
    void computeFacePoints(rude::HalfEdgeMeshPtr mesh, 
                          std::unordered_map<rude::FacePtr, glm::vec3>& facePoints) const;
    void computeEdgePoints(rude::HalfEdgeMeshPtr mesh,
                          const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
                          std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints) const;
    void computeVertexPoints(rude::HalfEdgeMeshPtr mesh,
                            const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
                            const std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints,
                            std::unordered_map<rude::VertexPtr, glm::vec3>& vertexPoints) const;
    
    // Loop subdivision  
    rude::HalfEdgeMeshPtr applyLoop(rude::HalfEdgeMeshPtr mesh) const;
    glm::vec3 computeLoopVertexPoint(rude::VertexPtr vertex) const;
    glm::vec3 computeLoopEdgePoint(rude::EdgePtr edge) const;
    
    // Doo-Sabin subdivision
    rude::HalfEdgeMeshPtr applyDooSabin(rude::HalfEdgeMeshPtr mesh) const;
    
    // Helper methods
    bool isCreaseEdge(rude::EdgePtr edge) const;
    bool isBoundaryVertex(rude::VertexPtr vertex) const;
    bool isBoundaryEdge(rude::EdgePtr edge) const;
    float calculateDihedralAngle(rude::EdgePtr edge) const;
    
    // Mesh construction helpers
    rude::HalfEdgeMeshPtr buildSubdividedMesh(
        const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
        const std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints,
        const std::unordered_map<rude::VertexPtr, glm::vec3>& vertexPoints) const;
    
    void createSubdividedFaces(rude::HalfEdgeMeshPtr newMesh,
                              const std::unordered_map<rude::FacePtr, glm::vec3>& facePoints,
                              const std::unordered_map<rude::EdgePtr, glm::vec3>& edgePoints,
                              const std::unordered_map<rude::VertexPtr, glm::vec3>& vertexPoints) const;
    
    // Adaptive subdivision helpers
    float calculateSubdivisionError(rude::FacePtr face) const;
    bool needsSubdivision(rude::FacePtr face, float threshold) const;
    
    // Quality metrics
    float calculateAspectRatio(rude::FacePtr face) const;
    float calculateTriangleQuality(rude::FacePtr face) const;
};
