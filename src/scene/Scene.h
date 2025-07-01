#pragma once

#include "Common.h"
#include <vector>
#include <QObject>

class Scene : public QObject {
    Q_OBJECT

public:
    Scene(QObject* parent = nullptr);
    ~Scene() = default;

    // Object management
    void addObject(SceneObjectPtr object);
    void removeObject(SceneObjectPtr object);
    void removeObject(unsigned int objectId);
    SceneObjectPtr getObject(unsigned int objectId) const;
    SceneObjectPtr getObjectByName(const QString& name) const;
    
    const std::vector<SceneObjectPtr>& getObjects() const { return m_objects; }
    void clear();
    
    // Selection
    void setSelectedObject(SceneObjectPtr object);
    void clearSelection();
    SceneObjectPtr getSelectedObject() const { return m_selectedObject; }
    
    // Scene queries
    size_t getObjectCount() const { return m_objects.size(); }
    bool isEmpty() const { return m_objects.empty(); }
    
    // Ray casting for object picking
    SceneObjectPtr pickObject(const QVector3D& rayOrigin, const QVector3D& rayDirection) const;
    
    // Bounding box
    QVector3D getSceneBoundingBoxMin() const;
    QVector3D getSceneBoundingBoxMax() const;
    QVector3D getSceneBoundingBoxCenter() const;
    
    // Rendering
    void render(const QMatrix4x4& viewMatrix, const QMatrix4x4& projectionMatrix, RenderMode mode);

signals:
    void objectAdded(SceneObjectPtr object);
    void objectRemoved(SceneObjectPtr object);
    void selectionChanged(SceneObjectPtr object);
    void sceneChanged();

private:
    std::vector<SceneObjectPtr> m_objects;
    SceneObjectPtr m_selectedObject;
    
    // Helper methods
    bool rayIntersectsAABB(const QVector3D& rayOrigin, const QVector3D& rayDirection,
                          const QVector3D& aabbMin, const QVector3D& aabbMax) const;
};
