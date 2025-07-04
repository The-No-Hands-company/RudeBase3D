#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform {
    glm::vec3 position{0, 0, 0};
    glm::quat rotation{1, 0, 0, 0};
    glm::vec3 scale{1, 1, 1};

    glm::mat4 toMatrix() const;
    glm::mat4 getModelMatrix() const { return toMatrix(); } // Alias for toMatrix()

    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::quat getRotation() const { return rotation; }
    glm::vec3 getScale() const { return scale; }

    // Setters
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setRotation(const glm::quat& rot) { rotation = rot; }
    void setScale(const glm::vec3& scl) { scale = scl; }
}; 