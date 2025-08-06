#pragma once

#include "System.h"
#include "Entity.h"
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <cassert>

namespace rude::ecs {

/**
 * @brief Manages all systems in the ECS
 * 
 * The SystemManager is responsible for:
 * - Registering new system types
 * - Creating and managing system instances
 * - Updating systems each frame
 * - Managing entity-system relationships
 */
class SystemManager {
public:
    /**
     * @brief Register a new system type
     * @tparam T The system type to register
     * @return Shared pointer to the created system
     */
    template<typename T>
    std::shared_ptr<T> registerSystem() {
        const char* typeName = typeid(T).name();

        assert(m_systems.find(typeName) == m_systems.end() && 
               "Registering system more than once.");

        // Create a pointer to the system and return it so it can be used externally
        auto system = std::make_shared<T>();
        m_systems.insert({typeName, system});
        return system;
    }

    /**
     * @brief Get a registered system
     * @tparam T The system type to get
     * @return Shared pointer to the system
     */
    template<typename T>
    std::shared_ptr<T> getSystem() {
        const char* typeName = typeid(T).name();

        assert(m_systems.find(typeName) != m_systems.end() && 
               "System not registered before use.");

        return std::static_pointer_cast<T>(m_systems[typeName]);
    }

    /**
     * @brief Set the signature for a system
     * @tparam T The system type
     * @param signature The component signature the system requires
     */
    template<typename T>
    void setSignature(Signature signature) {
        const char* typeName = typeid(T).name();

        assert(m_systems.find(typeName) != m_systems.end() && 
               "System used before registered.");

        m_signatures.insert({typeName, signature});
    }

    /**
     * @brief Called when an entity's signature changes
     * @param entity The entity whose signature changed
     * @param entitySignature The new signature of the entity
     */
    void entitySignatureChanged(Entity entity, Signature entitySignature) {
        // Notify each system that an entity's signature changed
        for (auto const& pair : m_systems) {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = m_signatures[type];

            // Entity signature matches system signature - insert into set
            if ((entitySignature & systemSignature) == systemSignature) {
                system->addEntity(entity);
            }
            // Entity signature does not match system signature - erase from set
            else {
                system->removeEntity(entity);
            }
        }
    }

    /**
     * @brief Called when an entity is destroyed
     * @param entity The entity that was destroyed
     */
    void entityDestroyed(Entity entity) {
        // Erase a destroyed entity from all system lists
        // m_entities is a set so no check needed
        for (auto const& pair : m_systems) {
            auto const& system = pair.second;
            system->removeEntity(entity);
        }
    }

    /**
     * @brief Update all systems
     * @param deltaTime Time elapsed since last update
     */
    void updateSystems(float deltaTime) {
        for (auto const& pair : m_systems) {
            pair.second->update(deltaTime);
        }
    }

    /**
     * @brief Initialize all systems
     */
    void initializeSystems() {
        for (auto const& pair : m_systems) {
            pair.second->initialize();
        }
    }

    /**
     * @brief Shutdown all systems
     */
    void shutdownSystems() {
        for (auto const& pair : m_systems) {
            pair.second->shutdown();
        }
    }

    /**
     * @brief Get the number of registered systems
     * @return Number of systems
     */
    size_t getSystemCount() const {
        return m_systems.size();
    }

private:
    // Map from system type string to a signature
    std::unordered_map<const char*, Signature> m_signatures{};

    // Map from system type string to a system pointer
    std::unordered_map<const char*, std::shared_ptr<System>> m_systems{};
};

} // namespace rude::ecs
