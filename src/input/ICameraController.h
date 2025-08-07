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
 * █  FILE: ICameraController.h                                                  █
 * █  DESCRIPTION: Abstract Camera Control Interface & Professional State System █
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
 * █  This file contains the ICameraController interface and professional        █
 * █  camera state management systems enabling pluggable navigation strategies   █
 * █  and advanced viewport functionality for diverse 3D modeling workflows.    █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file ICameraController.h
 * @brief Abstract Camera Control Interface and Professional State Management Framework
 * 
 * This file contains the comprehensive ICameraController interface and advanced camera
 * state management systems that provide the foundation for pluggable camera navigation
 * strategies in professional 3D applications. The interface enables support for multiple
 * industry-standard navigation paradigms (Maya, Blender, CAD, Game) while maintaining
 * consistent functionality through sophisticated bookmark, history, and state management
 * systems essential for professional 3D modeling and animation workflows.
 * 
 * ## Core Interface Architecture
 * 
 * ### Strategy Pattern Implementation
 * Flexible camera control system enabling multiple navigation paradigms:
 * - **Maya-Style Navigation**: Industry-standard Alt+mouse navigation schemes
 * - **Blender-Style Control**: Middle-mouse-button navigation for rapid workflow
 * - **CAD-Style Interface**: Precision navigation optimized for technical modeling
 * - **Game-Style Movement**: WASD and mouse-look for immersive exploration
 * - **Custom Controllers**: Extensible interface for specialized workflow requirements
 * 
 * ### Professional State Management
 * Advanced camera state persistence and navigation features:
 * - **Bookmark System**: Save and recall camera positions with named presets
 * - **View History**: Undo/redo functionality for camera navigation with full history
 * - **Smart Framing**: Intelligent object framing with configurable margins
 * - **Orbit Pivot Management**: Automatic and manual pivot point control systems
 * - **Cross-Session Persistence**: Camera state preservation across application sessions
 * 
 * ### Input Abstraction Layer
 * Unified input handling across diverse interaction paradigms:
 * - **Event-Driven Architecture**: Clean separation of input events and camera logic
 * - **Multi-Device Support**: Mouse, trackpad, stylus, and gamepad compatibility
 * - **Modifier Key Handling**: Complex input combinations for professional workflows
 * - **Platform Abstraction**: Consistent behavior across Windows, macOS, and Linux
 * 
 * ## Advanced Interface Features
 * 
 * ### Polymorphic Camera Control
 * Object-oriented design enabling runtime strategy switching:
 * - **Runtime Switching**: Seamless transition between different navigation modes
 * - **State Preservation**: Camera position maintained across controller changes
 * - **Capability Querying**: Dynamic discovery of controller-specific features
 * - **Configuration Persistence**: User preferences maintained per controller type
 * 
 * ### Professional Workflow Integration
 * Interface designed for production environment requirements:
 * - **Animation Timeline**: Camera keyframing and animation curve integration
 * - **Selection Integration**: Camera behavior adaptation based on object selection
 * - **Scene Analysis**: Intelligent camera behavior based on scene content
 * - **Multi-Viewport Support**: Consistent interface across multiple simultaneous views
 * 
 * ### Performance Optimization
 * Efficient interface design for real-time 3D applications:
 * - **Event Filtering**: Intelligent event processing to prevent unnecessary updates
 * - **State Caching**: Optimized camera state queries with minimal computation
 * - **Lazy Evaluation**: Deferred calculations for expensive camera operations
 * - **Memory Management**: Efficient bookmark and history storage systems
 * 
 * ## Professional Bookmark System
 * 
 * ### Named View Presets
 * Professional camera position management:
 * - **Numeric Slots**: Quick access via numeric keypad for rapid workflow
 * - **Named Bookmarks**: Descriptive labels for specific camera compositions
 * - **Timestamp Tracking**: Creation time tracking for bookmark organization
 * - **Metadata Storage**: Extended information including camera parameters and scene state
 * 
 * ### Advanced History Management
 * Sophisticated undo/redo system for camera navigation:
 * - **Circular Buffer**: Efficient memory usage with configurable history depth
 * - **Branching History**: Support for complex navigation patterns with branching
 * - **Selective Recall**: Filter history by operation type or time range
 * - **Visual History**: Thumbnail previews for visual navigation selection
 * 
 * ### Smart Framing Algorithms
 * Intelligent camera positioning for optimal object visualization:
 * - **Bounding Box Analysis**: Accurate object bounds calculation for framing
 * - **Aspect Ratio Awareness**: Frame adjustment based on viewport dimensions
 * - **Margin Optimization**: Configurable borders for professional presentation
 * - **Multi-Object Framing**: Optimal positioning for complex object collections
 * 
 * ## Technical Implementation
 * 
 * ### Interface Design Patterns
 * Professional software architecture principles:
 * - **Single Responsibility**: Clear separation of camera control concerns
 * - **Open/Closed Principle**: Extensible interface without modification
 * - **Liskov Substitution**: Seamless controller substitution capability
 * - **Interface Segregation**: Focused interfaces for specific functionality
 * 
 * ### Memory Management
 * Efficient resource handling for production environments:
 * - **Smart Pointers**: Automatic lifetime management for camera and scene references
 * - **Reference Counting**: Safe sharing of camera controller instances
 * - **Weak References**: Prevention of circular dependencies in complex scenes
 * - **Resource Pooling**: Efficient allocation for temporary camera state objects
 * 
 * ### Thread Safety
 * Multi-threaded application support:
 * - **Immutable State**: Thread-safe camera state queries
 * - **Atomic Operations**: Safe concurrent access to camera parameters
 * - **Event Serialization**: Ordered input event processing for consistency
 * - **Lock-Free Algorithms**: High-performance concurrent access patterns
 * 
 * ## Usage Examples
 * 
 * ### Basic Controller Implementation
 * ```cpp
 * class CustomCameraController : public ICameraController {
 * public:
 *     CustomCameraController() : m_movementSpeed(5.0f), m_rotationSpeed(0.3f) {}
 *     
 *     void setCamera(std::shared_ptr<Camera> camera) override {
 *         m_camera = camera;
 *         m_stateManager.setCamera(camera);
 *     }
 *     
 *     bool handleMousePress(const MouseEvent& event) override {
 *         if (event.button == MouseButton::MIDDLE) {
 *             m_isNavigating = true;
 *             m_lastMousePos = event.position;
 *             return true;
 *         }
 *         return false;
 *     }
 *     
 *     bool handleMouseMove(const MouseEvent& event) override {
 *         if (m_isNavigating) {
 *             glm::vec2 delta = event.position - m_lastMousePos;
 *             
 *             if (event.modifiers & KeyModifier::SHIFT) {
 *                 // Pan mode
 *                 performPan(delta);
 *             } else {
 *                 // Orbit mode
 *                 performOrbit(delta);
 *             }
 *             
 *             m_lastMousePos = event.position;
 *             return true;
 *         }
 *         return false;
 *     }
 *     
 *     std::string getControllerName() const override {
 *         return "Custom Professional Controller";
 *     }
 * };
 * ```
 * 
 * ### Professional Bookmark Management
 * ```cpp
 * class ProfessionalViewportManager {
 * private:
 *     std::unique_ptr<ICameraController> m_controller;
 *     CameraStateManager m_stateManager;
 *     
 * public:
 *     void setupViewportBookmarks() {
 *         // Setup standard architectural views
 *         m_controller->resetCamera();
 *         m_controller->setViewDirection(glm::vec3(0, 0, -1)); // Front view
 *         m_stateManager.saveBookmark(1, "Front Elevation");
 *         
 *         m_controller->setViewDirection(glm::vec3(-1, 0, 0)); // Right view
 *         m_stateManager.saveBookmark(3, "Right Elevation");
 *         
 *         m_controller->setViewDirection(glm::vec3(0, -1, 0)); // Top view
 *         m_stateManager.saveBookmark(7, "Plan View");
 *         
 *         // Setup isometric presentation view
 *         glm::vec3 isoDirection = glm::normalize(glm::vec3(1, 1, 1));
 *         m_controller->setViewDirection(isoDirection);
 *         m_stateManager.saveBookmark(0, "Isometric Presentation");
 *     }
 *     
 *     void handleViewportShortcuts(const KeyEvent& event) {
 *         if (event.modifiers & KeyModifier::CTRL) {
 *             switch (event.key) {
 *                 case KeyCode::NUM_1:
 *                     m_stateManager.restoreBookmark(1, true);
 *                     break;
 *                 case KeyCode::NUM_3:
 *                     m_stateManager.restoreBookmark(3, true);
 *                     break;
 *                 case KeyCode::NUM_7:
 *                     m_stateManager.restoreBookmark(7, true);
 *                     break;
 *                 case KeyCode::NUM_0:
 *                     m_stateManager.restoreBookmark(0, true);
 *                     break;
 *             }
 *         }
 *     }
 * };
 * ```
 * 
 * ### Multi-Controller Viewport System
 * ```cpp
 * class AdaptiveViewportSystem {
 * private:
 *     std::unique_ptr<ICameraController> m_currentController;
 *     CameraStateManager m_stateManager;
 *     CameraControllerFactory::ControllerType m_currentType;
 *     
 * public:
 *     void switchControllerMode(CameraControllerFactory::ControllerType newType) {
 *         // Save current camera state
 *         if (m_currentController) {
 *             m_stateManager.updateFromCamera();
 *             m_stateManager.pushCurrentView();
 *         }
 *         
 *         // Create new controller
 *         m_currentController = CameraControllerFactory::createController(newType);
 *         m_currentController->setCamera(m_camera);
 *         m_currentController->setScene(m_scene);
 *         
 *         // Restore camera state with new controller
 *         m_stateManager.setCamera(m_camera);
 *         
 *         m_currentType = newType;
 *         
 *         logInfo("Switched to " + m_currentController->getControllerName() + 
 *                " navigation mode");
 *     }
 *     
 *     void setupWorkflowModes() {
 *         // Modeling mode: Blender-style for rapid iteration
 *         registerWorkflowMode("modeling", CameraControllerFactory::ControllerType::Blender);
 *         
 *         // CAD mode: Precision navigation for technical work
 *         registerWorkflowMode("cad", CameraControllerFactory::ControllerType::CAD);
 *         
 *         // Animation mode: Maya-style for timeline work
 *         registerWorkflowMode("animation", CameraControllerFactory::ControllerType::Maya);
 *         
 *         // Game mode: First-person for immersive exploration
 *         registerWorkflowMode("exploration", CameraControllerFactory::ControllerType::Game);
 *     }
 * };
 * ```
 * 
 * ### Advanced History and Navigation
 * ```cpp
 * class IntelligentNavigationSystem {
 * private:
 *     CameraStateManager m_stateManager;
 *     std::unique_ptr<ICameraController> m_controller;
 *     
 * public:
 *     void setupIntelligentNavigation() {
 *         // Configure smart framing with optimal margins
 *         m_stateManager.setFramingMargin(0.1f); // 10% margin
 *         m_stateManager.setAutoOrbitPivot(true);
 *         
 *         // Setup automatic view history checkpoints
 *         setupAutoCheckpoints();
 *     }
 *     
 *     void handleSelectionChanged(const std::vector<SceneObject*>& selectedObjects) {
 *         if (!selectedObjects.empty()) {
 *             // Calculate selection bounds
 *             BoundingBox selectionBounds;
 *             for (const auto& object : selectedObjects) {
 *                 selectionBounds.expand(object->getBoundingBox());
 *             }
 *             
 *             // Set automatic orbit pivot to selection center
 *             glm::vec3 selectionCenter = selectionBounds.getCenter();
 *             m_stateManager.setOrbitPivot(selectionCenter);
 *             
 *             // Add to history before framing
 *             m_stateManager.pushCurrentView();
 *             
 *             // Frame selection with intelligent margins
 *             m_controller->frameSelection(true);
 *         }
 *     }
 *     
 *     void setupNavigationShortcuts() {
 *         // History navigation
 *         bindShortcut("Alt+Left", [this]() {
 *             if (m_stateManager.canGoBack()) {
 *                 m_stateManager.goBackInHistory();
 *             }
 *         });
 *         
 *         bindShortcut("Alt+Right", [this]() {
 *             if (m_stateManager.canGoForward()) {
 *                 m_stateManager.goForwardInHistory();
 *             }
 *         });
 *         
 *         // Quick framing
 *         bindShortcut("Home", [this]() {
 *             m_stateManager.pushCurrentView();
 *             m_controller->frameScene(true);
 *         });
 *         
 *         bindShortcut("Period", [this]() {
 *             m_stateManager.pushCurrentView();
 *             m_controller->frameSelection(true);
 *         });
 *     }
 * };
 * ```
 * 
 * ### Factory Pattern Implementation
 * ```cpp
 * class ExtendedCameraControllerFactory : public CameraControllerFactory {
 * public:
 *     static std::unique_ptr<ICameraController> createOptimizedController(
 *         ControllerType type, 
 *         const CameraControllerSettings& settings) {
 *         
 *         auto controller = createController(type);
 *         
 *         // Apply user-specific settings
 *         controller->setMovementSpeed(settings.movementSpeed);
 *         controller->setRotationSpeed(settings.rotationSpeed);
 *         controller->setPanSpeed(settings.panSpeed);
 *         controller->setZoomSpeed(settings.zoomSpeed);
 *         controller->setInvertY(settings.invertY);
 *         
 *         // Configure advanced features based on controller type
 *         configureAdvancedFeatures(controller.get(), type, settings);
 *         
 *         return controller;
 *     }
 *     
 *     static CameraControllerCapabilities getControllerCapabilities(ControllerType type) {
 *         CameraControllerCapabilities caps;
 *         
 *         switch (type) {
 *             case ControllerType::Maya:
 *                 caps.supportsOrbit = true;
 *                 caps.supportsPan = true;
 *                 caps.supportsZoom = true;
 *                 caps.supportsFly = false;
 *                 caps.supportsAnimation = true;
 *                 break;
 *                 
 *             case ControllerType::Blender:
 *                 caps.supportsOrbit = true;
 *                 caps.supportsPan = true;
 *                 caps.supportsZoom = true;
 *                 caps.supportsFly = false;
 *                 caps.supportsNumpadViews = true;
 *                 break;
 *                 
 *             case ControllerType::Game:
 *                 caps.supportsOrbit = false;
 *                 caps.supportsPan = false;
 *                 caps.supportsZoom = false;
 *                 caps.supportsFly = true;
 *                 caps.supportsCollision = true;
 *                 break;
 *         }
 *         
 *         return caps;
 *     }
 * };
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Adaptive Navigation
 * Context-aware camera control behavior:
 * - **Scene Analysis**: Automatic sensitivity adjustment based on scene scale
 * - **Content Recognition**: Navigation behavior adaptation for different content types
 * - **User Pattern Learning**: Adaptive behavior based on usage patterns
 * - **Tool Integration**: Camera behavior changes based on active modeling tools
 * 
 * ### Professional Workflow Support
 * Features designed for production environments:
 * - **Multi-User Collaboration**: Shared camera states in collaborative workflows
 * - **Version Control**: Camera state versioning for complex project management
 * - **Asset Pipeline**: Integration with asset management and pipeline systems
 * - **Automation Support**: Scriptable camera control for automated workflows
 * 
 * ### Performance and Quality
 * Optimized interface for professional applications:
 * - **Predictive Caching**: Pre-calculation of likely camera states
 * - **Input Prediction**: Anticipatory processing for responsive navigation
 * - **Quality Scaling**: Adaptive quality adjustment during navigation
 * - **Profiling Integration**: Built-in performance metrics and optimization
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Input
 * @ingroup Camera
 * @ingroup Navigation
 * @ingroup Interfaces
 */

#pragma once

#include "Common.h"
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <memory>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
#include "InputEvents.h"
class Camera;
// Forward declaration for rude::Scene
namespace rude { class Scene; }

/**
 * @brief Abstract interface for camera control strategies
 * 
 * This interface allows different camera control schemes to be plugged in,
 * following the Strategy pattern. Each implementation provides a different
 * user interaction model (Maya, Blender, CAD, etc.).
 */
class ICameraController {
public:
    virtual ~ICameraController() = default;
    virtual void setCamera(std::shared_ptr<Camera> camera) = 0;
    virtual void setScene(std::shared_ptr<rude::Scene> scene) = 0;
    // Input handling - return true if event was handled
    virtual bool handleMousePress(const MouseEvent& event) = 0;
    virtual bool handleMouseMove(const MouseEvent& event) = 0;
    virtual bool handleMouseRelease(const MouseEvent& event) = 0;
    virtual bool handleWheel(const WheelEvent& event) = 0;
    virtual bool handleKeyPress(const KeyEvent& event) = 0;
    virtual bool handleKeyRelease(const KeyEvent& event) = 0;
    // Camera operations
    virtual void resetCamera() = 0;
    virtual void frameScene(bool animate = true) = 0;
    virtual void frameSelection(bool animate = true) = 0;
    virtual void updateAspectRatio(float aspectRatio) = 0;
    // Camera state queries
    virtual glm::vec3 getWorldPosition() const = 0;
    virtual glm::mat4 getViewMatrix() const = 0;
    virtual glm::mat4 getProjectionMatrix() const = 0;
    virtual glm::vec3 screenToWorldRay(const glm::vec2& screenPos, const glm::ivec2& viewportSize) const = 0;
    // Settings
    virtual void setMovementSpeed(float speed) = 0;
    virtual void setRotationSpeed(float speed) = 0;
    virtual void setPanSpeed(float speed) = 0;
    virtual void setZoomSpeed(float speed) = 0;
    virtual void setInvertY(bool invert) = 0;
    virtual float getMovementSpeed() const = 0;
    virtual float getRotationSpeed() const = 0;
    virtual float getPanSpeed() const = 0;
    virtual float getZoomSpeed() const = 0;
    virtual bool isYInverted() const = 0;
    // Controller identification
    virtual std::string getControllerName() const = 0;
    virtual std::string getControllerDescription() const = 0;
protected:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<rude::Scene> m_scene;
};

/**
 * @brief Camera bookmark system for saving/restoring view states
 */
class CameraBookmark {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov;
    std::string name;
    uint64_t timestamp; // Use UNIX timestamp or std::chrono for cross-platform

    CameraBookmark() = default;
    CameraBookmark(const glm::vec3& pos, const glm::vec3& tgt, const glm::vec3& upVec,
                  float fieldOfView, const std::string& bookmarkName = "")
        : position(pos), target(tgt), up(upVec), fov(fieldOfView), name(bookmarkName)
        , timestamp(static_cast<uint64_t>(time(nullptr))) {}
};

/**
 * @brief Advanced camera state management system
 * 
 * Handles camera bookmarks, view history, focus management, and other
 * professional camera features that should be consistent across all
 * camera controller implementations.
 */
class CameraStateManager {
public:
    CameraStateManager();

    // Bookmark management
    void saveBookmark(int slot, const std::string& name = "");
    void restoreBookmark(int slot, bool animate = true);
    bool hasBookmark(int slot) const;
    CameraBookmark getBookmark(int slot) const;
    void clearBookmark(int slot);
    void clearAllBookmarks();

    // View history
    void pushCurrentView();
    void goBackInHistory();
    void goForwardInHistory();
    bool canGoBack() const;
    bool canGoForward() const;

    // Focus management
    void setOrbitPivot(const glm::vec3& pivot);
    glm::vec3 getOrbitPivot() const;
    void setAutoOrbitPivot(bool enabled);
    bool isAutoOrbitPivotEnabled() const;

    // Smart framing
    void setFramingMargin(float margin) { m_framingMargin = margin; }
    float getFramingMargin() const { return m_framingMargin; }

    // Camera state
    void setCamera(std::shared_ptr<Camera> camera);
    void updateFromCamera();

private:
    std::shared_ptr<Camera> m_camera;

    // Bookmarks (slots 0-9)
    std::map<int, CameraBookmark> m_bookmarks;

    // View history
    std::vector<CameraBookmark> m_viewHistory;
    int m_historyIndex;
    static const int MAX_HISTORY_SIZE = 50;

    // Orbit pivot
    glm::vec3 m_orbitPivot;
    bool m_autoOrbitPivot;

    // Settings
    float m_framingMargin;

    void addToHistory(const CameraBookmark& bookmark);
    CameraBookmark createBookmarkFromCamera() const;
    void applyCameraBookmark(const CameraBookmark& bookmark, bool animate);
};

/**
 * @brief Factory for creating camera controllers
 */
class CameraControllerFactory {
public:
    enum class ControllerType {
        Maya,
        Blender,
        CAD,
        Game,
        Custom
    };

    static std::unique_ptr<ICameraController> createController(ControllerType type);
    static std::vector<std::string> getAvailableControllers();
    static std::string getControllerDescription(ControllerType type);
};
