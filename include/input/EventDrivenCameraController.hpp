#pragma once

#include "event/event_types.hpp"
#include "event/event_dispatcher.hpp"
#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <memory>

class Camera;
class Scene;

namespace input {

/**
 * @brief Modern event-driven camera controller for Maya-style navigation
 * 
 * This controller uses the event system to handle input and provides
 * professional Maya-style camera navigation:
 * - Alt + LMB = Orbit around pivot
 * - Alt + MMB = Pan camera and pivot
 * - Alt + RMB = Dolly (zoom in/out)
 * - Mouse Wheel = Quick zoom
 */
class EventDrivenCameraController : public QObject {
    Q_OBJECT

public:
    enum class NavigationMode {
        None,
        Orbit,
        Pan,
        Dolly
    };

    explicit EventDrivenCameraController(event::EventDispatcher& dispatcher, QObject* parent = nullptr);
    ~EventDrivenCameraController() override;

    // Setup
    void setCamera(std::shared_ptr<Camera> camera);
    void setScene(std::shared_ptr<Scene> scene);

    // Event handlers
    void onMousePress(event::Event& event);
    void onMouseMove(event::Event& event);
    void onMouseRelease(event::Event& event);
    void onMouseWheel(event::Event& event);

    // Camera operations
    void resetCamera();
    void frameScene(bool animate = true);
    void frameSelection(bool animate = true);

    // Settings
    void setOrbitSensitivity(float sensitivity) { m_orbitSensitivity = sensitivity; }
    void setPanSensitivity(float sensitivity) { m_panSensitivity = sensitivity; }
    void setZoomSensitivity(float sensitivity) { m_zoomSensitivity = sensitivity; }
    void setInvertZoom(bool invert) { m_invertZoom = invert; }

    // State
    bool isNavigating() const { return m_currentMode != NavigationMode::None; }
    NavigationMode getCurrentMode() const { return m_currentMode; }

signals:
    void cameraChanged();

private:
    // Helper methods
    void startNavigation(NavigationMode mode, const QPoint& mousePos);
    void updateNavigation(const QPoint& mousePos);
    void endNavigation();

    void performOrbit(const QVector2D& delta);
    void performPan(const QVector2D& delta);
    void performDolly(float delta);

    bool isMayaOrbitAction(const event::MouseEvent& mouseEvent) const;
    bool isMayaPanAction(const event::MouseEvent& mouseEvent) const;
    bool isMayaDollyAction(const event::MouseEvent& mouseEvent) const;

    QVector2D getMouseDelta(const QPoint& currentPos) const;
    void updateOrbitPivot();

private:
    event::EventDispatcher& m_dispatcher;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Scene> m_scene;

    // Navigation state
    NavigationMode m_currentMode = NavigationMode::None;
    QPoint m_lastMousePos;
    QPoint m_mousePressPos;
    bool m_isNavigating = false;

    // Orbit settings
    QVector3D m_orbitPivot = QVector3D(0, 0, 0);
    float m_orbitDistance = 10.0f;

    // Sensitivity settings
    float m_orbitSensitivity = 1.0f;
    float m_panSensitivity = 1.0f;
    float m_zoomSensitivity = 1.0f;
    bool m_invertZoom = false;
};

} // namespace input
