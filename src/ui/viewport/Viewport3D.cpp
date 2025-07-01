#include "Viewport3D.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Camera.h"
#include "Renderer.h"
#include "CameraController.h"
#include "InputController.h"
#include "SelectionManager.h"
#include "LightingSystem.h"
#include "GridSystem.h"
#include "Mesh.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>

Viewport3D::Viewport3D(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_camera(std::make_shared<Camera>())
    , m_renderer(std::make_shared<Renderer>())
    , m_selectionManager(std::make_shared<SelectionManager>())
    , m_renderMode(RenderMode::Solid)
    , m_transformMode(TransformMode::Select)
    , m_showTransformGizmo(true)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

Viewport3D::~Viewport3D()
{
    makeCurrent();
    // Cleanup will be handled by the renderer destructor
    doneCurrent();
}

void Viewport3D::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
    
    // Set scene on camera controller for framing operations (if available)
    if (m_cameraController) {
        m_cameraController->setScene(scene);
    }
    
    // Set scene on input controller for object selection (if available)
    if (m_inputController) {
        m_inputController->setScene(scene);
    }
    
    if (m_scene) {
        connect(m_scene.get(), &Scene::selectionChanged, this, &Viewport3D::objectSelected);
    }
    
    update();
}

void Viewport3D::setRenderMode(RenderMode mode)
{
    m_renderMode = mode;
    update();
}

void Viewport3D::setTransformMode(TransformMode mode)
{
    m_transformMode = mode;
    emit transformModeChanged(mode);
    update();
}

void Viewport3D::resetCamera()
{
    if (m_cameraController) {
        m_cameraController->resetCamera();
    }
}

void Viewport3D::frameScene()
{
    if (m_cameraController) {
        m_cameraController->frameScene();
    }
}

void Viewport3D::frameSelectedObject()
{
    if (m_cameraController) {
        m_cameraController->frameSelectedObject();
    }
}

void Viewport3D::setShowGrid(bool show)
{
    if (m_gridSystem) {
        m_gridSystem->setVisible(show);
    }
    update();
}

bool Viewport3D::isGridVisible() const
{
    return m_gridSystem ? m_gridSystem->isVisible() : false;
}

void Viewport3D::initializeGL()
{
    initializeOpenGLFunctions();
    
    if (!m_renderer->initialize()) {
        qDebug() << "Failed to initialize renderer";
        return;
    }
    
    // Industry standard viewport background (Blender/Maya style)
    glClearColor(0.13f, 0.13f, 0.13f, 1.0f); // Dark gray #212121
    
    // Enable proper OpenGL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // Note: Alpha blending is now controlled per-material in Renderer::setMaterial()
    
    // Enable line smoothing for better grid appearance
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    qDebug() << "OpenGL initialized successfully";
    qDebug() << "OpenGL Version:" << (char*)glGetString(GL_VERSION);
    qDebug() << "GLSL Version:" << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void Viewport3D::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    
    m_cameraController->updateAspectRatio(static_cast<float>(w) / static_cast<float>(h));
}

void Viewport3D::paintGL()
{
    // Clear with background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_renderer->beginFrame();
    
    // Update camera and renderer matrices
    m_renderer->setViewMatrix(m_cameraController->getViewMatrix());
    m_renderer->setProjectionMatrix(m_cameraController->getProjectionMatrix());
    
    // Set camera position for lighting calculations
    QVector3D cameraPos = m_camera->getTransform().getPosition();
    m_renderer->setViewPosition(cameraPos);
    
    // Apply lighting from LightingSystem
    if (m_lightingSystem) {
        m_lightingSystem->applyLighting(m_renderer, cameraPos);
    }
    
    // Render grid using GridSystem
    if (m_gridSystem && m_gridSystem->isVisible()) {
        m_gridSystem->render(m_renderer, m_cameraController->getViewMatrix(), m_cameraController->getProjectionMatrix());
    }
    
    // Render scene
    if (m_scene) {
        renderScene();
    }
    
    // Render selection visualization
    renderSelection();
    
    // Render transform gizmo for selected object
    if (m_showTransformGizmo && m_scene && m_scene->getSelectedObject()) {
        renderTransformGizmo();
    }
    
    m_renderer->endFrame();
}

void Viewport3D::mousePressEvent(QMouseEvent* event)
{
    if (m_inputController) {
        m_inputController->handleMousePress(event);
    }
    setFocus();
}

void Viewport3D::mouseMoveEvent(QMouseEvent* event)
{
    if (m_inputController) {
        m_inputController->handleMouseMove(event);
    }
}

void Viewport3D::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_inputController) {
        m_inputController->handleMouseRelease(event);
    }
}

void Viewport3D::wheelEvent(QWheelEvent* event)
{
    if (m_inputController) {
        m_inputController->handleWheel(event);
    }
}

void Viewport3D::keyPressEvent(QKeyEvent* event)
{
    if (m_inputController) {
        m_inputController->handleKeyPress(event);
    }
}

void Viewport3D::updateCamera()
{
    if (width() > 0 && height() > 0 && m_cameraController) {
        m_cameraController->updateAspectRatio(static_cast<float>(width()) / static_cast<float>(height()));
    }
}

void Viewport3D::renderScene()
{
    if (!m_scene) {
        return;
    }
    
    // Render all objects in the scene
    for (const auto& object : m_scene->getObjects()) {
        if (!object->isVisible()) {
            continue;
        }
        
        // Set model matrix
        m_renderer->setModelMatrix(object->getTransform().getModelMatrix());
        
        // Set material
        m_renderer->setMaterial(object->getMaterial());
        
        // Render mesh
        m_renderer->renderMesh(object->getMesh(), m_renderMode);
        
        // Render bounding box for selected object
        if (object->isSelected()) {
            QVector3D min = object->getBoundingBoxMin();
            QVector3D max = object->getBoundingBoxMax();
            
            // Transform to world space
            QMatrix4x4 modelMatrix = object->getTransform().getModelMatrix();
            m_renderer->setModelMatrix(QMatrix4x4()); // Use identity for world space lines
            m_renderer->renderAABB(min, max, QVector4D(1.0f, 1.0f, 0.0f, 1.0f));
        }
    }
}

void Viewport3D::renderTransformGizmo()
{
    if (!m_scene) {
        return;
    }
    
    auto selectedObject = m_scene->getSelectedObject();
    if (!selectedObject) {
        return;
    }
    
    QVector3D objectPos = selectedObject->getTransform().getPosition();
    
    // Set model matrix to object position
    QMatrix4x4 gizmoMatrix;
    gizmoMatrix.translate(objectPos);
    m_renderer->setModelMatrix(gizmoMatrix);
    
    // Render transform gizmo axes
    float axisLength = 1.5f;
    
    // X axis (red)
    m_renderer->renderLine(QVector3D(0, 0, 0), QVector3D(axisLength, 0, 0), QVector4D(1, 0, 0, 1));
    
    // Y axis (green)
    m_renderer->renderLine(QVector3D(0, 0, 0), QVector3D(0, axisLength, 0), QVector4D(0, 1, 0, 1));
    
    // Z axis (blue)
    m_renderer->renderLine(QVector3D(0, 0, 0), QVector3D(0, 0, axisLength), QVector4D(0, 0, 1, 1));
}

void Viewport3D::setCameraController(std::shared_ptr<CameraController> controller)
{
    m_cameraController = controller;
    
    if (m_cameraController) {
        // Set up camera controller
        m_cameraController->setCamera(m_camera);
        
        // Connect camera controller signals
        connect(m_cameraController.get(), &CameraController::cameraChanged, this, [this]() {
            update();
        });
        
        // Initialize camera to industry-standard isometric view
        // Position camera at professional modeling angle (45 degrees from XZ plane)
        QVector3D defaultPosition(7.0f, 7.0f, 7.0f);
        QVector3D defaultTarget(0.0f, 0.0f, 0.0f);
        
        m_camera->getTransform().setPosition(defaultPosition);
        m_camera->lookAt(defaultTarget);
        
        // Update camera controller to match
        m_cameraController->resetCamera();
        
        qDebug() << "Camera positioned at:" << defaultPosition << "looking at:" << defaultTarget;
    }
}

void Viewport3D::setInputController(std::shared_ptr<InputController> controller)
{
    m_inputController = controller;
    
    if (m_inputController) {
        // Set up input controller
        m_inputController->setViewport(this);
        m_inputController->setCameraController(m_cameraController);
    }
}

void Viewport3D::setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem)
{
    m_lightingSystem = lightingSystem;
    
    if (m_lightingSystem) {
        // Connect to lighting changes for viewport updates
        connect(m_lightingSystem.get(), &LightingSystem::lightingChanged, this, [this]() {
            update();
        });
    }
}

void Viewport3D::setGridSystem(std::shared_ptr<GridSystem> gridSystem)
{
    m_gridSystem = gridSystem;
    
    if (m_gridSystem) {
        // Connect to grid changes for viewport updates
        connect(m_gridSystem.get(), &GridSystem::gridChanged, this, [this]() {
            update();
        });
        connect(m_gridSystem.get(), &GridSystem::visibilityChanged, this, [this](bool) {
            update();
        });
    }
}

void Viewport3D::setSelectionManager(std::shared_ptr<SelectionManager> selectionManager)
{
    m_selectionManager = selectionManager;
}

void Viewport3D::setSelectionType(SelectionType type)
{
    if (m_selectionManager) {
        m_selectionManager->setSelectionType(type);
        update(); // Refresh viewport to show selection mode change
    }
}

SelectionType Viewport3D::getSelectionType() const
{
    if (m_selectionManager) {
        return m_selectionManager->getSelectionType();
    }
    return SelectionType::Object; // Default fallback
}

void Viewport3D::renderSelection()
{
    if (!m_selectionManager || !m_scene) {
        return;
    }
    
    // Get the selected object and its mesh
    auto selectedObject = m_scene->getSelectedObject();
    if (!selectedObject) {
        return;
    }
    
    auto mesh = selectedObject->getMesh();
    if (!mesh) {
        return;
    }
    
    // Set up for rendering selection highlights
    m_renderer->setModelMatrix(selectedObject->getTransform().getModelMatrix());
    m_renderer->enableDepthTest(false); // Render on top
    m_renderer->setLineWidth(3.0f); // Thick lines for visibility
    
    // Selection colors
    const QVector4D vertexColor(1.0f, 0.5f, 0.0f, 1.0f);  // Orange for vertices
    const QVector4D edgeColor(0.0f, 1.0f, 0.0f, 1.0f);    // Green for edges  
    const QVector4D faceColor(0.0f, 0.5f, 1.0f, 0.8f);    // Blue for faces
    
    SelectionType selectionType = m_selectionManager->getSelectionType();
    
    // Render selected vertices
    if (selectionType == SelectionType::Vertex) {
        auto selectedVertices = m_selectionManager->getSelectedVertices();
        for (auto vertex : selectedVertices) {
            if (vertex) {
                QVector3D pos = vertex->getPosition();
                float size = 0.05f; // Vertex highlight size
                
                // Draw a small cross/point to highlight the vertex
                m_renderer->renderLine(pos + QVector3D(-size, 0, 0), pos + QVector3D(size, 0, 0), vertexColor);
                m_renderer->renderLine(pos + QVector3D(0, -size, 0), pos + QVector3D(0, size, 0), vertexColor);
                m_renderer->renderLine(pos + QVector3D(0, 0, -size), pos + QVector3D(0, 0, size), vertexColor);
            }
        }
    }
    
    // Render selected edges
    else if (selectionType == SelectionType::Edge) {
        auto selectedEdges = m_selectionManager->getSelectedEdges();
        for (auto edge : selectedEdges) {
            if (edge && edge->getOriginVertex() && edge->getTwin() && edge->getTwin()->getOriginVertex()) {
                QVector3D start = edge->getOriginVertex()->getPosition();
                QVector3D end = edge->getTwin()->getOriginVertex()->getPosition();
                m_renderer->renderLine(start, end, edgeColor);
            }
        }
    }
    
    // Render selected faces
    else if (selectionType == SelectionType::Face) {
        auto selectedFaces = m_selectionManager->getSelectedFaces();
        for (auto face : selectedFaces) {
            if (face && face->getOuterEdge()) {
                // Draw the face outline
                auto currentEdge = face->getOuterEdge();
                std::vector<QVector3D> faceVertices;
                
                do {
                    if (currentEdge->getOriginVertex()) {
                        faceVertices.push_back(currentEdge->getOriginVertex()->getPosition());
                    }
                    currentEdge = currentEdge->getNext();
                } while (currentEdge && currentEdge != face->getOuterEdge());
                
                // Draw face outline
                for (size_t i = 0; i < faceVertices.size(); ++i) {
                    size_t nextIndex = (i + 1) % faceVertices.size();
                    m_renderer->renderLine(faceVertices[i], faceVertices[nextIndex], faceColor);
                }
            }
        }
    }
    
    // Reset renderer state
    m_renderer->enableDepthTest(true);
    m_renderer->setLineWidth(1.0f);
}
