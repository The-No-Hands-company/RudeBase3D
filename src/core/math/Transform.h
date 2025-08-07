#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class LegacyTransform {
public:
    LegacyTransform();
    ~LegacyTransform() = default;

    // Position
    void setPosition(const glm::vec3& position);
    const glm::vec3& getPosition() const { return m_position; }
    
    // Rotation
    void setRotation(const glm::quat& rotation);
    const glm::quat& getRotation() const { return m_rotation; }
    void setEulerAngles(const glm::vec3& euler);
    glm::vec3 getEulerAngles() const;
    
    // Scale
    void setScale(const glm::vec3& scale);
    const glm::vec3& getScale() const { return m_scale; }
    void setUniformScale(float scale);
    
    // Transformation methods
    void translate(const glm::vec3& translation);
    void rotate(const glm::quat& rotation);
    void rotateAround(const glm::vec3& axis, float angle);
    void scale(const glm::vec3& scale);
    
    // Look at target
    void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 1, 0));
    
    // Matrix generation
    glm::mat4 getModelMatrix() const;
    glm::mat4 getInverseModelMatrix() const;
    
    // Forward, right, up vectors
    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;
    
    // Reset to identity
    void reset();

private:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    mutable bool m_matrixDirty;
    mutable glm::mat4 m_modelMatrix;
    void updateMatrix() const;
};
