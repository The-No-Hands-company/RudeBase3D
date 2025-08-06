

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
    void setScene(std::shared_ptr<rude::Scene> scene);

    // Manager access (for specific use cases)
    rude::SceneManager* getSceneManager() const { return m_sceneManager.get(); }
    PrimitiveManager* getPrimitiveManager() const { return m_primitiveManager.get(); }
    rude::SelectionManager* getSelectionManager() const { return m_selectionManager.get(); }
    rude::MeshOperationManager* getMeshOperationManager() const { return m_meshOperationManager.get(); }

    // High-level operations that coordinate multiple managers
    Entity* createPrimitive(const std::string& primitiveType, const std::string& name = "");
    bool executeOperation(const std::string& operationName);
    void selectEntity(Entity* entity);
    void clearSelection();

    // File operations
    bool newScene();
    bool saveScene(const std::string& filePath);
    bool loadScene(const std::string& filePath);
    bool importMesh(const std::string& filePath);

    // Access to current scene
    std::shared_ptr<rude::Scene> getScene() const { return m_scene; }


signals:
    void sceneChanged();
    void entityCreated(Entity* entity);
    void entityDeleted(Entity* entity);
    void selectionChanged();

public slots:
    void onEntityCreated(Entity* entity);
    void onEntityDeleted(Entity* entity);
    void onSelectionChanged();

public:
    void setupManagerConnections();

private:
    // Core managers
    std::unique_ptr<rude::SceneManager> m_sceneManager;
    std::unique_ptr<PrimitiveManager> m_primitiveManager;
    std::unique_ptr<rude::SelectionManager> m_selectionManager;
    std::unique_ptr<rude::MeshOperationManager> m_meshOperationManager;
    // Current scene
    std::shared_ptr<rude::Scene> m_scene;
    // State
    bool m_initialized = false;
};
