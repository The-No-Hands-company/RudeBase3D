#include "Scene.h"
#include "SceneObject.h"
#include <algorithm>

Scene::Scene(QObject* parent)
    : QObject(parent)
{
}

void Scene::addObject(SceneObjectPtr object)
{
    if (!object) {
        return;
    }
    
    auto it = std::find(m_objects.begin(), m_objects.end(), object);
    if (it == m_objects.end()) {
        m_objects.push_back(object);
        emit objectAdded(object);
        emit sceneChanged();
    }
}

void Scene::removeObject(SceneObjectPtr object)
{
    if (!object) {
        return;
    }
    
    auto it = std::find(m_objects.begin(), m_objects.end(), object);
    if (it != m_objects.end()) {
        if (m_selectedObject == object) {
            clearSelection();
        }
        
        m_objects.erase(it);
        emit objectRemoved(object);
        emit sceneChanged();
    }
}

void Scene::removeObject(unsigned int objectId)
{
    auto object = getObject(objectId);
    if (object) {
        removeObject(object);
    }
}

SceneObjectPtr Scene::getObject(unsigned int objectId) const
{
    auto it = std::find_if(m_objects.begin(), m_objects.end(),
                          [objectId](const SceneObjectPtr& obj) {
                              return obj->getId() == objectId;
                          });
    
    return (it != m_objects.end()) ? *it : nullptr;
}

SceneObjectPtr Scene::getObjectByName(const QString& name) const
{
    auto it = std::find_if(m_objects.begin(), m_objects.end(),
                          [&name](const SceneObjectPtr& obj) {
                              return obj->getName() == name;
                          });
    
    return (it != m_objects.end()) ? *it : nullptr;
}

void Scene::clear()
{
    clearSelection();
    m_objects.clear();
    emit sceneChanged();
}

void Scene::setSelectedObject(SceneObjectPtr object)
{
    if (m_selectedObject == object) {
        return;
    }
    
    // Deselect previous object
    if (m_selectedObject) {
        m_selectedObject->setSelected(false);
    }
    
    m_selectedObject = object;
    
    // Select new object
    if (m_selectedObject) {
        m_selectedObject->setSelected(true);
    }
    
    emit selectionChanged(m_selectedObject);
}

void Scene::clearSelection()
{
    setSelectedObject(nullptr);
}

SceneObjectPtr Scene::pickObject(const QVector3D& rayOrigin, const QVector3D& rayDirection) const
{
    float closestDistance = std::numeric_limits<float>::max();
    SceneObjectPtr closestObject = nullptr;
    
    for (const auto& object : m_objects) {
        if (!object->isVisible()) {
            continue;
        }
        
        QVector3D aabbMin = object->getBoundingBoxMin();
        QVector3D aabbMax = object->getBoundingBoxMax();
        
        if (rayIntersectsAABB(rayOrigin, rayDirection, aabbMin, aabbMax)) {
            // Calculate distance to object center
            QVector3D objectCenter = object->getBoundingBoxCenter();
            float distance = (objectCenter - rayOrigin).length();
            
            if (distance < closestDistance) {
                closestDistance = distance;
                closestObject = object;
            }
        }
    }
    
    return closestObject;
}

QVector3D Scene::getSceneBoundingBoxMin() const
{
    if (m_objects.empty()) {
        return QVector3D(-1, -1, -1);
    }
    
    try {
        QVector3D sceneMin = m_objects[0]->getBoundingBoxMin();
        for (const auto& object : m_objects) {
            if (!object) continue; // Skip null objects
            QVector3D objectMin = object->getBoundingBoxMin();
            sceneMin.setX(std::min(sceneMin.x(), objectMin.x()));
            sceneMin.setY(std::min(sceneMin.y(), objectMin.y()));
            sceneMin.setZ(std::min(sceneMin.z(), objectMin.z()));
        }
        return sceneMin;
    } catch (...) {
        return QVector3D(-1, -1, -1);
    }
}

QVector3D Scene::getSceneBoundingBoxMax() const
{
    if (m_objects.empty()) {
        return QVector3D(1, 1, 1);
    }
    
    try {
        QVector3D sceneMax = m_objects[0]->getBoundingBoxMax();
        for (const auto& object : m_objects) {
            if (!object) continue; // Skip null objects
            QVector3D objectMax = object->getBoundingBoxMax();
            sceneMax.setX(std::max(sceneMax.x(), objectMax.x()));
            sceneMax.setY(std::max(sceneMax.y(), objectMax.y()));
            sceneMax.setZ(std::max(sceneMax.z(), objectMax.z()));
        }
        return sceneMax;
    } catch (...) {
        return QVector3D(1, 1, 1);
    }
}

QVector3D Scene::getSceneBoundingBoxCenter() const
{
    return (getSceneBoundingBoxMin() + getSceneBoundingBoxMax()) * 0.5f;
}

void Scene::render(const QMatrix4x4& viewMatrix, const QMatrix4x4& projectionMatrix, RenderMode mode)
{
    for (const auto& object : m_objects) {
        if (object->isVisible()) {
            object->render(viewMatrix, projectionMatrix, mode);
        }
    }
}

bool Scene::rayIntersectsAABB(const QVector3D& rayOrigin, const QVector3D& rayDirection,
                             const QVector3D& aabbMin, const QVector3D& aabbMax) const
{
    // Ray-AABB intersection test using the slab method
    QVector3D invDir = QVector3D(
        1.0f / rayDirection.x(),
        1.0f / rayDirection.y(),
        1.0f / rayDirection.z()
    );
    
    QVector3D t1 = QVector3D(
        (aabbMin.x() - rayOrigin.x()) * invDir.x(),
        (aabbMin.y() - rayOrigin.y()) * invDir.y(),
        (aabbMin.z() - rayOrigin.z()) * invDir.z()
    );
    
    QVector3D t2 = QVector3D(
        (aabbMax.x() - rayOrigin.x()) * invDir.x(),
        (aabbMax.y() - rayOrigin.y()) * invDir.y(),
        (aabbMax.z() - rayOrigin.z()) * invDir.z()
    );
    
    QVector3D tmin = QVector3D(
        std::min(t1.x(), t2.x()),
        std::min(t1.y(), t2.y()),
        std::min(t1.z(), t2.z())
    );
    
    QVector3D tmax = QVector3D(
        std::max(t1.x(), t2.x()),
        std::max(t1.y(), t2.y()),
        std::max(t1.z(), t2.z())
    );
    
    float tnear = std::max({tmin.x(), tmin.y(), tmin.z()});
    float tfar = std::min({tmax.x(), tmax.y(), tmax.z()});
    
    return tnear <= tfar && tfar >= 0.0f;
}
