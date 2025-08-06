#include "core/core_system.hpp"
#include "core/scene_manager.hpp"
#include "core/selection_manager.hpp"
#include "core/primitive_manager.hpp"
#include "ecs/ECSManager.h"
#include <spdlog/spdlog.h>

CoreSystem::CoreSystem() {
    spdlog::info("CoreSystem initializing...");
}

CoreSystem::~CoreSystem() {
    spdlog::info("CoreSystem shutting down...");
}

void CoreSystem::initialize() {
    spdlog::info("CoreSystem::initialize() - Setting up core systems");
    
    // Create ECS manager first as other systems may depend on it
    m_ecsManager = std::make_unique<rude::ecs::ECSManager>();
    if (!m_ecsManager->initialize()) {
        spdlog::critical("Failed to initialize ECS Manager!");
        return;
    }
    spdlog::info("ECS Manager initialized successfully");
    
    // Create scene manager
    m_sceneManager = std::make_unique<rude::SceneManager>();
    
    // Create selection manager
    m_selectionManager = std::make_unique<rude::SelectionManager>();
    
    // Create a scene and set it in the scene manager
    auto scene = std::make_shared<rude::Scene>();
    m_sceneManager->setScene(scene);
    
    // Connect selection manager to scene
    if (m_sceneManager && m_selectionManager) {
        // Link the selection manager to the scene manager
        // This enables selection manager to listen for scene changes
        m_selectionManager->setSceneManager(m_sceneManager.get());
        
    spdlog::info("Connected SelectionManager to SceneManager");
    }
    
    spdlog::info("Core systems initialized successfully");
    spdlog::info("CoreSystem::initialize() - Core systems ready");
    
    // Print ECS statistics
    spdlog::info("ECS Statistics:");
    spdlog::info("{}", m_ecsManager->getStatistics());
}

void CoreSystem::update(float deltaTime) {
    // Update ECS systems
    if (m_ecsManager) {
        m_ecsManager->update(deltaTime);
    }
    
    // Update other systems as needed
    // Scene manager and selection manager are typically event-driven
}

void CoreSystem::shutdown() {
    spdlog::info("CoreSystem::shutdown() - Shutting down core systems");
    
    // Clear selection first to avoid dangling references
    if (m_selectionManager) {
        m_selectionManager->clearSelection();
    }
    
    // Release scene
    if (m_sceneManager) {
        m_sceneManager->setScene(nullptr);
    }
    
    // Shutdown ECS manager
    if (m_ecsManager) {
        m_ecsManager->shutdown();
    }
    
    // Destroy managers in reverse order of creation
    m_ecsManager.reset();
    m_selectionManager.reset();
    m_sceneManager.reset();
    
    spdlog::info("Core systems shut down successfully");
}
