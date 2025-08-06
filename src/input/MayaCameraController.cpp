#include "InputEvents.h"
#include "MayaCameraController.h"
#include "tools/selection/SelectionManager.h"
#include "scene/Camera.h"
#include "core/scene.hpp"
#include "scene/SceneObject.h"

void MayaCameraController::setSelectionManager(std::shared_ptr<SelectionManager> selectionManager)
{
    m_selectionManager = selectionManager;
}

MayaCameraController::MayaCameraController()
    : ICameraController()
    , m_orbitPivot(0.0f, 0.0f, 0.0f)
    , m_orbitDistance(10.0f)
    , m_smartPivot(true)
    , m_currentMode(InteractionMode::None)
    , m_movementSpeed(5.0f)
    , m_rotationSpeed(1.0f)
    , m_panSpeed(0.01f)
    , m_zoomSpeed(0.1f)
    , m_invertY(false)
    , m_isAnimating(false)
    , m_animationTime(0.0f)
    , m_animationDuration(1.0f)
    , m_animationInterval(0.016f) // ~60 FPS
{
}

MayaCameraController::~MayaCameraController() = default;

void MayaCameraController::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
    if (m_camera) {
        updateOrbitDistance();
    }
}

void MayaCameraController::setScene(std::shared_ptr<rude::Scene> scene)
{
    m_scene = scene;
}

std::string MayaCameraController::getControllerDescription() const
{
    return "Maya-style navigation:\n"
           "• Alt + Left Mouse: Tumble (orbit)\n"
           "• Alt + Middle Mouse: Track (pan)\n"
           "• Alt + Right Mouse: Dolly (zoom)\n"
           "• Mouse Wheel: Quick zoom\n"
           "• F: Frame selected/all\n"
           "• Ctrl+Alt+LMB: Rotate around view";
}

bool MayaCameraController::handleMousePress(const MouseEvent& event)
{
    if (!m_camera) return false;
    m_lastMousePos = event.pos;
    m_interactionStartPos = event.pos;
    m_currentModifiers = event.modifiers;
    stopAnimation();
    // Use KeyboardModifier enum for modifiers
    if ((m_currentModifiers & KeyboardModifier::Alt) != KeyboardModifier::None) {
        if (event.button == MouseButton::Left) {
            if ((m_currentModifiers & KeyboardModifier::Control) != KeyboardModifier::None) {
                m_currentMode = InteractionMode::ViewRotate;
            } else {
                m_currentMode = InteractionMode::Tumble;
                tumbleStart(event.pos);
            }
            return true;
        } else if (event.button == MouseButton::Middle) {
            m_currentMode = InteractionMode::Track;
            trackStart(event.pos);
            return true;
        } else if (event.button == MouseButton::Right) {
            m_currentMode = InteractionMode::Dolly;
            dollyStart(event.pos);
            return true;
        }
    }
    return false;
}

bool MayaCameraController::handleMouseMove(const MouseEvent& event)
{
    if (!m_camera || m_currentMode == InteractionMode::None) {
        return false;
    }
    glm::ivec2 delta = event.pos - m_lastMousePos;
    m_lastMousePos = event.pos;
    switch (m_currentMode) {
        case InteractionMode::Tumble:
            tumbleUpdate(event.pos);
            break;
        case InteractionMode::Track:
            trackUpdate(event.pos);
            break;
        case InteractionMode::Dolly:
            dollyUpdate(event.pos);
            break;
        case InteractionMode::ViewRotate:
            orbitAroundPivot(-delta.x * m_rotationSpeed * 0.5f, delta.y * m_rotationSpeed * 0.5f);
            break;
        default:
            return false;
    }
    // cameraChanged(); // Replace with callback if needed
    return true;
}

bool MayaCameraController::handleMouseRelease(const MouseEvent& event)
{
    (void)event;
    if (m_currentMode == InteractionMode::None) {
        return false;
    }
    switch (m_currentMode) {
        case InteractionMode::Tumble:
            tumbleEnd();
            break;
        case InteractionMode::Track:
            trackEnd();
            break;
        case InteractionMode::Dolly:
            dollyEnd();
            break;
        default:
            break;
    }
    m_currentMode = InteractionMode::None;
    return true;
}

bool MayaCameraController::handleWheel(const WheelEvent& event)
{
    if (!m_camera) return false;
    float delta = event.delta;
    dollyCamera(delta * m_zoomSpeed * m_orbitDistance * 0.1f);
    // cameraChanged(); // Replace with callback if needed
    return true;
}

bool MayaCameraController::handleKeyPress(const KeyEvent& event)
{
    m_pressedKeys.insert(event.key);
    // Example: F = 70, Shift = 4 (define as needed)
    constexpr int Key_F = 70;
    if (event.key == Key_F) {
        if ((event.modifiers & KeyboardModifier::Shift) != KeyboardModifier::None) {
            frameSelection(true);
        } else {
            frameScene(true);
        }
        return true;
    }
    // Number keys 1-9 = 49-57 (ASCII)
    if (event.key >= 49 && event.key <= 57) {
        // TODO: Implement bookmark system integration
        return false;
    }
    return false;
}

void MayaCameraController::frameScene(bool animate)
{
    if (!m_camera || !m_scene) return;
    
    glm::vec3 center = calculateSceneCenter();
    float sceneRadius = 10.0f; // TODO: Calculate actual scene radius
    float distance = calculateFramingDistance(center, sceneRadius);
    glm::vec3 currentPos = m_camera->getWorldPosition();
    glm::vec3 toTarget = glm::normalize(center - currentPos);
    glm::vec3 newPosition = center - toTarget * distance;
    setOrbitPivot(center);
    moveToPosition(newPosition, center, animate);
}

void MayaCameraController::frameSelection(bool animate)
{
    if (!m_camera || !m_scene || !m_selectionManager) return;
    // If no selection, frame entire scene
    auto selectedVertices = m_selectionManager->getSelectedVertices();
    if (selectedVertices.empty() || !selectedVertices[0]) {
        frameScene(animate);
        return;
    }
    auto selected = selectedVertices[0];
    glm::vec3 center = selected->getPosition();
    float objectRadius = 5.0f; // TODO: Calculate actual object radius
    float distance = calculateFramingDistance(center, objectRadius);
    glm::vec3 currentPos = m_camera->getWorldPosition();
    glm::vec3 toTarget = glm::normalize(center - currentPos);
    glm::vec3 newPosition = center - toTarget * distance;
    setOrbitPivot(center);
    moveToPosition(newPosition, center, animate);
}

void MayaCameraController::updateAspectRatio(float aspectRatio)
{
    if (m_camera) {
        m_camera->setAspectRatio(aspectRatio);
        if (cameraChangedCallback) cameraChangedCallback();
    }
}

glm::vec3 MayaCameraController::getWorldPosition() const
{
    return m_camera ? m_camera->getWorldPosition() : glm::vec3(0.0f);
}

glm::mat4 MayaCameraController::getViewMatrix() const
{
    return m_camera ? m_camera->getViewMatrix() : glm::mat4(1.0f);
}

glm::mat4 MayaCameraController::getProjectionMatrix() const
{
    return m_camera ? m_camera->getProjectionMatrix() : glm::mat4(1.0f);
}

glm::vec3 MayaCameraController::screenToWorldRay(const glm::vec2& screenPos, const glm::ivec2& viewportSize) const
{
    return m_camera ? m_camera->screenToWorldRay(screenPos, viewportSize) : glm::vec3(0.0f);
}

void MayaCameraController::setOrbitPivot(const glm::vec3& pivot)
{
    if (m_orbitPivot != pivot) {
        m_orbitPivot = pivot;
        updateOrbitDistance();
        // TODO: Replace Qt signal with cross-platform callback if needed
    }
}

void MayaCameraController::tumbleStart(const glm::ivec2& startPos)
{
    m_lastMousePos = startPos;
    if (m_smartPivot) {
        updateSmartPivot(m_lastMousePos);
    }
}

void MayaCameraController::tumbleUpdate(const glm::ivec2& currentPos)
{
    glm::ivec2 delta = currentPos - m_lastMousePos;
    orbitAroundPivot(-delta.x * m_rotationSpeed * 0.5f, delta.y * m_rotationSpeed * 0.5f);
}

void MayaCameraController::tumbleEnd()
{
    // Nothing special needed for tumble end
}

void MayaCameraController::trackStart(const glm::ivec2& startPos)
{
    m_lastMousePos = startPos;
}

void MayaCameraController::trackUpdate(const glm::ivec2& currentPos)
{
    glm::ivec2 delta = currentPos - m_lastMousePos;
    glm::vec3 right = m_camera->getRight();
    glm::vec3 up = m_camera->getUp();
    float panScale = m_panSpeed * m_orbitDistance * 0.001f;
    glm::vec3 worldDelta = right * (-delta.x * panScale) + up * (delta.y * panScale);
    panCamera(worldDelta);
}

void MayaCameraController::trackEnd()
{
    // Nothing special needed for track end
}

void MayaCameraController::dollyStart(const glm::ivec2& startPos)
{
    m_lastMousePos = startPos;
}

void MayaCameraController::dollyUpdate(const glm::ivec2& currentPos)
{
    glm::ivec2 delta = currentPos - m_lastMousePos;
    float dollyDelta = delta.y * m_zoomSpeed * m_orbitDistance * 0.01f;
    dollyCamera(dollyDelta);
}

void MayaCameraController::dollyEnd()
{
    // Nothing special needed for dolly end
}

void MayaCameraController::updateAnimation()
{
    if (!m_isAnimating || !m_camera) {
        return;
    }
    m_animationTime += m_animationInterval;
    float t = glm::min(1.0f, m_animationTime / m_animationDuration);
    t = t * t * (3.0f - 2.0f * t);
    glm::vec3 currentPos = m_animStartPos + (m_animTargetPos - m_animStartPos) * t;
    glm::vec3 currentPivot = m_animStartPivot + (m_animTargetPivot - m_animStartPivot) * t;
    m_camera->getTransform().setPosition(currentPos);
    m_camera->lookAt(currentPivot);
    m_orbitPivot = currentPivot;
    updateOrbitDistance();
    if (t >= 1.0f) {
        stopAnimation();
    }
}

bool MayaCameraController::isAltPressed() const
{
    return (m_currentModifiers & KeyboardModifier::Alt) != KeyboardModifier::None;
}

bool MayaCameraController::isCtrlPressed() const
{
    return (m_currentModifiers & KeyboardModifier::Control) != KeyboardModifier::None;
}

bool MayaCameraController::isShiftPressed() const
{
    return (m_currentModifiers & KeyboardModifier::Shift) != KeyboardModifier::None;
}

void MayaCameraController::updateOrbitDistance()
{
    if (!m_camera) return;
    glm::vec3 cameraPos = m_camera->getWorldPosition();
    m_orbitDistance = glm::length(cameraPos - m_orbitPivot);
}

void MayaCameraController::updateSmartPivot(const glm::ivec2& screenPos)
{
    // TODO: Implement ray casting to find intersection with scene geometry
    // For now, keep current pivot
    (void)screenPos;
}

glm::vec3 MayaCameraController::calculateSceneCenter() const
{
    if (!m_scene) return glm::vec3(0, 0, 0);
    
    // TODO: Calculate actual scene bounding box center
    return glm::vec3(0, 0, 0);
}

glm::vec3 MayaCameraController::calculateSelectionCenter() const
{
    if (!m_selectionManager) return glm::vec3(0, 0, 0);
    auto selectedVertices = m_selectionManager->getSelectedVertices();
    if (selectedVertices.empty() || !selectedVertices[0]) return calculateSceneCenter();
    auto selected = selectedVertices[0];
    // TODO: Calculate actual vertex center
    return selected->getPosition();
}

float MayaCameraController::calculateFramingDistance(const glm::vec3& target, float objectSize) const
{
    if (!m_camera) return 10.0f;
    float fov = glm::radians(m_camera->getFOV());
    float distance = objectSize / (2.0f * tan(fov * 0.5f));
    // Add some margin
    return distance * 1.2f;
}

void MayaCameraController::startAnimation(const glm::vec3& targetPos, const glm::vec3& targetPivot)
{
    if (!m_camera) return;
    m_animStartPos = m_camera->getWorldPosition();
    m_animTargetPos = targetPos;
    m_animStartPivot = m_orbitPivot;
    m_animTargetPivot = targetPivot;
    m_animationTime = 0.0f;
    m_isAnimating = true;
    // In a real app, call updateAnimation() on a timer or in the main loop
}

void MayaCameraController::stopAnimation()
{
    m_isAnimating = false;
}

void MayaCameraController::orbitAroundPivot(float deltaYaw, float deltaPitch)
{
    if (!m_camera) return;
    // Apply Y inversion if enabled
    if (m_invertY) {
        deltaPitch = -deltaPitch;
    }
    glm::vec3 currentPos = m_camera->getWorldPosition();
    glm::vec3 offset = currentPos - m_orbitPivot;
    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), deltaYaw, glm::vec3(0, 1, 0));
    glm::vec3 rightVector = m_camera->getRight();
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), deltaPitch, rightVector);
    glm::vec4 newOffset4 = pitchRotation * yawRotation * glm::vec4(offset, 1.0f);
    glm::vec3 newOffset = glm::vec3(newOffset4);
    glm::vec3 newPos = m_orbitPivot + newOffset;
    glm::vec3 toCenter = glm::normalize(m_orbitPivot - newPos);
    float dot = glm::dot(toCenter, glm::vec3(0, 1, 0));
    if (dot > 0.95f || dot < -0.95f) {
        // Skip this movement due to gimbal lock protection
        return;
    }
    m_camera->getTransform().setPosition(newPos);
    m_camera->lookAt(m_orbitPivot);
    updateOrbitDistance();
}

void MayaCameraController::panCamera(const glm::vec3& worldDelta)
{
    if (!m_camera) return;
    glm::vec3 currentPos = m_camera->getWorldPosition();
    glm::vec3 newPos = currentPos + worldDelta;
    glm::vec3 newPivot = m_orbitPivot + worldDelta;
    m_camera->getTransform().setPosition(newPos);
    m_orbitPivot = newPivot;
    m_camera->lookAt(m_orbitPivot);
    updateOrbitDistance();
}

void MayaCameraController::dollyCamera(float deltaDistance)
{
    if (!m_camera) return;
    glm::vec3 currentPos = m_camera->getWorldPosition();
    glm::vec3 toPivot = glm::normalize(m_orbitPivot - currentPos);
    glm::vec3 newPos = currentPos + toPivot * deltaDistance;
    float newDistance = glm::length(newPos - m_orbitPivot);
    if (newDistance < 0.1f) {
        newDistance = 0.1f;
        newPos = m_orbitPivot - toPivot * newDistance;
    }
    m_camera->getTransform().setPosition(newPos);
    m_camera->lookAt(m_orbitPivot);
    updateOrbitDistance();
}

void MayaCameraController::moveToPosition(const glm::vec3& position, const glm::vec3& target, bool animate)
{
    if (!m_camera) return;
    if (animate) {
        startAnimation(position, target);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(target);
        setOrbitPivot(target);
        // TODO: Replace Qt signal with cross-platform callback if needed
    }
}

bool MayaCameraController::handleKeyRelease(const KeyEvent& event)
{
    m_pressedKeys.erase(event.key);
    return false; // Base implementation doesn't handle key releases
}

void MayaCameraController::resetCamera()
{
    if (!m_camera) return;
    
    // Reset to default position and orientation
    m_camera->getTransform().setPosition(glm::vec3(7.0f, 5.0f, 7.0f));
    m_camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    setOrbitPivot(glm::vec3(0.0f, 0.0f, 0.0f));
    m_orbitDistance = 10.0f;
    
    // Reset any ongoing interactions
    m_currentMode = InteractionMode::None;
    stopAnimation();
}
