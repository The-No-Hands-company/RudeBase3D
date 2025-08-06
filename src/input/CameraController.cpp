
#include "CameraController.h"
#include "scene/Camera.h"
#include "core/scene.hpp"
#include "core/scene_manager.hpp"
#include "tools/selection/SelectionManager.h"
#include "scene/SceneObject.h"
#include "core/mesh.hpp"
#include "core/math/Transform.h"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

CameraController::CameraController()
    : m_cameraMode(CameraMode::Orbit)
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
    , m_animationTime(0.0f)
    , m_animationDuration(1.0f)
{
    // No QTimer, use manual time tracking for animation
}

void CameraController::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
    if (m_camera) {
        updateOrbitDistance();
    }
}

void CameraController::setSceneManager(std::shared_ptr<rude::SceneManager> sceneManager)
{
    m_sceneManager = sceneManager;
}

void CameraController::setSelectionManager(std::shared_ptr<SelectionManager> selectionManager)
{
    m_selectionManager = selectionManager;
}

void CameraController::setCameraMode(CameraMode mode)
{
    if (m_cameraMode != mode) {
        m_cameraMode = mode;
        // cameraModeChanged(mode); // Qt signal removed
    }
}

void CameraController::setOrbitMode(OrbitMode mode)
{
    if (m_orbitMode != mode) {
        m_orbitMode = mode;
        updateOrbitDistance();
        // orbitModeChanged(mode); // Qt signal removed
    }
}


void CameraController::setCustomPivot(const glm::vec3& pivot)
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
    glm::vec3 target = getOrbitCenter();
    glm::vec3 position = target + glm::vec3(5.0f, 5.0f, 5.0f);
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    m_orbitCenter = target;
    updateOrbitDistance();
    // cameraChanged(); // Qt signal removed
}

void CameraController::frameScene(bool animate)
{
    if (!m_camera || !m_sceneManager || m_sceneManager->isEmpty()) {
        resetCamera();
        return;
    }
    glm::vec3 sceneCenter = getSceneCenter();
    glm::vec3 sceneSize = glm::vec3(2.0f, 2.0f, 2.0f); // Default size
    glm::vec3 targetPosition = calculateFramingPosition(sceneCenter, sceneSize);
    if (animate && !isAnimating()) {
        startAnimation(targetPosition, sceneCenter);
    } else {
        m_camera->getTransform().setPosition(targetPosition);
        m_camera->lookAt(sceneCenter);
        m_orbitCenter = sceneCenter;
        updateOrbitDistance();
        // cameraChanged(); // Qt signal removed
    }
}

void CameraController::frameSelectedObject(bool animate)
{
    if (!m_camera || !m_selectionManager) return;
    auto selectedVertices = m_selectionManager->getSelectedVertices();
    if (selectedVertices.empty()) {
        frameScene(animate);
        return;
    }
    glm::vec3 center = getSelectionCenter();
    focusOnPoint(center, animate);
}

void CameraController::frameAll(bool animate)
{
    frameScene(animate);
}

void CameraController::updateAspectRatio(float aspectRatio)
{
    if (!m_camera) return;
    m_camera->setAspectRatio(aspectRatio);
    // cameraChanged(); // Qt signal removed
}


void CameraController::orbit(float deltaYaw, float deltaPitch)
{
    if (m_cameraMode != CameraMode::Orbit) return;
    glm::vec3 center = getOrbitCenter();
    orbitAroundPoint(center, deltaYaw, deltaPitch);
}


void CameraController::orbitAroundPoint(const glm::vec3& center, float deltaYaw, float deltaPitch)
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
    glm::vec3 currentPos = m_camera->getWorldPosition();
    glm::vec3 offset = currentPos - center;

    // Yaw: rotate around world Y axis
    glm::mat4 yawRot = glm::rotate(glm::mat4(1.0f), glm::radians(deltaYaw), glm::vec3(0, 1, 0));
    offset = glm::vec3(yawRot * glm::vec4(offset, 1.0f));

    // Pitch: rotate around camera's right vector
    glm::vec3 rightVector = m_camera->getRight();
    glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), glm::radians(deltaPitch), rightVector);
    offset = glm::vec3(pitchRot * glm::vec4(offset, 1.0f));

    // Clamp pitch to avoid flipping
    glm::vec3 newPos = center + offset;
    glm::vec3 toCenter = glm::normalize(center - newPos);
    float dot = glm::dot(toCenter, glm::vec3(0, 1, 0));
    if (dot > 0.95f || dot < -0.95f) {
        return; // Prevent gimbal lock
    }

    // Update camera position and look at center
    m_camera->getTransform().setPosition(newPos);
    m_camera->lookAt(center);

    // Update orbit state
    m_orbitCenter = center;
    m_orbitDistance = glm::length(offset);

    // cameraChanged(); // Qt signal removed
}


void CameraController::pan(const glm::vec2& delta)
{
    if (!m_camera) return;
    // Scale by pan speed
    glm::vec2 scaledDelta = delta * m_panSpeed;
    // Transform delta to world space using camera orientation
    glm::vec3 right = m_camera->getRight();
    glm::vec3 up = m_camera->getUp();
    glm::vec3 worldDelta = right * scaledDelta.x + up * scaledDelta.y;
    // Move camera and orbit center
    m_camera->getTransform().translate(worldDelta);
    m_orbitCenter += worldDelta;

}

void CameraController::dolly(float delta)
{
    if (!m_camera) return;
    
    glm::vec3 forward = m_camera->getForward();
    glm::vec3 movement = forward * (delta * m_movementSpeed);
    m_camera->getTransform().translate(movement);
    // Update orbit distance if in orbit mode
    if (m_cameraMode == CameraMode::Orbit) {
        updateOrbitDistance();
    }
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
        newFOV = std::clamp(newFOV, 10.0f, 120.0f); // Clamp FOV
        m_camera->setFOV(newFOV);
        // cameraChanged(); // Qt signal removed
    }
}

// Advanced movement methods for fly mode
void CameraController::moveForward(float distance)
{
    if (!m_camera) return;
    glm::vec3 movement = m_camera->getForward() * (distance * m_movementSpeed);
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
    glm::vec3 movement = -m_camera->getRight() * (distance * m_movementSpeed);
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
    glm::vec3 movement = glm::vec3(0, 1, 0) * (distance * m_movementSpeed);
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
    glm::vec3 currentRotation = m_camera->getTransform().getEulerAngles();
    
    // Apply rotations
    currentRotation.x += deltaPitch;
    currentRotation.y += deltaYaw;
    currentRotation.z += deltaRoll;
    
    // Clamp pitch to avoid flipping
    currentRotation.x = glm::clamp(currentRotation.x, -89.0f, 89.0f);
    m_camera->getTransform().setEulerAngles(currentRotation);
    // cameraChanged(); // Qt signal removed
}


void CameraController::focusOnPoint(const glm::vec3& point, bool animate)
{
    if (!m_camera) return;

    glm::vec3 direction = glm::normalize(point - m_camera->getWorldPosition());
    glm::vec3 targetPosition = point - direction * 5.0f;

    if (animate && !isAnimating()) {
        startAnimation(targetPosition, point);
    } else {
        m_camera->getTransform().setPosition(targetPosition);
        m_camera->lookAt(point);
        m_orbitCenter = point;
        updateOrbitDistance();
        // cameraChanged(); // Qt signal removed
    }
}

void CameraController::focusOnObject(std::shared_ptr<SceneObject> object, bool animate)
{
    if (!object || !m_camera) return;
    
    auto mesh = object->getMesh();
    if (!mesh) return;
    // Get object center and size
    glm::vec3 min = mesh->getBoundingBoxMin();
    glm::vec3 max = mesh->getBoundingBoxMax();
    glm::vec3 center = (min + max) * 0.5f;
    glm::vec3 size = max - min;
    glm::vec3 targetPosition = calculateFramingPosition(center, size);
    if (animate && !isAnimating()) {
        startAnimation(targetPosition, center);
    } else {
        m_camera->getTransform().setPosition(targetPosition);
        m_camera->lookAt(center);
        m_orbitCenter = center;
        updateOrbitDistance();
        // cameraChanged(); // Qt signal removed
    }
}

void CameraController::setViewDirection(const glm::vec3& direction, const glm::vec3& up, bool animate)
{
    if (!m_camera) return;
    glm::vec3 currentPos = m_camera->getWorldPosition();
    glm::vec3 dir = glm::normalize(direction);
    glm::vec3 target = currentPos + dir * 10.0f;
    glm::vec3 upVec = glm::normalize(up);
    if (animate && !isAnimating()) {
        startAnimation(currentPos, target);
    } else {
        m_camera->lookAt(target, upVec);
    }
}

// Predefined views
void CameraController::setTopView(bool animate)
{
    glm::vec3 center = getOrbitCenter();
    glm::vec3 position = center + glm::vec3(0, 10, 0);
    glm::vec3 up = glm::vec3(0, 0, -1);
    if (animate && !isAnimating()) {
        startAnimation(position, center);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(center, up);
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::setBottomView(bool animate)
{
    glm::vec3 center = getOrbitCenter();
    glm::vec3 position = center + glm::vec3(0, -10, 0);
    glm::vec3 up = glm::vec3(0, 0, 1);
    if (animate && !isAnimating()) {
        startAnimation(position, center);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(center, up);
        updateOrbitDistance();
        emit cameraChanged();
    }
}

void CameraController::setFrontView(bool animate)
{
    glm::vec3 center = getOrbitCenter();
    glm::vec3 position = center + glm::vec3(0, 0, 10);
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
    glm::vec3 center = getOrbitCenter();
    glm::vec3 position = center + glm::vec3(0, 0, -10);
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
    glm::vec3 center = getOrbitCenter();
    glm::vec3 position = center + glm::vec3(-10, 0, 0);
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
    glm::vec3 center = getOrbitCenter();
    glm::vec3 position = center + glm::vec3(10, 0, 0);
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
    glm::vec3 center = getOrbitCenter();
    glm::vec3 position = center + glm::vec3(7.07f, 7.07f, 7.07f); // 45° angles
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

glm::vec3 CameraController::getWorldPosition() const
{
    if (!m_camera) return glm::vec3(0.0f);
    return m_camera->getWorldPosition();
}

glm::mat4 CameraController::getViewMatrix() const
{
    if (!m_camera) return glm::mat4(1.0f);
    return m_camera->getViewMatrix();
}

glm::mat4 CameraController::getProjectionMatrix() const
{
    if (!m_camera) return glm::mat4(1.0f);
    return m_camera->getProjectionMatrix();
}

glm::vec3 CameraController::screenToWorldRay(const glm::vec2& screenPos, const glm::ivec2& viewportSize) const
{
    if (!m_camera) return glm::vec3(0.0f);
    return m_camera->screenToWorldRay(screenPos, viewportSize);
}

glm::vec3 CameraController::getCurrentPivot() const
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
    // Manual time tracking: caller should update m_animationTime externally (e.g., per-frame)
    float t = m_animationTime / m_animationDuration;
    if (t >= 1.0f) {
        // Animation complete
        t = 1.0f;
        stopAnimation();
    }
    // Smooth easing (ease-in-out)
    t = t * t * (3.0f - 2.0f * t);
    // Interpolate position and look-at target
    glm::vec3 currentPos = m_animStartPosition + (m_animTargetPosition - m_animStartPosition) * t;
    glm::vec3 currentLookAt = m_animStartLookAt + (m_animTargetLookAt - m_animStartLookAt) * t;
    m_camera->getTransform().setPosition(currentPos);
    m_camera->lookAt(currentLookAt);
    if (t >= 1.0f) {
        m_orbitCenter = currentLookAt;
        updateOrbitDistance();
    }
}

void CameraController::startAnimation(const glm::vec3& targetPosition, const glm::vec3& targetLookAt)
{
    if (!m_camera) return;
    m_animStartPosition = m_camera->getWorldPosition();
    m_animTargetPosition = targetPosition;
    m_animStartLookAt = getOrbitCenter();
    m_animTargetLookAt = targetLookAt;
    m_animationTime = 0.0f;
    m_animationDuration = 1.0f / m_animationSpeed;
    // Manual time tracking: caller should update m_animationTime externally (e.g., per-frame)
}

void CameraController::stopAnimation()
{
    m_animationTime = 0.0f;
}

// Helper methods
glm::vec3 CameraController::getSceneCenter() const
{
    if (!m_sceneManager || m_sceneManager->isEmpty()) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
    auto scene = m_sceneManager->getScene();
    if (scene) {
        return scene->getSceneBoundingBoxCenter();
    }
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 CameraController::getSelectionCenter() const
{
    if (!m_selectionManager) return getSceneCenter();
    auto selectedVertices = m_selectionManager->getSelectedVertices();
    if (selectedVertices.empty()) return getSceneCenter();
    glm::vec3 sum(0, 0, 0);
    for (const auto& v : selectedVertices) {
        sum += v->getPosition();
    }
    return sum / float(selectedVertices.size());
}

// Removed duplicate/incorrect signature for calculateFramingDistance
// No Qt math types, only GLM
void CameraController::cameraChanged() {
    // No-op: event/callback system can be implemented if needed
}

float CameraController::calculateFramingDistance(const glm::vec3& sceneSize) const
{
    if (!m_camera) return 10.0f;
    float maxDimension = std::max({sceneSize.x, sceneSize.y, sceneSize.z});
    if (maxDimension < 0.1f) {
        maxDimension = 2.0f;
    }
    float fov = m_camera->getFOV();
    float distance = (maxDimension * 0.5f) / std::tan(glm::radians(fov * 0.5f));
    return std::max(distance * 1.5f, maxDimension * 2.0f);
}

glm::vec3 CameraController::calculateFramingPosition(const glm::vec3& target, const glm::vec3& size) const
{
    float distance = calculateFramingDistance(size);
    glm::vec3 direction;
    if (m_camera) {
        direction = glm::normalize(target - m_camera->getWorldPosition());
        if (glm::length(direction) < 0.1f) {
            direction = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
        }
    } else {
        direction = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
    }
    return target - direction * distance;
}


glm::vec3 CameraController::getOrbitCenter() const
{
    switch (m_orbitMode) {
        case OrbitMode::WorldCenter:
            return glm::vec3(0.0f);
        case OrbitMode::SceneCenter:
            try {
                return getSceneCenter();
            } catch (...) {
                return glm::vec3(0.0f);
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
    glm::vec3 center = getOrbitCenter();
    glm::vec3 cameraPos = m_camera->getWorldPosition();
    m_orbitDistance = glm::length(cameraPos - center);
    m_orbitCenter = center;
}
