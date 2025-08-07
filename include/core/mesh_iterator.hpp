//==============================================================================
// RudeBase3D Engine - mesh_iterator.hpp
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

#include <iterator>
#include <memory>
#include <vector>
#include "core/mesh_forward.hpp"

namespace rude {

// Base iterator interface
template<typename T, typename Derived>
class MeshIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T*;
    using const_reference = const T&;

    virtual ~MeshIterator() = default;
    virtual const_reference operator*() const = 0;
    virtual const_pointer operator->() const = 0;
    virtual Derived& operator++() = 0;
    virtual Derived operator++(int) = 0;
    virtual bool operator==(const MeshIterator& other) const = 0;
    virtual bool operator!=(const MeshIterator& other) const = 0;
};

// Concrete iterator implementations
class VertexIterator : public MeshIterator<VertexPtr, VertexIterator> {
public:
    using Base = MeshIterator<VertexPtr, VertexIterator>;
    using Base::const_reference;
    using Base::const_pointer;

    VertexIterator(const std::vector<VertexPtr>& vertices, size_t index = 0);
    const_reference operator*() const override;
    const_pointer operator->() const override;
    VertexIterator& operator++() override;
    VertexIterator operator++(int) override;
    bool operator==(const Base& other) const override;
    bool operator!=(const Base& other) const override;

private:
    const std::vector<VertexPtr>& vertices;
    size_t index;
};

class EdgeIterator : public MeshIterator<EdgePtr, EdgeIterator> {
public:
    using Base = MeshIterator<EdgePtr, EdgeIterator>;
    using Base::const_reference;
    using Base::const_pointer;

    EdgeIterator(const std::vector<EdgePtr>& edges, size_t index = 0);
    const_reference operator*() const override;
    const_pointer operator->() const override;
    EdgeIterator& operator++() override;
    EdgeIterator operator++(int) override;
    bool operator==(const Base& other) const override;
    bool operator!=(const Base& other) const override;

private:
    const std::vector<EdgePtr>& edges;
    size_t index;
};

class FaceIterator : public MeshIterator<FacePtr, FaceIterator> {
public:
    using Base = MeshIterator<FacePtr, FaceIterator>;
    using Base::const_reference;
    using Base::const_pointer;

    FaceIterator(const std::vector<FacePtr>& faces, size_t index = 0);
    const_reference operator*() const override;
    const_pointer operator->() const override;
    FaceIterator& operator++() override;
    FaceIterator operator++(int) override;
    bool operator==(const Base& other) const override;
    bool operator!=(const Base& other) const override;

private:
    const std::vector<FacePtr>& faces;
    size_t index;
};

class HalfEdgeIterator : public MeshIterator<HalfEdgePtr, HalfEdgeIterator> {
public:
    using Base = MeshIterator<HalfEdgePtr, HalfEdgeIterator>;
    using Base::const_reference;
    using Base::const_pointer;

    HalfEdgeIterator(const std::vector<HalfEdgePtr>& halfEdges, size_t index = 0);
    const_reference operator*() const override;
    const_pointer operator->() const override;
    HalfEdgeIterator& operator++() override;
    HalfEdgeIterator operator++(int) override;
    bool operator==(const Base& other) const override;
    bool operator!=(const Base& other) const override;

private:
    const std::vector<HalfEdgePtr>& halfEdges;
    size_t index;
};

} // namespace rude 