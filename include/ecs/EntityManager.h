#pragma once

#include "Entity.h"
#include <array>
#include <bitset>
#include <queue>
#include <cassert>

namespace rude::ecs {

/**
 * @brief Type alias for component signatures
 * 
 * A signature is a bitset that represents which components an entity has.
 * Each bit corresponds to a component type.
 */
using Signature = std::bitset<MAX_COMPONENTS>;

/**
 * @brief Manages entity creation, destruction, and signatures
 * 
 * The EntityManager is responsible for:
 * - Creating new entities
 * - Destroying entities
 * - Managing entity signatures (which components each entity has)
 * - Reusing entity IDs when entities are destroyed
 */
class EntityManager {
public:
    /**
     * @brief Constructor initializes the available entities queue
     */
    EntityManager() {
        // Initialize the queue with all possible entity IDs
        for (Entity entity = 1; entity < MAX_ENTITIES; ++entity) {
            m_availableEntities.push(entity);
        }
    }

    /**
     * @brief Create a new entity
     * @return The ID of the newly created entity
     */
    Entity createEntity() {
        assert(m_livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

        // Take an ID from the front of the queue
        Entity id = m_availableEntities.front();
        m_availableEntities.pop();
        ++m_livingEntityCount;

        return id;
    }

    /**
     * @brief Destroy an entity
     * @param entity The entity to destroy
     */
    void destroyEntity(Entity entity) {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        // Invalidate the destroyed entity's signature
        m_signatures[entity].reset();

        // Put the destroyed ID at the back of the queue
        m_availableEntities.push(entity);
        --m_livingEntityCount;
    }

    /**
     * @brief Set the signature for an entity
     * @param entity The entity to set the signature for
     * @param signature The new signature
     */
    void setSignature(Entity entity, Signature signature) {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        m_signatures[entity] = signature;
    }

    /**
     * @brief Get the signature for an entity
     * @param entity The entity to get the signature for
     * @return The entity's signature
     */
    Signature getSignature(Entity entity) const {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        return m_signatures[entity];
    }

    /**
     * @brief Get the number of living entities
     * @return Number of currently existing entities
     */
    std::uint32_t getLivingEntityCount() const {
        return m_livingEntityCount;
    }

    /**
     * @brief Check if an entity is valid (exists)
     * @param entity The entity to check
     * @return True if the entity exists
     */
    bool isEntityValid(Entity entity) const {
        if (entity == NULL_ENTITY || entity >= MAX_ENTITIES) {
            return false;
        }
        
        // If the entity has any components, it's valid
        return m_signatures[entity].any();
    }

private:
    // Queue of unused entity IDs
    std::queue<Entity> m_availableEntities{};

    // Array of signatures where the index corresponds to the entity ID
    std::array<Signature, MAX_ENTITIES> m_signatures{};

    // Total living entities - used to keep limits on how many exist
    std::uint32_t m_livingEntityCount{};
};

} // namespace rude::ecs
