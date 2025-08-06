#pragma once

/**
 * @file ECS.h
 * @brief Main include file for the RudeBase3D Entity Component System
 * 
 * This file provides a convenient way to include all ECS components
 * and systems. Include this file to get access to the complete ECS API.
 */

// Core ECS classes
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "World.h"

// Managers (typically not used directly, but available if needed)
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "ComponentArray.h"

// Core components for 3D applications
#include "components/CoreComponents.h"

// Core systems for 3D applications
#include "systems/CoreSystems.h"

/**
 * @namespace rude::ecs
 * @brief Namespace containing all ECS-related classes and functions
 * 
 * The ECS (Entity Component System) is an architectural pattern used in
 * game development and 3D applications. It promotes composition over
 * inheritance and provides better data locality and performance.
 * 
 * Key concepts:
 * - Entity: A unique identifier that groups components together
 * - Component: Pure data containers that define what an entity has
 * - System: Logic that operates on entities with specific component combinations
 * - World: The main coordinator that manages entities, components, and systems
 * 
 * Basic usage:
 * @code
 * using namespace rude::ecs;
 * 
 * // Create a world
 * World world;
 * 
 * // Register component types
 * world.registerComponent<TransformComponent>();
 * world.registerComponent<MeshComponent>();
 * 
 * // Register and setup systems
 * auto renderSystem = world.registerSystem<RenderSystem>();
 * Signature renderSignature;
 * renderSignature.set(world.getComponentType<TransformComponent>());
 * renderSignature.set(world.getComponentType<MeshComponent>());
 * world.setSystemSignature<RenderSystem>(renderSignature);
 * 
 * // Create an entity
 * Entity entity = world.createEntity();
 * 
 * // Add components
 * world.addComponent(entity, TransformComponent{});
 * world.addComponent(entity, MeshComponent{someMesh});
 * 
 * // Update the world (typically called each frame)
 * world.update(deltaTime);
 * @endcode
 */

namespace rude::ecs {

/**
 * @brief Helper function to create a signature for multiple component types
 * @tparam ComponentTypes The component types to include in the signature
 * @param world Reference to the world to get component type IDs from
 * @return Signature with the specified component types set
 */
template<typename... ComponentTypes>
Signature createSignature(World& world) {
    Signature signature;
    (signature.set(world.getComponentType<ComponentTypes>()), ...);
    return signature;
}

/**
 * @brief Helper function to check if an entity has all specified components
 * @tparam ComponentTypes The component types to check for
 * @param world Reference to the world
 * @param entity The entity to check
 * @return True if the entity has all specified component types
 */
template<typename... ComponentTypes>
bool hasComponents(World& world, Entity entity) {
    return (world.hasComponent<ComponentTypes>(entity) && ...);
}

/**
 * @brief Helper function to add multiple components to an entity at once
 * @tparam ComponentTypes The component types to add
 * @param world Reference to the world
 * @param entity The entity to add components to
 * @param components The component instances to add
 */
template<typename... ComponentTypes>
void addComponents(World& world, Entity entity, ComponentTypes&&... components) {
    (world.addComponent<ComponentTypes>(entity, std::forward<ComponentTypes>(components)), ...);
}

} // namespace rude::ecs
