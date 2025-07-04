#pragma once

#include "ICameraController.h"
#include <QPoint>
#include <QTimer>
#include <QSet>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

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
class MayaCameraController : public ICameraController
{
    Q_OBJECT
    
public:
    explicit MayaCameraController(QObject* parent = nullptr);
    ~MayaCameraController() override;
    
    // ICameraController interface
    void setCamera(std::shared_ptr<Camera> camera) override;
    void setScene(std::shared_ptr<Scene> scene) override;
    
    bool handleMousePress(QMouseEvent* event) override;
    bool handleMouseMove(QMouseEvent* event) override;
    bool handleMouseRelease(QMouseEvent* event) override;
    bool handleWheel(QWheelEvent* event) override;
    bool handleKeyPress(QKeyEvent* event) override;
    bool handleKeyRelease(QKeyEvent* event) override;
    
    void resetCamera() override;
    void frameScene(bool animate = true) override;
    void frameSelection(bool animate = true) override;
    void updateAspectRatio(float aspectRatio) override;
    
    QVector3D getWorldPosition() const override;
    QMatrix4x4 getViewMatrix() const override;
    QMatrix4x4 getProjectionMatrix() const override;
    QVector3D screenToWorldRay(const QVector2D& screenPos, const QSize& viewportSize) const override;
    
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
    
    QString getControllerName() const override { return "Maya"; }
    QString getControllerDescription() const override;
    
    // Maya-specific features
    void setOrbitPivot(const QVector3D& pivot);
    QVector3D getOrbitPivot() const { return m_orbitPivot; }
    void setSmartPivot(bool enabled) { m_smartPivot = enabled; }
    bool isSmartPivotEnabled() const { return m_smartPivot; }
    
    // View operations
    void tumbleStart(const QPoint& startPos);
    void tumbleUpdate(const QPoint& currentPos);
    void tumbleEnd();
    
    void trackStart(const QPoint& startPos);
    void trackUpdate(const QPoint& currentPos);
    void trackEnd();
    
    void dollyStart(const QPoint& startPos);
    void dollyUpdate(const QPoint& currentPos);
    void dollyEnd();
    
private slots:
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
    QVector3D m_orbitPivot;
    float m_orbitDistance;
    bool m_smartPivot;
    
    // Interaction state
    InteractionMode m_currentMode;
    QPoint m_lastMousePos;
    QPoint m_interactionStartPos;
    QSet<int> m_pressedKeys;
    Qt::KeyboardModifiers m_currentModifiers;
    
    // Settings
    float m_movementSpeed;
    float m_rotationSpeed;
    float m_panSpeed;
    float m_zoomSpeed;
    bool m_invertY;
    
    // Animation system
    QTimer* m_animationTimer;
    bool m_isAnimating;
    QVector3D m_animStartPos;
    QVector3D m_animTargetPos;
    QVector3D m_animStartPivot;
    QVector3D m_animTargetPivot;
    float m_animationTime;
    float m_animationDuration;
    
    // Helper methods
    bool isAltPressed() const;
    bool isCtrlPressed() const;
    bool isShiftPressed() const;
    
    void updateOrbitDistance();
    void updateSmartPivot(const QPoint& screenPos);
    QVector3D calculateSceneCenter() const;
    QVector3D calculateSelectionCenter() const;
    float calculateFramingDistance(const QVector3D& target, float objectSize) const;
    
    void startAnimation(const QVector3D& targetPos, const QVector3D& targetPivot);
    void stopAnimation();
    
    // Camera manipulation
    void orbitAroundPivot(float deltaYaw, float deltaPitch);
    void panCamera(const QVector3D& worldDelta);
    void dollyCamera(float deltaDistance);
    void moveToPosition(const QVector3D& position, const QVector3D& target, bool animate = true);
};
