/*
 * ██████╗ ██╗   ██╗██████╗ ███████╗██████╗  █████╗ ███████╗███████╗██████╗ ██████╗ 
 * ██╔══██╗██║   ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝╚════██╗██╔══██╗
 * ██████╔╝██║   ██║██║  ██║█████╗  ██████╔╝███████║███████╗█████╗   █████╔╝██║  ██║
 * ██╔══██╗██║   ██║██║  ██║██╔══╝  ██╔══██╗██╔══██║╚════██║██╔══╝   ╚═══██╗██║  ██║
 * ██║  ██║╚██████╔╝██████╔╝███████╗██████╔╝██║  ██║███████║███████╗██████╔╝██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═════╝ ╚═════╝ 
 * 
 * Professional 3D Modeling & Animation Software
 * 
 * ████████████████████████████████████████████████████████████████████████████████
 * █                                                                              █
 * █  FILE: CoreSystems.h                                                         █
 * █  DESCRIPTION: ECS Core System Definitions & Logic Processors               █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite            █
 * █  VERSION: 1.0.0                                                              █
 * █  CREATED: 2024                                                               █
 * █  UPDATED: August 7, 2025                                                     █
 * █                                                                              █
 * █  LICENSE: MIT License                                                        █
 * █  COMPANY: The No Hands Company                                               █
 * █  AUTHOR: RudeBase3D Development Team                                         █
 * █                                                                              █
 * █  This file contains the core system definitions for logic processing        █
 * █  and entity management in the Entity Component System architecture          █
 * █  used throughout the RudeBase3D application.                                █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

#pragma once

#include "../System.h"
#include "../components/CoreComponents.h"
#include <glm/glm.hpp>
#include <vector>

namespace rude::ecs {

// Forward declaration
class World;

/**
 * @brief System that manages hierarchical transformations
 * 
 * This system ensures that child entities inherit the transformations
 * of their parent entities, creating a proper hierarchy.
 */
class HierarchySystem : public System {
public:
    void initialize() override;
    void update(float deltaTime) override;

private:
    void updateHierarchy(Entity entity, const glm::mat4& parentTransform);
    World* m_world = nullptr;
    
    friend class World;
    void setWorld(World* world) { m_world = world; }
};

/**
 * @brief System that handles rendering of entities
 * 
 * This system processes entities with Transform, Mesh, Material, and Renderable
 * components to render them to the screen.
 */
class RenderSystem : public System {
public:
    void initialize() override;
    void update(float deltaTime) override;
    void shutdown() override;

private:
    void renderEntity(Entity entity);
    void setupRenderState();
    void cleanupRenderState();
    
    World* m_world = nullptr;
    
    friend class World;
    void setWorld(World* world) { m_world = world; }
};

/**
 * @brief System that manages camera operations
 * 
 * This system handles camera updates, view matrix calculations,
 * and manages the active camera.
 */
class CameraSystem : public System {
public:
    void initialize() override;
    void update(float deltaTime) override;
    
    /**
     * @brief Get the active camera entity
     * @return Entity ID of the active camera, or NULL_ENTITY if none
     */
    Entity getActiveCamera() const { return m_activeCamera; }
    
    /**
     * @brief Set the active camera
     * @param camera Entity ID of the camera to activate
     */
    void setActiveCamera(Entity camera);
    
    /**
     * @brief Get the view matrix of the active camera
     * @return View matrix, or identity if no active camera
     */
    glm::mat4 getViewMatrix() const;
    
    /**
     * @brief Get the projection matrix of the active camera
     * @return Projection matrix, or identity if no active camera
     */
    glm::mat4 getProjectionMatrix() const;

private:
    Entity m_activeCamera = NULL_ENTITY;
    World* m_world = nullptr;
    
    friend class World;
    void setWorld(World* world) { m_world = world; }
};

/**
 * @brief System that manages selection of entities
 * 
 * This system handles entity selection, highlighting, and selection state management.
 */
class SelectionSystem : public System {
public:
    void initialize() override;
    void update(float deltaTime) override;
    
    /**
     * @brief Select an entity
     * @param entity Entity to select
     * @param addToSelection If true, add to current selection; if false, replace selection
     */
    void selectEntity(Entity entity, bool addToSelection = false);
    
    /**
     * @brief Deselect an entity
     * @param entity Entity to deselect
     */
    void deselectEntity(Entity entity);
    
    /**
     * @brief Clear all selections
     */
    void clearSelection();
    
    /**
     * @brief Check if an entity is selected
     * @param entity Entity to check
     * @return True if the entity is selected
     */
    bool isEntitySelected(Entity entity) const;
    
    /**
     * @brief Get all selected entities
     * @return Vector of selected entity IDs
     */
    std::vector<Entity> getSelectedEntities() const;
    
    /**
     * @brief Set hover state for an entity
     * @param entity Entity to set hover state for
     * @param isHovered True if entity is hovered
     */
    void setEntityHover(Entity entity, bool isHovered);

private:
    std::vector<Entity> m_selectedEntities;
    World* m_world = nullptr;
    
    friend class World;
    void setWorld(World* world) { m_world = world; }
};

/**
 * @brief System that manages lighting calculations
 * 
 * This system processes light entities and calculates lighting
 * information for the renderer.
 */
class LightingSystem : public System {
public:
    void initialize() override;
    void update(float deltaTime) override;
    
    /**
     * @brief Get all light entities
     * @return Vector of light entity IDs
     */
    std::vector<Entity> getLights() const;
    
    /**
     * @brief Get the main directional light
     * @return Entity ID of the main directional light, or NULL_ENTITY if none
     */
    Entity getMainDirectionalLight() const;

private:
    World* m_world = nullptr;
    
    friend class World;
    void setWorld(World* world) { m_world = world; }
};

/**
 * @brief System that manages mesh operations and updates
 * 
 * This system handles mesh loading, unloading, and GPU resource management.
 */
class MeshSystem : public System {
public:
    void initialize() override;
    void update(float deltaTime) override;
    void shutdown() override;
    
    /**
     * @brief Mark a mesh as dirty (needs GPU upload)
     * @param entity Entity with the mesh to mark as dirty
     */
    void markMeshDirty(Entity entity);
    
    /**
     * @brief Upload mesh data to GPU
     * @param entity Entity with the mesh to upload
     */
    void uploadMeshToGPU(Entity entity);

private:
    void processDirtyMeshes();
    
    World* m_world = nullptr;
    
    friend class World;
    void setWorld(World* world) { m_world = world; }
};

} // namespace rude::ecs
