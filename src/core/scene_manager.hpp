#pragma once
#include "scene.hpp"
#include <QVector3D>

class SceneManager {
public:
    SceneManager();
    ~SceneManager() = default;
    
    void setScene(std::shared_ptr<Scene> scene) { m_scene = scene; }
    std::shared_ptr<Scene> getScene() const { return m_scene; }
    
    Entity* pickObject(const QVector3D& rayOrigin, const QVector3D& rayDirection);
    void setSelectedObject(Entity* entity);
    Entity* getSelectedObject() const { return m_selectedEntity; }
    
private:
    std::shared_ptr<Scene> m_scene;
    Entity* m_selectedEntity = nullptr;
};
