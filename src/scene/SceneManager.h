#pragma once

#include "Common.h"
#include <QObject>
#include <memory>

class Scene;
class SceneObject;
class Mesh;
class Material;

class SceneManager : public QObject
{
    Q_OBJECT
    
public:
    explicit SceneManager(QObject* parent = nullptr);
    
    // Scene management
    void setScene(std::shared_ptr<Scene> scene);
    std::shared_ptr<Scene> getScene() const { return m_scene; }
    
    // Scene operations
    void newScene();
    void clearScene();
    bool loadScene(const QString& filePath);
    bool saveScene(const QString& filePath);
    
    // Object creation
    SceneObjectPtr createCube();
    SceneObjectPtr createSphere();
    SceneObjectPtr createPlane();
    SceneObjectPtr createCylinder();
    SceneObjectPtr createCone();
    SceneObjectPtr createTorus();
    SceneObjectPtr createIcosphere();
    
    // Generic object creation helper
    SceneObjectPtr createPrimitiveObject(const QString& name, std::shared_ptr<Mesh> mesh);
    
    // Object management
    void addObject(SceneObjectPtr object);
    void removeObject(SceneObjectPtr object);
    void duplicateObject(SceneObjectPtr object);
    
    // Selection management
    void selectObject(SceneObjectPtr object);
    void clearSelection();
    SceneObjectPtr getSelectedObject() const;
    
    // Scene queries
    size_t getObjectCount() const;
    bool isEmpty() const;
    
signals:
    void sceneChanged();
    void objectAdded(SceneObjectPtr object);
    void objectRemoved(SceneObjectPtr object);
    void selectionChanged(SceneObjectPtr object);
    
private:
    void setupDefaultScene();
    
    std::shared_ptr<Scene> m_scene;
    unsigned int m_nextObjectId;
};
