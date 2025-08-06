#pragma once

#include "ICameraController.h"
#include <functional>
#include "InputEvents.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Blender-style camera controller
 * 
 * Implements Blender's navigation style:
 * - Middle Mouse Button (MMB) + Drag: Orbit
 * - Shift + MMB + Drag: Pan
 * - Ctrl + MMB + Drag: Zoom (alternative to scroll)
 * - Mouse Wheel: Zoom
 * - Numpad keys: Predefined views (1=Front, 3=Right, 7=Top, etc.)
 * - Home: Frame all objects
 * - Period (.): Focus on selected object
 */
class BlenderCameraController : public ICameraController
{
    // Cross-platform: no Q_OBJECT
public:
    // Cross-platform camera changed callback
    std::function<void()> cameraChangedCallback;

public:
    BlenderCameraController();
    ~BlenderCameraController() override;

    // ICameraController interface
    std::string getControllerDescription() const override;

    // Input handling
    bool handleMousePress(const MouseEvent& event) override;
    bool handleMouseMove(const MouseEvent& event) override;
    bool handleMouseRelease(const MouseEvent& event) override;
    bool handleWheel(const WheelEvent& event) override;
    bool handleKeyPress(const KeyEvent& event) override;
    bool handleKeyRelease(const KeyEvent& event) override;

    // Camera operations
    void frameScene(bool animate = true) override;
    void frameSelection(bool animate = true) override;
    void resetCamera() override;

    // Configuration
    void setOrbitSensitivity(float sensitivity);
    void setPanSensitivity(float sensitivity);
    void setZoomSensitivity(float sensitivity);
    void setInvertZoom(bool invert);
    void setSmoothing(bool enabled);

    void updateSmoothing();

private:
    NavigationAction m_currentAction;
    glm::ivec2 m_lastMousePos;
    glm::ivec2 m_mousePressPos;
    bool m_isNavigating;
    float m_orbitSensitivity;
    float m_panSensitivity;
    float m_zoomSensitivity;
    bool m_invertZoom;
    bool m_smoothingEnabled;
    glm::vec3 m_targetPosition;
    glm::quat m_targetRotation;
    float m_smoothingFactor;
    void startNavigation(NavigationAction action, const glm::ivec2& mousePos);
    void updateNavigation(const glm::ivec2& mousePos);
    void endNavigation();
    void performOrbit(const glm::vec2& delta);
    void performPan(const glm::vec2& delta);
    void performZoom(float delta);
    void setNumpadView(int key);
    void setFrontView();
    void setBackView();
    void setRightView();
    void setLeftView();
    void setTopView();
    void setBottomView();
    void setUserView();
    bool isMiddleMouseButton(int button) const;
    glm::vec2 getMouseDelta(const glm::ivec2& currentPos) const;
    void updateSmoothCamera();
};
