#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace rude {

// Forward declarations
class Vertex;
class Edge;
class Face;
class HalfEdge;

class Mesh;
class HalfEdgeMesh;

// Type aliases
using VertexPtr = std::shared_ptr<Vertex>;
using EdgePtr = std::shared_ptr<Edge>;
using FacePtr = std::shared_ptr<Face>;
using HalfEdgePtr = std::shared_ptr<HalfEdge>;

using MeshPtr = std::shared_ptr<Mesh>;
using HalfEdgeMeshPtr = std::shared_ptr<HalfEdgeMesh>;

// Common types
using Vec3 = glm::vec3;
using Vec2 = glm::vec2;

// Iterator type declarations
template<typename T, typename Derived>
class MeshIterator;

class VertexIterator;
class EdgeIterator;
class FaceIterator;
class HalfEdgeIterator;

} // namespace rude 