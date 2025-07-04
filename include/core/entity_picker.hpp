#pragma once
#include <glm/glm.hpp>
class Scene;
class Entity;
class Camera;

class EntityPicker {
public:
    EntityPicker();
    void setScene(Scene* scene);
    void setCamera(Camera* camera);
    void pickEntity(float mouseX, float mouseY, int viewportWidth, int viewportHeight);
    Entity* getSelectedEntity() const;
    void setSelectedEntity(Entity* entity) { selectedEntity = entity; }
private:
    Scene* scene = nullptr;
    Camera* camera = nullptr;
    Entity* selectedEntity = nullptr;
    bool rayAABBIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& tmin, float& tmax);
}; 