//==============================================================================
// RudeBase3D Engine - entity.hpp
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
 * @file entity.hpp
 * @brief 3D scene entity with hierarchical transforms and rendering
 * 
 * This file contains the Entity class which represents individual objects
 * in the 3D scene. Entities combine geometric data (meshes), spatial transforms,
 * hierarchical relationships, and rendering properties into a unified object
 * suitable for scene graph organization.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup Core
 */

#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QMatrix4x4>
#include "core/transform.hpp"
#include "core/qt_glm_utils.hpp"
#include "core/mesh_forward.hpp"

/**
 * @addtogroup Core
 * @{
 */

namespace rude {
    /**
     * @brief Enumeration of built-in primitive geometry types
     * 
     * Defines the standard geometric primitives that can be automatically
     * generated for entities. These primitives provide basic building blocks
     * for 3D scenes and modeling operations.
     * 
     * @note Custom meshes can be assigned using PrimitiveType::Unknown
     */
    enum class PrimitiveType { 
        Unknown,    ///< Custom or unspecified geometry
        None,       ///< No geometry (empty entity)
        Cube,       ///< Axis-aligned cube primitive
        Sphere,     ///< UV-sphere primitive
        Plane,      ///< Flat rectangular plane
        Cylinder,   ///< Cylindrical primitive
        Cone,       ///< Conical primitive
        Torus,      ///< Toroidal (donut-shaped) primitive
        Icosphere   ///< Geodesic sphere primitive
    };
    
    class Mesh; ///< Forward declaration for mesh class
}

/**
 * @brief Individual 3D object in the scene with transform hierarchy
 * 
 * The Entity class represents a single object in the 3D scene, combining
 * geometric data, spatial transformation, hierarchical relationships, and
 * rendering properties. Entities form the fundamental building blocks of
 * the scene graph in RudeBase3D.
 * 
 * ## Core Features
 * - **Geometric Data**: Mesh storage and primitive type management
 * - **Spatial Transform**: Position, rotation, scale in 3D space
 * - **Hierarchy Support**: Parent-child relationships for complex objects
 * - **Rendering Integration**: OpenGL rendering with transform matrices
 * - **Bounding Volume**: Axis-aligned bounding box for culling and queries
 * 
 * ## Transform Hierarchy
 * Entities support parent-child relationships where:
 * - Child transforms are relative to their parent
 * - World transforms are computed by concatenating parent transforms
 * - Moving a parent automatically moves all children
 * - Local transforms remain unchanged when reparenting
 * 
 * ## Coordinate Systems
 * - **Local Space**: Entity's own coordinate system
 * - **World Space**: Global scene coordinate system
 * - **Parent Space**: Coordinate system of the entity's parent
 * 
 * ## Usage Examples
 * ```cpp
 * // Create entity with primitive geometry
 * auto entity = std::make_unique<Entity>(1, rude::PrimitiveType::Cube, "MyCube");
 * 
 * // Transform operations
 * entity->setPosition({0, 0, 5});
 * entity->setRotation(glm::angleAxis(glm::radians(45.0f), glm::vec3(0,1,0)));
 * entity->setScale({2, 1, 1});
 * 
 * // Hierarchy
 * auto child = std::make_unique<Entity>(2, rude::PrimitiveType::Sphere, "Child");
 * entity->addChild(child.get());
 * 
 * // Rendering
 * entity->draw(viewMatrix, projMatrix);
 * ```
 * 
 * ## Performance Considerations
 * - Bounding boxes are cached and updated when transforms change
 * - World transform computation is deferred until needed
 * - Hierarchy operations have O(1) complexity for direct operations
 * - Deep hierarchies may impact transform computation performance
 * 
 * @see Scene for entity container and management
 * @see Transform for spatial transformation details
 * @see Mesh for geometric data storage
 * 
 * @ingroup Core
 */
class Entity {
public:
    // ========================================================================
    // Construction and Identity
    // ========================================================================
    
    /**
     * @brief Construct entity with specified primitive type
     * 
     * Creates a new entity with the given ID, primitive type, and optional name.
     * The entity is initialized with identity transform and no parent/children.
     * 
     * @param id Unique identifier for the entity
     * @param type Primitive type for automatic geometry generation
     * @param name Display name for the entity (defaults to "Entity")
     * 
     * @post Entity has identity transform (position=0, rotation=identity, scale=1)
     * @post Entity has no parent or children
     * @post Entity has generated or empty mesh based on primitive type
     * 
     * @note Entity IDs should be unique within a scene for proper lookup
     * @see getPrimitiveType() to retrieve the assigned primitive type
     */
    Entity(int id, rude::PrimitiveType type, const std::string& name = "Entity");
    
    /**
     * @brief Get the unique identifier of this entity
     * 
     * @return The entity's unique ID as assigned during construction
     * 
     * @note IDs are typically assigned by the Scene class for uniqueness
     */
    int getId() const;
    
    /**
     * @brief Get the display name of this entity
     * 
     * @return Const reference to the entity's name string
     * 
     * @note Names are not required to be unique and are primarily for user interface
     */
    const std::string& getName() const;
    
    /**
     * @brief Get the primitive type of this entity
     * 
     * @return The primitive type used for geometry generation
     * 
     * @note Returns PrimitiveType::Unknown for entities with custom meshes
     */
    rude::PrimitiveType getPrimitiveType() const;
    
    /**
     * @brief Get string representation of the entity type
     * 
     * Provides a human-readable string describing the entity's primitive type
     * or custom type designation for display in user interfaces.
     * 
     * @return String representation of the entity type
     * 
     * @note Returns "Custom" for PrimitiveType::Unknown
     * @note Useful for property panels and object browsers
     */
    std::string getType() const;
    
    // ========================================================================
    // Transform Management
    // ========================================================================
    
    /**
     * @brief Get mutable reference to the entity's transform
     * 
     * Provides direct access to the entity's transform component for
     * advanced manipulation and batch operations.
     * 
     * @return Mutable reference to the Transform object
     * 
     * @note Changes to the transform automatically invalidate cached world matrices
     * @see Transform class for detailed transformation operations
     */
    Transform& getTransform();
    
    /**
     * @brief Get the local position of the entity
     * 
     * @return 3D position vector in local/parent space
     * 
     * @note This is the position relative to the parent (or world if no parent)
     */
    glm::vec3 getPosition() const { 
        return transform.getPosition();
    }
    
    /**
     * @brief Set the local position of the entity
     * 
     * @param pos New position in local/parent space coordinates
     * 
     * @post Entity's local position is updated
     * @post World transform cache is invalidated
     * @post Bounding box is invalidated and will be recomputed
     */
    void setPosition(const glm::vec3& pos) { 
        transform.setPosition(pos);
    }
    
    /**
     * @brief Get the local rotation of the entity
     * 
     * @return Rotation quaternion in local/parent space
     * 
     * @note Quaternion represents rotation relative to parent's orientation
     */
    glm::quat getRotation() const { 
        return transform.getRotation();
    }
    
    /**
     * @brief Set the local rotation of the entity
     * 
     * @param rot New rotation quaternion in local/parent space
     * 
     * @post Entity's local rotation is updated
     * @post World transform cache is invalidated
     * @post Bounding box is invalidated and will be recomputed
     */
    void setRotation(const glm::quat& rot) { 
        transform.setRotation(rot);
    }
    
    /**
     * @brief Get the local scale of the entity
     * 
     * @return 3D scale vector (per-axis scaling factors)
     * 
     * @note Scale values of 1.0 represent no scaling along that axis
     */
    glm::vec3 getScale() const { 
        return transform.getScale();
    }
    
    /**
     * @brief Set the local scale of the entity
     * 
     * @param scale New scale vector (per-axis scaling factors)
     * 
     * @post Entity's local scale is updated
     * @post World transform cache is invalidated
     * @post Bounding box is invalidated and will be recomputed
     * 
     * @warning Zero or negative scale values may cause rendering artifacts
     */
    void setScale(const glm::vec3& scale) { 
        transform.setScale(scale);
    }
    
    /**
     * @brief Get the world-space position of the entity
     * 
     * Computes the entity's position in world coordinates by applying
     * all parent transforms in the hierarchy chain.
     * 
     * @return 3D position vector in world space coordinates
     * 
     * @note For root entities, world position equals local position
     * @note Computation cost increases with hierarchy depth
     */
    glm::vec3 getWorldPosition() const;
    
    /**
     * @brief Get the complete world transformation matrix
     * 
     * Computes the combined transformation matrix that converts from
     * the entity's local space to world space, including all parent transforms.
     * 
     * @return 4x4 transformation matrix (local-to-world)
     * 
     * @note Matrix includes translation, rotation, and scaling
     * @note Result is cached until transform hierarchy changes
     * @note For root entities, world transform equals local transform
     */
    glm::mat4 getWorldTransform() const;
    
    // ========================================================================
    // Geometry and Mesh Management
    // ========================================================================
    
    /**
     * @brief Get the mesh associated with this entity
     * 
     * Returns the geometric mesh data used for rendering this entity.
     * The mesh may be automatically generated from a primitive type or
     * manually assigned for custom geometry.
     * 
     * @return Shared pointer to the entity's mesh (may be null)
     * 
     * @note Null meshes result in no geometry being rendered
     * @note Mesh data is shared between entities for memory efficiency
     */
    rude::MeshPtr getMesh() const;
    
    /**
     * @brief Set a new mesh for this entity
     * 
     * Assigns new geometric data to the entity, replacing any existing mesh.
     * This allows switching between different geometric representations
     * or assigning custom meshes to primitive-based entities.
     * 
     * @param newMesh Shared pointer to the new mesh data
     * 
     * @post Entity's mesh is updated to the new value
     * @post Bounding box is invalidated and will be recomputed
     * 
     * @note Passing nullptr removes geometry from the entity
     * @note Mesh sharing enables efficient memory usage for duplicated objects
     */
    void setMesh(const rude::MeshPtr& newMesh) { mesh = newMesh; }
    
    /**
     * @brief Get the axis-aligned bounding box in world space
     * 
     * Computes the world-space bounding box that fully contains the entity's
     * geometry after applying all transformations. This is used for culling,
     * collision detection, and spatial queries.
     * 
     * @return Pair containing minimum and maximum bounds (min, max)
     * 
     * @note Bounding box includes effects of scale, rotation, and position
     * @note Empty entities return degenerate bounding boxes at the entity position
     * @note Results are cached until mesh or transform changes
     */
    std::pair<glm::vec3, glm::vec3> getAABB() const;
    
    // ========================================================================
    // Hierarchy Management
    // ========================================================================
    
    /**
     * @brief Get the parent entity in the hierarchy
     * 
     * @return Pointer to parent entity, or nullptr if this is a root entity
     * 
     * @note Root entities have no parent and use world space as their coordinate system
     */
    Entity* getParent() const { return parent; }
    
    /**
     * @brief Get all child entities
     * 
     * Returns a const reference to the vector containing all direct children
     * of this entity. Children inherit the parent's transform and move with it.
     * 
     * @return Const reference to vector of child entity pointers
     * 
     * @note Vector contains direct children only (not grandchildren)
     * @note Children maintain their own child lists for deeper hierarchies
     */
    const std::vector<Entity*>& getChildren() const { return children; }
    
    /**
     * @brief Add a child entity to this entity
     * 
     * Establishes a parent-child relationship where the child's transform
     * becomes relative to this entity's transform. The child is automatically
     * removed from its previous parent if it had one.
     * 
     * @param child Pointer to the entity to add as a child
     * 
     * @pre child must be a valid pointer to an existing entity
     * @pre child must not be this entity (no self-parenting)
     * @pre child must not be an ancestor of this entity (no cycles)
     * 
     * @post child's parent is set to this entity
     * @post child is added to this entity's children list
     * @post child's transform becomes relative to this entity
     * 
     * @note Adding a child does not change the child's world position immediately
     * @warning Circular hierarchies will cause infinite loops in transform computation
     */
    void addChild(Entity* child);
    
    /**
     * @brief Remove a child entity from this entity
     * 
     * Breaks the parent-child relationship, making the child a root entity.
     * The child's world transform is preserved by converting its relative
     * transform to world space.
     * 
     * @param child Pointer to the child entity to remove
     * 
     * @pre child must be a direct child of this entity
     * 
     * @post child's parent is set to nullptr
     * @post child is removed from this entity's children list
     * @post child's world position is preserved (local transform adjusted)
     * 
     * @note Does nothing if child is not actually a child of this entity
     */
    void removeChild(Entity* child);
    
    /**
     * @brief Change the parent of this entity
     * 
     * Moves this entity from its current parent (if any) to a new parent.
     * The entity's world transform is preserved during the reparenting operation.
     * 
     * @param newParent Pointer to the new parent entity (nullptr for root)
     * 
     * @pre newParent must not be this entity or a descendant of this entity
     * 
     * @post This entity's parent is changed to newParent
     * @post This entity is removed from old parent's children list
     * @post This entity is added to new parent's children list
     * @post World position is preserved (local transform adjusted)
     * 
     * @note Passing nullptr makes this entity a root entity
     * @warning Circular hierarchies will cause infinite loops in transform computation
     */
    void setParent(Entity* newParent);
    
    // ========================================================================
    // Rendering Operations
    // ========================================================================
    
    /**
     * @brief Render the entity with specified view and projection matrices
     * 
     * Performs OpenGL rendering of the entity's mesh using the provided
     * camera matrices and optional parent transform. This handles the complete
     * rendering pipeline including transform setup and mesh drawing.
     * 
     * @param view Camera view matrix (world-to-camera transform)
     * @param proj Camera projection matrix (camera-to-screen transform)
     * @param parentTransform Optional parent transform matrix for hierarchy rendering
     * 
     * @pre OpenGL context must be current and valid
     * @pre Shaders must be properly configured before calling
     * @pre Matrices must be valid (non-degenerate)
     * 
     * @note Does nothing if the entity has no mesh assigned
     * @note Automatically handles transform hierarchy by combining with parentTransform
     * @note Suitable for both individual entity rendering and scene graph traversal
     */
    void draw(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform = glm::mat4(1.0f)) const;

    // ========================================================================
    // Public Data Members
    // ========================================================================
    
    /**
     * @brief The geometric mesh data for rendering
     * 
     * Shared pointer to the mesh containing vertex data, indices, and
     * rendering state. Multiple entities can share the same mesh for
     * memory efficiency.
     * 
     * @note Made public for convenience during development
     * @note Consider making private with accessor methods for production code
     */
    rude::MeshPtr mesh;
    
    /**
     * @brief User-friendly display name
     * 
     * Human-readable name for the entity, used in user interfaces
     * and debugging. Names are not required to be unique.
     * 
     * @note Made public for easy access from UI components
     * @note Consider validation for special characters in production
     */
    std::string name;

private:
    // ========================================================================
    // Internal Data Members
    // ========================================================================
    
    /**
     * @brief Unique identifier for this entity
     * 
     * Numeric ID that uniquely identifies this entity within its scene.
     * Used for efficient lookup and serialization operations.
     */
    int id;
    
    /**
     * @brief Local spatial transformation
     * 
     * Contains position, rotation, and scale relative to the parent entity
     * (or world space for root entities). This is the core transform data
     * from which world transforms are computed.
     */
    Transform transform;
    
    /**
     * @brief The primitive type used to generate initial geometry
     * 
     * Records the type of primitive geometry that was used to create this
     * entity's initial mesh. Used for UI display and geometry regeneration.
     */
    rude::PrimitiveType primitiveType;
    
    /**
     * @brief Cached world-space bounding box bounds
     * 
     * Pre-computed minimum and maximum extents of the entity's geometry
     * in world coordinates. Updated when mesh or transforms change.
     */
    glm::vec3 aabb_min{0.0f}, aabb_max{0.0f};
    
    /**
     * @brief Pointer to parent entity in hierarchy
     * 
     * Null for root entities, otherwise points to the parent whose
     * transform serves as the coordinate system for this entity.
     */
    Entity* parent = nullptr;
    
    /**
     * @brief List of direct child entities
     * 
     * Vector containing pointers to all entities that are direct children
     * of this entity. Children inherit this entity's transform.
     */
    std::vector<Entity*> children;
};

/**
 * @}
 */
};