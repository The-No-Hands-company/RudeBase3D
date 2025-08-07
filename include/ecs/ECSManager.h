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
 * █  FILE: ECSManager.h                                                          █
 * █  DESCRIPTION: ECS Coordinator & Central Management System                   █
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
 * █  This file contains the ECSManager class providing centralized              █
 * █  coordination and management of all Entity Component System                 █
 * █  operations used throughout the RudeBase3D application.                     █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

#pragma once

#include "ECS.h"
#include "components/CoreComponents.h"
#include <memory>

namespace rude::ecs {

/**
 * @brief Manager class that integrates the ECS system with RudeBase3D
 * 
 * This class serves as a bridge between the ECS system and the existing
 * CoreSystem architecture. It manages the ECS World and provides
 * high-level operations for 3D modeling applications.
 */
class ECSManager {
public:
    /**
     * @brief Constructor
     */
    ECSManager();
    
    /**
     * @brief Destructor
     */
    ~ECSManager();

    /**
     * @brief Initialize the ECS system
     * @return True if initialization was successful
     */
    bool initialize();
    
    /**
     * @brief Shutdown the ECS system
     */
    void shutdown();
    
    /**
     * @brief Update the ECS system (called each frame)
     * @param deltaTime Time elapsed since last update in seconds
     */
    void update(float deltaTime);
    
    /**
     * @brief Get the ECS world
     * @return Reference to the ECS world
     */
    World& getWorld() { return *m_world; }
    const World& getWorld() const { return *m_world; }

    // ===========================================
    // High-level entity creation methods
    // ===========================================
    
    /**
     * @brief Create a basic 3D object entity
     * @param name Name for the object
     * @param position Initial position
     * @param mesh Mesh to use for the object
     * @return Entity ID of the created object
     */
    Entity createObject(const std::string& name, const glm::vec3& position = glm::vec3(0.0f));
    
    /**
     * @brief Create a camera entity
     * @param name Name for the camera
     * @param position Initial position
     * @param lookAt Point to look at
     * @return Entity ID of the created camera
     */
    Entity createCamera(const std::string& name, 
                       const glm::vec3& position = glm::vec3(0.0f, 0.0f, 5.0f),
                       const glm::vec3& lookAt = glm::vec3(0.0f));
    
    /**
     * @brief Create a light entity
     * @param name Name for the light
     * @param type Type of light to create
     * @param position Initial position
     * @param color Light color
     * @param intensity Light intensity
     * @return Entity ID of the created light
     */
    Entity createLight(const std::string& name,
                      LightComponent::LightType type,
                      const glm::vec3& position = glm::vec3(0.0f, 5.0f, 0.0f),
                      const glm::vec3& color = glm::vec3(1.0f),
                      float intensity = 1.0f);

    // ===========================================
    // System access methods
    // ===========================================
    
    /**
     * @brief Get the render system
     * @return Shared pointer to the render system
     */
    std::shared_ptr<RenderSystem> getRenderSystem() const { return m_renderSystem; }
    
    /**
     * @brief Get the camera system
     * @return Shared pointer to the camera system
     */
    std::shared_ptr<CameraSystem> getCameraSystem() const { return m_cameraSystem; }
    
    /**
     * @brief Get the selection system
     * @return Shared pointer to the selection system
     */
    std::shared_ptr<SelectionSystem> getSelectionSystem() const { return m_selectionSystem; }
    
    /**
     * @brief Get the lighting system
     * @return Shared pointer to the lighting system
     */
    std::shared_ptr<LightingSystem> getLightingSystem() const { return m_lightingSystem; }
    
    /**
     * @brief Get the hierarchy system
     * @return Shared pointer to the hierarchy system
     */
    std::shared_ptr<HierarchySystem> getHierarchySystem() const { return m_hierarchySystem; }
    
    /**
     * @brief Get the mesh system
     * @return Shared pointer to the mesh system
     */
    std::shared_ptr<MeshSystem> getMeshSystem() const { return m_meshSystem; }

    // ===========================================
    // Utility methods
    // ===========================================
    
    /**
     * @brief Get entities by name
     * @param name Name to search for
     * @return Vector of entities with the specified name
     */
    std::vector<Entity> getEntitiesByName(const std::string& name) const;
    
    /**
     * @brief Get the first entity with a specific name
     * @param name Name to search for
     * @return Entity with the name, or NULL_ENTITY if not found
     */
    Entity getEntityByName(const std::string& name) const;
    
    /**
     * @brief Create a parent-child relationship between entities
     * @param parent Parent entity
     * @param child Child entity
     */
    void setParent(Entity parent, Entity child);
    
    /**
     * @brief Remove a parent-child relationship
     * @param child Child entity to remove from its parent
     */
    void removeParent(Entity child);
    
    /**
     * @brief Get statistics about the ECS system
     * @return String containing ECS statistics
     */
    std::string getStatistics() const;

private:
    /**
     * @brief Register all core component types
     */
    void registerCoreComponents();
    
    /**
     * @brief Register and setup all core systems
     */
    void registerCoreSystems();
    
    /**
     * @brief Set up system signatures
     */
    void setupSystemSignatures();

private:
    std::unique_ptr<World> m_world;
    
    // Core systems
    std::shared_ptr<RenderSystem> m_renderSystem;
    std::shared_ptr<CameraSystem> m_cameraSystem;
    std::shared_ptr<SelectionSystem> m_selectionSystem;
    std::shared_ptr<LightingSystem> m_lightingSystem;
    std::shared_ptr<HierarchySystem> m_hierarchySystem;
    std::shared_ptr<MeshSystem> m_meshSystem;
    
    bool m_initialized = false;
};

} // namespace rude::ecs
