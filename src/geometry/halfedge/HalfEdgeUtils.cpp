
#include "core/half_edge_mesh.hpp"
#include <QDebug>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <glm/glm.hpp>

namespace rude {
namespace HalfEdgeUtils {

std::pair<rude::EdgePtr, rude::EdgePtr> splitEdge(rude::HalfEdgeMeshPtr mesh, rude::EdgePtr edge, const glm::vec3& position) {
    if (!edge || !mesh) {
        return {nullptr, nullptr};
    }

    // This is a simplified implementation
    // In a real implementation, we'd need proper mesh connectivity
    qDebug() << "Edge splitting not yet fully implemented";
    return {nullptr, nullptr};
}

bool collapseEdge(rude::HalfEdgeMeshPtr mesh, rude::EdgePtr edge) {
    if (!edge || !mesh) {
        return false;
    }

    // Edge collapse implementation would go here
    qDebug() << "Edge collapse not yet implemented";
    return false;
}

bool flipEdge(rude::EdgePtr edge) {
    if (!edge) {
        return false;
    }

    // Edge flip implementation would go here
    qDebug() << "Edge flip not yet implemented";
    return false;
}

std::vector<rude::FacePtr> extrudeFaces(rude::HalfEdgeMeshPtr mesh, const std::vector<rude::FacePtr>& faces, float distance) {
    std::vector<rude::FacePtr> extrudedFaces;
    
    if (!mesh || faces.empty()) {
        return extrudedFaces;
    }
    
    // Step 1: Collect all vertices that will be moved
    std::unordered_set<rude::VertexPtr> verticesToExtrude;
    std::unordered_map<rude::VertexPtr, rude::VertexPtr> vertexMapping;
    
    for (const auto& face : faces) {
        if (!face) continue;
        
        auto vertices = face->getVertices();
        for (const auto& vertex : vertices) {
            verticesToExtrude.insert(vertex);
        }
    }
    
    // Step 2: Create new vertices for extruded positions
    for (const auto& vertex : verticesToExtrude) {
        // Calculate average normal from all adjacent faces of this vertex
        glm::vec3 averageNormal(0, 0, 0);
        int normalCount = 0;
        
        // Get all faces adjacent to this vertex that are being extruded
        for (const auto& face : faces) {
            auto faceVertices = face->getVertices();
            if (std::find(faceVertices.begin(), faceVertices.end(), vertex) != faceVertices.end()) {
                // Use the face's normal member directly
                averageNormal += face->normal;
                normalCount++;
            }
        }
        
        if (normalCount > 0) {
            averageNormal = glm::normalize(averageNormal);
            glm::vec3 newPosition = vertex->position + averageNormal * distance;
            auto newVertex = mesh->addVertex(newPosition);
            vertexMapping[vertex] = newVertex;
        }
    }
    
    // Step 3: Create new top faces from the extruded vertices
    for (const auto& face : faces) {
        if (!face) continue;
        
        auto vertices = face->getVertices();
        std::vector<rude::VertexPtr> newVertices;
        
        for (const auto& vertex : vertices) {
            auto it = vertexMapping.find(vertex);
            if (it != vertexMapping.end()) {
                newVertices.push_back(it->second);
            }
        }
        
        if (newVertices.size() >= 3) {
            auto extrudedFace = mesh->addFace(newVertices);
            if (extrudedFace) {
                extrudedFaces.push_back(extrudedFace);
            }
        }
    }
    
    // Step 4: Create side faces (connecting original edges to extruded edges)
    for (const auto& face : faces) {
        if (!face) continue;
        
        auto edges = face->getEdges();
        for (const auto& edge : edges) {
            // Check if this edge is on the boundary of the extrusion
            // (i.e., the adjacent face is not being extruded)
            auto halfEdge = face->halfEdge; // Get the half-edge from the face
            if (!halfEdge) continue;
            
            auto twin = halfEdge->twin;
            bool isBoundaryEdge = !twin; // No twin means mesh boundary
            
            if (twin) {
                auto adjacentFace = twin->face;
                if (adjacentFace && std::find(faces.begin(), faces.end(), adjacentFace) != faces.end()) {
                    // Adjacent face is also being extruded, skip this edge for side face creation
                    continue;
                }
                isBoundaryEdge = true; // Adjacent face not extruded = boundary of extrusion
            }
            
            if (isBoundaryEdge) {
                auto v1 = halfEdge->vertex;
                auto v2 = halfEdge->next ? halfEdge->next->vertex : nullptr;
                
                if (v1 && v2) {
                    auto v1_new = vertexMapping[v1];
                    auto v2_new = vertexMapping[v2];
                    
                    if (v1_new && v2_new) {
                        // Create a quad face: v1 -> v2 -> v2_new -> v1_new
                        std::vector<rude::VertexPtr> quadVertices = {v1, v2, v2_new, v1_new};
                        auto sideFace = mesh->addFace(quadVertices);
                        if (sideFace) {
                            extrudedFaces.push_back(sideFace);
                        }
                    }
                }
            }
        }
    }
    
    // Step 5: Remove original faces (they are now the "bottom" of the extrusion)
    for (const auto& face : faces) {
        if (face) {
            mesh->removeFace(face);
        }
    }
    
    qDebug() << "Extruded" << faces.size() << "faces with distance" << distance;
    qDebug() << "Created" << extrudedFaces.size() << "new faces";
    
    return extrudedFaces;
}

std::vector<rude::FacePtr> insetFaces(rude::HalfEdgeMeshPtr mesh, const std::vector<rude::FacePtr>& faces, float inset) {
    std::vector<rude::FacePtr> insetFaces;
    
    if (!mesh || faces.empty()) {
        return insetFaces;
    }
    
    for (const auto& face : faces) {
        if (!face) continue;
        
        // Get face vertices and calculate centroid
        auto vertices = face->getVertices();
        // Calculate centroid manually
        glm::vec3 centroid(0.0f);
        if (!vertices.empty()) {
            for (const auto& vertex : vertices) {
                centroid += vertex->position;
            }
            centroid /= static_cast<float>(vertices.size());
        }
        
        if (vertices.size() < 3) continue;
        
        // Create inset vertices by moving towards centroid
        std::vector<rude::VertexPtr> insetVertices;
        for (const auto& vertex : vertices) {
            glm::vec3 toCenter = glm::normalize(centroid - vertex->position);
            glm::vec3 newPosition = vertex->position + toCenter * inset;
            auto newVertex = mesh->addVertex(newPosition);
            insetVertices.push_back(newVertex);
        }
        
        // Create the inset face
        if (insetVertices.size() >= 3) {
            auto insetFace = mesh->addFace(insetVertices);
            if (insetFace) {
                insetFaces.push_back(insetFace);
            }
            
            // Create connecting faces between original edges and inset edges
            for (size_t i = 0; i < vertices.size(); ++i) {
                size_t next = (i + 1) % vertices.size();
                
                auto v1 = vertices[i];
                auto v2 = vertices[next];
                auto v1_inset = insetVertices[i];
                auto v2_inset = insetVertices[next];
                
                // Create quad: v1 -> v2 -> v2_inset -> v1_inset
                std::vector<rude::VertexPtr> quadVertices = {v1, v2, v2_inset, v1_inset};
                auto connectingFace = mesh->addFace(quadVertices);
                if (connectingFace) {
                    insetFaces.push_back(connectingFace);
                }
            }
        }
        
        // Remove the original face
        mesh->removeFace(face);
    }
    
    qDebug() << "Inset" << faces.size() << "faces with inset distance" << inset;
    return insetFaces;
}

std::vector<rude::EdgePtr> bevelEdges(rude::HalfEdgeMeshPtr mesh, const std::vector<rude::EdgePtr>& edges, float amount) {
    std::vector<rude::EdgePtr> beveledEdges;
    
    if (!mesh || edges.empty()) {
        return beveledEdges;
    }
    
    for (const auto& edge : edges) {
        if (!edge) continue;
        
        // Basic edge bevel implementation
        // This is a simplified version - real bevel would be more complex
        auto origin = edge->halfEdge ? edge->halfEdge->vertex : nullptr;
        auto target = edge->halfEdge && edge->halfEdge->twin ? edge->halfEdge->twin->vertex : nullptr;
        
        if (!origin || !target) continue;
        
        // Calculate bevel direction (perpendicular to edge in face plane)
        auto edgeVector = target->position - origin->position;
        auto face = edge->halfEdge ? edge->halfEdge->face : nullptr;
        
        if (face) {
            auto faceNormal = face->normal; // Use the face normal directly
            auto bevelDirection = glm::cross(edgeVector, faceNormal);
            
            // Create new vertices offset from original edge vertices
            glm::vec3 offset = glm::normalize(bevelDirection) * amount;
            auto newOrigin = mesh->addVertex(origin->position + offset);
            auto newTarget = mesh->addVertex(target->position + offset);
            
            // Create new edge - TODO: Implement proper edge creation in half-edge mesh
            // For now, this is a placeholder - edges are typically created when faces are added
            // auto newEdge = mesh->addEdge(newOrigin, newTarget);
            // if (newEdge) {
            //     beveledEdges.push_back(newEdge);
            // }
        }
    }
    
    qDebug() << "Beveled" << edges.size() << "edges with amount" << amount;
    return beveledEdges;
}

std::vector<rude::EdgePtr> getEdgeLoop(rude::EdgePtr startEdge) {
    std::vector<rude::EdgePtr> loop;
    
    if (!startEdge) {
        return loop;
    }
    
    auto current = startEdge;
    std::unordered_set<rude::EdgePtr> visited;
    
    do {
        if (visited.find(current) != visited.end()) {
            break; // Avoid infinite loops
        }
        
        visited.insert(current);
        loop.push_back(current);
        
        // Find next edge in loop
        // This is a simplified implementation
        // Real edge loop detection requires proper mesh topology analysis
        auto twin = current->halfEdge ? current->halfEdge->twin : nullptr;
        if (twin) {
            auto next = twin->next;
            if (next && next->next) {
                current = next->next->edge;
            } else {
                break;
            }
        } else {
            break;
        }
        
    } while (current && current != startEdge && loop.size() < 1000); // Safety limit
    
    return loop;
}

std::vector<rude::EdgePtr> getEdgeRing(rude::EdgePtr startEdge) {
    std::vector<rude::EdgePtr> ring;
    
    if (!startEdge) {
        return ring;
    }
    
    // Edge ring implementation
    // This would traverse edges that are parallel to the start edge
    ring.push_back(startEdge);
    
    qDebug() << "Edge ring detection simplified - needs proper implementation";
    return ring;
}

// Catmull-Clark subdivision
rude::HalfEdgeMeshPtr catmullClarkSubdivide(rude::HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return nullptr;
    }
    auto subdividedMesh = std::make_shared<rude::HalfEdgeMesh>();
    
    // Catmull-Clark subdivision algorithm:
    // 1. Create face points (centroid of each face)
    // 2. Create edge points (average of edge endpoints and adjacent face points)
    // 3. Update original vertices (weighted average)
    // 4. Connect all points to create new faces
    
    std::unordered_map<rude::FacePtr, rude::VertexPtr> facePoints;
    std::unordered_map<rude::EdgePtr, rude::VertexPtr> edgePoints;
    
    // Step 1: Create face points
    for (const auto& face : mesh->getFaces()) {
        // Calculate centroid manually
        auto vertices = face->getVertices();
        glm::vec3 centroid(0.0f);
        if (!vertices.empty()) {
            for (const auto& vertex : vertices) {
                centroid += vertex->position;
            }
            centroid /= static_cast<float>(vertices.size());
        }
        auto facePoint = subdividedMesh->addVertex(centroid);
        facePoints[face] = facePoint;
    }
    
    // Step 2: Create edge points
    for (const auto& edge : mesh->getEdges()) {
        if (edgePoints.find(edge) != edgePoints.end()) {
            continue; // Already processed
        }
        
        auto origin = edge->halfEdge ? edge->halfEdge->vertex : nullptr;
        auto target = edge->halfEdge && edge->halfEdge->twin ? edge->halfEdge->twin->vertex : nullptr;
        auto twin = edge->halfEdge ? edge->halfEdge->twin : nullptr;
        
        if (!origin || !target) continue;
        
        glm::vec3 edgePoint = (origin->position + target->position) * 0.5f;
        
        // Add face points if they exist
        if (auto face = edge->halfEdge ? edge->halfEdge->face : nullptr) {
            if (auto fp = facePoints[face]) {
                edgePoint += fp->position;
            }
        }
        if (twin && twin->face) {
            if (auto fp = facePoints[twin->face]) {
                edgePoint += fp->position;
            }
        }
        
        edgePoint *= 0.5f; // Average
        
        auto edgePointVertex = subdividedMesh->addVertex(edgePoint);
        edgePoints[edge] = edgePointVertex;
        if (twin) {
            edgePoints[twin->edge] = edgePointVertex;
        }
    }
    
    // Step 3: Update original vertices (simplified)
    std::unordered_map<rude::VertexPtr, rude::VertexPtr> originalToNew;
    for (const auto& vertex : mesh->getVertices()) {
        // For now, just copy the vertex
        // Real Catmull-Clark would apply proper weighting
        auto newVertex = subdividedMesh->addVertex(vertex->position);
        originalToNew[vertex] = newVertex;
    }
    
    // Step 4: Create new faces (simplified)
    for (const auto& face : mesh->getFaces()) {
        auto facePoint = facePoints[face];
        auto edges = face->getEdges();
        
        for (const auto& edge : edges) {
            auto edgePoint = edgePoints[edge];
            // Get origin vertex through the half-edge
            auto originVertex = edge->halfEdge ? originalToNew[edge->halfEdge->vertex] : nullptr;
            auto nextHalfEdge = edge->halfEdge ? edge->halfEdge->next : nullptr;
            auto nextEdge = nextHalfEdge ? nextHalfEdge->edge : nullptr;
            auto nextEdgePoint = nextEdge ? edgePoints[nextEdge] : nullptr;
            
            if (facePoint && edgePoint && originVertex && nextEdgePoint) {
                std::vector<rude::VertexPtr> quadVertices = {
                    originVertex, edgePoint, facePoint, nextEdgePoint
                };
                subdividedMesh->addFace(quadVertices);
            }
        }
    }
    
    subdividedMesh->updateNormals();
    return subdividedMesh;
}

// Loop subdivision
rude::HalfEdgeMeshPtr loopSubdivide(rude::HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return nullptr;
    }
    // Loop subdivision is primarily for triangle meshes
    // This is a simplified implementation
    qDebug() << "Loop subdivision not yet implemented";
    return nullptr;
}

std::vector<std::vector<rude::VertexPtr>> findConnectedComponents(rude::HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return {};
    }
    std::vector<std::vector<rude::VertexPtr>> components;
    std::unordered_set<rude::VertexPtr> visited;
    
    for (const auto& vertex : mesh->getVertices()) {
        if (visited.find(vertex) != visited.end()) {
            continue;
        }
        
        // BFS to find connected component
        std::vector<rude::VertexPtr> component;
        std::queue<rude::VertexPtr> queue;
        
        queue.push(vertex);
        visited.insert(vertex);
        
        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();
            component.push_back(current);
            
            // Add adjacent vertices - manually traverse half-edge structure
            std::vector<rude::VertexPtr> adjacent;
            if (current->halfEdge) {
                auto startHE = current->halfEdge;
                auto currentHE = startHE;
                do {
                    // Get the twin to find the vertex at the other end of the edge
                    if (currentHE->twin && currentHE->twin->vertex) {
                        adjacent.push_back(currentHE->twin->vertex);
                    }
                    // Move to next outgoing half-edge around this vertex
                    if (currentHE->twin && currentHE->twin->next) {
                        currentHE = currentHE->twin->next;
                    } else {
                        break;
                    }
                } while (currentHE && currentHE != startHE);
            }
            
            for (const auto& adj : adjacent) {
                if (visited.find(adj) == visited.end()) {
                    visited.insert(adj);
                    queue.push(adj);
                }
            }
        }
        
        components.push_back(std::move(component));
    }
    
    return components;
}

std::vector<rude::EdgePtr> findNonManifoldEdges(rude::HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return {};
    }
    std::vector<rude::EdgePtr> nonManifoldEdges;
    
    for (const auto& edge : mesh->getEdges()) {
        int faceCount = 0;
        if (edge->halfEdge && edge->halfEdge->face) faceCount++;
        if (auto twin = edge->halfEdge ? edge->halfEdge->twin : nullptr) {
            if (twin->face) faceCount++;
        }
        
        // Non-manifold if more than 2 faces share this edge
        if (faceCount > 2) {
            nonManifoldEdges.push_back(edge);
        }
    }
    
    return nonManifoldEdges;
}

std::vector<rude::VertexPtr> findNonManifoldVertices(rude::HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return {};
    }
    std::vector<rude::VertexPtr> nonManifoldVertices;
    
    for (const auto& vertex : mesh->getVertices()) {
        // Check if vertex has non-manifold configuration
        auto outgoingEdges = vertex->getOutgoingEdges();
        auto adjacentFaces = vertex->getAdjacentFaces();
        
        // Simple check: if faces don't form a fan around vertex
        if (adjacentFaces.size() > 0 && outgoingEdges.size() > 0) {
            // More sophisticated manifold checking would be needed here
            if (adjacentFaces.size() != outgoingEdges.size() && !vertex->isBoundary()) {
                nonManifoldVertices.push_back(vertex);
            }
        }
    }
    
    return nonManifoldVertices;
}

} // namespace HalfEdgeUtils
} // namespace rude
