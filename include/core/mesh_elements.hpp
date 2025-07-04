#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "core/mesh_forward.hpp"

namespace rude {

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
};

class Edge {
public:
    Edge();
    HalfEdgePtr halfEdge;  // One of the half-edges of this edge

    // Modified getHalfEdge to accept an index
    HalfEdgePtr getHalfEdge(int index = 0) const { return halfEdge; }

    // Modified setHalfEdge to accept two arguments
    void setHalfEdge(int index, HalfEdgePtr he) {
        if (index == 0) {
            halfEdge = he;
        }
        // You can add logic for index 1 if needed
    }
};

class Face {
public:
    Face();
    HalfEdgePtr halfEdge;  // One of the half-edges of this face

    // Added getter and setter for half-edge
    HalfEdgePtr getHalfEdge() const { return halfEdge; }
    void setHalfEdge(HalfEdgePtr he) { halfEdge = he; }
};

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
    void setVertex(VertexPtr newVertex) {
        vertex = newVertex;
    }

    void setTwin(HalfEdgePtr newTwin) {
        twin = newTwin;
    }

    void setEdge(EdgePtr newEdge) {
        edge = newEdge;
    }

    void setFace(FacePtr newFace) {
        face = newFace;
    }

    void setNext(HalfEdgePtr newNext) {
        next = newNext;
    }

    void setPrev(HalfEdgePtr newPrev) {
        prev = newPrev;
    }
};

} // namespace rude 