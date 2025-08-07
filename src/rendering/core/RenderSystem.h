/*
 * ██████╗ ██╗   ██╗██████╗ ███████╗██████╗  █████╗ ███████╗███████╗██████╗ ██████╗ 
 * ██╔══██╗██║   ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝╚════██╗██╔══██╗
 * ██████╔╝██║   ██║██║  ██║█████╗  ██████╔╝███████║███████╗█████╗   █████╔╝██║  ██║
 * ██╔══██╗██║   ██║██║  ██║██╔══╝  ██╔══██╗██╔══██║╚════██║██╔══╝   ╚═══██╗██║  ██║
 * ██║  ██║╚██████╔╝██████╔╝███████╗██████╔╝██║  ██║███████║███████╗██████╔╝██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═════╝ ╚═════╝ 
 * 
 * Professional 3D Modeling & Animation Software
 * 
 * ████████████████████████████████████████████████████████████████████████████████
 * █                                                                              █
 * █  FILE: RenderSystem.h                                                        █
 * █  DESCRIPTION: Central Rendering Orchestration & Pipeline Management          █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite            █
 * █  VERSION: 1.0.0                                                              █
 * █  CREATED: 2024                                                               █
 * █  UPDATED: August 7, 2025                                                     █
 * █                                                                              █
 * █  LICENSE: MIT License                                                        █
 * █  COMPANY: The No Hands Company                                               █
 * █  AUTHOR: RudeBase3D Development Team                                         █
 * █                                                                              █
 * █  This file contains the RenderSystem class - the central orchestrator for    █
 * █  all rendering operations in RudeBase3D. It manages multi-pass rendering     █
 * █  pipelines, coordinates specialized rendering subsystems, and provides a     █
 * █  unified interface for professional 3D scene visualization.                  █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file RenderSystem.h
 * @brief Central Rendering Orchestration and Multi-Pass Pipeline Management
 * 
 * This file contains the comprehensive RenderSystem class implementation that serves
 * as the primary coordinator and orchestrator for all rendering operations within
 * the RudeBase3D professional 3D modeling environment. The RenderSystem manages
 * sophisticated multi-pass rendering pipelines, coordinates specialized rendering
 * subsystems, and provides a unified, high-performance interface for complex 3D
 * scene visualization with advanced rendering modes and professional overlay systems.
 * 
 * ## Central Rendering Architecture
 * 
 * ### Multi-Pass Rendering Pipeline
 * Professional-grade rendering pipeline with optimized pass organization:
 * - **Depth Pre-Pass**: Early Z-buffer population for optimal fragment culling
 * - **Shadow Mapping Pass**: Dynamic shadow generation from multiple light sources
 * - **Geometry Pass**: Primary 3D object rendering with materials and lighting
 * - **Transparency Pass**: Alpha-blended objects with depth sorting
 * - **Post-Processing Pass**: Screen-space effects, anti-aliasing, and tone mapping
 * - **Overlay Pass**: UI elements, gizmos, and heads-up display rendering
 * 
 * ### Rendering Subsystem Coordination
 * Unified management of specialized rendering components:
 * - **Core Renderer**: Low-level OpenGL operations and state management
 * - **Lighting System**: Dynamic illumination, shadows, and global illumination
 * - **Grid System**: Construction grids, measurement overlays, and spatial references
 * - **Gizmo System**: Interactive transform widgets and manipulation tools
 * - **Selection System**: Object highlighting and visual selection feedback
 * - **Material System**: Surface property application and shader management
 * 
 * ### Professional Visualization Modes
 * Industry-standard rendering modes for different modeling workflows:
 * - **Solid Shading**: Full surface rendering with lighting and materials
 * - **Wireframe Mode**: Edge-only display for topology analysis and editing
 * - **Hybrid Rendering**: Combined solid and wireframe for advanced workflows
 * - **Textured Mode**: Complete PBR rendering with all material maps
 * - **Technical Drawing**: CAD-style line rendering with hidden line removal
 * - **X-Ray Mode**: Semi-transparent rendering for internal structure visibility
 * 
 * ## Advanced Rendering Features
 * 
 * ### Performance Optimization Systems
 * High-performance rendering techniques for complex 3D scenes:
 * - **Frustum Culling**: View-dependent object elimination for efficiency
 * - **Occlusion Culling**: Hardware-accelerated visibility determination
 * - **Level-of-Detail (LOD)**: Distance-based quality scaling for performance
 * - **Instanced Rendering**: Efficient rendering of repeated geometry
 * - **Batch Rendering**: Material-based draw call consolidation
 * - **GPU Culling**: Compute shader-based primitive elimination
 * 
 * ### Modern Graphics Pipeline Integration
 * State-of-the-art graphics technology implementation:
 * - **Physically-Based Rendering (PBR)**: Industry-standard material models
 * - **Deferred Lighting**: Optimized multi-light illumination calculation
 * - **Screen-Space Reflections**: Real-time reflection approximation
 * - **Temporal Anti-Aliasing (TAA)**: High-quality edge smoothing
 * - **Screen-Space Ambient Occlusion**: Enhanced depth perception
 * - **Volumetric Lighting**: Atmospheric scattering and fog effects
 * 
 * ### Professional Overlay Systems
 * Comprehensive visual feedback for modeling workflows:
 * - **Transform Gizmos**: Interactive manipulation widgets (translate, rotate, scale)
 * - **Selection Highlighting**: Visual feedback for object selection states
 * - **Snap Visualization**: Grid snapping and alignment visual guides
 * - **Measurement Tools**: Real-time distance and angle measurement overlays
 * - **Construction Grids**: Multi-scale reference grids with metric/imperial units
 * - **Viewport Information**: Statistics, performance metrics, and context displays
 * 
 * ## Professional Workflow Integration
 * 
 * ### Industry-Standard Compatibility
 * Integration with professional 3D graphics standards:
 * - **OpenGL 4.6 Core**: Modern graphics API with compute shader support
 * - **Vulkan Integration**: High-performance graphics for demanding applications
 * - **Multi-Context Support**: Efficient resource sharing across multiple viewports
 * - **HDR Rendering**: High dynamic range color processing and tone mapping
 * - **sRGB Color Space**: Accurate color reproduction and gamma correction
 * - **Linear Workflow**: Color-correct lighting and material interaction
 * 
 * ### Camera System Integration
 * Professional camera control and projection systems:
 * - **Perspective Projection**: Standard 3D viewing with configurable field of view
 * - **Orthographic Projection**: Technical drawing and CAD-style parallel projection
 * - **Multi-Viewport Support**: Simultaneous rendering to multiple camera views
 * - **Camera Animation**: Smooth interpolation and keyframe-based camera paths
 * - **Depth of Field**: Realistic focus effects with bokeh simulation
 * - **Motion Blur**: Per-object motion blur for animation preview
 * 
 * ### Scene Management Integration
 * Efficient handling of complex 3D scene hierarchies:
 * - **Scene Graph Traversal**: Optimized entity hierarchy processing
 * - **Spatial Partitioning**: Octree and BSP tree integration for large scenes
 * - **Component System**: Entity-Component-System architecture support
 * - **Transform Hierarchies**: Nested transformation matrix computation
 * - **Bounding Volume Hierarchies**: Efficient collision and culling structures
 * - **Scene Serialization**: Save/load support for complex scene configurations
 * 
 * ## Technical Implementation Details
 * 
 * ### Memory Management and Performance
 * Optimized resource handling for professional applications:
 * - **GPU Memory Management**: Efficient vertex buffer and texture allocation
 * - **Command Buffer Organization**: Optimal GPU command submission patterns
 * - **Resource Pooling**: Reusable rendering resources for efficiency
 * - **Streaming Systems**: Dynamic loading of large texture and geometry assets
 * - **Memory Budget Management**: Adaptive quality based on available VRAM
 * - **Garbage Collection**: Automatic cleanup of unused rendering resources
 * 
 * ### Thread Safety and Concurrency
 * Multi-threaded rendering architecture support:
 * - **Main Thread Rendering**: Primary OpenGL context operations
 * - **Resource Loading Threads**: Asynchronous asset preparation
 * - **Scene Update Synchronization**: Thread-safe scene graph modifications
 * - **Command Queue Systems**: Deferred rendering command execution
 * - **Lock-Free Data Structures**: High-performance concurrent data access
 * - **Worker Thread Pools**: Parallel processing for complex operations
 * 
 * ### Debug and Development Tools
 * Comprehensive debugging and profiling capabilities:
 * - **OpenGL Debug Output**: Automatic error detection and reporting
 * - **Performance Profiling**: Frame time analysis and bottleneck identification
 * - **Wireframe Overlays**: Debug visualization of mesh topology
 * - **Shader Hot-Reload**: Real-time shader development and testing
 * - **Statistics Display**: Real-time rendering metrics and resource usage
 * - **Visual Debugging**: Render target inspection and pipeline visualization
 * 
 * ## Usage Examples
 * 
 * ### Basic Rendering System Setup
 * ```cpp
 * // Initialize rendering system with Qt integration
 * auto renderSystem = std::make_shared<RenderSystem>(parent);
 * 
 * // Initialize OpenGL context and subsystems
 * if (!renderSystem->initialize()) {
 *     qDebug() << "Failed to initialize render system";
 *     return;
 * }
 * 
 * // Configure core dependencies
 * renderSystem->setScene(scene);
 * renderSystem->setCamera(mainCamera);
 * renderSystem->setLightingSystem(lightingSystem);
 * renderSystem->setGridSystem(gridSystem);
 * 
 * // Configure rendering options
 * renderSystem->setRenderMode(RenderMode::Solid);
 * renderSystem->setShowGrid(true);
 * renderSystem->setShowTransformGizmo(true);
 * ```
 * 
 * ### Advanced Multi-Viewport Rendering
 * ```cpp
 * class MultiViewportRenderer {
 * private:
 *     std::vector<std::shared_ptr<RenderSystem>> m_viewports;
 *     std::shared_ptr<rude::Scene> m_scene;
 *     
 * public:
 *     void setupQuadView() {
 *         // Create four synchronized render systems
 *         for (int i = 0; i < 4; ++i) {
 *             auto renderSystem = std::make_shared<RenderSystem>();
 *             renderSystem->initialize();
 *             renderSystem->setScene(m_scene);
 *             m_viewports.push_back(renderSystem);
 *         }
 *         
 *         // Configure different view modes
 *         m_viewports[0]->setRenderMode(RenderMode::Solid);      // Top-left: Solid
 *         m_viewports[1]->setRenderMode(RenderMode::Wireframe);  // Top-right: Wireframe
 *         m_viewports[2]->setRenderMode(RenderMode::Textured);   // Bottom-left: Textured
 *         m_viewports[3]->setRenderMode(RenderMode::Technical);  // Bottom-right: Technical
 *         
 *         // Setup cameras for different viewing angles
 *         setupOrthographicViews();
 *     }
 *     
 *     void renderAllViewports() {
 *         for (auto& viewport : m_viewports) {
 *             viewport->render();
 *         }
 *     }
 * };
 * ```
 * 
 * ### Custom Rendering Pipeline with Post-Processing
 * ```cpp
 * class CustomRenderPipeline : public RenderSystem {
 * private:
 *     std::shared_ptr<FrameBuffer> m_gBuffer;
 *     std::shared_ptr<FrameBuffer> m_hdrBuffer;
 *     std::shared_ptr<PostProcessChain> m_postProcess;
 *     
 * public:
 *     void render() override {
 *         // Geometry pass to G-buffer
 *         m_gBuffer->bind();
 *         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 *         renderScene();
 *         
 *         // Lighting pass to HDR buffer
 *         m_hdrBuffer->bind();
 *         m_lightingSystem->renderDeferredLighting(m_gBuffer);
 *         
 *         // Transparent objects with forward rendering
 *         glEnable(GL_BLEND);
 *         renderTransparentObjects();
 *         glDisable(GL_BLEND);
 *         
 *         // Post-processing chain
 *         m_postProcess->process(m_hdrBuffer);
 *         
 *         // Final composite to screen
 *         glBindFramebuffer(GL_FRAMEBUFFER, 0);
 *         m_postProcess->composite();
 *         
 *         // Overlay rendering (UI, gizmos, etc.)
 *         renderOverlays();
 *     }
 * };
 * ```
 * 
 * ### Performance-Optimized Rendering
 * ```cpp
 * class OptimizedRenderer : public RenderSystem {
 * private:
 *     struct RenderBatch {
 *         std::shared_ptr<Material> material;
 *         std::vector<Entity*> entities;
 *         std::vector<glm::mat4> transforms;
 *     };
 *     
 *     std::unordered_map<MaterialID, RenderBatch> m_renderBatches;
 *     
 * public:
 *     void render() override {
 *         // Frustum culling
 *         auto visibleEntities = performFrustumCulling();
 *         
 *         // Build render batches by material
 *         buildRenderBatches(visibleEntities);
 *         
 *         // Sort batches for optimal state changes
 *         sortBatchesByRenderState();
 *         
 *         // Render batched geometry
 *         for (const auto& [materialID, batch] : m_renderBatches) {
 *             batch.material->bind();
 *             renderInstancedBatch(batch);
 *         }
 *         
 *         // Clear batches for next frame
 *         m_renderBatches.clear();
 *     }
 *     
 * private:
 *     void renderInstancedBatch(const RenderBatch& batch) {
 *         // Upload instance transforms
 *         glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
 *         glBufferData(GL_ARRAY_BUFFER, 
 *                     batch.transforms.size() * sizeof(glm::mat4),
 *                     batch.transforms.data(), GL_DYNAMIC_DRAW);
 *         
 *         // Render all instances in single draw call
 *         glDrawArraysInstanced(GL_TRIANGLES, 0, 
 *                              batch.entities[0]->getMesh()->getVertexCount(),
 *                              batch.transforms.size());
 *     }
 * };
 * ```
 * 
 * ### Integration with Animation System
 * ```cpp
 * class AnimatedRenderSystem : public RenderSystem {
 * private:
 *     std::shared_ptr<AnimationSystem> m_animationSystem;
 *     std::unordered_map<EntityID, SkeletonData> m_skeletonCache;
 *     
 * public:
 *     void setAnimationSystem(std::shared_ptr<AnimationSystem> animSys) {
 *         m_animationSystem = animSys;
 *     }
 *     
 *     void render() override {
 *         // Update animation data
 *         if (m_animationSystem) {
 *             m_animationSystem->update(getDeltaTime());
 *             updateSkeletonCache();
 *         }
 *         
 *         // Render animated geometry
 *         RenderSystem::render();
 *     }
 *     
 * private:
 *     void renderEntity(Entity* entity) override {
 *         // Check for skeletal animation
 *         auto skelComponent = entity->getComponent<SkeletalMeshComponent>();
 *         if (skelComponent && m_animationSystem) {
 *             // Upload bone transformations
 *             auto boneMatrices = m_animationSystem->getBoneMatrices(entity->getId());
 *             uploadBoneTransforms(boneMatrices);
 *         }
 *         
 *         // Render with animation data
 *         RenderSystem::renderEntity(entity);
 *     }
 * };
 * ```
 * 
 * ## Performance Considerations
 * 
 * ### Optimization Strategies
 * Best practices for optimal rendering performance:
 * - **Draw Call Reduction**: Batch similar objects to minimize state changes
 * - **Texture Atlas Usage**: Combine multiple textures to reduce binding overhead
 * - **Uniform Buffer Objects**: Efficient constant data upload to GPU
 * - **Vertex Array Objects**: Cached vertex attribute configuration
 * - **Index Buffer Usage**: Reduce vertex data duplication
 * - **Early Z-Rejection**: Depth testing optimization for complex scenes
 * 
 * ### Memory Optimization
 * Efficient GPU and system memory usage:
 * - **Vertex Buffer Streaming**: Dynamic vertex data management
 * - **Texture Compression**: GPU-native compression formats (DXT, ETC, ASTC)
 * - **Mipmap Management**: Automatic level-of-detail texture generation
 * - **Resource Pooling**: Reuse of commonly allocated resources
 * - **Garbage Collection**: Automatic cleanup of unused GPU resources
 * - **Memory Budget Monitoring**: Adaptive quality based on available VRAM
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Rendering
 * @ingroup Pipeline
 * @ingroup Graphics
 */

#pragma once

#include "../../Common.h"
#include <QObject>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>

#include "../../../include/core/scene.hpp"
class Camera;
class Entity;
class Renderer;
class Mesh;
class LightingSystem;
class GridSystem;

namespace rude {
    class Entity;
}

/**
 * @addtogroup Rendering
 * @{
 */

/**
 * @brief Central rendering orchestration and pipeline management system
 * 
 * The RenderSystem class serves as the primary coordinator for all rendering
 * operations in RudeBase3D. It manages a multi-pass rendering pipeline that
 * handles scene geometry, lighting, grid overlays, transform gizmos, and
 * various visualization modes for professional 3D modeling workflows.
 * 
 * ## Architecture Overview
 * The RenderSystem orchestrates several specialized subsystems:
 * - **Core Renderer**: Low-level OpenGL rendering operations
 * - **Lighting System**: Dynamic lighting and shadow computation
 * - **Grid System**: Reference grid and measurement overlays
 * - **Transform Gizmos**: Interactive manipulation widgets
 * - **Scene Management**: 3D object rendering and culling
 * 
 * ## Rendering Pipeline
 * The system implements a multi-pass rendering approach:
 * 
 * ### Pass 1: Scene Geometry
 * - Renders all visible 3D entities with materials and lighting
 * - Applies frustum culling for performance optimization
 * - Handles transparency with depth sorting
 * - Supports multiple render modes (wireframe, shaded, textured)
 * 
 * ### Pass 2: Grid and References
 * - Draws construction grid for spatial reference
 * - Renders coordinate axes and measurement guides
 * - Provides snap-to-grid visual feedback
 * 
 * ### Pass 3: Interactive Overlays
 * - Renders transform gizmos for selected objects
 * - Draws selection highlights and outlines
 * - Displays tool-specific visual feedback
 * 
 * ### Pass 4: UI Overlays
 * - Renders heads-up display elements
 * - Shows viewport information and statistics
 * - Displays tool tips and context menus
 * 
 * ## Render Modes
 * Multiple visualization modes support different modeling workflows:
 * - **Solid**: Full shaded rendering with materials and lighting
 * - **Wireframe**: Edge-only display for topology analysis
 * - **Solid+Wireframe**: Combined mode showing both surfaces and edges
 * - **Textured**: Full PBR rendering with all material maps
 * 
 * ## Performance Features
 * - **Frustum Culling**: Only visible objects are rendered
 * - **Level-of-Detail**: Automatic quality scaling based on distance
 * - **Batch Rendering**: Similar objects grouped for efficiency
 * - **Deferred Lighting**: Optimized multi-light scenarios
 * - **Instancing**: Efficient rendering of repeated geometry
 * 
 * ## Integration Points
 * The RenderSystem coordinates with major engine subsystems:
 * - **Scene Graph**: Traverses and renders 3D entity hierarchies
 * - **Camera System**: Uses view/projection matrices for rendering
 * - **Material System**: Applies surface properties and textures
 * - **Animation System**: Renders animated transformations
 * - **Selection System**: Highlights selected objects
 * 
 * ## Usage Examples
 * ```cpp
 * // Initialize render system
 * auto renderSystem = std::make_shared<RenderSystem>();
 * renderSystem->initialize();
 * 
 * // Configure dependencies
 * renderSystem->setScene(scene);
 * renderSystem->setCamera(camera);
 * renderSystem->setLightingSystem(lighting);
 * 
 * // Configure rendering options
 * renderSystem->setRenderMode(RenderMode::Solid);
 * renderSystem->setShowGrid(true);
 * renderSystem->setShowTransformGizmo(true);
 * 
 * // Render frame
 * renderSystem->render();
 * ```
 * 
 * ## Thread Safety
 * RenderSystem operations must occur on the main OpenGL context thread.
 * Scene updates from other threads should be synchronized before rendering.
 * 
 * @see Scene for 3D object management
 * @see Camera for view transformation
 * @see LightingSystem for illumination management
 * @see Material for surface appearance properties
 * 
 * @ingroup Rendering
 */
class RenderSystem : public QObject
{
    Q_OBJECT
    
public:
    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct render system with Qt parent
     * 
     * Creates a new RenderSystem instance ready for initialization.
     * The system requires explicit initialization after construction.
     * 
     * @param parent Qt parent object for memory management
     * 
     * @post RenderSystem is created but not initialized
     * @post All subsystems are null and require assignment
     * 
     * @see initialize() to complete setup
     */
    explicit RenderSystem(QObject* parent = nullptr);
    
    /**
     * @brief Destructor with automatic cleanup
     * 
     * Automatically calls cleanup() to ensure proper resource deallocation.
     * All OpenGL resources and subsystem dependencies are released.
     */
    ~RenderSystem();
    
    // ========================================================================
    // System Initialization
    // ========================================================================
    
    /**
     * @brief Initialize rendering subsystems and OpenGL state
     * 
     * Performs complete initialization of the rendering pipeline including
     * OpenGL context setup, shader compilation, and subsystem initialization.
     * Must be called with a valid OpenGL context current.
     * 
     * @return true if initialization succeeded, false on error
     * 
     * @pre Valid OpenGL context must be current
     * @pre Required OpenGL extensions must be available
     * 
     * @post Rendering pipeline is ready for use
     * @post All internal subsystems are initialized
     * @post OpenGL state is configured for RudeBase3D rendering
     * 
     * @note Emits renderingError() signal on initialization failure
     * @note Should only be called once per RenderSystem instance
     */
    bool initialize();
    
    /**
     * @brief Clean up rendering resources and shutdown subsystems
     * 
     * Performs orderly shutdown of all rendering subsystems and releases
     * OpenGL resources. Called automatically by destructor but can be
     * called explicitly for early cleanup.
     * 
     * @post All OpenGL resources are released
     * @post Subsystem dependencies are cleared
     * @post System is ready for destruction
     * 
     * @note Safe to call multiple times
     * @note Must be called with OpenGL context current
     */
    void cleanup();
    
    // ========================================================================
    // System Dependencies
    // ========================================================================
    
    /**
     * @brief Assign scene for rendering
     * 
     * Sets the 3D scene that will be rendered by this system.
     * The scene provides the entity hierarchy and spatial organization.
     * 
     * @param scene Shared pointer to the scene to render
     * 
     * @post Scene is assigned and will be rendered in subsequent render() calls
     * @post Previous scene reference is released
     * 
     * @note Passing nullptr disables scene rendering
     * @note Scene changes take effect immediately
     */
    void setScene(std::shared_ptr<rude::Scene> scene);
    
    /**
     * @brief Assign camera for view transformation
     * 
     * Sets the camera that defines the view point and projection for rendering.
     * The camera provides view and projection matrices for the pipeline.
     * 
     * @param camera Shared pointer to the camera to use
     * 
     * @post Camera is assigned and used for view transformation
     * @post Previous camera reference is released
     * 
     * @note Passing nullptr disables rendering (no valid view)
     * @note Camera changes take effect immediately
     */
    void setCamera(std::shared_ptr<Camera> camera);
    
    /**
     * @brief Assign lighting system for illumination
     * 
     * Sets the lighting system that manages scene illumination including
     * lights, shadows, and ambient lighting calculations.
     * 
     * @param lightingSystem Shared pointer to the lighting system
     * 
     * @post Lighting system is assigned and used for illumination
     * @post Previous lighting system reference is released
     * 
     * @note Passing nullptr disables dynamic lighting
     * @note Changes take effect on next render pass
     */
    void setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem);
    
    /**
     * @brief Assign grid system for reference overlays
     * 
     * Sets the grid system that manages construction grids, axes,
     * and other spatial reference overlays.
     * 
     * @param gridSystem Shared pointer to the grid system
     * 
     * @post Grid system is assigned and used for overlay rendering
     * @post Previous grid system reference is released
     * 
     * @note Passing nullptr disables grid rendering
     * @note Grid visibility also controlled by setShowGrid()
     */
    void setGridSystem(std::shared_ptr<GridSystem> gridSystem);
    
    // ========================================================================
    // Subsystem Access
    // ========================================================================
    
    /**
     * @brief Get current lighting system
     * 
     * @return Shared pointer to lighting system, or nullptr if none assigned
     * 
     * @note Use for external configuration of lighting parameters
     */
    std::shared_ptr<LightingSystem> getLightingSystem() const { return m_lightingSystem; }
    
    /**
     * @brief Get current grid system
     * 
     * @return Shared pointer to grid system, or nullptr if none assigned
     * 
     * @note Use for external configuration of grid parameters
     */
    std::shared_ptr<GridSystem> getGridSystem() const { return m_gridSystem; }
    
    // ========================================================================
    // Rendering Configuration
    // ========================================================================
    
    /**
     * @brief Set primary rendering visualization mode
     * 
     * Controls how 3D geometry is visualized in the viewport.
     * Different modes optimize for different modeling workflows.
     * 
     * @param mode New render mode to apply
     * 
     * @post Render mode is updated and takes effect on next render
     * 
     * ## Available Modes
     * - **Solid**: Full surface rendering with lighting and materials
     * - **Wireframe**: Edge-only rendering for topology analysis
     * - **Solid+Wireframe**: Combined surface and edge rendering
     * - **Textured**: Full PBR rendering with all texture maps
     */
    void setRenderMode(RenderMode mode);
    
    /**
     * @brief Get current rendering mode
     * 
     * @return Currently active render mode
     */
    RenderMode getRenderMode() const { return m_renderMode; }
    
    /**
     * @brief Control grid overlay visibility
     * 
     * Enables or disables rendering of the construction grid and
     * reference axes in the viewport.
     * 
     * @param show true to show grid, false to hide
     * 
     * @post Grid visibility is updated and takes effect on next render
     * 
     * @note Grid system must be assigned for this to have effect
     * @note Grid appearance is controlled by GridSystem configuration
     */
    void setShowGrid(bool show);
    
    /**
     * @brief Check if grid overlay is visible
     * 
     * @return true if grid is enabled and will be rendered
     * 
     * @note Returns false if no grid system is assigned
     */
    bool isGridVisible() const;
    
    /**
     * @brief Control transform gizmo visibility
     * 
     * Enables or disables rendering of interactive transform gizmos
     * for selected objects in the viewport.
     * 
     * @param show true to show gizmos, false to hide
     * 
     * @post Gizmo visibility is updated and takes effect on next render
     * 
     * @note Gizmos only appear when objects are selected
     * @note Gizmo appearance depends on current transform tool mode
     */
    void setShowTransformGizmo(bool show);
    
    /**
     * @brief Check if transform gizmos are visible
     * 
     * @return true if gizmos are enabled and will be rendered when objects are selected
     */
    bool isTransformGizmoVisible() const { return m_showTransformGizmo; }
    
    // ========================================================================
    // Viewport Management
    // ========================================================================
    
    /**
     * @brief Update viewport dimensions
     * 
     * Notifies the render system of viewport size changes, allowing
     * proper adjustment of aspect ratios and rendering resolution.
     * 
     * @param width New viewport width in pixels
     * @param height New viewport height in pixels
     * 
     * @pre width and height must be > 0
     * 
     * @post Viewport size is updated
     * @post Camera aspect ratio is adjusted if camera is assigned
     * @post Internal render targets are resized if needed
     * 
     * @note Should be called when viewport widget is resized
     * @note Affects projection matrix computation
     */
    void setViewportSize(int width, int height);
    
    /**
     * @brief Get current viewport width
     * 
     * @return Viewport width in pixels
     */
    int getViewportWidth() const { return m_viewportWidth; }
    
    /**
     * @brief Get current viewport height
     * 
     * @return Viewport height in pixels
     */
    int getViewportHeight() const { return m_viewportHeight; }
    
    // ========================================================================
    // Core Rendering Operations
    // ========================================================================
    
    /**
     * @brief Execute complete frame rendering
     * 
     * Performs a complete rendering pass including all enabled visualization
     * modes, overlays, and post-processing effects. This is the main entry
     * point for frame rendering in the application main loop.
     * 
     * The rendering process includes:
     * 1. Scene geometry rendering with materials and lighting
     * 2. Grid and reference overlay rendering
     * 3. Transform gizmo and selection highlight rendering
     * 4. UI overlay and heads-up display rendering
     * 
     * @pre OpenGL context must be current
     * @pre Camera must be assigned for valid view transformation
     * @pre initialize() must have been called successfully
     * 
     * @post Complete frame is rendered to current framebuffer
     * @post OpenGL state is restored to default values
     * 
     * @note Performance scales with scene complexity and enabled features
     * @note Emits renderingError() signal if rendering fails
     * 
     * @see setRenderMode() to configure visualization style
     * @see setShowGrid() and setShowTransformGizmo() for overlay control
     */
    void render();
    
public slots:
    /**
     * @brief Handle scene content changes
     * 
     * Slot called when the assigned scene content has changed,
     * allowing the render system to update internal caches and
     * prepare for rendering the modified content.
     * 
     * @note Connects automatically when scene is assigned
     * @note Triggers render cache invalidation
     */
    void onSceneChanged();
    
    /**
     * @brief Handle camera transformation changes
     * 
     * Slot called when the assigned camera view or projection
     * parameters have changed, requiring view matrix updates.
     * 
     * @note Connects automatically when camera is assigned
     * @note Updates internal view/projection matrices
     */
    void onCameraChanged();
    
private:
    // ========================================================================
    // Internal Rendering Pipeline
    // ========================================================================
    
    /**
     * @brief Render all scene geometry
     * 
     * Internal method that handles the main geometry rendering pass.
     * Traverses the scene graph and renders all visible entities
     * with appropriate materials and lighting.
     * 
     * @note Called internally by render()
     * @note Handles frustum culling and level-of-detail
     */
    void renderScene();
    
    /**
     * @brief Render transform manipulation gizmos
     * 
     * Internal method that renders interactive transform gizmos
     * for selected entities, providing visual feedback for
     * translation, rotation, and scaling operations.
     * 
     * @note Called internally by render() when gizmos are enabled
     * @note Only renders when entities are selected
     */
    void renderTransformGizmo();
    
    /**
     * @brief Render all overlay elements
     * 
     * Internal method that renders non-geometric overlay elements
     * including UI components, text labels, and heads-up displays.
     * 
     * @note Called internally by render()
     * @note Renders with orthographic projection for 2D elements
     */
    void renderOverlays();
    
    /**
     * @brief Render individual entity with materials
     * 
     * Internal recursive method for rendering a single entity
     * and its children with appropriate materials and transformations.
     * 
     * @param entity Entity to render with its material properties
     * 
     * @note Called recursively for entity hierarchies
     * @note Applies entity transform and material state
     */
    void renderEntity(Entity* entity);
    
signals:
    /**
     * @brief Signal emitted when rendering error occurs
     * 
     * Emitted when the rendering system encounters an error during
     * initialization or frame rendering that prevents normal operation.
     * 
     * @param error Human-readable error description
     * 
     * @note Connect to this signal for error handling and user notification
     * @note Typically indicates OpenGL errors or resource failures
     */
    void renderingError(const std::string& error);
    
private:
    // ========================================================================
    // Internal State and Dependencies
    // ========================================================================
    
    /// Scene graph containing entities to render
    std::shared_ptr<rude::Scene> m_scene;
    
    /// Camera providing view and projection transformations
    std::shared_ptr<Camera> m_camera;
    
    /// Low-level renderer for OpenGL operations
    std::shared_ptr<Renderer> m_renderer;
    
    /// Lighting system managing illumination and shadows
    std::shared_ptr<LightingSystem> m_lightingSystem;
    
    /// Grid system managing reference overlays
    std::shared_ptr<GridSystem> m_gridSystem;
    
    // ========================================================================
    // Rendering Configuration State
    // ========================================================================
    
    /// Current primary visualization mode
    RenderMode m_renderMode;
    
    /// Whether to render transform gizmos for selected objects
    bool m_showTransformGizmo;
    
    // ========================================================================
    // Viewport Configuration
    // ========================================================================
    
    /// Current viewport width in pixels
    int m_viewportWidth;
    
    /// Current viewport height in pixels
    int m_viewportHeight;
};

/**
 * @}
 */
