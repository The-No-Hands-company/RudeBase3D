#include "core/core_system.hpp"
#include "core/scene_manager.hpp"
#include "core/selection_manager.hpp"
#include "core/primitive_manager.hpp"
#include <QDebug>

CoreSystem::CoreSystem() {
    qDebug() << "CoreSystem initializing...";
}

CoreSystem::~CoreSystem() {
    qDebug() << "CoreSystem shutting down...";
}

void CoreSystem::initialize() {
    qDebug() << "CoreSystem::initialize() - Setting up core systems";
    
    // Create scene manager
    m_sceneManager = std::make_unique<SceneManager>();
    
    // Create selection manager
    m_selectionManager = std::make_unique<rude::SelectionManager>();
    
    // Create a scene and set it in the scene manager
    auto scene = std::make_shared<Scene>();
    m_sceneManager->setScene(scene);
    
    // Connect selection manager to scene
    if (m_sceneManager && m_selectionManager) {
        // Link the selection manager to the scene manager
        // This enables selection manager to listen for scene changes
        m_selectionManager->setSceneManager(m_sceneManager.get());
        
        qDebug() << "Connected SelectionManager to SceneManager";
    }
    
    qDebug() << "Core systems initialized successfully";
    qDebug() << "CoreSystem::initialize() - Core systems ready";
}

void CoreSystem::shutdown() {
    qDebug() << "CoreSystem::shutdown() - Shutting down core systems";
    
    // Clear selection first to avoid dangling references
    if (m_selectionManager) {
        m_selectionManager->clearSelection();
    }
    
    // Release scene
    if (m_sceneManager) {
        m_sceneManager->setScene(nullptr);
    }
    
    // Destroy managers in reverse order of creation
    m_selectionManager.reset();
    m_sceneManager.reset();
    
    qDebug() << "Core systems shut down successfully";
}
