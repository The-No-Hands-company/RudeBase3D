//==============================================================================
// RudeBase3D Engine - scene.hpp
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

/**
 * @file scene.hpp
 * @brief Scene graph management and entity organization
 * 
 * This file contains the Scene class which manages the hierarchical organization
 * of 3D entities, cameras, lights, and other scene objects. The scene graph
 * provides spatial organization, rendering order management, and efficient
 * culling for complex 3D scenes.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup Core
 */

#pragma once
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include "entity.hpp"
#include <glm/glm.hpp>
#include "primitive_manager.hpp"

/**
 * @addtogroup Core
 * @{
 */

namespace rude {

/**
 * @brief Scene graph manager for 3D entities and spatial organization
 * 
 * The Scene class provides a hierarchical container for organizing 3D entities,
 * managing their relationships, and coordinating rendering operations. It serves
 * as the central hub for all scene-related operations in RudeBase3D.
 * 
 * ## Core Responsibilities
 * - **Entity Management**: Creation, deletion, and organization of scene entities
 * - **Hierarchical Structure**: Parent-child relationships for complex objects
 * - **Rendering Coordination**: Efficient batching and culling of visible objects
 * - **Spatial Queries**: Fast lookup and intersection testing
 * - **State Management**: Scene-wide settings and configurations
 * 
 * ## Scene Graph Architecture
 * The scene maintains a forest of entity trees where each entity can have:
 * - Multiple children (child entities)
 * - One parent (or null for root entities)
 * - Local and world transforms
 * - Visibility and rendering properties
 * 
 * ## Performance Optimization
 * - **Frustum Culling**: Only visible entities are processed for rendering
 * - **Spatial Partitioning**: Efficient spatial queries and collision detection
 * - **Batch Rendering**: Similar objects are batched for optimal GPU usage
 * - **Level-of-Detail**: Automatic LOD selection based on distance
 * 
 * ## Usage Patterns
 * ```cpp
 * // Create and populate scene
 * auto scene = std::make_unique<rude::Scene>();
 * 
 * // Add entities
 * auto cube = scene->createEntity(rude::PrimitiveType::Cube, "MyCube");
 * auto sphere = scene->createEntity(rude::PrimitiveType::Sphere, "MySphere");
 * 
 * // Organize hierarchy
 * cube->addChild(sphere);
 * 
 * // Render scene
 * scene->draw(viewMatrix, projectionMatrix);
 * ```
 * 
 * ## Thread Safety
 * Scene operations are not thread-safe. All modifications should occur
 * on the main thread. For multi-threaded rendering, consider using
 * render command buffers or separate scene instances.
 * 
 * @see Entity for individual scene objects
 * @see Camera for viewport management
 * @see SceneManager for multi-scene coordination
 * 
 * @ingroup Core
 */
class Scene {
public:
    // ========================================================================
    // Construction and Destruction
    // ========================================================================
    
    /**
     * @brief Default constructor
     * 
     * Creates an empty scene with no entities. The scene is ready for
     * entity creation and rendering operations.
     */
    Scene();
    
    // ========================================================================
    // Entity Management
    // ========================================================================
    
    /**
     * @brief Create a new entity with specified primitive type
     * 
     * Creates a new entity in the scene with the specified primitive geometry.
     * The entity is automatically added to the scene's entity list and
     * assigned a unique identifier.
     * 
     * @param type The primitive type for the entity's initial geometry
     * @param name Optional name for the entity (auto-generated if empty)
     * 
     * @return Pointer to the newly created entity (never null)
     * 
     * @post Entity is added to scene and ready for use
     * @post Entity has valid unique ID and transform
     * 
     * @note The returned pointer remains valid until the entity is removed
     *       from the scene or the scene is destroyed
     * 
     * @see removeEntity() to remove entities
     * @see PrimitiveType for available primitive types
     */
    Entity* createEntity(rude::PrimitiveType type, const std::string& name = "");
    
    /**
     * @brief Get read-only access to all entities in the scene
     * 
     * Returns a const reference to the internal entity storage, allowing
     * iteration over all entities without modification capabilities.
     * 
     * @return Const reference to entity container
     * 
     * @note Entities are stored as unique_ptr for automatic memory management
     * @note The container may be reallocated when entities are added/removed
     */
    const std::vector<std::unique_ptr<Entity>>& getEntities() const;
    
    /**
     * @brief Get all root entities (entities with no parent)
     * 
     * Returns a vector of pointers to entities that have no parent,
     * representing the top level of the scene hierarchy.
     * 
     * @return Vector of root entity pointers
     * 
     * @note Useful for traversing the scene hierarchy from the top
     * @note Results may change when entity parent-child relationships change
     */
    std::vector<Entity*> getRootEntities() const;
    
    /**
     * @brief Remove an entity from the scene
     * 
     * Removes the specified entity and all its children from the scene.
     * This operation invalidates any pointers to the removed entities.
     * 
     * @param entity Pointer to the entity to remove (must be valid)
     * 
     * @pre entity must be a valid pointer to an entity in this scene
     * @post entity and all its children are removed from the scene
     * @post All pointers to removed entities become invalid
     * 
     * @warning Using entity pointers after removal results in undefined behavior
     */
    void removeEntity(Entity* entity);
    
    /**
     * @brief Clear all entities from the scene
     * 
     * Removes all entities from the scene, effectively resetting it to
     * an empty state. This operation invalidates all entity pointers.
     * 
     * @post Scene contains no entities
     * @post All entity pointers become invalid
     * 
     * @note More efficient than removing entities individually
     */
    void clear();
    
    /**
     * @brief Find an entity by its unique identifier
     * 
     * Searches the scene for an entity with the specified ID.
     * This operation performs a linear search through all entities.
     * 
     * @param id The unique identifier to search for
     * 
     * @return Pointer to the entity if found, nullptr otherwise
     * 
     * @complexity O(n) where n is the number of entities in the scene
     * 
     * @note For frequent lookups, consider maintaining your own ID->Entity map
     */
    Entity* findEntityById(int id) const;
    
    /**
     * @brief Find an entity by its display name
     * 
     * Searches the scene for an entity with the specified name.
     * Names are not required to be unique, so this returns the first match.
     * 
     * @param name The display name to search for
     * 
     * @return Pointer to the first entity with matching name, nullptr if not found
     * 
     * @complexity O(n) where n is the number of entities in the scene
     * 
     * @note For performance-critical lookups, consider using entity IDs instead
     * @see findEntityById() for unique identifier lookup
     */
    Entity* findEntityByName(const std::string& name) const;
    
    // ========================================================================
    // Rendering Operations
    // ========================================================================
    
    /**
     * @brief Render the entire scene with specified view and projection matrices
     * 
     * Performs a complete rendering pass of the scene, including frustum culling,
     * depth sorting, and batch rendering optimization. Only visible entities
     * within the view frustum are rendered.
     * 
     * @param view View matrix (camera transform)
     * @param proj Projection matrix (perspective/orthographic)
     * 
     * @pre OpenGL context must be current and valid
     * @pre Matrices must be valid (non-degenerate)
     * 
     * @note This method handles the complete rendering pipeline:
     *       - Frustum culling for performance
     *       - Depth sorting for transparency
     *       - Batch rendering for efficiency
     *       - LOD selection based on distance
     */
    void draw(const glm::mat4& view, const glm::mat4& proj);
    
    // ========================================================================
    // Scene Queries and Utilities
    // ========================================================================
    
    /**
     * @brief Get the center point of the scene's bounding box
     * 
     * Calculates the geometric center of all entities in the scene by
     * computing the combined bounding box and returning its center point.
     * 
     * @return World-space center point of scene bounding box
     * 
     * @note Useful for camera auto-framing and scene navigation
     * @note Computation includes all entities regardless of visibility
     * @note Returns origin (0,0,0) for empty scenes
     */
    glm::vec3 getSceneBoundingBoxCenter() const;
    
    /**
     * @brief Get the extents of the scene's bounding box
     * 
     * Calculates the minimum and maximum bounds of all entities in the scene,
     * providing the complete spatial extents for culling and camera positioning.
     * 
     * @return Pair containing min and max bounds of scene
     * 
     * @note Useful for automatic camera positioning and frustum calculation
     * @note Includes all entities regardless of visibility or active state
     */
    std::pair<glm::vec3, glm::vec3> getSceneBoundingBox() const;
    
    /**
     * @brief Check if the scene contains any entities
     * 
     * @return true if the scene has one or more entities, false otherwise
     * 
     * @note More efficient than checking getEntities().size() > 0
     */
    bool isEmpty() const;
    
    /**
     * @brief Get the total number of entities in the scene
     * 
     * @return Count of all entities in the scene
     * 
     * @note Includes all entities regardless of hierarchy level
     */
    size_t getEntityCount() const;

private:
    // ========================================================================
    // Internal Data Members
    // ========================================================================
    
    /**
     * @brief Container for all entities in the scene
     * 
     * Stores entities as unique_ptr for automatic memory management.
     * The scene owns all entities and is responsible for their lifecycle.
     */
    std::vector<std::unique_ptr<Entity>> entities_;
    
    /**
     * @brief Next available entity ID for assignment
     * 
     * Provides unique IDs for entity identification and lookup.
     * Incremented for each new entity created.
     */
    int nextEntityId_;
    
    /**
     * @brief Counter for auto-generated entity names
     * 
     * Used to ensure uniqueness when auto-generating entity names.
     * Incremented for each unnamed entity created.
     */
    int entityNameCounter_;
    
    // ========================================================================
    // Internal Helper Methods
    // ========================================================================
    
    /**
     * @brief Generate a unique name for unnamed entities
     * 
     * Creates a descriptive name based on the entity's primitive type
     * and a unique counter to ensure name uniqueness within the scene.
     * 
     * @param type The primitive type of the entity
     * 
     * @return Unique name string for the entity
     */
    std::string generateEntityName(rude::PrimitiveType type);
};

} // namespace rude

/**
 * @}
 */