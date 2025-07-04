#include "input/EventDrivenCameraController.hpp"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include <QDebug>
#include <QtMath>

namespace input {

EventDrivenCameraController::EventDrivenCameraController(event::EventDispatcher& dispatcher, QObject* parent)
    : QObject(parent)
    , m_dispatcher(dispatcher)
{
    qDebug() << "[EventDrivenCameraController] Creating event-driven camera controller";
    
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
    
    qDebug() << "[EventDrivenCameraController] Subscribed to mouse events";
}

EventDrivenCameraController::~EventDrivenCameraController()
{
    qDebug() << "[EventDrivenCameraController] Destroying event-driven camera controller";
}

void EventDrivenCameraController::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
    if (m_camera) {
        qDebug() << "[EventDrivenCameraController] Camera set, position:" << m_camera->getWorldPosition();
        updateOrbitPivot();
    } else {
        qDebug() << "[EventDrivenCameraController] Camera cleared";
    }
}

void EventDrivenCameraController::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
    qDebug() << "[EventDrivenCameraController] Scene set";
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
        qDebug() << "[EventDrivenCameraController] Starting Maya orbit mode";
    } else if (isMayaPanAction(mouseEvent)) {
        mode = NavigationMode::Pan;
        qDebug() << "[EventDrivenCameraController] Starting Maya pan mode";
    } else if (isMayaDollyAction(mouseEvent)) {
        mode = NavigationMode::Dolly;
        qDebug() << "[EventDrivenCameraController] Starting Maya dolly mode";
    }
    
    if (mode != NavigationMode::None) {
        QPoint mousePos(mouseEvent.position.x, mouseEvent.position.y);
        startNavigation(mode, mousePos);
        event.handled = true;
    }
}

void EventDrivenCameraController::onMouseMove(event::Event& event)
{
    if (event.handled || !m_isNavigating) return;
    
    auto& mouseEvent = static_cast<event::MouseMoveEvent&>(event);
    
    if (!m_camera) return;
    
    QPoint mousePos(mouseEvent.position.x, mouseEvent.position.y);
    updateNavigation(mousePos);
    
    event.handled = true;
}

void EventDrivenCameraController::onMouseRelease(event::Event& event)
{
    if (event.handled) return;
    
    if (m_isNavigating) {
        qDebug() << "[EventDrivenCameraController] Ending navigation";
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
    
    qDebug() << "[EventDrivenCameraController] Mouse wheel delta:" << delta;
    
    performDolly(delta * m_zoomSensitivity);
    
    event.handled = true;
}

void EventDrivenCameraController::resetCamera()
{
    if (!m_camera) return;
    
    qDebug() << "[EventDrivenCameraController] Resetting camera";
    
    QVector3D defaultPosition(10.0f, 8.0f, 10.0f);
    QVector3D target(0, 0, 0);
    
    m_camera->getTransform().setPosition(defaultPosition);
    m_camera->lookAt(target);
    
    updateOrbitPivot();
    emit cameraChanged();
}

void EventDrivenCameraController::frameScene(bool animate)
{
    if (!m_camera || !m_scene) return;
    
    qDebug() << "[EventDrivenCameraController] Framing scene";
    
    // TODO: Calculate actual scene bounds
    QVector3D minBounds(-5, -5, -5);
    QVector3D maxBounds(5, 5, 5);
    
    QVector3D center = (minBounds + maxBounds) * 0.5f;
    QVector3D size = maxBounds - minBounds;
    float maxDimension = qMax(qMax(size.x(), size.y()), size.z());
    
    float distance = maxDimension * 1.5f;
    QVector3D newPosition = center + QVector3D(distance, distance * 0.7f, distance);
    
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

void EventDrivenCameraController::startNavigation(NavigationMode mode, const QPoint& mousePos)
{
    m_currentMode = mode;
    m_isNavigating = true;
    m_lastMousePos = mousePos;
    m_mousePressPos = mousePos;
    
    qDebug() << "[EventDrivenCameraController] Started navigation mode:" << static_cast<int>(mode);
}

void EventDrivenCameraController::updateNavigation(const QPoint& mousePos)
{
    if (!m_isNavigating || !m_camera) return;
    
    QVector2D delta = getMouseDelta(mousePos);
    
    switch (m_currentMode) {
        case NavigationMode::Orbit:
            performOrbit(delta);
            break;
        case NavigationMode::Pan:
            performPan(delta);
            break;
        case NavigationMode::Dolly:
            performDolly(delta.y() * 0.1f);
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
    
    qDebug() << "[EventDrivenCameraController] Navigation ended";
}

void EventDrivenCameraController::performOrbit(const QVector2D& delta)
{
    if (!m_camera) return;
    
    // Get current camera transform
    Transform& transform = m_camera->getTransform();
    QVector3D position = transform.getPosition();
    
    // Calculate orbit around pivot
    QVector3D toCamera = position - m_orbitPivot;
    float distance = toCamera.length();
    
    if (distance < 0.01f) return;
    
    // Horizontal rotation (yaw around Y-axis)
    float yawDelta = -delta.x() * m_orbitSensitivity * 0.005f;
    QMatrix4x4 yawRotation;
    yawRotation.rotate(qRadiansToDegrees(yawDelta), QVector3D(0, 1, 0));
    
    // Vertical rotation (pitch around camera's right vector)
    QVector3D right = QVector3D::crossProduct(toCamera.normalized(), QVector3D(0, 1, 0)).normalized();
    float pitchDelta = -delta.y() * m_orbitSensitivity * 0.005f;
    QMatrix4x4 pitchRotation;
    pitchRotation.rotate(qRadiansToDegrees(pitchDelta), right);
    
    // Apply rotations
    QVector3D newToCamera = (yawRotation * pitchRotation).map(toCamera);
    QVector3D newPosition = m_orbitPivot + newToCamera;
    
    transform.setPosition(newPosition);
    m_camera->lookAt(m_orbitPivot);
    
    emit cameraChanged();
}

void EventDrivenCameraController::performPan(const QVector2D& delta)
{
    if (!m_camera) return;
    
    Transform& transform = m_camera->getTransform();
    QVector3D position = transform.getPosition();
    QVector3D forward = transform.getForward();
    QVector3D right = transform.getRight();
    QVector3D up = QVector3D::crossProduct(right, forward).normalized();
    
    // Calculate pan movement
    float panSpeed = m_panSensitivity * 0.01f;
    QVector3D panMovement = (-right * delta.x() + up * delta.y()) * panSpeed;
    
    // Move both camera and pivot
    transform.setPosition(position + panMovement);
    m_orbitPivot += panMovement;
    
    emit cameraChanged();
}

void EventDrivenCameraController::performDolly(float delta)
{
    if (!m_camera) return;
    
    Transform& transform = m_camera->getTransform();
    QVector3D position = transform.getPosition();
    
    // Dolly toward/away from pivot
    QVector3D toPivot = (m_orbitPivot - position).normalized();
    float dollySpeed = m_zoomSensitivity * 0.5f;
    QVector3D dollyMovement = toPivot * delta * dollySpeed;
    
    // Prevent getting too close to pivot
    QVector3D newPosition = position + dollyMovement;
    float distanceToPivot = (m_orbitPivot - newPosition).length();
    
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

QVector2D EventDrivenCameraController::getMouseDelta(const QPoint& currentPos) const
{
    QPoint delta = currentPos - m_lastMousePos;
    return QVector2D(delta.x(), delta.y());
}

void EventDrivenCameraController::updateOrbitPivot()
{
    if (!m_camera) return;
    
    QVector3D position = m_camera->getTransform().getPosition();
    QVector3D forward = m_camera->getTransform().getForward();
    
    // Set pivot to a point in front of the camera
    m_orbitDistance = position.length();
    m_orbitPivot = position + forward * qMin(m_orbitDistance, 10.0f);
    
    qDebug() << "[EventDrivenCameraController] Updated orbit pivot to:" << m_orbitPivot;
}

} // namespace input
