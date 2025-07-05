#include "ecs/systems/CoreSystems.h"
#include "ecs/World.h"
#include <iostream>

namespace rude::ecs {

// =====================================
// Initialize static member variable
// =====================================
std::uint32_t Component::s_nextTypeId = 0;

// =====================================
// HierarchySystem Implementation
// =====================================

void HierarchySystem::initialize() {
    std::cout << "HierarchySystem initialized" << std::endl;
}

void HierarchySystem::update(float deltaTime) {
    // Update hierarchical transformations
    if (!m_world) return;
    
    // Process all entities with hierarchy components
    for (Entity entity : m_entities) {
        if (m_world->hasComponent<HierarchyComponent>(entity)) {
            auto& hierarchy = m_world->getComponent<HierarchyComponent>(entity);
            
            // If this entity has no parent, it's a root - start hierarchy update from here
            if (hierarchy.parent == NULL_ENTITY) {
                if (m_world->hasComponent<TransformComponent>(entity)) {
                    auto& transform = m_world->getComponent<TransformComponent>(entity);
                    updateHierarchy(entity, transform.getMatrix());
                }
            }
        }
    }
}

void HierarchySystem::updateHierarchy(Entity entity, const glm::mat4& parentTransform) {
    if (!m_world || !m_world->hasComponent<HierarchyComponent>(entity)) {
        return;
    }
    
    auto& hierarchy = m_world->getComponent<HierarchyComponent>(entity);
    
    // Update all children
    for (Entity child : hierarchy.children) {
        if (m_world->hasComponent<TransformComponent>(child)) {
            auto& childTransform = m_world->getComponent<TransformComponent>(child);
            // Here you would normally update the child's world transform
            // based on its local transform and the parent's world transform
            
            // Recursively update children
            updateHierarchy(child, parentTransform * childTransform.getMatrix());
        }
    }
}

// =====================================
// RenderSystem Implementation
// =====================================

void RenderSystem::initialize() {
    std::cout << "RenderSystem initialized" << std::endl;
    setupRenderState();
}

void RenderSystem::update(float deltaTime) {
    // Render all entities with the required components
    for (Entity entity : m_entities) {
        renderEntity(entity);
    }
}

void RenderSystem::shutdown() {
    cleanupRenderState();
    std::cout << "RenderSystem shutdown" << std::endl;
}

void RenderSystem::renderEntity(Entity entity) {
    // This is a stub - in a real implementation, this would:
    // 1. Get the transform, mesh, material, and renderable components
    // 2. Set up rendering state
    // 3. Submit draw calls to the graphics API
    // 4. Handle LOD, culling, etc.
}

void RenderSystem::setupRenderState() {
    // Initialize rendering resources, shaders, etc.
}

void RenderSystem::cleanupRenderState() {
    // Clean up rendering resources
}

// =====================================
// CameraSystem Implementation
// =====================================

void CameraSystem::initialize() {
    std::cout << "CameraSystem initialized" << std::endl;
}

void CameraSystem::update(float deltaTime) {
    // Update camera-related logic
    // Find active camera, update view matrices, etc.
    for (Entity entity : m_entities) {
        if (m_world && m_world->hasComponent<CameraComponent>(entity)) {
            auto& camera = m_world->getComponent<CameraComponent>(entity);
            
            if (camera.isActive && m_activeCamera == NULL_ENTITY) {
                m_activeCamera = entity;
            }
        }
    }
}

void CameraSystem::setActiveCamera(Entity camera) {
    if (m_activeCamera != NULL_ENTITY && m_world && m_world->hasComponent<CameraComponent>(m_activeCamera)) {
        auto& oldCamera = m_world->getComponent<CameraComponent>(m_activeCamera);
        oldCamera.isActive = false;
    }
    
    m_activeCamera = camera;
    
    if (m_world && m_world->hasComponent<CameraComponent>(camera)) {
        auto& newCamera = m_world->getComponent<CameraComponent>(camera);
        newCamera.isActive = true;
    }
}

glm::mat4 CameraSystem::getViewMatrix() const {
    if (m_activeCamera == NULL_ENTITY || !m_world) {
        return glm::mat4(1.0f);
    }
    
    if (m_world->hasComponent<TransformComponent>(m_activeCamera)) {
        auto& transform = m_world->getComponent<TransformComponent>(m_activeCamera);
        return glm::inverse(transform.getMatrix());
    }
    
    return glm::mat4(1.0f);
}

glm::mat4 CameraSystem::getProjectionMatrix() const {
    if (m_activeCamera == NULL_ENTITY || !m_world) {
        return glm::mat4(1.0f);
    }
    
    if (m_world->hasComponent<CameraComponent>(m_activeCamera)) {
        auto& camera = m_world->getComponent<CameraComponent>(m_activeCamera);
        return camera.getProjectionMatrix();
    }
    
    return glm::mat4(1.0f);
}

// =====================================
// SelectionSystem Implementation
// =====================================

void SelectionSystem::initialize() {
    std::cout << "SelectionSystem initialized" << std::endl;
}

void SelectionSystem::update(float deltaTime) {
    // Update selection highlighting, hover effects, etc.
}

void SelectionSystem::selectEntity(Entity entity, bool addToSelection) {
    if (!m_world || !m_world->hasComponent<SelectableComponent>(entity)) {
        return;
    }
    
    if (!addToSelection) {
        clearSelection();
    }
    
    auto& selectable = m_world->getComponent<SelectableComponent>(entity);
    if (!selectable.isSelected) {
        selectable.isSelected = true;
        m_selectedEntities.push_back(entity);
    }
}

void SelectionSystem::deselectEntity(Entity entity) {
    if (!m_world || !m_world->hasComponent<SelectableComponent>(entity)) {
        return;
    }
    
    auto& selectable = m_world->getComponent<SelectableComponent>(entity);
    selectable.isSelected = false;
    
    m_selectedEntities.erase(
        std::remove(m_selectedEntities.begin(), m_selectedEntities.end(), entity),
        m_selectedEntities.end()
    );
}

void SelectionSystem::clearSelection() {
    for (Entity entity : m_selectedEntities) {
        if (m_world && m_world->hasComponent<SelectableComponent>(entity)) {
            auto& selectable = m_world->getComponent<SelectableComponent>(entity);
            selectable.isSelected = false;
        }
    }
    m_selectedEntities.clear();
}

bool SelectionSystem::isEntitySelected(Entity entity) const {
    return std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity) != m_selectedEntities.end();
}

std::vector<Entity> SelectionSystem::getSelectedEntities() const {
    return m_selectedEntities;
}

void SelectionSystem::setEntityHover(Entity entity, bool isHovered) {
    if (!m_world || !m_world->hasComponent<SelectableComponent>(entity)) {
        return;
    }
    
    auto& selectable = m_world->getComponent<SelectableComponent>(entity);
    selectable.isHovered = isHovered;
}

// =====================================
// LightingSystem Implementation
// =====================================

void LightingSystem::initialize() {
    std::cout << "LightingSystem initialized" << std::endl;
}

void LightingSystem::update(float deltaTime) {
    // Update lighting calculations, shadow maps, etc.
}

std::vector<Entity> LightingSystem::getLights() const {
    std::vector<Entity> lights;
    for (Entity entity : m_entities) {
        lights.push_back(entity);
    }
    return lights;
}

Entity LightingSystem::getMainDirectionalLight() const {
    for (Entity entity : m_entities) {
        if (m_world && m_world->hasComponent<LightComponent>(entity)) {
            auto& light = m_world->getComponent<LightComponent>(entity);
            if (light.type == LightComponent::LightType::Directional) {
                return entity;
            }
        }
    }
    return NULL_ENTITY;
}

// =====================================
// MeshSystem Implementation
// =====================================

void MeshSystem::initialize() {
    std::cout << "MeshSystem initialized" << std::endl;
}

void MeshSystem::update(float deltaTime) {
    processDirtyMeshes();
}

void MeshSystem::shutdown() {
    std::cout << "MeshSystem shutdown" << std::endl;
}

void MeshSystem::markMeshDirty(Entity entity) {
    if (!m_world || !m_world->hasComponent<MeshComponent>(entity)) {
        return;
    }
    
    auto& mesh = m_world->getComponent<MeshComponent>(entity);
    mesh.isDirty = true;
}

void MeshSystem::uploadMeshToGPU(Entity entity) {
    if (!m_world || !m_world->hasComponent<MeshComponent>(entity)) {
        return;
    }
    
    auto& meshComponent = m_world->getComponent<MeshComponent>(entity);
    if (meshComponent.mesh && meshComponent.isDirty) {
        // Here you would upload the mesh data to GPU
        // This is a stub implementation
        meshComponent.isDirty = false;
    }
}

void MeshSystem::processDirtyMeshes() {
    for (Entity entity : m_entities) {
        if (m_world && m_world->hasComponent<MeshComponent>(entity)) {
            auto& mesh = m_world->getComponent<MeshComponent>(entity);
            if (mesh.isDirty) {
                uploadMeshToGPU(entity);
            }
        }
    }
}

} // namespace rude::ecs
