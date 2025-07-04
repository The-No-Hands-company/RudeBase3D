#pragma once

#include <memory>
#include <vector>
#include <QString>
#include <QObject>
#include "entity.hpp"
#include "core/scene.hpp"

// Forward declaration
class PrimitiveManager;

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
    void setScene(std::shared_ptr<Scene> scene);
    std::shared_ptr<Scene> getScene() const { return m_scene; }
    
    // Manager coordination
    void setPrimitiveManager(PrimitiveManager* primitiveManager);

    // Entity creation (delegates to appropriate factories)
    Entity* createPrimitive(const QString& primitiveType, const QString& name = QString());
    Entity* createEntity(const QString& name = QString());
    Entity* importMesh(const QString& filePath, const QString& name = QString());
    
    // Entity management
    void deleteEntity(Entity* entity);
    void deleteSelectedEntities();
    void duplicateEntity(Entity* entity);
    
    // Scene operations
    void clearScene();
    bool saveScene(const QString& filePath);
    bool loadScene(const QString& filePath);
    
    // Query operations
    std::vector<Entity*> getAllEntities() const;
    Entity* findEntityByName(const QString& name) const;
    Entity* findEntityById(uint32_t id) const;
    
    // Selection operations
    Entity* pickObject(const QVector3D& rayOrigin, const QVector3D& rayDirection) const;
    void setSelectedObject(Entity* entity);
    Entity* getSelectedObject() const { return m_selectedEntity; }
    bool isEmpty() const;
    QVector3D getSceneBoundingBoxCenter() const;

signals:
    void entityCreated(Entity* entity);
    void entityDeleted(Entity* entity);
    void sceneCleared();
    void sceneLoaded(const QString& filePath);
    void sceneSaved(const QString& filePath);
    void selectionChanged();

private:
    // Data
    std::shared_ptr<Scene> m_scene;
    std::unique_ptr<PrimitiveManager> m_primitiveManager;
    PrimitiveManager* m_externalPrimitiveManager = nullptr;
    Entity* m_selectedEntity = nullptr;
    
    // Helper methods
    QString generateEntityName(const QString& baseName) const;
    void connectEntitySignals(Entity* entity);
};
