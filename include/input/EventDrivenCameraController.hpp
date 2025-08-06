#pragma once

#include "event/event_types.hpp"
#include "event/event_dispatcher.hpp"
#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <memory>

class Camera;
namespace rude { class Scene; }

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
    void setScene(std::shared_ptr<rude::Scene> scene);

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
    void startNavigation(NavigationMode mode, const glm::ivec2& mousePos);
    void updateNavigation(const glm::ivec2& mousePos);
    void endNavigation();

    void performOrbit(const glm::vec2& delta);
    void performPan(const glm::vec2& delta);
    void performDolly(float delta);

    bool isMayaOrbitAction(const event::MouseEvent& mouseEvent) const;
    bool isMayaPanAction(const event::MouseEvent& mouseEvent) const;
    bool isMayaDollyAction(const event::MouseEvent& mouseEvent) const;

    glm::vec2 getMouseDelta(const glm::ivec2& currentPos) const;
    void updateOrbitPivot();

private:
    event::EventDispatcher& m_dispatcher;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<rude::Scene> m_scene;

    // Navigation state
    NavigationMode m_currentMode = NavigationMode::None;
    glm::ivec2 m_lastMousePos;
    glm::ivec2 m_mousePressPos;
    bool m_isNavigating = false;

    // Orbit settings
    glm::vec3 m_orbitPivot = glm::vec3(0, 0, 0);
    float m_orbitDistance = 10.0f;

    // Sensitivity settings
    float m_orbitSensitivity = 1.0f;
    float m_panSensitivity = 1.0f;
    float m_zoomSensitivity = 1.0f;
    bool m_invertZoom = false;
};

} // namespace input
