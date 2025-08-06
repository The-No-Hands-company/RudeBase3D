
#pragma once

#include "Common.h"
#include "core/scene_manager.hpp"
#include <memory>
#include <glm/glm.hpp>

// Forward declarations
class Camera;
class SelectionManager;

// Camera controller navigation modes
enum class CameraMode {
    Orbit,      // Orbit around a pivot point (default for modeling)
    Fly,        // Free-flying camera (FPS-style)
    Pan,        // Pan-only mode (orthographic-style)
    Focus       // Focus on selected object with smooth transitions
};

enum class OrbitMode {
    WorldCenter,    // Orbit around world origin (0,0,0)
    SceneCenter,    // Orbit around scene bounding box center
    Selection,      // Orbit around selected object
    CustomPivot     // Orbit around a custom pivot point
};

class CameraController {
public:
    // Signals (Qt-style, but can be replaced with callback/event system if needed)
    void cameraChanged();

    CameraController();

    void setSceneManager(std::shared_ptr<rude::SceneManager> sceneManager);
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);

    // Camera modes
    void setCameraMode(CameraMode mode);
    CameraMode getCameraMode() const { return m_cameraMode; }

    void setOrbitMode(OrbitMode mode);
    OrbitMode getOrbitMode() const { return m_orbitMode; }

    void setCustomPivot(const glm::vec3& pivot);
    glm::vec3 getCustomPivot() const { return m_customPivot; }
    glm::vec3 getCurrentPivot() const;
    float getDistanceToTarget() const;

    // Camera operations
    void resetCamera();
    void frameScene(bool animate = true);
    void frameSelectedObject(bool animate = true);
    void frameAll(bool animate = true);
    void updateAspectRatio(float aspectRatio);
    void setCamera(std::shared_ptr<Camera> camera);

    // Basic camera movement
    void orbit(float deltaYaw, float deltaPitch);
    void orbitAroundPoint(const glm::vec3& center, float deltaYaw, float deltaPitch);
    void pan(const glm::vec2& delta);
    void dolly(float delta);
    void zoom(float delta);

    // Advanced camera movement (fly mode)
    void moveForward(float distance);
    void moveBackward(float distance);
    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
    void rotate(float deltaPitch, float deltaYaw, float deltaRoll = 0.0f);

    // Focus and framing
    void focusOnPoint(const glm::vec3& point, bool animate = true);
    void focusOnObject(std::shared_ptr<class SceneObject> object, bool animate = true);
    void setViewDirection(const glm::vec3& direction, const glm::vec3& up = glm::vec3(0, 1, 0), bool animate = true);

    // Predefined views
    void setTopView(bool animate = true);
    void setBottomView(bool animate = true);
    void setFrontView(bool animate = true);
    void setBackView(bool animate = true);
    void setLeftView(bool animate = true);
    void setRightView(bool animate = true);
    void setIsometricView(bool animate = true);

    // Settings
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setRotationSpeed(float speed) { m_rotationSpeed = speed; }
    void setPanSpeed(float speed) { m_panSpeed = speed; }
    void setZoomSpeed(float speed) { m_zoomSpeed = speed; }
    void setAnimationSpeed(float speed) { m_animationSpeed = speed; }
    void setInvertY(bool invert) { m_invertY = invert; }

    float getMovementSpeed() const { return m_movementSpeed; }
    float getRotationSpeed() const { return m_rotationSpeed; }
    float getPanSpeed() const { return m_panSpeed; }
    float getZoomSpeed() const { return m_zoomSpeed; }
    bool isYInverted() const { return m_invertY; }

    // Camera queries
    glm::vec3 getWorldPosition() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 screenToWorldRay(const glm::vec2& screenPos, const glm::ivec2& viewportSize) const;

    // Animation methods
    void updateAnimation();
    void startAnimation(const glm::vec3& targetPosition, const glm::vec3& targetLookAt);
    void stopAnimation();
    bool isAnimating() const { return m_animationTime < m_animationDuration; }

private:
    // Helper methods
    glm::vec3 getSceneCenter() const;
    glm::vec3 getSelectionCenter() const;
    float calculateFramingDistance(const glm::vec3& sceneSize) const;
    glm::vec3 calculateFramingPosition(const glm::vec3& target, const glm::vec3& size) const;
    glm::vec3 getOrbitCenter() const;
    void updateOrbitDistance();

    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<rude::SceneManager> m_sceneManager;
    std::shared_ptr<SelectionManager> m_selectionManager;

    // Camera control modes
    CameraMode m_cameraMode;
    OrbitMode m_orbitMode;
    glm::vec3 m_customPivot;

    // Movement settings
    float m_movementSpeed;
    float m_rotationSpeed;
    float m_panSpeed;
    float m_zoomSpeed;
    float m_animationSpeed;
    bool m_invertY;

    // Orbit settings
    float m_orbitDistance;
    glm::vec3 m_orbitCenter;

    // Animation system
    glm::vec3 m_animStartPosition;
    glm::vec3 m_animTargetPosition;
    glm::vec3 m_animStartLookAt;
    glm::vec3 m_animTargetLookAt;
    float m_animationTime;
    float m_animationDuration;
};

