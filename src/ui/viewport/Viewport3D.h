#pragma once

#include "Common.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <memory>

// Forward declarations
namespace rude { class Scene; }
class Camera;
class Renderer;
class ICameraController;
class InputController;
class SelectionManager;
class LightingSystem;
class GridSystem;

class Viewport3D : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit Viewport3D(QWidget* parent = nullptr);
    ~Viewport3D();

    // Scene and camera
    void setScene(std::shared_ptr<rude::Scene> scene);
    std::shared_ptr<rude::Scene> getScene() const { return m_scene; }
    
    std::shared_ptr<Camera> getCamera() const { return m_camera; }
    
    // Rendering settings
    void setRenderMode(RenderMode mode);
    RenderMode getRenderMode() const { return m_renderMode; }
    
    void setTransformMode(TransformMode mode);
    TransformMode getTransformMode() const { return m_transformMode; }
    
    // Controllers and systems
    void setCameraController(std::shared_ptr<ICameraController> controller);
    void setInputController(std::shared_ptr<InputController> controller);
    void setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem);
    void setGridSystem(std::shared_ptr<GridSystem> gridSystem);
    
    std::shared_ptr<ICameraController> getCameraController() const { return m_cameraController; }
    std::shared_ptr<InputController> getInputController() const { return m_inputController; }
    std::shared_ptr<LightingSystem> getLightingSystem() const { return m_lightingSystem; }
    std::shared_ptr<GridSystem> getGridSystem() const { return m_gridSystem; }
    
    // Selection
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);
    std::shared_ptr<SelectionManager> getSelectionManager() const { return m_selectionManager; }
    void setSelectionType(SelectionType type);
    SelectionType getSelectionType() const;
    
    // Camera controls
    void resetCamera();
    void frameScene();
    void frameSelectedObject();
    
    // Grid and helpers (delegated to GridSystem)
    void setShowGrid(bool show);
    bool isGridVisible() const;

signals:
    void objectSelected(SceneObjectPtr object);
    void transformModeChanged(TransformMode mode);

protected:
    // OpenGL overrides
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    
    // Mouse and keyboard events
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    
private:
    std::shared_ptr<rude::Scene> m_scene;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<ICameraController> m_cameraController;
    std::shared_ptr<InputController> m_inputController;
    std::shared_ptr<SelectionManager> m_selectionManager;
    std::shared_ptr<LightingSystem> m_lightingSystem;
    std::shared_ptr<GridSystem> m_gridSystem;
    
    RenderMode m_renderMode;
    TransformMode m_transformMode;
    
    // Mouse interaction
    QPoint m_lastMousePos;
    Qt::MouseButton m_mouseButton;
    bool m_isDragging;
    
    // Transform gizmo
    bool m_showTransformGizmo;
    
    // Helper methods
    void updateCamera();
    
    // Rendering helpers
    void renderScene();
    void renderTransformGizmo();
    void renderSelection(); // Selection visualization
};
