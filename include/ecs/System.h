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
 * █  FILE: System.h                                                              █
 * █  DESCRIPTION: ECS System Base Class & Logic Processing Framework            █
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
 * █  This file contains the System base class providing logic processing        █
 * █  and entity management functionality for the Entity Component System        █
 * █  architecture used throughout the RudeBase3D application.                   █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

#pragma once

#include "Entity.h"
#include <set>
#include <bitset>

namespace rude::ecs {

/**
 * @brief Type alias for component signatures
 */
using Signature = std::bitset<32>; // Forward declaration from EntityManager

/**
 * @brief Base class for all systems in the ECS
 * 
 * Systems contain the logic that operates on entities with specific
 * component combinations. Each system defines which components it
 * requires and processes entities that have those components.
 */
class System {
public:
    virtual ~System() = default;

    /**
     * @brief Update the system (called every frame)
     * @param deltaTime Time elapsed since last update
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Initialize the system
     * Called once when the system is first created
     */
    virtual void initialize() {}

    /**
     * @brief Shutdown the system
     * Called when the system is being destroyed
     */
    virtual void shutdown() {}

    /**
     * @brief Get the signature of components this system requires
     * @return The component signature
     */
    Signature getComponentSignature() const { return m_componentSignature; }

    /**
     * @brief Set the signature of components this system requires
     * @param signature The component signature
     */
    void setComponentSignature(Signature signature) { m_componentSignature = signature; }

    /**
     * @brief Get the set of entities this system operates on
     * @return Set of entities with the required components
     */
    const std::set<Entity>& getEntities() const { return m_entities; }

    /**
     * @brief Add an entity to this system
     * @param entity The entity to add
     */
    void addEntity(Entity entity) { m_entities.insert(entity); }

    /**
     * @brief Remove an entity from this system
     * @param entity The entity to remove
     */
    void removeEntity(Entity entity) { m_entities.erase(entity); }

    /**
     * @brief Check if this system contains an entity
     * @param entity The entity to check
     * @return True if the system contains the entity
     */
    bool hasEntity(Entity entity) const { return m_entities.count(entity) > 0; }

    /**
     * @brief Get the number of entities this system operates on
     * @return Number of entities
     */
    size_t getEntityCount() const { return m_entities.size(); }

protected:
    // Set of entities that this system operates on
    std::set<Entity> m_entities{};

    // Component signature that defines which components this system requires
    Signature m_componentSignature{};
};

} // namespace rude::ecs
