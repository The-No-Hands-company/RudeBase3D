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
 * █  FILE: MayaCameraController.h                                               █
 * █  DESCRIPTION: Professional Maya-Style Camera Navigation & Control System   █
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
 * █  This file contains the MayaCameraController class implementing the         █
 * █  industry-standard Maya navigation paradigm with professional animation     █
 * █  workflow support, pivot-based camera control, and smooth transitions.     █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file MayaCameraController.h
 * @brief Professional Maya-Style Camera Navigation and Control System
 * 
 * This file contains the comprehensive MayaCameraController class implementation
 * that provides industry-standard Maya navigation paradigm essential for professional
 * 3D animation and modeling workflows. The system implements the precise Alt+mouse
 * interaction patterns established by Autodesk Maya, including tumble (orbit), track
 * (pan), and dolly (zoom) operations with intelligent pivot management, smooth camera
 * transitions, and professional animation-oriented camera control suitable for
 * complex production environments.
 * 
 * ## Maya Navigation Paradigm Overview
 * 
 * ### Industry-Standard Interaction Patterns
 * Professional animation industry navigation conventions:
 * - **Alt + Left Mouse Button (Tumble)**: Orbit camera around pivot point with constrained motion
 * - **Alt + Middle Mouse Button (Track)**: Pan camera and pivot point in screen space
 * - **Alt + Right Mouse Button (Dolly)**: Zoom camera toward/away from pivot point
 * - **Mouse Wheel**: Quick zoom operation with immediate response
 * - **F Key**: Frame selected objects or entire scene with smooth animation
 * - **Ctrl + Alt + Left Mouse**: Rotate camera around view center for examination
 * 
 * ### Professional Workflow Integration
 * Seamless integration with animation and modeling pipelines:
 * - **Smart Pivot Management**: Automatic pivot calculation based on selection or scene content
 * - **Animation-Friendly Transitions**: Smooth camera movements suitable for keyframe animation
 * - **Selection-Aware Navigation**: Frame and focus operations based on current object selection
 * - **Precision Control**: Fine-tuned sensitivity settings for detailed animation work
 * - **Multi-Viewport Support**: Consistent behavior across multiple viewport configurations
 * 
 * ## Advanced Camera Control Features
 * 
 * ### Intelligent Pivot System
 * Professional pivot-based camera control with adaptive behavior:
 * - **Selection-Based Pivots**: Automatic pivot positioning based on selected objects
 * - **Scene Center Calculation**: Dynamic scene bounding calculation for optimal framing
 * - **Smart Pivot Updates**: Context-sensitive pivot repositioning during navigation
 * - **Manual Pivot Control**: User-defined pivot points for specialized camera work
 * - **Distance Preservation**: Orbit distance maintenance during pivot transitions
 * 
 * ### Animation System Integration
 * Smooth camera transitions suitable for professional animation workflows:
 * - **Eased Transitions**: Smooth interpolation curves for natural camera movement
 * - **Keyframeable Properties**: Animation system compatible camera state management
 * - **Frame Operations**: Professional "F" key framing with customizable timing
 * - **Camera Bookmarks**: Save and restore camera positions for complex scenes
 * - **Smooth Zoom**: Graduated zoom response preventing jarring camera jumps
 * 
 * ### Professional Interaction Modes
 * Comprehensive interaction mode support for diverse animation tasks:
 * - **Tumble Mode**: Constrained orbital rotation preserving up vector orientation
 * - **Track Mode**: Screen-space panning maintaining view orientation and zoom
 * - **Dolly Mode**: Distance-based zoom with pivot preservation and smooth scaling
 * - **View Rotate Mode**: Free camera rotation around view center for examination
 * - **Hybrid Interactions**: Combined mode support for complex camera movements
 * 
 * ## Technical Implementation Architecture
 * 
 * ### State Management System
 * Robust interaction state tracking for reliable camera control:
 * - **Modal Interaction**: Clear separation of tumble, track, dolly, and view rotation modes
 * - **Input Event Processing**: Comprehensive mouse and keyboard event handling
 * - **Modifier Key Support**: Complex Alt, Ctrl, Shift combination processing
 * - **Animation State**: Smooth transition state management with timing control
 * - **Session Persistence**: Camera state preservation across application sessions
 * 
 * ### Mathematical Foundation
 * Precision mathematics for professional 3D camera control:
 * - **Spherical Coordinates**: Robust orbital motion using spherical coordinate mathematics
 * - **Quaternion Rotations**: Smooth rotation interpolation preventing gimbal lock
 * - **World-Space Transformations**: Accurate world coordinate system camera positioning
 * - **Perspective Projection**: Professional perspective camera matrix calculation
 * - **Ray Casting**: Screen-to-world ray calculation for selection and interaction
 * 
 * ### Performance Optimization
 * Optimized camera control for real-time animation workflows:
 * - **Incremental Updates**: Efficient delta-based camera position updates
 * - **View Frustum Calculation**: Optimized projection matrix computation
 * - **Animation Interpolation**: Hardware-accelerated smooth transition calculations
 * - **Memory Management**: Efficient state storage and animation keyframe caching
 * - **Event Batching**: Input event consolidation for optimal performance
 * 
 * ## Usage Examples
 * 
 * ### Basic Maya Camera Setup
 * ```cpp
 * // Create and configure Maya-style camera controller for animation workflow
 * auto mayaController = std::make_shared<MayaCameraController>();
 * auto camera = std::make_shared<Camera>();
 * auto scene = std::make_shared<rude::Scene>();
 * auto selectionManager = std::make_shared<SelectionManager>();
 * 
 * // Setup camera integration
 * mayaController->setCamera(camera);
 * mayaController->setScene(scene);
 * mayaController->setSelectionManager(selectionManager);
 * 
 * // Configure for animation workflow
 * mayaController->setRotationSpeed(0.5f);       // Smooth rotation for keyframing
 * mayaController->setPanSpeed(0.3f);            // Precise panning control
 * mayaController->setZoomSpeed(0.1f);           // Gradual zoom for smooth transitions
 * mayaController->setInvertY(false);            // Standard Maya Y-axis orientation
 * mayaController->setSmartPivot(true);          // Enable intelligent pivot management
 * 
 * // Setup camera change notifications for animation integration
 * mayaController->cameraChangedCallback = [&]() {
 *     updateViewportDisplay();
 *     notifyAnimationSystem();
 * };
 * ```
 * 
 * ### Professional Animation Workflow Integration
 * ```cpp
 * class AnimationCameraManager {
 * private:
 *     std::shared_ptr<MayaCameraController> m_mayaController;
 *     AnimationTimeline m_timeline;
 *     std::vector<CameraKeyframe> m_cameraKeyframes;
 *     
 * public:
 *     void configureForCharacterAnimation() {
 *         // Setup for character animation workflow
 *         m_mayaController->setRotationSpeed(0.4f);    // Smooth character examination
 *         m_mayaController->setPanSpeed(0.25f);        // Precise framing control
 *         m_mayaController->setZoomSpeed(0.08f);       // Gradual zoom for detail work
 *         
 *         // Configure smart pivot for character focus
 *         m_mayaController->setSmartPivot(true);
 *         
 *         logInfo("Configured Maya camera for character animation workflow");
 *     }
 *     
 *     void configureForEnvironmentExploration() {
 *         // Setup for environment and set exploration
 *         m_mayaController->setRotationSpeed(0.6f);    // Faster exploration rotation
 *         m_mayaController->setPanSpeed(0.4f);         // Broader panning movements
 *         m_mayaController->setZoomSpeed(0.15f);       // Quicker zoom for coverage
 *         
 *         // Focus on scene center for environment work
 *         glm::vec3 sceneCenter = m_mayaController->calculateSceneCenter();
 *         m_mayaController->setOrbitPivot(sceneCenter);
 *         
 *         logInfo("Configured Maya camera for environment exploration");
 *     }
 *     
 *     void frameSelectedForAnimation() {
 *         // Professional framing operation for animation
 *         if (hasSelectedObjects()) {
 *             m_mayaController->frameSelection(true);  // Smooth animated framing
 *             
 *             // Update pivot to selection center for future orbit operations
 *             glm::vec3 selectionCenter = m_mayaController->calculateSelectionCenter();
 *             m_mayaController->setOrbitPivot(selectionCenter);
 *             
 *             logInfo("Framed selection with animated transition");
 *         } else {
 *             m_mayaController->frameScene(true);      // Frame entire scene
 *             logInfo("Framed entire scene with animated transition");
 *         }
 *     }
 *     
 *     void createCameraKeyframe() {
 *         // Capture current camera state for animation keyframe
 *         CameraKeyframe keyframe;
 *         keyframe.position = m_mayaController->getWorldPosition();
 *         keyframe.viewMatrix = m_mayaController->getViewMatrix();
 *         keyframe.orbitPivot = m_mayaController->getOrbitPivot();
 *         keyframe.time = m_timeline.getCurrentTime();
 *         
 *         m_cameraKeyframes.push_back(keyframe);
 *         
 *         logInfo("Created camera keyframe at time: " + std::to_string(keyframe.time));
 *     }
 * };
 * ```
 * 
 * ### Advanced Pivot Management System
 * ```cpp
 * class AdvancedPivotManager {
 * private:
 *     std::shared_ptr<MayaCameraController> m_controller;
 *     std::vector<glm::vec3> m_pivotBookmarks;
 *     PivotCalculationMode m_currentMode;
 *     
 * public:
 *     void enableAdaptivePivoting() {
 *         m_controller->setSmartPivot(true);
 *         
 *         // Setup automatic pivot updates based on selection changes
 *         m_controller->selectionChangedCallback = [this](const auto& selection) {
 *             updatePivotForSelection(selection);
 *         };
 *         
 *         logInfo("Enabled adaptive pivoting system");
 *     }
 *     
 *     void updatePivotForSelection(const SelectionSet& selection) {
 *         if (selection.empty()) {
 *             // No selection - use scene center
 *             glm::vec3 sceneCenter = m_controller->calculateSceneCenter();
 *             m_controller->setOrbitPivot(sceneCenter);
 *         } else if (selection.size() == 1) {
 *             // Single object - use object center
 *             glm::vec3 objectCenter = calculateObjectCenter(selection.front());
 *             m_controller->setOrbitPivot(objectCenter);
 *         } else {
 *             // Multiple objects - use selection bounding center
 *             glm::vec3 selectionCenter = m_controller->calculateSelectionCenter();
 *             m_controller->setOrbitPivot(selectionCenter);
 *         }
 *     }
 *     
 *     void bookmarkCurrentPivot() {
 *         glm::vec3 currentPivot = m_controller->getOrbitPivot();
 *         m_pivotBookmarks.push_back(currentPivot);
 *         
 *         logInfo("Bookmarked pivot position: " + 
 *                vec3ToString(currentPivot));
 *     }
 *     
 *     void restorePivotBookmark(size_t index) {
 *         if (index < m_pivotBookmarks.size()) {
 *             m_controller->setOrbitPivot(m_pivotBookmarks[index]);
 *             
 *             logInfo("Restored pivot bookmark " + std::to_string(index));
 *         }
 *     }
 *     
 *     void createInteractivePivotMode() {
 *         // Enable interactive pivot placement mode
 *         m_pivotPlacementMode = true;
 *         
 *         // Setup click-to-place pivot functionality
 *         m_controller->clickCallback = [this](const glm::ivec2& screenPos) {
 *             if (m_pivotPlacementMode) {
 *                 glm::vec3 worldPos = screenToWorldPosition(screenPos);
 *                 m_controller->setOrbitPivot(worldPos);
 *                 m_pivotPlacementMode = false;
 *                 
 *                 logInfo("Interactive pivot placed at: " + vec3ToString(worldPos));
 *             }
 *         };
 *     }
 * };
 * ```
 * 
 * ### Professional Input Event Processing
 * ```cpp
 * class MayaInputProcessor {
 * private:
 *     std::shared_ptr<MayaCameraController> m_controller;
 *     InputEventQueue m_eventQueue;
 *     
 * public:
 *     void processInputEvents() {
 *         while (!m_eventQueue.empty()) {
 *             auto event = m_eventQueue.front();
 *             m_eventQueue.pop();
 *             
 *             switch (event.type) {
 *                 case InputEventType::MOUSE_PRESS:
 *                     handleMousePress(event.mouseEvent);
 *                     break;
 *                     
 *                 case InputEventType::MOUSE_MOVE:
 *                     handleMouseMove(event.mouseEvent);
 *                     break;
 *                     
 *                 case InputEventType::MOUSE_RELEASE:
 *                     handleMouseRelease(event.mouseEvent);
 *                     break;
 *                     
 *                 case InputEventType::WHEEL:
 *                     handleWheelEvent(event.wheelEvent);
 *                     break;
 *                     
 *                 case InputEventType::KEY_PRESS:
 *                     handleKeyPress(event.keyEvent);
 *                     break;
 *             }
 *         }
 *     }
 *     
 *     void handleMousePress(const MouseEvent& event) {
 *         // Route Maya-specific mouse interactions
 *         if (event.modifiers & KeyboardModifier::Alt) {
 *             switch (event.button) {
 *                 case MouseButton::Left:
 *                     // Begin tumble operation
 *                     m_controller->tumbleStart(event.pos);
 *                     logDebug("Started Maya tumble operation");
 *                     break;
 *                     
 *                 case MouseButton::Middle:
 *                     // Begin track operation
 *                     m_controller->trackStart(event.pos);
 *                     logDebug("Started Maya track operation");
 *                     break;
 *                     
 *                 case MouseButton::Right:
 *                     // Begin dolly operation
 *                     m_controller->dollyStart(event.pos);
 *                     logDebug("Started Maya dolly operation");
 *                     break;
 *             }
 *         } else if ((event.modifiers & KeyboardModifier::Control) &&
 *                   (event.modifiers & KeyboardModifier::Alt) &&
 *                   event.button == MouseButton::Left) {
 *             // Begin view rotate operation
 *             m_controller->viewRotateStart(event.pos);
 *             logDebug("Started Maya view rotate operation");
 *         }
 *     }
 *     
 *     void handleKeyPress(const KeyEvent& event) {
 *         switch (event.key) {
 *             case KeyCode::F:
 *                 // Maya-style frame operation
 *                 if (hasSelectedObjects()) {
 *                     m_controller->frameSelection(true);
 *                 } else {
 *                     m_controller->frameScene(true);
 *                 }
 *                 break;
 *                 
 *             case KeyCode::A:
 *                 // Alt+A for frame all
 *                 if (event.modifiers & KeyboardModifier::Alt) {
 *                     m_controller->frameScene(true);
 *                 }
 *                 break;
 *                 
 *             case KeyCode::HOME:
 *                 // Reset to default camera position
 *                 m_controller->resetCamera();
 *                 break;
 *         }
 *     }
 * };
 * ```
 * 
 * ### Animation Integration System
 * ```cpp
 * class MayaAnimationIntegration {
 * private:
 *     std::shared_ptr<MayaCameraController> m_controller;
 *     AnimationCurveManager m_curveManager;
 *     std::vector<CameraKeyframe> m_keyframes;
 *     
 * public:
 *     void setupAnimationIntegration() {
 *         // Configure smooth transitions for animation work
 *         m_controller->setAnimationDuration(0.5f);  // Half-second transitions
 *         
 *         // Setup camera change notifications
 *         m_controller->cameraChangedCallback = [this]() {
 *             notifyAnimationSystem();
 *             updateTimelineDisplay();
 *         };
 *         
 *         // Configure for keyframe-friendly operation
 *         m_controller->setSmartPivot(true);
 *         m_controller->setRotationSpeed(0.3f);  // Smooth for keyframing
 *     }
 *     
 *     void recordCameraKeyframe() {
 *         CameraKeyframe keyframe;
 *         keyframe.position = m_controller->getWorldPosition();
 *         keyframe.rotation = extractRotationFromMatrix(m_controller->getViewMatrix());
 *         keyframe.pivot = m_controller->getOrbitPivot();
 *         keyframe.distance = m_controller->getOrbitDistance();
 *         keyframe.time = getCurrentAnimationTime();
 *         
 *         m_keyframes.push_back(keyframe);
 *         
 *         // Create animation curves for smooth interpolation
 *         createCameraAnimationCurves(keyframe);
 *         
 *         logInfo("Recorded camera keyframe at time: " + 
 *                std::to_string(keyframe.time));
 *     }
 *     
 *     void interpolateCameraPosition(float time) {
 *         // Smooth interpolation between camera keyframes
 *         auto [startKeyframe, endKeyframe] = findKeyframeInterval(time);
 *         
 *         if (startKeyframe && endKeyframe) {
 *             float t = calculateInterpolationWeight(time, 
 *                                                   startKeyframe->time, 
 *                                                   endKeyframe->time);
 *             
 *             // Smooth curve interpolation
 *             glm::vec3 position = m_curveManager.evaluatePositionCurve(time);
 *             glm::vec3 pivot = m_curveManager.evaluatePivotCurve(time);
 *             
 *             // Apply interpolated camera state
 *             m_controller->moveToPosition(position, pivot, false);
 *         }
 *     }
 * };
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Smart Pivot Management
 * Intelligent pivot calculation and management for professional workflows:
 * - **Selection-Aware Pivoting**: Automatic pivot updates based on object selection changes
 * - **Bounding Box Calculation**: Precise object and scene bounding box center calculation
 * - **Distance Preservation**: Orbit distance maintenance during pivot transitions
 * - **Interactive Pivot Placement**: Click-to-place pivot functionality for custom positioning
 * - **Pivot Bookmarking**: Save and restore pivot positions for complex scene navigation
 * 
 * ### Animation System Integration
 * Professional animation workflow support with smooth transitions:
 * - **Keyframe Recording**: Camera state capture for animation timeline integration
 * - **Smooth Interpolation**: Bezier curve interpolation between camera keyframes
 * - **Timeline Synchronization**: Frame-accurate camera playback with animation timeline
 * - **Camera Bookmarks**: Save and restore camera positions for efficient workflow
 * - **Live Animation**: Real-time camera animation during scene playback
 * 
 * ### Professional Workflow Features
 * Industry-standard features for demanding production environments:
 * - **Multi-Viewport Sync**: Synchronized camera control across multiple viewports
 * - **Camera Matching**: Match camera to view for compositing and integration work
 * - **Safe Frame Display**: Camera frame indicators for film and video production
 * - **Depth of Field Preview**: Real-time camera depth of field visualization
 * - **Camera Shake**: Procedural camera shake for dynamic scene enhancement
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Input
 * @ingroup Camera
 * @ingroup Animation
 * @ingroup Maya
 */

#pragma once

#include "ICameraController.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <set>
#include <functional>
#include "InputEvents.h"

// Forward declaration for rude::Scene
namespace rude { class Scene; }
class SelectionManager;

/**
 * @brief Professional Maya-style camera controller
 * 
 * Implements industry-standard Maya navigation:
 * - Alt + LMB = Tumble (orbit around pivot)
 * - Alt + MMB = Track (pan camera and pivot)
 * - Alt + RMB = Dolly (zoom in/out)
 * - Wheel = Quick zoom
 * - F = Frame selected/all
 * - Ctrl+Alt+LMB = Rotate around view center
 */
class MayaCameraController : public ICameraController {
public:
    // Cross-platform camera changed callback
    std::function<void()> cameraChangedCallback;
public:
    MayaCameraController();
    ~MayaCameraController() override;
    // ICameraController interface
    void setCamera(std::shared_ptr<Camera> camera) override;
    void setScene(std::shared_ptr<rude::Scene> scene) override;
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);

    bool handleMousePress(const MouseEvent& event) override;
    bool handleMouseMove(const MouseEvent& event) override;
    bool handleMouseRelease(const MouseEvent& event) override;
    bool handleWheel(const WheelEvent& event) override;
    bool handleKeyPress(const KeyEvent& event) override;
    bool handleKeyRelease(const KeyEvent& event) override;

    void resetCamera() override;
    void frameScene(bool animate = true) override;
    void frameSelection(bool animate = true) override;
    void updateAspectRatio(float aspectRatio) override;

    glm::vec3 getWorldPosition() const override;
    glm::mat4 getViewMatrix() const override;
    glm::mat4 getProjectionMatrix() const override;
    glm::vec3 screenToWorldRay(const glm::vec2& screenPos, const glm::ivec2& viewportSize) const override;

    void setMovementSpeed(float speed) override { m_movementSpeed = speed; }
    void setRotationSpeed(float speed) override { m_rotationSpeed = speed; }
    void setPanSpeed(float speed) override { m_panSpeed = speed; }
    void setZoomSpeed(float speed) override { m_zoomSpeed = speed; }
    void setInvertY(bool invert) override { m_invertY = invert; }

    float getMovementSpeed() const override { return m_movementSpeed; }
    float getRotationSpeed() const override { return m_rotationSpeed; }
    float getPanSpeed() const override { return m_panSpeed; }
    float getZoomSpeed() const override { return m_zoomSpeed; }
    bool isYInverted() const override { return m_invertY; }

    std::string getControllerName() const override { return "Maya"; }
    std::string getControllerDescription() const override;

    // Maya-specific features
    void setOrbitPivot(const glm::vec3& pivot);
    glm::vec3 getOrbitPivot() const { return m_orbitPivot; }
    void setSmartPivot(bool enabled) { m_smartPivot = enabled; }
    bool isSmartPivotEnabled() const { return m_smartPivot; }

    // View operations
    void tumbleStart(const glm::ivec2& startPos);
    void tumbleUpdate(const glm::ivec2& currentPos);
    void tumbleEnd();

    void trackStart(const glm::ivec2& startPos);
    void trackUpdate(const glm::ivec2& currentPos);
    void trackEnd();

    void dollyStart(const glm::ivec2& startPos);
    void dollyUpdate(const glm::ivec2& currentPos);
    void dollyEnd();

    void updateAnimation();

private:
    enum class InteractionMode {
        None,
        Tumble,    // Alt + LMB
        Track,     // Alt + MMB  
        Dolly,     // Alt + RMB
        ViewRotate // Ctrl + Alt + LMB
    };

    // Camera state
    glm::vec3 m_orbitPivot;
    float m_orbitDistance;
    bool m_smartPivot;

    // Interaction state
    InteractionMode m_currentMode;
    glm::ivec2 m_lastMousePos;
    glm::ivec2 m_interactionStartPos;
    std::set<int> m_pressedKeys;
    KeyboardModifier m_currentModifiers;

    // Settings
    float m_movementSpeed;
    float m_rotationSpeed;
    float m_panSpeed;
    float m_zoomSpeed;
    bool m_invertY;

    // Animation system
    // Animation system
    bool m_isAnimating;
    glm::vec3 m_animStartPos;
    glm::vec3 m_animTargetPos;
    glm::vec3 m_animStartPivot;
    glm::vec3 m_animTargetPivot;
    float m_animationTime;
    float m_animationDuration;
    float m_animationInterval; // in seconds

    // Helper methods
    bool isAltPressed() const;
    bool isCtrlPressed() const;
    bool isShiftPressed() const;

    void updateOrbitDistance();
    void updateSmartPivot(const glm::ivec2& screenPos);
    glm::vec3 calculateSceneCenter() const;
    glm::vec3 calculateSelectionCenter() const;
    float calculateFramingDistance(const glm::vec3& target, float objectSize) const;

    void startAnimation(const glm::vec3& targetPos, const glm::vec3& targetPivot);
    void stopAnimation();

    // Camera manipulation
    void orbitAroundPivot(float deltaYaw, float deltaPitch);
    void panCamera(const glm::vec3& worldDelta);
    void dollyCamera(float deltaDistance);
    void moveToPosition(const glm::vec3& position, const glm::vec3& target, bool animate = true);

    std::shared_ptr<SelectionManager> m_selectionManager;
    std::shared_ptr<rude::Scene> m_scene;
};
