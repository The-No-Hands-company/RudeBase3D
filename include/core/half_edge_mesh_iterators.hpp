//==============================================================================
// RudeBase3D Engine - half_edge_mesh_iterators.hpp
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

#include <vector>
#include <memory>
#include <iterator>
#include <glm/glm.hpp>
#include "core/half_edge_mesh.hpp"

namespace rude {

// Forward declarations
class HalfEdgeMesh;
using VertexPtr = ::std::shared_ptr<class HalfEdgeMesh::Vertex>;
using EdgePtr = ::std::shared_ptr<class HalfEdgeMesh::Edge>;
using FacePtr = ::std::shared_ptr<class HalfEdgeMesh::Face>;
using HalfEdgePtr = ::std::shared_ptr<class HalfEdgeMesh::HalfEdge>;

// Vertex Iterator
class HalfEdgeMesh::VertexIterator {
public:
    using iterator_category = ::std::forward_iterator_tag;
    using value_type = VertexPtr;
    using difference_type = ::std::ptrdiff_t;
    using pointer = VertexPtr*;
    using reference = VertexPtr&;

    VertexIterator(const ::std::vector<VertexPtr>& vertices, size_t index = 0)
        : vertices(vertices), index(index) {}

    reference operator*() { return vertices[index]; }
    pointer operator->() { return &vertices[index]; }
    
    VertexIterator& operator++() {
        ++index;
        return *this;
    }
    
    VertexIterator operator++(int) {
        VertexIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    bool operator==(const VertexIterator& other) const {
        return index == other.index;
    }
    
    bool operator!=(const VertexIterator& other) const {
        return !(*this == other);
    }

private:
    const ::std::vector<VertexPtr>& vertices;
    size_t index;
};

// Edge Iterator
class HalfEdgeMesh::EdgeIterator {
public:
    using iterator_category = ::std::forward_iterator_tag;
    using value_type = EdgePtr;
    using difference_type = ::std::ptrdiff_t;
    using pointer = EdgePtr*;
    using reference = EdgePtr&;

    EdgeIterator(const ::std::vector<EdgePtr>& edges, size_t index = 0)
        : edges(edges), index(index) {}

    reference operator*() { return edges[index]; }
    pointer operator->() { return &edges[index]; }
    
    EdgeIterator& operator++() {
        ++index;
        return *this;
    }
    
    EdgeIterator operator++(int) {
        EdgeIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    bool operator==(const EdgeIterator& other) const {
        return index == other.index;
    }
    
    bool operator!=(const EdgeIterator& other) const {
        return !(*this == other);
    }

private:
    const ::std::vector<EdgePtr>& edges;
    size_t index;
};

// Face Iterator
class HalfEdgeMesh::FaceIterator {
public:
    using iterator_category = ::std::forward_iterator_tag;
    using value_type = FacePtr;
    using difference_type = ::std::ptrdiff_t;
    using pointer = FacePtr*;
    using reference = FacePtr&;

    FaceIterator(const ::std::vector<FacePtr>& faces, size_t index = 0)
        : faces(faces), index(index) {}

    reference operator*() { return faces[index]; }
    pointer operator->() { return &faces[index]; }
    
    FaceIterator& operator++() {
        ++index;
        return *this;
    }
    
    FaceIterator operator++(int) {
        FaceIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    bool operator==(const FaceIterator& other) const {
        return index == other.index;
    }
    
    bool operator!=(const FaceIterator& other) const {
        return !(*this == other);
    }

private:
    const ::std::vector<FacePtr>& faces;
    size_t index;
};

// HalfEdge Iterator
class HalfEdgeMesh::HalfEdgeIterator {
public:
    using iterator_category = ::std::forward_iterator_tag;
    using value_type = HalfEdgePtr;
    using difference_type = ::std::ptrdiff_t;
    using pointer = HalfEdgePtr*;
    using reference = HalfEdgePtr&;

    HalfEdgeIterator(const ::std::vector<HalfEdgePtr>& halfEdges, size_t index = 0)
        : halfEdges(halfEdges), index(index) {}

    reference operator*() { return halfEdges[index]; }
    pointer operator->() { return &halfEdges[index]; }
    
    HalfEdgeIterator& operator++() {
        ++index;
        return *this;
    }
    
    HalfEdgeIterator operator++(int) {
        HalfEdgeIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    bool operator==(const HalfEdgeIterator& other) const {
        return index == other.index;
    }
    
    bool operator!=(const HalfEdgeIterator& other) const {
        return !(*this == other);
    }

private:
    const ::std::vector<HalfEdgePtr>& halfEdges;
    size_t index;
};

} // namespace rude 