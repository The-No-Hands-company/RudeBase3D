#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "core/transform.hpp"

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
    const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
    const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
    glm::mat4 getViewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }
    
    // Camera movement
    void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
    void orbit(float deltaX, float deltaY, const glm::vec3& pivot);
    void pan(float deltaX, float deltaY);
    void zoom(float delta);
    
    // Get camera vectors
    glm::vec3 getPosition() const { return m_transform.getPosition(); }
    glm::vec3 getForward() const { return m_transform.getForward(); }
    glm::vec3 getUp() const { return m_transform.getUp(); }
    glm::vec3 getRight() const { return m_transform.getRight(); }
    
    // Helper methods for picking/raycasting
    glm::vec3 screenToWorldPoint(float screenX, float screenY, float depth) const;
    glm::vec3 screenToWorldDirection(float screenX, float screenY) const;
    glm::vec3 getViewportSize() const { return glm::vec3(m_viewportWidth, m_viewportHeight, 0); }
    int getViewportWidth() const { return m_viewportWidth; }
    int getViewportHeight() const { return m_viewportHeight; }
    
    // For compatibility with older code
    const glm::mat4& getProjMatrix() const { return getProjectionMatrix(); }
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
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    
    // Viewport dimensions
    int m_viewportWidth;
    int m_viewportHeight;
    
    void updateAspectRatio();
};
