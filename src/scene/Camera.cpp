#include "Camera.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : m_fov(50.0f)  // Slightly wider FOV for better workspace view
    , m_aspectRatio(16.0f / 9.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(1000.0f)
    , m_isPerspective(true)
    , m_left(-1.0f)
    , m_right(1.0f)
    , m_bottom(-1.0f)
    , m_top(1.0f)
{
    // Set professional isometric-style default camera position
    // Position camera at 45-degree angle for good 3D overview
    m_transform.setPosition(glm::vec3(7.0f, 5.0f, 7.0f));
    m_transform.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    updateProjectionMatrix();
}

void Camera::setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_isPerspective = true;
    updateProjectionMatrix();
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_isPerspective = false;
    updateProjectionMatrix();
}

void Camera::lookAt(const glm::vec3& target, const glm::vec3& up)
{
    glm::vec3 position = m_transform.getPosition();
    glm::vec3 forward = glm::normalize(target - position);
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    glm::vec3 actualUp = glm::cross(right, forward);
    // Build rotation matrix from basis vectors
    glm::mat3 rotationMatrix(
        right,
        actualUp,
        -forward
    );
    m_transform.setRotation(rotationMatrix);
}

void Camera::orbit(const glm::vec3& center, float deltaYaw, float deltaPitch)
{
    glm::vec3 position = m_transform.getPosition();
    glm::vec3 offset = position - center;
    float radius = glm::length(offset);
    float yaw = atan2(offset.x, offset.z);
    float pitch = asin(offset.y / radius);
    yaw += glm::radians(deltaYaw);
    pitch += glm::radians(deltaPitch);
    pitch = std::max(-glm::half_pi<float>() + 0.01f, std::min(glm::half_pi<float>() - 0.01f, pitch));
    glm::vec3 newOffset;
    newOffset.x = radius * cos(pitch) * sin(yaw);
    newOffset.y = radius * sin(pitch);
    newOffset.z = radius * cos(pitch) * cos(yaw);
    m_transform.setPosition(center + newOffset);
    lookAt(center);
}

void Camera::pan(const glm::vec3& delta)
{
    glm::vec3 right = m_transform.getRight();
    glm::vec3 up = m_transform.getUp();
    glm::vec3 translation = right * delta.x + up * delta.y;
    m_transform.translate(translation);
}

void Camera::zoom(float delta)
{
    glm::vec3 forward = m_transform.getForward();
    m_transform.translate(forward * delta);
}

void Camera::moveForward(float distance)
{
    glm::vec3 forward = m_transform.getForward();
    m_transform.translate(forward * distance);
}

void Camera::moveRight(float distance)
{
    glm::vec3 right = m_transform.getRight();
    m_transform.translate(right * distance);
}

void Camera::moveUp(float distance)
{
    glm::vec3 up = m_transform.getUp();
    m_transform.translate(up * distance);
}

void Camera::updateProjectionMatrix()
{
    if (m_isPerspective) {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
    } else {
        m_projectionMatrix = glm::ortho(m_left, m_right, m_bottom, m_top, m_nearPlane, m_farPlane);
    }
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::inverse(m_transform.getModelMatrix());
}

glm::mat4 Camera::getProjectionMatrix() const {
    return m_projectionMatrix;
}

glm::vec3 Camera::screenToWorldRay(const glm::vec2& screenPos, const glm::vec2& screenSize) const {
    // Convert screen coordinates to normalized device coordinates
    glm::vec2 ndc = (screenPos / screenSize) * 2.0f - 1.0f;
    ndc.y = -ndc.y; // Flip Y coordinate
    
    // Create ray in view space
    glm::vec4 rayClip = glm::vec4(ndc.x, ndc.y, -1.0f, 1.0f);
    glm::vec4 rayEye = glm::inverse(m_projectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    
    // Transform to world space
    glm::vec3 rayWorld = glm::vec3(glm::inverse(getViewMatrix()) * rayEye);
    return glm::normalize(rayWorld);
}

// GLM method implementations
glm::mat4 Camera::getViewMatrixGLM() const {
    return getViewMatrix();
}

glm::mat4 Camera::getProjectionMatrixGLM() const {
    return m_projectionMatrix;
}

glm::vec3 Camera::getPosition() const {
    return m_transform.getPosition();
}
