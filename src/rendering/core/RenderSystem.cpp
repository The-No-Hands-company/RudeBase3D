#include "RenderSystem.h"
#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "SceneObject.h"
#include "Mesh.h"
#include "LightingSystem.h"
#include "GridSystem.h"
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QDebug>

RenderSystem::RenderSystem(QObject* parent)
    : QObject(parent)
    , m_renderer(std::make_shared<Renderer>())
    , m_renderMode(RenderMode::Solid)
    , m_showTransformGizmo(true)
    , m_viewportWidth(800)
    , m_viewportHeight(600)
{
}

RenderSystem::~RenderSystem()
{
    cleanup();
}

bool RenderSystem::initialize()
{
    if (!m_renderer->initialize()) {
        emit renderingError("Failed to initialize renderer");
        return false;
    }
    
    qDebug() << "RenderSystem initialized successfully";
    return true;
}

void RenderSystem::cleanup()
{
    // Systems will clean up themselves
}

void RenderSystem::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
}

void RenderSystem::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

void RenderSystem::setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem)
{
    m_lightingSystem = lightingSystem;
}

void RenderSystem::setGridSystem(std::shared_ptr<GridSystem> gridSystem)
{
    m_gridSystem = gridSystem;
}

void RenderSystem::setRenderMode(RenderMode mode)
{
    m_renderMode = mode;
}

void RenderSystem::setShowGrid(bool show)
{
    if (m_gridSystem) {
        m_gridSystem->setVisible(show);
    }
}

bool RenderSystem::isGridVisible() const
{
    return m_gridSystem ? m_gridSystem->isVisible() : false;
}

void RenderSystem::setShowTransformGizmo(bool show)
{
    m_showTransformGizmo = show;
}

void RenderSystem::setViewportSize(int width, int height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;
}

void RenderSystem::render()
{
    if (!m_renderer || !m_camera) return;
    
    m_renderer->beginFrame();
    
    // Update camera and renderer matrices
    m_renderer->setViewMatrix(m_camera->getViewMatrix());
    m_renderer->setProjectionMatrix(m_camera->getProjectionMatrix());
    
    // Apply lighting from LightingSystem
    if (m_lightingSystem) {
        m_lightingSystem->applyLighting(m_renderer, m_camera->getWorldPosition());
    }
    
    // Render grid using GridSystem
    if (m_gridSystem && m_gridSystem->isVisible()) {
        m_gridSystem->render(m_renderer, m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
    }
    
    // Render scene
    if (m_scene) {
        renderScene();
    }
    
    // Render transform gizmo for selected object
    if (m_showTransformGizmo && m_scene && m_scene->getSelectedObject()) {
        renderTransformGizmo();
    }
    
    // Render any overlays (UI elements in 3D space)
    renderOverlays();
    
    m_renderer->endFrame();
}

void RenderSystem::renderGrid()
{
    // Grid rendering is now handled by GridSystem
    // This method can be removed or kept for backward compatibility
    if (m_gridSystem && m_gridSystem->isVisible()) {
        m_gridSystem->render(m_renderer, m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
    }
}

void RenderSystem::renderScene()
{
    if (!m_scene || !m_renderer) return;
    
    // Render all objects in the scene
    for (const auto& object : m_scene->getObjects()) {
        if (!object->isVisible()) {
            continue;
        }
        
        renderSceneObject(object);
        
        // Render bounding box for selected object
        if (object->isSelected()) {
            renderBoundingBox(object);
        }
    }
}

void RenderSystem::renderTransformGizmo()
{
    if (!m_scene || !m_renderer) return;
    
    auto selectedObject = m_scene->getSelectedObject();
    if (!selectedObject) return;
    
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

void RenderSystem::renderOverlays()
{
    // TODO: Implement overlay rendering (HUD elements, text, etc.)
}

void RenderSystem::renderSceneObject(SceneObjectPtr object)
{
    if (!object || !m_renderer) return;
    
    // Set model matrix
    m_renderer->setModelMatrix(object->getTransform().getModelMatrix());
    
    // Set material
    m_renderer->setMaterial(object->getMaterial());
    
    // Render mesh
    m_renderer->renderMesh(object->getMesh(), m_renderMode);
}

void RenderSystem::renderBoundingBox(SceneObjectPtr object)
{
    if (!object || !m_renderer) return;
    
    QVector3D min = object->getBoundingBoxMin();
    QVector3D max = object->getBoundingBoxMax();
    
    // Transform to world space
    QMatrix4x4 modelMatrix = object->getTransform().getModelMatrix();
    m_renderer->setModelMatrix(QMatrix4x4()); // Use identity for world space lines
    m_renderer->renderAABB(min, max, QVector4D(1.0f, 1.0f, 0.0f, 1.0f));
}
