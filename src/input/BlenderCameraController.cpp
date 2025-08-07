
#include "InputEvents.h"
#include "BlenderCameraController.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>

BlenderCameraController::BlenderCameraController()
    : ICameraController()
    , m_currentAction(NavigationAction::None)
    , m_isNavigating(false)
    , m_orbitSensitivity(1.0f)
    , m_panSensitivity(1.0f)
    , m_zoomSensitivity(1.0f)
    , m_invertZoom(false)
    , m_smoothingEnabled(true)
    , m_smoothingFactor(0.15f)
{
    // No QTimer, use manual time tracking for smoothing if needed
}

BlenderCameraController::~BlenderCameraController() = default;

std::string BlenderCameraController::getControllerDescription() const
{
    return "Blender Camera Controller";
}

bool BlenderCameraController::handleMousePress(const MouseEvent& event)
{
    if (!m_camera) return false;

    m_lastMousePos = event.pos;
    m_mousePressPos = event.pos;

    // Use unified MouseButton and KeyboardModifier enums
    if (event.button == MouseButton::Middle) {
        NavigationAction action = NavigationAction::Orbit;
        if ((event.modifiers & KeyboardModifier::Shift) != KeyboardModifier::None) {
            action = NavigationAction::Pan;
        } else if ((event.modifiers & KeyboardModifier::Control) != KeyboardModifier::None) {
            action = NavigationAction::Zoom;
        }
        startNavigation(action, event.pos);
        return true;
    }
    return false;
}

bool BlenderCameraController::handleMouseMove(const MouseEvent& event)
{
    if (!m_camera || !m_isNavigating) return false;
    updateNavigation(event.pos);
    m_lastMousePos = event.pos;
    return true;
}

bool BlenderCameraController::handleMouseRelease(const MouseEvent& event)
{
    if (!m_camera) return false;
    if (m_isNavigating && event.button == MouseButton::Middle) {
        endNavigation();
        return true;
    }
    return false;
}

bool BlenderCameraController::handleWheel(const WheelEvent& event)
{
    if (!m_camera) return false;
    float delta = event.delta / 120.0f;
    if (m_invertZoom) delta = -delta;
    performZoom(delta * m_zoomSensitivity);
    return true;
}

bool BlenderCameraController::handleKeyPress(const KeyEvent& event)
{
    // Handle numpad view shortcuts
    if (event.key >= 0 && event.key <= 9) {
        // If you need to check for Keypad, ensure KeyboardModifier has a Keypad value or remove this check
        // if ((event.modifiers & KeyboardModifier::Keypad) != KeyboardModifier::None) {
        setNumpadView(event.key);
        return true;
    }
    // Handle other navigation keys
    switch (event.key) {
        case 1000: // Home
            frameScene(true);
            return true;
        case 1001: // Period
            frameSelection(true);
            return true;
        default:
            break;
    }
    return false;
}

bool BlenderCameraController::handleKeyRelease(const KeyEvent& event)
{
    // No need to track modifier state, handled via event.modifiers
    // Event parameter acknowledged for future key-specific release handling
    (void)event;
    return false;
}
void BlenderCameraController::frameScene(bool animate)
{
    if (!m_camera || !m_scene) return;

    // Calculate bounding box of all objects in scene
    glm::vec3 minBounds(-5, -5, -5), maxBounds(5, 5, 5);
    bool hasObjects = false;
    // TODO: Implement scene bounding box calculation
    // For now, use a default framing
    // hasObjects will be used to determine if scene has content for framing
    (void)hasObjects;

    glm::vec3 center = (minBounds + maxBounds) * 0.5f;
    glm::vec3 size = maxBounds - minBounds;
    float maxDimension = std::max(std::max(size.x, size.y), size.z);

    // Position camera to view the entire scene
    float distance = maxDimension * 1.5f; // Add some padding
    glm::vec3 newPosition = center + glm::vec3(distance, distance * 0.7f, distance);

    if (animate && m_smoothingEnabled) {
        m_targetPosition = newPosition;
        // TODO: Implement cross-platform smoothing timer
    } else {
        m_camera->getTransform().setPosition(newPosition);
        m_camera->lookAt(center);
        if (cameraChangedCallback) cameraChangedCallback();
    }
}

void BlenderCameraController::frameSelection(bool animate)
{
    // TODO: Implement selection-based framing
    // For now, fall back to frame scene
    frameScene(animate);
}

void BlenderCameraController::resetCamera()
{
    if (!m_camera) return;

    glm::vec3 defaultPosition(10.0f, 8.0f, 10.0f);
    glm::vec3 target(0, 0, 0);
    
    if (m_smoothingEnabled) {
        m_targetPosition = defaultPosition;
        // TODO: Implement cross-platform smoothing timer
    } else {
        m_camera->getTransform().setPosition(defaultPosition);
        m_camera->lookAt(target);
        if (cameraChangedCallback) cameraChangedCallback();
    }
}

void BlenderCameraController::setOrbitSensitivity(float sensitivity)
{
    m_orbitSensitivity = sensitivity;
}

void BlenderCameraController::setPanSensitivity(float sensitivity)
{
    m_panSensitivity = sensitivity;
}

void BlenderCameraController::setZoomSensitivity(float sensitivity)
{
    m_zoomSensitivity = sensitivity;
}

void BlenderCameraController::setInvertZoom(bool invert)
{
    m_invertZoom = invert;
}

void BlenderCameraController::setSmoothing(bool enabled)
{
    m_smoothingEnabled = enabled;
    // TODO: Implement cross-platform smoothing timer stop
}

void BlenderCameraController::updateSmoothing()
{
    if (!m_camera || !m_smoothingEnabled) {
        // TODO: Implement cross-platform smoothing timer stop
        return;
    }

    glm::vec3 currentPosition = m_camera->getTransform().getPosition();
    glm::vec3 deltaPosition = m_targetPosition - currentPosition;
    
    // Check if we're close enough to stop smoothing
    if (glm::length(deltaPosition) < 0.01f) {
        m_camera->getTransform().setPosition(m_targetPosition);
        // TODO: Implement cross-platform smoothing timer stop
        if (cameraChangedCallback) cameraChangedCallback();
        return;
    }

    // Smooth interpolation
    glm::vec3 newPosition = currentPosition + deltaPosition * m_smoothingFactor;
    m_camera->getTransform().setPosition(newPosition);
    if (cameraChangedCallback) cameraChangedCallback();
}

void BlenderCameraController::startNavigation(NavigationAction action, const glm::ivec2& mousePos)
{
    m_currentAction = action;
    m_isNavigating = true;
    m_lastMousePos = mousePos;
    m_mousePressPos = mousePos;
    // Stop any ongoing smoothing
    // TODO: Implement cross-platform smoothing timer stop if active
}

void BlenderCameraController::updateNavigation(const glm::ivec2& mousePos)
{
    if (!m_isNavigating || !m_camera) return;

    glm::vec2 delta = getMouseDelta(mousePos);
    switch (m_currentAction) {
        case NavigationAction::Orbit:
            performOrbit(delta);
            break;
        case NavigationAction::Pan:
            performPan(delta);
            break;
        case NavigationAction::Zoom:
            performZoom(delta.y * m_zoomSensitivity);
            break;
        default:
            break;
    }
}

void BlenderCameraController::endNavigation()
{
    m_currentAction = NavigationAction::None;
    m_isNavigating = false;
}

void BlenderCameraController::performOrbit(const glm::vec2& delta)
{
    if (!m_camera) return;

    Transform& transform = m_camera->getTransform();
    glm::vec3 position = transform.getPosition();
    glm::vec3 target(0, 0, 0); // TODO: Get actual orbit target

    glm::vec3 toCamera = position - target;
    float distance = glm::length(toCamera);
    if (distance < 0.01f) return;

    // Horizontal rotation (around Y-axis)
    float yawDelta = -delta.x * m_orbitSensitivity * 0.01f;
    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), yawDelta, glm::vec3(0, 1, 0));

    // Vertical rotation (around camera's right vector)
    glm::vec3 right = glm::normalize(glm::cross(glm::normalize(toCamera), glm::vec3(0, 1, 0)));
    float pitchDelta = -delta.y * m_orbitSensitivity * 0.01f;
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), pitchDelta, right);

    glm::vec3 newToCamera = glm::vec3(yawRotation * pitchRotation * glm::vec4(toCamera, 1.0f));
    glm::vec3 newPosition = target + newToCamera;

    transform.setPosition(newPosition);
    m_camera->lookAt(target);
    if (cameraChangedCallback) cameraChangedCallback();
}


void BlenderCameraController::performPan(const glm::vec2& delta)
{
    if (!m_camera) return;

    Transform& transform = m_camera->getTransform();
    glm::vec3 position = transform.getPosition();
    glm::vec3 forward = transform.getForward();
    glm::vec3 right = transform.getRight();
    glm::vec3 up = glm::normalize(glm::cross(right, forward));

    float panSpeed = m_panSensitivity * 0.01f;
    glm::vec3 panMovement = (-right * delta.x + up * delta.y) * panSpeed;

    transform.setPosition(position + panMovement);
    if (cameraChangedCallback) cameraChangedCallback();
}

void BlenderCameraController::performZoom(float delta)
{
    if (!m_camera) return;

    Transform& transform = m_camera->getTransform();
    glm::vec3 position = transform.getPosition();
    glm::vec3 forward = transform.getForward();
    
    // Calculate zoom movement
    float zoomSpeed = m_zoomSensitivity * 0.5f;
    glm::vec3 zoomMovement = forward * delta * zoomSpeed;
    
    transform.setPosition(position + zoomMovement);
    if (cameraChangedCallback) cameraChangedCallback();
}

void BlenderCameraController::setNumpadView(int key)
{
    // TODO: Implement numpad view switching logic here
    // Example: if (key == 1) setFrontView();
    // Key parameter will be used to determine specific view to set
    (void)key; // Future: switch camera to specific numpad view based on key
}

void BlenderCameraController::setFrontView()
{
    if (!m_camera) return;
    
    glm::vec3 position(0, 0, 10);
    glm::vec3 target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    if (cameraChangedCallback) cameraChangedCallback();
}

void BlenderCameraController::setBackView()
{
    if (!m_camera) return;
    
    glm::vec3 position(0, 0, -10);
    glm::vec3 target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    if (cameraChangedCallback) cameraChangedCallback();
}

void BlenderCameraController::setRightView()
{
    if (!m_camera) return;
    
    glm::vec3 position(10, 0, 0);
    glm::vec3 target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    if (cameraChangedCallback) cameraChangedCallback();
}

void BlenderCameraController::setLeftView()
{
    if (!m_camera) return;
    
    glm::vec3 position(-10, 0, 0);
    glm::vec3 target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    if (cameraChangedCallback) cameraChangedCallback();
}

void BlenderCameraController::setTopView()
{
    if (!m_camera) return;
    
    glm::vec3 position(0, 10, 0);
    glm::vec3 target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    if (cameraChangedCallback) cameraChangedCallback();
}

void BlenderCameraController::setBottomView()
{
    if (!m_camera) return;
    
    glm::vec3 position(0, -10, 0);
    glm::vec3 target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    if (cameraChangedCallback) cameraChangedCallback();
}

void BlenderCameraController::setUserView()
{
    // Default perspective view
    resetCamera();
}

bool BlenderCameraController::isMiddleMouseButton(int button) const
{
    return button == 2;
}


glm::vec2 BlenderCameraController::getMouseDelta(const glm::ivec2& currentPos) const
{
    glm::ivec2 delta = currentPos - m_lastMousePos;
    return glm::vec2(delta.x, delta.y);
}

void BlenderCameraController::updateSmoothCamera()
{
    // This method is called for general smooth camera updates
    // The actual smoothing is handled by updateSmoothing()
    updateSmoothing();
}
