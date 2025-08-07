//==============================================================================
// RudeBase3D Engine - scene_manager.hpp
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

#pragma once

#include <memory>
#include <vector>
#include <QString>
#include <QObject>
#include "entity.hpp"
#include "core/scene.hpp"

// Forward declaration
class PrimitiveManager;

namespace rude {

/**
 * @brief Central manager for scene operations and entity lifecycle
 * 
 * This class handles all scene-related operations including:
 * - Entity creation and deletion
 * - Scene state management
 * - Entity hierarchy management
 * - Scene serialization/deserialization
 * 
 * Responsibilities:
 * - Manage the scene graph
 * - Handle entity lifecycle (create, destroy, parent/child relationships)
 * - Coordinate with other managers (selection, mesh operations, etc.)
 * - Provide clean API for UI layer to interact with scene
 */
class SceneManager : public QObject
{
    Q_OBJECT

public:
    explicit SceneManager(QObject* parent = nullptr);
    ~SceneManager();

    // Scene management
    void setScene(std::shared_ptr<rude::Scene> scene);
    std::shared_ptr<rude::Scene> getScene() const { return m_scene; }
    
    // Manager coordination
    void setPrimitiveManager(PrimitiveManager* primitiveManager);

    // Entity creation (delegates to appropriate factories)
    Entity* createPrimitive(const std::string& primitiveType, const std::string& name = "");
    Entity* createEntity(const std::string& name = "");
    Entity* importMesh(const std::string& filePath, const std::string& name = "");
    
    // Entity management
    void deleteEntity(Entity* entity);
    void deleteSelectedEntities();
    void duplicateEntity(Entity* entity);
    
    // Scene operations
    void clearScene();
    bool saveScene(const std::string& filePath);
    bool loadScene(const std::string& filePath);
    
    // Query operations
    std::vector<Entity*> getAllEntities() const;
    Entity* findEntityByName(const std::string& name) const;
    Entity* findEntityById(uint32_t id) const;
    
    // Selection operations
    Entity* pickObject(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const;
    void setSelectedObject(Entity* entity);
    Entity* getSelectedObject() const { return m_selectedEntity; }
    bool isEmpty() const;
    glm::vec3 getSceneBoundingBoxCenter() const;

signals:
    void entityCreated(Entity* entity);
    void entityDeleted(Entity* entity);
    void sceneCleared();
    void sceneLoaded(const std::string& filePath);
    void sceneSaved(const std::string& filePath);
    void selectionChanged();

private:
    // Data
    std::shared_ptr<rude::Scene> m_scene;
    std::unique_ptr<PrimitiveManager> m_primitiveManager;
    PrimitiveManager* m_externalPrimitiveManager = nullptr;
    Entity* m_selectedEntity = nullptr;
    
    // Helper methods
    std::string generateEntityName(const std::string& baseName) const;
    void connectEntitySignals(Entity* entity);
};

} // namespace rude
