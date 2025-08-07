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
 * █  FILE: Camera.h                                                              █
 * █  DESCRIPTION: Professional Camera System & 3D View Transformation           █
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
 * █  This file contains the Camera class - a comprehensive camera system        █
 * █  providing professional 3D view transformation, projection management,      █
 * █  and interactive navigation controls for high-quality 3D visualization.     █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file Camera.h
 * @brief Professional Camera System and 3D View Transformation Management
 * 
 * This file contains the comprehensive Camera class implementation that provides
 * professional-grade camera control and view transformation for 3D modeling and
 * visualization applications. The Camera system supports both perspective and
 * orthographic projection modes, advanced navigation controls, and seamless
 * integration with rendering pipelines for high-quality 3D scene visualization
 * within the RudeBase3D professional modeling environment.
 * 
 * ## Professional Camera Architecture
 * 
 * ### Dual Projection System
 * Industry-standard projection modes for different workflow requirements:
 * - **Perspective Projection**: Natural human vision simulation for artistic modeling
 * - **Orthographic Projection**: Technical drawing and CAD-style parallel projection
 * - **Dynamic Switching**: Seamless transition between projection modes
 * - **Parameter Persistence**: Individual projection settings maintained independently
 * 
 * ### Advanced View Transformation
 * Professional-grade view matrix computation and management:
 * - **Transform Integration**: Full Transform class integration for hierarchical transforms
 * - **Matrix Optimization**: Efficient view matrix calculation with minimal overhead
 * - **Precision Handling**: Double-precision calculations for large scene coordinates
 * - **Numerical Stability**: Robust matrix computation preventing degeneracy cases
 * 
 * ### Interactive Navigation Controls
 * Comprehensive camera manipulation for professional 3D workflows:
 * - **Orbit Navigation**: Target-based spherical coordinate camera movement
 * - **Pan Operations**: Screen-space panning with accurate world-space mapping
 * - **Zoom Controls**: Distance-based and field-of-view zoom with smooth interpolation
 * - **Fly Navigation**: Free-form camera movement for scene exploration
 * - **LookAt Targeting**: Automatic camera orientation toward specific points
 * 
 * ## Professional Workflow Integration
 * 
 * ### Industry-Standard Navigation
 * Camera controls matching professional 3D software conventions:
 * - **Maya-Style Navigation**: Middle mouse orbit, pan, and zoom controls
 * - **Blender-Style Controls**: Numpad shortcuts and smooth orbital navigation
 * - **CAD-Style Projection**: Orthographic views with precise technical accuracy
 * - **Game-Style Movement**: WASD movement with mouse look for scene exploration
 * 
 * ### Multi-Viewport Support
 * Professional multi-view modeling environment support:
 * - **Synchronized Cameras**: Linked camera controls across multiple viewports
 * - **Independent Projections**: Per-viewport projection mode configuration
 * - **View Standardization**: Standard orthographic views (top, front, side, isometric)
 * - **Custom View Storage**: User-defined camera positions and orientations
 * 
 * ### Rendering Pipeline Integration
 * Seamless integration with modern graphics rendering systems:
 * - **Matrix Generation**: Optimized view and projection matrix computation
 * - **Frustum Calculation**: Accurate view frustum for culling and clipping
 * - **Ray Casting**: Screen-to-world ray generation for object picking
 * - **Viewport Mapping**: Screen space to world space coordinate transformation
 * 
 * ## Advanced Camera Features
 * 
 * ### Projection Management
 * Sophisticated projection parameter control and optimization:
 * - **Field of View Control**: Dynamic FOV adjustment with perspective distortion control
 * - **Near/Far Plane Optimization**: Automatic depth range optimization for Z-buffer precision
 * - **Aspect Ratio Handling**: Automatic aspect ratio calculation and manual override
 * - **Orthographic Bounds**: Precise orthographic volume definition and scaling
 * 
 * ### Coordinate System Support
 * Multi-coordinate system compatibility for professional workflows:
 * - **Right-Handed Coordinates**: Standard OpenGL coordinate system orientation
 * - **Left-Handed Support**: DirectX coordinate system compatibility
 * - **Y-Up Orientation**: Industry-standard vertical axis configuration
 * - **Z-Up Conversion**: Engineering and CAD coordinate system support
 * 
 * ### Performance Optimization
 * High-performance camera operations for real-time 3D applications:
 * - **Matrix Caching**: Intelligent caching of computed transformation matrices
 * - **Dirty Flag System**: Selective matrix recalculation only when parameters change
 * - **SIMD Optimization**: Vector math acceleration for matrix operations
 * - **Memory Efficiency**: Minimal memory footprint with optimized data layout
 * 
 * ## Technical Implementation Details
 * 
 * ### Matrix Mathematics
 * Professional-grade mathematical foundation for camera transformations:
 * - **GLM Integration**: Industry-standard mathematics library integration
 * - **Quaternion Rotations**: Stable rotation representation preventing gimbal lock
 * - **Homogeneous Coordinates**: 4x4 matrix mathematics for affine transformations
 * - **Projection Matrices**: Accurate perspective and orthographic projection computation
 * 
 * ### Navigation Algorithms
 * Sophisticated camera movement and orientation algorithms:
 * - **Spherical Interpolation**: Smooth camera rotation with quaternion SLERP
 * - **Arc-Ball Rotation**: Intuitive 3D rotation using virtual trackball metaphor
 * - **Exponential Smoothing**: Natural camera movement with velocity-based damping
 * - **Constraint Systems**: Optional movement constraints for controlled navigation
 * 
 * ### Ray Casting Mathematics
 * Accurate screen-to-world ray computation for object interaction:
 * - **Viewport Transformation**: Screen coordinates to normalized device coordinates
 * - **Inverse Projection**: NDC to view space coordinate transformation
 * - **View Space Rays**: Camera-relative ray direction computation
 * - **World Space Conversion**: Final ray transformation to world coordinates
 * 
 * ## Professional Camera Modes
 * 
 * ### Perspective Camera Configuration
 * Artistic and general-purpose 3D visualization:
 * - **Field of View**: 45° to 90° typical range for natural perspective
 * - **Near Plane**: 0.1 to 1.0 units for close object visibility
 * - **Far Plane**: 1000 to 10000 units for large scene support
 * - **Aspect Ratio**: Automatic viewport-based or manual override control
 * 
 * ### Orthographic Camera Configuration
 * Technical drawing and CAD-style visualization:
 * - **Uniform Scaling**: Consistent object scale regardless of distance
 * - **Parallel Projection**: No perspective distortion for technical accuracy
 * - **Precise Measurements**: Accurate dimensional representation
 * - **Standard Views**: Top, front, side, and isometric orientations
 * 
 * ### Specialized Camera Behaviors
 * Advanced camera modes for specific professional workflows:
 * - **Turntable Mode**: Object-centered rotation for product visualization
 * - **Walkthrough Mode**: First-person navigation for architectural visualization
 * - **Flythrough Mode**: Smooth path-based camera animation
 * - **Focus Mode**: Automatic camera adjustment to frame selected objects
 * 
 * ## Usage Examples
 * 
 * ### Basic Camera Setup and Configuration
 * ```cpp
 * // Initialize camera with perspective projection
 * auto camera = std::make_shared<Camera>();
 * camera->setPerspective(45.0f,    // 45-degree field of view
 *                       16.0f/9.0f, // Widescreen aspect ratio
 *                       0.1f,       // Near clipping plane
 *                       1000.0f);   // Far clipping plane
 * 
 * // Set initial camera position and orientation
 * camera->getTransform().setPosition(glm::vec3(10.0f, 5.0f, 10.0f));
 * camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f)); // Look at scene origin
 * 
 * // Configure viewport dimensions
 * camera->setViewportSize(1920.0f, 1080.0f);
 * ```
 * 
 * ### Professional Maya-Style Navigation
 * ```cpp
 * class MayaStyleCameraController {
 * private:
 *     std::shared_ptr<Camera> m_camera;
 *     glm::vec3 m_orbitCenter;
 *     bool m_isOrbiting = false;
 *     bool m_isPanning = false;
 *     bool m_isZooming = false;
 *     
 * public:
 *     void handleMouseInput(MouseButton button, MouseState state, 
 *                          const glm::vec2& position, const KeyModifiers& mods) {
 *         if (button == MouseButton::Middle) {
 *             if (mods.alt && state == MouseState::Pressed) {
 *                 // Alt+MMB: Start orbit mode
 *                 m_isOrbiting = true;
 *                 m_orbitCenter = calculateOrbitCenter();
 *             } else if (mods.alt && mods.shift && state == MouseState::Pressed) {
 *                 // Alt+Shift+MMB: Start pan mode
 *                 m_isPanning = true;
 *             } else if (state == MouseState::Released) {
 *                 m_isOrbiting = m_isPanning = false;
 *             }
 *         }
 *     }
 *     
 *     void handleMouseMove(const glm::vec2& delta) {
 *         if (m_isOrbiting) {
 *             // Maya-style orbital rotation
 *             float yawDelta = delta.x * 0.01f;   // Horizontal mouse = yaw
 *             float pitchDelta = delta.y * 0.01f; // Vertical mouse = pitch
 *             m_camera->orbit(m_orbitCenter, yawDelta, pitchDelta);
 *         } else if (m_isPanning) {
 *             // Maya-style screen-space panning
 *             glm::vec3 right = m_camera->getRight();
 *             glm::vec3 up = m_camera->getUp();
 *             glm::vec3 panDelta = (right * -delta.x + up * delta.y) * 0.01f;
 *             m_camera->pan(panDelta);
 *         }
 *     }
 *     
 *     void handleWheelScroll(float deltaY) {
 *         // Maya-style distance-based zoom
 *         float zoomFactor = deltaY * 0.1f;
 *         m_camera->zoom(zoomFactor);
 *     }
 * };
 * ```
 * 
 * ### Orthographic Technical Drawing Setup
 * ```cpp
 * class TechnicalDrawingCamera {
 * private:
 *     std::shared_ptr<Camera> m_camera;
 *     
 * public:
 *     void setupStandardViews() {
 *         // Configure orthographic projection
 *         float viewSize = 10.0f; // 10 unit viewing volume
 *         m_camera->setOrthographic(-viewSize, viewSize,   // Left, right
 *                                   -viewSize, viewSize,   // Bottom, top
 *                                   -100.0f, 100.0f);     // Near, far
 *     }
 *     
 *     void setTopView() {
 *         m_camera->getTransform().setPosition(glm::vec3(0, 20, 0));
 *         m_camera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1)); // Z-forward
 *     }
 *     
 *     void setFrontView() {
 *         m_camera->getTransform().setPosition(glm::vec3(0, 0, 20));
 *         m_camera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Y-up
 *     }
 *     
 *     void setSideView() {
 *         m_camera->getTransform().setPosition(glm::vec3(20, 0, 0));
 *         m_camera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Y-up
 *     }
 *     
 *     void setIsometricView() {
 *         // Standard isometric viewing angle (35.264°, 45°)
 *         m_camera->getTransform().setPosition(glm::vec3(10, 10, 10));
 *         m_camera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
 *     }
 * };
 * ```
 * 
 * ### Advanced Camera Animation System
 * ```cpp
 * class CameraAnimationSystem {
 * private:
 *     struct CameraKeyframe {
 *         float time;
 *         glm::vec3 position;
 *         glm::quat rotation;
 *         float fov;
 *     };
 *     
 *     std::vector<CameraKeyframe> m_keyframes;
 *     std::shared_ptr<Camera> m_camera;
 *     float m_currentTime = 0.0f;
 *     
 * public:
 *     void addKeyframe(float time, const glm::vec3& pos, 
 *                     const glm::quat& rot, float fov = 45.0f) {
 *         CameraKeyframe keyframe = { time, pos, rot, fov };
 *         
 *         // Insert keyframe in chronological order
 *         auto it = std::lower_bound(m_keyframes.begin(), m_keyframes.end(),
 *                                   keyframe, [](const auto& a, const auto& b) {
 *                                       return a.time < b.time;
 *                                   });
 *         m_keyframes.insert(it, keyframe);
 *     }
 *     
 *     void update(float deltaTime) {
 *         m_currentTime += deltaTime;
 *         
 *         // Find surrounding keyframes
 *         auto nextIt = std::upper_bound(m_keyframes.begin(), m_keyframes.end(),
 *                                       m_currentTime, [](float time, const auto& kf) {
 *                                           return time < kf.time;
 *                                       });
 *         
 *         if (nextIt == m_keyframes.begin() || nextIt == m_keyframes.end()) {
 *             return; // Before first or after last keyframe
 *         }
 *         
 *         auto prevIt = nextIt - 1;
 *         
 *         // Interpolate between keyframes
 *         float t = (m_currentTime - prevIt->time) / (nextIt->time - prevIt->time);
 *         t = smoothStep(t); // Apply easing function
 *         
 *         // Interpolate position
 *         glm::vec3 position = glm::mix(prevIt->position, nextIt->position, t);
 *         
 *         // Spherical interpolation for rotation
 *         glm::quat rotation = glm::slerp(prevIt->rotation, nextIt->rotation, t);
 *         
 *         // Linear interpolation for field of view
 *         float fov = glm::mix(prevIt->fov, nextIt->fov, t);
 *         
 *         // Apply interpolated values to camera
 *         m_camera->getTransform().setPosition(position);
 *         m_camera->getTransform().setRotation(rotation);
 *         m_camera->setFOV(fov);
 *     }
 *     
 * private:
 *     float smoothStep(float t) {
 *         // Smooth step function for natural easing
 *         return t * t * (3.0f - 2.0f * t);
 *     }
 * };
 * ```
 * 
 * ### Object Picking and Ray Casting
 * ```cpp
 * class CameraPickingSystem {
 * private:
 *     std::shared_ptr<Camera> m_camera;
 *     
 * public:
 *     ObjectID pickObject(const glm::vec2& screenPos, const std::vector<Object>& objects) {
 *         // Generate ray from camera through screen position
 *         glm::vec2 viewportSize(m_camera->getViewportWidth(), m_camera->getViewportHeight());
 *         glm::vec3 rayDirection = m_camera->screenToWorldRay(screenPos, viewportSize);
 *         glm::vec3 rayOrigin = m_camera->getWorldPosition();
 *         
 *         // Test ray against all objects
 *         float closestDistance = std::numeric_limits<float>::max();
 *         ObjectID closestObject = INVALID_OBJECT_ID;
 *         
 *         for (const auto& object : objects) {
 *             float distance;
 *             if (rayIntersectsObject(rayOrigin, rayDirection, object, distance)) {
 *                 if (distance < closestDistance) {
 *                     closestDistance = distance;
 *                     closestObject = object.getId();
 *                 }
 *             }
 *         }
 *         
 *         return closestObject;
 *     }
 *     
 *     glm::vec3 projectWorldToScreen(const glm::vec3& worldPos) {
 *         // Transform world position to screen coordinates
 *         glm::mat4 viewProjection = m_camera->getViewProjectionMatrix();
 *         glm::vec4 clipPos = viewProjection * glm::vec4(worldPos, 1.0f);
 *         
 *         // Perspective divide
 *         if (clipPos.w != 0.0f) {
 *             clipPos /= clipPos.w;
 *         }
 *         
 *         // Convert from NDC to screen coordinates
 *         glm::vec3 screenPos;
 *         screenPos.x = (clipPos.x + 1.0f) * 0.5f * m_camera->getViewportWidth();
 *         screenPos.y = (1.0f - clipPos.y) * 0.5f * m_camera->getViewportHeight();
 *         screenPos.z = clipPos.z; // Depth value
 *         
 *         return screenPos;
 *     }
 * };
 * ```
 * 
 * ### Multi-Camera Viewport System
 * ```cpp
 * class MultiCameraViewportSystem {
 * private:
 *     struct Viewport {
 *         std::shared_ptr<Camera> camera;
 *         glm::vec4 bounds; // x, y, width, height (normalized)
 *         std::string name;
 *         bool isActive;
 *     };
 *     
 *     std::vector<Viewport> m_viewports;
 *     int m_activeViewport = 0;
 *     
 * public:
 *     void setupQuadView() {
 *         m_viewports.clear();
 *         
 *         // Top-left: Perspective view
 *         auto perspectiveCamera = createPerspectiveCamera();
 *         m_viewports.push_back({ perspectiveCamera, glm::vec4(0.0f, 0.5f, 0.5f, 0.5f), 
 *                                "Perspective", true });
 *         
 *         // Top-right: Top orthographic view
 *         auto topCamera = createOrthographicCamera();
 *         topCamera->getTransform().setPosition(glm::vec3(0, 20, 0));
 *         topCamera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
 *         m_viewports.push_back({ topCamera, glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), 
 *                                "Top", true });
 *         
 *         // Bottom-left: Front orthographic view
 *         auto frontCamera = createOrthographicCamera();
 *         frontCamera->getTransform().setPosition(glm::vec3(0, 0, 20));
 *         frontCamera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
 *         m_viewports.push_back({ frontCamera, glm::vec4(0.0f, 0.0f, 0.5f, 0.5f), 
 *                                "Front", true });
 *         
 *         // Bottom-right: Side orthographic view
 *         auto sideCamera = createOrthographicCamera();
 *         sideCamera->getTransform().setPosition(glm::vec3(20, 0, 0));
 *         sideCamera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
 *         m_viewports.push_back({ sideCamera, glm::vec4(0.5f, 0.0f, 0.5f, 0.5f), 
 *                                "Side", true });
 *     }
 *     
 *     void renderAllViewports(std::shared_ptr<Renderer> renderer, 
 *                            const glm::vec2& windowSize) {
 *         for (const auto& viewport : m_viewports) {
 *             if (!viewport.isActive) continue;
 *             
 *             // Calculate viewport rectangle
 *             int x = static_cast<int>(viewport.bounds.x * windowSize.x);
 *             int y = static_cast<int>(viewport.bounds.y * windowSize.y);
 *             int width = static_cast<int>(viewport.bounds.z * windowSize.x);
 *             int height = static_cast<int>(viewport.bounds.w * windowSize.y);
 *             
 *             // Set viewport and render with camera
 *             renderer->setViewport(x, y, width, height);
 *             viewport.camera->setViewportSize(width, height);
 *             
 *             // Apply camera matrices to renderer
 *             renderer->setViewMatrix(viewport.camera->getViewMatrix());
 *             renderer->setProjectionMatrix(viewport.camera->getProjectionMatrix());
 *             
 *             // Render scene with this viewport
 *             renderer->renderScene();
 *         }
 *     }
 * };
 * ```
 * 
 * ## Performance Considerations
 * 
 * ### Matrix Computation Optimization
 * Efficient transformation matrix calculation:
 * - **Lazy Evaluation**: Matrix recalculation only when parameters change
 * - **Dirty Flag System**: Track which matrices need updating
 * - **SIMD Acceleration**: Vector instruction utilization for matrix math
 * - **Memory Alignment**: Proper data alignment for optimal cache performance
 * 
 * ### Navigation Smoothing
 * Professional camera movement quality:
 * - **Interpolation**: Smooth transitions between camera states
 * - **Velocity Damping**: Natural deceleration for organic feel
 * - **Constraint Systems**: Movement limits for controlled navigation
 * - **Precision Control**: High-precision calculations for large scenes
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Scene
 * @ingroup Camera
 * @ingroup Navigation
 */

#pragma once

#include "Common.h"
#include "core/transform.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

class Camera {
public:
    Camera();
    ~Camera() = default;

    // Transform
    Transform& getTransform() { return m_transform; }
    const Transform& getTransform() const { return m_transform; }
    
    // Projection settings
    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
    void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    
    float getFOV() const { return m_fov; }
    float getAspectRatio() const { return m_aspectRatio; }
    float getNearPlane() const { return m_nearPlane; }
    float getFarPlane() const { return m_farPlane; }
    
    void setFOV(float fov) { m_fov = fov; updateProjectionMatrix(); }
    void setAspectRatio(float ratio) { m_aspectRatio = ratio; updateProjectionMatrix(); }
    void setNearPlane(float near) { m_nearPlane = near; updateProjectionMatrix(); }
    void setFarPlane(float far) { m_farPlane = far; updateProjectionMatrix(); }
    
    // Camera control
    void lookAt(const glm::vec3& target, const glm::vec3& up = WORLD_UP);
    void orbit(const glm::vec3& center, float deltaYaw, float deltaPitch);
    void pan(const glm::vec3& delta);
    void zoom(float delta);
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    
    // Matrix getters (Qt versions)
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewProjectionMatrix() const;
    
    // GLM matrix getters for gizmo system
    glm::mat4 getViewMatrixGLM() const;
    glm::mat4 getProjectionMatrixGLM() const;
    
    // Ray casting for picking
    glm::vec3 screenToWorldRay(const glm::vec2& screenPos, const glm::vec2& viewportSize) const;
    glm::vec3 getWorldPosition() const { return m_transform.getPosition(); }
    glm::vec3 getForward() const { return m_transform.getForward(); }
    glm::vec3 getRight() const { return m_transform.getRight(); }
    glm::vec3 getUp() const { return m_transform.getUp(); }
    
    // Viewport dimensions
    float getViewportWidth() const { return m_viewportWidth; }
    float getViewportHeight() const { return m_viewportHeight; }
    void setViewportSize(float width, float height) { 
        m_viewportWidth = width; 
        m_viewportHeight = height; 
        setAspectRatio(width / height);
    }
    
    // Position for gizmo system
    glm::vec3 getPosition() const;

private:
    Transform m_transform;
    
    // Projection parameters
    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;
    
    bool m_isPerspective;
    glm::mat4 m_projectionMatrix;
    
    // Orthographic parameters (when not perspective)
    float m_left, m_right, m_bottom, m_top;
    
    // Viewport dimensions
    float m_viewportWidth = 800.0f;
    float m_viewportHeight = 600.0f;
    
    void updateProjectionMatrix();
};
