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
