#include "core/scene_manager.hpp"
#include "core/primitive_manager.hpp"
#include "geometry/core/Material.h"
#include <glm/gtc/quaternion.hpp>
#include <QDebug>

SceneManager::SceneManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[SceneManager] Created";
    // Initialize primitive manager
    m_primitiveManager = std::make_unique<PrimitiveManager>(this);
}

SceneManager::~SceneManager()
{
    qDebug() << "[SceneManager] Destroyed";
}

void SceneManager::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
    qDebug() << "[SceneManager] Scene set";
}

void SceneManager::setPrimitiveManager(PrimitiveManager* primitiveManager)
{
    if (primitiveManager) {
        // Release the internal primitive manager and use the external one
        m_primitiveManager.reset();
        m_externalPrimitiveManager = primitiveManager;
        qDebug() << "[SceneManager] External primitive manager set and will be used";
    } else {
        qDebug() << "[SceneManager] Warning: null primitive manager provided";
    }
}

Entity* SceneManager::createPrimitive(const QString& primitiveType, const QString& name)
{
    qDebug() << "[SceneManager] ==> START createPrimitive:" << primitiveType << "name:" << name;
    
    if (!m_scene) {
        qWarning() << "[SceneManager] Cannot create primitive: No scene set";
        return nullptr;
    }

    qDebug() << "[SceneManager] Creating primitive:" << primitiveType;

    try {
        // Determine the primitive type enum
        rude::PrimitiveType type = rude::PrimitiveType::Cube; // Default
        if (primitiveType.toLower() == "cube") {
            type = rude::PrimitiveType::Cube;
            qDebug() << "[SceneManager] Primitive type set to CUBE";
        } else if (primitiveType.toLower() == "plane") {
            type = rude::PrimitiveType::Plane;
            qDebug() << "[SceneManager] Primitive type set to PLANE";
        } else if (primitiveType.toLower() == "sphere") {
            type = rude::PrimitiveType::Sphere;
            qDebug() << "[SceneManager] Primitive type set to SPHERE";
        } else {
            qWarning() << "[SceneManager] Unknown primitive type:" << primitiveType;
            return nullptr;
        }

        // Generate name if not provided
        QString entityName = name.isEmpty() ? generateEntityName(primitiveType) : name;
        qDebug() << "[SceneManager] Generated entity name:" << entityName;

        // Create the entity using the correct Scene interface
        qDebug() << "[SceneManager] About to call m_scene->createEntity...";
        Entity* entity = m_scene->createEntity(type, entityName.toStdString());
        if (!entity) {
            qWarning() << "[SceneManager] Failed to create entity";
            return nullptr;
        }
        qDebug() << "[SceneManager] Entity created successfully with ID:" << entity->getId();

        // Create the mesh using PrimitiveManager
        PrimitiveManager* manager = m_externalPrimitiveManager ? m_externalPrimitiveManager : m_primitiveManager.get();
        if (!manager) {
            qWarning() << "[SceneManager] No primitive manager available";
            return nullptr;
        }
        qDebug() << "[SceneManager] Using primitive manager:" << (m_externalPrimitiveManager ? "external" : "internal");
        
        std::shared_ptr<Mesh> mesh;
        if (primitiveType.toLower() == "cube") {
            qDebug() << "[SceneManager] Creating cube mesh...";
            mesh = manager->createCube();
        } else if (primitiveType.toLower() == "plane") {
            qDebug() << "[SceneManager] Creating plane mesh...";
            mesh = manager->createPlane();
        } else if (primitiveType.toLower() == "sphere") {
            qDebug() << "[SceneManager] Creating sphere mesh...";
            mesh = manager->createSphere();
        }

        if (!mesh) {
            qWarning() << "[SceneManager] Failed to create mesh for primitive:" << primitiveType;
            // Clean up the entity
            // TODO: Implement entity deletion in scene
            return nullptr;
        }

        // Set up the entity
        entity->setMesh(mesh);
        entity->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        entity->setScale(glm::vec3(1.0f));
        entity->setRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

        // TODO: Set default material on entity instead of mesh
        // Material defaultMaterial;
        // defaultMaterial.diffuse = glm::vec3(0.7f, 0.7f, 0.8f);
        // defaultMaterial.specular = glm::vec3(1.0f);
        // defaultMaterial.shininess = 32.0f;
        // entity->setMaterial(defaultMaterial);

        connectEntitySignals(entity);

        emit entityCreated(entity);
        qDebug() << "[SceneManager] Successfully created primitive:" << primitiveType << "named:" << entityName;

        return entity;

    } catch (const std::exception& e) {
        qWarning() << "[SceneManager] Exception creating primitive:" << e.what();
        return nullptr;
    }
}

Entity* SceneManager::createEntity(const QString& name)
{
    if (!m_scene) {
        qWarning() << "[SceneManager] Cannot create entity: No scene set";
        return nullptr;
    }

    QString entityName = name.isEmpty() ? generateEntityName("Entity") : name;
    auto entity = m_scene->createEntity(rude::PrimitiveType::None, entityName.toStdString());
    
    if (entity) {
        connectEntitySignals(entity);
        emit entityCreated(entity);
    }

    return entity;
}

Entity* SceneManager::importMesh(const QString& filePath, const QString& name)
{
    // TODO: Implement mesh import functionality
    qDebug() << "[SceneManager] Mesh import not yet implemented:" << filePath;
    return nullptr;
}

void SceneManager::deleteEntity(Entity* entity)
{
    if (!entity || !m_scene) {
        return;
    }

    qDebug() << "[SceneManager] Deleting entity:" << entity->getName();
    
    // TODO: Implement entity deletion in scene
    // m_scene->deleteEntity(entity);
    
    emit entityDeleted(entity);
}

void SceneManager::deleteSelectedEntities()
{
    // TODO: Coordinate with SelectionManager to delete selected entities
    qDebug() << "[SceneManager] Delete selected entities not yet implemented";
}

void SceneManager::duplicateEntity(Entity* entity)
{
    // TODO: Implement entity duplication
    qDebug() << "[SceneManager] Entity duplication not yet implemented";
}

void SceneManager::clearScene()
{
    if (m_scene) {
        // TODO: Implement scene clearing
        emit sceneCleared();
    }
}

bool SceneManager::saveScene(const QString& filePath)
{
    // TODO: Implement scene serialization
    qDebug() << "[SceneManager] Scene saving not yet implemented:" << filePath;
    return false;
}

bool SceneManager::loadScene(const QString& filePath)
{
    // TODO: Implement scene deserialization
    qDebug() << "[SceneManager] Scene loading not yet implemented:" << filePath;
    return false;
}

std::vector<Entity*> SceneManager::getAllEntities() const
{
    if (!m_scene) {
        return {};
    }

    // Convert from unique_ptr vector to raw pointer vector
    const auto& entityPtrs = m_scene->getEntities();
    std::vector<Entity*> result;
    result.reserve(entityPtrs.size());
    
    for (const auto& entityPtr : entityPtrs) {
        result.push_back(entityPtr.get());
    }
    
    return result;
}

Entity* SceneManager::findEntityByName(const QString& name) const
{
    if (!m_scene) {
        return nullptr;
    }

    const auto& entities = m_scene->getEntities();
    for (const auto& entityPtr : entities) {
        if (entityPtr && entityPtr->getName() == name.toStdString()) {
            return entityPtr.get();
        }
    }
    return nullptr;
}

Entity* SceneManager::findEntityById(uint32_t id) const
{
    if (!m_scene) {
        return nullptr;
    }

    const auto& entities = m_scene->getEntities();
    for (const auto& entityPtr : entities) {
        if (entityPtr && entityPtr->getId() == static_cast<int>(id)) {
            return entityPtr.get();
        }
    }
    return nullptr;
}

QString SceneManager::generateEntityName(const QString& baseName) const
{
    QString name = baseName;
    int counter = 1;
    
    while (findEntityByName(name)) {
        name = QString("%1_%2").arg(baseName).arg(counter++);
    }
    
    return name;
}

void SceneManager::connectEntitySignals(Entity* entity)
{
    if (!entity) return;
    
    // TODO: Connect entity-specific signals if any
    // For example, entity property changes, etc.
}

// Implement the picking method
Entity* SceneManager::pickObject(const QVector3D& rayOrigin, const QVector3D& rayDirection) const
{
    if (!m_scene) return nullptr;
    
    Entity* closestEntity = nullptr;
    float closestDist = std::numeric_limits<float>::max();
    
    // Get all root entities and perform ray intersection tests
    auto rootEntities = m_scene->getRootEntities();
    for (auto* entity : rootEntities) {
        // Simple ray-entity intersection test
        // This is a stub implementation - real implementation would use proper ray-mesh intersection
        
        // For now, just return the first entity
        if (!closestEntity) {
            closestEntity = entity;
            break;
        }
    }
    
    return closestEntity;
}

void SceneManager::setSelectedObject(Entity* entity)
{
    if (m_selectedEntity == entity) return;
    
    m_selectedEntity = entity;
    
    // Emit signal for selection change
    emit selectionChanged();
}

bool SceneManager::isEmpty() const
{
    return !m_scene || m_scene->getEntities().empty();
}

QVector3D SceneManager::getSceneBoundingBoxCenter() const
{
    if (!m_scene) return QVector3D(0, 0, 0);
    
    // Calculate center of all entities
    // This is a stub implementation - real implementation would calculate proper bounding box
    return QVector3D(0, 0, 0);
}
