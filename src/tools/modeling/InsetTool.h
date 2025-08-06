#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
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
 * @brief Tool for insetting faces in half-edge meshes
 * 
 * The InsetTool creates smaller faces inside selected faces, maintaining
 * proper topology and creating connecting faces between the original and
 * inset face boundaries.
 */
class InsetTool {
public:
    enum class InsetMode {
        Individual,  // Inset each face individually
        Region      // Inset selected faces as a region
    };

    InsetTool();
    ~InsetTool() = default;

    // Tool state
    void setMesh(rude::HalfEdgeMeshPtr mesh) { m_mesh = mesh; }
    rude::HalfEdgeMeshPtr getMesh() const { return m_mesh; }

    // Inset operations
    bool insetFaces(const std::vector<rude::FacePtr>& faces, float insetAmount);
    bool insetFace(rude::FacePtr face, float insetAmount);
    
    // Tool settings
    void setInsetMode(InsetMode mode) { m_insetMode = mode; }
    InsetMode getInsetMode() const { return m_insetMode; }
    
    void setInsetDepth(float depth) { m_insetDepth = depth; }
    float getInsetDepth() const { return m_insetDepth; }
    
    void setScaleEvenThickness(bool enabled) { m_scaleEvenThickness = enabled; }
    bool getScaleEvenThickness() const { return m_scaleEvenThickness; }

    // Validation
    bool canInset() const;
    
    // Result information
    const std::vector<rude::FacePtr>& getCreatedFaces() const { return m_createdFaces; }
    const std::vector<rude::VertexPtr>& getCreatedVertices() const { return m_createdVertices; }

private:
    // Core mesh
    rude::HalfEdgeMeshPtr m_mesh;
    
    // Tool settings
    InsetMode m_insetMode = InsetMode::Individual;
    float m_insetDepth = 0.0f;
    bool m_scaleEvenThickness = false;
    
    // Result tracking
    std::vector<rude::FacePtr> m_createdFaces;
    std::vector<rude::VertexPtr> m_createdVertices;
    
    // Internal operations
    rude::FacePtr insetFaceIndividual(rude::FacePtr face, float insetAmount);
    std::vector<rude::FacePtr> insetRegion(const std::vector<rude::FacePtr>& faces, float insetAmount);
    
    // Helper methods
    glm::vec3 calculateFaceCenter(rude::FacePtr face) const;
    glm::vec3 calculateInsetPosition(rude::VertexPtr vertex, rude::FacePtr face, float insetAmount) const;
    std::vector<rude::VertexPtr> createInsetVertices(rude::FacePtr face, float insetAmount);
    void createBridgeFaces(const std::vector<rude::VertexPtr>& originalVertices, 
                          const std::vector<rude::VertexPtr>& insetVertices);
    
    // Topology helpers
    rude::VertexPtr duplicateVertex(rude::VertexPtr vertex, const glm::vec3& newPosition);
    rude::FacePtr createQuadFace(rude::VertexPtr v1, rude::VertexPtr v2,
                                   rude::VertexPtr v3, rude::VertexPtr v4);
    
    // Cleanup
    void clearResults();
};
