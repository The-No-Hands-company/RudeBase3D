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
 * █  FILE: LoopCutTool.h                                                         █
 * █  DESCRIPTION: Advanced Loop Cut Tool for Mesh Edge Loop Division            █
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
 * █  This file contains the LoopCutTool class providing advanced edge loop      █
 * █  cutting functionality for precise mesh division and topology modification  █
 * █  in professional 3D modeling workflows.                                     █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <unordered_set>
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
 * @brief Tool for creating edge loops in half-edge meshes
 * 
 * The LoopCutTool creates edge loops by cutting through faces,
 * adding new vertices and edges while maintaining proper topology.
 * Essential for polygon modeling workflows.
 */
class LoopCutTool {
public:
    enum class LoopDirection {
        Automatic,      // Detect best direction automatically
        Horizontal,     // Cut horizontally relative to face orientation
        Vertical,       // Cut vertically relative to face orientation
        Custom          // Use custom direction vector
    };

    LoopCutTool();
    ~LoopCutTool() = default;

    // Tool state
    void setMesh(HalfEdgeMeshPtr mesh) { m_mesh = mesh; }
    HalfEdgeMeshPtr getMesh() const { return m_mesh; }

    // Loop cut operations
    bool createLoopCut(HalfEdgeEdgePtr startEdge, int numCuts = 1);
    bool createLoopCutAtPosition(HalfEdgeEdgePtr startEdge, float position, int numCuts = 1);
    bool createParallelLoops(HalfEdgeEdgePtr startEdge, int numLoops, float spacing);
    
    // Tool settings
    void setLoopDirection(LoopDirection direction) { m_loopDirection = direction; }
    LoopDirection getLoopDirection() const { return m_loopDirection; }
    
    void setCustomDirection(const glm::vec3& direction) { m_customDirection = glm::normalize(direction); }
    glm::vec3 getCustomDirection() const { return m_customDirection; }
    
    void setEvenSpacing(bool enabled) { m_evenSpacing = enabled; }
    bool getEvenSpacing() const { return m_evenSpacing; }
    
    void setCutPosition(float position) { m_cutPosition = glm::clamp(position, 0.0f, 1.0f); }
    float getCutPosition() const { return m_cutPosition; }

    // Validation
    bool canCreateLoopCut(HalfEdgeEdgePtr startEdge) const;
    
    // Result information
    const std::vector<std::vector<HalfEdgeEdgePtr>>& getCreatedLoops() const { return m_createdLoops; }
    const std::vector<HalfEdgeVertexPtr>& getCreatedVertices() const { return m_createdVertices; }
    const std::vector<HalfEdgeEdgePtr>& getCreatedEdges() const { return m_createdEdges; }

private:
    // Core mesh
    HalfEdgeMeshPtr m_mesh;
    
    // Tool settings
    LoopDirection m_loopDirection = LoopDirection::Automatic;
    glm::vec3 m_customDirection = glm::vec3(1, 0, 0);
    bool m_evenSpacing = true;
    float m_cutPosition = 0.5f;
    
    // Result tracking
    std::vector<std::vector<HalfEdgeEdgePtr>> m_createdLoops;
    std::vector<HalfEdgeVertexPtr> m_createdVertices;
    std::vector<HalfEdgeEdgePtr> m_createdEdges;
    
    // Internal operations
    std::vector<HalfEdgeEdgePtr> findEdgeLoop(HalfEdgeEdgePtr startEdge) const;
    std::vector<HalfEdgeFacePtr> findAffectedFaces(const std::vector<HalfEdgeEdgePtr>& edgeLoop) const;
    
    bool cutEdgeLoop(const std::vector<HalfEdgeEdgePtr>& edgeLoop, float position);
    bool cutSingleEdge(HalfEdgeEdgePtr edge, float position);
    
    // Loop detection helpers
    HalfEdgeEdgePtr findNextLoopEdge(HalfEdgeEdgePtr currentEdge, HalfEdgeFacePtr throughFace) const;
    bool isValidLoopContinuation(HalfEdgeEdgePtr currentEdge, HalfEdgeEdgePtr nextEdge) const;
    glm::vec3 calculateLoopDirection(HalfEdgeEdgePtr startEdge) const;
    
    // Edge operations
    std::pair<HalfEdgeVertexPtr, HalfEdgeEdgePtr> splitEdge(HalfEdgeEdgePtr edge, float position);
    HalfEdgeVertexPtr createVertexOnEdge(HalfEdgeEdgePtr edge, float position);
    
    // Face operations
    bool splitFaceAtVertices(HalfEdgeFacePtr face, HalfEdgeVertexPtr vertex1, HalfEdgeVertexPtr vertex2);
    std::vector<HalfEdgeFacePtr> triangulateQuad(HalfEdgeFacePtr quadFace);
    
    // Topology helpers
    HalfEdgeEdgePtr createEdgeBetweenVertices(HalfEdgeVertexPtr v1, HalfEdgeVertexPtr v2);
    void updateFaceConnectivity(HalfEdgeFacePtr face);
    
    // Validation helpers
    bool isManifoldEdge(HalfEdgeEdgePtr edge) const;
    bool wouldCreateInvalidTopology(HalfEdgeEdgePtr edge, float position) const;
    
    // Cleanup
    void clearResults();
};
