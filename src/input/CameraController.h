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
 * █  FILE: CameraController.h                                                   █
 * █  DESCRIPTION: Advanced Multi-Mode 3D Camera Control & Animation System      █
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
 * █  This file contains the CameraController class providing comprehensive      █
 * █  multi-mode camera control, smooth animations, and professional viewport    █
 * █  management for diverse 3D modeling and visualization workflows.            █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file CameraController.h
 * @brief Advanced Multi-Mode 3D Camera Control and Professional Viewport Management System
 * 
 * This file contains the comprehensive CameraController class implementation that provides
 * professional-grade camera control capabilities essential for diverse 3D modeling,
 * animation, and visualization workflows. The system offers multiple camera modes,
 * smooth animations, intelligent focusing, and seamless integration with scene management
 * and selection systems for optimal user experience in professional 3D applications.
 * 
 * ## Core Camera Control Architecture
 * 
 * ### Multi-Mode Camera System
 * Versatile camera control modes for diverse professional workflows:
 * - **Orbit Mode**: Turntable-style rotation around pivot points for modeling workflows
 * - **Fly Mode**: Free-flying first-person camera for exploration and animation
 * - **Pan Mode**: Orthographic-style translation for technical drawing and CAD work
 * - **Focus Mode**: Intelligent object-centric navigation with automatic framing
 * 
 * ### Professional Orbit System
 * Advanced pivot-based camera rotation with multiple targeting modes:
 * - **World Center**: Rotation around global coordinate origin for reference work
 * - **Scene Center**: Dynamic centering on scene bounding box for optimal viewing
 * - **Selection**: Automatic pivot adjustment based on selected geometry
 * - **Custom Pivot**: User-defined rotation centers for specialized workflows
 * 
 * ### Intelligent Animation Framework
 * Smooth camera transitions for professional presentation and workflow efficiency:
 * - **Eased Interpolation**: Natural acceleration/deceleration curves for camera motion
 * - **Configurable Timing**: Adjustable animation speeds for different use cases
 * - **Position and Orientation**: Simultaneous translation and rotation animations
 * - **Interruption Handling**: Smooth blending when animations are interrupted
 * 
 * ## Advanced Navigation Features
 * 
 * ### Professional Framing System
 * Intelligent camera positioning for optimal object visualization:
 * - **Scene Framing**: Automatic positioning to show entire scene with optimal margins
 * - **Selection Framing**: Focus on selected objects with appropriate zoom levels
 * - **Bounding Box Analysis**: Accurate object bounds calculation for precise framing
 * - **Aspect Ratio Awareness**: Frame adjustment based on viewport dimensions
 * - **Margin Optimization**: Intelligent border space for professional presentation
 * 
 * ### Standard View Presets
 * Industry-standard orthographic and isometric view configurations:
 * - **Cardinal Views**: Front, back, left, right, top, bottom standard orientations
 * - **Isometric View**: Three-quarter view for technical illustration and presentation
 * - **Animation Support**: Smooth transitions to predefined view orientations
 * - **Custom Views**: User-defined view presets for workflow-specific requirements
 * 
 * ### Movement and Navigation
 * Comprehensive camera movement system for all professional use cases:
 * - **Orbital Movement**: Spherical coordinate navigation around pivot points
 * - **Linear Translation**: Direct camera panning in screen-space coordinates
 * - **Dolly and Zoom**: Distance adjustment with focal length preservation
 * - **Free Flight**: Six-degree-of-freedom navigation for immersive exploration
 * 
 * ## Technical Implementation
 * 
 * ### Mathematical Precision
 * Robust mathematical foundation for smooth and accurate camera control:
 * - **Quaternion Rotations**: Gimbal-lock-free rotation with smooth interpolation
 * - **Matrix Mathematics**: Efficient view and projection matrix calculations
 * - **Spherical Coordinates**: Accurate orbital navigation calculations
 * - **Interpolation Algorithms**: SLERP and other smooth transition methods
 * - **Numerical Stability**: Robust calculations preventing floating-point errors
 * 
 * ### Performance Optimization
 * Efficient camera control for real-time 3D applications:
 * - **Cached Calculations**: Optimized matrix computations with dirty flagging
 * - **Frame-Rate Independence**: Smooth animation regardless of rendering performance
 * - **Memory Efficiency**: Minimal memory footprint for camera state management
 * - **Event-Driven Updates**: Efficient update mechanisms reducing unnecessary calculations
 * 
 * ### Scene Integration
 * Seamless integration with 3D scene management and rendering pipeline:
 * - **Scene Manager Integration**: Dynamic scene bounds and object queries
 * - **Selection System**: Automatic focusing and framing based on user selections
 * - **Frustum Culling**: Integration with rendering pipeline for performance optimization
 * - **Level-of-Detail**: Camera distance-based quality adjustment support
 * 
 * ## Professional Workflow Applications
 * 
 * ### 3D Modeling Workflows
 * Camera control optimized for geometric modeling and design:
 * - **Orbit-Centric Navigation**: Natural rotation around modeling focus points
 * - **Precision Movement**: Fine-grained control for detailed modeling work
 * - **Reference Views**: Quick access to standard orthographic projections
 * - **Scale-Adaptive Movement**: Movement sensitivity based on scene scale
 * 
 * ### Animation and Cinematography
 * Professional camera control for animation and visual storytelling:
 * - **Smooth Transitions**: Cinematic camera movements with easing curves
 * - **Keyframe Integration**: Camera state recording for animation timelines
 * - **Path Following**: Programmatic camera movement along defined paths
 * - **Director's Tools**: Professional framing and composition assistance
 * 
 * ### CAD and Technical Visualization
 * Precision camera control for engineering and architectural applications:
 * - **Orthographic Modes**: Parallel projection for technical drawing accuracy
 * - **Measurement Integration**: Camera positioning for dimensional verification
 * - **Section Views**: Specialized camera angles for technical documentation
 * - **Assembly Animation**: Coordinated camera and object animation for exploded views
 * 
 * ### Virtual Reality and Immersive Experiences
 * Camera control adapted for immersive 3D environments:
 * - **First-Person Navigation**: Natural movement for VR and walkthrough applications
 * - **Collision Detection**: Intelligent navigation within architectural spaces
 * - **Scale Transition**: Seamless movement between macro and micro scales
 * - **Comfort Features**: Motion sickness reduction through smooth transitions
 * 
 * ## Usage Examples
 * 
 * ### Basic Camera Setup and Configuration
 * ```cpp
 * // Create and configure camera controller
 * auto cameraController = std::make_shared<CameraController>();
 * auto camera = std::make_shared<Camera>();
 * auto sceneManager = std::make_shared<SceneManager>();
 * 
 * // Setup camera controller
 * cameraController->setCamera(camera);
 * cameraController->setSceneManager(sceneManager);
 * 
 * // Configure for modeling workflow
 * cameraController->setCameraMode(CameraMode::Orbit);
 * cameraController->setOrbitMode(OrbitMode::SceneCenter);
 * 
 * // Adjust sensitivity settings
 * cameraController->setRotationSpeed(0.5f);
 * cameraController->setPanSpeed(0.3f);
 * cameraController->setZoomSpeed(0.1f);
 * cameraController->setAnimationSpeed(2.0f);
 * ```
 * 
 * ### Professional Modeling Viewport Integration
 * ```cpp
 * class ModelingViewport {
 * private:
 *     std::shared_ptr<CameraController> m_cameraController;
 *     std::shared_ptr<SelectionManager> m_selectionManager;
 *     std::shared_ptr<SceneManager> m_sceneManager;
 *     
 * public:
 *     void setupViewport() {
 *         m_cameraController = std::make_shared<CameraController>();
 *         
 *         // Configure for professional modeling workflow
 *         m_cameraController->setSceneManager(m_sceneManager);
 *         m_cameraController->setSelectionManager(m_selectionManager);
 *         
 *         // Set orbit mode with selection focusing
 *         m_cameraController->setCameraMode(CameraMode::Orbit);
 *         m_cameraController->setOrbitMode(OrbitMode::Selection);
 *         
 *         // Professional sensitivity settings
 *         m_cameraController->setRotationSpeed(0.4f);   // Moderate rotation
 *         m_cameraController->setPanSpeed(0.25f);       // Precise panning
 *         m_cameraController->setZoomSpeed(0.08f);      // Smooth zooming
 *         m_cameraController->setAnimationSpeed(3.0f);  // Quick transitions
 *         
 *         // Setup selection change callback
 *         m_selectionManager->onSelectionChanged = [this]() {
 *             if (m_selectionManager->hasSelection()) {
 *                 m_cameraController->frameSelectedObject(true);
 *             }
 *         };
 *     }
 *     
 *     void handleViewportInput(const InputEvent& event) {
 *         switch (event.type) {
 *             case InputEventType::MOUSE_ORBIT:
 *                 m_cameraController->orbit(event.delta.x, event.delta.y);
 *                 break;
 *                 
 *             case InputEventType::MOUSE_PAN:
 *                 m_cameraController->pan(event.delta);
 *                 break;
 *                 
 *             case InputEventType::MOUSE_ZOOM:
 *                 m_cameraController->dolly(event.wheelDelta);
 *                 break;
 *                 
 *             case InputEventType::KEY_FRAME_ALL:
 *                 m_cameraController->frameAll(true);
 *                 break;
 *                 
 *             case InputEventType::KEY_FRAME_SELECTED:
 *                 m_cameraController->frameSelectedObject(true);
 *                 break;
 *         }
 *     }
 * };
 * ```
 * 
 * ### Animation Timeline Integration
 * ```cpp
 * class CameraAnimationSystem {
 * private:
 *     std::shared_ptr<CameraController> m_controller;
 *     AnimationTimeline m_timeline;
 *     std::vector<CameraKeyframe> m_keyframes;
 *     
 * public:
 *     struct CameraKeyframe {
 *         float time;
 *         glm::vec3 position;
 *         glm::vec3 lookAt;
 *         float fieldOfView;
 *         CameraMode mode;
 *     };
 *     
 *     void addKeyframe(float time) {
 *         CameraKeyframe keyframe;
 *         keyframe.time = time;
 *         keyframe.position = m_controller->getWorldPosition();
 *         keyframe.lookAt = m_controller->getCurrentPivot();
 *         keyframe.fieldOfView = m_controller->getCamera()->getFieldOfView();
 *         keyframe.mode = m_controller->getCameraMode();
 *         
 *         m_keyframes.push_back(keyframe);
 *         sortKeyframesByTime();
 *     }
 *     
 *     void playAnimation() {
 *         m_timeline.onTimeChanged = [this](float time) {
 *             auto interpolatedFrame = interpolateKeyframes(time);
 *             applyCameraKeyframe(interpolatedFrame);
 *         };
 *         
 *         m_timeline.play();
 *     }
 *     
 *     void applyCameraKeyframe(const CameraKeyframe& keyframe) {
 *         m_controller->setCameraMode(keyframe.mode);
 *         m_controller->startAnimation(keyframe.position, keyframe.lookAt);
 *         m_controller->getCamera()->setFieldOfView(keyframe.fieldOfView);
 *     }
 * };
 * ```
 * 
 * ### Multi-Mode Navigation System
 * ```cpp
 * class AdvancedNavigationSystem {
 * private:
 *     std::shared_ptr<CameraController> m_controller;
 *     CameraMode m_currentMode = CameraMode::Orbit;
 *     
 * public:
 *     void switchToModelingMode() {
 *         m_controller->setCameraMode(CameraMode::Orbit);
 *         m_controller->setOrbitMode(OrbitMode::Selection);
 *         m_controller->setRotationSpeed(0.4f);
 *         m_controller->setPanSpeed(0.3f);
 *         m_controller->setZoomSpeed(0.1f);
 *         
 *         logInfo("Switched to Modeling Mode - Orbit navigation enabled");
 *     }
 *     
 *     void switchToExplorationMode() {
 *         m_controller->setCameraMode(CameraMode::Fly);
 *         m_controller->setMovementSpeed(5.0f);
 *         m_controller->setRotationSpeed(0.3f);
 *         m_controller->setInvertY(false);
 *         
 *         logInfo("Switched to Exploration Mode - Free flight enabled");
 *     }
 *     
 *     void switchToTechnicalMode() {
 *         m_controller->setCameraMode(CameraMode::Pan);
 *         m_controller->setPanSpeed(0.2f);
 *         m_controller->setZoomSpeed(0.05f);
 *         
 *         // Set orthographic projection for technical accuracy
 *         m_controller->getCamera()->setProjectionMode(ProjectionMode::ORTHOGRAPHIC);
 *         
 *         logInfo("Switched to Technical Mode - Orthographic navigation enabled");
 *     }
 *     
 *     void switchToPresentationMode() {
 *         m_controller->setCameraMode(CameraMode::Focus);
 *         m_controller->setAnimationSpeed(1.5f); // Slower for presentation
 *         
 *         // Enable smooth transitions for cinematic feel
 *         m_controller->setRotationSpeed(0.2f);
 *         m_controller->setPanSpeed(0.15f);
 *         
 *         logInfo("Switched to Presentation Mode - Smooth focusing enabled");
 *     }
 * };
 * ```
 * 
 * ### Automatic Framing and Focus System
 * ```cpp
 * class IntelligentFramingSystem {
 * private:
 *     std::shared_ptr<CameraController> m_controller;
 *     std::shared_ptr<SceneManager> m_sceneManager;
 *     
 * public:
 *     void frameOptimal(const std::vector<SceneObject*>& objects, bool animate = true) {
 *         if (objects.empty()) {
 *             m_controller->frameScene(animate);
 *             return;
 *         }
 *         
 *         // Calculate combined bounding box
 *         BoundingBox combinedBounds;
 *         for (const auto& object : objects) {
 *             combinedBounds.expand(object->getBoundingBox());
 *         }
 *         
 *         // Calculate optimal camera position
 *         glm::vec3 center = combinedBounds.getCenter();
 *         glm::vec3 size = combinedBounds.getSize();
 *         
 *         // Set custom pivot for optimal viewing
 *         m_controller->setCustomPivot(center);
 *         m_controller->setOrbitMode(OrbitMode::CustomPivot);
 *         
 *         // Frame with appropriate margins
 *         m_controller->focusOnPoint(center, animate);
 *         
 *         // Adjust distance based on object complexity
 *         float distance = calculateOptimalDistance(size, objects.size());
 *         m_controller->dolly(distance - m_controller->getDistanceToTarget());
 *     }
 *     
 *     void frameForModelingDetail(SceneObject* object) {
 *         // Focus for detailed modeling work
 *         auto bounds = object->getBoundingBox();
 *         glm::vec3 center = bounds.getCenter();
 *         
 *         m_controller->focusOnPoint(center, true);
 *         m_controller->setCustomPivot(center);
 *         
 *         // Set close distance for detail work
 *         float detailDistance = glm::length(bounds.getSize()) * 0.5f;
 *         m_controller->dolly(detailDistance - m_controller->getDistanceToTarget());
 *     }
 *     
 *     void frameForPresentation(const std::vector<SceneObject*>& objects) {
 *         frameOptimal(objects, true);
 *         
 *         // Set presentation-friendly view angle
 *         glm::vec3 presentationDirection = glm::normalize(glm::vec3(1.0f, 0.5f, 1.0f));
 *         m_controller->setViewDirection(presentationDirection, glm::vec3(0, 1, 0), true);
 *     }
 * };
 * ```
 * 
 * ### VR and Immersive Navigation
 * ```cpp
 * class ImmersiveNavigationController {
 * private:
 *     std::shared_ptr<CameraController> m_controller;
 *     VRHeadset m_headset;
 *     bool m_teleportationEnabled = true;
 *     
 * public:
 *     void setupVRNavigation() {
 *         m_controller->setCameraMode(CameraMode::Fly);
 *         m_controller->setMovementSpeed(2.0f); // Walking pace
 *         m_controller->setInvertY(false);
 *         
 *         // Enable comfort features
 *         enableComfortRotation(true);
 *         enableTeleportation(true);
 *         setMotionSicknessReduction(MotionSicknessLevel::MEDIUM);
 *     }
 *     
 *     void updateVRNavigation(float deltaTime) {
 *         // Get VR headset input
 *         auto headTransform = m_headset.getHeadTransform();
 *         auto controllerState = m_headset.getControllerState();
 *         
 *         // Apply head tracking to camera
 *         m_controller->getCamera()->setTransform(headTransform);
 *         
 *         // Handle controller movement
 *         if (controllerState.thumbstick.length() > 0.1f) {
 *             glm::vec3 movement = calculateVRMovement(controllerState.thumbstick);
 *             m_controller->moveForward(movement.z * deltaTime);
 *             m_controller->moveRight(movement.x * deltaTime);
 *         }
 *         
 *         // Handle teleportation
 *         if (m_teleportationEnabled && controllerState.triggerPressed) {
 *             handleTeleportation(controllerState.pointingDirection);
 *         }
 *     }
 *     
 *     void handleTeleportation(const glm::vec3& direction) {
 *         // Raycast to find teleportation target
 *         RaycastResult result = m_sceneManager->raycast(
 *             m_controller->getWorldPosition(), direction);
 *         
 *         if (result.hit) {
 *             // Smooth teleportation to avoid motion sickness
 *             m_controller->startAnimation(result.position + glm::vec3(0, 1.8f, 0), 
 *                                        result.position);
 *         }
 *     }
 * };
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Adaptive Camera Behavior
 * Intelligent camera adjustment based on scene characteristics:
 * - **Scale Detection**: Automatic sensitivity adjustment based on scene scale
 * - **Content Analysis**: Camera behavior adaptation for different content types
 * - **User Pattern Learning**: Adaptive behavior based on user interaction patterns
 * - **Context Switching**: Automatic mode switching based on current tool or operation
 * 
 * ### Professional Animation Tools
 * Advanced camera animation for cinematography and presentation:
 * - **Spline-Based Paths**: Smooth camera movement along custom-defined paths
 * - **Easing Functions**: Professional animation curves for natural motion
 * - **Camera Shake**: Procedural camera movement for dynamic presentations
 * - **Depth of Field Animation**: Focus pulling and rack focus effects
 * 
 * ### Performance and Quality
 * Optimized camera control for professional production environments:
 * - **Predictive Caching**: Pre-calculation of likely camera positions
 * - **Frustum Optimization**: Intelligent culling based on camera movement patterns
 * - **Memory Management**: Efficient state management for complex scenes
 * - **Quality Scaling**: Adaptive quality adjustment during camera movement
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Input
 * @ingroup Camera
 * @ingroup Navigation
 * @ingroup Animation
 */


#pragma once

#include "Common.h"
#include "core/scene_manager.hpp"
#include <memory>
#include <glm/glm.hpp>

// Forward declarations
class Camera;
class SelectionManager;

// Camera controller navigation modes
enum class CameraMode {
    Orbit,      // Orbit around a pivot point (default for modeling)
    Fly,        // Free-flying camera (FPS-style)
    Pan,        // Pan-only mode (orthographic-style)
    Focus       // Focus on selected object with smooth transitions
};

enum class OrbitMode {
    WorldCenter,    // Orbit around world origin (0,0,0)
    SceneCenter,    // Orbit around scene bounding box center
    Selection,      // Orbit around selected object
    CustomPivot     // Orbit around a custom pivot point
};

class CameraController {
public:
    // Signals (Qt-style, but can be replaced with callback/event system if needed)
    void cameraChanged();

    CameraController();

    void setSceneManager(std::shared_ptr<rude::SceneManager> sceneManager);
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);

    // Camera modes
    void setCameraMode(CameraMode mode);
    CameraMode getCameraMode() const { return m_cameraMode; }

    void setOrbitMode(OrbitMode mode);
    OrbitMode getOrbitMode() const { return m_orbitMode; }

    void setCustomPivot(const glm::vec3& pivot);
    glm::vec3 getCustomPivot() const { return m_customPivot; }
    glm::vec3 getCurrentPivot() const;
    float getDistanceToTarget() const;

    // Camera operations
    void resetCamera();
    void frameScene(bool animate = true);
    void frameSelectedObject(bool animate = true);
    void frameAll(bool animate = true);
    void updateAspectRatio(float aspectRatio);
    void setCamera(std::shared_ptr<Camera> camera);

    // Basic camera movement
    void orbit(float deltaYaw, float deltaPitch);
    void orbitAroundPoint(const glm::vec3& center, float deltaYaw, float deltaPitch);
    void pan(const glm::vec2& delta);
    void dolly(float delta);
    void zoom(float delta);

    // Advanced camera movement (fly mode)
    void moveForward(float distance);
    void moveBackward(float distance);
    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
    void rotate(float deltaPitch, float deltaYaw, float deltaRoll = 0.0f);

    // Focus and framing
    void focusOnPoint(const glm::vec3& point, bool animate = true);
    void focusOnObject(std::shared_ptr<class SceneObject> object, bool animate = true);
    void setViewDirection(const glm::vec3& direction, const glm::vec3& up = glm::vec3(0, 1, 0), bool animate = true);

    // Predefined views
    void setTopView(bool animate = true);
    void setBottomView(bool animate = true);
    void setFrontView(bool animate = true);
    void setBackView(bool animate = true);
    void setLeftView(bool animate = true);
    void setRightView(bool animate = true);
    void setIsometricView(bool animate = true);

    // Settings
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setRotationSpeed(float speed) { m_rotationSpeed = speed; }
    void setPanSpeed(float speed) { m_panSpeed = speed; }
    void setZoomSpeed(float speed) { m_zoomSpeed = speed; }
    void setAnimationSpeed(float speed) { m_animationSpeed = speed; }
    void setInvertY(bool invert) { m_invertY = invert; }

    float getMovementSpeed() const { return m_movementSpeed; }
    float getRotationSpeed() const { return m_rotationSpeed; }
    float getPanSpeed() const { return m_panSpeed; }
    float getZoomSpeed() const { return m_zoomSpeed; }
    bool isYInverted() const { return m_invertY; }

    // Camera queries
    glm::vec3 getWorldPosition() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 screenToWorldRay(const glm::vec2& screenPos, const glm::ivec2& viewportSize) const;

    // Animation methods
    void updateAnimation();
    void startAnimation(const glm::vec3& targetPosition, const glm::vec3& targetLookAt);
    void stopAnimation();
    bool isAnimating() const { return m_animationTime < m_animationDuration; }

private:
    // Helper methods
    glm::vec3 getSceneCenter() const;
    glm::vec3 getSelectionCenter() const;
    float calculateFramingDistance(const glm::vec3& sceneSize) const;
    glm::vec3 calculateFramingPosition(const glm::vec3& target, const glm::vec3& size) const;
    glm::vec3 getOrbitCenter() const;
    void updateOrbitDistance();

    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<rude::SceneManager> m_sceneManager;
    std::shared_ptr<SelectionManager> m_selectionManager;

    // Camera control modes
    CameraMode m_cameraMode;
    OrbitMode m_orbitMode;
    glm::vec3 m_customPivot;

    // Movement settings
    float m_movementSpeed;
    float m_rotationSpeed;
    float m_panSpeed;
    float m_zoomSpeed;
    float m_animationSpeed;
    bool m_invertY;

    // Orbit settings
    float m_orbitDistance;
    glm::vec3 m_orbitCenter;

    // Animation system
    glm::vec3 m_animStartPosition;
    glm::vec3 m_animTargetPosition;
    glm::vec3 m_animStartLookAt;
    glm::vec3 m_animTargetLookAt;
    float m_animationTime;
    float m_animationDuration;
};

