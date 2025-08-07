//==============================================================================
// RudeBase3D Engine - half_edge_mesh.cpp
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

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
    return VertexIterator(m_vertices, 0);
}

VertexIterator HalfEdgeMesh::vertices_end() const {
    return VertexIterator(m_vertices, m_vertices.size());
}

EdgeIterator HalfEdgeMesh::edges_begin() const {
    return EdgeIterator(m_edges, 0);
}

EdgeIterator HalfEdgeMesh::edges_end() const {
    return EdgeIterator(m_edges, m_edges.size());
}

FaceIterator HalfEdgeMesh::faces_begin() const {
    return FaceIterator(m_faces, 0);
}

FaceIterator HalfEdgeMesh::faces_end() const {
    return FaceIterator(m_faces, m_faces.size());
}

HalfEdgeIterator HalfEdgeMesh::half_edges_begin() const {
    return HalfEdgeIterator(m_halfEdges, 0);
}

HalfEdgeIterator HalfEdgeMesh::half_edges_end() const {
    return HalfEdgeIterator(m_halfEdges, m_halfEdges.size());
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
    m_vertices.clear();
    m_edges.clear();
    m_faces.clear();
    m_halfEdges.clear();
}

bool HalfEdgeMesh::isValid() const {
    // Check if all m_vertices have valid half-m_edges
    for (const auto& vertex : m_vertices) {
        if (!vertex->halfEdge) return false;
    }

    // Check if all m_edges have valid half-m_edges
    for (const auto& edge : m_edges) {
        if (!edge->halfEdge) return false;
    }

    // Check if all m_faces have valid half-m_edges
    for (const auto& face : m_faces) {
        if (!face->halfEdge) return false;
    }

    // Check if all half-m_edges have valid connections
    for (const auto& he : m_halfEdges) {
        if (!he->vertex || !he->edge) return false;
        if (!he->next || !he->prev) return false;
        if (!he->twin) return false;
    }

    return true;
}

VertexPtr HalfEdgeMesh::addVertex(const glm::vec3& position) {
    auto vertex = std::make_shared<Vertex>(position);
    m_vertices.push_back(vertex);
    return vertex;
}

void HalfEdgeMesh::removeVertex(VertexPtr vertex) {
    if (!vertex) return;
    
    // Remove all m_edges connected to this vertex
    auto adjacentEdges = getAdjacentEdges(vertex);
    for (const auto& edge : adjacentEdges) {
        removeEdge(edge);
    }
    
    // Remove the vertex
    m_vertices.erase(std::remove(m_vertices.begin(), m_vertices.end(), vertex), m_vertices.end());
}

EdgePtr HalfEdgeMesh::addEdge(VertexPtr v1, VertexPtr v2) {
    if (!v1 || !v2) return nullptr;
    
    auto edge = std::make_shared<Edge>();
    auto he1 = std::make_shared<HalfEdge>();
    auto he2 = std::make_shared<HalfEdge>();
    
    // Set up half-m_edges
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
    m_edges.push_back(edge);
    m_halfEdges.push_back(he1);
    m_halfEdges.push_back(he2);
    
    return edge;
}

void HalfEdgeMesh::removeEdge(EdgePtr edge) {
    if (!edge) return;
    
    // Remove associated half-m_edges
    auto he1 = edge->getHalfEdge(0);
    auto he2 = edge->getHalfEdge(1);
    
    if (he1) {
        m_halfEdges.erase(std::remove(m_halfEdges.begin(), m_halfEdges.end(), he1), m_halfEdges.end());
    }
    if (he2) {
        m_halfEdges.erase(std::remove(m_halfEdges.begin(), m_halfEdges.end(), he2), m_halfEdges.end());
    }
    
    // Remove the edge
    m_edges.erase(std::remove(m_edges.begin(), m_edges.end(), edge), m_edges.end());
}

FacePtr HalfEdgeMesh::addFace(const std::vector<VertexPtr>& m_vertices) {
    if (m_vertices.size() < 3) return nullptr;
    
    auto face = std::make_shared<Face>();
    std::vector<HalfEdgePtr> faceHalfEdges;
    
    // Create half-m_edges for the face
    for (size_t i = 0; i < m_vertices.size(); ++i) {
        auto he = std::make_shared<HalfEdge>();
        he->setVertex(m_vertices[i]);
        he->setFace(face);
        faceHalfEdges.push_back(he);
        m_halfEdges.push_back(he);
    }
    
    // Link half-m_edges
    for (size_t i = 0; i < faceHalfEdges.size(); ++i) {
        auto he = faceHalfEdges[i];
        he->setNext(faceHalfEdges[(i + 1) % faceHalfEdges.size()]);
        he->setPrev(faceHalfEdges[(i + faceHalfEdges.size() - 1) % faceHalfEdges.size()]);
    }
    
    // Set face's half-edge
    face->setHalfEdge(faceHalfEdges[0]);
    
    // Add face to mesh
    m_faces.push_back(face);
    
    return face;
}

void HalfEdgeMesh::removeFace(FacePtr face) {
    if (!face) return;
    
    // Remove associated half-m_edges
    auto he = face->getHalfEdge();
    if (he) {
        auto current = he;
        do {
            auto next = current->getNext();
            m_halfEdges.erase(std::remove(m_halfEdges.begin(), m_halfEdges.end(), current), m_halfEdges.end());
            current = next;
        } while (current != he);
    }
    
    // Remove the face
    m_faces.erase(std::remove(m_faces.begin(), m_faces.end(), face), m_faces.end());
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
    m_vertices.push_back(vertex);
    return vertex;
}

EdgePtr HalfEdgeMesh::createEdge(VertexPtr v1, VertexPtr v2) {
    auto edge = std::make_shared<Edge>();
    m_edges.push_back(edge);
    return edge;
}

FacePtr HalfEdgeMesh::createFace(const std::vector<VertexPtr>& m_vertices) {
    auto face = std::make_shared<Face>();
    m_faces.push_back(face);
    return face;
}

void HalfEdgeMesh::deleteVertex(VertexPtr vertex) {
    auto it = std::find(m_vertices.begin(), m_vertices.end(), vertex);
    if (it != m_vertices.end()) {
        m_vertices.erase(it);
    }
}

void HalfEdgeMesh::deleteEdge(EdgePtr edge) {
    auto it = std::find(m_edges.begin(), m_edges.end(), edge);
    if (it != m_edges.end()) {
        m_edges.erase(it);
    }
}

void HalfEdgeMesh::deleteFace(FacePtr face) {
    auto it = std::find(m_faces.begin(), m_faces.end(), face);
    if (it != m_faces.end()) {
        m_faces.erase(it);
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

    // Create new m_edges
    EdgePtr newEdge1 = std::make_shared<Edge>();
    EdgePtr newEdge2 = std::make_shared<Edge>();
    
    // Create new half-m_edges
    HalfEdgePtr newHE1 = std::make_shared<HalfEdge>();
    HalfEdgePtr newHE2 = std::make_shared<HalfEdge>();
    HalfEdgePtr newHE3 = std::make_shared<HalfEdge>();
    HalfEdgePtr newHE4 = std::make_shared<HalfEdge>();

    // Set up new half-m_edges for first new edge (v1 -> newVertex)
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

    // Set up new half-m_edges for second new edge (newVertex -> v2)
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

    // Update face half-m_edges if needed
    if (he1->face && he1->face->halfEdge == he1) {
        he1->face->setHalfEdge(newHE1);
    }
    if (he2->face && he2->face->halfEdge == he2) {
        he2->face->setHalfEdge(newHE2);
    }

    // Set up m_edges
    newEdge1->halfEdge = newHE1;
    newEdge2->halfEdge = newHE3;

    // Add new elements to mesh
    m_edges.push_back(newEdge1);
    m_edges.push_back(newEdge2);
    m_halfEdges.push_back(newHE1);
    m_halfEdges.push_back(newHE2);
    m_halfEdges.push_back(newHE3);
    m_halfEdges.push_back(newHE4);

    // Remove old edge and half-m_edges
    m_halfEdges.erase(std::remove(m_halfEdges.begin(), m_halfEdges.end(), he1), m_halfEdges.end());
    m_halfEdges.erase(std::remove(m_halfEdges.begin(), m_halfEdges.end(), he2), m_halfEdges.end());
    m_edges.erase(std::remove(m_edges.begin(), m_edges.end(), edge), m_edges.end());
}

void HalfEdgeMesh::mergeVertices(VertexPtr v1, VertexPtr v2) {
    if (!v1 || !v2 || v1 == v2) return;

    // Get all half-m_edges connected to v2
    std::vector<HalfEdgePtr> v2HalfEdges = getAdjacentHalfEdges(v2);

    // Redirect all half-m_edges pointing to v2 to point to v1
    for (auto he : v2HalfEdges) {
        if (he->vertex == v2) {
            he->setVertex(v1);
        }
        // Update twin half-m_edges that point to v2
        if (he->twin && he->twin->vertex == v2) {
            he->twin->setVertex(v1);
        }
    }

    // Update v1's outgoing half-edge if needed
    if (!v1->halfEdge && !v2HalfEdges.empty()) {
        v1->setOutgoingHalfEdge(v2HalfEdges[0]);
    }

    // Remove v2 from m_vertices
    m_vertices.erase(std::remove(m_vertices.begin(), m_vertices.end(), v2), m_vertices.end());
}

void HalfEdgeMesh::flipEdge(EdgePtr edge) {
    if (!edge || !edge->halfEdge) return;

    HalfEdgePtr he1 = edge->halfEdge;
    HalfEdgePtr he2 = he1->twin;

    // Check if edge can be flipped (must be between two triangular m_faces)
    if (!he1->face || !he2->face) return;

    // Get the four m_vertices of the quadrilateral
    VertexPtr v1 = he1->prev->vertex;  // Bottom left
    VertexPtr v2 = he1->vertex;        // Bottom right
    VertexPtr v3 = he1->next->vertex;  // Top right
    VertexPtr v4 = he2->next->vertex;  // Top left

    // Store the neighboring half-m_edges
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

    // Update neighboring half-m_edges
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
    // Check if all m_vertices have valid half-m_edges
    for (const auto& vertex : m_vertices) {
        if (!vertex->halfEdge) return false;
        
        // Check if the outgoing half-edge actually points away from this vertex
        if (vertex->halfEdge->twin->vertex != vertex) return false;
    }

    // Check if all m_edges have valid half-m_edges
    for (const auto& edge : m_edges) {
        if (!edge->halfEdge) return false;
        if (!edge->halfEdge->twin) return false;
        if (edge->halfEdge->twin->twin != edge->halfEdge) return false;
    }

    // Check if all m_faces have valid half-m_edges
    for (const auto& face : m_faces) {
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
        
        if (count < 3) return false; // Face must have at least 3 m_vertices
    }

    // Check if all half-m_edges have valid connections
    for (const auto& he : m_halfEdges) {
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
    for (auto vertex : m_vertices) {
        vertex->normal = Vec3(0.0f);
    }

    // Accumulate face normals at m_vertices
    for (auto face : m_faces) {
        auto he = face->halfEdge;
        if (!he) continue;

        // Get face m_vertices
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

            // Accumulate at m_vertices
            current = he;
            do {
                current->vertex->normal += faceNormal;
                current = current->next;
            } while (current != he);
        }
    }

    // Normalize accumulated normals
    for (auto vertex : m_vertices) {
        if (glm::length(vertex->normal) > 0.0f) {
            vertex->normal = glm::normalize(vertex->normal);
        } else {
            vertex->normal = Vec3(0.0f, 1.0f, 0.0f); // Default up
        }
    }
}

// Getter functions for collections
std::vector<std::shared_ptr<Vertex>> HalfEdgeMesh::getVertices() const {
    return m_vertices;
}

std::vector<std::shared_ptr<Edge>> HalfEdgeMesh::getEdges() const {
    return m_edges;
}

std::vector<std::shared_ptr<Face>> HalfEdgeMesh::getFaces() const {
    return m_faces;
}

bool HalfEdgeMesh::isEmpty() const {
    return m_vertices.empty() && m_edges.empty() && m_faces.empty();
}

void HalfEdgeMesh::updateNormals() {
    updateVertexNormals();
}

} // namespace rude
