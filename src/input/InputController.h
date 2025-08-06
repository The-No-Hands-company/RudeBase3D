#pragma once

// Core includes
#include "Common.h"
#include "InputEvents.h"
#include "CameraController.h"
#include "tools/selection/SelectionManager.h"
#include "ui/viewport/Viewport3D.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <set>
#include <memory>
// Forward declaration for rude::Scene
class InputController
{
public:
    InputController();

    // Set dependencies
    void setCameraController(std::shared_ptr<CameraController> cameraController);
    void setScene(std::shared_ptr<rude::Scene> scene);
    void setViewport(Viewport3D* viewport);
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);

    // Input handling (cross-platform, unified enums only)
    void handleMousePress(const glm::ivec2& mousePos, MouseButton button, KeyboardModifier modifiers);
    void handleMouseMove(const glm::ivec2& mousePos, KeyboardModifier modifiers);
    void handleMouseRelease(const glm::ivec2& mousePos, MouseButton button, KeyboardModifier modifiers);
    void handleWheel(float wheelDelta, KeyboardModifier modifiers);
    void handleKeyPress(int key, KeyboardModifier modifiers);
    void handleKeyRelease(int key, KeyboardModifier modifiers);

    // Navigation settings
    void setNavigationMode(NavigationMode mode);
    NavigationMode getNavigationMode() const;

    // Sensitivity settings
    void setCameraSensitivity(float sensitivity);
    void setPanSpeed(float speed);
    void setZoomSpeed(float speed);
    void setMovementSpeed(float speed);

    // Input mode settings
    void setInvertY(bool invert);
    void setEnableMouseCapture(bool enable);

    float getCameraSensitivity() const;
    float getPanSpeed() const;
    float getZoomSpeed() const;
    float getMovementSpeed() const;
    bool isYInverted() const;

private:
    void handleCameraControl(const glm::ivec2& delta);
    void handleObjectSelection(const glm::ivec2& pos);
    void handleMeshElementSelection(const glm::ivec2& pos);
    void updateCameraFromKeys();
    glm::vec3 getSceneCenter() const;
    void handleMayaNavigation(const glm::ivec2& delta);
    void handleBlenderNavigation(const glm::ivec2& delta);
    void handleCADNavigation(const glm::ivec2& delta);
    void handleFPSNavigation(const glm::ivec2& delta);
    bool isModifierPressed(KeyboardModifier modifier) const;
    bool isOrbitAction() const;
    bool isPanAction() const;
    bool isZoomAction() const;

    std::shared_ptr<CameraController> m_cameraController;
    std::shared_ptr<rude::Scene> m_scene;
    Viewport3D* m_viewport;
    std::shared_ptr<SelectionManager> m_selectionManager;

    MouseButton m_mouseButton;
    glm::ivec2 m_lastMousePos;
    bool m_isDragging;
    NavigationMode m_navigationMode;
    std::set<int> m_pressedKeys;
    KeyboardModifier m_currentModifiers;

    float m_cameraSensitivity;
    float m_panSpeed;
    float m_zoomSpeed;
    float m_movementSpeed;
    bool m_invertY;
    bool m_enableMouseCapture;

    bool m_fpsMode;
    glm::ivec2 m_centerPos;
};
