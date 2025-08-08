#include "RenderSystem.h"
#include "core/scene.hpp"
#include "core/entity.hpp"
#include "core/transform.hpp"
#include "Camera.h"
#include "Renderer.h"
#include "SceneObject.h"
#include "Mesh.h"
#include "LightingSystem.h"
#include "GridSystem.h"
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    spdlog::info("RenderSystem initialized successfully");
    return true;
}

void RenderSystem::cleanup()
{
    // Systems will clean up themselves
}

void RenderSystem::setScene(std::shared_ptr<rude::Scene> scene)
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
    
    // Set camera position for lighting calculations
    m_renderer->setViewPosition(m_camera->getWorldPosition());
    
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
    
    // Render transform gizmo for selected entity (TODO: implement entity selection system)
    if (m_showTransformGizmo && m_scene) {
        renderTransformGizmo();
    }
    
    // Render any overlays (UI elements in 3D space)
    renderOverlays();
    
    m_renderer->endFrame();
}

void RenderSystem::renderScene()
{
    if (!m_scene || !m_renderer) return;
    
    // Render all entities in the scene
    const auto& entities = m_scene->getEntities();
    
    // Simple debug without QString to avoid include issues
    if (!entities.empty()) {
        spdlog::info("RenderSystem::renderScene() - Found {} entities", entities.size());
    }
    
    for (const auto& entity : entities) {
        if (entity) {
            spdlog::info("Rendering entity ID: {} Name: {}", entity->getId(), entity->getName());
            renderEntity(entity.get());
        }
    }
}

void RenderSystem::renderTransformGizmo()
{
    if (!m_scene || !m_renderer) return;
    
    // TODO: Implement entity selection system and gizmo rendering for ECS
    // For now, disable transform gizmo rendering until selection system is available
    
    /*
    // Find selected entity
    auto entities = m_scene->getAllEntities();
    Entity* selectedEntity = nullptr;
    for (auto* entity : entities) {
        // TODO: Check if entity is selected when selection system is available
        // if (entity->isSelected()) {
        //     selectedEntity = entity;
        //     break;
        // }
    }
    if (!selectedEntity) return;
    
    // TODO: Get entity position from transform component
    // glm::vec3 entityPos = selectedEntity->getTransform().getPosition();
    // glm::mat4 gizmoMatrix = glm::translate(glm::mat4(1.0f), entityPos);
    // m_renderer->setModelMatrix(gizmoMatrix);
    */
}

void RenderSystem::renderOverlays()
{
    // TODO: Implement overlay rendering (HUD elements, text, etc.)
}

void RenderSystem::renderEntity(rude::Entity* entity)
{
    if (!entity || !m_renderer) return;
    
    spdlog::info("RenderSystem::renderEntity - Rendering entity ID: {} Name: {}", entity->getId(), entity->getName());
    
    // Get the entity's mesh
    auto mesh = entity->getMesh();
    if (!mesh) {
        spdlog::warn("Entity {} has no mesh - skipping render", entity->getName());
        return;
    }
    
    // Set the model matrix based on entity's world transform
    glm::mat4 modelMatrix = entity->getWorldTransform();
    m_renderer->setModelMatrix(modelMatrix);
    
    // DEBUG: Log the entity's transform details
    const Transform& transform = entity->getTransform();
    const glm::vec3& pos = transform.getPosition();
    const glm::vec3& scl = transform.getScale();
    spdlog::info("Entity {} transform - Position: ({:.3f}, {:.3f}, {:.3f}), Scale: ({:.3f}, {:.3f}, {:.3f})", 
                 entity->getName(), pos.x, pos.y, pos.z, scl.x, scl.y, scl.z);
    
    // Render the mesh using the Renderer's renderMesh method
    try {
        m_renderer->renderMesh(mesh, RenderMode::Solid);
        spdlog::info("Entity {} successfully rendered with mesh", entity->getName());
    } catch (const std::exception& e) {
        spdlog::error("Failed to render entity {}: {}", entity->getName(), e.what());
    }
}

void RenderSystem::onSceneChanged()
{
    // TODO: Handle scene change events
    // This could involve updating render lists, clearing caches, etc.
}

void RenderSystem::onCameraChanged()
{
    // TODO: Handle camera change events  
    // This could involve updating view matrices, frustum culling, etc.
}
