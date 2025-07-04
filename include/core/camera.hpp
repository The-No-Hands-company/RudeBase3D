#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include "core/math/Transform.h"

/**
 * @brief Camera class for handling view and projection matrices
 * 
 * This is a wrapper around the existing Camera class in the scene namespace.
 * It provides compatibility for older code that expects a camera.hpp header.
 */
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
    
    // Getters for projection parameters
    float getFOV() const { return m_fov; }
    float getAspectRatio() const { return m_aspectRatio; }
    float getNearPlane() const { return m_nearPlane; }
    float getFarPlane() const { return m_farPlane; }
    bool isOrthographic() const { return m_isOrthographic; }
    
    // Matrix getters
    const QMatrix4x4& getViewMatrix() const { return m_viewMatrix; }
    const QMatrix4x4& getProjectionMatrix() const { return m_projectionMatrix; }
    QMatrix4x4 getViewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }
    
    // Camera movement
    void lookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up);
    void orbit(float deltaX, float deltaY, const QVector3D& pivot);
    void pan(float deltaX, float deltaY);
    void zoom(float delta);
    
    // Get camera vectors
    QVector3D getPosition() const { return m_transform.getPosition(); }
    QVector3D getForward() const { return m_transform.getForward(); }
    QVector3D getUp() const { return m_transform.getUp(); }
    QVector3D getRight() const { return m_transform.getRight(); }
    
    // Helper methods for picking/raycasting
    QVector3D screenToWorldPoint(float screenX, float screenY, float depth) const;
    QVector3D screenToWorldDirection(float screenX, float screenY) const;
    QVector3D getViewportSize() const { return QVector3D(m_viewportWidth, m_viewportHeight, 0); }
    int getViewportWidth() const { return m_viewportWidth; }
    int getViewportHeight() const { return m_viewportHeight; }
    
    // For compatibility with older code
    const QMatrix4x4& getProjMatrix() const { return getProjectionMatrix(); }
    void setViewportSize(int width, int height) { m_viewportWidth = width; m_viewportHeight = height; updateAspectRatio(); }
    
    // Update matrices
    void updateViewMatrix();
    void updateProjectionMatrix();

private:
    Transform m_transform;
    
    // Projection parameters
    bool m_isOrthographic;
    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;
    
    // Orthographic parameters
    float m_left, m_right, m_bottom, m_top;
    
    // Cached matrices
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
    
    // Viewport dimensions
    int m_viewportWidth;
    int m_viewportHeight;
    
    void updateAspectRatio();
};
