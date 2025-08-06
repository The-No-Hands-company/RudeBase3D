#pragma once

#include "Common.h"
#include "core/selection_manager.hpp"
#include <QWidget>
#include <QGridLayout>
#include <QSplitter>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <memory>

class ICameraController;
class CameraStateManager;
class Scene;
class Camera;
class LightingSystem;
class GridSystem;
class RenderSystem;
class GizmoManager;

// Forward declarations for event system
namespace event {
    class EventDispatcher;
    class MouseHandler;
}

namespace input {
    class EventDrivenCameraController;
}

/**
 * @brief Individual viewport widget with independent camera and rendering
 */
class ViewportWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
    
public:
    enum class ViewType {
        Perspective,
        Top,
        Front,
        Right,
        User,
        Camera
    };
    
    explicit ViewportWidget(QWidget* parent = nullptr);
    ~ViewportWidget() override;
    
    // Setup
    void setScene(std::shared_ptr<rude::Scene> scene);
    void setCameraController(std::unique_ptr<ICameraController> controller);
    void setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem);
    void setGridSystem(std::shared_ptr<GridSystem> gridSystem);
    void setRenderSystem(std::shared_ptr<RenderSystem> renderSystem);
    
    // Viewport configuration
    void setViewType(ViewType type);
    ViewType getViewType() const { return m_viewType; }
    
    void setViewName(const QString& name);
    QString getViewName() const { return m_viewName; }
    
    void setActive(bool active);
    bool isActive() const { return m_isActive; }
    
    // Gizmo access
    GizmoManager* getGizmoManager() const { return m_gizmoManager.get(); }
    
    // Camera access
    std::shared_ptr<Camera> getCamera() const { return m_camera; }
    ICameraController* getCameraController() const { return m_cameraController.get(); }
    
    // Rendering settings
    void setRenderMode(RenderMode mode);
    RenderMode getRenderMode() const { return m_renderMode; }
    
    void setShowGrid(bool show);
    bool isGridVisible() const;
    
    void setShowGizmos(bool show);
    bool areGizmosVisible() const { return m_showGizmos; }
    
    // View operations
    void frameScene(bool animate = true);
    void frameSelection(bool animate = true);
    void resetCamera();
    
signals:
    void viewportActivated(ViewportWidget* viewport);
    void cameraChanged();
    void renderModeChanged(RenderMode mode);
    void selectionChanged();

protected:
    // QOpenGLWidget interface
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    
    // Input events
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void updateSelectionDisplay();

private:
    // Selection methods
    bool performSelection(int x, int y, rude::SelectionMode mode);
    
    // Core components
    std::shared_ptr<rude::Scene> m_scene;
    std::shared_ptr<Camera> m_camera;
    std::unique_ptr<ICameraController> m_cameraController;
    std::shared_ptr<LightingSystem> m_lightingSystem;
    std::shared_ptr<GridSystem> m_gridSystem;
    std::shared_ptr<RenderSystem> m_renderSystem;
    
    // Event system
    std::unique_ptr<event::EventDispatcher> m_eventDispatcher;
    std::unique_ptr<event::MouseHandler> m_mouseHandler;
    std::unique_ptr<input::EventDrivenCameraController> m_eventCameraController;
    std::unique_ptr<GizmoManager> m_gizmoManager;

    // Viewport state
    ViewType m_viewType;
    QString m_viewName;
    bool m_isActive;
    RenderMode m_renderMode;
    bool m_showGizmos;
    
    // View overlay
    void paintViewportOverlay();
    void drawViewportBorder();
    void drawViewportLabel();
    void drawViewportStats();
    
    void setupPredefinedView();
    void updateViewportTitle();
};

/**
 * @brief Professional viewport layout manager
 * 
 * Manages multiple viewport configurations:
 * - Single viewport (1x1)
 * - Quad layout (2x2) 
 * - Triple layout (1x3)
 * - Custom layouts
 */
class ViewportManager : public QWidget
{
    Q_OBJECT
    
public:
    enum class LayoutType {
        Single,     // 1 viewport
        Quad,       // 4 viewports (2x2)
        Triple,     // 3 viewports
        Dual,       // 2 viewports
        Custom      // User-defined layout
    };
    
    explicit ViewportManager(QWidget* parent = nullptr);
    ~ViewportManager() override;
    
    // Layout management
    void setLayout(LayoutType layout);
    LayoutType getCurrentLayout() const { return m_currentLayout; }
    
    // Viewport access
    ViewportWidget* getViewport(int index) const;
    ViewportWidget* getActiveViewport() const { return m_activeViewport; }
    int getViewportCount() const { return m_viewports.size(); }
    const QList<ViewportWidget*>& getViewports() const { return m_viewports; }
    
    // Global settings
    void setScene(std::shared_ptr<rude::Scene> scene);
    void setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem);
    void setGridSystem(std::shared_ptr<GridSystem> gridSystem);
    void setRenderSystem(std::shared_ptr<RenderSystem> renderSystem);
    
    // Camera controller management
    void setCameraControllerType(const QString& controllerType);
    QString getCameraControllerType() const { return m_controllerType; }
    
    // Viewport synchronization
    void setSynchronizeViews(bool sync) { m_synchronizeViews = sync; }
    bool arViewsSynchronized() const { return m_synchronizeViews; }
    
    void setSynchronizeSelection(bool sync) { m_synchronizeSelection = sync; }
    bool isSelectionSynchronized() const { return m_synchronizeSelection; }
    
    // Global operations (applied to all viewports)
    void frameSceneAll(bool animate = true);
    void frameSelectionAll(bool animate = true);
    void resetAllCameras();
    
    // Render mode control
    void setGlobalRenderMode(RenderMode mode);
    void setGridVisibilityAll(bool visible);
    void setGizmosVisibilityAll(bool visible);
    
signals:
    void activeViewportChanged(ViewportWidget* viewport);
    void layoutChanged(LayoutType layout);
    void viewportCountChanged(int count);
    
private slots:
    void onViewportActivated(ViewportWidget* viewport);
    void onViewportCameraChanged();
    
private:
    // Layout management
    LayoutType m_currentLayout;
    QGridLayout* m_gridLayout;
    QList<ViewportWidget*> m_viewports;
    ViewportWidget* m_activeViewport;
    
    // Global systems
    std::shared_ptr<rude::Scene> m_scene;
    std::shared_ptr<LightingSystem> m_lightingSystem;
    std::shared_ptr<GridSystem> m_gridSystem;
    std::shared_ptr<RenderSystem> m_renderSystem;
    
    // Settings
    QString m_controllerType;
    bool m_synchronizeViews;
    bool m_synchronizeSelection;
    
    // Layout creation
    void createSingleLayout();
    void createQuadLayout();
    void createTripleLayout();
    void createDualLayout();
    void clearLayout();
    
    // Viewport management
    ViewportWidget* createViewport(ViewportWidget::ViewType type, const QString& name);
    void setupViewportDefaults(ViewportWidget* viewport);
    void connectViewportSignals(ViewportWidget* viewport);
    
    // Synchronization
    void synchronizeViewportSettings(ViewportWidget* sourceViewport);
};

/**
 * @brief Viewport navigation widget (view cube)
 * 
 * Interactive 3D cube for viewport navigation, similar to those found
 * in professional CAD applications like AutoCAD and 3ds Max.
 */
class ViewCube : public QWidget
{
    Q_OBJECT
    
public:
    explicit ViewCube(QWidget* parent = nullptr);
    
    void setCameraController(ICameraController* controller);
    
signals:
    void viewChangeRequested(ViewportWidget::ViewType view);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    
private:
    ICameraController* m_cameraController;
    QPoint m_lastMousePos;
    bool m_isDragging;
    
    void drawCube(QPainter& painter);
    ViewportWidget::ViewType getViewFromPosition(const QPoint& pos);
};
