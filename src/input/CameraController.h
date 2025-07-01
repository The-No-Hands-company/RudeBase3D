#pragma once

#include "Common.h"
#include <QObject>
#include <QTimer>
#include <memory>

class Camera;
class Scene;

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
    CustomPivot     // Orbit around user-defined pivot point
};

class CameraController : public QObject
{
    Q_OBJECT
    
public:
    explicit CameraController(QObject* parent = nullptr);
    
    // Dependencies
    void setCamera(std::shared_ptr<Camera> camera);
    void setScene(std::shared_ptr<Scene> scene);
    
    // Camera modes
    void setCameraMode(CameraMode mode);
    CameraMode getCameraMode() const { return m_cameraMode; }
    
    void setOrbitMode(OrbitMode mode);
    OrbitMode getOrbitMode() const { return m_orbitMode; }
    
    void setCustomPivot(const QVector3D& pivot);
    QVector3D getCustomPivot() const { return m_customPivot; }
    
    // Camera operations
    void resetCamera();
    void frameScene(bool animate = true);
    void frameSelectedObject(bool animate = true);
    void frameAll(bool animate = true);
    void updateAspectRatio(float aspectRatio);
    
    // Basic camera movement
    void orbit(float deltaYaw, float deltaPitch);
    void orbitAroundPoint(const QVector3D& center, float deltaYaw, float deltaPitch);
    void pan(const QVector3D& delta);
    void dolly(float delta); // Forward/backward movement along view direction
    void zoom(float delta);  // FOV-based zoom for perspective cameras
    
    // Advanced camera movement (fly mode)
    void moveForward(float distance);
    void moveBackward(float distance);
    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
    void rotate(float deltaPitch, float deltaYaw, float deltaRoll = 0.0f);
    
    // Focus and framing
    void focusOnPoint(const QVector3D& point, bool animate = true);
    void focusOnObject(std::shared_ptr<class SceneObject> object, bool animate = true);
    void setViewDirection(const QVector3D& direction, const QVector3D& up = QVector3D(0, 1, 0), bool animate = true);
    
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
    QVector3D getWorldPosition() const;
    QMatrix4x4 getViewMatrix() const;
    QMatrix4x4 getProjectionMatrix() const;
    QVector3D screenToWorldRay(const QVector2D& screenPos, const QSize& viewportSize) const;
    QVector3D getCurrentPivot() const;
    float getDistanceToTarget() const;
    
signals:
    void cameraChanged();
    void cameraModeChanged(CameraMode mode);
    void orbitModeChanged(OrbitMode mode);
    
private slots:
    void updateAnimation();
    
private:
    // Helper methods
    QVector3D getSceneCenter() const;
    QVector3D getSelectionCenter() const;
    float calculateFramingDistance(const QVector3D& sceneSize) const;
    QVector3D calculateFramingPosition(const QVector3D& target, const QVector3D& size) const;
    
    // Animation helpers
    void startAnimation(const QVector3D& targetPosition, const QVector3D& targetLookAt);
    void stopAnimation();
    bool isAnimating() const { return m_animationTimer && m_animationTimer->isActive(); }
    
    // Orbit helpers
    QVector3D getOrbitCenter() const;
    void updateOrbitDistance();
    
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Scene> m_scene;
    
    // Camera control modes
    CameraMode m_cameraMode;
    OrbitMode m_orbitMode;
    QVector3D m_customPivot;
    
    // Movement settings
    float m_movementSpeed;
    float m_rotationSpeed;
    float m_panSpeed;
    float m_zoomSpeed;
    float m_animationSpeed;
    bool m_invertY;
    
    // Orbit settings
    float m_orbitDistance;
    QVector3D m_orbitCenter;
    
    // Animation system
    QTimer* m_animationTimer;
    QVector3D m_animStartPosition;
    QVector3D m_animTargetPosition;
    QVector3D m_animStartLookAt;
    QVector3D m_animTargetLookAt;
    float m_animationTime;
    float m_animationDuration;
};

