#include "core/selection_manager.hpp"
#include "core/entity.hpp"
#include "core/scene.hpp"
#include <algorithm>
#include <map>
#include <spdlog/spdlog.h>

namespace rude {

rude::SelectionManager::SelectionManager() : QObject() {
    // Initialize with empty selection
    m_currentSelectionType = ComponentType::None;
}

void rude::SelectionManager::setScene(rude::Scene* scene) {
    if (m_scene == scene) return;
    
    // Clear any existing selection
    clearSelection();
    
    m_scene = scene;
}

// Selection operations
void SelectionManager::selectVertex(VertexPtr vertex, SelectionMode mode) {
    if (!vertex) return;
    
    SelectionData data;
    data.type = ComponentType::Vertex;
    data.vertex = vertex;
    
    selectComponent(data, mode);
}

void SelectionManager::selectEdge(EdgePtr edge, SelectionMode mode) {
    if (!edge) return;
    
    SelectionData data;
    data.type = ComponentType::Edge;
    data.edge = edge;
    
    selectComponent(data, mode);
}

void SelectionManager::selectFace(FacePtr face, SelectionMode mode) {
    if (!face) return;
    
    SelectionData data;
    data.type = ComponentType::Face;
    data.face = face;
    
    selectComponent(data, mode);
}

void SelectionManager::selectEntity(Entity* entity, SelectionMode mode) {
    if (!entity) return;
    
    SelectionData data;
    data.type = ComponentType::Entity;
    data.entity = entity;
    
    selectComponent(data, mode);
}

void SelectionManager::selectMultiple(const std::vector<SelectionData>& elements, SelectionMode mode) {
    if (elements.empty()) return;

    // Handle first element according to mode
    if (!elements.empty()) {
        selectComponent(elements[0], mode);
    }
    
    // Add the rest with Add mode
    for (size_t i = 1; i < elements.size(); i++) {
        selectComponent(elements[i], SelectionMode::Add);
    }
}

void SelectionManager::selectByRay(const glm::vec3& rayOrigin, const glm::vec3& rayDir, ComponentType type) {
    spdlog::info("SelectionManager: Ray selection not implemented yet");
    // This would implement ray-picking for the specified component type
}

void SelectionManager::selectByRect(const QRect& rect, const glm::mat4& viewProj, ComponentType type) {
    spdlog::info("SelectionManager: Rectangle selection not implemented yet");
    // This would implement rectangle selection for the specified component type
}

// Selection actions
void SelectionManager::selectAll() {
    if (!m_scene) return;
    
    switch (m_currentSelectionType) {
        case ComponentType::Vertex: {
            // Select all vertices in all meshes
            spdlog::info("SelectionManager: Select all vertices not implemented yet");
            break;
        }
        case ComponentType::Edge: {
            // Select all edges in all meshes
            spdlog::info("SelectionManager: Select all edges not implemented yet");
            break;
        }
        case ComponentType::Face: {
            // Select all faces in all meshes
            spdlog::info("SelectionManager: Select all faces not implemented yet");
            break;
        }
        case ComponentType::Entity: {
            // Select all entities in the scene
            const auto& entities = m_scene->getEntities();
            for (const auto& entity : entities) {
                selectEntity(entity.get(), SelectionMode::Add);
            }
            break;
        }
        default:
            break;
    }
}

void SelectionManager::invertSelection() {
    if (!m_scene) return;
    
    spdlog::info("SelectionManager: Invert selection not fully implemented yet");

    switch (m_currentSelectionType) {
        case ComponentType::Entity: {
            // Invert entity selection
            std::vector<Entity*> toSelect;
            const auto& allEntities = m_scene->getEntities();
            
            for (const auto& entity : allEntities) {
                if (m_selectedEntities.find(entity.get()) == m_selectedEntities.end()) {
                    toSelect.push_back(entity.get());
                }
            }
            
            clearSelection();
            for (Entity* entity : toSelect) {
                selectEntity(entity, SelectionMode::Add);
            }
            break;
        }
        default:
            spdlog::info("SelectionManager: Invert selection not implemented for this component type");
            break;
    }
}

void SelectionManager::clearSelection() {
    bool hadSelection = hasSelection();
    
    m_selectedVertices.clear();
    m_selectedEdges.clear();
    m_selectedFaces.clear();
    m_selectedEntities.clear();
    
    updateSelectionCounts();
    
    if (hadSelection) {
        emit selectionChanged();
    }
}

// Component queries
bool SelectionManager::isSelected(const SelectionData& data) const {
    switch (data.type) {
        case ComponentType::Vertex:
            return m_selectedVertices.find(data.vertex) != m_selectedVertices.end();
        case ComponentType::Edge:
            return m_selectedEdges.find(data.edge) != m_selectedEdges.end();
        case ComponentType::Face:
            return m_selectedFaces.find(data.face) != m_selectedFaces.end();
        case ComponentType::Entity:
            return m_selectedEntities.find(data.entity) != m_selectedEntities.end();
        default:
            return false;
    }
}

bool SelectionManager::hasMixedSelection() const {
    int typeCount = 0;
    
    if (!m_selectedVertices.empty()) typeCount++;
    if (!m_selectedEdges.empty()) typeCount++;
    if (!m_selectedFaces.empty()) typeCount++;
    if (!m_selectedEntities.empty()) typeCount++;
    
    return typeCount > 1;
}

// Type conversion
void SelectionManager::convertSelection(ComponentType targetType) {
    if (m_currentSelectionType == targetType || !hasSelection()) return;
    
    spdlog::info("SelectionManager: Converting selection from {} to {}", static_cast<int>(m_currentSelectionType), static_cast<int>(targetType));
    
    // Store current selection
    std::set<VertexPtr> oldVertices = m_selectedVertices;
    std::set<EdgePtr> oldEdges = m_selectedEdges;
    std::set<FacePtr> oldFaces = m_selectedFaces;
    std::set<Entity*> oldEntities = m_selectedEntities;
    
    // Clear current selection
    clearSelection();
    
    // Convert based on source and target types
    switch (m_currentSelectionType) {
        case ComponentType::Vertex:
            if (targetType == ComponentType::Edge) {
                for (const auto& vertex : oldVertices) {
                    auto edges = getEdgesFromVertex(vertex);
                    for (const auto& edge : edges) {
                        m_selectedEdges.insert(edge);
                    }
                }
            }
            else if (targetType == ComponentType::Face) {
                for (const auto& vertex : oldVertices) {
                    auto faces = getFacesFromVertex(vertex);
                    for (const auto& face : faces) {
                        m_selectedFaces.insert(face);
                    }
                }
            }
            break;
            
        case ComponentType::Edge:
            if (targetType == ComponentType::Vertex) {
                for (const auto& edge : oldEdges) {
                    auto vertices = getVerticesFromEdge(edge);
                    for (const auto& vertex : vertices) {
                        m_selectedVertices.insert(vertex);
                    }
                }
            }
            else if (targetType == ComponentType::Face) {
                for (const auto& edge : oldEdges) {
                    auto faces = getFacesFromEdge(edge);
                    for (const auto& face : faces) {
                        m_selectedFaces.insert(face);
                    }
                }
            }
            break;
            
        case ComponentType::Face:
            if (targetType == ComponentType::Vertex) {
                for (const auto& face : oldFaces) {
                    auto vertices = getVerticesFromFace(face);
                    for (const auto& vertex : vertices) {
                        m_selectedVertices.insert(vertex);
                    }
                }
            }
            else if (targetType == ComponentType::Edge) {
                for (const auto& face : oldFaces) {
                    auto edges = getEdgesFromFace(face);
                    for (const auto& edge : edges) {
                        m_selectedEdges.insert(edge);
                    }
                }
            }
            break;
            
        default:
            break;
    }
    
    // Update selection state
    updateSelectionCounts();
    
    // Emit signal
    emit selectionChanged();
}

void SelectionManager::expandSelection() {
    spdlog::info("SelectionManager: Expand selection not fully implemented yet");
    
    switch (m_currentSelectionType) {
        case ComponentType::Vertex: {
            // Add connected vertices
            std::set<VertexPtr> newVertices = m_selectedVertices;
            
            for (const auto& vertex : m_selectedVertices) {
                auto edges = getEdgesFromVertex(vertex);
                for (const auto& edge : edges) {
                    auto connectedVertices = getVerticesFromEdge(edge);
                    for (const auto& v : connectedVertices) {
                        newVertices.insert(v);
                    }
                }
            }
            
            m_selectedVertices = newVertices;
            break;
        }
            
        case ComponentType::Edge: {
            // Add connected edges
            std::set<EdgePtr> newEdges = m_selectedEdges;
            
            for (const auto& edge : m_selectedEdges) {
                auto vertices = getVerticesFromEdge(edge);
                for (const auto& v : vertices) {
                    auto connectedEdges = getEdgesFromVertex(v);
                    for (const auto& e : connectedEdges) {
                        newEdges.insert(e);
                    }
                }
            }
            
            m_selectedEdges = newEdges;
            break;
        }
            
        case ComponentType::Face: {
            // Add adjacent faces
            std::set<FacePtr> newFaces = m_selectedFaces;
            
            for (const auto& face : m_selectedFaces) {
                auto edges = getEdgesFromFace(face);
                for (const auto& edge : edges) {
                    auto adjacentFaces = getFacesFromEdge(edge);
                    for (const auto& f : adjacentFaces) {
                        newFaces.insert(f);
                    }
                }
            }
            
            m_selectedFaces = newFaces;
            break;
        }
            
        default:
            break;
    }
    
    updateSelectionCounts();
    emit selectionChanged();
}

// Selection utilities
bool SelectionManager::hasSelection() const {
    return !m_selectedVertices.empty() || 
           !m_selectedEdges.empty() || 
           !m_selectedFaces.empty() || 
           !m_selectedEntities.empty();
}

ComponentType SelectionManager::getCurrentSelectionType() const {
    return m_currentSelectionType;
}

ComponentType SelectionManager::getPrimarySelectionType() const {
    if (!m_selectedVertices.empty()) {
        return ComponentType::Vertex;
    }
    else if (!m_selectedEdges.empty()) {
        return ComponentType::Edge;
    }
    else if (!m_selectedFaces.empty()) {
        return ComponentType::Face;
    }
    else if (!m_selectedEntities.empty()) {
        return ComponentType::Entity;
    }
    
    return ComponentType::None;
}

glm::vec3 SelectionManager::getSelectionCenter() const {
    glm::vec3 center(0.0f);
    int count = 0;
    
    // Get center based on the current selection type
    ComponentType type = getPrimarySelectionType();
    
    if (type == ComponentType::Vertex) {
        for (const auto& vertex : m_selectedVertices) {
            if (vertex) {
                center += glm::vec3(vertex->position.x, vertex->position.y, vertex->position.z);
                count++;
            }
        }
    }
    else if (type == ComponentType::Edge) {
        // Use the midpoint of each edge
        for (const auto& edge : m_selectedEdges) {
            if (edge) {
                std::vector<VertexPtr> vertices = getVerticesFromEdge(edge);
                if (vertices.size() >= 2) {
                    glm::vec3 v1(vertices[0]->position.x, vertices[0]->position.y, vertices[0]->position.z);
                    glm::vec3 v2(vertices[1]->position.x, vertices[1]->position.y, vertices[1]->position.z);
                    center += (v1 + v2) * 0.5f;
                    count++;
                }
            }
        }
    }
    else if (type == ComponentType::Face) {
        // Use the centroid of each face
        for (const auto& face : m_selectedFaces) {
            if (face) {
                std::vector<VertexPtr> vertices = getVerticesFromFace(face);
                glm::vec3 faceCentroid(0.0f);
                int vertCount = 0;
                
                // Fixing incorrect member access for glm::vec3 components
                for (const auto& vertex : vertices) {
                    if (vertex) {
                        faceCentroid += glm::vec3(vertex->position.x, vertex->position.y, vertex->position.z);
                        vertCount++;
                    }
                }
                
                if (vertCount > 0) {
                    faceCentroid /= static_cast<float>(vertCount);
                    center += faceCentroid;
                    count++;
                }
            }
        }
    }
    else if (type == ComponentType::Entity) {
        // Use the world position of each entity
        for (const auto& entity : m_selectedEntities) {
            if (entity) {
                center += entity->getWorldPosition();
                count++;
            }
        }
    }
    
    if (count > 0) {
        center /= static_cast<float>(count);
    }
    
    return center;
}

size_t SelectionManager::getSelectionCount(ComponentType type) const {
    switch (type) {
        case ComponentType::Vertex:
            return m_selectedVertices.size();
        case ComponentType::Edge:
            return m_selectedEdges.size();
        case ComponentType::Face:
            return m_selectedFaces.size();
        case ComponentType::Entity:
            return m_selectedEntities.size();
        default:
            return 0;
    }
}

// Private methods

void SelectionManager::selectComponent(const SelectionData& data, SelectionMode mode) {
    if (!data.isValid()) return;
    
    // If replacing and changing type, clear first
    if (mode == SelectionMode::Replace && m_currentSelectionType != data.type) {
        clearSelection();
        m_currentSelectionType = data.type;
    }
    
    // Apply selection based on type
    switch (data.type) {
        case ComponentType::Vertex:
            if (mode == SelectionMode::Replace) m_selectedVertices.clear();
            if (mode == SelectionMode::Toggle && m_selectedVertices.find(data.vertex) != m_selectedVertices.end()) {
                m_selectedVertices.erase(data.vertex);
            } else if (mode != SelectionMode::Subtract) {
                m_selectedVertices.insert(data.vertex);
            } else {
                m_selectedVertices.erase(data.vertex);
            }
            break;
            
        case ComponentType::Edge:
            if (mode == SelectionMode::Replace) m_selectedEdges.clear();
            if (mode == SelectionMode::Toggle && m_selectedEdges.find(data.edge) != m_selectedEdges.end()) {
                m_selectedEdges.erase(data.edge);
            } else if (mode != SelectionMode::Subtract) {
                m_selectedEdges.insert(data.edge);
            } else {
                m_selectedEdges.erase(data.edge);
            }
            break;
            
        case ComponentType::Face:
            if (mode == SelectionMode::Replace) m_selectedFaces.clear();
            if (mode == SelectionMode::Toggle && m_selectedFaces.find(data.face) != m_selectedFaces.end()) {
                m_selectedFaces.erase(data.face);
            } else if (mode != SelectionMode::Subtract) {
                m_selectedFaces.insert(data.face);
            } else {
                m_selectedFaces.erase(data.face);
            }
            break;
            
        case ComponentType::Entity:
            if (mode == SelectionMode::Replace) m_selectedEntities.clear();
            if (mode == SelectionMode::Toggle && m_selectedEntities.find(data.entity) != m_selectedEntities.end()) {
                m_selectedEntities.erase(data.entity);
            } else if (mode != SelectionMode::Subtract) {
                m_selectedEntities.insert(data.entity);
            } else {
                m_selectedEntities.erase(data.entity);
            }
            break;
            
        default:
            break;
    }
    
    updateSelectionCounts();
    emitAppropriateSignal(data.type);
}

void SelectionManager::deselectComponent(const SelectionData& data) {
    switch (data.type) {
        case ComponentType::Vertex:
            m_selectedVertices.erase(data.vertex);
            break;
        case ComponentType::Edge:
            m_selectedEdges.erase(data.edge);
            break;
        case ComponentType::Face:
            m_selectedFaces.erase(data.face);
            break;
        case ComponentType::Entity:
            m_selectedEntities.erase(data.entity);
            break;
        default:
            break;
    }
    
    updateSelectionCounts();
    emitAppropriateSignal(data.type);
}

void SelectionManager::applySelectionMode(SelectionData& data, SelectionMode mode) {
    // This function is kept for future use - currently the logic is in selectComponent
}

void SelectionManager::emitAppropriateSignal(ComponentType type) {
    // Always emit the general signal
    emit selectionChanged();
    
    // Emit type-specific signal
    switch (type) {
        case ComponentType::Vertex:
            emit vertexSelectionChanged();
            break;
        case ComponentType::Edge:
            emit edgeSelectionChanged();
            break;
        case ComponentType::Face:
            emit faceSelectionChanged();
            break;
        case ComponentType::Entity:
            emit entitySelectionChanged();
            break;
        default:
            break;
    }
}

void SelectionManager::updateSelectionCounts() {
    // Update counts
    m_selectionCounts[ComponentType::Vertex] = static_cast<int>(m_selectedVertices.size());
    m_selectionCounts[ComponentType::Edge] = static_cast<int>(m_selectedEdges.size());
    m_selectionCounts[ComponentType::Face] = static_cast<int>(m_selectedFaces.size());
    m_selectionCounts[ComponentType::Entity] = static_cast<int>(m_selectedEntities.size());
    
    // Update current selection type
    if (m_selectedVertices.size() > 0) {
        m_currentSelectionType = ComponentType::Vertex;
    }
    else if (m_selectedEdges.size() > 0) {
        m_currentSelectionType = ComponentType::Edge;
    }
    else if (m_selectedFaces.size() > 0) {
        m_currentSelectionType = ComponentType::Face;
    }
    else if (m_selectedEntities.size() > 0) {
        m_currentSelectionType = ComponentType::Entity;
    }
    else {
        m_currentSelectionType = ComponentType::None;
    }
}

void SelectionManager::filterByType(ComponentType type) {
    switch (type) {
        case ComponentType::Vertex:
            m_selectedEdges.clear();
            m_selectedFaces.clear();
            m_selectedEntities.clear();
            break;
        case ComponentType::Edge:
            m_selectedVertices.clear();
            m_selectedFaces.clear();
            m_selectedEntities.clear();
            break;
        case ComponentType::Face:
            m_selectedVertices.clear();
            m_selectedEdges.clear();
            m_selectedEntities.clear();
            break;
        case ComponentType::Entity:
            m_selectedVertices.clear();
            m_selectedEdges.clear();
            m_selectedFaces.clear();
            break;
        default:
            break;
    }
}

// Topology helper methods - stub implementations
std::vector<VertexPtr> SelectionManager::getVerticesFromEdge(EdgePtr edge) const {
    std::vector<VertexPtr> result;
    // Stub implementation - in a real app, we would access the half-edge mesh data
    return result;
}

std::vector<VertexPtr> SelectionManager::getVerticesFromFace(FacePtr face) const {
    std::vector<VertexPtr> result;
    // Stub implementation - in a real app, we would access the half-edge mesh data
    return result;
}

std::vector<EdgePtr> SelectionManager::getEdgesFromVertex(VertexPtr vertex) const {
    std::vector<EdgePtr> result;
    // Stub implementation - in a real app, we would access the half-edge mesh data
    return result;
}

std::vector<EdgePtr> SelectionManager::getEdgesFromFace(FacePtr face) const {
    std::vector<EdgePtr> result;
    // Stub implementation - in a real app, we would access the half-edge mesh data
    return result;
}

std::vector<FacePtr> SelectionManager::getFacesFromVertex(VertexPtr vertex) const {
    std::vector<FacePtr> result;
    // Stub implementation - in a real app, we would access the half-edge mesh data
    return result;
}

std::vector<FacePtr> SelectionManager::getFacesFromEdge(EdgePtr edge) const {
    std::vector<FacePtr> result;
    // Stub implementation - in a real app, we would access the half-edge mesh data
    return result;
}

std::vector<SelectionData> SelectionManager::getSelection() const {
    std::vector<SelectionData> result;
    
    // Add all selected vertices
    for (const auto& vertex : m_selectedVertices) {
        SelectionData data;
        data.type = ComponentType::Vertex;
        data.vertex = vertex;
        result.push_back(data);
    }
    
    // Add all selected edges
    for (const auto& edge : m_selectedEdges) {
        SelectionData data;
        data.type = ComponentType::Edge;
        data.edge = edge;
        result.push_back(data);
    }
    
    // Add all selected faces
    for (const auto& face : m_selectedFaces) {
        SelectionData data;
        data.type = ComponentType::Face;
        data.face = face;
        result.push_back(data);
    }
    
    // Add all selected entities
    for (const auto& entity : m_selectedEntities) {
        SelectionData data;
        data.type = ComponentType::Entity;
        data.entity = entity;
        result.push_back(data);
    }
    
    return result;
}

} // namespace rude
