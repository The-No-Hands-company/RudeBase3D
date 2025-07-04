#include "InputController.h"
#include "CameraController.h"
#include "core/scene.hpp"
#include "tools/selection/SelectionManager.h"
#include "ui/viewport/Viewport3D.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QCursor>
#include <QApplication>
#include <cmath>

InputController::InputController(QObject* parent)
    : QObject(parent)
    , m_viewport(nullptr)
    , m_mouseButton(Qt::NoButton)
    , m_isDragging(false)
    , m_navigationMode(NavigationMode::Maya)
    , m_cameraSensitivity(1.0f)
    , m_panSpeed(0.01f)
    , m_zoomSpeed(0.1f)
    , m_movementSpeed(5.0f)
    , m_invertY(false)
    , m_enableMouseCapture(true)
    , m_fpsMode(false)
{
}

void InputController::setCameraController(std::shared_ptr<CameraController> cameraController)
{
    m_cameraController = cameraController;
}

void InputController::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
}

void InputController::setViewport(Viewport3D* viewport)
{
    m_viewport = viewport;
}

void InputController::setSelectionManager(std::shared_ptr<SelectionManager> selectionManager)
{
    m_selectionManager = selectionManager;
}

void InputController::handleMousePress(QMouseEvent* event)
{
    m_mouseButton = event->button();
    m_lastMousePos = event->pos();
    m_isDragging = false;
    m_currentModifiers = event->modifiers();
    
    bool altPressed = (event->modifiers() & Qt::AltModifier) != 0;
    qDebug() << "Mouse press - Button:" << event->button() << "Modifiers:" << event->modifiers() << "Alt pressed:" << altPressed;
    
    // For FPS mode, capture mouse on right click
    if (m_navigationMode == NavigationMode::FPS && event->button() == Qt::RightButton && m_viewport) {
        m_fpsMode = true;
        m_centerPos = m_viewport->rect().center();
        if (m_enableMouseCapture) {
            m_viewport->setCursor(Qt::BlankCursor);
            QCursor::setPos(m_viewport->mapToGlobal(m_centerPos));
        }
    }
}

void InputController::handleMouseMove(QMouseEvent* event)
{
    QPoint currentPos = event->pos();
    QPoint delta = currentPos - m_lastMousePos;
    
    // Update current modifiers from the move event, but preserve the button state from press
    m_currentModifiers = event->modifiers();
    
    // Start dragging if mouse moved enough
    if (!m_isDragging && (abs(delta.x()) > 2 || abs(delta.y()) > 2)) {
        m_isDragging = true;
        bool altPressed = (m_currentModifiers & Qt::AltModifier) != 0;
        qDebug() << "Started dragging with button:" << m_mouseButton << "modifiers:" << m_currentModifiers << "Alt pressed:" << altPressed;
    }
    
    if (m_isDragging || m_fpsMode) {
        handleCameraControl(delta);
    }
    
    // For FPS mode, reset cursor to center
    if (m_fpsMode && m_viewport && m_enableMouseCapture) {
        QCursor::setPos(m_viewport->mapToGlobal(m_centerPos));
        m_lastMousePos = m_centerPos;
    } else {
        m_lastMousePos = currentPos;
    }
}

void InputController::handleMouseRelease(QMouseEvent* event)
{
    // Handle selection on click (not drag)
    if (!m_isDragging && event->button() == Qt::LeftButton && !isModifierPressed(Qt::AltModifier)) {
        // Check if we have a selection manager and are in a mesh selection mode
        if (m_selectionManager && m_selectionManager->getSelectionType() != SelectionType::Object) {
            handleMeshElementSelection(event->pos());
        } else {
            handleObjectSelection(event->pos());
        }
    }
    
    // Exit FPS mode
    if (m_fpsMode && event->button() == Qt::RightButton) {
        m_fpsMode = false;
        if (m_viewport) {
            m_viewport->setCursor(Qt::ArrowCursor);
        }
    }
    
    m_mouseButton = Qt::NoButton;
    m_isDragging = false;
    m_currentModifiers = Qt::NoModifier;
}

void InputController::handleWheel(QWheelEvent* event)
{
    if (!m_cameraController) {
        return;
    }
    
    float delta = event->angleDelta().y() / 120.0f;
    
    // Check for zoom modifiers based on navigation mode
    if (m_navigationMode == NavigationMode::Blender && (m_currentModifiers & Qt::ControlModifier)) {
        // Blender: Ctrl+Wheel = zoom
        m_cameraController->zoom(delta * m_zoomSpeed);
    } else {
        // Default wheel zoom for other modes
        m_cameraController->zoom(delta * m_zoomSpeed);
    }
}

void InputController::handleKeyPress(QKeyEvent* event)
{
    m_pressedKeys.insert(event->key());
    
    if (!m_viewport) return;
    
    // Handle immediate key actions
    switch (event->key()) {
        case Qt::Key_F:
            if (m_cameraController) {
                if (event->modifiers() & Qt::ShiftModifier) {
                    m_cameraController->frameSelectedObject();
                } else {
                    m_cameraController->frameScene();
                }
            }
            break;
            
        case Qt::Key_G:
            m_viewport->setShowGrid(!m_viewport->isGridVisible());
            emit gridToggled(m_viewport->isGridVisible());
            break;
            
        // Render modes
        case Qt::Key_1:
            if (!(event->modifiers() & Qt::ControlModifier)) {
                m_viewport->setRenderMode(RenderMode::Wireframe);
                emit renderModeChanged(RenderMode::Wireframe);
            } else if (m_cameraController) {
                m_cameraController->setFrontView();
            }
            break;
        case Qt::Key_2:
            m_viewport->setRenderMode(RenderMode::Solid);
            emit renderModeChanged(RenderMode::Solid);
            break;
        case Qt::Key_3:
            if (!(event->modifiers() & Qt::ControlModifier)) {
                m_viewport->setRenderMode(RenderMode::SolidWireframe);
                emit renderModeChanged(RenderMode::SolidWireframe);
            } else if (m_cameraController) {
                m_cameraController->setRightView();
            }
            break;
            
        // Transform modes
        case Qt::Key_Q:
            if (!(m_pressedKeys.contains(Qt::Key_Shift) || m_pressedKeys.contains(Qt::Key_Control))) {
                m_viewport->setTransformMode(TransformMode::Select);
                emit transformModeChanged(TransformMode::Select);
            }
            break;
        case Qt::Key_W:
            if (!(event->modifiers() & Qt::ControlModifier)) {
                m_viewport->setTransformMode(TransformMode::Translate);
                emit transformModeChanged(TransformMode::Translate);
            }
            break;
        case Qt::Key_E:
            if (!(m_pressedKeys.contains(Qt::Key_Shift) || m_pressedKeys.contains(Qt::Key_Control))) {
                m_viewport->setTransformMode(TransformMode::Rotate);
                emit transformModeChanged(TransformMode::Rotate);
            }
            break;
        case Qt::Key_R:
            if (!(m_pressedKeys.contains(Qt::Key_Shift) || m_pressedKeys.contains(Qt::Key_Control))) {
                m_viewport->setTransformMode(TransformMode::Scale);
                emit transformModeChanged(TransformMode::Scale);
            }
            break;
            
        // Predefined views (numpad-style)
        case Qt::Key_7:
            if (m_cameraController) m_cameraController->setTopView();
            break;
        case Qt::Key_9:
            if (m_cameraController) m_cameraController->setIsometricView();
            break;
            
        // Camera mode switching
        case Qt::Key_Tab:
            if (event->modifiers() & Qt::ControlModifier && m_cameraController) {
                // Toggle between Orbit and Fly modes
                auto currentMode = m_cameraController->getCameraMode();
                if (currentMode == CameraMode::Orbit) {
                    m_cameraController->setCameraMode(CameraMode::Fly);
                } else {
                    m_cameraController->setCameraMode(CameraMode::Orbit);
                }
            }
            break;
    }
    
    // Handle continuous movement for FPS mode
    if (m_navigationMode == NavigationMode::FPS || 
        (m_cameraController && m_cameraController->getCameraMode() == CameraMode::Fly)) {
        updateCameraFromKeys();
    }
}

void InputController::handleKeyRelease(QKeyEvent* event)
{
    m_pressedKeys.remove(event->key());
    
    // Update movement for FPS mode
    if (m_navigationMode == NavigationMode::FPS || 
        (m_cameraController && m_cameraController->getCameraMode() == CameraMode::Fly)) {
        updateCameraFromKeys();
    }
}

void InputController::handleCameraControl(const QPoint& delta)
{
    if (!m_cameraController) {
        return;
    }
    
    switch (m_navigationMode) {
        case NavigationMode::Maya:
            handleMayaNavigation(delta);
            break;
        case NavigationMode::Blender:
            handleBlenderNavigation(delta);
            break;
        case NavigationMode::CAD:
            handleCADNavigation(delta);
            break;
        case NavigationMode::FPS:
            handleFPSNavigation(delta);
            break;
    }
}

void InputController::handleMayaNavigation(const QPoint& delta)
{
    if (!m_cameraController) return;
    
    bool altPressed = isModifierPressed(Qt::AltModifier);
    bool shiftPressed = isModifierPressed(Qt::ShiftModifier);
    
    qDebug() << "Maya nav - Button:" << m_mouseButton << "Alt:" << altPressed << "Shift:" << shiftPressed << "Delta:" << delta;
    
    // Maya navigation requires Alt modifier for most actions
    if (!altPressed) {
        qDebug() << "Maya navigation: Alt not pressed, ignoring";
        return;
    }
    
    if (m_mouseButton == Qt::LeftButton) {
        // Alt + LMB = Orbit around scene center
        QVector3D target = getSceneCenter();
        qDebug() << "ORBIT: Alt+LMB - Delta:" << delta << "Target:" << target;
        m_cameraController->orbitAroundPoint(target, -delta.x() * m_cameraSensitivity * 0.5f, 
                                            delta.y() * m_cameraSensitivity * 0.5f);
    } else if (m_mouseButton == Qt::MiddleButton) {
        // Alt + MMB = Pan
        qDebug() << "PAN: Alt+MMB - Delta:" << delta;
        m_cameraController->pan(QVector3D(-delta.x() * m_panSpeed * 50.0f, 
                                         delta.y() * m_panSpeed * 50.0f, 0.0f));
    } else if (m_mouseButton == Qt::RightButton) {
        // Alt + RMB = Dolly (zoom)
        qDebug() << "DOLLY: Alt+RMB - Delta:" << delta;
        float zoomDelta = -delta.y() * m_zoomSpeed * 0.1f;
        m_cameraController->dolly(zoomDelta);
    }
}

void InputController::handleBlenderNavigation(const QPoint& delta)
{
    if (!m_cameraController) return;
    
    bool shiftPressed = isModifierPressed(Qt::ShiftModifier);
    bool ctrlPressed = isModifierPressed(Qt::ControlModifier);
    
    if (m_mouseButton == Qt::MiddleButton) {
        if (shiftPressed) {
            // Shift + MMB = Pan
            m_cameraController->pan(QVector3D(-delta.x() * m_panSpeed, 
                                             delta.y() * m_panSpeed, 0.0f));
        } else if (ctrlPressed) {
            // Ctrl + MMB = Zoom
            m_cameraController->zoom(-delta.y() * m_panSpeed * 10.0f);
        } else {
            // MMB = Orbit
            QVector3D target = getSceneCenter();
            m_cameraController->orbitAroundPoint(target, -delta.x() * m_cameraSensitivity, 
                                                delta.y() * m_cameraSensitivity);
        }
    }
}

void InputController::handleCADNavigation(const QPoint& delta)
{
    if (!m_cameraController) return;
    
    if (m_mouseButton == Qt::RightButton) {
        // RMB = Orbit
        m_cameraController->orbit(-delta.x() * m_cameraSensitivity, 
                                 delta.y() * m_cameraSensitivity);
    } else if (m_mouseButton == Qt::MiddleButton) {
        // MMB = Pan  
        m_cameraController->pan(QVector3D(-delta.x() * m_panSpeed, 
                                         delta.y() * m_panSpeed, 0.0f));
    }
}

void InputController::handleFPSNavigation(const QPoint& delta)
{
    if (!m_cameraController) return;
    
    if (m_fpsMode) {
        // Mouse look
        float sensitivity = m_cameraSensitivity * 0.1f;
        m_cameraController->rotate(-delta.y() * sensitivity, 
                                   -delta.x() * sensitivity);
    }
}

void InputController::updateCameraFromKeys()
{
    if (!m_cameraController) return;
    
    float speed = m_movementSpeed * 0.016f; // Assume 60 FPS
    
    // Shift for faster movement
    if (m_pressedKeys.contains(Qt::Key_Shift)) {
        speed *= 3.0f;
    }
    
    // WASD movement
    if (m_pressedKeys.contains(Qt::Key_W) && !m_pressedKeys.contains(Qt::Key_Control)) {
        m_cameraController->moveForward(speed);
    }
    if (m_pressedKeys.contains(Qt::Key_S)) {
        m_cameraController->moveBackward(speed);
    }
    if (m_pressedKeys.contains(Qt::Key_A)) {
        m_cameraController->moveLeft(speed);
    }
    if (m_pressedKeys.contains(Qt::Key_D)) {
        m_cameraController->moveRight(speed);
    }
    
    // QE for up/down (only in fly mode when not used for other functions)
    if (m_pressedKeys.contains(Qt::Key_Q) && 
        (m_pressedKeys.contains(Qt::Key_Shift) || m_pressedKeys.contains(Qt::Key_Control))) {
        m_cameraController->moveUp(speed);
    }
    if (m_pressedKeys.contains(Qt::Key_E) && 
        (m_pressedKeys.contains(Qt::Key_Shift) || m_pressedKeys.contains(Qt::Key_Control))) {
        m_cameraController->moveDown(speed);
    }
}

void InputController::handleObjectSelection(const QPoint& pos)
{
    if (!m_scene || !m_cameraController || !m_viewport) return;
    
    // Convert mouse position to world ray
    QVector2D screenPos(pos.x(), pos.y());
    QVector3D rayDirection = m_cameraController->screenToWorldRay(screenPos, m_viewport->size());
    QVector3D rayOrigin = m_cameraController->getWorldPosition();
    
    // Use scene manager to pick object using ray
    // We need to add scene manager reference to InputController for this to work
    // For now this is a stub implementation
    Entity* pickedObject = nullptr;
    
    // We'll need to implement this in a real application
    // For now just log a message
    qDebug() << "Object selection at" << pos;
}

void InputController::handleMeshElementSelection(const QPoint& pos)
{
    if (!m_selectionManager || !m_cameraController || !m_viewport) return;
    
    // Convert mouse position to world ray
    QVector2D screenPos(pos.x(), pos.y());
    QVector3D rayDirection = m_cameraController->screenToWorldRay(screenPos, m_viewport->size());
    QVector3D rayOrigin = m_cameraController->getWorldPosition();
    
    // Use SelectionManager's raycast to find mesh elements
    auto rayHit = m_selectionManager->raycast(rayOrigin, rayDirection);
    
    if (rayHit.hit) {
        bool addToSelection = isModifierPressed(Qt::ControlModifier);
        
        // Clear previous selection if not adding to it
        if (!addToSelection) {
            m_selectionManager->clearSelection();
        }
        
        // Select the appropriate element based on selection type
        switch (m_selectionManager->getSelectionType()) {
            case SelectionType::Vertex:
                if (rayHit.vertex) {
                    m_selectionManager->selectVertex(rayHit.vertex, true);
                    qDebug() << "Selected vertex at" << rayHit.vertex->getPosition();
                }
                break;
            case SelectionType::Edge:
                if (rayHit.edge) {
                    m_selectionManager->selectEdge(rayHit.edge, true);
                    qDebug() << "Selected edge";
                }
                break;
            case SelectionType::Face:
                if (rayHit.face) {
                    m_selectionManager->selectFace(rayHit.face, true);
                    qDebug() << "Selected face";
                }
                break;
            default:
                break;
        }
    } else {
        // Clear selection if not holding Ctrl
        if (!isModifierPressed(Qt::ControlModifier)) {
            m_selectionManager->clearSelection();
            qDebug() << "Cleared selection";
        }
    }
}

QVector3D InputController::getSceneCenter() const
{
    if (!m_scene) return QVector3D(0, 0, 0);
    
    // In a real implementation, we would use SceneManager to get the center
    // For now, return the origin as the center
    return QVector3D(0, 0, 0);
}

bool InputController::isModifierPressed(Qt::KeyboardModifier modifier) const
{
    return (m_currentModifiers & modifier) != 0;
}

bool InputController::isOrbitAction() const
{
    switch (m_navigationMode) {
        case NavigationMode::Maya:
            return isModifierPressed(Qt::AltModifier) && m_mouseButton == Qt::LeftButton;
        case NavigationMode::Blender:
            return m_mouseButton == Qt::MiddleButton && 
                   !isModifierPressed(Qt::ShiftModifier) && 
                   !isModifierPressed(Qt::ControlModifier);
        case NavigationMode::CAD:
            return m_mouseButton == Qt::RightButton;
        case NavigationMode::FPS:
            return false; // No orbit in FPS mode
        default:
            return false;
    }
}

bool InputController::isPanAction() const
{
    switch (m_navigationMode) {
        case NavigationMode::Maya:
            return isModifierPressed(Qt::AltModifier) && m_mouseButton == Qt::MiddleButton;
        case NavigationMode::Blender:
            return m_mouseButton == Qt::MiddleButton && isModifierPressed(Qt::ShiftModifier);
        case NavigationMode::CAD:
            return m_mouseButton == Qt::MiddleButton;
        case NavigationMode::FPS:
            return false; // No pan in FPS mode
        default:
            return false;
    }
}

bool InputController::isZoomAction() const
{
    switch (m_navigationMode) {
        case NavigationMode::Maya:
            return isModifierPressed(Qt::AltModifier) && m_mouseButton == Qt::RightButton;
        case NavigationMode::Blender:
            return m_mouseButton == Qt::MiddleButton && isModifierPressed(Qt::ControlModifier);
        case NavigationMode::CAD:
            return false; // CAD uses wheel for zoom
        case NavigationMode::FPS:
            return false; // FPS uses keys for movement
        default:
            return false;
    }
}
