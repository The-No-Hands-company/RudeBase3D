#pragma once

#include "Common.h"
#include "core/math/Transform.h"
#include <glm/glm.hpp>

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
    void lookAt(const QVector3D& target, const QVector3D& up = WORLD_UP);
    void orbit(const QVector3D& center, float deltaYaw, float deltaPitch);
    void pan(const QVector3D& delta);
    void zoom(float delta);
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    
    // Matrix getters (Qt versions)
    QMatrix4x4 getViewMatrix() const;
    QMatrix4x4 getProjectionMatrix() const { return m_projectionMatrix; }
    QMatrix4x4 getViewProjectionMatrix() const;
    
    // GLM matrix getters for gizmo system
    glm::mat4 getViewMatrixGLM() const;
    glm::mat4 getProjectionMatrixGLM() const;
    
    // Ray casting for picking
    QVector3D screenToWorldRay(const QVector2D& screenPos, const QSize& viewportSize) const;
    QVector3D getWorldPosition() const { return m_transform.getPosition(); }
    QVector3D getForward() const { return m_transform.getForward(); }
    QVector3D getRight() const { return m_transform.getRight(); }
    QVector3D getUp() const { return m_transform.getUp(); }
    
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
    QMatrix4x4 m_projectionMatrix;
    
    // Orthographic parameters (when not perspective)
    float m_left, m_right, m_bottom, m_top;
    
    // Viewport dimensions
    float m_viewportWidth = 800.0f;
    float m_viewportHeight = 600.0f;
    
    void updateProjectionMatrix();
};
