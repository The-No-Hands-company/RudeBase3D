//==============================================================================
// RudeBase3D Engine - core_system.hpp
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
 * @file core_system.hpp
 * @brief Core system singleton for managing application-wide systems
 * 
 * This file defines the CoreSystem singleton class that serves as the central
 * coordination point for all major system managers in RudeBase3D. It provides
 * a clean interface for accessing scene management, selection, ECS, and other
 * core functionality without complex dependency injection.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * @copyright Copyright (c) 2025 The-No-Hands-company
 * 
 * @ingroup Core
 * 
 * ## Architecture Pattern
 * The CoreSystem uses the Singleton pattern to ensure a single point of access
 * to system managers. This simplifies architecture while maintaining proper
 * initialization order and lifecycle management.
 * 
 * ## Thread Safety
 * The singleton implementation is thread-safe for creation, but individual
 * manager access is not synchronized. Most managers are designed for single-
 * threaded access from the main UI thread.
 * 
 * ## Usage Example
 * ```cpp
 * // Get access to managers
 * CoreSystem& core = getCoreSystem();
 * auto* sceneManager = core.getSceneManager();
 * auto* selectionManager = core.getSelectionManager();
 * 
 * // Use managers for operations
 * sceneManager->createScene("MyScene");
 * selectionManager->selectObject(objectId);
 * ```
 */

/**************************************************************************/
/*  core_system.hpp                                                       */
/**************************************************************************/
/*                         This file is part of:                         */
/*                             RUDEBASE3D                                 */
/*                     https://github.com/The-No-Hands-company/RudeBase3D */
/**************************************************************************/
/* Copyright (c) 2025-present The-No-Hands-company.                      */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "core/scene_manager.hpp"
#include "core/selection_manager.hpp"
#include "core/edit_preview_system.hpp"
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
    rude::SceneManager* getSceneManager() const { return m_sceneManager.get(); }
    rude::SelectionManager* getSelectionManager() const { return m_selectionManager.get(); }
    rude::ecs::ECSManager* getECSManager() const { return m_ecsManager.get(); }
    RudeBase3D::Core::EditPreviewSystem* getEditPreviewSystem() const { return m_editPreviewSystem.get(); }

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
    std::unique_ptr<rude::SceneManager> m_sceneManager;
    std::unique_ptr<rude::SelectionManager> m_selectionManager;
    std::unique_ptr<rude::ecs::ECSManager> m_ecsManager;
    std::unique_ptr<RudeBase3D::Core::EditPreviewSystem> m_editPreviewSystem;
    
    // Other managers will be added as needed:
    // - PrimitiveManager
    // - MeshOperationManager
    // - etc.
};

// Convenience function for accessing the CoreSystem singleton
inline CoreSystem& getCoreSystem() {
    return CoreSystem::getInstance();
}
