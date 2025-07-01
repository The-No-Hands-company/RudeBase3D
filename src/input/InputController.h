#pragma once

#include "Common.h"
#include <QObject>
#include <QPoint>
#include <QSet>
#include <Qt>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class CameraController;
class Scene;
class Viewport3D;
class SelectionManager; // Add SelectionManager forward declaration

enum class NavigationMode {
    Maya,        // Maya-style: Alt+LMB=orbit, Alt+MMB=pan, Alt+RMB=zoom
    Blender,     // Blender-style: MMB=orbit, Shift+MMB=pan, Ctrl+MMB=zoom
    CAD,         // CAD-style: RMB=orbit, MMB=pan, wheel=zoom
    FPS          // FPS-style: WASD movement, mouse look
};

class InputController : public QObject
{
    Q_OBJECT
    
public:
    explicit InputController(QObject* parent = nullptr);
    
    // Set dependencies
    void setCameraController(std::shared_ptr<CameraController> cameraController);
    void setScene(std::shared_ptr<Scene> scene);
    void setViewport(Viewport3D* viewport);
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager); // Add SelectionManager setter
    
    // Input handling
    void handleMousePress(QMouseEvent* event);
    void handleMouseMove(QMouseEvent* event);
    void handleMouseRelease(QMouseEvent* event);
    void handleWheel(QWheelEvent* event);
    void handleKeyPress(QKeyEvent* event);
    void handleKeyRelease(QKeyEvent* event);
    
    // Navigation settings
    void setNavigationMode(NavigationMode mode) { m_navigationMode = mode; }
    NavigationMode getNavigationMode() const { return m_navigationMode; }
    
    // Sensitivity settings
    void setCameraSensitivity(float sensitivity) { m_cameraSensitivity = sensitivity; }
    void setPanSpeed(float speed) { m_panSpeed = speed; }
    void setZoomSpeed(float speed) { m_zoomSpeed = speed; }
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    
    // Input mode settings
    void setInvertY(bool invert) { m_invertY = invert; }
    void setEnableMouseCapture(bool enable) { m_enableMouseCapture = enable; }
    
    float getCameraSensitivity() const { return m_cameraSensitivity; }
    float getPanSpeed() const { return m_panSpeed; }
    float getZoomSpeed() const { return m_zoomSpeed; }
    float getMovementSpeed() const { return m_movementSpeed; }
    bool isYInverted() const { return m_invertY; }
    
signals:
    void transformModeChanged(TransformMode mode);
    void renderModeChanged(RenderMode mode);
    void gridToggled(bool visible);
    void sceneFrameRequested();
    void viewportUpdateRequested();
    void navigationModeChanged(NavigationMode mode);
    
private:
    void handleCameraControl(const QPoint& delta);
    void handleObjectSelection(const QPoint& pos);
    void handleMeshElementSelection(const QPoint& pos); // Add mesh element selection method
    void handleFPSMovement();
    void updateCameraFromKeys();
    QVector3D getSceneCenter() const;
    
    // Maya-style navigation
    void handleMayaNavigation(const QPoint& delta);
    
    // Blender-style navigation  
    void handleBlenderNavigation(const QPoint& delta);
    
    // CAD-style navigation
    void handleCADNavigation(const QPoint& delta);
    
    // FPS-style navigation
    void handleFPSNavigation(const QPoint& delta);
    
    // Key mapping helpers
    bool isModifierPressed(Qt::KeyboardModifier modifier) const;
    bool isOrbitAction() const;
    bool isPanAction() const;
    bool isZoomAction() const;
    
    std::shared_ptr<CameraController> m_cameraController;
    std::shared_ptr<Scene> m_scene;
    Viewport3D* m_viewport;
    std::shared_ptr<SelectionManager> m_selectionManager; // Add SelectionManager member
    
    // Input state
    Qt::MouseButton m_mouseButton;
    QPoint m_lastMousePos;
    bool m_isDragging;
    NavigationMode m_navigationMode;
    QSet<int> m_pressedKeys;
    Qt::KeyboardModifiers m_currentModifiers;
    
    // Settings
    float m_cameraSensitivity;
    float m_panSpeed;
    float m_zoomSpeed;
    float m_movementSpeed;
    bool m_invertY;
    bool m_enableMouseCapture;
    
    // FPS mode state
    bool m_fpsMode;
    QPoint m_centerPos;
};
