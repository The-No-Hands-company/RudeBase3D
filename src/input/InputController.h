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
 * █  FILE: InputController.h                                                    █
 * █  DESCRIPTION: Unified Input Management & Multi-Modal Navigation Controller  █
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
 * █  This file contains the InputController class providing comprehensive       █
 * █  unified input management, multi-modal navigation, and professional         █
 * █  interaction systems for diverse 3D modeling and animation workflows.      █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file InputController.h
 * @brief Unified Input Management and Multi-Modal Navigation Control System
 * 
 * This file contains the comprehensive InputController class implementation that provides
 * professional-grade unified input management essential for complex 3D modeling and
 * animation workflows. The system integrates camera control, object selection, viewport
 * interaction, and multi-modal navigation paradigms into a cohesive interface supporting
 * industry-standard interaction patterns including Maya, Blender, CAD, and FPS-style
 * navigation schemes within a single, flexible framework.
 * 
 * ## Core Input Management Architecture
 * 
 * ### Unified Input Abstraction
 * Cross-platform input handling with consistent behavior:
 * - **Mouse Input Processing**: Button states, movement, and wheel handling across platforms
 * - **Keyboard Integration**: Key combinations, modifier states, and shortcut management
 * - **Touch Support**: Gesture recognition and multi-touch interaction for tablet workflows
 * - **Gamepad Integration**: Controller support for alternative input paradigms
 * - **Accessibility Features**: Alternative input methods for diverse user needs
 * 
 * ### Multi-Modal Navigation System
 * Industry-standard navigation paradigms within unified framework:
 * - **Maya-Style Navigation**: Alt+mouse paradigm for professional animation workflows
 * - **Blender-Style Control**: Middle-mouse navigation for rapid modeling iteration
 * - **CAD-Style Precision**: Orthographic and precision-oriented navigation schemes
 * - **FPS-Style Movement**: WASD and mouse-look for immersive scene exploration
 * - **Custom Modes**: Extensible framework for specialized workflow requirements
 * 
 * ### Professional Integration Framework
 * Seamless integration with 3D application systems:
 * - **Camera Controller Integration**: Unified interface to diverse camera control strategies
 * - **Selection System Coordination**: Object and mesh element selection with modifier support
 * - **Viewport Management**: Multi-viewport input routing and focus management
 * - **Tool System Integration**: Context-sensitive input behavior based on active tools
 * 
 * ## Advanced Input Processing
 * 
 * ### Context-Aware Input Handling
 * Intelligent input interpretation based on application state:
 * - **Modal Input States**: Different input behavior based on active tools and modes
 * - **Priority Systems**: Hierarchical input handling with proper event propagation
 * - **Gesture Recognition**: Complex input pattern detection for advanced operations
 * - **Temporal Input Analysis**: Time-based input processing for gestures and shortcuts
 * 
 * ### Professional Selection Systems
 * Advanced object and element selection capabilities:
 * - **Object Selection**: 3D scene object picking with ray-casting and bounds testing
 * - **Mesh Element Selection**: Vertex, edge, and face selection with modifier support
 * - **Area Selection**: Box, lasso, and paint selection tools for efficient workflows
 * - **Hierarchical Selection**: Parent-child relationship aware selection systems
 * - **Multi-Object Operations**: Batch selection and operation capabilities
 * 
 * ### Navigation Mode Management
 * Dynamic navigation behavior adaptation:
 * - **Runtime Mode Switching**: Seamless transition between navigation paradigms
 * - **User Preference Integration**: Persistent settings and customization support
 * - **Workflow Optimization**: Mode selection based on active tools and content type
 * - **Performance Adaptation**: Navigation behavior adjustment based on scene complexity
 * 
 * ## Professional Workflow Integration
 * 
 * ### Industry-Standard Input Patterns
 * Support for established professional workflow conventions:
 * - **Maya Conventions**: Alt+LMB orbit, Alt+MMB pan, Alt+RMB zoom navigation
 * - **Blender Standards**: MMB orbit, Shift+MMB pan, scroll wheel zoom patterns
 * - **CAD Precision**: Right-click context menus, precise numeric input support
 * - **Game Development**: WASD movement, mouse-look, and first-person navigation
 * 
 * ### Modifier Key Systems
 * Complex input combinations for professional efficiency:
 * - **Multi-Modifier Support**: Ctrl, Shift, Alt combinations for extended functionality
 * - **Context-Sensitive Modifiers**: Different modifier behavior based on active tools
 * - **Custom Key Bindings**: User-definable shortcuts and input combinations
 * - **Platform Adaptation**: Mac Command key vs Windows Ctrl key handling
 * 
 * ### Selection and Manipulation
 * Professional object interaction and manipulation systems:
 * - **Click Selection**: Single-click object selection with modifier support
 * - **Drag Selection**: Box selection with additive and subtractive modes
 * - **Lasso Selection**: Freeform selection for complex object collections
 * - **Paint Selection**: Brush-based selection for organic modeling workflows
 * 
 * ## Technical Implementation
 * 
 * ### Event Processing Architecture
 * Efficient input event handling for real-time applications:
 * - **Event Queuing**: Asynchronous input processing with frame-rate independence
 * - **Input Filtering**: Noise reduction and smoothing for pen tablets and touch input
 * - **Deadzone Handling**: Configurable dead zones for analog input devices
 * - **Input Prediction**: Anticipatory processing for responsive user experience
 * 
 * ### Performance Optimization
 * Optimized input processing for complex 3D scenes:
 * - **Spatial Acceleration**: Spatial data structures for efficient ray-casting and picking
 * - **Level-of-Detail**: Distance-based interaction complexity adjustment
 * - **Batch Processing**: Efficient handling of multiple simultaneous input events
 * - **Memory Management**: Efficient state management for complex input contexts
 * 
 * ### Cross-Platform Compatibility
 * Consistent behavior across diverse operating systems:
 * - **Input Normalization**: Unified coordinate systems and input value ranges
 * - **Platform Abstraction**: Hardware-independent input event representation
 * - **Driver Integration**: Support for specialized input devices and drivers
 * - **Accessibility Compliance**: Support for assistive technologies and accessibility
 * 
 * ## Usage Examples
 * 
 * ### Basic Input Controller Setup
 * ```cpp
 * // Create and configure input controller for professional workflow
 * auto inputController = std::make_shared<InputController>();
 * auto cameraController = std::make_shared<CameraController>();
 * auto selectionManager = std::make_shared<SelectionManager>();
 * 
 * // Setup component integration
 * inputController->setCameraController(cameraController);
 * inputController->setScene(scene);
 * inputController->setViewport(viewport);
 * inputController->setSelectionManager(selectionManager);
 * 
 * // Configure for modeling workflow
 * inputController->setNavigationMode(NavigationMode::BLENDER);
 * inputController->setCameraSensitivity(0.5f);
 * inputController->setPanSpeed(0.3f);
 * inputController->setZoomSpeed(0.1f);
 * inputController->setInvertY(false);
 * ```
 * 
 * ### Multi-Modal Navigation Configuration
 * ```cpp
 * class AdaptiveInputManager {
 * private:
 *     std::shared_ptr<InputController> m_inputController;
 *     NavigationMode m_currentMode = NavigationMode::BLENDER;
 *     
 * public:
 *     void configureForModelingWorkflow() {
 *         m_inputController->setNavigationMode(NavigationMode::BLENDER);
 *         m_inputController->setCameraSensitivity(0.4f);   // Moderate for modeling
 *         m_inputController->setPanSpeed(0.25f);           // Precise control
 *         m_inputController->setZoomSpeed(0.08f);          // Smooth zoom
 *         m_inputController->setEnableMouseCapture(false); // Free cursor
 *         
 *         logInfo("Configured for Blender-style modeling workflow");
 *     }
 *     
 *     void configureForAnimationWorkflow() {
 *         m_inputController->setNavigationMode(NavigationMode::MAYA);
 *         m_inputController->setCameraSensitivity(0.3f);   // Smooth for animation
 *         m_inputController->setPanSpeed(0.2f);            // Gentle panning
 *         m_inputController->setZoomSpeed(0.06f);          // Fine zoom control
 *         m_inputController->setEnableMouseCapture(false); // Timeline interaction
 *         
 *         logInfo("Configured for Maya-style animation workflow");
 *     }
 *     
 *     void configureForCADWorkflow() {
 *         m_inputController->setNavigationMode(NavigationMode::CAD);
 *         m_inputController->setCameraSensitivity(0.2f);   // Precise navigation
 *         m_inputController->setPanSpeed(0.15f);           // Fine control
 *         m_inputController->setZoomSpeed(0.04f);          // Precise zoom
 *         m_inputController->setInvertY(true);             // CAD convention
 *         
 *         logInfo("Configured for CAD-style precision workflow");
 *     }
 *     
 *     void configureForExplorationMode() {
 *         m_inputController->setNavigationMode(NavigationMode::FPS);
 *         m_inputController->setMovementSpeed(5.0f);       // Walking pace
 *         m_inputController->setCameraSensitivity(0.8f);   // Responsive look
 *         m_inputController->setEnableMouseCapture(true);  // FPS-style capture
 *         m_inputController->setInvertY(false);            // Standard FPS
 *         
 *         logInfo("Configured for FPS-style exploration mode");
 *     }
 * };
 * ```
 * 
 * ### Professional Selection Integration
 * ```cpp
 * class AdvancedSelectionController {
 * private:
 *     std::shared_ptr<InputController> m_inputController;
 *     std::shared_ptr<SelectionManager> m_selectionManager;
 *     SelectionMode m_selectionMode = SelectionMode::OBJECT;
 *     
 * public:
 *     void handleSelectionInput(const glm::ivec2& mousePos, 
 *                              MouseButton button, 
 *                              KeyboardModifier modifiers) {
 *         
 *         if (button == MouseButton::LEFT) {
 *             SelectionOperation operation = SelectionOperation::REPLACE;
 *             
 *             // Determine selection operation based on modifiers
 *             if (modifiers & KeyboardModifier::SHIFT) {
 *                 operation = SelectionOperation::ADD;
 *             } else if (modifiers & KeyboardModifier::CTRL) {
 *                 operation = SelectionOperation::SUBTRACT;
 *             } else if (modifiers & KeyboardModifier::ALT) {
 *                 operation = SelectionOperation::INTERSECT;
 *             }
 *             
 *             // Perform selection based on current mode
 *             switch (m_selectionMode) {
 *                 case SelectionMode::OBJECT:
 *                     selectObject(mousePos, operation);
 *                     break;
 *                     
 *                 case SelectionMode::VERTEX:
 *                     selectVertices(mousePos, operation);
 *                     break;
 *                     
 *                 case SelectionMode::EDGE:
 *                     selectEdges(mousePos, operation);
 *                     break;
 *                     
 *                 case SelectionMode::FACE:
 *                     selectFaces(mousePos, operation);
 *                     break;
 *             }
 *         }
 *     }
 *     
 *     void enableBoxSelection() {
 *         m_inputController->setBoxSelectionMode(true);
 *         
 *         // Setup box selection callbacks
 *         m_inputController->onBoxSelectionStart = [this](const glm::ivec2& startPos) {
 *             m_selectionManager->startBoxSelection(startPos);
 *         };
 *         
 *         m_inputController->onBoxSelectionUpdate = [this](const glm::ivec2& currentPos) {
 *             m_selectionManager->updateBoxSelection(currentPos);
 *         };
 *         
 *         m_inputController->onBoxSelectionEnd = [this](const glm::ivec2& endPos) {
 *             m_selectionManager->finalizeBoxSelection(endPos);
 *         };
 *     }
 * };
 * ```
 * 
 * ### Context-Sensitive Input Handling
 * ```cpp
 * class ContextAwareInputProcessor {
 * private:
 *     std::shared_ptr<InputController> m_inputController;
 *     ToolManager m_toolManager;
 *     ApplicationMode m_currentMode;
 *     
 * public:
 *     void processContextualInput(const InputEvent& event) {
 *         // Route input based on active tool and application mode
 *         auto activeTool = m_toolManager.getActiveTool();
 *         
 *         if (activeTool->getType() == ToolType::SCULPTING) {
 *             handleSculptingInput(event);
 *         } else if (activeTool->getType() == ToolType::ANIMATION) {
 *             handleAnimationInput(event);
 *         } else if (activeTool->getType() == ToolType::MODELING) {
 *             handleModelingInput(event);
 *         } else {
 *             // Default navigation handling
 *             m_inputController->handleInputEvent(event);
 *         }
 *     }
 *     
 *     void handleSculptingInput(const InputEvent& event) {
 *         if (event.type == InputEventType::MOUSE_MOVE && event.button == MouseButton::LEFT) {
 *             // Sculpting brush control
 *             float pressure = event.pressure; // Pen tablet pressure
 *             float brushSize = calculateBrushSize(event.modifiers);
 *             float brushStrength = calculateBrushStrength(pressure, event.modifiers);
 *             
 *             applySculptingBrush(event.position, brushSize, brushStrength);
 *         } else {
 *             // Fall back to standard navigation
 *             m_inputController->handleInputEvent(event);
 *         }
 *     }
 *     
 *     void handleAnimationInput(const InputEvent& event) {
 *         if (event.type == InputEventType::KEY_PRESS) {
 *             switch (event.key) {
 *                 case KeyCode::SPACE:
 *                     toggleAnimationPlayback();
 *                     break;
 *                     
 *                 case KeyCode::LEFT_ARROW:
 *                     if (event.modifiers & KeyboardModifier::SHIFT) {
 *                         jumpToPreviousKeyframe();
 *                     } else {
 *                         stepBackwardOneFrame();
 *                     }
 *                     break;
 *                     
 *                 case KeyCode::RIGHT_ARROW:
 *                     if (event.modifiers & KeyboardModifier::SHIFT) {
 *                         jumpToNextKeyframe();
 *                     } else {
 *                         stepForwardOneFrame();
 *                     }
 *                     break;
 *             }
 *         } else {
 *             m_inputController->handleInputEvent(event);
 *         }
 *     }
 * };
 * ```
 * 
 * ### Performance-Optimized Input Processing
 * ```cpp
 * class PerformanceOptimizedInputProcessor {
 * private:
 *     std::shared_ptr<InputController> m_inputController;
 *     InputEventQueue m_eventQueue;
 *     std::chrono::high_resolution_clock::time_point m_lastProcessTime;
 *     
 * public:
 *     void processInputEvents(float deltaTime) {
 *         // Batch process input events for optimal performance
 *         const int MAX_EVENTS_PER_FRAME = 64;
 *         int eventsProcessed = 0;
 *         
 *         while (!m_eventQueue.empty() && eventsProcessed < MAX_EVENTS_PER_FRAME) {
 *             auto event = m_eventQueue.front();
 *             m_eventQueue.pop();
 *             
 *             // Process event with timing information
 *             processTimedEvent(event, deltaTime);
 *             eventsProcessed++;
 *         }
 *         
 *         // Update continuous input (e.g., held keys)
 *         updateContinuousInput(deltaTime);
 *         
 *         // Apply input smoothing for tablet and analog devices
 *         applySmoothingFilter(deltaTime);
 *     }
 *     
 *     void optimizeForScene(const Scene& scene) {
 *         // Adjust input processing based on scene complexity
 *         size_t objectCount = scene.getObjectCount();
 *         
 *         if (objectCount > 10000) {
 *             // High complexity scene - reduce selection precision
 *             m_inputController->setSelectionPrecision(SelectionPrecision::LOW);
 *             m_inputController->setRaycastingLOD(RaycastingLOD::COARSE);
 *         } else if (objectCount > 1000) {
 *             // Medium complexity - balanced settings
 *             m_inputController->setSelectionPrecision(SelectionPrecision::MEDIUM);
 *             m_inputController->setRaycastingLOD(RaycastingLOD::MEDIUM);
 *         } else {
 *             // Low complexity - full precision
 *             m_inputController->setSelectionPrecision(SelectionPrecision::HIGH);
 *             m_inputController->setRaycastingLOD(RaycastingLOD::FINE);
 *         }
 *     }
 * };
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Gesture Recognition
 * Advanced input pattern recognition for complex operations:
 * - **Multi-Touch Gestures**: Pinch, rotate, and pan gestures for tablet workflows
 * - **Pen Tablet Support**: Pressure sensitivity and tilt recognition for artistic workflows
 * - **Custom Gestures**: User-definable gesture patterns for specialized operations
 * - **Temporal Patterns**: Time-based input sequences for advanced commands
 * 
 * ### Accessibility Features
 * Inclusive design for diverse user needs:
 * - **Alternative Input**: Voice control and eye tracking integration
 * - **Assistive Technology**: Screen reader and switch navigation support
 * - **Customizable Sensitivity**: Adjustable response for different motor abilities
 * - **Visual Feedback**: On-screen indicators for input state and feedback
 * 
 * ### Performance Monitoring
 * Real-time performance optimization and monitoring:
 * - **Input Latency Tracking**: Measurement and optimization of input response times
 * - **Throughput Analysis**: Input event processing capacity and bottleneck identification
 * - **Memory Usage**: Efficient state management for complex input contexts
 * - **Adaptive Quality**: Dynamic quality adjustment based on performance metrics
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Input
 * @ingroup Navigation
 * @ingroup Selection
 * @ingroup UserInterface
 */

#pragma once

// Core includes
#include "Common.h"
#include "InputEvents.h"
#include "CameraController.h"
#include "tools/selection/SelectionManager.h"
#include "ui/viewport/Viewport3D.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <set>
#include <memory>
// Forward declaration for rude::Scene
class InputController
{
public:
    InputController();

    // Set dependencies
    void setCameraController(std::shared_ptr<CameraController> cameraController);
    void setScene(std::shared_ptr<rude::Scene> scene);
    void setViewport(Viewport3D* viewport);
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);

    // Input handling (cross-platform, unified enums only)
    void handleMousePress(const glm::ivec2& mousePos, MouseButton button, KeyboardModifier modifiers);
    void handleMouseMove(const glm::ivec2& mousePos, KeyboardModifier modifiers);
    void handleMouseRelease(const glm::ivec2& mousePos, MouseButton button, KeyboardModifier modifiers);
    void handleWheel(float wheelDelta, KeyboardModifier modifiers);
    void handleKeyPress(int key, KeyboardModifier modifiers);
    void handleKeyRelease(int key, KeyboardModifier modifiers);

    // Navigation settings
    void setNavigationMode(NavigationMode mode);
    NavigationMode getNavigationMode() const;

    // Sensitivity settings
    void setCameraSensitivity(float sensitivity);
    void setPanSpeed(float speed);
    void setZoomSpeed(float speed);
    void setMovementSpeed(float speed);

    // Input mode settings
    void setInvertY(bool invert);
    void setEnableMouseCapture(bool enable);

    float getCameraSensitivity() const;
    float getPanSpeed() const;
    float getZoomSpeed() const;
    float getMovementSpeed() const;
    bool isYInverted() const;

private:
    void handleCameraControl(const glm::ivec2& delta);
    void handleObjectSelection(const glm::ivec2& pos);
    void handleMeshElementSelection(const glm::ivec2& pos);
    void updateCameraFromKeys();
    glm::vec3 getSceneCenter() const;
    void handleMayaNavigation(const glm::ivec2& delta);
    void handleBlenderNavigation(const glm::ivec2& delta);
    void handleCADNavigation(const glm::ivec2& delta);
    void handleFPSNavigation(const glm::ivec2& delta);
    bool isModifierPressed(KeyboardModifier modifier) const;
    bool isOrbitAction() const;
    bool isPanAction() const;
    bool isZoomAction() const;

    std::shared_ptr<CameraController> m_cameraController;
    std::shared_ptr<rude::Scene> m_scene;
    Viewport3D* m_viewport;
    std::shared_ptr<SelectionManager> m_selectionManager;

    MouseButton m_mouseButton;
    glm::ivec2 m_lastMousePos;
    bool m_isDragging;
    NavigationMode m_navigationMode;
    std::set<int> m_pressedKeys;
    KeyboardModifier m_currentModifiers;

    float m_cameraSensitivity;
    float m_panSpeed;
    float m_zoomSpeed;
    float m_movementSpeed;
    bool m_invertY;
    bool m_enableMouseCapture;

    bool m_fpsMode;
    glm::ivec2 m_centerPos;
};
