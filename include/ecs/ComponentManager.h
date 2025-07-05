#pragma once

#include "Component.h"
#include "ComponentArray.h"
#include <memory>
#include <unordered_map>
#include <typeinfo>

namespace rude::ecs {

/**
 * @brief Manages all component types and their storage
 * 
 * The ComponentManager is responsible for:
 * - Registering new component types
 * - Creating and managing component arrays
 * - Providing access to component data
 * - Handling component cleanup when entities are destroyed
 */
class ComponentManager {
public:
    /**
     * @brief Register a new component type
     * @tparam T The component type to register
     */
    template<typename T>
    void registerComponent() {
        const char* typeName = typeid(T).name();

        assert(m_componentTypes.find(typeName) == m_componentTypes.end() && 
               "Registering component type more than once.");

        // Add this component type to the component type map
        m_componentTypes.insert({typeName, m_nextComponentType});
        
        // Create a ComponentArray pointer and add it to the component arrays map
        m_componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

        // Increment the value so that the next component registered will be different
        ++m_nextComponentType;
    }

    /**
     * @brief Get the type ID for a component type
     * @tparam T The component type
     * @return The type ID for this component type
     */
    template<typename T>
    std::uint32_t getComponentType() {
        const char* typeName = typeid(T).name();

        assert(m_componentTypes.find(typeName) != m_componentTypes.end() && 
               "Component not registered before use.");

        return m_componentTypes[typeName];
    }

    /**
     * @brief Add a component to an entity
     * @tparam T The component type
     * @param entity The entity to add the component to
     * @param component The component data
     */
    template<typename T>
    void addComponent(Entity entity, T component) {
        getComponentArray<T>()->insertData(entity, component);
    }

    /**
     * @brief Remove a component from an entity
     * @tparam T The component type
     * @param entity The entity to remove the component from
     */
    template<typename T>
    void removeComponent(Entity entity) {
        getComponentArray<T>()->removeData(entity);
    }

    /**
     * @brief Get a component from an entity
     * @tparam T The component type
     * @param entity The entity to get the component from
     * @return Reference to the component data
     */
    template<typename T>
    T& getComponent(Entity entity) {
        return getComponentArray<T>()->getData(entity);
    }

    /**
     * @brief Check if an entity has a component type
     * @tparam T The component type
     * @param entity The entity to check
     * @return True if the entity has this component type
     */
    template<typename T>
    bool hasComponent(Entity entity) const {
        return getComponentArray<T>()->hasData(entity);
    }

    /**
     * @brief Called when an entity is destroyed
     * @param entity The entity that was destroyed
     */
    void entityDestroyed(Entity entity) {
        // Notify each component array that an entity has been destroyed
        // If it has a component for that entity, it will remove it
        for (auto const& pair : m_componentArrays) {
            auto const& component = pair.second;
            component->entityDestroyed(entity);
        }
    }

private:
    // Map from type string pointer to a component type
    std::unordered_map<const char*, std::uint32_t> m_componentTypes{};

    // Map from type string pointer to a component array
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_componentArrays{};

    // The component type to be assigned to the next registered component - starting at 0
    std::uint32_t m_nextComponentType{};

    /**
     * @brief Get the component array for a specific component type
     * @tparam T The component type
     * @return Shared pointer to the component array
     */
    template<typename T>
    std::shared_ptr<ComponentArray<T>> getComponentArray() {
        const char* typeName = typeid(T).name();

        assert(m_componentTypes.find(typeName) != m_componentTypes.end() && 
               "Component not registered before use.");

        return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
    }

    /**
     * @brief Get the component array for a specific component type (const version)
     * @tparam T The component type
     * @return Shared pointer to the component array
     */
    template<typename T>
    std::shared_ptr<const ComponentArray<T>> getComponentArray() const {
        const char* typeName = typeid(T).name();

        assert(m_componentTypes.find(typeName) != m_componentTypes.end() && 
               "Component not registered before use.");

        return std::static_pointer_cast<const ComponentArray<T>>(m_componentArrays.at(typeName));
    }
};

} // namespace rude::ecs
