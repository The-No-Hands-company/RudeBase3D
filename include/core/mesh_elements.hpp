#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "core/mesh_forward.hpp"

namespace rude {

// Move HalfEdge definition to the top
class HalfEdge {
public:
    HalfEdge();
    VertexPtr vertex;      // The vertex this half-edge points to
    FacePtr face;         // The face this half-edge belongs to
    HalfEdgePtr next;     // The next half-edge around the face
    HalfEdgePtr prev;     // The previous half-edge around the face
    HalfEdgePtr twin;     // The twin half-edge
    EdgePtr edge;         // The edge this half-edge belongs to

    // Added getter methods
    VertexPtr getVertex() const { return vertex; }
    FacePtr getFace() const { return face; }
    HalfEdgePtr getNext() const { return next; }
    HalfEdgePtr getPrev() const { return prev; }
    HalfEdgePtr getTwin() const { return twin; }
    EdgePtr getEdge() const { return edge; }

    // Added setter methods
    void setVertex(VertexPtr newVertex) { vertex = newVertex; }
    void setTwin(HalfEdgePtr newTwin) { twin = newTwin; }
    void setEdge(EdgePtr newEdge) { edge = newEdge; }
    void setFace(FacePtr newFace) { face = newFace; }
    void setNext(HalfEdgePtr newNext) { next = newNext; }
    void setPrev(HalfEdgePtr newPrev) { prev = newPrev; }
};

class Vertex {
public:
    Vertex(const Vec3& position = Vec3(0.0f));
    
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
    HalfEdgePtr halfEdge;  // One of the half-edges emanating from this vertex

    // Added getter and setter for outgoing half-edge
    HalfEdgePtr getOutgoingHalfEdge() const { return halfEdge; }
    void setOutgoingHalfEdge(HalfEdgePtr he) { halfEdge = he; }

    // New API for ECS/GLM mesh system
    Vec3 getPosition() const { return position; }
    std::vector<EdgePtr> getOutgoingEdges() const {
        std::vector<EdgePtr> result;
        auto he = halfEdge;
        if (!he) return result;
        auto start = he;
        do {
            if (he->edge) result.push_back(he->edge);
            he = he->twin ? he->twin->next : nullptr;
        } while (he && he != start);
        return result;
    }
    std::vector<FacePtr> getAdjacentFaces() const {
        std::vector<FacePtr> result;
        auto he = halfEdge;
        if (!he) return result;
        auto start = he;
        do {
            if (he->face) result.push_back(he->face);
            he = he->twin ? he->twin->next : nullptr;
        } while (he && he != start);
        return result;
    }
    bool isBoundary() const {
        auto he = halfEdge;
        if (!he) return true;
        auto start = he;
        do {
            if (!he->face) return true;
            he = he->twin ? he->twin->next : nullptr;
        } while (he && he != start);
        return false;
    }
};

class Edge {
public:
    Edge();
    HalfEdgePtr halfEdge;  // One of the half-edges of this edge

    // Modified getHalfEdge to accept an index
    HalfEdgePtr getHalfEdge(int index = 0) const { 
        if (index == 0) {
            return halfEdge;
        } else if (index == 1 && halfEdge && halfEdge->twin) {
            return halfEdge->twin;
        }
        return nullptr; // Invalid index or no twin
    }
    void setHalfEdge(int index, HalfEdgePtr he) {
        if (index == 0) {
            halfEdge = he;
        }
    }
    // New API
    bool isBoundary() const {
        if (!halfEdge) return true;
        if (!halfEdge->face) return true;
        if (!halfEdge->twin || !halfEdge->twin->face) return true;
        return false;
    }
};

class Face {
public:
    Face();
    HalfEdgePtr halfEdge;  // One of the half-edges of this face
    std::vector<VertexPtr> vertices;  // Vertices of this face
    Vec3 normal;  // Face normal (now glm::vec3)

    HalfEdgePtr getHalfEdge() const { return halfEdge; }
    void setHalfEdge(HalfEdgePtr he) { halfEdge = he; }

    // New API
    std::vector<VertexPtr> getVertices() const {
        std::vector<VertexPtr> result;
        auto he = halfEdge;
        if (!he) return result;
        auto start = he;
        do {
            if (he->vertex) result.push_back(he->vertex);
            he = he->next;
        } while (he && he != start);
        return result;
    }
    std::vector<EdgePtr> getEdges() const {
        std::vector<EdgePtr> result;
        auto he = halfEdge;
        if (!he) return result;
        auto start = he;
        do {
            if (he->edge) result.push_back(he->edge);
            he = he->next;
        } while (he && he != start);
        return result;
    }
    std::vector<FacePtr> getAdjacentFaces() const {
        std::vector<FacePtr> result;
        auto he = halfEdge;
        if (!he) return result;
        auto start = he;
        do {
            if (he->twin && he->twin->face) result.push_back(he->twin->face);
            he = he->next;
        } while (he && he != start);
        return result;
    }
};

} // namespace rude 