#pragma once

#include "Common.h"
#include <QObject>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>

#include "core/scene.hpp"
class Camera;
class Entity;
class Renderer;
class Mesh;
class LightingSystem;
class GridSystem;

namespace rude {
    class Entity;
}

class RenderSystem : public QObject
{
    Q_OBJECT
    
public:
    explicit RenderSystem(QObject* parent = nullptr);
    ~RenderSystem();
    
    // Initialization
    bool initialize();
    void cleanup();
    
    // Dependencies
    void setScene(std::shared_ptr<rude::Scene> scene);
    void setCamera(std::shared_ptr<Camera> camera);
    void setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem);
    void setGridSystem(std::shared_ptr<GridSystem> gridSystem);
    
    // Get systems for external configuration
    std::shared_ptr<LightingSystem> getLightingSystem() const { return m_lightingSystem; }
    std::shared_ptr<GridSystem> getGridSystem() const { return m_gridSystem; }
    
    // Rendering settings
    void setRenderMode(RenderMode mode);
    RenderMode getRenderMode() const { return m_renderMode; }
    void setShowGrid(bool show);
    bool isGridVisible() const;
    void setShowTransformGizmo(bool show);
    bool isTransformGizmoVisible() const { return m_showTransformGizmo; }
    
    // Viewport settings
    void setViewportSize(int width, int height);
    
    // Main rendering
    void render();
    
    // Individual render passes
    void renderGrid();
    void renderScene();
    void renderTransformGizmo();
    void renderOverlays();
    
signals:
    void renderingError(const std::string& error);
    
private:
    void renderEntity(Entity* entity);
    
    std::shared_ptr<rude::Scene> m_scene;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<LightingSystem> m_lightingSystem;
    std::shared_ptr<GridSystem> m_gridSystem;
    
    // Rendering settings
    RenderMode m_renderMode;
    bool m_showTransformGizmo;
    
    // Viewport
    int m_viewportWidth;
    int m_viewportHeight;
};
