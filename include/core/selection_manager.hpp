//==============================================================================
// RudeBase3D Engine - selection_manager.hpp
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
#include <vector>
#include <set>
#include <map>
#include "core/mesh_forward.hpp"
#include <memory>
#include <glm/glm.hpp>
#include <QObject>
#include <QRect>

// Forward declarations
namespace rude {
    class Entity;
    class Scene;
    class SceneManager;
}

namespace rude {

enum class SelectionMode {
    Replace,    // Replace current selection
    Add,        // Add to current selection (Ctrl+click)
    Subtract,   // Remove from current selection (Ctrl+Shift+click)
    Toggle      // Toggle selection state (Shift+click)
};

enum class ComponentType {
    None,
    Vertex,
    Edge,
    Face,
    Entity
};

struct SelectionData {
    ComponentType type = ComponentType::None;
    rude::VertexPtr vertex = nullptr;
    rude::EdgePtr edge = nullptr;
    rude::FacePtr face = nullptr;
    rude::Entity* entity = nullptr;
    
    bool isValid() const {
        return type != ComponentType::None && 
               ((type == ComponentType::Vertex && vertex != nullptr) ||
                (type == ComponentType::Edge && edge != nullptr) ||
                (type == ComponentType::Face && face != nullptr) ||
                (type == ComponentType::Entity && entity != nullptr));
    }
    
    bool operator==(const SelectionData& other) const {
        if (type != other.type) return false;
        switch (type) {
            case ComponentType::Vertex: return vertex == other.vertex;
            case ComponentType::Edge: return edge == other.edge;
            case ComponentType::Face: return face == other.face;
            case ComponentType::Entity: return entity == other.entity;
            default: return false;
        }
    }
    
    bool operator<(const SelectionData& other) const {
        if (type != other.type) return type < other.type;
        switch (type) {
            case ComponentType::Vertex: return vertex < other.vertex;
            case ComponentType::Edge: return edge < other.edge;
            case ComponentType::Face: return face < other.face;
            case ComponentType::Entity: return entity < other.entity;
            default: return false;
        }
    }
};

class SelectionManager : public QObject {
    Q_OBJECT
    
public:
    SelectionManager();
    ~SelectionManager() = default;
    
    // Core selection methods
    void clear();
    void selectVertex(VertexPtr vertex, SelectionMode mode = SelectionMode::Replace);
    void selectEdge(EdgePtr edge, SelectionMode mode = SelectionMode::Replace);
    void selectFace(FacePtr face, SelectionMode mode = SelectionMode::Replace);
    void selectEntity(Entity* entity, SelectionMode mode = SelectionMode::Replace);
    void selectMultiple(const std::vector<SelectionData>& elements, SelectionMode mode = SelectionMode::Replace);
    
    // Geometry selection
    void selectByRay(const glm::vec3& rayOrigin, const glm::vec3& rayDir, ComponentType type);
    void selectByRect(const QRect& rect, const glm::mat4& viewProj, ComponentType type);
    
    // Scene management
    void setScene(rude::Scene* scene);
    rude::Scene* getScene() const { return m_scene; }
    void setSceneManager(rude::SceneManager* sceneManager) { m_sceneManager = sceneManager; }
    rude::SceneManager* getSceneManager() const { return m_sceneManager; }
    
    // Selection actions
    void selectAll();
    void invertSelection();
    void clearSelection();
    
    // Component queries
    bool isSelected(const SelectionData& data) const;
    bool hasMixedSelection() const;
    
    // Type conversion
    void convertSelection(ComponentType targetType);
    void expandSelection();
    
    // Getters for current selections
    const std::set<VertexPtr>& getSelectedVertices() const { return m_selectedVertices; }
    const std::set<EdgePtr>& getSelectedEdges() const { return m_selectedEdges; }
    const std::set<FacePtr>& getSelectedFaces() const { return m_selectedFaces; }
    const std::set<Entity*>& getSelectedEntities() const { return m_selectedEntities; }
    
    // Get all current selections as a vector
    std::vector<SelectionData> getSelection() const;
    
    // Selection utilities
    bool hasSelection() const;
    ComponentType getCurrentSelectionType() const;
    ComponentType getPrimarySelectionType() const;
    glm::vec3 getSelectionCenter() const;
    size_t getSelectionCount(ComponentType type) const;

signals:
    void selectionChanged();
    void vertexSelectionChanged();
    void edgeSelectionChanged();
    void faceSelectionChanged();
    void entitySelectionChanged();
    
private:
    // Current selection data
    std::set<VertexPtr> m_selectedVertices;
    std::set<EdgePtr> m_selectedEdges;
    std::set<FacePtr> m_selectedFaces;
    std::set<Entity*> m_selectedEntities;
    
    // Selection state tracking
    std::map<ComponentType, int> m_selectionCounts;
    ComponentType m_currentSelectionType = ComponentType::None;
    
    // Parent objects
    rude::Scene* m_scene = nullptr;
    rude::SceneManager* m_sceneManager = nullptr;
    
    // Internal helpers
    void emitAppropriateSignal(ComponentType type);
    void updateSelectionCounts();
    void selectComponent(const SelectionData& data, SelectionMode mode);
    void deselectComponent(const SelectionData& data);
    void applySelectionMode(SelectionData& data, SelectionMode mode);
    void filterByType(ComponentType type);
    
    // Topology helpers for selection conversion
    std::vector<VertexPtr> getVerticesFromEdge(EdgePtr edge) const;
    std::vector<VertexPtr> getVerticesFromFace(FacePtr face) const;
    std::vector<EdgePtr> getEdgesFromVertex(VertexPtr vertex) const;
    std::vector<EdgePtr> getEdgesFromFace(FacePtr face) const;
    std::vector<FacePtr> getFacesFromVertex(VertexPtr vertex) const;
    std::vector<FacePtr> getFacesFromEdge(EdgePtr edge) const;
};

} // namespace rude
