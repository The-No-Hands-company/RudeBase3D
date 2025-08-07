#pragma once

#include "Common.h"
#include "core/transform.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

class Camera {
public:
    Camera();
    ~Camera() = default;

    // Transform
    Transform& getTransform() { return m_transform; }
    const Transform& getTransform() const { return m_transform; }
    
    // Projection settings
    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
    void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    
    float getFOV() const { return m_fov; }
    float getAspectRatio() const { return m_aspectRatio; }
    float getNearPlane() const { return m_nearPlane; }
    float getFarPlane() const { return m_farPlane; }
    
    void setFOV(float fov) { m_fov = fov; updateProjectionMatrix(); }
    void setAspectRatio(float ratio) { m_aspectRatio = ratio; updateProjectionMatrix(); }
    void setNearPlane(float near) { m_nearPlane = near; updateProjectionMatrix(); }
    void setFarPlane(float far) { m_farPlane = far; updateProjectionMatrix(); }
    
    // Camera control
    void lookAt(const glm::vec3& target, const glm::vec3& up = WORLD_UP);
    void orbit(const glm::vec3& center, float deltaYaw, float deltaPitch);
    void pan(const glm::vec3& delta);
    void zoom(float delta);
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    
    // Matrix getters (Qt versions)
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewProjectionMatrix() const;
    
    // GLM matrix getters for gizmo system
    glm::mat4 getViewMatrixGLM() const;
    glm::mat4 getProjectionMatrixGLM() const;
    
    // Ray casting for picking
    glm::vec3 screenToWorldRay(const glm::vec2& screenPos, const glm::vec2& viewportSize) const;
    glm::vec3 getWorldPosition() const { return m_transform.getPosition(); }
    glm::vec3 getForward() const { return m_transform.getForward(); }
    glm::vec3 getRight() const { return m_transform.getRight(); }
    glm::vec3 getUp() const { return m_transform.getUp(); }
    
    // Viewport dimensions
    float getViewportWidth() const { return m_viewportWidth; }
    float getViewportHeight() const { return m_viewportHeight; }
    void setViewportSize(float width, float height) { 
        m_viewportWidth = width; 
        m_viewportHeight = height; 
        setAspectRatio(width / height);
    }
    
    // Position for gizmo system
    glm::vec3 getPosition() const;

private:
    Transform m_transform;
    
    // Projection parameters
    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;
    
    bool m_isPerspective;
    glm::mat4 m_projectionMatrix;
    
    // Orthographic parameters (when not perspective)
    float m_left, m_right, m_bottom, m_top;
    
    // Viewport dimensions
    float m_viewportWidth = 800.0f;
    float m_viewportHeight = 600.0f;
    
    void updateProjectionMatrix();
};
