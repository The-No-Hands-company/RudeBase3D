#include "core/application_controller.hpp"
#include <QDebug>

ApplicationController::ApplicationController(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[ApplicationController] Created";
    
    // Create managers
    m_sceneManager = std::make_unique<SceneManager>(this);
    m_primitiveManager = std::make_unique<PrimitiveManager>(this);
    m_selectionManager = std::make_unique<rude::SelectionManager>();
    m_meshOperationManager = std::make_unique<rude::MeshOperationManager>();
}

ApplicationController::~ApplicationController()
{
    qDebug() << "[ApplicationController] Destroyed";
}

void ApplicationController::initialize()
{
    if (m_initialized) {
        qWarning() << "[ApplicationController] Already initialized";
        return;
    }

    qDebug() << "[ApplicationController] Initializing...";

    // Set up cross-manager dependencies
    m_meshOperationManager->setSelectionManager(m_selectionManager.get());

    // Set up signal connections
    setupManagerConnections();

    m_initialized = true;
    qDebug() << "[ApplicationController] Initialization complete";
}

void ApplicationController::setScene(std::shared_ptr<Scene> scene)
{
    if (!m_initialized) {
        qWarning() << "[ApplicationController] Not initialized";
        return;
    }

    m_sceneManager->setScene(scene);
    
    // Set scene for selection manager
    m_selectionManager->setScene(scene.get());
    
    qDebug() << "[ApplicationController] Scene set on all managers";
}

Entity* ApplicationController::createPrimitive(const QString& primitiveType, const QString& name)
{
    if (!m_initialized) {
        qWarning() << "[ApplicationController] Not initialized";
        return nullptr;
    }

    qDebug() << "[ApplicationController] Creating primitive:" << primitiveType;

    // Delegate to scene manager
    auto entity = m_sceneManager->createPrimitive(primitiveType, name);
    
    if (entity) {
        qDebug() << "[ApplicationController] Successfully created primitive:" << primitiveType;
        // The entityCreated signal will be forwarded via setupManagerConnections
    } else {
        qWarning() << "[ApplicationController] Failed to create primitive:" << primitiveType;
    }

    return entity;
}

bool ApplicationController::executeOperation(const QString& operationName)
{
    if (!m_initialized) {
        qWarning() << "[ApplicationController] Not initialized";
        return false;
    }

    qDebug() << "[ApplicationController] Executing operation:" << operationName;

    // Delegate to mesh operation manager
    bool success = m_meshOperationManager->executeOperation(operationName);
    
    emit operationExecuted(operationName, success);
    
    if (success) {
        emit sceneChanged();
    }

    return success;
}

void ApplicationController::selectEntity(Entity* entity)
{
    if (!m_initialized) {
        qWarning() << "[ApplicationController] Not initialized";
        return;
    }

    // Delegate to selection manager
    m_selectionManager->selectEntity(entity);
}

void ApplicationController::clearSelection()
{
    if (!m_initialized) {
        qWarning() << "[ApplicationController] Not initialized";
        return;
    }

    m_selectionManager->clearSelection();
}

bool ApplicationController::newScene()
{
    if (!m_initialized) {
        qWarning() << "[ApplicationController] Not initialized";
        return false;
    }

    // Clear current scene
    m_sceneManager->clearScene();
    
    // Create new empty scene
    auto newScene = std::make_shared<Scene>();
    setScene(newScene);
    
    emit sceneChanged();
    return true;
}

bool ApplicationController::saveScene(const QString& filePath)
{
    if (!m_initialized) {
        qWarning() << "[ApplicationController] Not initialized";
        return false;
    }

    return m_sceneManager->saveScene(filePath);
}

bool ApplicationController::loadScene(const QString& filePath)
{
    if (!m_initialized) {
        qWarning() << "[ApplicationController] Not initialized";
        return false;
    }

    bool success = m_sceneManager->loadScene(filePath);
    if (success) {
        emit sceneChanged();
    }
    return success;
}

bool ApplicationController::importMesh(const QString& filePath)
{
    if (!m_initialized) {
        qWarning() << "[ApplicationController] Not initialized";
        return false;
    }

    auto entity = m_sceneManager->importMesh(filePath);
    return entity != nullptr;
}

void ApplicationController::onEntityCreated(Entity* entity)
{
    qDebug() << "[ApplicationController] Entity created signal received:" << (entity ? entity->name : "null");
    emit entityCreated(entity);
    emit sceneChanged();
}

void ApplicationController::onEntityDeleted(Entity* entity)
{
    qDebug() << "[ApplicationController] Entity deleted signal received";
    emit entityDeleted(entity);
    emit sceneChanged();
}

void ApplicationController::onSelectionChanged()
{
    qDebug() << "[ApplicationController] Selection changed signal received";
    emit selectionChanged();
}

void ApplicationController::setupManagerConnections()
{
    // Connect SceneManager signals
    connect(m_sceneManager.get(), &SceneManager::entityCreated,
            this, &ApplicationController::onEntityCreated);
    connect(m_sceneManager.get(), &SceneManager::entityDeleted,
            this, &ApplicationController::onEntityDeleted);

    // Connect SelectionManager signals
    connect(m_selectionManager.get(), &rude::SelectionManager::selectionChanged,
            this, &ApplicationController::onSelectionChanged);

    // Connect PrimitiveManager signals
    connect(m_primitiveManager.get(), &PrimitiveManager::primitiveCreated,
            [this](const QString& type, std::shared_ptr<Mesh> mesh) {
                qDebug() << "[ApplicationController] Primitive created:" << type;
            });
    
    connect(m_primitiveManager.get(), &PrimitiveManager::primitiveCreationFailed,
            [this](const QString& type, const QString& error) {
                qWarning() << "[ApplicationController] Primitive creation failed:" << type << error;
            });

    qDebug() << "[ApplicationController] Manager connections established";
}
