#pragma once

#include "core/scene_manager.hpp"
#include "core/selection_manager.hpp"
#include "ecs/ECSManager.h"
#include <memory>

/**
 * @brief Core singleton that provides access to the main system managers
 * 
 * This class serves as a central access point to key system managers,
 * allowing components throughout the application to easily access them
 * without complex dependency injection.
 */
class CoreSystem {
public:
    static CoreSystem& getInstance() {
        static CoreSystem instance;
        return instance;
    }

    // Delete copy/move constructors and assignment operators
    CoreSystem(const CoreSystem&) = delete;
    CoreSystem& operator=(const CoreSystem&) = delete;
    CoreSystem(CoreSystem&&) = delete;
    CoreSystem& operator=(CoreSystem&&) = delete;

    // Manager accessors
    SceneManager* getSceneManager() const { return m_sceneManager.get(); }
    rude::SelectionManager* getSelectionManager() const { return m_selectionManager.get(); }
    rude::ecs::ECSManager* getECSManager() const { return m_ecsManager.get(); }

    // Initialize all core systems
    void initialize();
    
    // Shutdown all core systems
    void shutdown();
    
    // Update all core systems (called each frame)
    void update(float deltaTime);

private:
    // Private constructor for singleton
    CoreSystem();
    ~CoreSystem();

    // Core system managers
    std::unique_ptr<SceneManager> m_sceneManager;
    std::unique_ptr<rude::SelectionManager> m_selectionManager;
    std::unique_ptr<rude::ecs::ECSManager> m_ecsManager;
    
    // Other managers will be added as needed:
    // - PrimitiveManager
    // - MeshOperationManager
    // - etc.
};

// Convenience function for accessing the CoreSystem singleton
inline CoreSystem& getCoreSystem() {
    return CoreSystem::getInstance();
}
