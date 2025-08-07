#pragma once

#include "ICameraController.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <set>
#include <functional>
#include "InputEvents.h"

// Forward declaration for rude::Scene
namespace rude { class Scene; }
class SelectionManager;

/**
 * @brief Professional Maya-style camera controller
 * 
 * Implements industry-standard Maya navigation:
 * - Alt + LMB = Tumble (orbit around pivot)
 * - Alt + MMB = Track (pan camera and pivot)
 * - Alt + RMB = Dolly (zoom in/out)
 * - Wheel = Quick zoom
 * - F = Frame selected/all
 * - Ctrl+Alt+LMB = Rotate around view center
 */
class MayaCameraController : public ICameraController {
public:
    // Cross-platform camera changed callback
    std::function<void()> cameraChangedCallback;
public:
    MayaCameraController();
    ~MayaCameraController() override;
    // ICameraController interface
    void setCamera(std::shared_ptr<Camera> camera) override;
    void setScene(std::shared_ptr<rude::Scene> scene) override;
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);

    bool handleMousePress(const MouseEvent& event) override;
    bool handleMouseMove(const MouseEvent& event) override;
    bool handleMouseRelease(const MouseEvent& event) override;
    bool handleWheel(const WheelEvent& event) override;
    bool handleKeyPress(const KeyEvent& event) override;
    bool handleKeyRelease(const KeyEvent& event) override;

    void resetCamera() override;
    void frameScene(bool animate = true) override;
    void frameSelection(bool animate = true) override;
    void updateAspectRatio(float aspectRatio) override;

    glm::vec3 getWorldPosition() const override;
    glm::mat4 getViewMatrix() const override;
    glm::mat4 getProjectionMatrix() const override;
    glm::vec3 screenToWorldRay(const glm::vec2& screenPos, const glm::ivec2& viewportSize) const override;

    void setMovementSpeed(float speed) override { m_movementSpeed = speed; }
    void setRotationSpeed(float speed) override { m_rotationSpeed = speed; }
    void setPanSpeed(float speed) override { m_panSpeed = speed; }
    void setZoomSpeed(float speed) override { m_zoomSpeed = speed; }
    void setInvertY(bool invert) override { m_invertY = invert; }

    float getMovementSpeed() const override { return m_movementSpeed; }
    float getRotationSpeed() const override { return m_rotationSpeed; }
    float getPanSpeed() const override { return m_panSpeed; }
    float getZoomSpeed() const override { return m_zoomSpeed; }
    bool isYInverted() const override { return m_invertY; }

    std::string getControllerName() const override { return "Maya"; }
    std::string getControllerDescription() const override;

    // Maya-specific features
    void setOrbitPivot(const glm::vec3& pivot);
    glm::vec3 getOrbitPivot() const { return m_orbitPivot; }
    void setSmartPivot(bool enabled) { m_smartPivot = enabled; }
    bool isSmartPivotEnabled() const { return m_smartPivot; }

    // View operations
    void tumbleStart(const glm::ivec2& startPos);
    void tumbleUpdate(const glm::ivec2& currentPos);
    void tumbleEnd();

    void trackStart(const glm::ivec2& startPos);
    void trackUpdate(const glm::ivec2& currentPos);
    void trackEnd();

    void dollyStart(const glm::ivec2& startPos);
    void dollyUpdate(const glm::ivec2& currentPos);
    void dollyEnd();

    void updateAnimation();

private:
    enum class InteractionMode {
        None,
        Tumble,    // Alt + LMB
        Track,     // Alt + MMB  
        Dolly,     // Alt + RMB
        ViewRotate // Ctrl + Alt + LMB
    };

    // Camera state
    glm::vec3 m_orbitPivot;
    float m_orbitDistance;
    bool m_smartPivot;

    // Interaction state
    InteractionMode m_currentMode;
    glm::ivec2 m_lastMousePos;
    glm::ivec2 m_interactionStartPos;
    std::set<int> m_pressedKeys;
    KeyboardModifier m_currentModifiers;

    // Settings
    float m_movementSpeed;
    float m_rotationSpeed;
    float m_panSpeed;
    float m_zoomSpeed;
    bool m_invertY;

    // Animation system
    // Animation system
    bool m_isAnimating;
    glm::vec3 m_animStartPos;
    glm::vec3 m_animTargetPos;
    glm::vec3 m_animStartPivot;
    glm::vec3 m_animTargetPivot;
    float m_animationTime;
    float m_animationDuration;
    float m_animationInterval; // in seconds

    // Helper methods
    bool isAltPressed() const;
    bool isCtrlPressed() const;
    bool isShiftPressed() const;

    void updateOrbitDistance();
    void updateSmartPivot(const glm::ivec2& screenPos);
    glm::vec3 calculateSceneCenter() const;
    glm::vec3 calculateSelectionCenter() const;
    float calculateFramingDistance(const glm::vec3& target, float objectSize) const;

    void startAnimation(const glm::vec3& targetPos, const glm::vec3& targetPivot);
    void stopAnimation();

    // Camera manipulation
    void orbitAroundPivot(float deltaYaw, float deltaPitch);
    void panCamera(const glm::vec3& worldDelta);
    void dollyCamera(float deltaDistance);
    void moveToPosition(const glm::vec3& position, const glm::vec3& target, bool animate = true);

    std::shared_ptr<SelectionManager> m_selectionManager;
    std::shared_ptr<rude::Scene> m_scene;
};
