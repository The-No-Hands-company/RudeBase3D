#include "Viewport3D.h"
#include "core/scene.hpp"
#include "SceneObject.h"
#include "Camera.h"
#include "Renderer.h"
#include "ICameraController.h"
#include "InputController.h"
#include "SelectionManager.h"
#include "LightingSystem.h"
#include "GridSystem.h"
#include "Mesh.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>
#include <glm/gtc/matrix_transform.hpp>

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

void Viewport3D::setScene(std::shared_ptr<rude::Scene> scene)
{
    m_scene = scene;
    
    // Set scene on camera controller for framing operations (if available)
    if (m_cameraController) {
        m_cameraController->setScene(scene);
    }
    
    // TODO: Implement selection changed signal for rude::Scene
    // if (m_scene) {
    //     connect(m_scene.get(), &rude::Scene::selectionChanged, this, &Viewport3D::objectSelected);
    // }
    
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
        m_cameraController->frameSelection();
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
    
    if (m_cameraController) {
        m_cameraController->updateAspectRatio(static_cast<float>(w) / static_cast<float>(h));
    }
}

void Viewport3D::paintGL()
{
    // Don't clear here - let the renderer handle it
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_renderer->beginFrame();
    
    // Update camera and renderer matrices
    if (m_cameraController) {
        m_renderer->setViewMatrix(m_cameraController->getViewMatrix());
        m_renderer->setProjectionMatrix(m_cameraController->getProjectionMatrix());
    }
    
    // Set camera position for lighting calculations
    const glm::vec3& cameraGlmPos = m_camera->getTransform().getPosition();
    QVector3D cameraPos(cameraGlmPos.x, cameraGlmPos.y, cameraGlmPos.z);
    m_renderer->setViewPosition(cameraGlmPos);
    
    // Apply lighting from LightingSystem
    if (m_lightingSystem) {
        m_lightingSystem->applyLighting(m_renderer, cameraGlmPos);
    }
    
    // Render grid using GridSystem
    if (m_gridSystem && m_gridSystem->isVisible() && m_cameraController) {
        m_gridSystem->render(m_renderer, m_cameraController->getViewMatrix(), m_cameraController->getProjectionMatrix());
    }
    
    // Render scene
    if (m_scene) {
        renderScene();
    }
    
    // Render selection visualization
    renderSelection();
    
    // TODO: Implement selection management for rude::Scene
    // The rude::Scene API doesn't have getSelectedObject() method
    // Render transform gizmo for selected object
    // if (m_showTransformGizmo && m_scene && m_scene->getSelectedObject()) {
    //     renderTransformGizmo();
    // }
    
    m_renderer->endFrame();
}

void Viewport3D::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "Viewport3D::mousePressEvent - Button:" << event->button() << "Modifiers:" << event->modifiers();
    if (m_inputController) {
        // TODO: Fix BlenderCameraController integration
        // Convert QMouseEvent to standard types for BlenderCameraController
        // auto blenderController = std::dynamic_pointer_cast<BlenderCameraController>(m_cameraController);
        // if (blenderController) {
        //     glm::ivec2 pos(event->pos().x(), event->pos().y());
        //     int button = static_cast<int>(event->button());
        //     blenderController->handleMousePress(pos, button);
        // } else {
        //     m_inputController->handleMousePress(event);
        // }
        qDebug() << "Input controller would handle mouse press event (disabled)";
    }
    setFocus();
}

void Viewport3D::mouseMoveEvent(QMouseEvent* event)
{
    if (m_inputController) {
        // TODO: Fix BlenderCameraController integration
        // auto blenderController = std::dynamic_pointer_cast<BlenderCameraController>(m_cameraController);
        // if (blenderController) {
        //     glm::ivec2 pos(event->pos().x(), event->pos().y());
        //     blenderController->handleMouseMove(pos);
        // } else {
        //     m_inputController->handleMouseMove(event);
        // }
        // Update is handled by InputController signals or CameraController changes
    }
}

void Viewport3D::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_inputController) {
        // TODO: Fix BlenderCameraController integration
        // auto blenderController = std::dynamic_pointer_cast<BlenderCameraController>(m_cameraController);
        // if (blenderController) {
        //     glm::ivec2 pos(event->pos().x(), event->pos().y());
        //     int button = static_cast<int>(event->button());
        //     blenderController->handleMouseRelease(pos, button);
        // } else {
        //     m_inputController->handleMouseRelease(event);
        // }
    }
}

void Viewport3D::wheelEvent(QWheelEvent* event)
{
    if (m_inputController) {
        // TODO: Fix BlenderCameraController integration
        // auto blenderController = std::dynamic_pointer_cast<BlenderCameraController>(m_cameraController);
        // if (blenderController) {
        //     float delta = static_cast<float>(event->angleDelta().y());
        //     blenderController->handleWheel(delta);
        // } else {
        //     m_inputController->handleWheel(event);
        // }
    }
}

void Viewport3D::keyPressEvent(QKeyEvent* event)
{
    if (m_inputController) {
        // TODO: Fix BlenderCameraController integration
        // auto blenderController = std::dynamic_pointer_cast<BlenderCameraController>(m_cameraController);
        // if (blenderController) {
        //     int key = event->key();
        //     int modifiers = static_cast<int>(event->modifiers());
        //     blenderController->handleKeyPress(key, modifiers);
        // } else {
        //     m_inputController->handleKeyPress(event);
        // }
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
    
    // TODO: Implement scene rendering for rude::Scene API
    // The rude::Scene API doesn't have getObjects() method
    // Available methods: getEntities(), getAllEntities(), getRootEntities()
    
    // Render all entities in the scene
    auto entities = m_scene->getAllEntities();
    for (auto entity : entities) {
        if (!entity) {
            continue;
        }
        // TODO: Check visibility when Entity has isVisible() method
        // if (!entity->isVisible()) {
        //     continue;
        // }
        
        // TODO: Implement entity rendering for rude::Entity API
        // The rude::Entity API is different from SceneObject API
        // Need to implement proper rendering methods for Entity
        /*
        // Set model matrix
        m_renderer->setModelMatrix(entity->getTransform().getModelMatrix());
        
        // Set material
        m_renderer->setMaterial(entity->getMaterial());
        
        // Render mesh
        m_renderer->renderMesh(entity->getMesh(), m_renderMode);
        
        // Render bounding box for selected object
        if (entity->isSelected()) {
            QVector3D min = entity->getBoundingBoxMin();
            QVector3D max = entity->getBoundingBoxMax();
            
            // Transform to world space
            glm::mat4 modelMatrix = entity->getTransform().getModelMatrix();
            m_renderer->setModelMatrix(glm::mat4(1.0f)); // Use identity for world space lines
            // TODO: Convert QVector3D to glm::vec3 for renderAABB call
            // m_renderer->renderAABB(min, max, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        }
        */
    }
}

void Viewport3D::renderTransformGizmo()
{
    // TODO: Implement transform gizmo for rude::Scene API
    // The rude::Scene API doesn't have getSelectedObject() method
    return;
    
    /*
    if (!m_scene) {
        return;
    }
    
    auto selectedObject = m_scene->getSelectedObject();
    if (!selectedObject) {
        return;
    }
    
    const glm::vec3& objectGlmPos = selectedObject->getTransform().getPosition();
    QVector3D objectPos(objectGlmPos.x, objectGlmPos.y, objectGlmPos.z);
    
    // Set model matrix to object position
    glm::mat4 gizmoMatrix = glm::translate(glm::mat4(1.0f), objectGlmPos);
    m_renderer->setModelMatrix(gizmoMatrix);
    
    // Render transform gizmo axes
    float axisLength = 1.5f;
    
    // X axis (red)
    m_renderer->renderLine(glm::vec3(0, 0, 0), glm::vec3(axisLength, 0, 0), glm::vec4(1, 0, 0, 1));
    
    // Y axis (green)
    m_renderer->renderLine(glm::vec3(0, 0, 0), glm::vec3(0, axisLength, 0), glm::vec4(0, 1, 0, 1));
    
    // Z axis (blue)
    m_renderer->renderLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, axisLength), glm::vec4(0, 0, 1, 1));
    */
}

void Viewport3D::setCameraController(std::shared_ptr<ICameraController> controller)
{
    m_cameraController = controller;
    
    if (m_cameraController) {
        // Set up camera controller
        m_cameraController->setCamera(m_camera);
        // TODO: Fix BlenderCameraController integration
        // Set cross-platform camera changed callback for BlenderCameraController
        // auto blenderController = std::dynamic_pointer_cast<BlenderCameraController>(m_cameraController);
        // if (blenderController) {
        //     blenderController->cameraChangedCallback = [this]() {
        //         qDebug() << "Viewport3D::cameraChanged callback received - triggering update()";
        //         update();
        //     };
        // }
        // Initialize camera to industry-standard isometric view using camera controller
        m_cameraController->resetCamera();
        qDebug() << "Camera controller set up and reset to default position";
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
    
    // TODO: Implement selection rendering for rude::Scene API
    // The rude::Scene API doesn't have getSelectedObject() method
    // Need to implement selection management in Scene or use SelectionManager
    /*
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
    */
    
    // Temporary return until selection system is implemented
    return;
    
    // Selection colors
    const glm::vec4 vertexColor(1.0f, 0.5f, 0.0f, 1.0f);  // Orange for vertices
    const glm::vec4 edgeColor(0.0f, 1.0f, 0.0f, 1.0f);    // Green for edges  
    const glm::vec4 faceColor(0.0f, 0.5f, 1.0f, 0.8f);    // Blue for faces
    
    SelectionType selectionType = m_selectionManager->getSelectionType();
    
    // Render selected vertices
    if (selectionType == SelectionType::Vertex) {
        auto selectedVertices = m_selectionManager->getSelectedVertices();
        for (auto vertex : selectedVertices) {
            if (vertex) {
                glm::vec3 pos(vertex->position.x, vertex->position.y, vertex->position.z);
                float size = 0.05f; // Vertex highlight size
                
                // Draw a small cross/point to highlight the vertex
                m_renderer->renderLine(pos + glm::vec3(-size, 0, 0), pos + glm::vec3(size, 0, 0), vertexColor);
                m_renderer->renderLine(pos + glm::vec3(0, -size, 0), pos + glm::vec3(0, size, 0), vertexColor);
                m_renderer->renderLine(pos + glm::vec3(0, 0, -size), pos + glm::vec3(0, 0, size), vertexColor);
            }
        }
    }
    
    // Render selected edges
    else if (selectionType == SelectionType::Edge) {
        auto selectedEdges = m_selectionManager->getSelectedEdges();
        for (auto edge : selectedEdges) {
            // TODO: Fix edge rendering for rude:: API
            // Original implementation used getOriginVertex() and getTwin() which don't exist
            Q_UNUSED(edge);
            qDebug() << "Viewport3D: Edge selection rendering not yet implemented for rude:: API";
            /*
            if (edge && edge->getOriginVertex() && edge->getTwin() && edge->getTwin()->getOriginVertex()) {
                QVector3D start = edge->getOriginVertex()->getPosition();
                QVector3D end = edge->getTwin()->getOriginVertex()->getPosition();
                m_renderer->renderLine(start, end, edgeColor);
            }
            */
        }
    }
    
    // Render selected faces
    else if (selectionType == SelectionType::Face) {
        auto selectedFaces = m_selectionManager->getSelectedFaces();
        for (auto face : selectedFaces) {
            // TODO: Fix face rendering for rude:: API
            // Original implementation used getOuterEdge() which doesn't exist
            Q_UNUSED(face);
            qDebug() << "Viewport3D: Face selection rendering not yet implemented for rude:: API";
            /*
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
            */
        }
    }
    
    // Reset renderer state
    m_renderer->enableDepthTest(true);
    m_renderer->setLineWidth(1.0f);
}

void Viewport3D::setInputController(std::shared_ptr<InputController> controller)
{
    m_inputController = controller;
    
    if (m_inputController) {
        // Set up input controller dependencies
        m_inputController->setViewport(this);
        if (m_scene) {
            m_inputController->setScene(m_scene);
        }
        if (m_selectionManager) {
            m_inputController->setSelectionManager(m_selectionManager);
        }
        
        qDebug() << "Input controller set up with dependencies";
    }
}
