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
    SceneObjectPtr getObjectByName(const std::string& name) const;
    
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

    SceneObjectPtr pickObject(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const;

    // Bounding box
    glm::vec3 getSceneBoundingBoxMin() const;
    glm::vec3 getSceneBoundingBoxMax() const;
    glm::vec3 getSceneBoundingBoxCenter() const;

    // Rendering
    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, RenderMode mode);

signals:
    void objectAdded(SceneObjectPtr object);
    void objectRemoved(SceneObjectPtr object);
    void selectionChanged(SceneObjectPtr object);
    void sceneChanged();

private:
    std::vector<SceneObjectPtr> m_objects;
    SceneObjectPtr m_selectedObject;
    
    // Helper methods
    bool rayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                          const glm::vec3& aabbMin, const glm::vec3& aabbMax) const;
};
