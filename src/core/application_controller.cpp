//==============================================================================
// RudeBase3D Engine - application_controller.cpp
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



#include "core/application_controller.hpp"
#include "core/entity.hpp"
#include <spdlog/spdlog.h>


ApplicationController::ApplicationController(QObject* parent)
    : QObject(parent)
{
    spdlog::info("[ApplicationController] Created");
    // Create managers
    m_sceneManager = std::make_unique<rude::SceneManager>();
    m_primitiveManager = std::make_unique<PrimitiveManager>();
    m_selectionManager = std::make_unique<rude::SelectionManager>();
    m_meshOperationManager = std::make_unique<rude::MeshOperationManager>();
}

ApplicationController::~ApplicationController()
{
    spdlog::info("[ApplicationController] Destroyed");
}

void ApplicationController::initialize()
{
    if (m_initialized) {
    spdlog::warn("[ApplicationController] Already initialized");
        return;
    }

    spdlog::info("[ApplicationController] Initializing...");

    // Set up cross-manager dependencies
    m_meshOperationManager->setSelectionManager(m_selectionManager.get());

    // No Qt signal/slot connections

    m_initialized = true;
    spdlog::info("[ApplicationController] Initialization complete");
}

void ApplicationController::setScene(std::shared_ptr<rude::Scene> scene)
{
    if (!m_initialized) {
    spdlog::warn("[ApplicationController] Not initialized");
        return;
    }

    m_sceneManager->setScene(scene);
    
    // Set scene for selection manager
    m_selectionManager->setScene(scene.get());
    
    spdlog::info("[ApplicationController] Scene set on all managers");
}

rude::Entity* ApplicationController::createPrimitive(const std::string& primitiveType, const std::string& name)
{
    if (!m_initialized) {
        spdlog::warn("[ApplicationController] Not initialized");
        return nullptr;
    }

    spdlog::info("[ApplicationController] Creating primitive: {}", primitiveType);

    // Delegate to scene manager
    auto entity = m_sceneManager->createPrimitive(primitiveType, name);
    
    if (entity) {
        spdlog::info("[ApplicationController] Successfully created primitive: {}", primitiveType);
        // The entityCreated signal will be forwarded via setupManagerConnections
    } else {
        spdlog::warn("[ApplicationController] Failed to create primitive: {}", primitiveType);
    }

    return entity;
}

bool ApplicationController::executeOperation(const std::string& operationName)
{
    if (!m_initialized) {
        spdlog::warn("[ApplicationController] Not initialized");
        return false;
    }

    spdlog::info("[ApplicationController] Executing operation: {}", operationName);

    // Delegate to mesh operation manager
    bool success = m_meshOperationManager->executeOperation(operationName);
    
    // Operation executed: log or callback can be added here
    if (success) {
        // Scene changed: log or callback can be added here
    }

    return success;
}

void ApplicationController::selectEntity(rude::Entity* entity)
{
    if (!m_initialized) {
        spdlog::warn("[ApplicationController] Not initialized");
        return;
    }

    // Delegate to selection manager
    m_selectionManager->selectEntity(entity);
}

void ApplicationController::clearSelection()
{
    if (!m_initialized) {
        spdlog::warn("[ApplicationController] Not initialized");
        return;
    }

    m_selectionManager->clearSelection();
}

bool ApplicationController::newScene()
{
    if (!m_initialized) {
        spdlog::warn("[ApplicationController] Not initialized");
        return false;
    }

    // Clear current scene
    m_sceneManager->clearScene();
    
    // Create new empty scene
    auto newScene = std::make_shared<rude::Scene>();
    setScene(newScene);
    
    emit sceneChanged();
    return true;
}

bool ApplicationController::saveScene(const std::string& filePath)
{
    if (!m_initialized) {
        spdlog::warn("[ApplicationController] Not initialized");
        return false;
    }

    return m_sceneManager->saveScene(filePath);
}

bool ApplicationController::loadScene(const std::string& filePath)
{
    if (!m_initialized) {
        spdlog::warn("[ApplicationController] Not initialized");
        return false;
    }

    bool success = m_sceneManager->loadScene(filePath);
    if (success) {
        emit sceneChanged();
    }
    return success;
}

bool ApplicationController::importMesh(const std::string& filePath)
{
    if (!m_initialized) {
        spdlog::warn("[ApplicationController] Not initialized");
        return false;
    }

    auto entity = m_sceneManager->importMesh(filePath);
    return entity != nullptr;
}

void ApplicationController::onEntityCreated(rude::Entity* entity)
{
    spdlog::info("[ApplicationController] Entity created signal received: {}", (entity ? entity->name : "null"));
    emit entityCreated(entity);
    emit sceneChanged();
}

void ApplicationController::onEntityDeleted(rude::Entity* entity)
{
    spdlog::info("[ApplicationController] Entity deleted signal received");
    emit entityDeleted(entity);
    emit sceneChanged();
}

void ApplicationController::onSelectionChanged()
{
    spdlog::info("[ApplicationController] Selection changed signal received");
    emit selectionChanged();
}

void ApplicationController::setupManagerConnections()
{
    // Connect SceneManager signals
    connect(m_sceneManager.get(), &rude::SceneManager::entityCreated,
            this, &ApplicationController::onEntityCreated);
    connect(m_sceneManager.get(), &rude::SceneManager::entityDeleted,
            this, &ApplicationController::onEntityDeleted);

    // Connect SelectionManager signals
    connect(m_selectionManager.get(), &rude::SelectionManager::selectionChanged,
            this, &ApplicationController::onSelectionChanged);

    // Connect PrimitiveManager signals
    connect(m_primitiveManager.get(), &PrimitiveManager::primitiveCreated,
            [this](const QString& type, ::std::shared_ptr<Mesh> mesh) {
                spdlog::info("[ApplicationController] Primitive created: {}", type.toStdString());
                // mesh parameter available for future integration with scene management
                (void)mesh; // Future: add mesh to scene or entity system
            });
    connect(m_primitiveManager.get(), &PrimitiveManager::primitiveCreationFailed,
            [this](const QString& type, const QString& error) {
                spdlog::warn("[ApplicationController] Primitive creation failed: {} {}", type.toStdString(), error.toStdString());
            });

    spdlog::info("[ApplicationController] Manager connections established");
}
