#include "ViewportManager.h"
#include "ICameraController.h"
#include "MayaCameraController.h"
#include "input/EventDrivenCameraController.hpp"
#include "event/event_dispatcher.hpp"
#include "event/mouse_handler.hpp"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "core/entity.hpp"
#include "core/core_system.hpp"
#include "gizmo/gizmo_manager.hpp"
#include "rendering/core/RenderSystem.h"
#include "rendering/effects/LightingSystem.h"
#include "ui/viewport/GridSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QOpenGLFunctions>
#include <QDebug>

// ViewportWidget Implementation
ViewportWidget::ViewportWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_camera(std::make_shared<Camera>())
    , m_viewType(ViewType::Perspective)
    , m_viewName("Perspective")
    , m_isActive(false)
    , m_renderMode(RenderMode::Solid)
    , m_showGizmos(true)
{
    qDebug() << "ViewportWidget constructor called";
    setFocusPolicy(Qt::StrongFocus);
    
    // Initialize event system
    m_eventDispatcher = std::make_unique<event::EventDispatcher>();
    m_mouseHandler = std::make_unique<event::MouseHandler>(*m_eventDispatcher);
    m_eventCameraController = std::make_unique<input::EventDrivenCameraController>(*m_eventDispatcher);
    
    // Initialize gizmo manager with core system's selection manager
    m_gizmoManager = std::make_unique<GizmoManager>();
    // TODO: Fix GizmoManager to support setSelectionManager method
    // m_gizmoManager->setSelectionManager(CoreSystem::getInstance().getSelectionManager());
    
    // Connect event-driven camera controller to our camera
    m_eventCameraController->setCamera(m_camera);
    
    // Connect camera changed signal
    connect(m_eventCameraController.get(), &input::EventDrivenCameraController::cameraChanged,
            this, &ViewportWidget::cameraChanged);
    
    // Set professional default camera position (like Maya's persp view)
    // Position the camera to show the grid from a good angle
    glm::vec3 defaultCameraPos(10.0f, 8.0f, 10.0f);  // Further back and higher up
    m_camera->getTransform().setPosition(defaultCameraPos);
    m_camera->lookAt(glm::vec3(0, 0, 0));             // Look at origin

    // Connect to the core system's scene
    auto* sceneManager = CoreSystem::getInstance().getSceneManager();
    if (sceneManager && sceneManager->getScene()) {
        m_scene = sceneManager->getScene();
    }
    
    // Set the active gizmo type
    m_gizmoManager->setActiveGizmo(GizmoType::Translate);

    updateViewportTitle();
    qDebug() << "Event system initialized with dispatcher at:" << m_eventDispatcher.get();
}

ViewportWidget::~ViewportWidget() = default;

void ViewportWidget::setScene(std::shared_ptr<rude::Scene> scene)
{
    m_scene = scene;
    if (m_cameraController) {
        m_cameraController->setScene(scene);
    }
}

void ViewportWidget::setCameraController(std::unique_ptr<ICameraController> controller)
{
    m_cameraController = std::move(controller);
    if (m_cameraController) {
        m_cameraController->setCamera(m_camera);
        m_cameraController->setScene(m_scene);
        
        // Note: ICameraController needs to be updated to inherit from QObject
        // to support signals, for now we'll manually emit cameraChanged
    }
}

void ViewportWidget::setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem)
{
    m_lightingSystem = lightingSystem;
}

void ViewportWidget::setGridSystem(std::shared_ptr<GridSystem> gridSystem)
{
    m_gridSystem = gridSystem;
}

void ViewportWidget::setRenderSystem(std::shared_ptr<RenderSystem> renderSystem)
{
    m_renderSystem = renderSystem;
    if (m_renderSystem) {
        m_renderSystem->setCamera(m_camera);
        m_renderSystem->setScene(m_scene);
        m_renderSystem->setLightingSystem(m_lightingSystem);
        m_renderSystem->setGridSystem(m_gridSystem);
    }
}

void ViewportWidget::setViewType(ViewType type)
{
    if (m_viewType != type) {
        m_viewType = type;
        setupPredefinedView();
        updateViewportTitle();
        update();
    }
}

void ViewportWidget::setViewName(const QString& name)
{
    if (m_viewName != name) {
        m_viewName = name;
        updateViewportTitle();
        update();
    }
}

void ViewportWidget::setActive(bool active)
{
    if (m_isActive != active) {
        m_isActive = active;
        update(); // Redraw border
        
        if (active) {
            setFocus();
            emit viewportActivated(this);
        }
    }
}

void ViewportWidget::setRenderMode(RenderMode mode)
{
    if (m_renderMode != mode) {
        m_renderMode = mode;
        if (m_renderSystem) {
            m_renderSystem->setRenderMode(mode);
        }
        emit renderModeChanged(mode);
        update();
    }
}

void ViewportWidget::setShowGrid(bool show)
{
    if (m_gridSystem) {
        m_gridSystem->setVisible(show);
        update();
    }
}

bool ViewportWidget::isGridVisible() const
{
    return m_gridSystem ? m_gridSystem->isVisible() : false;
}

void ViewportWidget::setShowGizmos(bool show)
{
    if (m_showGizmos != show) {
        m_showGizmos = show;
        if (m_renderSystem) {
            m_renderSystem->setShowTransformGizmo(show);
        }
        update();
    }
}

void ViewportWidget::frameScene(bool animate)
{
    if (m_cameraController) {
        m_cameraController->frameScene(animate);
    }
}

void ViewportWidget::frameSelection(bool animate)
{
    if (m_cameraController) {
        m_cameraController->frameSelection(animate);
    }
}

void ViewportWidget::resetCamera()
{
    if (m_cameraController) {
        m_cameraController->resetCamera();
    }
}

void ViewportWidget::initializeGL()
{
    qDebug() << "ViewportWidget::initializeGL() called";
    initializeOpenGLFunctions();
    
    // Set professional viewport background (dark gray like Maya/Blender)
    glClearColor(0.13f, 0.13f, 0.13f, 1.0f); // Professional dark background
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    if (m_renderSystem) {
        qDebug() << "Initializing render system in viewport";
        m_renderSystem->initialize();
    } else {
        qDebug() << "WARNING: No render system in viewport!";
    }
    
    qDebug() << "ViewportWidget OpenGL initialization complete";
}

void ViewportWidget::resizeGL(int w, int h)
{
    qDebug() << "ViewportWidget::resizeGL() called - Size:" << w << "x" << h;
    
    if (m_camera) {
        m_camera->setAspectRatio(static_cast<float>(w) / static_cast<float>(h));
    }
    
    if (m_cameraController) {
        m_cameraController->updateAspectRatio(static_cast<float>(w) / static_cast<float>(h));
    }
    
    if (m_renderSystem) {
        m_renderSystem->setViewportSize(w, h);
    }
}

void ViewportWidget::paintGL()
{
    qDebug() << "ViewportWidget::paintGL() called, render system:" << (m_renderSystem ? "available" : "NULL");
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (m_renderSystem) {
        qDebug() << "Calling renderSystem->render()";
        m_renderSystem->render();
        qDebug() << "RenderSystem render completed";
    } else {
        qDebug() << "ERROR: No render system available for rendering!";
    }

    // Draw Gizmo
    if (m_gizmoManager && m_showGizmos) {
        // This is a temporary solution. The camera object needs to be consistent.
        // We will need to refactor the Camera class to be independent of Qt.
        Camera tempCam = *m_camera; // This will slice, but works for now
        m_gizmoManager->draw(tempCam);
    }

    // Paint viewport overlay
    paintViewportOverlay();
    
    qDebug() << "ViewportWidget::paintGL() completed";
}

void ViewportWidget::mousePressEvent(QMouseEvent* event)
{
    setActive(true);

    // Let the gizmo manager handle the event first
    if (m_gizmoManager) {
        // This is temporary. We need a proper way to pass the camera.
        Camera tempCam = *m_camera;
        // TODO: Fix event type conversion for GizmoManager
        // event::MouseButtonPressedEvent e(event->button(), event->x(), event->y(), event->modifiers());
        // if (m_gizmoManager->handleMousePress(e, tempCam))
        // {
        //     update();
        //     return;
        // }
        
        // Check for selection via CoreSystem if gizmo didn't handle it
        auto* selectionManager = CoreSystem::getInstance().getSelectionManager();
        if (selectionManager && event->button() == Qt::LeftButton) {
            // Determine selection mode based on modifiers
            rude::SelectionMode mode = rude::SelectionMode::Replace;
            if (event->modifiers() & Qt::ControlModifier) {
                if (event->modifiers() & Qt::ShiftModifier) {
                    mode = rude::SelectionMode::Subtract;
                } else {
                    mode = rude::SelectionMode::Add;
                }
            } else if (event->modifiers() & Qt::ShiftModifier) {
                mode = rude::SelectionMode::Toggle;
            }
            
            // Perform selection - this is a placeholder for actual ray-casting selection
            bool selectionHandled = performSelection(event->x(), event->y(), mode);
            
            if (selectionHandled) {
                // Notify that selection has changed
                emit selectionChanged();
                update();
                return;
            }
        }
    }

    // Process through event system for camera
    if (m_mouseHandler) {
        m_mouseHandler->handleMousePress(event);
    }
    
    // Fallback to legacy camera controller if event wasn't handled
    if (m_cameraController.get()) {
        // TODO: Fix MouseEvent conversion - ICameraController expects MouseEvent but we have QMouseEvent*
        // if (m_cameraController->handleMousePress(event)) {
        //     return;
        // }
    }
    
    QOpenGLWidget::mousePressEvent(event);
}

void ViewportWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Let the gizmo manager handle the event first
    if (m_gizmoManager) {
        // This is a temporary. We need a proper way to pass the camera.
        Camera tempCam = *m_camera;
        event::MouseMoveEvent e(event);
        if (m_gizmoManager->handleMouseMove(e, tempCam))
        {
            update();
            return;
        }
    }

    // Process through event system for camera
    if (m_mouseHandler) {
        m_mouseHandler->handleMouseMove(event);
    }
    
    // Fallback to legacy camera controller if event wasn't handled
    if (m_cameraController.get()) {
        // TODO: Fix MouseEvent conversion - ICameraController expects MouseEvent but we have QMouseEvent*
        // if (m_cameraController->handleMouseMove(event)) {
        //     update();
        //     return;
        // }
    }
    
    QOpenGLWidget::mouseMoveEvent(event);
}

void ViewportWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // Let the gizmo manager handle the event first
    if (m_gizmoManager) {
        // This is a temporary. We need a proper way to pass the camera.
        Camera tempCam = *m_camera;
        event::MouseReleaseEvent e(event);
        if (m_gizmoManager->handleMouseRelease(e, tempCam))
        {
            update();
            return;
        }
    }

    // Process through event system for camera
    if (m_mouseHandler) {
        m_mouseHandler->handleMouseRelease(event);
    }
    
    // Fallback to legacy camera controller if event wasn't handled
    if (m_cameraController.get()) {
        // TODO: Fix MouseEvent conversion - ICameraController expects MouseEvent but we have QMouseEvent*
        // if (m_cameraController->handleMouseRelease(event)) {
        //     return;
        // }
    }
    
    QOpenGLWidget::mouseReleaseEvent(event);
}

void ViewportWidget::wheelEvent(QWheelEvent* event)
{
    // Process through event system first
    if (m_mouseHandler) {
        m_mouseHandler->handleWheel(event);
    }
    
    // Fallback to legacy camera controller if event wasn't handled
    if (m_cameraController.get()) {
        // TODO: Fix WheelEvent conversion - ICameraController expects WheelEvent but we have QWheelEvent*
        // if (m_cameraController->handleWheel(event)) {
        //     update();
        //     return;
        // }
    }
    
    QOpenGLWidget::wheelEvent(event);
}

void ViewportWidget::keyPressEvent(QKeyEvent* event)
{
    if (m_cameraController.get()) {
        // TODO: Fix KeyEvent conversion - ICameraController expects KeyEvent but we have QKeyEvent*
        // if (m_cameraController->handleKeyPress(event)) {
        //     update();
        //     return;
        // }
    }
    
    QOpenGLWidget::keyPressEvent(event);
}

void ViewportWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (m_cameraController.get()) {
        // TODO: Fix KeyEvent conversion - ICameraController expects KeyEvent but we have QKeyEvent*
        // if (m_cameraController->handleKeyRelease(event)) {
        //     return;
        // }
    }
    
    QOpenGLWidget::keyReleaseEvent(event);
}

void ViewportWidget::focusInEvent(QFocusEvent* event)
{
    setActive(true);
    QOpenGLWidget::focusInEvent(event);
}

void ViewportWidget::paintEvent(QPaintEvent* event)
{
    QOpenGLWidget::paintEvent(event);
}

void ViewportWidget::paintViewportOverlay()
{
    // This would be implemented to draw overlays using QPainter
    // For now, we'll use OpenGL-based rendering
    drawViewportBorder();
    drawViewportLabel();
}

void ViewportWidget::drawViewportBorder()
{
    if (!m_isActive) return;
    
    // Draw active viewport border using OpenGL
    glDisable(GL_DEPTH_TEST);
    glLineWidth(2.0f);
    
    // Draw border (this is a simplified version)
    // In a real implementation, you'd use proper shader rendering
    
    glEnable(GL_DEPTH_TEST);
}

void ViewportWidget::drawViewportLabel()
{
    // This would draw the viewport name in the top-left corner
    // For now, we'll rely on the window title
}

void ViewportWidget::drawViewportStats()
{
    // This would draw viewport statistics (polygon count, etc.)
    // in the bottom-left corner
}

void ViewportWidget::setupPredefinedView()
{
    if (!m_camera) return;
    
    glm::vec3 position, target, up;
    
    switch (m_viewType) {
        case ViewType::Top:
            position = glm::vec3(0, 10, 0);
            target = glm::vec3(0, 0, 0);
            up = glm::vec3(0, 0, -1);
            break;
            
        case ViewType::Front:
            position = glm::vec3(0, 0, 10);
            target = glm::vec3(0, 0, 0);
            up = glm::vec3(0, 1, 0);
            break;
            
        case ViewType::Right:
            position = glm::vec3(10, 0, 0);
            target = glm::vec3(0, 0, 0);
            up = glm::vec3(0, 1, 0);
            break;
            
        case ViewType::Perspective:
        default:
            position = glm::vec3(7, 7, 7);
            target = glm::vec3(0, 0, 0);
            up = glm::vec3(0, 1, 0);
            break;
    }
    
    m_camera->getTransform().setPosition(position);
    m_camera->lookAt(target, up);
    
    emit cameraChanged();
}

void ViewportWidget::updateViewportTitle()
{
    QString title = QString("%1 View").arg(m_viewName);
    setWindowTitle(title);
}

// ViewportManager Implementation
ViewportManager::ViewportManager(QWidget* parent)
    : QWidget(parent)
    , m_currentLayout(LayoutType::Single)
    , m_gridLayout(new QGridLayout(this))
    , m_activeViewport(nullptr)
    , m_controllerType("Maya")
    , m_synchronizeViews(false)
    , m_synchronizeSelection(true)
{
    m_gridLayout->setSpacing(1);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    
    // Create initial single viewport
    createSingleLayout();
}

ViewportManager::~ViewportManager() = default;

void ViewportManager::setLayout(LayoutType layout)
{
    if (m_currentLayout == layout) return;
    
    m_currentLayout = layout;
    clearLayout();
    
    switch (layout) {
        case LayoutType::Single:
            createSingleLayout();
            break;
        case LayoutType::Quad:
            createQuadLayout();
            break;
        case LayoutType::Triple:
            createTripleLayout();
            break;
        case LayoutType::Dual:
            createDualLayout();
            break;
        default:
            createSingleLayout();
            break;
    }
    
    emit layoutChanged(layout);
    emit viewportCountChanged(m_viewports.size());
}

ViewportWidget* ViewportManager::getViewport(int index) const
{
    if (index >= 0 && index < m_viewports.size()) {
        return m_viewports[index];
    }
    return nullptr;
}

void ViewportManager::setScene(std::shared_ptr<rude::Scene> scene)
{
    m_scene = scene;
    for (auto* viewport : m_viewports) {
        viewport->setScene(scene);
    }
    
    // Also update the CoreSystem's scene manager if appropriate
    auto* sceneManager = CoreSystem::getInstance().getSceneManager();
    if (sceneManager) {
        sceneManager->setScene(scene);
    }
}

void ViewportManager::setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem)
{
    m_lightingSystem = lightingSystem;
    for (auto* viewport : m_viewports) {
        viewport->setLightingSystem(lightingSystem);
    }
}

void ViewportManager::setGridSystem(std::shared_ptr<GridSystem> gridSystem)
{
    m_gridSystem = gridSystem;
    for (auto* viewport : m_viewports) {
        viewport->setGridSystem(gridSystem);
    }
}

void ViewportManager::setRenderSystem(std::shared_ptr<RenderSystem> renderSystem)
{
    m_renderSystem = renderSystem;
    for (auto* viewport : m_viewports) {
        viewport->setRenderSystem(renderSystem);
    }
}

void ViewportManager::setCameraControllerType(const QString& controllerType)
{
    if (m_controllerType == controllerType) return;
    
    m_controllerType = controllerType;
    
    // Update all viewports with new controller type
    for (auto* viewport : m_viewports) {
        // Create new controller based on type
        std::unique_ptr<ICameraController> controller;
        
        if (controllerType == "Maya") {
            controller = std::make_unique<MayaCameraController>();
        }
        // TODO: Add other controller types as they're implemented
        
        if (controller) {
            viewport->setCameraController(std::move(controller));
        }
    }
}

void ViewportManager::frameSceneAll(bool animate)
{
    for (auto* viewport : m_viewports) {
        viewport->frameScene(animate);
    }
}

void ViewportManager::frameSelectionAll(bool animate)
{
    for (auto* viewport : m_viewports) {
        viewport->frameSelection(animate);
    }
}

void ViewportManager::resetAllCameras()
{
    for (auto* viewport : m_viewports) {
        viewport->resetCamera();
    }
}

void ViewportManager::setGlobalRenderMode(RenderMode mode)
{
    for (auto* viewport : m_viewports) {
        viewport->setRenderMode(mode);
    }
}

void ViewportManager::setGridVisibilityAll(bool visible)
{
    for (auto* viewport : m_viewports) {
        viewport->setShowGrid(visible);
    }
}

void ViewportManager::setGizmosVisibilityAll(bool visible)
{
    for (auto* viewport : m_viewports) {
        viewport->setShowGizmos(visible);
    }
}

void ViewportManager::onViewportActivated(ViewportWidget* viewport)
{
    if (m_activeViewport != viewport) {
        // Deactivate previous viewport
        if (m_activeViewport) {
            m_activeViewport->setActive(false);
        }
        
        m_activeViewport = viewport;
        emit activeViewportChanged(viewport);
        
        // Synchronize views if enabled
        if (m_synchronizeViews) {
            synchronizeViewportSettings(viewport);
        }
    }
}

void ViewportManager::onViewportCameraChanged()
{
    // Handle camera synchronization if enabled
    if (m_synchronizeViews && m_activeViewport) {
        synchronizeViewportSettings(m_activeViewport);
    }
}

void ViewportManager::createSingleLayout()
{
    auto* viewport = createViewport(ViewportWidget::ViewType::Perspective, "Perspective");
    m_gridLayout->addWidget(viewport, 0, 0);
    
    if (!m_activeViewport) {
        viewport->setActive(true);
        m_activeViewport = viewport;
    }
}

void ViewportManager::createQuadLayout()
{
    auto* perspectiveView = createViewport(ViewportWidget::ViewType::Perspective, "Perspective");
    auto* topView = createViewport(ViewportWidget::ViewType::Top, "Top");
    auto* frontView = createViewport(ViewportWidget::ViewType::Front, "Front");
    auto* rightView = createViewport(ViewportWidget::ViewType::Right, "Right");
    
    m_gridLayout->addWidget(perspectiveView, 0, 0);
    m_gridLayout->addWidget(topView, 0, 1);
    m_gridLayout->addWidget(frontView, 1, 0);
    m_gridLayout->addWidget(rightView, 1, 1);
    
    if (!m_activeViewport) {
        perspectiveView->setActive(true);
        m_activeViewport = perspectiveView;
    }
}

void ViewportManager::createTripleLayout()
{
    auto* perspectiveView = createViewport(ViewportWidget::ViewType::Perspective, "Perspective");
    auto* topView = createViewport(ViewportWidget::ViewType::Top, "Top");
    auto* frontView = createViewport(ViewportWidget::ViewType::Front, "Front");
    
    m_gridLayout->addWidget(perspectiveView, 0, 0, 2, 1); // Span 2 rows
    m_gridLayout->addWidget(topView, 0, 1);
    m_gridLayout->addWidget(frontView, 1, 1);
    
    if (!m_activeViewport) {
        perspectiveView->setActive(true);
        m_activeViewport = perspectiveView;
    }
}

void ViewportManager::createDualLayout()
{
    auto* perspectiveView = createViewport(ViewportWidget::ViewType::Perspective, "Perspective");
    auto* topView = createViewport(ViewportWidget::ViewType::Top, "Top");
    
    m_gridLayout->addWidget(perspectiveView, 0, 0);
    m_gridLayout->addWidget(topView, 0, 1);
    
    if (!m_activeViewport) {
        perspectiveView->setActive(true);
        m_activeViewport = perspectiveView;
    }
}

void ViewportManager::clearLayout()
{
    // Remove all viewports from layout
    for (auto* viewport : m_viewports) {
        m_gridLayout->removeWidget(viewport);
        viewport->deleteLater();
    }
    
    m_viewports.clear();
    m_activeViewport = nullptr;
}

ViewportWidget* ViewportManager::createViewport(ViewportWidget::ViewType type, const QString& name)
{
    auto* viewport = new ViewportWidget(this);
    viewport->setViewType(type);
    viewport->setViewName(name);
    
    setupViewportDefaults(viewport);
    connectViewportSignals(viewport);
    
    m_viewports.append(viewport);
    
    return viewport;
}

void ViewportManager::setupViewportDefaults(ViewportWidget* viewport)
{
    // Apply global systems
    viewport->setScene(m_scene);
    viewport->setLightingSystem(m_lightingSystem);
    viewport->setGridSystem(m_gridSystem);
    viewport->setRenderSystem(m_renderSystem);
    
    // Create camera controller
    std::unique_ptr<ICameraController> controller;
    if (m_controllerType == "Maya") {
        controller = std::make_unique<MayaCameraController>();
    }
    
    if (controller) {
        viewport->setCameraController(std::move(controller));
    }
}

void ViewportManager::connectViewportSignals(ViewportWidget* viewport)
{
    connect(viewport, &ViewportWidget::viewportActivated,
            this, &ViewportManager::onViewportActivated);
    connect(viewport, &ViewportWidget::cameraChanged,
            this, &ViewportManager::onViewportCameraChanged);
}

void ViewportManager::synchronizeViewportSettings(ViewportWidget* sourceViewport)
{
    // This would implement view synchronization logic
    Q_UNUSED(sourceViewport)
    // TODO: Implement camera position synchronization
}

// ViewCube Implementation (simplified)
ViewCube::ViewCube(QWidget* parent)
    : QWidget(parent)
    , m_cameraController(nullptr)
    , m_isDragging(false)
{
    setFixedSize(100, 100);
}

void ViewCube::setCameraController(ICameraController* controller)
{
    m_cameraController = controller;
}

void ViewCube::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    drawCube(painter);
}

void ViewCube::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePos = event->pos();
    m_isDragging = true;
}

void ViewCube::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_isDragging) {
        ViewportWidget::ViewType view = getViewFromPosition(event->pos());
        emit viewChangeRequested(view);
    }
    
    m_isDragging = false;
}

void ViewCube::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging && m_cameraController) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();
        
        // Rotate the view cube visualization
        update();
    }
}

void ViewCube::drawCube(QPainter& painter)
{
    // Simplified cube drawing
    painter.fillRect(rect(), QColor(100, 100, 100));
    painter.setPen(Qt::white);
    painter.drawRect(rect());
    
    // Draw face labels
    painter.drawText(rect(), Qt::AlignCenter, "View\nCube");
}

ViewportWidget::ViewType ViewCube::getViewFromPosition(const QPoint& pos)
{
    // Simplified view detection based on position
    Q_UNUSED(pos)
    return ViewportWidget::ViewType::Perspective;
}

void ViewCube::enterEvent(QEnterEvent* event)
{
    // TODO: Handle mouse enter for ViewCube highlighting
    Q_UNUSED(event)
    update(); // Trigger repaint for hover effect
}

void ViewCube::leaveEvent(QEvent* event)
{
    // TODO: Handle mouse leave for ViewCube unhighlighting  
    Q_UNUSED(event)
    update(); // Trigger repaint to remove hover effect
}

bool ViewportWidget::performSelection(int x, int y, rude::SelectionMode mode)
{
    // This is a placeholder implementation for selection
    // In a real implementation, this would involve:
    // 1. Ray casting from screen coordinates into 3D space
    // 2. Intersection testing with scene geometry
    // 3. Updating the selection manager with results
    
    auto* selectionManager = CoreSystem::getInstance().getSelectionManager();
    if (!selectionManager || !m_scene) {
        return false;
    }
    
    // For now, we'll just demonstrate the selection system integration
    // TODO: Implement proper ray-casting and intersection testing
    
    qDebug() << "Performing selection at" << x << "," << y << "with mode" << static_cast<int>(mode);
    
    // Placeholder: clear selection when clicking in empty space
    if (mode == rude::SelectionMode::Replace) {
        selectionManager->clearSelection();
        qDebug() << "Cleared selection";
        return true;
    }
    
    return false;
}

void ViewportWidget::updateSelectionDisplay()
{
    // Update viewport display to reflect current selection
    update();
}
