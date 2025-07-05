#include "HalfEdgeMesh.h"
#include "core/half_edge_mesh.hpp"
#include <QDebug>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <glm/glm.hpp>

namespace HalfEdgeUtils {

std::pair<HalfEdgeEdgePtr, HalfEdgeEdgePtr> splitEdge(HalfEdgeMeshPtr mesh, HalfEdgeEdgePtr edge, const glm::vec3& position) {
    if (!edge || !mesh) {
        return {nullptr, nullptr};
    }

    // This is a simplified implementation
    // In a real implementation, we'd need proper mesh connectivity
    qDebug() << "Edge splitting not yet fully implemented";
    return {nullptr, nullptr};
}

bool collapseEdge(HalfEdgeMeshPtr mesh, HalfEdgeEdgePtr edge) {
    if (!edge || !mesh) {
        return false;
    }

    // Edge collapse implementation would go here
    qDebug() << "Edge collapse not yet implemented";
    return false;
}

bool flipEdge(HalfEdgeEdgePtr edge) {
    if (!edge || !edge->getTwin()) {
        return false;
    }

    // Edge flip implementation would go here
    qDebug() << "Edge flip not yet implemented";
    return false;
}

std::vector<HalfEdgeFacePtr> extrudeFaces(HalfEdgeMeshPtr mesh, const std::vector<HalfEdgeFacePtr>& faces, float distance) {
    std::vector<HalfEdgeFacePtr> extrudedFaces;
    
    if (!mesh || faces.empty()) {
        return extrudedFaces;
    }
    
    // Step 1: Collect all vertices that will be moved
    std::unordered_set<HalfEdgeVertexPtr> verticesToExtrude;
    std::unordered_map<HalfEdgeVertexPtr, HalfEdgeVertexPtr> vertexMapping;
    
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
                averageNormal += face->computeNormal();
                normalCount++;
            }
        }
        
        if (normalCount > 0) {
            averageNormal = glm::normalize(averageNormal);
            glm::vec3 newPosition = vertex->getPosition() + averageNormal * distance;
            auto newVertex = mesh->addVertex(newPosition);
            vertexMapping[vertex] = newVertex;
        }
    }
    
    // Step 3: Create new top faces from the extruded vertices
    for (const auto& face : faces) {
        if (!face) continue;
        
        auto vertices = face->getVertices();
        std::vector<HalfEdgeVertexPtr> newVertices;
        
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
            auto twin = edge->getTwin();
            bool isBoundaryEdge = !twin; // No twin means mesh boundary
            
            if (twin) {
                auto adjacentFace = twin->getFace();
                if (adjacentFace && std::find(faces.begin(), faces.end(), adjacentFace) != faces.end()) {
                    // Adjacent face is also being extruded, skip this edge for side face creation
                    continue;
                }
                isBoundaryEdge = true; // Adjacent face not extruded = boundary of extrusion
            }
            
            if (isBoundaryEdge) {
                auto v1 = edge->getOriginVertex();
                auto v2 = edge->getNext() ? edge->getNext()->getOriginVertex() : nullptr;
                
                if (v1 && v2) {
                    auto v1_new = vertexMapping[v1];
                    auto v2_new = vertexMapping[v2];
                    
                    if (v1_new && v2_new) {
                        // Create a quad face: v1 -> v2 -> v2_new -> v1_new
                        std::vector<HalfEdgeVertexPtr> quadVertices = {v1, v2, v2_new, v1_new};
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

std::vector<HalfEdgeFacePtr> insetFaces(HalfEdgeMeshPtr mesh, const std::vector<HalfEdgeFacePtr>& faces, float inset) {
    std::vector<HalfEdgeFacePtr> insetFaces;
    
    if (!mesh || faces.empty()) {
        return insetFaces;
    }
    
    for (const auto& face : faces) {
        if (!face) continue;
        
        // Get face vertices and calculate centroid
        auto vertices = face->getVertices();
        auto centroid = face->getCentroid();
        
        if (vertices.size() < 3) continue;
        
        // Create inset vertices by moving towards centroid
        std::vector<HalfEdgeVertexPtr> insetVertices;
        for (const auto& vertex : vertices) {
            glm::vec3 toCenter = glm::normalize(centroid - vertex->getPosition());
            glm::vec3 newPosition = vertex->getPosition() + toCenter * inset;
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
                std::vector<HalfEdgeVertexPtr> quadVertices = {v1, v2, v2_inset, v1_inset};
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

std::vector<HalfEdgeEdgePtr> bevelEdges(HalfEdgeMeshPtr mesh, const std::vector<HalfEdgeEdgePtr>& edges, float amount) {
    std::vector<HalfEdgeEdgePtr> beveledEdges;
    
    if (!mesh || edges.empty()) {
        return beveledEdges;
    }
    
    for (const auto& edge : edges) {
        if (!edge) continue;
        
        // Basic edge bevel implementation
        // This is a simplified version - real bevel would be more complex
        auto origin = edge->getOriginVertex();
        auto target = edge->getTargetVertex();
        
        if (!origin || !target) continue;
        
        // Calculate bevel direction (perpendicular to edge in face plane)
        auto edgeVector = edge->getVector();
        auto face = edge->getFace();
        
        if (face) {
            auto faceNormal = face->computeNormal();
            auto bevelDirection = glm::cross(edgeVector, faceNormal);
            
            // Create new vertices offset from original edge vertices
            glm::vec3 offset = glm::normalize(bevelDirection) * amount;
            auto newOrigin = mesh->addVertex(origin->getPosition() + offset);
            auto newTarget = mesh->addVertex(target->getPosition() + offset);
            
            // Create new edge
            auto newEdge = mesh->addEdge(newOrigin, newTarget);
            if (newEdge) {
                beveledEdges.push_back(newEdge);
            }
        }
    }
    
    qDebug() << "Beveled" << edges.size() << "edges with amount" << amount;
    return beveledEdges;
}

std::vector<HalfEdgeEdgePtr> getEdgeLoop(HalfEdgeEdgePtr startEdge) {
    std::vector<HalfEdgeEdgePtr> loop;
    
    if (!startEdge) {
        return loop;
    }
    
    auto current = startEdge;
    std::unordered_set<HalfEdgeEdgePtr> visited;
    
    do {
        if (visited.find(current) != visited.end()) {
            break; // Avoid infinite loops
        }
        
        visited.insert(current);
        loop.push_back(current);
        
        // Find next edge in loop
        // This is a simplified implementation
        // Real edge loop detection requires proper mesh topology analysis
        auto twin = current->getTwin();
        if (twin) {
            auto next = twin->getNext();
            if (next && next->getNext()) {
                current = next->getNext();
            } else {
                break;
            }
        } else {
            break;
        }
        
    } while (current && current != startEdge && loop.size() < 1000); // Safety limit
    
    return loop;
}

std::vector<HalfEdgeEdgePtr> getEdgeRing(HalfEdgeEdgePtr startEdge) {
    std::vector<HalfEdgeEdgePtr> ring;
    
    if (!startEdge) {
        return ring;
    }
    
    // Edge ring implementation
    // This would traverse edges that are parallel to the start edge
    ring.push_back(startEdge);
    
    qDebug() << "Edge ring detection simplified - needs proper implementation";
    return ring;
}

HalfEdgeMeshPtr catmullClarkSubdivide(HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return nullptr;
    }
    
    auto subdividedMesh = std::make_shared<HalfEdgeMesh>();
    
    // Catmull-Clark subdivision algorithm:
    // 1. Create face points (centroid of each face)
    // 2. Create edge points (average of edge endpoints and adjacent face points)
    // 3. Update original vertices (weighted average)
    // 4. Connect all points to create new faces
    
    std::unordered_map<HalfEdgeFacePtr, HalfEdgeVertexPtr> facePoints;
    std::unordered_map<HalfEdgeEdgePtr, HalfEdgeVertexPtr> edgePoints;
    
    // Step 1: Create face points
    for (const auto& face : mesh->getFaces()) {
        auto centroid = face->getCentroid();
        auto facePoint = subdividedMesh->addVertex(centroid);
        facePoints[face] = facePoint;
    }
    
    // Step 2: Create edge points
    for (const auto& edge : mesh->getEdges()) {
        if (edgePoints.find(edge) != edgePoints.end()) {
            continue; // Already processed
        }
        
        auto origin = edge->getOriginVertex();
        auto target = edge->getTargetVertex();
        auto twin = edge->getTwin();
        
        if (!origin || !target) continue;
        
        glm::vec3 edgePoint = (origin->getPosition() + target->getPosition()) * 0.5f;
        
        // Add face points if they exist
        if (auto face = edge->getFace()) {
            if (auto fp = facePoints[face]) {
                edgePoint += fp->getPosition();
            }
        }
        if (twin && twin->getFace()) {
            if (auto fp = facePoints[twin->getFace()]) {
                edgePoint += fp->getPosition();
            }
        }
        
        edgePoint *= 0.5f; // Average
        
        auto edgePointVertex = subdividedMesh->addVertex(edgePoint);
        edgePoints[edge] = edgePointVertex;
        if (twin) {
            edgePoints[twin] = edgePointVertex;
        }
    }
    
    // Step 3: Update original vertices (simplified)
    std::unordered_map<HalfEdgeVertexPtr, HalfEdgeVertexPtr> originalToNew;
    for (const auto& vertex : mesh->getVertices()) {
        // For now, just copy the vertex
        // Real Catmull-Clark would apply proper weighting
        auto newVertex = subdividedMesh->addVertex(vertex->getPosition());
        originalToNew[vertex] = newVertex;
    }
    
    // Step 4: Create new faces (simplified)
    for (const auto& face : mesh->getFaces()) {
        auto facePoint = facePoints[face];
        auto edges = face->getEdges();
        
        for (const auto& edge : edges) {
            auto edgePoint = edgePoints[edge];
            auto originVertex = originalToNew[edge->getOriginVertex()];
            auto nextEdge = edge->getNext();
            auto nextEdgePoint = nextEdge ? edgePoints[nextEdge] : nullptr;
            
            if (facePoint && edgePoint && originVertex && nextEdgePoint) {
                std::vector<HalfEdgeVertexPtr> quadVertices = {
                    originVertex, edgePoint, facePoint, nextEdgePoint
                };
                subdividedMesh->addFace(quadVertices);
            }
        }
    }
    
    subdividedMesh->updateNormals();
    return subdividedMesh;
}

HalfEdgeMeshPtr loopSubdivide(HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return nullptr;
    }
    
    // Loop subdivision is primarily for triangle meshes
    // This is a simplified implementation
    qDebug() << "Loop subdivision not yet implemented";
    return nullptr;
}

std::vector<std::vector<HalfEdgeVertexPtr>> findConnectedComponents(HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return {};
    }
    
    std::vector<std::vector<HalfEdgeVertexPtr>> components;
    std::unordered_set<HalfEdgeVertexPtr> visited;
    
    for (const auto& vertex : mesh->getVertices()) {
        if (visited.find(vertex) != visited.end()) {
            continue;
        }
        
        // BFS to find connected component
        std::vector<HalfEdgeVertexPtr> component;
        std::queue<HalfEdgeVertexPtr> queue;
        
        queue.push(vertex);
        visited.insert(vertex);
        
        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();
            component.push_back(current);
            
            // Add adjacent vertices
            auto adjacent = current->getAdjacentVertices();
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

std::vector<HalfEdgeEdgePtr> findNonManifoldEdges(HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return {};
    }
    
    std::vector<HalfEdgeEdgePtr> nonManifoldEdges;
    
    for (const auto& edge : mesh->getEdges()) {
        int faceCount = 0;
        if (edge->getFace()) faceCount++;
        if (auto twin = edge->getTwin()) {
            if (twin->getFace()) faceCount++;
        }
        
        // Non-manifold if more than 2 faces share this edge
        if (faceCount > 2) {
            nonManifoldEdges.push_back(edge);
        }
    }
    
    return nonManifoldEdges;
}

std::vector<HalfEdgeVertexPtr> findNonManifoldVertices(HalfEdgeMeshPtr mesh) {
    if (!mesh) {
        return {};
    }
    
    std::vector<HalfEdgeVertexPtr> nonManifoldVertices;
    
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
