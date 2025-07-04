#pragma once

#include <QObject>
#include <memory>
#include "core/scene_manager.hpp"
#include "core/primitive_manager.hpp"
#include "core/selection_manager.hpp"
#include "core/mesh_operation_manager.hpp"

/**
 * @brief Central controller that coordinates all major system components
 * 
 * This class acts as a facade/controller that:
 * - Coordinates between different managers
 * - Provides high-level operations for the UI layer
 * - Handles cross-cutting concerns
 * - Maintains consistent state across the application
 * 
 * The UI layer should primarily interact with this controller
 * rather than directly with individual managers.
 */
class ApplicationController : public QObject
{
    Q_OBJECT

public:
    explicit ApplicationController(QObject* parent = nullptr);
    ~ApplicationController();

    // Initialization
    void initialize();
    void setScene(std::shared_ptr<Scene> scene);

    // Manager access (for specific use cases)
    SceneManager* getSceneManager() const { return m_sceneManager.get(); }
    PrimitiveManager* getPrimitiveManager() const { return m_primitiveManager.get(); }
    rude::SelectionManager* getSelectionManager() const { return m_selectionManager.get(); }
    rude::MeshOperationManager* getMeshOperationManager() const { return m_meshOperationManager.get(); }

    // High-level operations that coordinate multiple managers
    Entity* createPrimitive(const QString& primitiveType, const QString& name = QString());
    bool executeOperation(const QString& operationName);
    void selectEntity(Entity* entity);
    void clearSelection();

    // File operations
    bool newScene();
    bool saveScene(const QString& filePath);
    bool loadScene(const QString& filePath);
    bool importMesh(const QString& filePath);

    // Access to current scene
    std::shared_ptr<Scene> getScene() const { return m_scene; }

signals:
    // Forwarded signals from managers for UI to connect to
    void entityCreated(Entity* entity);
    void entityDeleted(Entity* entity);
    void selectionChanged();
    void operationExecuted(const QString& operation, bool success);
    void sceneChanged();

private slots:
    // Internal coordination between managers
    void onEntityCreated(Entity* entity);
    void onEntityDeleted(Entity* entity);
    void onSelectionChanged();

private:
    void setupManagerConnections();

    // Core managers
    std::unique_ptr<SceneManager> m_sceneManager;
    std::unique_ptr<PrimitiveManager> m_primitiveManager;
    std::unique_ptr<rude::SelectionManager> m_selectionManager;
    std::unique_ptr<rude::MeshOperationManager> m_meshOperationManager;
    
    // Current scene
    std::shared_ptr<Scene> m_scene;
    
    // State
    bool m_initialized = false;
};
