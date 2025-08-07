#include "input/EventDrivenCameraController.hpp"
#include "scene/Camera.h"
#include "core/scene.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "InputEvents.h"
#include <memory>

namespace input {

EventDrivenCameraController::EventDrivenCameraController(event::EventDispatcher& dispatcher, QObject* parent)
    : QObject(parent)
    , m_dispatcher(dispatcher)
{
    // Subscribe to mouse events with high priority for camera control
    m_dispatcher.subscribe<event::MousePressEvent>(
        [this](event::Event& e) { onMousePress(e); },
        event::Priority::High
    );
    m_dispatcher.subscribe<event::MouseMoveEvent>(
        [this](event::Event& e) { onMouseMove(e); },
        event::Priority::High
    );
    m_dispatcher.subscribe<event::MouseReleaseEvent>(
        [this](event::Event& e) { onMouseRelease(e); },
        event::Priority::High
    );
    m_dispatcher.subscribe<event::MouseWheelEvent>(
        [this](event::Event& e) { onMouseWheel(e); },
        event::Priority::High
    );
}

EventDrivenCameraController::~EventDrivenCameraController()
{
}

void EventDrivenCameraController::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
    if (m_camera) {
        updateOrbitPivot();
    }
}

void EventDrivenCameraController::setScene(std::shared_ptr<rude::Scene> scene)
{
    m_scene = scene;

}

void EventDrivenCameraController::onMousePress(event::Event& event)
{
    if (event.handled) return;
    
    auto& mouseEvent = static_cast<event::MousePressEvent&>(event);
    
    qDebug() << "[EventDrivenCameraController] Mouse press:"
             << "LMB:" << mouseEvent.isLeftButton
             << "MMB:" << mouseEvent.isMiddleButton  
             << "RMB:" << mouseEvent.isRightButton
             << "Alt:" << mouseEvent.isAltPressed
             << "Ctrl:" << mouseEvent.isCtrlPressed
             << "Shift:" << mouseEvent.isShiftPressed;
    
    if (!m_camera) return;
    
    NavigationMode mode = NavigationMode::None;
    
    if (isMayaOrbitAction(mouseEvent)) {
        mode = NavigationMode::Orbit;
    } else if (isMayaPanAction(mouseEvent)) {
        mode = NavigationMode::Pan;
    } else if (isMayaDollyAction(mouseEvent)) {
        mode = NavigationMode::Dolly;
    }
    
    if (mode != NavigationMode::None) {
        // Convert Qt point to GLM ivec2
        glm::ivec2 mousePos(static_cast<int>(mouseEvent.position.x), static_cast<int>(mouseEvent.position.y));
        startNavigation(mode, mousePos);
        event.handled = true;
    }
}

void EventDrivenCameraController::onMouseMove(event::Event& event)
{
    if (event.handled || !m_isNavigating) return;
    
    auto& mouseEvent = static_cast<event::MouseMoveEvent&>(event);
    
    if (!m_camera) return;
    
    glm::ivec2 mousePos(static_cast<int>(mouseEvent.position.x), static_cast<int>(mouseEvent.position.y));
    updateNavigation(mousePos);
    event.handled = true;
}

void EventDrivenCameraController::onMouseRelease(event::Event& event)
{
    if (event.handled) return;
    
    if (m_isNavigating) {
        endNavigation();
        event.handled = true;
    }
}

void EventDrivenCameraController::onMouseWheel(event::Event& event)
{
    if (event.handled) return;
    
    auto& wheelEvent = static_cast<event::MouseWheelEvent&>(event);
    
    if (!m_camera) return;
    
    float delta = wheelEvent.delta;
    if (m_invertZoom) delta = -delta;
    

    
    performDolly(delta * m_zoomSensitivity);
    
    event.handled = true;
}

void EventDrivenCameraController::resetCamera()
{
    if (!m_camera) return;
    

    

    glm::vec3 defaultPosition(10.0f, 8.0f, 10.0f);
    glm::vec3 target(0, 0, 0);
    m_camera->getTransform().setPosition(defaultPosition);
    m_camera->lookAt(target);
    updateOrbitPivot();
    emit cameraChanged();
}

void EventDrivenCameraController::frameScene(bool animate)
{
    if (!m_camera || !m_scene) return;
    
    // animate parameter will be used for smooth camera transitions
    (void)animate; // Future: enable smooth animated framing vs instant
    
    // TODO: Calculate actual scene bounds

    glm::vec3 minBounds(-5, -5, -5);
    glm::vec3 maxBounds(5, 5, 5);
    glm::vec3 center = (minBounds + maxBounds) * 0.5f;
    glm::vec3 size = maxBounds - minBounds;
    float maxDimension = std::max(std::max(size.x, size.y), size.z);
    float distance = maxDimension * 1.5f;
    glm::vec3 newPosition = center + glm::vec3(distance, distance * 0.7f, distance);
    m_camera->getTransform().setPosition(newPosition);
    m_camera->lookAt(center);
    m_orbitPivot = center;
    updateOrbitPivot();
    emit cameraChanged();
}

void EventDrivenCameraController::frameSelection(bool animate)
{
    // TODO: Implement selection-based framing
    frameScene(animate);
}

void EventDrivenCameraController::startNavigation(NavigationMode mode, const glm::ivec2& mousePos)
{
    m_currentMode = mode;
    m_isNavigating = true;
    m_lastMousePos = mousePos;
    m_mousePressPos = mousePos;
}

void EventDrivenCameraController::updateNavigation(const glm::ivec2& mousePos)
{
    if (!m_isNavigating || !m_camera) return;

    glm::vec2 delta = getMouseDelta(mousePos);
    switch (m_currentMode) {
        case NavigationMode::Orbit:
            performOrbit(delta);
            break;
        case NavigationMode::Pan:
            performPan(delta);
            break;
        case NavigationMode::Dolly:
            performDolly(delta.y * 0.1f);
            break;
        default:
            break;
    }
    m_lastMousePos = mousePos;
}

void EventDrivenCameraController::endNavigation()
{
    m_currentMode = NavigationMode::None;
    m_isNavigating = false;
}


void EventDrivenCameraController::performOrbit(const glm::vec2& delta)
{
    if (!m_camera) return;
    Transform& transform = m_camera->getTransform();
    glm::vec3 position = transform.getPosition();
    glm::vec3 toCamera = position - m_orbitPivot;
    float distance = glm::length(toCamera);
    if (distance < 0.01f) return;
    float yawDelta = -delta.x * m_orbitSensitivity * 0.005f;
    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), yawDelta, glm::vec3(0, 1, 0));
    glm::vec3 right = glm::normalize(glm::cross(glm::normalize(toCamera), glm::vec3(0, 1, 0)));
    float pitchDelta = -delta.y * m_orbitSensitivity * 0.005f;
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), pitchDelta, right);
    glm::vec4 toCamera4(toCamera, 1.0f);
    glm::vec3 newToCamera = glm::vec3(yawRotation * pitchRotation * toCamera4);
    glm::vec3 newPosition = m_orbitPivot + newToCamera;
    transform.setPosition(newPosition);
    m_camera->lookAt(m_orbitPivot);
    emit cameraChanged();
}


void EventDrivenCameraController::performPan(const glm::vec2& delta)
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
    m_orbitPivot += panMovement;
    emit cameraChanged();
}


void EventDrivenCameraController::performDolly(float delta)
{
    if (!m_camera) return;
    Transform& transform = m_camera->getTransform();
    glm::vec3 position = transform.getPosition();
    glm::vec3 toPivot = glm::normalize(m_orbitPivot - position);
    float dollySpeed = m_zoomSensitivity * 0.5f;
    glm::vec3 dollyMovement = toPivot * delta * dollySpeed;
    glm::vec3 newPosition = position + dollyMovement;
    float distanceToPivot = glm::length(m_orbitPivot - newPosition);
    if (distanceToPivot > 0.1f) {
        transform.setPosition(newPosition);
        emit cameraChanged();
    }
}

bool EventDrivenCameraController::isMayaOrbitAction(const event::MouseEvent& mouseEvent) const
{
    return mouseEvent.isAltPressed && mouseEvent.isLeftButton;
}

bool EventDrivenCameraController::isMayaPanAction(const event::MouseEvent& mouseEvent) const
{
    return mouseEvent.isAltPressed && mouseEvent.isMiddleButton;
}

bool EventDrivenCameraController::isMayaDollyAction(const event::MouseEvent& mouseEvent) const
{
    return mouseEvent.isAltPressed && mouseEvent.isRightButton;
}


glm::vec2 EventDrivenCameraController::getMouseDelta(const glm::ivec2& currentPos) const
{
    glm::ivec2 delta = currentPos - m_lastMousePos;
    return glm::vec2(static_cast<float>(delta.x), static_cast<float>(delta.y));
}

void EventDrivenCameraController::updateOrbitPivot()
{
    if (!m_camera) return;
    

    glm::vec3 position = m_camera->getTransform().getPosition();
    glm::vec3 forward = m_camera->getTransform().getForward();
    m_orbitDistance = glm::length(position);
    m_orbitPivot = position + forward * std::min(m_orbitDistance, 10.0f);
}

} // namespace input
