#include "core/mesh_forward.hpp"
#include "core/mesh_elements.hpp"
#include "core/mesh_iterator.hpp"
#include "core/half_edge_mesh.hpp"
#include <stdexcept>
#include <algorithm>

namespace rude {

// Constructor implementations
HalfEdgeMesh::HalfEdgeMesh() {}

// Iterator method implementations
VertexIterator HalfEdgeMesh::vertices_begin() const {
    return VertexIterator(vertices, 0);
}

VertexIterator HalfEdgeMesh::vertices_end() const {
    return VertexIterator(vertices, vertices.size());
}

EdgeIterator HalfEdgeMesh::edges_begin() const {
    return EdgeIterator(edges, 0);
}

EdgeIterator HalfEdgeMesh::edges_end() const {
    return EdgeIterator(edges, edges.size());
}

FaceIterator HalfEdgeMesh::faces_begin() const {
    return FaceIterator(faces, 0);
}

FaceIterator HalfEdgeMesh::faces_end() const {
    return FaceIterator(faces, faces.size());
}

HalfEdgeIterator HalfEdgeMesh::half_edges_begin() const {
    return HalfEdgeIterator(halfEdges, 0);
}

HalfEdgeIterator HalfEdgeMesh::half_edges_end() const {
    return HalfEdgeIterator(halfEdges, halfEdges.size());
}

// Vertex implementation
Vertex::Vertex(const Vec3& position)
    : position(position), normal(Vec3(0.0f, 1.0f, 0.0f)), texCoord(Vec2(0.0f)), halfEdge(nullptr) {}

// Edge implementation
Edge::Edge() : halfEdge(nullptr) {}

// Face implementation
Face::Face() : halfEdge(nullptr) {}

// HalfEdge implementation
HalfEdge::HalfEdge()
    : vertex(nullptr), face(nullptr), next(nullptr), prev(nullptr), twin(nullptr), edge(nullptr) {}

// Mesh operations
void HalfEdgeMesh::clear() {
    vertices.clear();
    edges.clear();
    faces.clear();
    halfEdges.clear();
}

bool HalfEdgeMesh::isValid() const {
    // Check if all vertices have valid half-edges
    for (const auto& vertex : vertices) {
        if (!vertex->halfEdge) return false;
    }

    // Check if all edges have valid half-edges
    for (const auto& edge : edges) {
        if (!edge->halfEdge) return false;
    }

    // Check if all faces have valid half-edges
    for (const auto& face : faces) {
        if (!face->halfEdge) return false;
    }

    // Check if all half-edges have valid connections
    for (const auto& he : halfEdges) {
        if (!he->vertex || !he->edge) return false;
        if (!he->next || !he->prev) return false;
        if (!he->twin) return false;
    }

    return true;
}

VertexPtr HalfEdgeMesh::addVertex(const glm::vec3& position) {
    auto vertex = std::make_shared<Vertex>(position);
    vertices.push_back(vertex);
    return vertex;
}

void HalfEdgeMesh::removeVertex(VertexPtr vertex) {
    if (!vertex) return;
    
    // Remove all edges connected to this vertex
    auto adjacentEdges = getAdjacentEdges(vertex);
    for (const auto& edge : adjacentEdges) {
        removeEdge(edge);
    }
    
    // Remove the vertex
    vertices.erase(std::remove(vertices.begin(), vertices.end(), vertex), vertices.end());
}

EdgePtr HalfEdgeMesh::addEdge(VertexPtr v1, VertexPtr v2) {
    if (!v1 || !v2) return nullptr;
    
    auto edge = std::make_shared<Edge>();
    auto he1 = std::make_shared<HalfEdge>();
    auto he2 = std::make_shared<HalfEdge>();
    
    // Set up half-edges
    he1->setVertex(v2);
    he1->setTwin(he2);
    he1->setEdge(edge);
    
    he2->setVertex(v1);
    he2->setTwin(he1);
    he2->setEdge(edge);
    
    // Set up edge
    edge->setHalfEdge(0, he1);
    edge->setHalfEdge(1, he2);
    
    // Update vertex outgoing half-edge if needed
    if (!v1->getOutgoingHalfEdge()) {
        v1->setOutgoingHalfEdge(he1);
    }
    if (!v2->getOutgoingHalfEdge()) {
        v2->setOutgoingHalfEdge(he2);
    }
    
    // Add to mesh
    edges.push_back(edge);
    halfEdges.push_back(he1);
    halfEdges.push_back(he2);
    
    return edge;
}

void HalfEdgeMesh::removeEdge(EdgePtr edge) {
    if (!edge) return;
    
    // Remove associated half-edges
    auto he1 = edge->getHalfEdge(0);
    auto he2 = edge->getHalfEdge(1);
    
    if (he1) {
        halfEdges.erase(std::remove(halfEdges.begin(), halfEdges.end(), he1), halfEdges.end());
    }
    if (he2) {
        halfEdges.erase(std::remove(halfEdges.begin(), halfEdges.end(), he2), halfEdges.end());
    }
    
    // Remove the edge
    edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
}

FacePtr HalfEdgeMesh::addFace(const std::vector<VertexPtr>& vertices) {
    if (vertices.size() < 3) return nullptr;
    
    auto face = std::make_shared<Face>();
    std::vector<HalfEdgePtr> faceHalfEdges;
    
    // Create half-edges for the face
    for (size_t i = 0; i < vertices.size(); ++i) {
        auto he = std::make_shared<HalfEdge>();
        he->setVertex(vertices[i]);
        he->setFace(face);
        faceHalfEdges.push_back(he);
        halfEdges.push_back(he);
    }
    
    // Link half-edges
    for (size_t i = 0; i < faceHalfEdges.size(); ++i) {
        auto he = faceHalfEdges[i];
        he->setNext(faceHalfEdges[(i + 1) % faceHalfEdges.size()]);
        he->setPrev(faceHalfEdges[(i + faceHalfEdges.size() - 1) % faceHalfEdges.size()]);
    }
    
    // Set face's half-edge
    face->setHalfEdge(faceHalfEdges[0]);
    
    // Add face to mesh
    faces.push_back(face);
    
    return face;
}

void HalfEdgeMesh::removeFace(FacePtr face) {
    if (!face) return;
    
    // Remove associated half-edges
    auto he = face->getHalfEdge();
    if (he) {
        auto current = he;
        do {
            auto next = current->getNext();
            halfEdges.erase(std::remove(halfEdges.begin(), halfEdges.end(), current), halfEdges.end());
            current = next;
        } while (current != he);
    }
    
    // Remove the face
    faces.erase(std::remove(faces.begin(), faces.end(), face), faces.end());
}

std::vector<FacePtr> HalfEdgeMesh::getAdjacentFaces(VertexPtr vertex) const {
    std::vector<FacePtr> result;
    if (!vertex) return result;
    
    auto he = vertex->getOutgoingHalfEdge();
    if (!he) return result;
    
    auto current = he;
    do {
        if (current->getFace()) {
            result.push_back(current->getFace());
        }
        current = current->getTwin()->getNext();
    } while (current != he);
    
    return result;
}

std::vector<EdgePtr> HalfEdgeMesh::getAdjacentEdges(VertexPtr vertex) const {
    std::vector<EdgePtr> result;
    if (!vertex) return result;
    
    auto he = vertex->getOutgoingHalfEdge();
    if (!he) return result;
    
    auto current = he;
    do {
        result.push_back(current->getEdge());
        current = current->getTwin()->getNext();
    } while (current != he);
    
    return result;
}

std::vector<VertexPtr> HalfEdgeMesh::getAdjacentVertices(VertexPtr vertex) const {
    std::vector<VertexPtr> result;
    if (!vertex) return result;
    
    auto he = vertex->getOutgoingHalfEdge();
    if (!he) return result;
    
    auto current = he;
    do {
        result.push_back(current->getVertex());
        current = current->getTwin()->getNext();
    } while (current != he);
    
    return result;
}

// Mesh operations
VertexPtr HalfEdgeMesh::createVertex(const Vec3& position) {
    auto vertex = std::make_shared<Vertex>(position);
    vertices.push_back(vertex);
    return vertex;
}

EdgePtr HalfEdgeMesh::createEdge(VertexPtr v1, VertexPtr v2) {
    auto edge = std::make_shared<Edge>();
    edges.push_back(edge);
    return edge;
}

FacePtr HalfEdgeMesh::createFace(const std::vector<VertexPtr>& vertices) {
    auto face = std::make_shared<Face>();
    faces.push_back(face);
    return face;
}

void HalfEdgeMesh::deleteVertex(VertexPtr vertex) {
    auto it = std::find(vertices.begin(), vertices.end(), vertex);
    if (it != vertices.end()) {
        vertices.erase(it);
    }
}

void HalfEdgeMesh::deleteEdge(EdgePtr edge) {
    auto it = std::find(edges.begin(), edges.end(), edge);
    if (it != edges.end()) {
        edges.erase(it);
    }
}

void HalfEdgeMesh::deleteFace(FacePtr face) {
    auto it = std::find(faces.begin(), faces.end(), face);
    if (it != faces.end()) {
        faces.erase(it);
    }
}

// Topology queries
std::vector<HalfEdgePtr> HalfEdgeMesh::getAdjacentHalfEdges(VertexPtr vertex) const {
    std::vector<HalfEdgePtr> adjacent;
    if (!vertex || !vertex->halfEdge) return adjacent;

    HalfEdgePtr he = vertex->halfEdge;
    do {
        adjacent.push_back(he);
        he = he->twin->next;
    } while (he != vertex->halfEdge);

    return adjacent;
}

// Advanced mesh operations

void HalfEdgeMesh::splitEdge(EdgePtr edge, const Vec3& position) {
    if (!edge || !edge->halfEdge) return;

    HalfEdgePtr he1 = edge->halfEdge;
    HalfEdgePtr he2 = he1->twin;

    VertexPtr v1 = he2->vertex;  // Start vertex
    VertexPtr v2 = he1->vertex;  // End vertex

    // Create new vertex at split position
    VertexPtr newVertex = createVertex(position);

    // Create new edges
    EdgePtr newEdge1 = std::make_shared<Edge>();
    EdgePtr newEdge2 = std::make_shared<Edge>();
    
    // Create new half-edges
    HalfEdgePtr newHE1 = std::make_shared<HalfEdge>();
    HalfEdgePtr newHE2 = std::make_shared<HalfEdge>();
    HalfEdgePtr newHE3 = std::make_shared<HalfEdge>();
    HalfEdgePtr newHE4 = std::make_shared<HalfEdge>();

    // Set up new half-edges for first new edge (v1 -> newVertex)
    newHE1->setVertex(newVertex);
    newHE1->setEdge(newEdge1);
    newHE1->setTwin(newHE2);
    newHE1->setFace(he1->face);
    newHE1->setNext(he1->next);
    newHE1->setPrev(he1->prev);

    newHE2->setVertex(v1);
    newHE2->setEdge(newEdge1);
    newHE2->setTwin(newHE1);
    newHE2->setFace(he2->face);
    newHE2->setNext(he2->next);
    newHE2->setPrev(he2->prev);

    // Set up new half-edges for second new edge (newVertex -> v2)
    newHE3->setVertex(v2);
    newHE3->setEdge(newEdge2);
    newHE3->setTwin(newHE4);
    newHE3->setFace(he1->face);
    newHE3->setNext(newHE1->next);
    newHE3->setPrev(newHE1);

    newHE4->setVertex(newVertex);
    newHE4->setEdge(newEdge2);
    newHE4->setTwin(newHE3);
    newHE4->setFace(he2->face);
    newHE4->setNext(newHE2->next);
    newHE4->setPrev(newHE2);

    // Update connections
    newHE1->setNext(newHE3);
    newHE2->setNext(newHE4);
    
    if (he1->prev) he1->prev->setNext(newHE1);
    if (he1->next) he1->next->setPrev(newHE3);
    if (he2->prev) he2->prev->setNext(newHE2);
    if (he2->next) he2->next->setPrev(newHE4);

    // Set vertex outgoing half-edge
    newVertex->setOutgoingHalfEdge(newHE3);

    // Update face half-edges if needed
    if (he1->face && he1->face->halfEdge == he1) {
        he1->face->setHalfEdge(newHE1);
    }
    if (he2->face && he2->face->halfEdge == he2) {
        he2->face->setHalfEdge(newHE2);
    }

    // Set up edges
    newEdge1->halfEdge = newHE1;
    newEdge2->halfEdge = newHE3;

    // Add new elements to mesh
    edges.push_back(newEdge1);
    edges.push_back(newEdge2);
    halfEdges.push_back(newHE1);
    halfEdges.push_back(newHE2);
    halfEdges.push_back(newHE3);
    halfEdges.push_back(newHE4);

    // Remove old edge and half-edges
    halfEdges.erase(std::remove(halfEdges.begin(), halfEdges.end(), he1), halfEdges.end());
    halfEdges.erase(std::remove(halfEdges.begin(), halfEdges.end(), he2), halfEdges.end());
    edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
}

void HalfEdgeMesh::mergeVertices(VertexPtr v1, VertexPtr v2) {
    if (!v1 || !v2 || v1 == v2) return;

    // Get all half-edges connected to v2
    std::vector<HalfEdgePtr> v2HalfEdges = getAdjacentHalfEdges(v2);

    // Redirect all half-edges pointing to v2 to point to v1
    for (auto he : v2HalfEdges) {
        if (he->vertex == v2) {
            he->setVertex(v1);
        }
        // Update twin half-edges that point to v2
        if (he->twin && he->twin->vertex == v2) {
            he->twin->setVertex(v1);
        }
    }

    // Update v1's outgoing half-edge if needed
    if (!v1->halfEdge && !v2HalfEdges.empty()) {
        v1->setOutgoingHalfEdge(v2HalfEdges[0]);
    }

    // Remove v2 from vertices
    vertices.erase(std::remove(vertices.begin(), vertices.end(), v2), vertices.end());
}

void HalfEdgeMesh::flipEdge(EdgePtr edge) {
    if (!edge || !edge->halfEdge) return;

    HalfEdgePtr he1 = edge->halfEdge;
    HalfEdgePtr he2 = he1->twin;

    // Check if edge can be flipped (must be between two triangular faces)
    if (!he1->face || !he2->face) return;

    // Get the four vertices of the quadrilateral
    VertexPtr v1 = he1->prev->vertex;  // Bottom left
    VertexPtr v2 = he1->vertex;        // Bottom right
    VertexPtr v3 = he1->next->vertex;  // Top right
    VertexPtr v4 = he2->next->vertex;  // Top left

    // Store the neighboring half-edges
    HalfEdgePtr he1_prev = he1->prev;
    HalfEdgePtr he1_next = he1->next;
    HalfEdgePtr he2_prev = he2->prev;
    HalfEdgePtr he2_next = he2->next;

    // Update the flipped edge to connect v4 and v3
    he1->setVertex(v3);
    he2->setVertex(v4);

    // Update face connectivity
    he1->setNext(he2_next);
    he1->setPrev(he2_prev);
    he2->setNext(he1_next);
    he2->setPrev(he1_prev);

    // Update neighboring half-edges
    he1_prev->setNext(he2);
    he1_next->setPrev(he2);
    he2_prev->setNext(he1);
    he2_next->setPrev(he1);

    // Update face references
    he2_prev->setFace(he1->face);
    he2_next->setFace(he1->face);
    he1_prev->setFace(he2->face);
    he1_next->setFace(he2->face);
}

bool HalfEdgeMesh::isValidMesh() const {
    // Check if all vertices have valid half-edges
    for (const auto& vertex : vertices) {
        if (!vertex->halfEdge) return false;
        
        // Check if the outgoing half-edge actually points away from this vertex
        if (vertex->halfEdge->twin->vertex != vertex) return false;
    }

    // Check if all edges have valid half-edges
    for (const auto& edge : edges) {
        if (!edge->halfEdge) return false;
        if (!edge->halfEdge->twin) return false;
        if (edge->halfEdge->twin->twin != edge->halfEdge) return false;
    }

    // Check if all faces have valid half-edges
    for (const auto& face : faces) {
        if (!face->halfEdge) return false;
        
        // Check if face half-edge loop is valid
        auto he = face->halfEdge;
        auto current = he;
        int count = 0;
        do {
            if (!current->next || !current->prev) return false;
            if (current->face != face) return false;
            current = current->next;
            count++;
            if (count > 100) return false; // Prevent infinite loop
        } while (current != he);
        
        if (count < 3) return false; // Face must have at least 3 vertices
    }

    // Check if all half-edges have valid connections
    for (const auto& he : halfEdges) {
        if (!he->vertex || !he->edge) return false;
        if (!he->next || !he->prev) return false;
        if (!he->twin) return false;
        
        // Check twin relationship
        if (he->twin->twin != he) return false;
        
        // Check next/prev relationship
        if (he->next->prev != he) return false;
        if (he->prev->next != he) return false;
    }

    return true;
}

// Utility methods

EdgePtr HalfEdgeMesh::findEdge(VertexPtr v1, VertexPtr v2) const {
    if (!v1 || !v2) return nullptr;

    auto adjacentEdges = getAdjacentEdges(v1);
    for (auto edge : adjacentEdges) {
        HalfEdgePtr he = edge->halfEdge;
        if ((he->vertex == v2 && he->twin->vertex == v1) ||
            (he->vertex == v1 && he->twin->vertex == v2)) {
            return edge;
        }
    }
    return nullptr;
}

bool HalfEdgeMesh::areVerticesConnected(VertexPtr v1, VertexPtr v2) const {
    return findEdge(v1, v2) != nullptr;
}

void HalfEdgeMesh::updateVertexNormals() {
    // Reset all vertex normals
    for (auto vertex : vertices) {
        vertex->normal = Vec3(0.0f);
    }

    // Accumulate face normals at vertices
    for (auto face : faces) {
        auto he = face->halfEdge;
        if (!he) continue;

        // Get face vertices
        std::vector<Vec3> faceVertices;
        auto current = he;
        do {
            faceVertices.push_back(current->vertex->position);
            current = current->next;
        } while (current != he);

        if (faceVertices.size() >= 3) {
            // Compute face normal
            Vec3 faceNormal = glm::normalize(glm::cross(
                faceVertices[1] - faceVertices[0],
                faceVertices[2] - faceVertices[0]
            ));

            // Accumulate at vertices
            current = he;
            do {
                current->vertex->normal += faceNormal;
                current = current->next;
            } while (current != he);
        }
    }

    // Normalize accumulated normals
    for (auto vertex : vertices) {
        if (glm::length(vertex->normal) > 0.0f) {
            vertex->normal = glm::normalize(vertex->normal);
        } else {
            vertex->normal = Vec3(0.0f, 1.0f, 0.0f); // Default up
        }
    }
}

} // namespace rude