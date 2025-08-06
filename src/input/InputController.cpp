#include "InputController.h"
#include "InputEvents.h"
#include "CameraController.h"
#include "core/scene.hpp"
#include "tools/selection/SelectionManager.h"
#include "ui/viewport/Viewport3D.h"
#include <cmath>

InputController::InputController()
    : m_viewport(nullptr)
    , m_mouseButton(MouseButton::None)
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

void InputController::setCameraController(std::shared_ptr<CameraController> cameraController) {
    m_cameraController = cameraController;
}

void InputController::setScene(std::shared_ptr<rude::Scene> scene) {
    m_scene = scene;
}

void InputController::setViewport(Viewport3D* viewport) {
    m_viewport = viewport;
}

void InputController::setSelectionManager(std::shared_ptr<SelectionManager> selectionManager) {
    m_selectionManager = selectionManager;
}

void InputController::handleMousePress(const glm::ivec2& mousePos, MouseButton button, KeyboardModifier modifiers) {
    m_mouseButton = button;
    m_lastMousePos = mousePos;
    m_isDragging = false;
    m_currentModifiers = modifiers;
    if (m_navigationMode == NavigationMode::FPS && button == MouseButton::Right && m_viewport) {
        m_fpsMode = true;
        // Mouse centering logic for GLM/Qt interop (UI code should handle cursor)
    }
    // ...event dispatch code...
}

void InputController::handleMouseMove(const glm::ivec2& mousePos, KeyboardModifier modifiers) {
    glm::ivec2 delta = mousePos - m_lastMousePos;
    m_currentModifiers = modifiers;
    if (!m_isDragging && (abs(delta.x) > 2 || abs(delta.y) > 2)) {
        m_isDragging = true;
    }
    if (m_isDragging || m_fpsMode) {
        handleCameraControl(delta);
    }
    m_lastMousePos = mousePos;
    // ...event dispatch code...
}

void InputController::handleMouseRelease(const glm::ivec2& mousePos, MouseButton button, KeyboardModifier modifiers) {
    if (!m_isDragging && button == MouseButton::Left && !isModifierPressed(KeyboardModifier::Alt)) {
        if (m_selectionManager && m_selectionManager->getSelectionType() != SelectionType::Object) {
            handleMeshElementSelection(mousePos);
        } else {
            handleObjectSelection(mousePos);
        }
    }
    if (m_fpsMode && button == MouseButton::Right) {
        m_fpsMode = false;
        // UI code should handle cursor
    }
    m_mouseButton = MouseButton::None;
    m_isDragging = false;
    m_currentModifiers = KeyboardModifier::None;
    // ...event dispatch code...
}

void InputController::handleWheel(float wheelDelta, KeyboardModifier modifiers) {
    // ...event dispatch code...
}

void InputController::handleKeyPress(int key, KeyboardModifier modifiers) {
    // ...event dispatch code...
}

void InputController::handleKeyRelease(int key, KeyboardModifier modifiers) {
    // ...event dispatch code...
}

void InputController::setNavigationMode(NavigationMode mode) { m_navigationMode = mode; }
NavigationMode InputController::getNavigationMode() const { return m_navigationMode; }
void InputController::setCameraSensitivity(float sensitivity) { m_cameraSensitivity = sensitivity; }
void InputController::setPanSpeed(float speed) { m_panSpeed = speed; }
void InputController::setZoomSpeed(float speed) { m_zoomSpeed = speed; }
void InputController::setMovementSpeed(float speed) { m_movementSpeed = speed; }
void InputController::setInvertY(bool invert) { m_invertY = invert; }
void InputController::setEnableMouseCapture(bool enable) { m_enableMouseCapture = enable; }
float InputController::getCameraSensitivity() const { return m_cameraSensitivity; }
float InputController::getPanSpeed() const { return m_panSpeed; }
float InputController::getZoomSpeed() const { return m_zoomSpeed; }
// Duplicate definition removed

// Duplicate definition removed

void InputController::handleCameraControl(const glm::ivec2& delta)
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

void InputController::handleMayaNavigation(const glm::ivec2& delta)
{
    if (!m_cameraController) return;
    
    bool altPressed = isModifierPressed(KeyboardModifier::Alt);
    bool shiftPressed = isModifierPressed(KeyboardModifier::Shift);
    // Maya navigation requires Alt modifier for most actions
    if (!altPressed) {
        return;
    }
    if (m_mouseButton == MouseButton::Left) {
        // Alt + LMB = Orbit around scene center
        glm::vec3 target = getSceneCenter();
        m_cameraController->orbitAroundPoint(target, -delta.x * m_cameraSensitivity * 0.5f, 
                                            delta.y * m_cameraSensitivity * 0.5f);
    } else if (m_mouseButton == MouseButton::Middle) {
        // Alt + MMB = Pan
        m_cameraController->pan(glm::vec3(-delta.x * m_panSpeed * 50.0f, 
                                         delta.y * m_panSpeed * 50.0f, 0.0f));
    } else if (m_mouseButton == MouseButton::Right) {
        // Alt + RMB = Dolly (zoom)
        float zoomDelta = -delta.y * m_zoomSpeed * 0.1f;
        m_cameraController->dolly(zoomDelta);
    }
}

void InputController::handleBlenderNavigation(const glm::ivec2& delta)
{
    if (!m_cameraController) return;
    
    bool shiftPressed = isModifierPressed(KeyboardModifier::Shift);
    bool ctrlPressed = isModifierPressed(KeyboardModifier::Control);
    if (m_mouseButton == MouseButton::Middle) {
        if (shiftPressed) {
            // Shift + MMB = Pan
            m_cameraController->pan(glm::vec3(-delta.x * m_panSpeed, 
                                             delta.y * m_panSpeed, 0.0f));
        } else if (ctrlPressed) {
            // Ctrl + MMB = Zoom
            m_cameraController->zoom(-delta.y * m_panSpeed * 10.0f);
        } else {
            // MMB = Orbit
            glm::vec3 target = getSceneCenter();
            m_cameraController->orbitAroundPoint(target, -delta.x * m_cameraSensitivity, 
                                                delta.y * m_cameraSensitivity);
        }
    }
}

void InputController::handleCADNavigation(const glm::ivec2& delta)
{
    if (!m_cameraController) return;
    
    if (m_mouseButton == MouseButton::Right) {
        // RMB = Orbit
        m_cameraController->orbit(-delta.x * m_cameraSensitivity, 
                                 delta.y * m_cameraSensitivity);
    } else if (m_mouseButton == MouseButton::Middle) {
        // MMB = Pan  
        m_cameraController->pan(glm::vec3(-delta.x * m_panSpeed, 
                                         delta.y * m_panSpeed, 0.0f));
    }
}

void InputController::handleFPSNavigation(const glm::ivec2& delta)
{
    if (!m_cameraController) return;
    
    if (m_fpsMode) {
        // Mouse look
        float sensitivity = m_cameraSensitivity * 0.1f;
        m_cameraController->rotate(-delta.y * sensitivity, 
                                   -delta.x * sensitivity);
    }
}

void InputController::updateCameraFromKeys()
{
    if (!m_cameraController) return;
    
    float speed = m_movementSpeed * 0.016f; // Assume 60 FPS
    
    // Shift for faster movement
    // TODO: Update key handling to use unified key codes if available
}

void InputController::handleObjectSelection(const glm::ivec2& pos)
{
    if (!m_scene || !m_cameraController || !m_viewport) return;
    
    // Convert mouse position to world ray
    glm::vec2 screenPos(pos.x, pos.y);
    glm::ivec2 viewportSize(m_viewport->size().width(), m_viewport->size().height());
    glm::vec3 rayDirection = m_cameraController->screenToWorldRay(screenPos, viewportSize);
    glm::vec3 rayOrigin = m_cameraController->getWorldPosition();
    // Use scene manager to pick object using ray
    // We need to add scene manager reference to InputController for this to work
    // For now this is a stub implementation
    Entity* pickedObject = nullptr;
}

void InputController::handleMeshElementSelection(const glm::ivec2& pos)
{
    if (!m_selectionManager || !m_cameraController || !m_viewport) return;
    
    // Convert mouse position to world ray
    glm::vec2 screenPos(pos.x, pos.y);
    glm::ivec2 viewportSize(m_viewport->size().width(), m_viewport->size().height());
    glm::vec3 rayDirection = m_cameraController->screenToWorldRay(screenPos, viewportSize);
    glm::vec3 rayOrigin = m_cameraController->getWorldPosition();
    // Use SelectionManager's raycast to find mesh elements
    auto rayHit = m_selectionManager->raycast(rayOrigin, rayDirection);
    if (rayHit.hit) {
        bool addToSelection = isModifierPressed(KeyboardModifier::Control);
        // Clear previous selection if not adding to it
        if (!addToSelection) {
            m_selectionManager->clearSelection();
        }
        // Select the appropriate element based on selection type
        switch (m_selectionManager->getSelectionType()) {
            case SelectionType::Vertex:
                if (rayHit.vertex) {
                    m_selectionManager->selectVertex(rayHit.vertex, true);
                }
                break;
            case SelectionType::Edge:
                if (rayHit.edge) {
                    m_selectionManager->selectEdge(rayHit.edge, true);
                }
                break;
            case SelectionType::Face:
                if (rayHit.face) {
                    m_selectionManager->selectFace(rayHit.face, true);
                }
                break;
            default:
                break;
        }
    } else {
        // Clear selection if not holding Control
        if (!isModifierPressed(KeyboardModifier::Control)) {
            m_selectionManager->clearSelection();
        }
    }
}

glm::vec3 InputController::getSceneCenter() const
{
    if (!m_scene) return glm::vec3(0, 0, 0);
    // In a real implementation, we would use SceneManager to get the center
    // For now, return the origin as the center
    return glm::vec3(0, 0, 0);
}

bool InputController::isModifierPressed(KeyboardModifier modifier) const
{
    return (m_currentModifiers & modifier) != KeyboardModifier::None;
}

bool InputController::isOrbitAction() const
{
    switch (m_navigationMode) {
        case NavigationMode::Maya:
            return isModifierPressed(KeyboardModifier::Alt) && m_mouseButton == MouseButton::Left;
        case NavigationMode::Blender:
            return m_mouseButton == MouseButton::Middle && 
                   !isModifierPressed(KeyboardModifier::Shift) && 
                   !isModifierPressed(KeyboardModifier::Control);
        case NavigationMode::CAD:
            return m_mouseButton == MouseButton::Right;
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
            return isModifierPressed(KeyboardModifier::Alt) && m_mouseButton == MouseButton::Middle;
        case NavigationMode::Blender:
            return m_mouseButton == MouseButton::Middle && isModifierPressed(KeyboardModifier::Shift);
        case NavigationMode::CAD:
            return m_mouseButton == MouseButton::Middle;
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
            return isModifierPressed(KeyboardModifier::Alt) && m_mouseButton == MouseButton::Right;
        case NavigationMode::Blender:
            return m_mouseButton == MouseButton::Middle && isModifierPressed(KeyboardModifier::Control);
        case NavigationMode::CAD:
            return false; // CAD uses wheel for zoom
        case NavigationMode::FPS:
            return false; // FPS uses keys for movement
        default:
            return false;
    }
}

// Removed stray closing brace and comment
