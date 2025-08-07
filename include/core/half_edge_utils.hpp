//==============================================================================
// RudeBase3D Engine - half_edge_utils.hpp
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

#include "core/half_edge_mesh.hpp"
#include "core/mesh_elements.hpp"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace rude {
namespace HalfEdgeUtils {
    // Edge operations
    ::std::pair<EdgePtr, EdgePtr> splitEdge(::std::shared_ptr<HalfEdgeMesh> mesh, EdgePtr edge, const glm::vec3& position);
    bool collapseEdge(::std::shared_ptr<HalfEdgeMesh> mesh, EdgePtr edge);
    bool flipEdge(EdgePtr edge);

    // Face operations
    ::std::vector<FacePtr> extrudeFaces(::std::shared_ptr<HalfEdgeMesh> mesh, const ::std::vector<FacePtr>& faces, float distance);
    ::std::vector<FacePtr> insetFaces(::std::shared_ptr<HalfEdgeMesh> mesh, const ::std::vector<FacePtr>& faces, float inset);
    ::std::vector<EdgePtr> bevelEdges(::std::shared_ptr<HalfEdgeMesh> mesh, const ::std::vector<EdgePtr>& edges, float amount);

    // Loop operations
    ::std::vector<EdgePtr> getEdgeLoop(EdgePtr startEdge);
    ::std::vector<EdgePtr> getEdgeRing(EdgePtr startEdge);

    // Subdivision
    ::std::shared_ptr<HalfEdgeMesh> catmullClarkSubdivide(::std::shared_ptr<HalfEdgeMesh> mesh);
    ::std::shared_ptr<HalfEdgeMesh> loopSubdivide(::std::shared_ptr<HalfEdgeMesh> mesh);

    // Mesh analysis
    ::std::vector<::std::vector<VertexPtr>> findConnectedComponents(::std::shared_ptr<HalfEdgeMesh> mesh);
    ::std::vector<EdgePtr> findNonManifoldEdges(::std::shared_ptr<HalfEdgeMesh> mesh);
    ::std::vector<VertexPtr> findNonManifoldVertices(::std::shared_ptr<HalfEdgeMesh> mesh);
} // namespace HalfEdgeUtils
} // namespace rude 