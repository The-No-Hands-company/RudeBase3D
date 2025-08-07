#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/constants.hpp>

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
    
    // Transformation methods
    void translate(const glm::vec3& translation) {
        position += translation;
    }
    
    // Euler angle methods
    void setEulerAngles(const glm::vec3& euler) {
        // euler is in degrees, convert to radians
        glm::vec3 radians = glm::radians(euler);
        rotation = glm::quat(radians);
    }
    
    glm::vec3 getEulerAngles() const {
        glm::vec3 euler = glm::eulerAngles(rotation);
        return glm::degrees(euler);
    }
    
    // Direction vectors (needed for camera controllers)
    glm::vec3 getForward() const {
        return glm::normalize(rotation * glm::vec3(0, 0, -1));
    }
    
    glm::vec3 getRight() const {
        return glm::normalize(rotation * glm::vec3(1, 0, 0));
    }
    
    glm::vec3 getUp() const {
        return glm::normalize(rotation * glm::vec3(0, 1, 0));
    }
}; 