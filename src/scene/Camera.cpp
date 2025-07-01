#include "Camera.h"
#include <cmath>

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
    m_transform.setPosition(QVector3D(7.0f, 5.0f, 7.0f));
    m_transform.lookAt(QVector3D(0.0f, 0.0f, 0.0f));
    
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

void Camera::lookAt(const QVector3D& target, const QVector3D& up)
{
    QVector3D position = m_transform.getPosition();
    QVector3D forward = (target - position).normalized();
    QVector3D right = QVector3D::crossProduct(forward, up).normalized();
    QVector3D actualUp = QVector3D::crossProduct(right, forward);
    
    // Create rotation matrix
    QMatrix4x4 rotationMatrix;
    rotationMatrix.setRow(0, QVector4D(right, 0.0f));
    rotationMatrix.setRow(1, QVector4D(actualUp, 0.0f));
    rotationMatrix.setRow(2, QVector4D(-forward, 0.0f));
    rotationMatrix.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    
    QQuaternion rotation = QQuaternion::fromRotationMatrix(rotationMatrix.normalMatrix());
    m_transform.setRotation(rotation);
}

void Camera::orbit(const QVector3D& center, float deltaYaw, float deltaPitch)
{
    QVector3D position = m_transform.getPosition();
    QVector3D offset = position - center;
    
    // Convert to spherical coordinates
    float radius = offset.length();
    float yaw = atan2(offset.x(), offset.z());
    float pitch = asin(offset.y() / radius);
    
    // Apply deltas (convert degrees to radians)
    yaw += deltaYaw * PI / 180.0f;
    pitch += deltaPitch * PI / 180.0f;
    
    // Clamp pitch to avoid gimbal lock
    pitch = std::max(-PI/2.0f + 0.01f, std::min(PI/2.0f - 0.01f, pitch));
    
    // Convert back to Cartesian coordinates (corrected formula)
    QVector3D newOffset;
    newOffset.setX(radius * cos(pitch) * sin(yaw));
    newOffset.setY(radius * sin(pitch));
    newOffset.setZ(radius * cos(pitch) * cos(yaw));
    
    // Set new position and look at target
    m_transform.setPosition(center + newOffset);
    lookAt(center);
}

void Camera::pan(const QVector3D& delta)
{
    QVector3D right = m_transform.getRight();
    QVector3D up = m_transform.getUp();
    
    QVector3D translation = right * delta.x() + up * delta.y();
    m_transform.translate(translation);
}

void Camera::zoom(float delta)
{
    QVector3D forward = m_transform.getForward();
    m_transform.translate(forward * delta);
}

void Camera::moveForward(float distance)
{
    QVector3D forward = m_transform.getForward();
    m_transform.translate(forward * distance);
}

void Camera::moveRight(float distance)
{
    QVector3D right = m_transform.getRight();
    m_transform.translate(right * distance);
}

void Camera::moveUp(float distance)
{
    QVector3D up = m_transform.getUp();
    m_transform.translate(up * distance);
}

QMatrix4x4 Camera::getViewMatrix() const
{
    return m_transform.getModelMatrix().inverted();
}

QMatrix4x4 Camera::getViewProjectionMatrix() const
{
    return m_projectionMatrix * getViewMatrix();
}

QVector3D Camera::screenToWorldRay(const QVector2D& screenPos, const QSize& viewportSize) const
{
    // Convert screen coordinates to normalized device coordinates
    float ndcX = (2.0f * screenPos.x()) / viewportSize.width() - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPos.y()) / viewportSize.height();
    
    // Convert to clip coordinates
    QVector4D clipCoords(ndcX, ndcY, -1.0f, 1.0f);
    
    // Convert to eye coordinates
    QMatrix4x4 invProjection = m_projectionMatrix.inverted();
    QVector4D eyeCoords = invProjection * clipCoords;
    eyeCoords.setZ(-1.0f);
    eyeCoords.setW(0.0f);
    
    // Convert to world coordinates
    QMatrix4x4 invView = getViewMatrix().inverted();
    QVector4D worldCoords = invView * eyeCoords;
    
    return QVector3D(worldCoords).normalized();
}

void Camera::updateProjectionMatrix()
{
    m_projectionMatrix.setToIdentity();
    
    if (m_isPerspective) {
        m_projectionMatrix.perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
    } else {
        m_projectionMatrix.ortho(m_left, m_right, m_bottom, m_top, m_nearPlane, m_farPlane);
    }
}
