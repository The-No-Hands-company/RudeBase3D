#pragma once

#include "ICameraController.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

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
    Q_OBJECT

public:
    explicit BlenderCameraController(QObject* parent = nullptr);
    ~BlenderCameraController() override;

    // ICameraController interface
    QString getControllerDescription() const;

    // Input handling
    bool handleMousePress(QMouseEvent* event) override;
    bool handleMouseMove(QMouseEvent* event) override;
    bool handleMouseRelease(QMouseEvent* event) override;
    bool handleWheel(QWheelEvent* event) override;
    bool handleKeyPress(QKeyEvent* event) override;
    bool handleKeyRelease(QKeyEvent* event) override;

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

private slots:
    void updateSmoothing();

private:
    // Navigation state
    enum class NavigationMode {
        None,
        Orbit,      // MMB
        Pan,        // Shift + MMB
        Zoom        // Ctrl + MMB
    };

    NavigationMode m_currentMode;
    QPoint m_lastMousePos;
    QPoint m_mousePressPos;
    bool m_isNavigating;

    // Modifier keys
    bool m_shiftPressed;
    bool m_ctrlPressed;
    bool m_altPressed;

    // Configuration
    float m_orbitSensitivity;
    float m_panSensitivity;
    float m_zoomSensitivity;
    bool m_invertZoom;
    bool m_smoothingEnabled;

    // Smoothing
    QTimer* m_smoothingTimer;
    QVector3D m_targetPosition;
    QQuaternion m_targetRotation;
    float m_smoothingFactor;

    // Navigation methods
    void startNavigation(NavigationMode mode, const QPoint& mousePos);
    void updateNavigation(const QPoint& mousePos);
    void endNavigation();

    void performOrbit(const QVector2D& delta);
    void performPan(const QVector2D& delta);
    void performZoom(float delta);

    // Numpad view shortcuts
    void setNumpadView(int key);
    void setFrontView();
    void setBackView();
    void setRightView();
    void setLeftView();
    void setTopView();
    void setBottomView();
    void setUserView();

    // Utility methods
    bool isMiddleMouseButton(QMouseEvent* event) const;
    QVector2D getMouseDelta(const QPoint& currentPos) const;
    void updateSmoothCamera();
};
