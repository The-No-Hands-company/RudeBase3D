#include "CameraController.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "scene/SceneObject.h"
#include "geometry/core/Mesh.h"
#include "core/math/Transform.h"
#include <QTimer>
#include <algorithm>
#include <cmath>

CameraController::CameraController(QObject* parent)
    : QObject(parent)
    , m_cameraMode(CameraMode::Orbit)
    , m_orbitMode(OrbitMode::SceneCenter)
    , m_customPivot(0.0f, 0.0f, 0.0f)
    , m_movementSpeed(5.0f)
    , m_rotationSpeed(1.0f)
    , m_panSpeed(0.01f)
    , m_zoomSpeed(0.1f)
    , m_animationSpeed(2.0f)
    , m_invertY(false)
    , m_orbitDistance(10.0f)
    , m_orbitCenter(0.0f, 0.0f, 0.0f)
    , m_animationTimer(nullptr)
    , m_animationTime(0.0f)
    , m_animationDuration(1.0f)
{
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(16); // ~60 FPS
    connect(m_animationTimer, &QTimer::timeout, this, &CameraController::updateAnimation);
}

void CameraController::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
    if (m_camera) {
        updateOrbitDistance();
    }
}

void CameraController::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
}

void CameraController::setCameraMode(CameraMode mode)
{
    if (m_cameraMode != mode) {
        m_cameraMode = mode;
        emit cameraModeChanged(mode);
    }
}

void CameraController::setOrbitMode(OrbitMode mode)
{
    if (m_orbitMode != mode) {
        m_orbitMode = mode;
        updateOrbitDistance();
        emit orbitModeChanged(mode);
    }
}

void CameraController::setCustomPivot(const QVector3D& pivot)
{
    m_customPivot = pivot;
    if (m_orbitMode == OrbitMode::CustomPivot) {
        updateOrbitDistance();
    }
}

void CameraController::resetCamera()
{
    if (!m_camera) return;
    
    // Reset to default position based on scene
    QVector3D target = getOrbitCenter();
    QVector3D position = target + QVector3D(5.0f, 5.0f, 5.0f);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    
    m_orbitCenter = target;
    updateOrbitDistance();
    
    emit cameraChanged();
}

void CameraController::frameScene(bool animate)
{
    if (!m_camera || !m_scene || m_scene->isEmpty()) {
        resetCamera();
        return;
    }
    
    QVector3D sceneCenter = getSceneCenter();
    QVector3D sceneMin = m_scene->getSceneBoundingBoxMin();
    QVector3D sceneMax = m_scene->getSceneBoundingBoxMax();
    QVector3D sceneSize = sceneMax - sceneMin;
    
    QVector3D targetPosition = calculateFramingPosition(sceneCenter, sceneSize);
    
    if (animate && !isAnimating()) {
        startAnimation(targetPosition, sceneCenter);
    } else {
        m_camera->getTransform().setPosition(targetPosition);
        m_camera->lookAt(sceneCenter);
        m_orbitCenter = sceneCenter;
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::frameSelectedObject(bool animate)
{
    if (!m_camera || !m_scene) return;
    
    auto selectedObject = m_scene->getSelectedObject();
    if (!selectedObject) {
        frameScene(animate);
        return;
    }
    
    focusOnObject(selectedObject, animate);
}

void CameraController::frameAll(bool animate)
{
    frameScene(animate);
}

void CameraController::updateAspectRatio(float aspectRatio)
{
    if (!m_camera) return;
    
    m_camera->setAspectRatio(aspectRatio);
    emit cameraChanged();
}

void CameraController::orbit(float deltaYaw, float deltaPitch)
{
    if (m_cameraMode != CameraMode::Orbit) return;
    
    QVector3D center = getOrbitCenter();
    orbitAroundPoint(center, deltaYaw, deltaPitch);
}

void CameraController::orbitAroundPoint(const QVector3D& center, float deltaYaw, float deltaPitch)
{
    if (!m_camera) return;
    
    // Apply Y inversion if enabled
    if (m_invertY) {
        deltaPitch = -deltaPitch;
    }
    
    // Scale by rotation speed
    deltaYaw *= m_rotationSpeed;
    deltaPitch *= m_rotationSpeed;
    
    // Get current position relative to center
    QVector3D currentPos = m_camera->getWorldPosition();
    QVector3D offset = currentPos - center;
    
    // Create rotation matrices
    QMatrix4x4 yawRotation;
    yawRotation.rotate(deltaYaw, QVector3D(0, 1, 0)); // World Y axis
    
    // For pitch, we need to rotate around the camera's right vector
    QVector3D rightVector = m_camera->getRight();
    QMatrix4x4 pitchRotation;
    pitchRotation.rotate(deltaPitch, rightVector);
    
    // Apply rotations
    QVector4D newOffset4 = pitchRotation * yawRotation * QVector4D(offset, 1.0f);
    QVector3D newOffset = newOffset4.toVector3D();
    
    // Clamp pitch to avoid flipping
    QVector3D newPos = center + newOffset;
    QVector3D toCenter = (center - newPos).normalized();
    float dot = QVector3D::dotProduct(toCenter, QVector3D(0, 1, 0));
    
    // Prevent flipping by limiting pitch
    if (dot > 0.95f || dot < -0.95f) {
        return; // Skip this movement to prevent gimbal lock
    }
    
    // Update camera position and look at center
    m_camera->getTransform().setPosition(newPos);
    m_camera->lookAt(center);
    
    // Update orbit state
    m_orbitCenter = center;
    m_orbitDistance = newOffset.length();
    
    emit cameraChanged();
}

void CameraController::pan(const QVector3D& delta)
{
    if (!m_camera) return;
    
    // Scale by pan speed
    QVector3D scaledDelta = delta * m_panSpeed;
    
    // Transform delta to world space using camera orientation
    QVector3D right = m_camera->getRight();
    QVector3D up = m_camera->getUp();
    
    QVector3D worldDelta = right * scaledDelta.x() + up * scaledDelta.y();
    
    // Move camera and orbit center
    m_camera->getTransform().translate(worldDelta);
    m_orbitCenter += worldDelta;
    
    emit cameraChanged();
}

void CameraController::dolly(float delta)
{
    if (!m_camera) return;
    
    QVector3D forward = m_camera->getForward();
    QVector3D movement = forward * (delta * m_movementSpeed);
    
    m_camera->getTransform().translate(movement);
    
    // Update orbit distance if in orbit mode
    if (m_cameraMode == CameraMode::Orbit) {
        updateOrbitDistance();
    }
    
    emit cameraChanged();
}

void CameraController::zoom(float delta)
{
    if (!m_camera) return;
    
    if (m_cameraMode == CameraMode::Orbit) {
        // Dolly in/out for orbit mode
        dolly(delta * m_zoomSpeed);
    } else {
        // FOV zoom for fly mode
        float currentFOV = m_camera->getFOV();
        float newFOV = currentFOV - (delta * m_zoomSpeed * 5.0f);
        newFOV = qBound(10.0f, newFOV, 120.0f); // Clamp FOV
        m_camera->setFOV(newFOV);
        emit cameraChanged();
    }
}

// Advanced movement methods for fly mode
void CameraController::moveForward(float distance)
{
    if (!m_camera) return;
    QVector3D movement = m_camera->getForward() * (distance * m_movementSpeed);
    m_camera->getTransform().translate(movement);
    emit cameraChanged();
}

void CameraController::moveBackward(float distance)
{
    moveForward(-distance);
}

void CameraController::moveLeft(float distance)
{
    if (!m_camera) return;
    QVector3D movement = -m_camera->getRight() * (distance * m_movementSpeed);
    m_camera->getTransform().translate(movement);
    emit cameraChanged();
}

void CameraController::moveRight(float distance)
{
    moveLeft(-distance);
}

void CameraController::moveUp(float distance)
{
    if (!m_camera) return;
    QVector3D movement = QVector3D(0, 1, 0) * (distance * m_movementSpeed);
    m_camera->getTransform().translate(movement);
    emit cameraChanged();
}

void CameraController::moveDown(float distance)
{
    moveUp(-distance);
}

void CameraController::rotate(float deltaPitch, float deltaYaw, float deltaRoll)
{
    if (!m_camera || m_cameraMode != CameraMode::Fly) return;
    
    // Apply Y inversion if enabled
    if (m_invertY) {
        deltaPitch = -deltaPitch;
    }
    
    // Scale by rotation speed
    deltaPitch *= m_rotationSpeed;
    deltaYaw *= m_rotationSpeed;
    deltaRoll *= m_rotationSpeed;
    
    // Get current rotation as Euler angles
    QVector3D currentRotation = m_camera->getTransform().getEulerAngles();
    
    // Apply rotations
    currentRotation.setX(currentRotation.x() + deltaPitch);
    currentRotation.setY(currentRotation.y() + deltaYaw);
    currentRotation.setZ(currentRotation.z() + deltaRoll);
    
    // Clamp pitch to avoid flipping
    currentRotation.setX(qBound(-89.0f, currentRotation.x(), 89.0f));
    
    m_camera->getTransform().setEulerAngles(currentRotation);
    emit cameraChanged();
}

void CameraController::focusOnPoint(const QVector3D& point, bool animate)
{
    if (!m_camera) return;
    
    QVector3D direction = (point - m_camera->getWorldPosition()).normalized();
    QVector3D targetPosition = point - direction * 5.0f; // 5 units back from point
    
    if (animate && !isAnimating()) {
        startAnimation(targetPosition, point);
    } else {
        m_camera->getTransform().setPosition(targetPosition);
        m_camera->lookAt(point);
        m_orbitCenter = point;
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::focusOnObject(std::shared_ptr<SceneObject> object, bool animate)
{
    if (!object || !m_camera) return;
    
    auto mesh = object->getMesh();
    if (!mesh) return;
    
    // Get object center and size
    QVector3D objectMin = mesh->getBoundingBoxMin();
    QVector3D objectMax = mesh->getBoundingBoxMax();
    QMatrix4x4 transform = object->getTransform().getModelMatrix();
    
    // Transform bounding box to world space
    QVector3D center = transform.map((objectMin + objectMax) * 0.5f);
    QVector3D size = objectMax - objectMin;
    
    QVector3D targetPosition = calculateFramingPosition(center, size);
    
    if (animate && !isAnimating()) {
        startAnimation(targetPosition, center);
    } else {
        m_camera->getTransform().setPosition(targetPosition);
        m_camera->lookAt(center);
        m_orbitCenter = center;
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::setViewDirection(const QVector3D& direction, const QVector3D& up, bool animate)
{
    if (!m_camera) return;
    
    QVector3D currentPos = m_camera->getWorldPosition();
    QVector3D target = currentPos + direction.normalized() * 10.0f;
    
    if (animate && !isAnimating()) {
        startAnimation(currentPos, target);
    } else {
        m_camera->lookAt(target, up);
        emit cameraChanged();
    }
}

// Predefined views
void CameraController::setTopView(bool animate)
{
    QVector3D center = getOrbitCenter();
    QVector3D position = center + QVector3D(0, 10, 0);
    
    if (animate && !isAnimating()) {
        startAnimation(position, center);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(center, QVector3D(0, 0, -1));
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::setBottomView(bool animate)
{
    QVector3D center = getOrbitCenter();
    QVector3D position = center + QVector3D(0, -10, 0);
    
    if (animate && !isAnimating()) {
        startAnimation(position, center);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(center, QVector3D(0, 0, 1));
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::setFrontView(bool animate)
{
    QVector3D center = getOrbitCenter();
    QVector3D position = center + QVector3D(0, 0, 10);
    
    if (animate && !isAnimating()) {
        startAnimation(position, center);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(center);
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::setBackView(bool animate)
{
    QVector3D center = getOrbitCenter();
    QVector3D position = center + QVector3D(0, 0, -10);
    
    if (animate && !isAnimating()) {
        startAnimation(position, center);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(center);
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::setLeftView(bool animate)
{
    QVector3D center = getOrbitCenter();
    QVector3D position = center + QVector3D(-10, 0, 0);
    
    if (animate && !isAnimating()) {
        startAnimation(position, center);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(center);
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::setRightView(bool animate)
{
    QVector3D center = getOrbitCenter();
    QVector3D position = center + QVector3D(10, 0, 0);
    
    if (animate && !isAnimating()) {
        startAnimation(position, center);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(center);
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::setIsometricView(bool animate)
{
    QVector3D center = getOrbitCenter();
    QVector3D position = center + QVector3D(7.07f, 7.07f, 7.07f); // 45° angles
    
    if (animate && !isAnimating()) {
        startAnimation(position, center);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(center);
        updateOrbitDistance();
        emit cameraChanged();
    }
}

// Query methods
QVector3D CameraController::getWorldPosition() const
{
    if (!m_camera) return QVector3D();
    return m_camera->getWorldPosition();
}

QMatrix4x4 CameraController::getViewMatrix() const
{
    if (!m_camera) return QMatrix4x4();
    return m_camera->getViewMatrix();
}

QMatrix4x4 CameraController::getProjectionMatrix() const
{
    if (!m_camera) return QMatrix4x4();
    return m_camera->getProjectionMatrix();
}

QVector3D CameraController::screenToWorldRay(const QVector2D& screenPos, const QSize& viewportSize) const
{
    if (!m_camera) return QVector3D();
    return m_camera->screenToWorldRay(screenPos, viewportSize);
}

QVector3D CameraController::getCurrentPivot() const
{
    return getOrbitCenter();
}

float CameraController::getDistanceToTarget() const
{
    return m_orbitDistance;
}

// Animation methods
void CameraController::updateAnimation()
{
    if (!m_camera || !isAnimating()) return;
    
    m_animationTime += m_animationTimer->interval() / 1000.0f;
    float t = m_animationTime / m_animationDuration;
    
    if (t >= 1.0f) {
        // Animation complete
        t = 1.0f;
        stopAnimation();
    }
    
    // Smooth easing (ease-in-out)
    t = t * t * (3.0f - 2.0f * t);
    
    // Interpolate position and look-at target
    QVector3D currentPos = m_animStartPosition + (m_animTargetPosition - m_animStartPosition) * t;
    QVector3D currentLookAt = m_animStartLookAt + (m_animTargetLookAt - m_animStartLookAt) * t;
    
    m_camera->getTransform().setPosition(currentPos);
    m_camera->lookAt(currentLookAt);
    
    if (t >= 1.0f) {
        m_orbitCenter = currentLookAt;
        updateOrbitDistance();
    }
    
    emit cameraChanged();
}

void CameraController::startAnimation(const QVector3D& targetPosition, const QVector3D& targetLookAt)
{
    if (!m_camera) return;
    
    m_animStartPosition = m_camera->getWorldPosition();
    m_animTargetPosition = targetPosition;
    m_animStartLookAt = getOrbitCenter();
    m_animTargetLookAt = targetLookAt;
    m_animationTime = 0.0f;
    m_animationDuration = 1.0f / m_animationSpeed;
    
    m_animationTimer->start();
}

void CameraController::stopAnimation()
{
    if (m_animationTimer) {
        m_animationTimer->stop();
    }
    m_animationTime = 0.0f;
}

// Helper methods
QVector3D CameraController::getSceneCenter() const
{
    if (!m_scene) {
        return QVector3D(0.0f, 0.0f, 0.0f);
    }
    
    try {
        if (m_scene->isEmpty()) {
            return QVector3D(0.0f, 0.0f, 0.0f);
        }
        return m_scene->getSceneBoundingBoxCenter();
    } catch (...) {
        // Fall back to origin if scene access fails
        return QVector3D(0.0f, 0.0f, 0.0f);
    }
}

QVector3D CameraController::getSelectionCenter() const
{
    if (!m_scene) return QVector3D(0.0f, 0.0f, 0.0f);
    
    auto selectedObject = m_scene->getSelectedObject();
    if (!selectedObject) return getSceneCenter();
    
    auto mesh = selectedObject->getMesh();
    if (!mesh) return getSceneCenter();
    
    QVector3D min = mesh->getBoundingBoxMin();
    QVector3D max = mesh->getBoundingBoxMax();
    QMatrix4x4 transform = selectedObject->getTransform().getModelMatrix();
    
    return transform.map((min + max) * 0.5f);
}

float CameraController::calculateFramingDistance(const QVector3D& sceneSize) const
{
    if (!m_camera) return 10.0f;
    
    float maxDimension = std::max({sceneSize.x(), sceneSize.y(), sceneSize.z()});
    if (maxDimension < 0.1f) {
        maxDimension = 2.0f;
    }
    
    float fov = m_camera->getFOV();
    float distance = (maxDimension * 0.5f) / std::tan(qDegreesToRadians(fov * 0.5f));
    
    return std::max(distance * 1.5f, maxDimension * 2.0f);
}

QVector3D CameraController::calculateFramingPosition(const QVector3D& target, const QVector3D& size) const
{
    float distance = calculateFramingDistance(size);
    
    // Position camera at a good viewing angle
    QVector3D direction;
    if (m_camera) {
        // Maintain current viewing direction if possible
        direction = (target - m_camera->getWorldPosition()).normalized();
        if (direction.length() < 0.1f) {
            direction = QVector3D(1.0f, 1.0f, 1.0f).normalized();
        }
    } else {
        direction = QVector3D(1.0f, 1.0f, 1.0f).normalized();
    }
    
    return target - direction * distance;
}

QVector3D CameraController::getOrbitCenter() const
{
    switch (m_orbitMode) {
        case OrbitMode::WorldCenter:
            return QVector3D(0.0f, 0.0f, 0.0f);
        case OrbitMode::SceneCenter:
            try {
                return getSceneCenter();
            } catch (...) {
                return QVector3D(0.0f, 0.0f, 0.0f);
            }
        case OrbitMode::Selection:
            try {
                return getSelectionCenter();
            } catch (...) {
                return getSceneCenter();
            }
        case OrbitMode::CustomPivot:
            return m_customPivot;
        default:
            return m_orbitCenter;
    }
}

void CameraController::updateOrbitDistance()
{
    if (!m_camera) return;
    
    QVector3D center = getOrbitCenter();
    QVector3D cameraPos = m_camera->getWorldPosition();
    m_orbitDistance = (cameraPos - center).length();
    m_orbitCenter = center;
}
