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
 * █  FILE: Renderer.h                                                           █
 * █  DESCRIPTION: Professional OpenGL Rendering Engine & Graphics Pipeline     █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite          █
 * █  VERSION: 1.0.0                                                             █
 * █  CREATED: 2024                                                              █
 * █  UPDATED: August 7, 2025                                                    █
 * █                                                                              █
 * █  LICENSE: MIT License                                                       █
 * █  COMPANY: The No Hands Company                                              █
 * █  AUTHOR: RudeBase3D Development Team                                        █
 * █                                                                              █
 * █  This file contains the core Renderer class providing professional-grade   █
 * █  OpenGL rendering operations, shader management, and optimized graphics    █
 * █  pipeline for demanding 3D modeling and animation workflows.               █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file Renderer.h
 * @brief Professional OpenGL Rendering Engine and Graphics Pipeline Management
 * 
 * This file contains the comprehensive Renderer class implementation that provides
 * professional-grade OpenGL rendering operations, shader program management, and
 * optimized graphics pipeline coordination essential for demanding 3D modeling and
 * animation workflows. The Renderer serves as the foundational graphics layer
 * enabling high-performance real-time rendering with support for modern OpenGL
 * features, advanced material systems, and production-quality visual output
 * suitable for professional 3D content creation environments.
 * 
 * ## Core Rendering Architecture
 * 
 * ### Professional Graphics Pipeline
 * Comprehensive OpenGL rendering system with industry-standard features:
 * - **Modern OpenGL Support**: OpenGL 3.3+ with Core Profile compatibility
 * - **Shader Program Management**: Dynamic compilation, linking, and caching system
 * - **State Management**: Intelligent OpenGL state tracking and optimization
 * - **Matrix Stack System**: Efficient transformation hierarchy management
 * - **Material Pipeline**: Advanced material property handling with PBR support
 * 
 * ### High-Performance Rendering
 * Optimized rendering operations for real-time 3D applications:
 * - **Batch Rendering**: Efficient grouping of similar rendering operations
 * - **State Caching**: Minimized redundant OpenGL state changes
 * - **Uniform Optimization**: Cached uniform locations and batched updates
 * - **Buffer Management**: Efficient vertex buffer handling and reuse
 * - **Draw Call Reduction**: Optimized rendering strategies for complex scenes
 * 
 * ### Professional Shader System
 * Advanced GLSL shader program management and compilation:
 * - **Built-in Shaders**: Production-ready shaders for common rendering tasks
 * - **Dynamic Compilation**: Runtime shader compilation with error reporting
 * - **Uniform Caching**: Performance-optimized uniform location management
 * - **Shader Validation**: Comprehensive shader program validation and debugging
 * - **Cross-Platform Support**: Consistent shader behavior across platforms
 * 
 * ## Advanced Rendering Features
 * 
 * ### Material and Lighting System
 * Professional material rendering with advanced lighting models:
 * - **Physically-Based Rendering**: Modern PBR material support with metallic-roughness workflow
 * - **Traditional Lighting**: Classic Phong and Blinn-Phong lighting models
 * - **Multi-Light Support**: Dynamic lighting with multiple light sources
 * - **Shadow Mapping**: Real-time shadow generation and rendering
 * - **Normal Mapping**: Surface detail enhancement through normal maps
 * 
 * ### Advanced Visualization Modes
 * Comprehensive rendering modes for professional 3D workflows:
 * - **Solid Rendering**: High-quality surface rendering with materials and lighting
 * - **Wireframe Display**: Edge-only visualization for topology analysis
 * - **Combined Modes**: Solid surfaces with wireframe overlay for detailed inspection
 * - **Debug Visualization**: Specialized rendering for normals, UV coordinates, and topology
 * - **Transparency Support**: Alpha blending for glass, liquids, and transparent materials
 * 
 * ### Professional Debug and Utility Rendering
 * Specialized rendering for development and debugging workflows:
 * - **Line Rendering**: Optimized line drawing for gizmos, guides, and debug visualization
 * - **Bounding Box Display**: Spatial bounds visualization for collision and optimization
 * - **Grid Rendering**: Reference grids and construction aids for precise modeling
 * - **Axis Display**: Coordinate system visualization and orientation helpers
 * - **Performance Profiling**: Rendering performance metrics and optimization aids
 * 
 * ## Technical Implementation
 * 
 * ### OpenGL Core Profile
 * Modern OpenGL implementation with industry best practices:
 * - **Vertex Array Objects**: Efficient vertex state management and binding
 * - **Buffer Objects**: Optimized vertex, index, and uniform buffer handling
 * - **Texture Management**: Multi-texture support with efficient binding and sampling
 * - **Framebuffer Operations**: Render-to-texture capabilities for advanced effects
 * - **Compute Shaders**: GPU compute integration for advanced rendering techniques
 * 
 * ### Cross-Platform Graphics
 * Consistent rendering behavior across diverse operating systems:
 * - **Platform Abstraction**: Hardware-independent rendering interface
 * - **Driver Compatibility**: Robust handling of diverse OpenGL driver implementations
 * - **Extension Management**: Dynamic OpenGL extension detection and utilization
 * - **Context Management**: Proper OpenGL context handling and state preservation
 * - **Error Handling**: Comprehensive OpenGL error detection and reporting
 * 
 * ### Performance Optimization
 * Production-optimized rendering for demanding real-time applications:
 * - **CPU-GPU Synchronization**: Efficient command buffer management and synchronization
 * - **Memory Bandwidth**: Optimized vertex data layout and transfer patterns
 * - **GPU Resource Management**: Intelligent texture and buffer memory management
 * - **Rendering Order**: Depth-sorted and state-sorted rendering for optimal performance
 * - **Level-of-Detail**: Distance-based rendering complexity adjustment
 * 
 * ## Usage Examples
 * 
 * ### Basic Renderer Initialization and Setup
 * ```cpp
 * // Create and initialize renderer for professional 3D application
 * auto renderer = std::make_unique<Renderer>();
 * if (!renderer->initialize()) {
 *     logError("Failed to initialize OpenGL renderer");
 *     return false;
 * }
 * 
 * // Configure default rendering state
 * renderer->setClearColor(glm::vec4(0.2f, 0.2f, 0.25f, 1.0f)); // Professional dark gray
 * renderer->enableDepthTest(true);   // Enable 3D depth testing
 * renderer->enableBlending(false);   // Disable blending for opaque rendering
 * renderer->setLineWidth(1.0f);      // Standard line width
 * 
 * logInfo("Renderer initialized successfully");
 * ```
 * 
 * ### Professional Scene Rendering Loop
 * ```cpp
 * class SceneRenderer {
 * private:
 *     std::unique_ptr<Renderer> m_renderer;
 *     Camera m_camera;
 *     LightingManager m_lighting;
 *     
 * public:
 *     void renderFrame(const Scene& scene) {
 *         // Begin frame with proper setup
 *         m_renderer->beginFrame();
 *         
 *         // Configure camera and projection
 *         m_renderer->setViewMatrix(m_camera.getViewMatrix());
 *         m_renderer->setProjectionMatrix(m_camera.getProjectionMatrix());
 *         m_renderer->setViewPosition(m_camera.getPosition());
 *         
 *         // Setup lighting
 *         auto mainLight = m_lighting.getMainLight();
 *         m_renderer->setLighting(mainLight.direction, mainLight.color);
 *         
 *         // Render solid geometry with materials
 *         renderSolidGeometry(scene);
 *         
 *         // Render transparent geometry (back-to-front)
 *         renderTransparentGeometry(scene);
 *         
 *         // Render debug and utility geometry
 *         renderDebugGeometry(scene);
 *         
 *         // Complete frame
 *         m_renderer->endFrame();
 *     }
 *     
 * private:
 *     void renderSolidGeometry(const Scene& scene) {
 *         m_renderer->enableBlending(false); // Opaque rendering
 *         m_renderer->useShaderProgram("standard");
 *         
 *         for (const auto& entity : scene.getOpaqueEntities()) {
 *             // Set object transformation
 *             m_renderer->setModelMatrix(entity->getTransform().getMatrix());
 *             
 *             // Apply material properties
 *             m_renderer->setMaterial(entity->getMaterial());
 *             
 *             // Render geometry
 *             m_renderer->renderMesh(entity->getMesh(), RenderMode::Solid);
 *         }
 *     }
 *     
 *     void renderTransparentGeometry(const Scene& scene) {
 *         m_renderer->enableBlending(true);  // Enable alpha blending
 *         
 *         // Sort transparent objects back-to-front for proper blending
 *         auto sortedEntities = scene.getTransparentEntitiesSorted(m_camera.getPosition());
 *         
 *         for (const auto& entity : sortedEntities) {
 *             m_renderer->setModelMatrix(entity->getTransform().getMatrix());
 *             m_renderer->setMaterial(entity->getMaterial());
 *             m_renderer->renderMesh(entity->getMesh(), RenderMode::Solid);
 *         }
 *     }
 *     
 *     void renderDebugGeometry(const Scene& scene) {
 *         m_renderer->useShaderProgram("line");
 *         
 *         // Render bounding boxes
 *         for (const auto& entity : scene.getAllEntities()) {
 *             if (entity->isSelected()) {
 *                 auto bounds = entity->getMesh()->getBoundingBox();
 *                 m_renderer->renderAABB(bounds.min, bounds.max, 
 *                                       glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow
 *             }
 *         }
 *         
 *         // Render coordinate axes for selected objects
 *         for (const auto& entity : scene.getSelectedEntities()) {
 *             renderEntityAxes(entity);
 *         }
 *     }
 * };
 * ```
 * 
 * ### Advanced Material and Shader Management
 * ```cpp
 * class MaterialRenderingSystem {
 * private:
 *     Renderer* m_renderer;
 *     MaterialLibrary m_materials;
 *     
 * public:
 *     void renderWithAdvancedMaterials(const std::vector<RenderableEntity>& entities) {
 *         for (const auto& entity : entities) {
 *             auto material = entity.getMaterial();
 *             
 *             // Select appropriate shader based on material type
 *             selectOptimalShader(material);
 *             
 *             // Configure material-specific rendering state
 *             configureMaterialState(material);
 *             
 *             // Set transformation and render
 *             m_renderer->setModelMatrix(entity.getTransform());
 *             m_renderer->setMaterial(material);
 *             m_renderer->renderMesh(entity.getMesh(), entity.getRenderMode());
 *         }
 *     }
 *     
 * private:
 *     void selectOptimalShader(MaterialPtr material) {
 *         if (material->hasNormalMap()) {
 *             m_renderer->useShaderProgram("standard_normal_mapped");
 *         } else if (material->isMetallic()) {
 *             m_renderer->useShaderProgram("pbr_metallic");
 *         } else if (material->isTransparent()) {
 *             m_renderer->useShaderProgram("transparent");
 *         } else {
 *             m_renderer->useShaderProgram("standard");
 *         }
 *     }
 *     
 *     void configureMaterialState(MaterialPtr material) {
 *         // Configure blending for transparent materials
 *         if (material->isTransparent()) {
 *             m_renderer->enableBlending(true);
 *         } else {
 *             m_renderer->enableBlending(false);
 *         }
 *         
 *         // Configure depth testing for special materials
 *         if (material->isDepthTestedDisabled()) {
 *             m_renderer->enableDepthTest(false);
 *         } else {
 *             m_renderer->enableDepthTest(true);
 *         }
 *     }
 * };
 * ```
 * 
 * ### Professional Debugging and Visualization
 * ```cpp
 * class DebugRenderer {
 * private:
 *     Renderer* m_renderer;
 *     
 * public:
 *     void renderDebugInformation(const Scene& scene, const Camera& camera) {
 *         // Switch to line shader for debug rendering
 *         m_renderer->useShaderProgram("line");
 *         m_renderer->setLineWidth(2.0f);
 *         
 *         // Render scene analysis information
 *         renderSceneBounds(scene);
 *         renderLightingDebug(scene);
 *         renderPerformanceMetrics(scene);
 *         renderCameraFrustum(camera);
 *     }
 *     
 * private:
 *     void renderSceneBounds(const Scene& scene) {
 *         // Render overall scene bounding box
 *         auto sceneBounds = scene.calculateBoundingBox();
 *         m_renderer->renderAABB(sceneBounds.min, sceneBounds.max,
 *                               glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
 *         
 *         // Render individual object bounds
 *         for (const auto& entity : scene.getAllEntities()) {
 *             auto entityBounds = entity->getBoundingBox();
 *             glm::vec4 color = entity->isSelected() ? 
 *                              glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) :  // Yellow for selected
 *                              glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);   // Gray for others
 *             m_renderer->renderAABB(entityBounds.min, entityBounds.max, color);
 *         }
 *     }
 *     
 *     void renderLightingDebug(const Scene& scene) {
 *         // Render light direction vectors
 *         for (const auto& light : scene.getLights()) {
 *             glm::vec3 lightPos = light->getPosition();
 *             glm::vec3 lightDir = light->getDirection();
 *             glm::vec3 lightEnd = lightPos + lightDir * 5.0f;
 *             
 *             m_renderer->renderLine(lightPos, lightEnd,
 *                                   glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow
 *         }
 *     }
 *     
 *     void renderCameraFrustum(const Camera& camera) {
 *         // Calculate and render camera frustum edges
 *         auto frustumCorners = camera.calculateFrustumCorners();
 *         
 *         // Render near plane
 *         for (int i = 0; i < 4; ++i) {
 *             int next = (i + 1) % 4;
 *             m_renderer->renderLine(frustumCorners[i], frustumCorners[next],
 *                                   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue
 *         }
 *         
 *         // Render far plane
 *         for (int i = 4; i < 8; ++i) {
 *             int next = 4 + ((i - 4 + 1) % 4);
 *             m_renderer->renderLine(frustumCorners[i], frustumCorners[next],
 *                                   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue
 *         }
 *         
 *         // Render connecting edges
 *         for (int i = 0; i < 4; ++i) {
 *             m_renderer->renderLine(frustumCorners[i], frustumCorners[i + 4],
 *                                   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue
 *         }
 *     }
 * };
 * ```
 * 
 * ### Performance-Optimized Rendering Pipeline
 * ```cpp
 * class OptimizedRenderPipeline {
 * private:
 *     Renderer* m_renderer;
 *     std::vector<RenderBatch> m_renderBatches;
 *     
 * public:
 *     void renderSceneOptimized(const Scene& scene) {
 *         // Prepare optimized render batches
 *         prepareBatchedRendering(scene);
 *         
 *         // Render in optimal order
 *         renderBatchedGeometry();
 *         
 *         // Clear batches for next frame
 *         m_renderBatches.clear();
 *     }
 *     
 * private:
 *     void prepareBatchedRendering(const Scene& scene) {
 *         // Group entities by material and shader for efficient rendering
 *         std::unordered_map<MaterialPtr, std::vector<EntityPtr>> materialGroups;
 *         
 *         for (const auto& entity : scene.getAllEntities()) {
 *             materialGroups[entity->getMaterial()].push_back(entity);
 *         }
 *         
 *         // Create render batches for each material group
 *         for (const auto& [material, entities] : materialGroups) {
 *             RenderBatch batch;
 *             batch.material = material;
 *             batch.entities = entities;
 *             batch.shader = selectOptimalShader(material);
 *             m_renderBatches.push_back(batch);
 *         }
 *         
 *         // Sort batches for optimal rendering order
 *         std::sort(m_renderBatches.begin(), m_renderBatches.end(),
 *                  [](const RenderBatch& a, const RenderBatch& b) {
 *                      return a.shader < b.shader; // Minimize shader switches
 *                  });
 *     }
 *     
 *     void renderBatchedGeometry() {
 *         std::string currentShader = "";
 *         MaterialPtr currentMaterial = nullptr;
 *         
 *         for (const auto& batch : m_renderBatches) {
 *             // Switch shader only when necessary
 *             if (batch.shader != currentShader) {
 *                 m_renderer->useShaderProgram(batch.shader);
 *                 currentShader = batch.shader;
 *             }
 *             
 *             // Set material only when necessary
 *             if (batch.material != currentMaterial) {
 *                 m_renderer->setMaterial(batch.material);
 *                 currentMaterial = batch.material;
 *             }
 *             
 *             // Render all entities in the batch
 *             for (const auto& entity : batch.entities) {
 *                 m_renderer->setModelMatrix(entity->getTransform().getMatrix());
 *                 m_renderer->renderMesh(entity->getMesh(), entity->getRenderMode());
 *             }
 *         }
 *     }
 * };
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Shader Program Management
 * Professional shader compilation and management system:
 * - **Dynamic Compilation**: Runtime GLSL compilation with comprehensive error reporting
 * - **Uniform Caching**: Performance-optimized uniform location management and batching
 * - **Shader Validation**: Automatic program validation and cross-platform compatibility
 * - **Hot Reloading**: Development-time shader recompilation for rapid iteration
 * - **Preprocessor Support**: Conditional compilation and macro expansion for shader variants
 * 
 * ### Advanced Lighting Models
 * Professional lighting and shading capabilities:
 * - **Multi-Light Support**: Dynamic lighting with directional, point, and spot lights
 * - **Shadow Mapping**: Real-time shadow generation with cascaded shadow maps
 * - **Image-Based Lighting**: Environment mapping and reflection probes
 * - **Global Illumination**: Screen-space ambient occlusion and light bouncing
 * - **Volumetric Lighting**: Atmospheric scattering and fog effects
 * 
 * ### Professional Rendering Features
 * Industry-standard rendering capabilities for production workflows:
 * - **Anti-Aliasing**: Multi-sample anti-aliasing (MSAA) and temporal anti-aliasing
 * - **High Dynamic Range**: HDR rendering pipeline with tone mapping
 * - **Depth-of-Field**: Camera-based focus effects with realistic bokeh
 * - **Motion Blur**: Object and camera motion blur for cinematic quality
 * - **Post-Processing**: Comprehensive post-processing effects pipeline
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Rendering
 * @ingroup Graphics
 * @ingroup OpenGL
 * @ingroup Shaders
 */

#pragma once

#include "core/mesh_forward.hpp"
#include "../../Common.h"
// Removed Qt includes
#include <memory>
#include <unordered_map>
#include <string_view>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

/**
 * @addtogroup Rendering
 * @{
 */

/**
 * @brief Low-level OpenGL renderer for efficient graphics operations
 * 
 * The Renderer class provides a comprehensive OpenGL rendering interface
 * optimized for 3D modeling and animation workflows. It manages shader
 * programs, rendering state, transformations, and provides efficient
 * rendering operations for meshes, lines, and debugging geometry.
 * 
 * ## Core Architecture
 * The Renderer is designed as a stateful wrapper around OpenGL that:
 * - **Shader Management**: Loads, compiles, and manages GLSL shader programs
 * - **State Tracking**: Maintains OpenGL state efficiently to minimize redundant calls
 * - **Matrix Management**: Handles view, projection, and model transformations
 * - **Material System**: Applies material properties and lighting uniforms
 * - **Utility Rendering**: Provides optimized functions for common geometry
 * 
 * ## Shader System
 * The renderer includes built-in shaders for common operations:
 * 
 * ### Default Shaders
 * - **Standard Shader**: PBR-style shading with diffuse, specular, and normal maps
 * - **Wireframe Shader**: Edge-only rendering for topology visualization
 * - **Line Shader**: Optimized rendering for debug lines and gizmos
 * 
 * ### Custom Shaders
 * External shaders can be loaded and managed through the shader program system.
 * Each shader program maintains cached uniform locations for performance.
 * 
 * ## Rendering Pipeline Integration
 * The Renderer operates within the larger rendering pipeline:
 * 1. **Setup Phase**: Matrix and material configuration
 * 2. **Geometry Phase**: Mesh and primitive rendering
 * 3. **Debug Phase**: Line and utility geometry rendering
 * 4. **Cleanup Phase**: State restoration and resource management
 * 
 * ## Performance Optimization
 * Several optimizations are implemented for professional workflows:
 * - **State Caching**: Minimizes redundant OpenGL state changes
 * - **Uniform Batching**: Groups similar uniform updates
 * - **Buffer Management**: Efficient vertex buffer handling
 * - **Shader Switching**: Fast shader program transitions
 * 
 * ## Material and Lighting
 * The renderer supports a flexible material system:
 * - **Phong Lighting**: Classic diffuse/specular/ambient model
 * - **PBR Materials**: Physically-based rendering properties
 * - **Dynamic Lighting**: Runtime light parameter adjustment
 * - **Multi-texture**: Support for diffuse, normal, and specular maps
 * 
 * ## Coordinate System
 * The renderer uses a right-handed coordinate system:
 * - **X-axis**: Right (positive) / Left (negative)
 * - **Y-axis**: Up (positive) / Down (negative)  
 * - **Z-axis**: Towards viewer (positive) / Away from viewer (negative)
 * 
 * ## Thread Safety
 * Renderer operations must occur on the OpenGL context thread.
 * The class is not thread-safe and requires external synchronization.
 * 
 * ## Usage Examples
 * ```cpp
 * // Initialize renderer
 * auto renderer = std::make_unique<Renderer>();
 * renderer->initialize();
 * 
 * // Configure matrices
 * renderer->setViewMatrix(camera.getViewMatrix());
 * renderer->setProjectionMatrix(camera.getProjectionMatrix());
 * 
 * // Render frame
 * renderer->beginFrame();
 * 
 * // Render mesh with materials
 * renderer->setModelMatrix(entity.getTransform());
 * renderer->setMaterial(entity.getMaterial());
 * renderer->renderMesh(entity.getMesh(), RenderMode::Solid);
 * 
 * // Render debug lines
 * renderer->renderLine(start, end, glm::vec4(1,0,0,1));
 * 
 * renderer->endFrame();
 * ```
 * 
 * ## Error Handling
 * OpenGL errors are checked and reported through the logging system.
 * Failed operations return false and log detailed error information.
 * 
 * @see RenderSystem for high-level rendering coordination
 * @see Material for surface appearance properties
 * @see rude::Mesh for 3D geometry representation
 * 
 * @ingroup Rendering
 */
class Renderer {
public:
    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct renderer with default settings
     * 
     * Creates a new Renderer instance with default OpenGL state.
     * The renderer requires explicit initialization before use.
     * 
     * @post Renderer is created but not initialized
     * @post All shader programs are unloaded
     * @post Matrix stack is identity
     * 
     * @see initialize() to complete OpenGL setup
     */
    Renderer();
    
    /**
     * @brief Destructor with automatic cleanup
     * 
     * Automatically calls cleanup() to ensure proper OpenGL resource
     * deallocation and shader program cleanup.
     * 
     * @post All OpenGL resources are released
     * @post Shader programs are unloaded
     */
    ~Renderer();

    // ========================================================================
    // System Initialization
    // ========================================================================
    
    /**
     * @brief Initialize OpenGL context and load default shaders
     * 
     * Performs complete renderer initialization including OpenGL extension
     * checking, default shader compilation, and vertex buffer setup.
     * Must be called with a valid OpenGL context current.
     * 
     * @return true if initialization succeeded, false on error
     * 
     * @pre Valid OpenGL 3.3+ context must be current
     * @pre Required OpenGL extensions must be available
     * 
     * @post Renderer is ready for rendering operations
     * @post Default shaders are compiled and loaded
     * @post Line rendering system is initialized
     * @post OpenGL state is set to default values
     * 
     * @note Logs detailed error information on failure
     * @note Should only be called once per Renderer instance
     */
    bool initialize();
    
    /**
     * @brief Clean up OpenGL resources and reset state
     * 
     * Performs complete cleanup of all OpenGL resources including
     * shader programs, vertex buffers, and rendering state.
     * 
     * @post All shader programs are deleted
     * @post All vertex buffers are released
     * @post OpenGL state is reset to defaults
     * @post Renderer is ready for destruction
     * 
     * @note Safe to call multiple times
     * @note Must be called with OpenGL context current
     */
    void cleanup();
    
    // ========================================================================
    // Shader Management
    // ========================================================================
    
    /**
     * @brief Load and compile all default shader programs
     * 
     * Loads the built-in shader programs required for standard rendering
     * operations including solid, wireframe, and line shaders.
     * 
     * @return true if all shaders loaded successfully, false on error
     * 
     * @post Default shader programs are compiled and ready for use
     * @post Uniform locations are cached for performance
     * 
     * @note Called automatically by initialize()
     * @note Logs compilation errors for debugging
     */
    bool loadShaders();
    
    /**
     * @brief Activate specified shader program for rendering
     * 
     * Switches the current OpenGL shader program to the named program.
     * The program must have been previously loaded and compiled.
     * 
     * @param name Name of the shader program to activate
     * 
     * @pre Shader program with given name must exist
     * 
     * @post Named shader program is active for subsequent rendering
     * @post Uniform locations are updated for the active program
     * 
     * @note Performance optimized - no-op if program already active
     * @note Logs warning if shader program not found
     */
    void useShaderProgram(std::string_view name);
    
    // ========================================================================
    // Frame Management
    // ========================================================================
    
    /**
     * @brief Begin new rendering frame
     * 
     * Initializes OpenGL state for a new frame including clearing
     * buffers, resetting state, and preparing for rendering operations.
     * 
     * @post Color and depth buffers are cleared
     * @post OpenGL state is reset to defaults
     * @post Depth testing and culling are enabled
     * @post Blend mode is set for transparency
     * 
     * @note Must be paired with endFrame()
     * @note Call before any rendering operations
     */
    void beginFrame();
    
    /**
     * @brief Complete current rendering frame
     * 
     * Finalizes the current frame by flushing OpenGL commands,
     * restoring state, and preparing for buffer swap.
     * 
     * @post All OpenGL commands are flushed
     * @post Rendering state is restored
     * @post Frame is ready for presentation
     * 
     * @note Must be paired with beginFrame()
     * @note Call after all rendering operations complete
     */
    void endFrame();
    
    // ========================================================================
    // Matrix and Transform Management
    // ========================================================================
    
    /**
     * @brief Set view transformation matrix
     * 
     * Updates the view matrix used for camera transformations.
     * The view matrix transforms world coordinates to camera space.
     * 
     * @param view 4x4 view transformation matrix
     * 
     * @post View matrix is updated for subsequent rendering
     * @post Combined MVP matrix is recalculated
     * 
     * @note Typically derived from camera position and orientation
     * @note Applied to all subsequent mesh rendering calls
     */
    void setViewMatrix(const glm::mat4& view);
    
    /**
     * @brief Set projection transformation matrix
     * 
     * Updates the projection matrix used for perspective or orthographic
     * projection. The projection matrix transforms camera space to clip space.
     * 
     * @param projection 4x4 projection transformation matrix
     * 
     * @post Projection matrix is updated for subsequent rendering
     * @post Combined MVP matrix is recalculated
     * 
     * @note Typically perspective or orthographic projection
     * @note Defines visible frustum and depth range
     */
    void setProjectionMatrix(const glm::mat4& projection);
    
    /**
     * @brief Set model transformation matrix
     * 
     * Updates the model matrix for the current object being rendered.
     * The model matrix transforms object coordinates to world space.
     * 
     * @param model 4x4 model transformation matrix
     * 
     * @post Model matrix is updated for subsequent rendering
     * @post Combined MVP matrix is recalculated
     * @post Normal matrix is computed for lighting
     * 
     * @note Set this before each object/mesh rendering call
     * @note Combined with view and projection for final transformation
     */
    void setModelMatrix(const glm::mat4& model);
    
    // ========================================================================
    // Material and Lighting System
    // ========================================================================
    
    /**
     * @brief Apply material properties to current shader
     * 
     * Sets material uniforms including diffuse, specular, ambient colors,
     * shininess, and texture bindings for the currently active shader.
     * 
     * @param material Shared pointer to material containing surface properties
     * 
     * @post Material uniforms are uploaded to active shader
     * @post Textures are bound to appropriate texture units
     * 
     * @note Material properties depend on active shader program
     * @note Null material pointer uses default material values
     */
    void setMaterial(MaterialPtr material);
    
    /**
     * @brief Configure directional lighting parameters
     * 
     * Sets the primary directional light used for scene illumination
     * including light direction and color intensity.
     * 
     * @param lightDir Direction vector of the light (world space)
     * @param lightColor RGBA color and intensity of the light
     * 
     * @post Light direction uniform is updated in active shader
     * @post Light color uniform is updated in active shader
     * 
     * @note Light direction should be normalized
     * @note Multiple lights require shader program support
     */
    void setLighting(const glm::vec3& lightDir, const glm::vec4& lightColor);
    
    /**
     * @brief Set camera position for specular lighting
     * 
     * Updates the view position used for specular reflection calculations
     * in lighting shaders. Required for accurate specular highlights.
     * 
     * @param viewPos Camera position in world space coordinates
     * 
     * @post View position uniform is updated in active shader
     * @post Specular calculations use updated camera position
     * 
     * @note Critical for realistic specular reflection
     * @note Should match camera world position
     */
    void setViewPosition(const glm::vec3& viewPos);
    
    // ========================================================================
    // Mesh and Geometry Rendering
    // ========================================================================
    
    /**
     * @brief Render 3D mesh with specified visualization mode
     * 
     * Renders a mesh using the current material, lighting, and transformation
     * settings. Supports multiple render modes for different visualization needs.
     * 
     * @param mesh Shared pointer to mesh containing vertex and index data
     * @param mode Rendering mode (Solid, Wireframe, SolidWireframe)
     * 
     * @pre Mesh must be valid and contain vertex data
     * @pre Model matrix should be set for object positioning
     * @pre Appropriate shader program should be active
     * 
     * @post Mesh geometry is rendered to current framebuffer
     * @post OpenGL state may be modified for render mode
     * 
     * @note Performance scales with mesh complexity
     * @note Wireframe mode changes polygon rendering state
     */
    void renderMesh(rude::MeshPtr mesh, RenderMode mode = RenderMode::Solid);
    
    // ========================================================================
    // Utility and Debug Rendering
    // ========================================================================
    
    /**
     * @brief Render single line segment with color
     * 
     * Efficiently renders a single line segment using optimized line
     * rendering system. Ideal for debug visualization and gizmos.
     * 
     * @param start Starting point of the line in world coordinates
     * @param end Ending point of the line in world coordinates  
     * @param color RGBA color of the line
     * 
     * @post Line is rendered with specified color
     * @post Line shader program is temporarily activated
     * 
     * @note Uses optimized vertex buffer for single line
     * @note Line width controlled by setLineWidth()
     */
    void renderLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
    
    /**
     * @brief Render axis-aligned bounding box wireframe
     * 
     * Renders a wireframe bounding box defined by minimum and maximum
     * corners. Useful for debugging spatial bounds and collision volumes.
     * 
     * @param min Minimum corner coordinates of the bounding box
     * @param max Maximum corner coordinates of the bounding box
     * @param color RGBA color of the wireframe lines
     * 
     * @post 12 lines forming box wireframe are rendered
     * @post Line shader program is temporarily activated
     * 
     * @note Efficient rendering using line batching
     * @note Box is axis-aligned in world space
     */
    void renderAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color);
    
    // ========================================================================
    // OpenGL State Management
    // ========================================================================
    
    /**
     * @brief Control depth testing state
     * 
     * Enables or disables OpenGL depth testing for proper 3D rendering.
     * Depth testing ensures correct rendering order of overlapping geometry.
     * 
     * @param enable true to enable depth testing, false to disable
     * 
     * @post OpenGL depth testing state is updated
     * @post Subsequent rendering uses new depth test state
     * 
     * @note Typically enabled for 3D geometry
     * @note May be disabled for certain overlay rendering
     */
    void enableDepthTest(bool enable);
    
    /**
     * @brief Control alpha blending state
     * 
     * Enables or disables OpenGL alpha blending for transparency rendering.
     * Blending allows transparent and translucent materials.
     * 
     * @param enable true to enable blending, false to disable
     * 
     * @post OpenGL blending state is updated
     * @post Blend function is set to standard alpha blending
     * 
     * @note Required for transparent materials
     * @note May affect rendering performance when enabled
     */
    void enableBlending(bool enable);
    
    /**
     * @brief Set line rendering width
     * 
     * Controls the pixel width of rendered lines including debug lines,
     * wireframes, and utility geometry.
     * 
     * @param width Line width in pixels (typically 1.0-10.0)
     * 
     * @post OpenGL line width is updated
     * @post Subsequent line rendering uses new width
     * 
     * @note Width is clamped to hardware-supported range
     * @note Very wide lines may not be supported on all hardware
     */
    void setLineWidth(float width);
    
    /**
     * @brief Set framebuffer clear color
     * 
     * Sets the RGBA color used when clearing the color buffer
     * at the beginning of each frame.
     * 
     * @param color RGBA clear color (typically background color)
     * 
     * @post OpenGL clear color is updated
     * @post Subsequent beginFrame() calls use new clear color
     * 
     * @note Alpha component affects transparency with overlaid content
     * @note Common values: black (0,0,0,1), gray (0.2,0.2,0.2,1)
     */
    void setClearColor(const glm::vec4& color);
    
    // ========================================================================
    // Built-in Shader Source Access
    // ========================================================================
    
    /**
     * @brief Get default vertex shader source code
     * 
     * Returns the GLSL source code for the standard vertex shader
     * used for solid mesh rendering with lighting.
     * 
     * @return Reference to vertex shader source string
     * 
     * @note Includes MVP transformation and normal calculation
     * @note Compatible with Phong lighting model
     */
    const std::string& getDefaultVertexShader();
    
    /**
     * @brief Get default fragment shader source code
     * 
     * Returns the GLSL source code for the standard fragment shader
     * used for solid mesh rendering with material properties.
     * 
     * @return Reference to fragment shader source string
     * 
     * @note Implements Phong lighting with material support
     * @note Supports diffuse, specular, and ambient components
     */
    const std::string& getDefaultFragmentShader();
    
    /**
     * @brief Get wireframe vertex shader source code
     * 
     * Returns the GLSL source code for the wireframe vertex shader
     * optimized for edge-only rendering.
     * 
     * @return Reference to wireframe vertex shader source string
     * 
     * @note Simplified transformation without lighting calculation
     * @note Optimized for wireframe rendering performance
     */
    const std::string& getWireframeVertexShader();
    
    /**
     * @brief Get wireframe fragment shader source code
     * 
     * Returns the GLSL source code for the wireframe fragment shader
     * that renders edges with uniform color.
     * 
     * @return Reference to wireframe fragment shader source string
     * 
     * @note Simple color output without lighting
     * @note Supports alpha for transparent wireframes
     */
    const std::string& getWireframeFragmentShader();
    
    /**
     * @brief Get line vertex shader source code
     * 
     * Returns the GLSL source code for the line vertex shader
     * used for efficient line and debug geometry rendering.
     * 
     * @return Reference to line vertex shader source string
     * 
     * @note Optimized for single-line and multi-line rendering
     * @note Supports per-vertex color attributes
     */
    const std::string& getLineVertexShader();
    
    /**
     * @brief Get line fragment shader source code
     * 
     * Returns the GLSL source code for the line fragment shader
     * that renders lines with interpolated colors.
     * 
     * @return Reference to line fragment shader source string
     * 
     * @note Supports smooth color interpolation along lines
     * @note Alpha blending compatible for transparent lines
     */
    const std::string& getLineFragmentShader();

private:
    // ========================================================================
    // Internal Shader Program Structure
    // ========================================================================
    
    /**
     * @brief Shader program data with cached uniform locations
     * 
     * Internal structure that holds a compiled OpenGL shader program
     * along with cached uniform location data for efficient uniform
     * updates during rendering.
     */
    struct ShaderProgram {
        /// OpenGL shader program object ID
        unsigned int programID;
        
        // ========================================================================
        // Cached Uniform Locations
        // ========================================================================
        
        /// Combined model-view-projection matrix uniform location
        int mvpMatrixLoc;
        
        /// Model transformation matrix uniform location
        int modelMatrixLoc;
        
        /// View transformation matrix uniform location
        int viewMatrixLoc;
        
        /// Projection transformation matrix uniform location
        int projectionMatrixLoc;
        
        /// Normal transformation matrix uniform location (for lighting)
        int normalMatrixLoc;
        
        /// Material diffuse color uniform location
        int diffuseColorLoc;
        
        /// Material specular color uniform location
        int specularColorLoc;
        
        /// Material ambient color uniform location
        int ambientColorLoc;
        
        /// Material shininess factor uniform location
        int shininessLoc;
        
        /// Light direction vector uniform location
        int lightDirectionLoc;
        
        /// Light color and intensity uniform location
        int lightColorLoc;
        
        /// Camera/view position uniform location
        int viewPosLoc;
        
        /// Generic color uniform location (for lines, wireframes)
        int colorLoc;
    };
    
    // ========================================================================
    // Internal State and Resources
    // ========================================================================
    
    /// Map of loaded shader programs by name
    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> m_shaderPrograms;
    
    /// Pointer to currently active shader program
    ShaderProgram* m_currentShader;
    
    // ========================================================================
    // Transformation Matrices
    // ========================================================================
    
    /// Current view transformation matrix
    glm::mat4 m_viewMatrix;
    
    /// Current projection transformation matrix
    glm::mat4 m_projectionMatrix;
    
    /// Current model transformation matrix
    glm::mat4 m_modelMatrix;
    
    // ========================================================================
    // Lighting State
    // ========================================================================
    
    /// Current directional light direction
    glm::vec3 m_lightDirection;
    
    /// Current light color and intensity
    glm::vec4 m_lightColor;
    
    /// Current camera position for specular calculations
    glm::vec3 m_viewPosition;
    
    // ========================================================================
    // Line Rendering Resources
    // ========================================================================
    
    /// Vertex Array Object for line rendering
    unsigned int m_lineVAO;
    
    /// Vertex Buffer Object for line vertex data
    unsigned int m_lineVBO;
    
    // ========================================================================
    // Internal Helper Methods
    // ========================================================================
    
    /**
     * @brief Create and compile shader program from source
     * 
     * Internal method that compiles vertex and fragment shaders,
     * links them into a program, and caches uniform locations.
     * 
     * @param name Identifier name for the shader program
     * @param vertexSource GLSL vertex shader source code
     * @param fragmentSource GLSL fragment shader source code
     * 
     * @return true if compilation and linking succeeded, false on error
     * 
     * @post Shader program is compiled and added to internal map
     * @post Uniform locations are cached for performance
     * 
     * @note Logs detailed compilation and linking errors
     * @note Automatically validates program after linking
     */
    bool createShaderProgram(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
    
    /**
     * @brief Update uniform values in current shader program
     * 
     * Internal method that uploads current matrix, lighting, and
     * material values to the active shader program uniforms.
     * 
     * @pre Valid shader program must be active
     * 
     * @post All cached uniform values are uploaded to GPU
     * @post MVP matrix is computed and uploaded
     * @post Normal matrix is computed for lighting
     * 
     * @note Called automatically before mesh rendering
     * @note Optimized to skip redundant uploads
     */
    void updateUniforms();
    
    /**
     * @brief Initialize line rendering vertex buffers
     * 
     * Internal method that sets up VAO and VBO for efficient
     * line segment rendering used by utility functions.
     * 
     * @post Line VAO and VBO are created and configured
     * @post Vertex attributes are bound for line data
     * 
     * @note Called automatically by initialize()
     * @note Supports dynamic line data updates
     */
    void initializeLineRenderer();
};

/**
 * @}
 */
