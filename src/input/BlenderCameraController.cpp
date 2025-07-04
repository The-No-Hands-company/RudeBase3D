#include "BlenderCameraController.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include <QTimer>
#include <QApplication>
#include <QDebug>
#include <QtMath>

BlenderCameraController::BlenderCameraController(QObject* parent)
    : ICameraController(parent)
    , m_currentMode(NavigationMode::None)
    , m_isNavigating(false)
    , m_shiftPressed(false)
    , m_ctrlPressed(false)
    , m_altPressed(false)
    , m_orbitSensitivity(1.0f)
    , m_panSensitivity(1.0f)
    , m_zoomSensitivity(1.0f)
    , m_invertZoom(false)
    , m_smoothingEnabled(true)
    , m_smoothingTimer(new QTimer(this))
    , m_smoothingFactor(0.15f)
{
    // Set up smoothing timer
    m_smoothingTimer->setInterval(16); // ~60 FPS
    connect(m_smoothingTimer, &QTimer::timeout, this, &BlenderCameraController::updateSmoothing);
}

BlenderCameraController::~BlenderCameraController() = default;

QString BlenderCameraController::getControllerDescription() const
{
    return "Blender Camera Controller";
}

bool BlenderCameraController::handleMousePress(QMouseEvent* event)
{
    if (!m_camera) return false;

    m_lastMousePos = event->pos();
    m_mousePressPos = event->pos();

    // Check for middle mouse button navigation
    if (isMiddleMouseButton(event)) {
        NavigationMode mode = NavigationMode::Orbit;
        
        if (m_shiftPressed) {
            mode = NavigationMode::Pan;
        } else if (m_ctrlPressed) {
            mode = NavigationMode::Zoom;
        }
        
        startNavigation(mode, event->pos());
        return true;
    }

    return false;
}

bool BlenderCameraController::handleMouseMove(QMouseEvent* event)
{
    if (!m_camera || !m_isNavigating) return false;

    updateNavigation(event->pos());
    m_lastMousePos = event->pos();
    return true;
}

bool BlenderCameraController::handleMouseRelease(QMouseEvent* event)
{
    if (!m_camera) return false;

    if (m_isNavigating && isMiddleMouseButton(event)) {
        endNavigation();
        return true;
    }

    return false;
}

bool BlenderCameraController::handleWheel(QWheelEvent* event)
{
    if (!m_camera) return false;

    float delta = event->angleDelta().y() / 120.0f; // Standard wheel delta
    if (m_invertZoom) delta = -delta;
    
    performZoom(delta * m_zoomSensitivity);
    
    return true;
}

bool BlenderCameraController::handleKeyPress(QKeyEvent* event)
{
    // Update modifier key states
    m_shiftPressed = event->modifiers() & Qt::ShiftModifier;
    m_ctrlPressed = event->modifiers() & Qt::ControlModifier;
    m_altPressed = event->modifiers() & Qt::AltModifier;

    // Handle numpad view shortcuts
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        if (event->modifiers() & Qt::KeypadModifier) {
            setNumpadView(event->key());
            return true;
        }
    }

    // Handle other navigation keys
    switch (event->key()) {
        case Qt::Key_Home:
            frameScene(true);
            return true;
        case Qt::Key_Period:
            frameSelection(true);
            return true;
        default:
            break;
    }

    return false;
}

bool BlenderCameraController::handleKeyRelease(QKeyEvent* event)
{
    // Update modifier key states
    m_shiftPressed = event->modifiers() & Qt::ShiftModifier;
    m_ctrlPressed = event->modifiers() & Qt::ControlModifier;
    m_altPressed = event->modifiers() & Qt::AltModifier;

    return false;
}

void BlenderCameraController::frameScene(bool animate)
{
    if (!m_camera || !m_scene) return;

    // Calculate bounding box of all objects in scene
    QVector3D minBounds, maxBounds;
    bool hasObjects = false;
    
    // TODO: Implement scene bounding box calculation
    // For now, use a default framing
    if (!hasObjects) {
        minBounds = QVector3D(-5, -5, -5);
        maxBounds = QVector3D(5, 5, 5);
    }

    QVector3D center = (minBounds + maxBounds) * 0.5f;
    QVector3D size = maxBounds - minBounds;
    float maxDimension = qMax(qMax(size.x(), size.y()), size.z());
    
    // Position camera to view the entire scene
    float distance = maxDimension * 1.5f; // Add some padding
    QVector3D newPosition = center + QVector3D(distance, distance * 0.7f, distance);
    
    if (animate && m_smoothingEnabled) {
        m_targetPosition = newPosition;
        m_smoothingTimer->start();
    } else {
        m_camera->getTransform().setPosition(newPosition);
        m_camera->lookAt(center);
        emit cameraChanged();
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

    QVector3D defaultPosition(10.0f, 8.0f, 10.0f);
    QVector3D target(0, 0, 0);
    
    if (m_smoothingEnabled) {
        m_targetPosition = defaultPosition;
        m_smoothingTimer->start();
    } else {
        m_camera->getTransform().setPosition(defaultPosition);
        m_camera->lookAt(target);
        emit cameraChanged();
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
    if (!enabled) {
        m_smoothingTimer->stop();
    }
}

void BlenderCameraController::updateSmoothing()
{
    if (!m_camera || !m_smoothingEnabled) {
        m_smoothingTimer->stop();
        return;
    }

    QVector3D currentPosition = m_camera->getTransform().getPosition();
    QVector3D deltaPosition = m_targetPosition - currentPosition;
    
    // Check if we're close enough to stop smoothing
    if (deltaPosition.length() < 0.01f) {
        m_camera->getTransform().setPosition(m_targetPosition);
        m_smoothingTimer->stop();
        emit cameraChanged();
        return;
    }

    // Smooth interpolation
    QVector3D newPosition = currentPosition + deltaPosition * m_smoothingFactor;
    m_camera->getTransform().setPosition(newPosition);
    emit cameraChanged();
}

void BlenderCameraController::startNavigation(NavigationMode mode, const QPoint& mousePos)
{
    m_currentMode = mode;
    m_isNavigating = true;
    m_lastMousePos = mousePos;
    m_mousePressPos = mousePos;
    
    // Stop any ongoing smoothing
    if (m_smoothingTimer->isActive()) {
        m_smoothingTimer->stop();
    }
}

void BlenderCameraController::updateNavigation(const QPoint& mousePos)
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
        case NavigationMode::Zoom:
            performZoom(delta.y() * 0.1f); // Convert vertical movement to zoom
            break;
        default:
            break;
    }
}

void BlenderCameraController::endNavigation()
{
    m_currentMode = NavigationMode::None;
    m_isNavigating = false;
}

void BlenderCameraController::performOrbit(const QVector2D& delta)
{
    if (!m_camera) return;

    // Get current camera transform
    Transform& transform = m_camera->getTransform();
    QVector3D position = transform.getPosition();
    QVector3D target = QVector3D(0, 0, 0); // TODO: Get actual orbit target
    
    // Calculate orbit
    QVector3D toCamera = position - target;
    float distance = toCamera.length();
    
    if (distance < 0.01f) return; // Prevent division by zero
    
    // Horizontal rotation (around Y-axis)
    float yawDelta = -delta.x() * m_orbitSensitivity * 0.01f;
    QMatrix4x4 yawRotation;
    yawRotation.rotate(qRadiansToDegrees(yawDelta), QVector3D(0, 1, 0));
    
    // Vertical rotation (around camera's right vector)
    QVector3D right = QVector3D::crossProduct(toCamera.normalized(), QVector3D(0, 1, 0)).normalized();
    float pitchDelta = -delta.y() * m_orbitSensitivity * 0.01f;
    QMatrix4x4 pitchRotation;
    pitchRotation.rotate(qRadiansToDegrees(pitchDelta), right);
    
    // Apply rotations
    QVector3D newToCamera = (yawRotation * pitchRotation).map(toCamera);
    QVector3D newPosition = target + newToCamera;
    
    transform.setPosition(newPosition);
    m_camera->lookAt(target);
    
    emit cameraChanged();
}

void BlenderCameraController::performPan(const QVector2D& delta)
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
    
    transform.setPosition(position + panMovement);
    emit cameraChanged();
}

void BlenderCameraController::performZoom(float delta)
{
    if (!m_camera) return;

    Transform& transform = m_camera->getTransform();
    QVector3D position = transform.getPosition();
    QVector3D forward = transform.getForward();
    
    // Calculate zoom movement
    float zoomSpeed = m_zoomSensitivity * 0.5f;
    QVector3D zoomMovement = forward * delta * zoomSpeed;
    
    transform.setPosition(position + zoomMovement);
    emit cameraChanged();
}

void BlenderCameraController::setNumpadView(int key)
{
    switch (key) {
        case Qt::Key_1:
            setFrontView();
            break;
        case Qt::Key_3:
            setRightView();
            break;
        case Qt::Key_7:
            setTopView();
            break;
        case Qt::Key_9:
            setBackView();
            break;
        // Add more numpad views as needed
        default:
            break;
    }
}

void BlenderCameraController::setFrontView()
{
    if (!m_camera) return;
    
    QVector3D position(0, 0, 10);
    QVector3D target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    emit cameraChanged();
}

void BlenderCameraController::setBackView()
{
    if (!m_camera) return;
    
    QVector3D position(0, 0, -10);
    QVector3D target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    emit cameraChanged();
}

void BlenderCameraController::setRightView()
{
    if (!m_camera) return;
    
    QVector3D position(10, 0, 0);
    QVector3D target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    emit cameraChanged();
}

void BlenderCameraController::setLeftView()
{
    if (!m_camera) return;
    
    QVector3D position(-10, 0, 0);
    QVector3D target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    emit cameraChanged();
}

void BlenderCameraController::setTopView()
{
    if (!m_camera) return;
    
    QVector3D position(0, 10, 0);
    QVector3D target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    emit cameraChanged();
}

void BlenderCameraController::setBottomView()
{
    if (!m_camera) return;
    
    QVector3D position(0, -10, 0);
    QVector3D target(0, 0, 0);
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target);
    emit cameraChanged();
}

void BlenderCameraController::setUserView()
{
    // Default perspective view
    resetCamera();
}

bool BlenderCameraController::isMiddleMouseButton(QMouseEvent* event) const
{
    return event->button() == Qt::MiddleButton;
}

QVector2D BlenderCameraController::getMouseDelta(const QPoint& currentPos) const
{
    QPoint delta = currentPos - m_lastMousePos;
    return QVector2D(delta.x(), delta.y());
}

void BlenderCameraController::updateSmoothCamera()
{
    // This method is called for general smooth camera updates
    // The actual smoothing is handled by updateSmoothing()
    updateSmoothing();
}
