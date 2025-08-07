#include "core/mesh_forward.hpp"
#include "core/mesh_elements.hpp"
#include "core/mesh_iterator.hpp"
#include <vector>
#include <stdexcept>

namespace rude {

// VertexIterator implementation
VertexIterator::VertexIterator(const std::vector<VertexPtr>& vertices, size_t index)
    : vertices(vertices), index(index) {}

VertexIterator::const_reference VertexIterator::operator*() const {
    return vertices[index];
}

VertexIterator::const_pointer VertexIterator::operator->() const {
    return &vertices[index];
}

VertexIterator& VertexIterator::operator++() {
    ++index;
    return *this;
}

VertexIterator VertexIterator::operator++(int) {
    VertexIterator tmp = *this;
    ++index;
    return tmp;
}

bool VertexIterator::operator==(const Base& other) const {
    const VertexIterator* otherIter = dynamic_cast<const VertexIterator*>(&other);
    return otherIter && index == otherIter->index && &vertices == &otherIter->vertices;
}

bool VertexIterator::operator!=(const Base& other) const {
    return !(*this == other);
}

// EdgeIterator implementation
EdgeIterator::EdgeIterator(const std::vector<EdgePtr>& edges, size_t index)
    : edges(edges), index(index) {}

EdgeIterator::const_reference EdgeIterator::operator*() const {
    return edges[index];
}

EdgeIterator::const_pointer EdgeIterator::operator->() const {
    return &edges[index];
}

EdgeIterator& EdgeIterator::operator++() {
    ++index;
    return *this;
}

EdgeIterator EdgeIterator::operator++(int) {
    EdgeIterator tmp = *this;
    ++index;
    return tmp;
}

bool EdgeIterator::operator==(const Base& other) const {
    const EdgeIterator* otherIter = dynamic_cast<const EdgeIterator*>(&other);
    return otherIter && index == otherIter->index && &edges == &otherIter->edges;
}

bool EdgeIterator::operator!=(const Base& other) const {
    return !(*this == other);
}

// FaceIterator implementation
FaceIterator::FaceIterator(const std::vector<FacePtr>& faces, size_t index)
    : faces(faces), index(index) {}

FaceIterator::const_reference FaceIterator::operator*() const {
    return faces[index];
}

FaceIterator::const_pointer FaceIterator::operator->() const {
    return &faces[index];
}

FaceIterator& FaceIterator::operator++() {
    ++index;
    return *this;
}

FaceIterator FaceIterator::operator++(int) {
    FaceIterator tmp = *this;
    ++index;
    return tmp;
}

bool FaceIterator::operator==(const Base& other) const {
    const FaceIterator* otherIter = dynamic_cast<const FaceIterator*>(&other);
    return otherIter && index == otherIter->index && &faces == &otherIter->faces;
}

bool FaceIterator::operator!=(const Base& other) const {
    return !(*this == other);
}

// HalfEdgeIterator implementation
HalfEdgeIterator::HalfEdgeIterator(const std::vector<HalfEdgePtr>& halfEdges, size_t index)
    : halfEdges(halfEdges), index(index) {}

HalfEdgeIterator::const_reference HalfEdgeIterator::operator*() const {
    return halfEdges[index];
}

HalfEdgeIterator::const_pointer HalfEdgeIterator::operator->() const {
    return &halfEdges[index];
}

HalfEdgeIterator& HalfEdgeIterator::operator++() {
    ++index;
    return *this;
}

HalfEdgeIterator HalfEdgeIterator::operator++(int) {
    HalfEdgeIterator tmp = *this;
    ++index;
    return tmp;
}

bool HalfEdgeIterator::operator==(const Base& other) const {
    const HalfEdgeIterator* otherIter = dynamic_cast<const HalfEdgeIterator*>(&other);
    return otherIter && index == otherIter->index && &halfEdges == &otherIter->halfEdges;
}

bool HalfEdgeIterator::operator!=(const Base& other) const {
    return !(*this == other);
}

} // namespace rude 
