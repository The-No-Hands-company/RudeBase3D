#pragma once

#include "Entity.h"
#include <array>
#include <unordered_map>
#include <cassert>

namespace rude::ecs {

/**
 * @brief Interface for component arrays to allow polymorphic storage
 */
class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(Entity entity) = 0;
};

/**
 * @brief Packed array for storing components of a specific type
 * 
 * This class provides efficient storage for components by keeping them
 * in a contiguous array for cache-friendly access patterns.
 * 
 * @tparam T The component type to store
 */
template<typename T>
class ComponentArray : public IComponentArray {
public:
    /**
     * @brief Insert a component for an entity
     * @param entity The entity to associate the component with
     * @param component The component data to store
     */
    void insertData(Entity entity, T component) {
        assert(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end() && 
               "Component added to same entity more than once.");

        // Put new entry at end and update the maps
        size_t newIndex = m_size;
        m_entityToIndexMap[entity] = newIndex;
        m_indexToEntityMap[newIndex] = entity;
        m_componentArray[newIndex] = component;
        ++m_size;
    }

    /**
     * @brief Remove a component from an entity
     * @param entity The entity to remove the component from
     */
    void removeData(Entity entity) {
        assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && 
               "Removing non-existent component.");

        // Copy element at end into deleted element's place to maintain density
        size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
        size_t indexOfLastElement = m_size - 1;
        m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

        // Update map to point to moved spot
        Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
        m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        m_entityToIndexMap.erase(entity);
        m_indexToEntityMap.erase(indexOfLastElement);

        --m_size;
    }

    /**
     * @brief Get a component for an entity
     * @param entity The entity to get the component for
     * @return Reference to the component data
     */
    T& getData(Entity entity) {
        assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && 
               "Retrieving non-existent component.");

        return m_componentArray[m_entityToIndexMap[entity]];
    }

    /**
     * @brief Check if an entity has this component type
     * @param entity The entity to check
     * @return True if the entity has this component type
     */
    bool hasData(Entity entity) const {
        return m_entityToIndexMap.find(entity) != m_entityToIndexMap.end();
    }

    /**
     * @brief Called when an entity is destroyed
     * @param entity The entity that was destroyed
     */
    void entityDestroyed(Entity entity) override {
        if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end()) {
            removeData(entity);
        }
    }

    /**
     * @brief Get the number of components stored
     * @return Number of components
     */
    size_t size() const { return m_size; }

    /**
     * @brief Get direct access to the component array for systems
     * @return Pointer to the component array
     */
    T* data() { return m_componentArray.data(); }
    const T* data() const { return m_componentArray.data(); }

private:
    // The packed array of components (of generic type T),
    // set to a specified maximum amount, matching the maximum number
    // of entities allowed to exist simultaneously, so that each entity
    // has a unique spot.
    std::array<T, MAX_ENTITIES> m_componentArray{};

    // Map from an entity ID to an array index.
    std::unordered_map<Entity, size_t> m_entityToIndexMap{};

    // Map from an array index to an entity ID.
    std::unordered_map<size_t, Entity> m_indexToEntityMap{};

    // Total size of valid entries in the array.
    size_t m_size{};
};

} // namespace rude::ecs
