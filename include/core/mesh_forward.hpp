//==============================================================================
// RudeBase3D Engine - mesh_forward.hpp
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
using VertexPtr = ::std::shared_ptr<Vertex>;
using EdgePtr = ::std::shared_ptr<Edge>;
using FacePtr = ::std::shared_ptr<Face>;
using HalfEdgePtr = ::std::shared_ptr<HalfEdge>;

using MeshPtr = ::std::shared_ptr<Mesh>;
using HalfEdgeMeshPtr = ::std::shared_ptr<HalfEdgeMesh>;

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