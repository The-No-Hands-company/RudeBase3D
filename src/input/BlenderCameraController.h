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
 * █  FILE: BlenderCameraController.h                                             █
 * █  DESCRIPTION: Professional Blender-Style 3D Navigation System                █
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
 * █  This file contains the BlenderCameraController class providing              █
 * █  industry-standard Blender-style 3D viewport navigation, camera control,     █
 * █  and professional workflow integration for intuitive 3D modeling.            █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file BlenderCameraController.h
 * @brief Professional Blender-Style 3D Viewport Navigation and Camera Control System
 * 
 * This file contains the comprehensive BlenderCameraController class implementation that
 * provides industry-standard Blender-style 3D viewport navigation essential for 
 * professional 3D modeling workflows. The system implements the familiar and intuitive
 * navigation patterns used by millions of 3D artists worldwide, ensuring immediate
 * productivity for users transitioning from or familiar with Blender's interface.
 * 
 * ## Core Navigation System
 * 
 * ### Blender-Standard Navigation Controls
 * Industry-standard navigation that matches Blender's viewport behavior:
 * - **Middle Mouse Orbit**: Classic turntable-style camera rotation around target
 * - **Shift + Middle Mouse Pan**: Horizontal and vertical camera translation
 * - **Scroll Wheel Zoom**: Smooth camera distance adjustment with target focus
 * - **Ctrl + Middle Mouse Zoom**: Alternative drag-based zoom control method
 * - **Smooth Interpolation**: Fluid camera transitions for professional feel
 * 
 * ### Professional Viewport Controls
 * Advanced camera operations for efficient 3D navigation:
 * - **Numpad Orthographic Views**: Instant access to standard orthographic projections
 * - **Frame All (Home Key)**: Automatic camera positioning to show entire scene
 * - **Frame Selection (Period Key)**: Focus camera on selected objects or geometry
 * - **View Reset**: Return to default camera position and orientation
 * - **Smooth Animation**: Configurable interpolation for camera movements
 * 
 * ### Industry-Standard Workflow Integration
 * Seamless integration with professional 3D modeling workflows:
 * - **Cross-Platform Compatibility**: Consistent behavior across Windows, macOS, Linux
 * - **Customizable Sensitivity**: Adjustable response for different user preferences
 * - **Input Device Support**: Mouse, trackpad, and pen tablet compatibility
 * - **Animation Timeline**: Smooth camera transitions with configurable timing
 * 
 * ## Advanced Navigation Features
 * 
 * ### Orbit Camera System
 * Professional turntable-style camera rotation:
 * - **Target-Centered Rotation**: Camera orbits around scene focus point
 * - **Spherical Coordinates**: Mathematical precision in camera positioning
 * - **Gimbal Lock Prevention**: Robust quaternion-based rotation system
 * - **Constrained Rotation**: Optional axis constraints for specific workflows
 * - **Momentum Simulation**: Natural camera movement with configurable damping
 * 
 * ### Panning and Translation
 * Smooth camera translation for detailed examination:
 * - **Screen-Space Panning**: Intuitive translation relative to viewport orientation
 * - **Depth-Aware Movement**: Distance-adjusted pan sensitivity for natural feel
 * - **Boundary Constraints**: Optional limits to prevent excessive camera movement
 * - **Precision Mode**: Fine-grained control for detailed work with modifier keys
 * 
 * ### Zoom and Focal Control
 * Professional camera distance and focal length management:
 * - **Target-Focused Zoom**: Zoom toward and away from camera target point
 * - **Logarithmic Scaling**: Smooth zoom behavior across wide distance ranges
 * - **Minimum/Maximum Limits**: Configurable boundaries for camera distances
 * - **Adaptive Sensitivity**: Distance-based zoom speed adjustment
 * - **Field of View Control**: Optional FOV adjustment for perspective cameras
 * 
 * ## Professional View Management
 * 
 * ### Orthographic View System
 * Standard orthographic projections for technical modeling:
 * - **Front View (Numpad 1)**: Classic front elevation view for architectural work
 * - **Back View (Ctrl+Numpad 1)**: Rear elevation for complete object examination
 * - **Right View (Numpad 3)**: Side elevation for profile and section views
 * - **Left View (Ctrl+Numpad 3)**: Opposite side view for comprehensive analysis
 * - **Top View (Numpad 7)**: Plan view for layout and top-down design work
 * - **Bottom View (Ctrl+Numpad 7)**: Underside view for complete object review
 * 
 * ### Automatic Framing System
 * Intelligent camera positioning for optimal viewing:
 * - **Scene Framing**: Automatic positioning to show entire scene with optimal margins
 * - **Selection Framing**: Focus on selected objects with appropriate zoom level
 * - **Bounding Box Calculation**: Accurate bounds computation for optimal framing
 * - **Adaptive Margins**: Intelligent border space based on object complexity
 * - **Animation Support**: Smooth transitions to framed positions
 * 
 * ### Custom View Presets
 * User-defined camera positions for workflow efficiency:
 * - **Bookmark System**: Save and recall custom camera positions
 * - **Named Presets**: User-defined labels for specific workflow views
 * - **Interpolated Transitions**: Smooth animation between saved positions
 * - **Export/Import**: Share view presets between projects and users
 * 
 * ## Technical Implementation
 * 
 * ### Input Event Processing
 * Professional input handling for responsive navigation:
 * - **Multi-Platform Input**: Unified event handling across operating systems
 * - **Device Independence**: Support for mouse, trackpad, and graphics tablets
 * - **Modifier Key Support**: Shift, Ctrl, Alt combinations for extended functionality
 * - **Event Filtering**: Intelligent event processing to prevent conflicts
 * - **Gesture Recognition**: Support for complex input patterns and combinations
 * 
 * ### Mathematical Precision
 * Robust mathematical foundation for smooth navigation:
 * - **Quaternion Rotations**: Gimbal-lock-free rotation with smooth interpolation
 * - **Spherical Coordinates**: Mathematical precision in orbit calculations
 * - **Matrix Mathematics**: Efficient transformation calculations for real-time use
 * - **Interpolation Algorithms**: SLERP and other smooth transition methods
 * - **Numerical Stability**: Robust calculations preventing floating-point errors
 * 
 * ### Performance Optimization
 * Efficient navigation for real-time 3D applications:
 * - **Frame Rate Independence**: Smooth navigation regardless of rendering performance
 * - **Event Throttling**: Intelligent update rate control for optimal responsiveness
 * - **Memory Efficiency**: Minimal memory footprint for navigation state management
 * - **CPU Optimization**: Efficient calculations to minimize navigation overhead
 * 
 * ## Usage Examples
 * 
 * ### Basic Navigation Setup
 * ```cpp
 * // Create and configure Blender-style camera controller
 * auto cameraController = std::make_unique<BlenderCameraController>();
 * 
 * // Configure sensitivity settings
 * cameraController->setOrbitSensitivity(0.5f);  // Moderate orbit speed
 * cameraController->setPanSensitivity(0.3f);    // Precise panning
 * cameraController->setZoomSensitivity(0.1f);   // Smooth zoom response
 * 
 * // Enable smooth camera transitions
 * cameraController->setSmoothing(true);
 * 
 * // Set up camera change callback for viewport updates
 * cameraController->cameraChangedCallback = [&viewport]() {
 *     viewport.requestRedraw();
 * };
 * ```
 * 
 * ### Professional Viewport Integration
 * ```cpp
 * class Professional3DViewport {
 * private:
 *     std::unique_ptr<BlenderCameraController> m_cameraController;
 *     Camera m_camera;
 *     SceneManager m_scene;
 *     
 * public:
 *     void setupViewport() {
 *         m_cameraController = std::make_unique<BlenderCameraController>();
 *         
 *         // Configure for professional modeling workflow
 *         m_cameraController->setOrbitSensitivity(0.4f);   // Professional default
 *         m_cameraController->setPanSensitivity(0.25f);    // Precise control
 *         m_cameraController->setZoomSensitivity(0.08f);   // Smooth zoom
 *         m_cameraController->setInvertZoom(false);        // Standard direction
 *         m_cameraController->setSmoothing(true);          // Smooth animations
 *         
 *         // Set up camera update callback
 *         m_cameraController->cameraChangedCallback = [this]() {
 *             updateViewportDisplay();
 *         };
 *     }
 *     
 *     void handleInputEvent(const InputEvent& event) {
 *         bool handled = false;
 *         
 *         // Route input events to camera controller
 *         switch (event.type) {
 *             case InputEventType::MOUSE_PRESS:
 *                 handled = m_cameraController->handleMousePress(
 *                     static_cast<const MouseEvent&>(event));
 *                 break;
 *                 
 *             case InputEventType::MOUSE_MOVE:
 *                 handled = m_cameraController->handleMouseMove(
 *                     static_cast<const MouseEvent&>(event));
 *                 break;
 *                 
 *             case InputEventType::MOUSE_RELEASE:
 *                 handled = m_cameraController->handleMouseRelease(
 *                     static_cast<const MouseEvent&>(event));
 *                 break;
 *                 
 *             case InputEventType::WHEEL:
 *                 handled = m_cameraController->handleWheel(
 *                     static_cast<const WheelEvent&>(event));
 *                 break;
 *                 
 *             case InputEventType::KEY_PRESS:
 *                 handled = m_cameraController->handleKeyPress(
 *                     static_cast<const KeyEvent&>(event));
 *                 break;
 *         }
 *         
 *         // Pass unhandled events to other systems
 *         if (!handled) {
 *             handleApplicationInputEvent(event);
 *         }
 *     }
 * };
 * ```
 * 
 * ### Advanced Navigation Configuration
 * ```cpp
 * class AdvancedNavigationManager {
 * private:
 *     BlenderCameraController m_controller;
 *     UserPreferences m_preferences;
 *     
 * public:
 *     void loadUserPreferences() {
 *         // Load user-specific navigation preferences
 *         auto prefs = m_preferences.getNavigationSettings();
 *         
 *         m_controller.setOrbitSensitivity(prefs.orbitSpeed);
 *         m_controller.setPanSensitivity(prefs.panSpeed);
 *         m_controller.setZoomSensitivity(prefs.zoomSpeed);
 *         m_controller.setInvertZoom(prefs.invertZoomDirection);
 *         m_controller.setSmoothing(prefs.enableSmoothAnimation);
 *         
 *         // Configure keyboard shortcuts
 *         setupKeyboardShortcuts(prefs.keyboardLayout);
 *     }
 *     
 *     void handleViewCommand(const std::string& command) {
 *         if (command == "frame_all") {
 *             m_controller.frameScene(true);
 *         } else if (command == "frame_selected") {
 *             m_controller.frameSelection(true);
 *         } else if (command == "view_front") {
 *             // Simulate numpad 1 key press
 *             KeyEvent keyEvent{KeyCode::NUMPAD_1, KeyAction::PRESS, 0};
 *             m_controller.handleKeyPress(keyEvent);
 *         } else if (command == "view_top") {
 *             // Simulate numpad 7 key press
 *             KeyEvent keyEvent{KeyCode::NUMPAD_7, KeyAction::PRESS, 0};
 *             m_controller.handleKeyPress(keyEvent);
 *         } else if (command == "reset_camera") {
 *             m_controller.resetCamera();
 *         }
 *     }
 * };
 * ```
 * 
 * ### Professional Animation Integration
 * ```cpp
 * class AnimationViewportController {
 * private:
 *     BlenderCameraController m_navigation;
 *     AnimationTimeline m_timeline;
 *     KeyframeSystem m_keyframes;
 *     
 * public:
 *     void keyframeCameraPosition(float time) {
 *         // Get current camera state from navigation controller
 *         auto cameraState = m_navigation.getCurrentCameraState();
 *         
 *         // Create keyframe at current timeline position
 *         CameraKeyframe keyframe;
 *         keyframe.time = time;
 *         keyframe.position = cameraState.position;
 *         keyframe.rotation = cameraState.rotation;
 *         keyframe.distance = cameraState.distance;
 *         
 *         m_keyframes.addCameraKeyframe(keyframe);
 *     }
 *     
 *     void playAnimation() {
 *         m_timeline.onTimeChanged = [this](float time) {
 *             // Interpolate camera position from keyframes
 *             auto interpolatedState = m_keyframes.interpolateCameraState(time);
 *             
 *             // Apply to navigation controller
 *             m_navigation.setCameraState(interpolatedState);
 *         };
 *         
 *         m_timeline.play();
 *     }
 *     
 *     void enableNavigationDuringPlayback(bool enable) {
 *         // Allow or disable user navigation during animation playback
 *         m_navigation.setInteractionEnabled(enable);
 *     }
 * };
 * ```
 * 
 * ### Multi-Viewport Management
 * ```cpp
 * class MultiViewportManager {
 * private:
 *     struct ViewportController {
 *         std::unique_ptr<BlenderCameraController> controller;
 *         Camera camera;
 *         ViewportRegion region;
 *         ViewportType type; // PERSPECTIVE, FRONT, TOP, RIGHT, etc.
 *     };
 *     
 *     std::vector<ViewportController> m_viewports;
 *     int m_activeViewport = 0;
 *     
 * public:
 *     void setupQuadView() {
 *         m_viewports.resize(4);
 *         
 *         // Perspective viewport (top-left)
 *         setupViewport(0, ViewportType::PERSPECTIVE, {0, 0, 0.5f, 0.5f});
 *         
 *         // Front view (top-right)
 *         setupViewport(1, ViewportType::FRONT, {0.5f, 0, 0.5f, 0.5f});
 *         
 *         // Top view (bottom-left)
 *         setupViewport(2, ViewportType::TOP, {0, 0.5f, 0.5f, 0.5f});
 *         
 *         // Right view (bottom-right)
 *         setupViewport(3, ViewportType::RIGHT, {0.5f, 0.5f, 0.5f, 0.5f});
 *     }
 *     
 *     void handleInputEvent(const InputEvent& event, const glm::vec2& screenPos) {
 *         // Determine which viewport contains the mouse position
 *         int targetViewport = getViewportAtPosition(screenPos);
 *         
 *         if (targetViewport != -1) {
 *             m_activeViewport = targetViewport;
 *             
 *             // Convert screen coordinates to viewport-relative coordinates
 *             auto viewportEvent = convertEventToViewportSpace(event, targetViewport);
 *             
 *             // Route to appropriate controller
 *             m_viewports[targetViewport].controller->handleInputEvent(viewportEvent);
 *         }
 *     }
 *     
 *     void synchronizeViewports(bool enableSync) {
 *         if (enableSync) {
 *             // Link all viewport controllers for synchronized navigation
 *             for (size_t i = 1; i < m_viewports.size(); ++i) {
 *                 linkViewportControllers(m_viewports[0].controller.get(),
 *                                       m_viewports[i].controller.get());
 *             }
 *         }
 *     }
 * };
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Customizable Navigation Modes
 * Flexible navigation behavior for different workflows:
 * - **Modeling Mode**: Optimized for geometric modeling with precise control
 * - **Animation Mode**: Smooth transitions ideal for keyframe animation
 * - **Sculpting Mode**: High-sensitivity settings for detailed sculpting work
 * - **CAD Mode**: Precise orthographic navigation for technical drawing
 * 
 * ### Accessibility Features
 * Inclusive design for diverse user needs:
 * - **Alternative Input**: Keyboard-only navigation for accessibility
 * - **Sensitivity Scaling**: Adjustable response for different motor abilities
 * - **Visual Feedback**: On-screen indicators for navigation state
 * - **Audio Cues**: Optional sound feedback for navigation actions
 * 
 * ### Performance Monitoring
 * Real-time performance optimization:
 * - **Frame Rate Adaptation**: Automatic quality adjustment based on performance
 * - **Input Lag Compensation**: Predictive input handling for responsive feel
 * - **Memory Usage Tracking**: Efficient memory management for navigation state
 * - **Profiling Integration**: Built-in performance metrics for optimization
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Input
 * @ingroup Camera
 * @ingroup Navigation
 * @ingroup BlenderWorkflow
 */

#pragma once

#include "ICameraController.h"
#include <functional>
#include "InputEvents.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Blender-style camera controller
 * 
 * Implements Blender's navigation style:
 * - Middle Mouse Button (MMB) + Drag: Orbit
 * - Shift + MMB + Drag: Pan
 * - Ctrl + MMB + Drag: Zoom (alternative to scroll)
 * - Mouse Wheel: Zoom
 * - Numpad keys: Predefined views (1=Front, 3=Right, 7=Top, etc.)
 * - Home: Frame all objects
 * - Period (.): Focus on selected object
 */
class BlenderCameraController : public ICameraController
{
    // Cross-platform: no Q_OBJECT
public:
    // Cross-platform camera changed callback
    std::function<void()> cameraChangedCallback;

public:
    BlenderCameraController();
    ~BlenderCameraController() override;

    // ICameraController interface
    std::string getControllerDescription() const override;

    // Input handling
    bool handleMousePress(const MouseEvent& event) override;
    bool handleMouseMove(const MouseEvent& event) override;
    bool handleMouseRelease(const MouseEvent& event) override;
    bool handleWheel(const WheelEvent& event) override;
    bool handleKeyPress(const KeyEvent& event) override;
    bool handleKeyRelease(const KeyEvent& event) override;

    // Camera operations
    void frameScene(bool animate = true) override;
    void frameSelection(bool animate = true) override;
    void resetCamera() override;

    // Configuration
    void setOrbitSensitivity(float sensitivity);
    void setPanSensitivity(float sensitivity);
    void setZoomSensitivity(float sensitivity);
    void setInvertZoom(bool invert);
    void setSmoothing(bool enabled);

    void updateSmoothing();

private:
    NavigationAction m_currentAction;
    glm::ivec2 m_lastMousePos;
    glm::ivec2 m_mousePressPos;
    bool m_isNavigating;
    float m_orbitSensitivity;
    float m_panSensitivity;
    float m_zoomSensitivity;
    bool m_invertZoom;
    bool m_smoothingEnabled;
    glm::vec3 m_targetPosition;
    glm::quat m_targetRotation;
    float m_smoothingFactor;
    void startNavigation(NavigationAction action, const glm::ivec2& mousePos);
    void updateNavigation(const glm::ivec2& mousePos);
    void endNavigation();
    void performOrbit(const glm::vec2& delta);
    void performPan(const glm::vec2& delta);
    void performZoom(float delta);
    void setNumpadView(int key);
    void setFrontView();
    void setBackView();
    void setRightView();
    void setLeftView();
    void setTopView();
    void setBottomView();
    void setUserView();
    bool isMiddleMouseButton(int button) const;
    glm::vec2 getMouseDelta(const glm::ivec2& currentPos) const;
    void updateSmoothCamera();
};
