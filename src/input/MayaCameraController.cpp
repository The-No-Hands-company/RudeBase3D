#include "MayaCameraController.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "scene/SceneObject.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QtMath>

MayaCameraController::MayaCameraController(QObject* parent)
    : ICameraController(parent)
    , m_orbitPivot(0.0f, 0.0f, 0.0f)
    , m_orbitDistance(10.0f)
    , m_smartPivot(true)
    , m_currentMode(InteractionMode::None)
    , m_movementSpeed(5.0f)
    , m_rotationSpeed(1.0f)
    , m_panSpeed(0.01f)
    , m_zoomSpeed(0.1f)
    , m_invertY(false)
    , m_animationTimer(new QTimer(this))
    , m_isAnimating(false)
    , m_animationTime(0.0f)
    , m_animationDuration(1.0f)
{
    m_animationTimer->setInterval(16); // ~60 FPS
    connect(m_animationTimer, &QTimer::timeout, this, &MayaCameraController::updateAnimation);
}

MayaCameraController::~MayaCameraController() = default;

void MayaCameraController::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
    if (m_camera) {
        qDebug() << "MayaCameraController::setCamera - Camera set successfully";
        qDebug() << "  Current camera position:" << m_camera->getWorldPosition();
        updateOrbitDistance();
        qDebug() << "  Orbit distance:" << m_orbitDistance;
    } else {
        qDebug() << "MayaCameraController::setCamera - Camera is null!";
    }
}

void MayaCameraController::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
}

QString MayaCameraController::getControllerDescription() const
{
    return "Maya-style navigation:\n"
           "• Alt + Left Mouse: Tumble (orbit)\n"
           "• Alt + Middle Mouse: Track (pan)\n"
           "• Alt + Right Mouse: Dolly (zoom)\n"
           "• Mouse Wheel: Quick zoom\n"
           "• F: Frame selected/all\n"
           "• Ctrl+Alt+LMB: Rotate around view";
}

bool MayaCameraController::handleMousePress(QMouseEvent* event)
{
    qDebug() << "MayaCameraController::handleMousePress - Button:" << event->button() << "Modifiers:" << event->modifiers();
    
    if (!m_camera) return false;
    
    m_lastMousePos = event->pos();
    m_interactionStartPos = event->pos();
    m_currentModifiers = event->modifiers();
    
    // Stop any ongoing animation
    stopAnimation();
    
    // Determine interaction mode based on modifiers and button
    if (m_currentModifiers & Qt::AltModifier) {
        qDebug() << "Alt modifier detected";
        if (event->button() == Qt::LeftButton) {
            if (m_currentModifiers & Qt::ControlModifier) {
                m_currentMode = InteractionMode::ViewRotate;
            } else {
                qDebug() << "Starting tumble mode";
                m_currentMode = InteractionMode::Tumble;
                tumbleStart(event->pos());
            }
            return true;
        } else if (event->button() == Qt::MiddleButton) {
            qDebug() << "Starting track mode";
            m_currentMode = InteractionMode::Track;
            trackStart(event->pos());
            return true;
        } else if (event->button() == Qt::RightButton) {
            m_currentMode = InteractionMode::Dolly;
            dollyStart(event->pos());
            return true;
        }
    }
    
    return false;
}

bool MayaCameraController::handleMouseMove(QMouseEvent* event)
{
    if (!m_camera || m_currentMode == InteractionMode::None) {
        return false;
    }
    
    QPoint delta = event->pos() - m_lastMousePos;
    m_lastMousePos = event->pos();
    
    switch (m_currentMode) {
        case InteractionMode::Tumble:
            tumbleUpdate(event->pos());
            break;
        case InteractionMode::Track:
            trackUpdate(event->pos());
            break;
        case InteractionMode::Dolly:
            dollyUpdate(event->pos());
            break;
        case InteractionMode::ViewRotate:
            // Rotate around view center instead of pivot
            orbitAroundPivot(-delta.x() * m_rotationSpeed * 0.5f, 
                           delta.y() * m_rotationSpeed * 0.5f);
            break;
        default:
            return false;
    }
    
    emit cameraChanged();
    return true;
}

bool MayaCameraController::handleMouseRelease(QMouseEvent* event)
{
    Q_UNUSED(event)
    
    if (m_currentMode == InteractionMode::None) {
        return false;
    }
    
    // End the current interaction
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

bool MayaCameraController::handleWheel(QWheelEvent* event)
{
    if (!m_camera) return false;
    
    float delta = event->angleDelta().y() / 120.0f;
    dollyCamera(delta * m_zoomSpeed * m_orbitDistance * 0.1f);
    
    emit cameraChanged();
    return true;
}

bool MayaCameraController::handleKeyPress(QKeyEvent* event)
{
    m_pressedKeys.insert(event->key());
    
    // Handle frame operations
    if (event->key() == Qt::Key_F) {
        if (event->modifiers() & Qt::ShiftModifier) {
            frameSelection(true);
        } else {
            frameScene(true);
        }
        return true;
    }
    
    // Handle number keys for camera bookmarks (if we had a bookmark system)
    if (event->key() >= Qt::Key_1 && event->key() <= Qt::Key_9) {
        // TODO: Implement bookmark system integration
        return false;
    }
    
    return false;
}

bool MayaCameraController::handleKeyRelease(QKeyEvent* event)
{
    m_pressedKeys.remove(event->key());
    return false;
}

void MayaCameraController::resetCamera()
{
    if (!m_camera) {
        qDebug() << "MayaCameraController::resetCamera() - No camera available!";
        return;
    }
    
    qDebug() << "MayaCameraController::resetCamera() - Resetting to professional view";
    
    // Set to professional isometric-style view (like Maya's default persp view)
    QVector3D target = m_smartPivot ? calculateSceneCenter() : QVector3D(0, 0, 0);
    QVector3D position = target + QVector3D(10.0f, 8.0f, 10.0f); // Better viewing angle
    
    qDebug() << "Camera reset - Position:" << position << "Target:" << target;
    qDebug() << "  Before reset - Camera position:" << m_camera->getWorldPosition();
    
    moveToPosition(position, target, false); // Don't animate on startup
    
    qDebug() << "  After reset - Camera position:" << m_camera->getWorldPosition();
}

void MayaCameraController::frameScene(bool animate)
{
    if (!m_camera || !m_scene) return;
    
    QVector3D center = calculateSceneCenter();
    float sceneRadius = 10.0f; // TODO: Calculate actual scene radius
    float distance = calculateFramingDistance(center, sceneRadius);
    
    QVector3D currentPos = m_camera->getWorldPosition();
    QVector3D toTarget = (center - currentPos).normalized();
    QVector3D newPosition = center - toTarget * distance;
    
    setOrbitPivot(center);
    moveToPosition(newPosition, center, animate);
}

void MayaCameraController::frameSelection(bool animate)
{
    if (!m_camera || !m_scene) return;
    
    // If no selection, frame entire scene
    auto selected = m_scene->getSelectedObject();
    if (!selected) {
        frameScene(animate);
        return;
    }
    
    QVector3D center = calculateSelectionCenter();
    float objectRadius = 5.0f; // TODO: Calculate actual object radius
    float distance = calculateFramingDistance(center, objectRadius);
    
    QVector3D currentPos = m_camera->getWorldPosition();
    QVector3D toTarget = (center - currentPos).normalized();
    QVector3D newPosition = center - toTarget * distance;
    
    setOrbitPivot(center);
    moveToPosition(newPosition, center, animate);
}

void MayaCameraController::updateAspectRatio(float aspectRatio)
{
    if (m_camera) {
        m_camera->setAspectRatio(aspectRatio);
        emit cameraChanged();
    }
}

QVector3D MayaCameraController::getWorldPosition() const
{
    return m_camera ? m_camera->getWorldPosition() : QVector3D();
}

QMatrix4x4 MayaCameraController::getViewMatrix() const
{
    return m_camera ? m_camera->getViewMatrix() : QMatrix4x4();
}

QMatrix4x4 MayaCameraController::getProjectionMatrix() const
{
    return m_camera ? m_camera->getProjectionMatrix() : QMatrix4x4();
}

QVector3D MayaCameraController::screenToWorldRay(const QVector2D& screenPos, const QSize& viewportSize) const
{
    return m_camera ? m_camera->screenToWorldRay(screenPos, viewportSize) : QVector3D();
}

void MayaCameraController::setOrbitPivot(const QVector3D& pivot)
{
    if (m_orbitPivot != pivot) {
        m_orbitPivot = pivot;
        updateOrbitDistance();
        emit pivotChanged(pivot);
    }
}

void MayaCameraController::tumbleStart(const QPoint& startPos)
{
    if (m_smartPivot) {
        updateSmartPivot(startPos);
    }
}

void MayaCameraController::tumbleUpdate(const QPoint& currentPos)
{
    QPoint delta = currentPos - m_lastMousePos;
    orbitAroundPivot(-delta.x() * m_rotationSpeed * 0.5f, 
                    delta.y() * m_rotationSpeed * 0.5f);
}

void MayaCameraController::tumbleEnd()
{
    // Nothing special needed for tumble end
}

void MayaCameraController::trackStart(const QPoint& startPos)
{
    Q_UNUSED(startPos)
    // Nothing special needed for track start
}

void MayaCameraController::trackUpdate(const QPoint& currentPos)
{
    QPoint delta = currentPos - m_lastMousePos;
    
    // Calculate world-space pan delta
    QVector3D right = m_camera->getRight();
    QVector3D up = m_camera->getUp();
    
    float panScale = m_panSpeed * m_orbitDistance * 0.001f;
    QVector3D worldDelta = right * (-delta.x() * panScale) + up * (delta.y() * panScale);
    
    panCamera(worldDelta);
}

void MayaCameraController::trackEnd()
{
    // Nothing special needed for track end
}

void MayaCameraController::dollyStart(const QPoint& startPos)
{
    Q_UNUSED(startPos)
    // Nothing special needed for dolly start
}

void MayaCameraController::dollyUpdate(const QPoint& currentPos)
{
    QPoint delta = currentPos - m_lastMousePos;
    float dollyDelta = delta.y() * m_zoomSpeed * m_orbitDistance * 0.01f;
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
    
    m_animationTime += 0.016f; // Assume 60 FPS
    float t = qMin(1.0f, m_animationTime / m_animationDuration);
    
    // Smooth easing
    t = t * t * (3.0f - 2.0f * t);
    
    QVector3D currentPos = m_animStartPos + (m_animTargetPos - m_animStartPos) * t;
    QVector3D currentPivot = m_animStartPivot + (m_animTargetPivot - m_animStartPivot) * t;
    
    m_camera->getTransform().setPosition(currentPos);
    m_camera->lookAt(currentPivot);
    m_orbitPivot = currentPivot;
    updateOrbitDistance();
    
    emit cameraChanged();
    
    if (t >= 1.0f) {
        stopAnimation();
    }
}

bool MayaCameraController::isAltPressed() const
{
    return m_currentModifiers & Qt::AltModifier;
}

bool MayaCameraController::isCtrlPressed() const
{
    return m_currentModifiers & Qt::ControlModifier;
}

bool MayaCameraController::isShiftPressed() const
{
    return m_currentModifiers & Qt::ShiftModifier;
}

void MayaCameraController::updateOrbitDistance()
{
    if (!m_camera) return;
    
    QVector3D cameraPos = m_camera->getWorldPosition();
    m_orbitDistance = (cameraPos - m_orbitPivot).length();
}

void MayaCameraController::updateSmartPivot(const QPoint& screenPos)
{
    // TODO: Implement ray casting to find intersection with scene geometry
    // For now, keep current pivot
    Q_UNUSED(screenPos)
}

QVector3D MayaCameraController::calculateSceneCenter() const
{
    if (!m_scene) return QVector3D(0, 0, 0);
    
    // TODO: Calculate actual scene bounding box center
    return QVector3D(0, 0, 0);
}

QVector3D MayaCameraController::calculateSelectionCenter() const
{
    if (!m_scene) return QVector3D(0, 0, 0);
    
    auto selected = m_scene->getSelectedObject();
    if (!selected) return calculateSceneCenter();
    
    // TODO: Calculate actual object center
    return selected->getTransform().getPosition();
}

float MayaCameraController::calculateFramingDistance(const QVector3D& target, float objectSize) const
{
    if (!m_camera) return 10.0f;
    
    float fov = qDegreesToRadians(m_camera->getFOV());
    float distance = objectSize / (2.0f * qTan(fov * 0.5f));
    
    // Add some margin
    return distance * 1.2f;
}

void MayaCameraController::startAnimation(const QVector3D& targetPos, const QVector3D& targetPivot)
{
    if (!m_camera) return;
    
    m_animStartPos = m_camera->getWorldPosition();
    m_animTargetPos = targetPos;
    m_animStartPivot = m_orbitPivot;
    m_animTargetPivot = targetPivot;
    m_animationTime = 0.0f;
    m_isAnimating = true;
    
    m_animationTimer->start();
}

void MayaCameraController::stopAnimation()
{
    m_isAnimating = false;
    m_animationTimer->stop();
}

void MayaCameraController::orbitAroundPivot(float deltaYaw, float deltaPitch)
{
    if (!m_camera) return;
    
    qDebug() << "MayaCameraController::orbitAroundPivot - deltaYaw:" << deltaYaw << "deltaPitch:" << deltaPitch;
    
    // Apply Y inversion if enabled
    if (m_invertY) {
        deltaPitch = -deltaPitch;
    }
    
    QVector3D currentPos = m_camera->getWorldPosition();
    QVector3D offset = currentPos - m_orbitPivot;
    
    qDebug() << "  Current position:" << currentPos << "Pivot:" << m_orbitPivot << "Offset:" << offset;
    
    // Create rotation matrices
    QMatrix4x4 yawRotation;
    yawRotation.rotate(deltaYaw, QVector3D(0, 1, 0));
    
    QVector3D rightVector = m_camera->getRight();
    QMatrix4x4 pitchRotation;
    pitchRotation.rotate(deltaPitch, rightVector);
    
    // Apply rotations
    QVector4D newOffset4 = pitchRotation * yawRotation * QVector4D(offset, 1.0f);
    QVector3D newOffset = newOffset4.toVector3D();
    
    // Prevent gimbal lock by limiting pitch
    QVector3D newPos = m_orbitPivot + newOffset;
    QVector3D toCenter = (m_orbitPivot - newPos).normalized();
    float dot = QVector3D::dotProduct(toCenter, QVector3D(0, 1, 0));
    
    if (dot > 0.95f || dot < -0.95f) {
        qDebug() << "  Skipping movement due to gimbal lock protection";
        return; // Skip this movement
    }
    
    // Update camera
    qDebug() << "  New position:" << newPos;
    m_camera->getTransform().setPosition(newPos);
    m_camera->lookAt(m_orbitPivot);
    
    updateOrbitDistance();
    
    qDebug() << "  After update - Camera position:" << m_camera->getWorldPosition();
}

void MayaCameraController::panCamera(const QVector3D& worldDelta)
{
    if (!m_camera) return;
    
    qDebug() << "MayaCameraController::panCamera - worldDelta:" << worldDelta;
    
    // Move both camera and pivot
    QVector3D currentPos = m_camera->getWorldPosition();
    QVector3D newPos = currentPos + worldDelta;
    QVector3D newPivot = m_orbitPivot + worldDelta;
    
    qDebug() << "  Current position:" << currentPos << "New position:" << newPos;
    qDebug() << "  Current pivot:" << m_orbitPivot << "New pivot:" << newPivot;
    
    m_camera->getTransform().setPosition(newPos);
    m_orbitPivot = newPivot;
    
    // Update camera orientation to maintain look direction
    m_camera->lookAt(m_orbitPivot);
    
    qDebug() << "  After update - Camera position:" << m_camera->getWorldPosition();
}

void MayaCameraController::dollyCamera(float deltaDistance)
{
    if (!m_camera) return;
    
    QVector3D currentPos = m_camera->getWorldPosition();
    QVector3D toPivot = (m_orbitPivot - currentPos).normalized();
    
    QVector3D newPos = currentPos + toPivot * deltaDistance;
    
    // Prevent camera from passing through pivot
    float newDistance = (newPos - m_orbitPivot).length();
    if (newDistance < 0.1f) {
        newDistance = 0.1f;
        newPos = m_orbitPivot - toPivot * newDistance;
    }
    
    m_camera->getTransform().setPosition(newPos);
    m_camera->lookAt(m_orbitPivot);
    
    updateOrbitDistance();
}

void MayaCameraController::moveToPosition(const QVector3D& position, const QVector3D& target, bool animate)
{
    if (!m_camera) return;
    
    if (animate) {
        startAnimation(position, target);
    } else {
        m_camera->getTransform().setPosition(position);
        m_camera->lookAt(target);
        setOrbitPivot(target);
        emit cameraChanged();
    }
}
