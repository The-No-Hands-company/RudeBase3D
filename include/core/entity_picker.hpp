#pragma once
#include <glm/glm.hpp>

namespace rude {
    class Scene;
}

class Entity;
class Camera;

class EntityPicker {
public:
    EntityPicker();
    void setScene(rude::Scene* scene);
    void setCamera(Camera* camera);
    void pickEntity(float mouseX, float mouseY, int viewportWidth, int viewportHeight);
    Entity* getSelectedEntity() const;
    void setSelectedEntity(Entity* entity) { selectedEntity = entity; }
private:
    rude::Scene* scene = nullptr;
    Camera* camera = nullptr;
    Entity* selectedEntity = nullptr;
    bool rayAABBIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& tmin, float& tmax);
}; 