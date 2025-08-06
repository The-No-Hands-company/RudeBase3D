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