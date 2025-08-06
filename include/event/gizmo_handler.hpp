#pragma once

#include "gizmo/gizmo.hpp"
#include "scene/Camera.h"
#include <QObject>
#include <QMouseEvent>

class GizmoHandler : public QObject {
    Q_OBJECT

public:
    explicit GizmoHandler(QObject* parent = nullptr);
    ~GizmoHandler() = default;

    void setCamera(Camera* camera);
    void setActiveGizmo(Gizmo* gizmo);
    Gizmo* getActiveGizmo() const;

    void handleMousePress(QMouseEvent* event);
    void handleMouseMove(QMouseEvent* event);
    void handleMouseRelease(QMouseEvent* event);
    bool isHovered(int x, int y) const;

signals:
    void gizmoInteractionStarted();
    void gizmoInteractionEnded();
    void gizmoHoverChanged(bool isHovering);

private:
    Gizmo* m_activeGizmo;
    Camera* m_camera;
}; 