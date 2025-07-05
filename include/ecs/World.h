#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include <memory>

namespace rude::ecs {

/**
 * @brief The main ECS coordinator that manages all ECS operations
 * 
 * The World class is the primary interface for the ECS system.
 * It coordinates between the EntityManager, ComponentManager, and SystemManager
 * to provide a unified API for working with entities, components, and systems.
 */
class World {
public:
    /**
     * @brief Constructor initializes all managers
     */
    World() {
        m_componentManager = std::make_unique<ComponentManager>();
        m_entityManager = std::make_unique<EntityManager>();
        m_systemManager = std::make_unique<SystemManager>();
    }

    // ===========================================
    // Entity methods
    // ===========================================

    /**
     * @brief Create a new entity
     * @return The ID of the newly created entity
     */
    Entity createEntity() {
        return m_entityManager->createEntity();
    }

    /**
     * @brief Destroy an entity
     * @param entity The entity to destroy
     */
    void destroyEntity(Entity entity) {
        m_entityManager->destroyEntity(entity);
        m_componentManager->entityDestroyed(entity);
        m_systemManager->entityDestroyed(entity);
    }

    /**
     * @brief Check if an entity is valid
     * @param entity The entity to check
     * @return True if the entity exists
     */
    bool isEntityValid(Entity entity) const {
        return m_entityManager->isEntityValid(entity);
    }

    /**
     * @brief Get the number of living entities
     * @return Number of currently existing entities
     */
    std::uint32_t getLivingEntityCount() const {
        return m_entityManager->getLivingEntityCount();
    }

    // ===========================================
    // Component methods
    // ===========================================

    /**
     * @brief Register a new component type
     * @tparam T The component type to register
     */
    template<typename T>
    void registerComponent() {
        m_componentManager->registerComponent<T>();
    }

    /**
     * @brief Add a component to an entity
     * @tparam T The component type
     * @param entity The entity to add the component to
     * @param component The component data
     */
    template<typename T>
    void addComponent(Entity entity, T component) {
        m_componentManager->addComponent<T>(entity, component);

        auto signature = m_entityManager->getSignature(entity);
        signature.set(m_componentManager->getComponentType<T>(), true);
        m_entityManager->setSignature(entity, signature);

        m_systemManager->entitySignatureChanged(entity, signature);
    }

    /**
     * @brief Remove a component from an entity
     * @tparam T The component type
     * @param entity The entity to remove the component from
     */
    template<typename T>
    void removeComponent(Entity entity) {
        m_componentManager->removeComponent<T>(entity);

        auto signature = m_entityManager->getSignature(entity);
        signature.set(m_componentManager->getComponentType<T>(), false);
        m_entityManager->setSignature(entity, signature);

        m_systemManager->entitySignatureChanged(entity, signature);
    }

    /**
     * @brief Get a component from an entity
     * @tparam T The component type
     * @param entity The entity to get the component from
     * @return Reference to the component data
     */
    template<typename T>
    T& getComponent(Entity entity) {
        return m_componentManager->getComponent<T>(entity);
    }

    /**
     * @brief Check if an entity has a component type
     * @tparam T The component type
     * @param entity The entity to check
     * @return True if the entity has this component type
     */
    template<typename T>
    bool hasComponent(Entity entity) const {
        return m_componentManager->hasComponent<T>(entity);
    }

    /**
     * @brief Get the component type ID for a component type
     * @tparam T The component type
     * @return The type ID for this component type
     */
    template<typename T>
    std::uint32_t getComponentType() {
        return m_componentManager->getComponentType<T>();
    }

    // ===========================================
    // System methods
    // ===========================================

    /**
     * @brief Register a new system
     * @tparam T The system type to register
     * @return Shared pointer to the created system
     */
    template<typename T>
    std::shared_ptr<T> registerSystem() {
        return m_systemManager->registerSystem<T>();
    }

    /**
     * @brief Get a registered system
     * @tparam T The system type to get
     * @return Shared pointer to the system
     */
    template<typename T>
    std::shared_ptr<T> getSystem() {
        return m_systemManager->getSystem<T>();
    }

    /**
     * @brief Set the signature for a system
     * @tparam T The system type
     * @param signature The component signature the system requires
     */
    template<typename T>
    void setSystemSignature(Signature signature) {
        m_systemManager->setSignature<T>(signature);
    }

    /**
     * @brief Update all systems
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime) {
        m_systemManager->updateSystems(deltaTime);
    }

    /**
     * @brief Initialize all systems
     */
    void initialize() {
        m_systemManager->initializeSystems();
    }

    /**
     * @brief Shutdown all systems
     */
    void shutdown() {
        m_systemManager->shutdownSystems();
    }

    /**
     * @brief Get system count
     * @return Number of registered systems
     */
    size_t getSystemCount() const {
        return m_systemManager->getSystemCount();
    }

private:
    std::unique_ptr<ComponentManager> m_componentManager;
    std::unique_ptr<EntityManager> m_entityManager;
    std::unique_ptr<SystemManager> m_systemManager;
};

} // namespace rude::ecs
