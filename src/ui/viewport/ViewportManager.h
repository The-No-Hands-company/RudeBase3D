//==============================================================================
// RudeBase3D Engine - ViewportManager.h
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

/**
 * @file ViewportManager.h
 * @brief Multi-viewport layout management for professional 3D modeling workflows
 * 
 * This file contains the ViewportManager system which provides comprehensive
 * multi-viewport functionality for professional 3D modeling applications.
 * It manages flexible viewport layouts, synchronization, individual viewport
 * configuration, and the ViewCube navigation widget found in industry-standard
 * CAD and 3D modeling software.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup UI
 * @ingroup Viewport
 */

#pragma once

#include "Common.h"
#include "core/selection_manager.hpp"
#include <QWidget>
#include <QGridLayout>
#include <QSplitter>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QEnterEvent>
#include <memory>

class ICameraController;
class CameraStateManager;
class Scene;
class Camera;
class LightingSystem;
class GridSystem;
class RenderSystem;
class Renderer;
class GizmoManager;

// Forward declarations for event system
namespace event {
    class EventDispatcher;
    class MouseHandler;
}

namespace input {
    class EventDrivenCameraController;
}

/**
 * @addtogroup UI
 * @{
 */

/**
 * @brief Individual viewport widget providing independent 3D visualization
 * 
 * ViewportWidget represents a single 3D viewport within a multi-viewport layout,
 * providing independent camera control, rendering settings, and user interaction.
 * Each viewport can display different views (perspective, orthographic) and
 * maintain separate camera states while sharing the same scene content.
 * 
 * ## Core Features
 * - **Independent Camera**: Each viewport maintains its own camera and view state
 * - **Predefined Views**: Standard orthographic and perspective view types
 * - **Flexible Rendering**: Individual render modes and visualization settings
 * - **User Interaction**: Independent mouse and keyboard interaction handling
 * - **Professional Overlays**: View labels, statistics, and navigation aids
 * 
 * ## View Types
 * The viewport supports industry-standard view configurations:
 * - **Perspective**: Realistic 3D view with depth perception
 * - **Orthographic Views**: Top, Front, Right technical drawing views
 * - **User Views**: Custom camera positions saved by the user
 * - **Camera Views**: Views matching scene cameras for cinematography
 * 
 * ## Professional Integration
 * ViewportWidget integrates seamlessly with professional modeling workflows:
 * - **Maya/Blender Compatibility**: Familiar view layouts and interaction patterns
 * - **Technical Drawing**: Orthographic views for precise technical modeling
 * - **Animation Preview**: Camera viewport for shot composition and timing
 * - **Multi-User Workflows**: Independent viewports for collaborative modeling
 * 
 * @see ViewportManager for multi-viewport layout management
 * @see Viewport3D for single-viewport 3D rendering
 * @see Camera for view and projection management
 * 
 * @ingroup UI
 * @ingroup Viewport
 */
class ViewportWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
    
public:
    // ========================================================================
    // View Type Configuration
    // ========================================================================
    
    /**
     * @enum ViewType
     * @brief Standard viewport view types for professional modeling
     * 
     * Defines the predefined view types commonly used in professional
     * 3D modeling and CAD applications for different modeling workflows.
     */
    enum class ViewType {
        /**
         * @brief Perspective view with depth perception
         * 
         * Provides realistic 3D visualization with perspective projection,
         * ideal for general modeling, sculpting, and artistic work.
         */
        Perspective,
        
        /**
         * @brief Top orthographic view (XZ plane)
         * 
         * Top-down orthographic view looking along negative Y axis,
         * essential for floor plan layouts and top-view modeling.
         */
        Top,
        
        /**
         * @brief Front orthographic view (XY plane)
         * 
         * Front elevation orthographic view looking along negative Z axis,
         * crucial for front elevation and profile modeling work.
         */
        Front,
        
        /**
         * @brief Right orthographic view (YZ plane)
         * 
         * Right side orthographic view looking along positive X axis,
         * important for side elevation and cross-section modeling.
         */
        Right,
        
        /**
         * @brief User-defined custom view
         * 
         * Custom camera position and orientation saved by the user,
         * allowing quick access to frequently used viewing angles.
         */
        User,
        
        /**
         * @brief Scene camera view for cinematography
         * 
         * View matching a specific scene camera object, essential for
         * shot composition, lighting setup, and animation preview.
         */
        Camera
    };

    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct viewport widget with Qt parent
     * 
     * Creates a new ViewportWidget with independent 3D rendering capabilities,
     * default perspective view, and professional interaction handling.
     * 
     * @param parent Qt parent widget for memory management
     * 
     * @post Viewport is created with perspective view type
     * @post OpenGL context is configured for 3D rendering
     * @post Default camera controller and rendering systems are initialized
     * @post Professional overlay system is ready for view information display
     */
    explicit ViewportWidget(QWidget* parent = nullptr);
    
    /**
     * @brief Destructor with automatic resource cleanup
     * 
     * Properly cleans up OpenGL resources, camera controllers, and
     * disconnects from all managed systems for clean shutdown.
     */
    ~ViewportWidget() override;

    // ========================================================================
    // System Integration and Setup
    // ========================================================================
    
    /**
     * @brief Assign scene for 3D visualization
     * 
     * Sets the 3D scene to be rendered in this viewport. Multiple viewports
     * can share the same scene while maintaining independent camera views.
     * 
     * @param scene Shared pointer to the scene to visualize
     * 
     * @post Scene is assigned and becomes visible in viewport
     * @post Viewport rendering is updated to display scene content
     * @post Camera framing may be adjusted for optimal scene viewing
     * 
     * @note Multiple viewports can safely share the same scene
     * @note Scene changes are immediately reflected in the viewport
     */
    void setScene(std::shared_ptr<rude::Scene> scene);
    
    /**
     * @brief Assign camera controller for navigation
     * 
     * Sets the camera controller that manages user input for this viewport's
     * camera navigation, providing independent control for each viewport.
     * 
     * @param controller Unique pointer to camera controller (ownership transferred)
     * 
     * @post Camera controller manages navigation input for this viewport
     * @post Previous controller is released and cleaned up
     * @post Navigation behavior adapts to controller type and settings
     * 
     * @note Each viewport should have its own controller instance
     * @note Controller type can vary per viewport (orbit, fly, etc.)
     */
    void setCameraController(std::unique_ptr<ICameraController> controller);
    
    /**
     * @brief Assign lighting system for illumination
     * 
     * Sets the lighting system that provides scene illumination for this
     * viewport. Lighting systems can be shared across viewports.
     * 
     * @param lightingSystem Shared pointer to lighting system
     * 
     * @post Lighting system provides illumination for viewport rendering
     * @post Professional lighting presets become available
     * @post Lighting changes are immediately visible in viewport
     * 
     * @note Lighting systems are typically shared across all viewports
     * @note Lighting changes affect all viewports using the same system
     */
    void setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem);
    
    /**
     * @brief Assign grid system for reference overlays
     * 
     * Sets the grid system that manages reference grids and construction
     * aids for this viewport. Grid visibility can be controlled independently.
     * 
     * @param gridSystem Shared pointer to grid system
     * 
     * @post Grid system provides reference overlays for viewport
     * @post Grid visibility can be toggled independently per viewport
     * @post Professional grid presets become available
     * 
     * @note Grid systems are typically shared across viewports
     * @note Each viewport can independently control grid visibility
     */
    void setGridSystem(std::shared_ptr<GridSystem> gridSystem);
    
    /**
     * @brief Assign render system for graphics pipeline
     * 
     * Sets the render system that manages the graphics rendering pipeline
     * for this viewport, coordinating all rendering operations.
     * 
     * @param renderSystem Shared pointer to render system
     * 
     * @post Render system manages graphics pipeline for viewport
     * @post Advanced rendering features become available
     * @post Rendering performance is optimized through system coordination
     * 
     * @note Render systems are typically shared across viewports
     * @note System provides unified rendering pipeline management
     */
    void setRenderSystem(std::shared_ptr<RenderSystem> renderSystem);

    // ========================================================================
    // Viewport Configuration and Identity
    // ========================================================================
    
    /**
     * @brief Set predefined view type for viewport
     * 
     * Configures the viewport to use a standard predefined view type,
     * automatically adjusting camera position and projection settings.
     * 
     * @param type View type to apply to this viewport
     * 
     * @post Viewport adopts specified view type and camera configuration
     * @post Camera position and orientation are set to match view type
     * @post Projection type (perspective/orthographic) is adjusted as needed
     * 
     * @note Orthographic views use parallel projection for technical accuracy
     * @note Perspective view provides realistic depth perception
     * @note View type affects user interaction and navigation behavior
     */
    void setViewType(ViewType type);
    
    /**
     * @brief Get current view type
     * 
     * @return Currently active view type for this viewport
     */
    ViewType getViewType() const { return m_viewType; }
    
    /**
     * @brief Set display name for viewport
     * 
     * Sets the human-readable name displayed in the viewport overlay
     * for user identification and workflow organization.
     * 
     * @param name Display name for the viewport (e.g., "Front", "Perspective")
     * 
     * @post Viewport overlay displays the specified name
     * @post Viewport can be identified by name in multi-viewport layouts
     * 
     * @note Name is displayed in viewport corner for user reference
     * @note Custom names help organize complex multi-viewport workflows
     */
    void setViewName(const QString& name);
    
    /**
     * @brief Get current viewport display name
     * 
     * @return Current display name for this viewport
     */
    QString getViewName() const { return m_viewName; }
    
    /**
     * @brief Set viewport active state
     * 
     * Controls whether this viewport is the currently active viewport
     * receiving keyboard input and tool commands.
     * 
     * @param active true to make viewport active, false to deactivate
     * 
     * @post Viewport active state is updated
     * @post Visual indicators show active/inactive state
     * @post Keyboard input routing is updated based on active state
     * 
     * @note Only one viewport should be active at a time
     * @note Active viewport receives keyboard shortcuts and tool commands
     * @note Visual border or highlighting indicates active viewport
     */
    void setActive(bool active);
    
    /**
     * @brief Check if viewport is currently active
     * 
     * @return true if viewport is active, false if inactive
     */
    bool isActive() const { return m_isActive; }

    // ========================================================================
    // Component Access and Management
    // ========================================================================
    
    /**
     * @brief Get gizmo manager for transform tools
     * 
     * @return Pointer to gizmo manager, or nullptr if none assigned
     * 
     * @note Gizmo manager handles transform tool visualization and interaction
     * @note Use for external gizmo configuration and tool integration
     */
    GizmoManager* getGizmoManager() const { return m_gizmoManager.get(); }
    
    /**
     * @brief Get viewport camera for external access
     * 
     * @return Shared pointer to viewport camera
     * 
     * @note Each viewport maintains its own independent camera
     * @note Camera provides view and projection matrices for rendering
     * @note Use for external camera synchronization or state queries
     */
    std::shared_ptr<Camera> getCamera() const { return m_camera; }
    
    /**
     * @brief Get camera controller for configuration
     * 
     * @return Pointer to camera controller, or nullptr if none assigned
     * 
     * @note Controller manages navigation behavior and input handling
     * @note Use for external navigation configuration and customization
     */
    ICameraController* getCameraController() const { return m_cameraController.get(); }

    // ========================================================================
    // Rendering and Visualization Control
    // ========================================================================
    
    /**
     * @brief Set viewport rendering mode
     * 
     * Controls how 3D geometry is visualized in this specific viewport,
     * allowing different viewports to show different render modes simultaneously.
     * 
     * @param mode Render mode to apply to this viewport
     * 
     * @post Viewport rendering mode is updated
     * @post Viewport re-renders with new visualization style
     * @post renderModeChanged() signal is emitted
     * 
     * @note Different viewports can use different render modes simultaneously
     * @note Mode changes affect only this viewport, not others in the layout
     */
    void setRenderMode(RenderMode mode);
    
    /**
     * @brief Get current render mode
     * 
     * @return Currently active render mode for this viewport
     */
    RenderMode getRenderMode() const { return m_renderMode; }
    
    /**
     * @brief Toggle reference grid visibility
     * 
     * Controls whether the construction grid is visible in this specific
     * viewport, allowing independent grid control per viewport.
     * 
     * @param show true to show grid, false to hide
     * 
     * @post Grid visibility is updated for this viewport only
     * @post Grid rendering is enabled/disabled for performance
     * 
     * @note Grid visibility is independent per viewport
     * @note Hidden grid still provides snapping if enabled
     */
    void setShowGrid(bool show);
    
    /**
     * @brief Check if reference grid is visible
     * 
     * @return true if grid is visible in this viewport, false if hidden
     */
    bool isGridVisible() const;
    
    /**
     * @brief Toggle transform gizmo visibility
     * 
     * Controls whether transform gizmos (translate, rotate, scale) are
     * visible and interactive in this viewport for selected objects.
     * 
     * @param show true to show gizmos, false to hide
     * 
     * @post Transform gizmo visibility is updated for this viewport
     * @post Gizmo interaction is enabled/disabled accordingly
     * 
     * @note Gizmo visibility depends on object selection state
     * @note Hidden gizmos disable transform tool interaction
     */
    void setShowGizmos(bool show);
    
    /**
     * @brief Check if transform gizmos are visible
     * 
     * @return true if gizmos are visible in this viewport, false if hidden
     */
    bool areGizmosVisible() const { return m_showGizmos; }

    // ========================================================================
    // Camera Navigation and View Operations
    // ========================================================================
    
    /**
     * @brief Frame entire scene in viewport with optional animation
     * 
     * Automatically positions and zooms camera to show the entire scene
     * content with appropriate padding for optimal visualization.
     * 
     * @param animate true to animate camera transition, false for instant
     * 
     * @post Camera position frames all scene objects optimally
     * @post Zoom level shows entire scene with comfortable padding
     * @post Animation provides smooth transition if enabled
     * 
     * @note Framing calculation considers all visible scene objects
     * @note Animation improves user experience and spatial orientation
     */
    void frameScene(bool animate = true);
    
    /**
     * @brief Frame currently selected objects with optional animation
     * 
     * Automatically positions and zooms camera to optimally frame
     * the currently selected objects for detailed inspection or editing.
     * 
     * @param animate true to animate camera transition, false for instant
     * 
     * @post Camera frames selected objects with appropriate zoom level
     * @post Camera position provides optimal viewing angle for selection
     * @post Falls back to scene framing if nothing is selected
     * 
     * @note Requires active selection for optimal operation
     * @note Multiple selected objects are framed as a group
     * @note Animation helps maintain spatial orientation
     */
    void frameSelection(bool animate = true);
    
    /**
     * @brief Reset camera to default position for current view type
     * 
     * Moves camera to the standard default position appropriate for
     * the current view type (perspective, top, front, right, etc.).
     * 
     * @post Camera position is reset to view type default
     * @post Camera orientation matches standard view direction
     * @post Zoom level is set to default modeling distance
     * 
     * @note Default positions vary by view type (perspective vs orthographic)
     * @note Provides consistent starting point for modeling workflows
     */
    void resetCamera();

signals:
    // ========================================================================
    // Viewport Communication Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when viewport becomes active
     * 
     * Emitted when this viewport is activated by user interaction,
     * allowing the ViewportManager to coordinate active viewport state.
     * 
     * @param viewport Pointer to the viewport that became active (this viewport)
     * 
     * @note Only one viewport should be active at a time
     * @note Connect to ViewportManager for proper active state coordination
     */
    void viewportActivated(ViewportWidget* viewport);
    
    /**
     * @brief Signal emitted when camera parameters change
     * 
     * Emitted when camera position, orientation, or projection changes,
     * allowing synchronized viewports to update accordingly.
     * 
     * @note Useful for viewport synchronization features
     * @note Connect to ViewportManager for multi-viewport coordination
     */
    void cameraChanged();
    
    /**
     * @brief Signal emitted when render mode changes
     * 
     * Emitted when the viewport's rendering mode is changed,
     * allowing UI elements to update accordingly.
     * 
     * @param mode New render mode that has been applied
     * 
     * @note Connect to toolbar buttons for visual state synchronization
     */
    void renderModeChanged(RenderMode mode);
    
    /**
     * @brief Signal emitted when selection state changes
     * 
     * Emitted when object selection changes in this viewport,
     * allowing external systems to respond to selection updates.
     * 
     * @note Connect to property panels and outliner for selection sync
     * @note Selection changes may affect gizmo visibility and tools
     */
    void selectionChanged();

protected:
    // ========================================================================
    // OpenGL Lifecycle and Rendering
    // ========================================================================
    
    /**
     * @brief Initialize OpenGL resources and rendering state
     * 
     * Called automatically by Qt when the OpenGL context is ready.
     * Sets up OpenGL state, loads shaders, and prepares viewport rendering.
     * 
     * @post OpenGL context is configured for 3D rendering
     * @post Viewport is ready for scene rendering
     * 
     * @note Called automatically by Qt framework
     */
    void initializeGL() override;
    
    /**
     * @brief Handle viewport size changes
     * 
     * Called automatically when the viewport widget is resized.
     * Updates viewport dimensions and camera aspect ratio.
     * 
     * @param w New width of the viewport in pixels
     * @param h New height of the viewport in pixels
     * 
     * @post Viewport dimensions are updated
     * @post Camera aspect ratio is recalculated for proper proportions
     * 
     * @note Called automatically by Qt framework
     */
    void resizeGL(int w, int h) override;
    
    /**
     * @brief Render complete viewport frame
     * 
     * Called automatically by Qt to render the viewport content.
     * Performs complete frame rendering with scene, overlays, and gizmos.
     * 
     * @post Complete viewport frame is rendered
     * @post All visual elements are updated and displayed
     * 
     * @note Called automatically by Qt framework during repaints
     * @note Performance-critical method optimized for real-time rendering
     */
    void paintGL() override;

    // ========================================================================
    // User Input Event Handling
    // ========================================================================
    
    /**
     * @brief Handle mouse button press events
     * 
     * Processes mouse button press for selection, navigation, and tool activation.
     * Routes events to appropriate controllers based on viewport state.
     * 
     * @param event Qt mouse event containing button, position, and modifiers
     * 
     * @post Mouse interaction is initiated based on button and context
     * @post Viewport may become active if not already active
     * 
     * @note Left click handles selection and tool activation
     * @note Middle click initiates camera navigation
     * @note Right click shows context menus
     */
    void mousePressEvent(QMouseEvent* event) override;
    
    /**
     * @brief Handle mouse movement events
     * 
     * Processes mouse movement for camera navigation and object manipulation.
     * Provides smooth, responsive interaction for professional workflows.
     * 
     * @param event Qt mouse event containing position and movement delta
     * 
     * @post Mouse movement is processed by active controller
     * @post Visual feedback is updated for ongoing operations
     * 
     * @note Camera navigation requires active mouse button press
     * @note Transform gizmo interaction provides real-time feedback
     */
    void mouseMoveEvent(QMouseEvent* event) override;
    
    /**
     * @brief Handle mouse button release events
     * 
     * Processes mouse button release to complete interactions and finalize
     * operations. Ensures proper completion of navigation and manipulation.
     * 
     * @param event Qt mouse event containing button and final position
     * 
     * @post Active mouse interaction is completed
     * @post Final state is applied to camera or selected objects
     * 
     * @note Completes camera navigation movements
     * @note Finalizes object transformation operations
     */
    void mouseReleaseEvent(QMouseEvent* event) override;
    
    /**
     * @brief Handle mouse wheel scroll events
     * 
     * Processes mouse wheel input for camera zoom control with configurable
     * sensitivity and smooth scaling behavior.
     * 
     * @param event Qt wheel event containing scroll delta and position
     * 
     * @post Camera zoom level is adjusted based on wheel input
     * @post Zoom maintains focus point for intuitive navigation
     * 
     * @note Zoom speed is configurable through camera controller
     * @note Focus point typically follows mouse cursor position
     */
    void wheelEvent(QWheelEvent* event) override;
    
    /**
     * @brief Handle keyboard input events
     * 
     * Processes keyboard shortcuts for camera control, tool activation,
     * and viewport-specific operations.
     * 
     * @param event Qt keyboard event containing key code and modifiers
     * 
     * @post Keyboard shortcut is processed and appropriate action taken
     * @post Camera, selection, or tool state may be updated
     * 
     * @note Active viewport receives keyboard input
     * @note Shortcuts provide rapid access to common operations
     */
    void keyPressEvent(QKeyEvent* event) override;
    
    /**
     * @brief Handle keyboard key release events
     * 
     * Processes keyboard key release to complete key-based operations
     * and update modifier key states.
     * 
     * @param event Qt keyboard event containing released key and modifiers
     * 
     * @post Key release is processed by active systems
     * @post Modifier key states are updated accordingly
     * 
     * @note Important for modifier-based interaction modes
     * @note Completes key-based tool operations
     */
    void keyReleaseEvent(QKeyEvent* event) override;
    
    /**
     * @brief Handle viewport focus gained events
     * 
     * Called when the viewport gains keyboard focus, typically making
     * it the active viewport for input and tool operations.
     * 
     * @param event Qt focus event containing focus change information
     * 
     * @post Viewport becomes active and ready for keyboard input
     * @post Visual indicators show active state
     * @post viewportActivated() signal is emitted
     * 
     * @note Focus change affects keyboard input routing
     * @note Only one viewport should have focus at a time
     */
    void focusInEvent(QFocusEvent* event) override;

protected:
    /**
     * @brief Handle custom painting for viewport overlays
     * 
     * Provides custom painting for viewport overlays, labels, and
     * non-OpenGL visual elements using Qt's 2D painting system.
     * 
     * @param event Qt paint event containing update region information
     * 
     * @post Viewport overlays and labels are painted
     * @post 2D UI elements are rendered over 3D content
     * 
     * @note Called after 3D rendering for overlay content
     * @note Uses Qt 2D painting system for text and 2D graphics
     */
    void paintEvent(QPaintEvent* event) override;

private slots:
    /**
     * @brief Update selection display when selection changes
     * 
     * Slot called when selection state changes, updating visual
     * selection indicators and gizmo visibility.
     * 
     * @post Selection highlighting is updated
     * @post Transform gizmos are shown/hidden based on selection
     * @post Selection-dependent UI elements are updated
     * 
     * @note Connected to selection manager signals
     * @note Updates viewport visual feedback for selection changes
     */
    void updateSelectionDisplay();

private:
    // ========================================================================
    // Selection and Interaction Methods
    // ========================================================================
    
    /**
     * @brief Perform object selection at screen coordinates
     * 
     * Internal method that performs object selection at the specified
     * screen coordinates using OpenGL picking or ray casting.
     * 
     * @param x Screen X coordinate for selection
     * @param y Screen Y coordinate for selection
     * @param mode Selection mode (replace, add, subtract)
     * 
     * @return true if selection was successful, false if no object at coordinates
     * 
     * @note Internal method called by mouse click handlers
     * @note Uses GPU-based picking for accurate selection
     */
    bool performSelection(int x, int y, rude::SelectionMode mode);

    // ========================================================================
    // Core System Components
    // ========================================================================
    
    /// Scene containing all 3D objects for rendering
    std::shared_ptr<rude::Scene> m_scene;
    
    /// Independent camera for this viewport's view
    std::shared_ptr<Camera> m_camera;
    
    /// Camera controller for navigation input handling
    std::unique_ptr<ICameraController> m_cameraController;
    
    /// Lighting system for scene illumination
    std::shared_ptr<LightingSystem> m_lightingSystem;
    
    /// Grid system for reference overlays
    std::shared_ptr<GridSystem> m_gridSystem;
    
    /// Render system for graphics pipeline management
    std::shared_ptr<RenderSystem> m_renderSystem;

    /// Direct renderer for grid and debug drawing
    std::shared_ptr<Renderer> m_renderer;

    // ========================================================================
    // Event System Components
    // ========================================================================
    
    /// Event dispatcher for input event coordination
    std::unique_ptr<event::EventDispatcher> m_eventDispatcher;
    
    /// Mouse input handler for interaction processing
    std::unique_ptr<event::MouseHandler> m_mouseHandler;
    
    /// Event-driven camera controller for advanced navigation
    std::unique_ptr<input::EventDrivenCameraController> m_eventCameraController;
    
    /// Gizmo manager for transform tool visualization
    std::unique_ptr<GizmoManager> m_gizmoManager;

    // ========================================================================
    // Viewport State and Configuration
    // ========================================================================
    
    /// Current view type (perspective, top, front, right, etc.)
    ViewType m_viewType;
    
    /// Display name for viewport identification
    QString m_viewName;
    
    /// Whether this viewport is currently active
    bool m_isActive;
    
    /// Current rendering mode for this viewport
    RenderMode m_renderMode;
    
    /// Whether transform gizmos are visible
    bool m_showGizmos;

    // ========================================================================
    // Mouse Interaction State
    // ========================================================================
    
    /// Last mouse position for drag calculations
    QPoint m_lastMousePos;
    
    /// Whether mouse dragging is currently active
    bool m_isDragging;

    // ========================================================================
    // Rendering and Display Helper Methods
    // ========================================================================
    
    /**
     * @brief Paint viewport overlay elements
     * 
     * Internal method that renders viewport overlay elements including
     * view name, statistics, and navigation aids.
     * 
     * @note Called during paint events for 2D overlay rendering
     * @note Uses Qt 2D painting for text and simple graphics
     */
    void paintViewportOverlay();
    
    /**
     * @brief Draw viewport border indicating active state
     * 
     * Internal method that draws the viewport border with color
     * and style indicating active/inactive state.
     * 
     * @note Active viewports typically have highlighted borders
     * @note Visual feedback helps users identify active viewport
     */
    void drawViewportBorder();
    
    /**
     * @brief Draw viewport label with view name
     * 
     * Internal method that renders the viewport name label
     * in the corner for user identification.
     * 
     * @note Label position is typically top-left or bottom-left corner
     * @note Text style adapts to viewport background for readability
     */
    void drawViewportLabel();
    
    /**
     * @brief Draw viewport statistics and information
     * 
     * Internal method that renders viewport statistics such as
     * triangle count, frame rate, and camera information.
     * 
     * @note Statistics help users monitor performance and scene complexity
     * @note Information display can be toggled for clean workspace
     */
    void drawViewportStats();
    
    /**
     * @brief Configure camera for predefined view type
     * 
     * Internal method that sets up camera position, orientation,
     * and projection for the current predefined view type.
     * 
     * @note Called when view type changes to standard configurations
     * @note Ensures consistent view layouts across application instances
     */
    void setupPredefinedView();
    
    /**
     * @brief Update viewport title based on current state
     * 
     * Internal method that updates the viewport title based on
     * current view type, camera state, and configuration.
     * 
     * @note Title reflects current view mode and state
     * @note Helps users understand viewport configuration at a glance
     */
    void updateViewportTitle();
};

/**
 * @brief Professional multi-viewport layout management system
 * 
 * The ViewportManager class provides comprehensive multi-viewport functionality
 * for professional 3D modeling applications, managing flexible viewport layouts,
 * synchronization between viewports, and coordination of camera controllers.
 * It implements industry-standard multi-viewport workflows found in Maya, 3ds Max,
 * and Blender for enhanced productivity and workflow flexibility.
 * 
 * ## Core Features
 * - **Flexible Layouts**: Single, dual, quad, triple, and custom viewport arrangements
 * - **Independent Viewports**: Each viewport maintains separate camera and rendering state
 * - **Synchronization Options**: Coordinate camera movement and selection across viewports
 * - **Professional Presets**: Standard layout configurations matching industry software
 * - **Dynamic Reconfiguration**: Runtime layout switching without losing viewport state
 * 
 * ## Layout Types
 * The manager supports multiple professional layout configurations:
 * 
 * ### Single Layout (1x1)
 * - **Use Case**: Focus mode for detailed modeling and sculpting
 * - **Benefits**: Maximum viewport size for precise work
 * - **Workflow**: Ideal for artistic modeling and detailed editing
 * 
 * ### Quad Layout (2x2)
 * - **Use Case**: Classic four-view layout for technical modeling
 * - **Configuration**: Perspective + Top + Front + Right views
 * - **Benefits**: Simultaneous orthographic and perspective visualization
 * - **Industry Standard**: Matches Maya, 3ds Max, and CAD software layouts
 * 
 * ### Triple Layout (1x3 or 3x1)
 * - **Use Case**: Primary perspective with supporting orthographic views
 * - **Configuration**: Large perspective + two smaller orthographic views
 * - **Benefits**: Balanced artistic and technical workflow
 * 
 * ### Dual Layout (1x2 or 2x1)
 * - **Use Case**: Comparison workflows and dual-monitor optimization
 * - **Configuration**: Two equal-sized viewports with independent configurations
 * - **Benefits**: Direct comparison of different render modes or views
 * 
 * ### Custom Layout
 * - **Use Case**: Specialized workflows requiring unique viewport arrangements
 * - **Configuration**: User-defined viewport sizes and arrangements
 * - **Benefits**: Optimized for specific modeling or animation tasks
 * 
 * ## Synchronization Features
 * The manager provides sophisticated viewport synchronization:
 * 
 * ### View Synchronization
 * - **Camera Linking**: Synchronized camera movement across viewports
 * - **Focus Coordination**: Shared focus point for orbit navigation
 * - **Zoom Coordination**: Proportional zoom levels across orthographic views
 * - **Frame Operations**: Synchronized framing and view reset operations
 * 
 * ### Selection Synchronization
 * - **Unified Selection**: Selection changes reflected across all viewports
 * - **Multi-Viewport Feedback**: Consistent selection highlighting
 * - **Tool Coordination**: Transform gizmos visible in all relevant viewports
 * - **Workflow Efficiency**: No need to re-select objects per viewport
 * 
 * ## Professional Integration
 * ViewportManager integrates seamlessly with professional modeling workflows:
 * 
 * ### Industry Compatibility
 * - **Maya Layouts**: Matches Autodesk Maya's standard viewport configurations
 * - **Blender Compatibility**: Supports Blender-style viewport arrangements
 * - **CAD Integration**: Technical drawing layouts for engineering workflows
 * - **Animation Workflows**: Camera viewport integration for cinematography
 * 
 * ### Performance Optimization
 * - **Selective Rendering**: Only visible viewports are rendered
 * - **Level of Detail**: Distance-based geometry simplification per viewport
 * - **Resource Sharing**: Efficient sharing of shaders and textures
 * - **Update Coordination**: Minimize redundant rendering operations
 * 
 * ## Advanced Features
 * The manager provides advanced capabilities for professional workflows:
 * 
 * ### Dynamic Reconfiguration
 * - **Runtime Layout Changes**: Switch layouts without losing camera states
 * - **Viewport Persistence**: Camera positions maintained across layout changes
 * - **State Management**: Rendering settings preserved per viewport
 * - **Smooth Transitions**: Animated layout changes for user orientation
 * 
 * ### Multi-Monitor Support
 * - **Viewport Distribution**: Spread viewports across multiple monitors
 * - **Resolution Adaptation**: Automatic adjustment for different monitor DPI
 * - **Focus Management**: Proper keyboard focus across monitor boundaries
 * - **Performance Scaling**: Optimized rendering for multi-monitor setups
 * 
 * ## Usage Examples
 * ```cpp
 * // Create viewport manager with quad layout
 * auto viewportManager = std::make_unique<ViewportManager>(parent);
 * viewportManager->setLayout(ViewportManager::LayoutType::Quad);
 * 
 * // Configure global systems
 * viewportManager->setScene(sceneManager->getCurrentScene());
 * viewportManager->setLightingSystem(lightingSystem);
 * viewportManager->setGridSystem(gridSystem);
 * 
 * // Enable synchronization
 * viewportManager->setSynchronizeViews(true);
 * viewportManager->setSynchronizeSelection(true);
 * 
 * // Apply global render mode
 * viewportManager->setGlobalRenderMode(RenderMode::Solid);
 * 
 * // Frame scene in all viewports
 * viewportManager->frameSceneAll(true);
 * ```
 * 
 * ## Professional Workflow Benefits
 * - **Increased Productivity**: Multiple simultaneous views of the same scene
 * - **Enhanced Precision**: Orthographic views for accurate technical modeling
 * - **Improved Spatial Understanding**: Multiple perspectives aid 3D comprehension
 * - **Workflow Flexibility**: Adapt viewport configuration to specific tasks
 * - **Industry Standard**: Familiar layouts for professional 3D artists
 * 
 * @see ViewportWidget for individual viewport functionality
 * @see Viewport3D for single-viewport 3D rendering
 * @see Camera for view and projection management
 * @see ViewCube for interactive viewport navigation
 * 
 * @ingroup UI
 * @ingroup Viewport
 */
class ViewportManager : public QWidget
{
    Q_OBJECT
    
public:
    // ========================================================================
    // Layout Type Configuration
    // ========================================================================
    
    /**
     * @enum LayoutType
     * @brief Professional viewport layout configurations
     * 
     * Defines the standard multi-viewport layout types commonly used
     * in professional 3D modeling and CAD applications.
     */
    enum class LayoutType {
        /**
         * @brief Single viewport layout (1x1)
         * 
         * Single maximized viewport providing full window space
         * for detailed modeling and focused work.
         */
        Single,
        
        /**
         * @brief Quad viewport layout (2x2)
         * 
         * Four equal viewports arranged in 2x2 grid, typically
         * configured as Perspective + Top + Front + Right views.
         */
        Quad,
        
        /**
         * @brief Triple viewport layout
         * 
         * Three viewports with one large primary viewport and
         * two smaller supporting viewports for multi-view workflows.
         */
        Triple,
        
        /**
         * @brief Dual viewport layout
         * 
         * Two viewports side-by-side or stacked for comparison
         * workflows and dual-monitor optimization.
         */
        Dual,
        
        /**
         * @brief User-defined custom layout
         * 
         * Custom viewport arrangement defined by user requirements
         * for specialized modeling or animation workflows.
         */
        Custom
    };

    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct viewport manager with Qt parent
     * 
     * Creates a new ViewportManager with default single viewport layout,
     * ready for professional multi-viewport 3D modeling workflows.
     * 
     * @param parent Qt parent widget for memory management
     * 
     * @post ViewportManager is created with single viewport layout
     * @post Default viewport is configured and ready for use
     * @post Layout management system is initialized
     * @post Synchronization features are available but disabled by default
     */
    explicit ViewportManager(QWidget* parent = nullptr);
    
    /**
     * @brief Destructor with automatic resource cleanup
     * 
     * Properly cleans up all viewports, releases system references,
     * and disconnects from Qt object hierarchy for clean shutdown.
     */
    ~ViewportManager() override;

    // ========================================================================
    // Layout Management and Configuration
    // ========================================================================
    
    /**
     * @brief Set viewport layout configuration
     * 
     * Changes the viewport layout to the specified configuration,
     * creating or destroying viewports as needed while preserving
     * camera states where possible.
     * 
     * @param layout New layout type to apply
     * 
     * @post Viewport layout is updated to specified configuration
     * @post Existing camera states are preserved where applicable
     * @post New viewports are created with appropriate default views
     * @post layoutChanged() signal is emitted
     * 
     * @note Layout changes may temporarily affect rendering performance
     * @note Camera states are preserved during layout transitions
     * @note New viewports inherit global system assignments
     */
    void setLayout(LayoutType layout);
    
    /**
     * @brief Get current layout type
     * 
     * @return Currently active layout type
     */
    LayoutType getCurrentLayout() const { return m_currentLayout; }

    // ========================================================================
    // Viewport Access and Management
    // ========================================================================
    
    /**
     * @brief Get viewport by index
     * 
     * Returns the viewport widget at the specified index for
     * direct access and configuration.
     * 
     * @param index Index of viewport to retrieve (0-based)
     * 
     * @return Pointer to viewport widget, or nullptr if index is invalid
     * 
     * @note Index validity depends on current layout type and viewport count
     * @note Use getViewportCount() to determine valid index range
     */
    ViewportWidget* getViewport(int index) const;
    
    /**
     * @brief Get currently active viewport
     * 
     * Returns the viewport that is currently active and receiving
     * keyboard input and tool commands.
     * 
     * @return Pointer to active viewport, or nullptr if none active
     * 
     * @note Only one viewport should be active at a time
     * @note Active viewport receives keyboard shortcuts and tool operations
     */
    ViewportWidget* getActiveViewport() const { return m_activeViewport; }
    
    /**
     * @brief Get total number of viewports in current layout
     * 
     * @return Number of viewports in the current layout
     * 
     * @note Count varies by layout type (1 for Single, 4 for Quad, etc.)
     */
    int getViewportCount() const { return m_viewports.size(); }
    
    /**
     * @brief Get list of all viewports
     * 
     * @return Const reference to list of all viewport widgets
     * 
     * @note Use for iteration over all viewports in current layout
     * @note List order corresponds to viewport indices
     */
    const QList<ViewportWidget*>& getViewports() const { return m_viewports; }

    // ========================================================================
    // Global System Assignment
    // ========================================================================
    
    /**
     * @brief Assign scene to all viewports
     * 
     * Sets the 3D scene for all viewports in the current layout,
     * enabling multi-viewport visualization of the same scene content.
     * 
     * @param scene Shared pointer to the scene to visualize
     * 
     * @post Scene is assigned to all current and future viewports
     * @post All viewports display the same scene content
     * @post Scene changes are immediately reflected in all viewports
     * 
     * @note All viewports share the same scene but maintain independent cameras
     * @note Scene assignment is preserved during layout changes
     */
    void setScene(std::shared_ptr<rude::Scene> scene);
    
    /**
     * @brief Assign lighting system to all viewports
     * 
     * Sets the lighting system for all viewports, providing consistent
     * illumination across the multi-viewport layout.
     * 
     * @param lightingSystem Shared pointer to lighting system
     * 
     * @post Lighting system is assigned to all current and future viewports
     * @post All viewports use consistent illumination settings
     * @post Lighting changes affect all viewports simultaneously
     * 
     * @note Shared lighting system ensures consistent visualization
     * @note Professional lighting presets become available in all viewports
     */
    void setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem);
    
    /**
     * @brief Assign grid system to all viewports
     * 
     * Sets the grid system for all viewports, providing consistent
     * reference grids and construction aids across views.
     * 
     * @param gridSystem Shared pointer to grid system
     * 
     * @post Grid system is assigned to all current and future viewports
     * @post All viewports can display consistent reference grids
     * @post Grid visibility can still be controlled independently per viewport
     * 
     * @note Shared grid system ensures consistent measurement references
     * @note Grid visibility can be toggled independently per viewport
     */
    void setGridSystem(std::shared_ptr<GridSystem> gridSystem);
    
    /**
     * @brief Assign render system to all viewports
     * 
     * Sets the render system for all viewports, providing unified
     * rendering pipeline management and performance optimization.
     * 
     * @param renderSystem Shared pointer to render system
     * 
     * @post Render system is assigned to all current and future viewports
     * @post Unified rendering pipeline optimizes multi-viewport performance
     * @post Advanced rendering features become available in all viewports
     * 
     * @note Shared render system optimizes resource usage and performance
     * @note System coordinates rendering across multiple viewports efficiently
     */
    void setRenderSystem(std::shared_ptr<RenderSystem> renderSystem);

    // ========================================================================
    // Camera Controller Management
    // ========================================================================
    
    /**
     * @brief Set camera controller type for all viewports
     * 
     * Configures the camera controller type used by all viewports,
     * providing consistent navigation behavior across the layout.
     * 
     * @param controllerType String identifier for controller type
     * 
     * @post All viewports use the specified camera controller type
     * @post Navigation behavior is consistent across all viewports
     * @post Controller settings are applied to new viewports
     * 
     * @note Common types: "orbit", "fly", "walk", "maya", "blender"
     * @note Controller type affects navigation feel and interaction
     */
    void setCameraControllerType(const QString& controllerType);
    
    /**
     * @brief Get current camera controller type
     * 
     * @return String identifier of current camera controller type
     */
    QString getCameraControllerType() const { return m_controllerType; }

    // ========================================================================
    // Viewport Synchronization Control
    // ========================================================================
    
    /**
     * @brief Enable or disable view synchronization
     * 
     * Controls whether camera movements in one viewport are synchronized
     * to other viewports, enabling coordinated multi-viewport navigation.
     * 
     * @param sync true to enable view synchronization, false to disable
     * 
     * @post View synchronization state is updated
     * @post Camera movements may be coordinated across viewports
     * 
     * @note Synchronization typically affects orbit center and zoom levels
     * @note Useful for maintaining focus across orthographic views
     * @note May be disabled for independent viewport navigation
     */
    void setSynchronizeViews(bool sync) { m_synchronizeViews = sync; }
    
    /**
     * @brief Check if views are synchronized
     * 
     * @return true if view synchronization is enabled, false if disabled
     */
    bool arViewsSynchronized() const { return m_synchronizeViews; }
    
    /**
     * @brief Enable or disable selection synchronization
     * 
     * Controls whether object selection changes are synchronized across
     * all viewports, providing unified selection feedback.
     * 
     * @param sync true to enable selection synchronization, false to disable
     * 
     * @post Selection synchronization state is updated
     * @post Selection changes may be reflected across all viewports
     * 
     * @note Synchronized selection provides consistent visual feedback
     * @note Transform gizmos appear in all viewports when enabled
     * @note Useful for multi-viewport modeling workflows
     */
    void setSynchronizeSelection(bool sync) { m_synchronizeSelection = sync; }
    
    /**
     * @brief Check if selection is synchronized
     * 
     * @return true if selection synchronization is enabled, false if disabled
     */
    bool isSelectionSynchronized() const { return m_synchronizeSelection; }

    // ========================================================================
    // Global Viewport Operations
    // ========================================================================
    
    /**
     * @brief Frame scene in all viewports with optional animation
     * 
     * Automatically positions and zooms cameras in all viewports to
     * optimally frame the entire scene content.
     * 
     * @param animate true to animate camera transitions, false for instant
     * 
     * @post All viewport cameras frame the scene optimally
     * @post Camera positions provide best overview of scene content
     * @post Animation provides smooth transitions if enabled
     * 
     * @note Framing is calculated independently for each viewport's view type
     * @note Orthographic views frame scene bounds, perspective uses viewing distance
     * @note Animation helps maintain spatial orientation across viewports
     */
    void frameSceneAll(bool animate = true);
    
    /**
     * @brief Frame selected objects in all viewports with optional animation
     * 
     * Automatically positions and zooms cameras in all viewports to
     * optimally frame the currently selected objects.
     * 
     * @param animate true to animate camera transitions, false for instant
     * 
     * @post All viewport cameras frame selected objects optimally
     * @post Camera positions provide best viewing angles for selection
     * @post Falls back to scene framing if nothing is selected
     * 
     * @note Framing calculation considers selection bounds across all viewports
     * @note Multiple selected objects are framed as a unified group
     * @note Animation maintains spatial orientation during framing
     */
    void frameSelectionAll(bool animate = true);
    
    /**
     * @brief Reset all viewport cameras to default positions
     * 
     * Moves all viewport cameras to their default positions appropriate
     * for their respective view types (perspective, orthographic, etc.).
     * 
     * @post All viewport cameras are reset to view type defaults
     * @post Camera orientations match standard view directions
     * @post Zoom levels are set to default modeling distances
     * 
     * @note Default positions vary by viewport view type
     * @note Provides consistent starting point for modeling workflows
     * @note Useful for recovering from disorienting camera movements
     */
    void resetAllCameras();

    // ========================================================================
    // Global Rendering and Visualization Control
    // ========================================================================
    
    /**
     * @brief Set render mode for all viewports
     * 
     * Applies the specified render mode to all viewports in the layout,
     * providing unified visualization across the multi-viewport interface.
     * 
     * @param mode Render mode to apply to all viewports
     * 
     * @post All viewports use the specified render mode
     * @post Visualization is consistent across all views
     * @post Individual viewport render modes can still be changed afterward
     * 
     * @note Provides quick way to set consistent visualization
     * @note Individual viewports can override global setting if needed
     * @note Useful for workflow-specific render mode configurations
     */
    void setGlobalRenderMode(RenderMode mode);
    
    /**
     * @brief Set grid visibility for all viewports
     * 
     * Controls reference grid visibility across all viewports in the layout,
     * providing unified grid display control.
     * 
     * @param visible true to show grids in all viewports, false to hide
     * 
     * @post Grid visibility is updated in all viewports
     * @post Grid rendering is enabled/disabled for performance
     * @post Individual viewport grid visibility can still be changed afterward
     * 
     * @note Provides quick way to control grid display across all views
     * @note Individual viewports can override global setting if needed
     * @note Hidden grids still provide snapping functionality if enabled
     */
    void setGridVisibilityAll(bool visible);
    
    /**
     * @brief Set transform gizmo visibility for all viewports
     * 
     * Controls transform gizmo visibility across all viewports in the layout,
     * providing unified gizmo display control.
     * 
     * @param visible true to show gizmos in all viewports, false to hide
     * 
     * @post Gizmo visibility is updated in all viewports
     * @post Transform tool interaction is enabled/disabled accordingly
     * @post Individual viewport gizmo visibility can still be changed afterward
     * 
     * @note Provides quick way to control gizmo display across all views
     * @note Individual viewports can override global setting if needed
     * @note Gizmo visibility depends on object selection state
     */
    void setGizmosVisibilityAll(bool visible);

signals:
    // ========================================================================
    // Layout Management Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when active viewport changes
     * 
     * Emitted when a different viewport becomes active, allowing external
     * systems to update UI state and coordinate tool behavior.
     * 
     * @param viewport Pointer to the newly active viewport
     * 
     * @note Connect to toolbar and menu systems for context-sensitive UI
     * @note Active viewport receives keyboard input and tool commands
     */
    void activeViewportChanged(ViewportWidget* viewport);
    
    /**
     * @brief Signal emitted when layout configuration changes
     * 
     * Emitted when the viewport layout is changed, allowing external
     * systems to adapt to the new viewport configuration.
     * 
     * @param layout New layout type that has been applied
     * 
     * @note Connect to UI elements that need to adapt to layout changes
     * @note Layout changes may affect available viewport-specific controls
     */
    void layoutChanged(LayoutType layout);
    
    /**
     * @brief Signal emitted when viewport count changes
     * 
     * Emitted when the number of viewports changes due to layout
     * modifications, allowing systems to update viewport-specific state.
     * 
     * @param count New number of viewports in the layout
     * 
     * @note Connect to systems that manage per-viewport state or UI
     * @note Viewport count changes during layout transitions
     */
    void viewportCountChanged(int count);

private slots:
    /**
     * @brief Handle viewport activation events
     * 
     * Slot called when any viewport becomes active, coordinating
     * active state across the viewport layout.
     * 
     * @param viewport Pointer to the viewport that became active
     * 
     * @post Active viewport state is coordinated across layout
     * @post Previous active viewport is deactivated
     * @post activeViewportChanged() signal is emitted
     * 
     * @note Ensures only one viewport is active at a time
     * @note Connected to all viewport activation signals
     */
    void onViewportActivated(ViewportWidget* viewport);
    
    /**
     * @brief Handle viewport camera changes for synchronization
     * 
     * Slot called when any viewport camera changes, handling view
     * synchronization if enabled.
     * 
     * @post Camera changes are synchronized to other viewports if enabled
     * @post View coordination is maintained across synchronized viewports
     * 
     * @note Connected to all viewport camera change signals
     * @note Only processes changes when view synchronization is enabled
     */
    void onViewportCameraChanged();

private:
    // ========================================================================
    // Layout Management State
    // ========================================================================
    
    /// Current active layout type
    LayoutType m_currentLayout;
    
    /// Grid layout widget managing viewport arrangement
    QGridLayout* m_gridLayout;
    
    /// List of all viewports in current layout
    QList<ViewportWidget*> m_viewports;
    
    /// List of active GLFW viewports (temporary for testing)
    QList<class GLFWViewport*> m_activeGLFWViewports;
    
    /// Currently active viewport receiving input
    ViewportWidget* m_activeViewport;

    // ========================================================================
    // Global System References
    // ========================================================================
    
    /// Scene shared across all viewports
    std::shared_ptr<rude::Scene> m_scene;
    
    /// Lighting system shared across all viewports
    std::shared_ptr<LightingSystem> m_lightingSystem;
    
    /// Grid system shared across all viewports
    std::shared_ptr<GridSystem> m_gridSystem;
    
    /// Render system shared across all viewports
    std::shared_ptr<RenderSystem> m_renderSystem;

    // ========================================================================
    // Configuration and Synchronization Settings
    // ========================================================================
    
    /// Camera controller type for all viewports
    QString m_controllerType;
    
    /// Whether camera movements are synchronized across viewports
    bool m_synchronizeViews;
    
    /// Whether selection changes are synchronized across viewports
    bool m_synchronizeSelection;

    // ========================================================================
    // Layout Creation and Management Methods
    // ========================================================================
    
    /**
     * @brief Create single viewport layout
     * 
     * Internal method that creates a single maximized viewport
     * for focused modeling workflows.
     * 
     * @post Single viewport layout is created and configured
     * @note Called internally by setLayout() for Single layout type
     */
    void createSingleLayout();
    
    /**
     * @brief Create quad viewport layout (2x2)
     * 
     * Internal method that creates four viewports arranged in 2x2 grid
     * with standard perspective + orthographic view configuration.
     * 
     * @post Quad viewport layout is created with standard view types
     * @note Called internally by setLayout() for Quad layout type
     */
    void createQuadLayout();
    
    /**
     * @brief Create triple viewport layout
     * 
     * Internal method that creates three viewports with one large primary
     * viewport and two smaller supporting viewports.
     * 
     * @post Triple viewport layout is created and configured
     * @note Called internally by setLayout() for Triple layout type
     */
    void createTripleLayout();
    
    /**
     * @brief Create dual viewport layout
     * 
     * Internal method that creates two viewports side-by-side or stacked
     * for comparison workflows and dual-monitor optimization.
     * 
     * @post Dual viewport layout is created and configured
     * @note Called internally by setLayout() for Dual layout type
     */
    void createDualLayout();
    
    /**
     * @brief Clear current viewport layout
     * 
     * Internal method that removes all viewports from the current layout
     * and cleans up resources in preparation for new layout creation.
     * 
     * @post All viewports are removed and cleaned up
     * @post Layout is ready for new viewport creation
     * @note Called internally before creating new layouts
     */
    void clearLayout();

    // ========================================================================
    // Viewport Creation and Configuration
    // ========================================================================
    
    /**
     * @brief Create new viewport with specified configuration
     * 
     * Internal method that creates a new viewport widget with the
     * specified view type and display name.
     * 
     * @param type View type for the new viewport (perspective, top, etc.)
     * @param name Display name for the viewport
     * 
     * @return Pointer to newly created viewport widget
     * 
     * @post Viewport is created with specified type and name
     * @post Global systems are assigned to new viewport
     * @post Default settings are applied to new viewport
     * @note Called internally by layout creation methods
     */
    ViewportWidget* createViewport(ViewportWidget::ViewType type, const QString& name);
    
    /**
     * @brief Apply default settings to viewport
     * 
     * Internal method that applies default settings and assigns global
     * systems to a newly created viewport.
     * 
     * @param viewport Pointer to viewport to configure
     * 
     * @post Viewport has global systems assigned
     * @post Default camera controller is created and assigned
     * @post Viewport is ready for use in layout
     * @note Called for each newly created viewport
     */
    void setupViewportDefaults(ViewportWidget* viewport);
    
    /**
     * @brief Connect viewport signals to manager slots
     * 
     * Internal method that connects viewport signals to appropriate
     * manager slots for coordination and synchronization.
     * 
     * @param viewport Pointer to viewport to connect
     * 
     * @post Viewport signals are connected to manager coordination
     * @post Activation and camera change events are handled
     * @post Synchronization features are enabled for viewport
     * @note Called for each newly created viewport
     */
    void connectViewportSignals(ViewportWidget* viewport);

    // ========================================================================
    // Synchronization Implementation
    // ========================================================================
    
    /**
     * @brief Synchronize viewport settings from source viewport
     * 
     * Internal method that synchronizes camera and rendering settings
     * from source viewport to other viewports when synchronization is enabled.
     * 
     * @param sourceViewport Viewport whose settings should be synchronized
     * 
     * @post Other viewports are updated with synchronized settings
     * @post Synchronization respects view type differences
     * @note Called when synchronization is enabled and viewport changes occur
     */
    void synchronizeViewportSettings(ViewportWidget* sourceViewport);
};

/**
 * @brief Professional 3D viewport navigation cube widget
 * 
 * The ViewCube provides an intuitive 3D navigation interface for viewport
 * camera control, offering visual representation of current view orientation
 * and instant access to standard orthographic and perspective views.
 * This widget implements industry-standard navigation patterns found in
 * professional CAD and 3D modeling applications like AutoCAD, Maya, and 3ds Max.
 * 
 * ## Core Features
 * - **Visual 3D Cube**: Real-time 3D cube representing current camera orientation
 * - **Instant View Access**: Click faces, edges, or corners for standard views
 * - **Interactive Rotation**: Drag to interactively rotate viewport camera
 * - **Standard Views**: Access to Front, Back, Left, Right, Top, Bottom views
 * - **Perspective Toggle**: Quick switch between orthographic and perspective
 * - **Home Position**: Reset to default isometric or user-defined home view
 * 
 * ## Navigation Interface
 * The ViewCube provides multiple interaction methods:
 * 
 * ### Face-Based Navigation
 * - **Front/Back Faces**: Standard front and rear orthographic views
 * - **Left/Right Faces**: Standard side orthographic views
 * - **Top/Bottom Faces**: Plan and bottom orthographic views
 * - **Visual Feedback**: Face highlighting during hover for clear targeting
 * 
 * ### Edge-Based Navigation
 * - **Edge Clicks**: Provides intermediate viewing angles between faces
 * - **Angular Precision**: 45-degree angle views between primary faces
 * - **Professional Workflow**: Matches industry standard navigation patterns
 * - **Smooth Transitions**: Animated camera movement to target orientations
 * 
 * ### Corner-Based Navigation
 * - **Isometric Views**: Standard isometric viewing angles from cube corners
 * - **3D Perspective**: Optimal angles for general 3D modeling work
 * - **Multiple Options**: 8 corner positions for varied isometric angles
 * - **Default Home**: Corner views often serve as home position
 * 
 * ### Interactive Rotation
 * - **Direct Manipulation**: Drag cube to rotate camera interactively
 * - **Real-Time Update**: Camera orientation updates during drag operation
 * - **Proportional Control**: Drag distance controls rotation magnitude
 * - **Constrained Rotation**: Maintains proper camera up vector
 * 
 * ## Professional Integration
 * ViewCube integrates seamlessly with professional 3D workflows:
 * 
 * ### Industry Compatibility
 * - **AutoCAD Style**: Matches Autodesk navigation conventions
 * - **Maya Integration**: Compatible with Maya viewport navigation
 * - **3ds Max Patterns**: Follows 3ds Max navigation paradigms
 * - **SolidWorks Standards**: CAD-style navigation for technical modeling
 * 
 * ### Camera Controller Integration
 * - **Controller Coordination**: Works with any ICameraController implementation
 * - **Smooth Transitions**: Camera controller handles view change animations
 * - **State Preservation**: Maintains camera distance and focus point
 * - **Projection Compatibility**: Works with both orthographic and perspective
 * 
 * ## Visual Design
 * The ViewCube provides clear visual feedback:
 * 
 * ### 3D Representation
 * - **Shaded Faces**: 3D shading indicates current camera orientation
 * - **Face Labels**: Clear text labels on cube faces (Front, Top, etc.)
 * - **Edge Highlighting**: Visual feedback during hover and interaction
 * - **Orientation Feedback**: Cube rotation matches viewport camera orientation
 * 
 * ### Professional Appearance
 * - **Consistent Styling**: Matches overall application theme and appearance
 * - **Clear Typography**: Readable face labels at various widget sizes
 * - **Subtle Animation**: Smooth transitions for professional feel
 * - **Compact Design**: Efficient screen space usage in viewport corner
 * 
 * ## Performance Optimization
 * The ViewCube is optimized for responsive interaction:
 * 
 * ### Rendering Efficiency
 * - **Static Geometry**: Cube geometry is pre-computed for efficiency
 * - **Minimal Redraws**: Only redraws when orientation changes
 * - **Optimized Painting**: Efficient Qt painter operations
 * - **Level of Detail**: Simplified rendering at smaller sizes
 * 
 * ### Interaction Responsiveness
 * - **Fast Hit Testing**: Efficient calculation of face/edge/corner selection
 * - **Immediate Feedback**: Instant visual response to mouse interaction
 * - **Smooth Dragging**: Responsive camera updates during interactive rotation
 * - **Event Optimization**: Minimal processing during mouse movement
 * 
 * ## Usage Examples
 * ```cpp
 * // Create ViewCube for a viewport
 * auto viewCube = new ViewCube(viewport);
 * viewCube->setCameraController(viewport->getCameraController());
 * 
 * // Connect to viewport for view changes
 * connect(viewCube, &ViewCube::viewChangeRequested,
 *         viewport, &ViewportWidget::setViewType);
 * 
 * // Position ViewCube in viewport corner
 * viewCube->setGeometry(viewport->width() - 100, 10, 80, 80);
 * ```
 * 
 * ## Professional Benefits
 * - **Intuitive Navigation**: Visual 3D metaphor is immediately understandable
 * - **Rapid View Changes**: Instant access to standard viewing positions
 * - **Spatial Orientation**: Cube provides constant orientation reference
 * - **Workflow Efficiency**: Reduces time spent navigating 3D space
 * - **Industry Standard**: Familiar interface for professional 3D users
 * 
 * @see ViewportWidget for viewport integration
 * @see ICameraController for camera control interface
 * @see ViewportManager for multi-viewport coordination
 * 
 * @ingroup UI
 * @ingroup Viewport
 * @ingroup Navigation
 */
class ViewCube : public QWidget
{
    Q_OBJECT
    
public:
    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct ViewCube navigation widget
     * 
     * Creates a new ViewCube widget ready for 3D viewport navigation,
     * with default size and appearance settings appropriate for
     * embedding in viewport corners.
     * 
     * @param parent Qt parent widget for memory management
     * 
     * @post ViewCube is created and ready for camera controller assignment
     * @post Widget has default size appropriate for viewport embedding
     * @post Cube geometry and rendering are initialized
     * @post Mouse interaction is enabled for navigation
     */
    explicit ViewCube(QWidget* parent = nullptr);

    // ========================================================================
    // Camera Controller Integration
    // ========================================================================
    
    /**
     * @brief Assign camera controller for navigation
     * 
     * Sets the camera controller that the ViewCube will manipulate
     * for viewport navigation and orientation changes.
     * 
     * @param controller Pointer to camera controller to control
     * 
     * @post ViewCube controls the specified camera controller
     * @post View change requests will be sent to the controller
     * @post Cube orientation reflects controller's current state
     * 
     * @note Camera controller must remain valid for ViewCube lifetime
     * @note Controller handles view transition animations and constraints
     * @note Multiple ViewCubes can share the same controller if needed
     */
    void setCameraController(ICameraController* controller);

    // ========================================================================
    // Visual Configuration
    // ========================================================================
    
    /**
     * @brief Set ViewCube size and appearance
     * 
     * Configures the visual size and scaling of the ViewCube widget
     * for optimal integration with different viewport sizes.
     * 
     * @param size Preferred size for the cube widget in pixels
     * 
     * @post ViewCube widget size is updated
     * @post Cube rendering is scaled appropriately
     * @post Text labels are sized for readability
     * 
     * @note Typical sizes range from 60x60 to 120x120 pixels
     * @note Larger sizes provide better text readability
     * @note Smaller sizes save viewport space for modeling
     */
    void setCubeSize(int size);
    
    /**
     * @brief Enable or disable face labels
     * 
     * Controls whether text labels (Front, Top, etc.) are displayed
     * on cube faces for enhanced navigation clarity.
     * 
     * @param showLabels true to show face labels, false to hide
     * 
     * @post Face labels are shown or hidden according to setting
     * @post Cube appearance is updated immediately
     * 
     * @note Labels improve navigation clarity but require more screen space
     * @note Hidden labels provide cleaner appearance for compact layouts
     * @note Labels automatically scale with cube size
     */
    void setShowLabels(bool showLabels);
    
    /**
     * @brief Set home view orientation
     * 
     * Configures the default view orientation used when the home
     * button is clicked or when resetting the viewport.
     * 
     * @param homeView Default view type for home position
     * 
     * @post Home view is set to specified orientation
     * @post Home button navigation uses the specified view
     * 
     * @note Common home views include isometric corners or front view
     * @note Home view provides consistent starting point for modeling
     * @note Can be customized for different modeling workflows
     */
    void setHomeView(ViewportWidget::ViewType homeView);

signals:
    // ========================================================================
    // Navigation Request Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when user requests view change
     * 
     * Emitted when the user clicks on a cube face, edge, or corner
     * to request a specific viewport orientation change.
     * 
     * @param view Requested view type based on user interaction
     * 
     * @note Connect to ViewportWidget::setViewType() for direct navigation
     * @note View type corresponds to clicked cube region
     * @note Signal provides standard view types for immediate use
     */
    void viewChangeRequested(ViewportWidget::ViewType view);
    
    /**
     * @brief Signal emitted during interactive rotation
     * 
     * Emitted continuously during drag-based interactive rotation
     * to provide real-time camera orientation updates.
     * 
     * @param deltaX Horizontal rotation delta in pixels
     * @param deltaY Vertical rotation delta in pixels
     * 
     * @note Connect to camera controller for real-time rotation
     * @note Delta values are relative to previous mouse position
     * @note Enables smooth interactive camera manipulation
     */
    void interactiveRotation(float deltaX, float deltaY);
    
    /**
     * @brief Signal emitted when home view is requested
     * 
     * Emitted when the user clicks the home button or requests
     * a return to the default viewport orientation.
     * 
     * @note Connect to viewport reset functionality
     * @note Provides quick return to standard modeling orientation
     * @note Home view can be customized per viewport or workflow
     */
    void homeViewRequested();

protected:
    // ========================================================================
    // Qt Widget Event Handling
    // ========================================================================
    
    /**
     * @brief Handle widget paint events for cube rendering
     * 
     * Renders the 3D ViewCube using Qt painter operations,
     * including cube geometry, face labels, and highlighting.
     * 
     * @param event Paint event information from Qt
     * 
     * @post ViewCube is rendered with current orientation and state
     * @post Face highlighting reflects current mouse position
     * @post Text labels are drawn with appropriate scaling
     * 
     * @note Called automatically by Qt when widget needs repainting
     * @note Implements efficient rendering for smooth interaction
     */
    void paintEvent(QPaintEvent* event) override;
    
    /**
     * @brief Handle mouse press events for navigation initiation
     * 
     * Processes mouse press events to determine navigation intent
     * and initiate appropriate navigation or interaction mode.
     * 
     * @param event Mouse event information from Qt
     * 
     * @post Navigation mode is determined based on click location
     * @post Interactive dragging is initiated if appropriate
     * @post Face/edge/corner selection is calculated
     * 
     * @note Determines whether user intends view change or interactive rotation
     * @note Hit testing identifies clicked cube region
     */
    void mousePressEvent(QMouseEvent* event) override;
    
    /**
     * @brief Handle mouse release events for navigation completion
     * 
     * Processes mouse release events to complete navigation actions
     * and finalize view changes or interactive operations.
     * 
     * @param event Mouse event information from Qt
     * 
     * @post View change signals are emitted if appropriate
     * @post Interactive dragging mode is terminated
     * @post Final navigation state is established
     * 
     * @note Completes navigation actions initiated by mouse press
     * @note Emits appropriate signals for viewport updates
     */
    void mouseReleaseEvent(QMouseEvent* event) override;
    
    /**
     * @brief Handle mouse movement for interactive rotation
     * 
     * Processes mouse movement during interactive rotation mode
     * to provide real-time camera orientation feedback.
     * 
     * @param event Mouse event information from Qt
     * 
     * @post Interactive rotation signals are emitted with movement deltas
     * @post Cube orientation is updated to reflect dragging
     * @post Visual feedback is provided during interaction
     * 
     * @note Only processes movement during active dragging operations
     * @note Provides smooth real-time camera manipulation
     */
    void mouseMoveEvent(QMouseEvent* event) override;
    
    /**
     * @brief Handle mouse enter events for interaction feedback
     * 
     * Processes mouse enter events to provide visual feedback
     * when the mouse enters the ViewCube widget area.
     * 
     * @param event Mouse event information from Qt
     * 
     * @post Visual feedback indicates interactive state
     * @post Hover highlighting is activated
     * 
     * @note Provides clear indication of interactive capability
     * @note Enhances user experience with responsive feedback
     */
    void enterEvent(QEnterEvent* event) override;
    
    /**
     * @brief Handle mouse leave events to clear interaction feedback
     * 
     * Processes mouse leave events to clear visual feedback
     * when the mouse leaves the ViewCube widget area.
     * 
     * @param event Mouse event information from Qt
     * 
     * @post Visual feedback is cleared
     * @post Hover highlighting is deactivated
     * 
     * @note Ensures clean appearance when not actively interacting
     * @note Maintains visual consistency with application theme
     */
    void leaveEvent(QEvent* event) override;

private:
    // ========================================================================
    // Camera Integration State
    // ========================================================================
    
    /// Camera controller for viewport navigation
    ICameraController* m_cameraController;

    // ========================================================================
    // Interaction State and Tracking
    // ========================================================================
    
    /// Last recorded mouse position for delta calculation
    QPoint m_lastMousePos;
    
    /// Current mouse position for hover feedback
    QPoint m_currentMousePos;
    
    /// Whether interactive dragging is currently active
    bool m_isDragging;
    
    /// Whether mouse is currently over the widget
    bool m_isHovered;
    
    /// Currently highlighted cube region (face, edge, or corner)
    int m_highlightedRegion;

    // ========================================================================
    // Visual Configuration State
    // ========================================================================
    
    /// Size of the cube widget in pixels
    int m_cubeSize;
    
    /// Whether to display text labels on cube faces
    bool m_showLabels;
    
    /// Home view orientation for reset operations
    ViewportWidget::ViewType m_homeView;
    
    /// Current cube orientation for rendering
    QMatrix4x4 m_cubeOrientation;

    // ========================================================================
    // Rendering and Geometry Methods
    // ========================================================================
    
    /**
     * @brief Render the 3D cube with current orientation and highlighting
     * 
     * Internal method that renders the ViewCube using Qt painter operations,
     * including 3D projection, face shading, and text labels.
     * 
     * @param painter Qt painter object for rendering operations
     * 
     * @post Cube is rendered with appropriate 3D appearance
     * @post Face highlighting reflects current interaction state
     * @post Text labels are drawn with proper positioning and scaling
     * 
     * @note Uses 3D to 2D projection for proper cube appearance
     * @note Implements face sorting for correct depth rendering
     * @note Provides smooth shading for professional appearance
     */
    void drawCube(QPainter& painter);
    
    /**
     * @brief Draw individual cube face with appropriate styling
     * 
     * Internal method that renders a single cube face with proper
     * shading, highlighting, and label text.
     * 
     * @param painter Qt painter object for rendering operations
     * @param faceVertices 2D projected vertices of the face
     * @param faceNormal Normal vector of the face for shading
     * @param label Text label for the face (e.g., "Front", "Top")
     * @param isHighlighted Whether this face should be highlighted
     * 
     * @post Face is rendered with appropriate styling and shading
     * @post Text label is positioned and rendered if labels are enabled
     * @post Highlighting is applied if face is currently selected
     * 
     * @note Face shading depends on normal vector and lighting direction
     * @note Text positioning is calculated for optimal readability
     */
    void drawCubeFace(QPainter& painter, const QPolygonF& faceVertices, 
                      const QVector3D& faceNormal, const QString& label, 
                      bool isHighlighted);
    
    /**
     * @brief Update cube orientation based on camera controller state
     * 
     * Internal method that synchronizes the ViewCube's visual orientation
     * with the current camera controller orientation.
     * 
     * @post Cube orientation matrix reflects camera controller state
     * @post Visual representation matches viewport camera orientation
     * @post Widget is scheduled for repaint if orientation changed
     * 
     * @note Called when camera controller state changes
     * @note Maintains visual consistency between cube and viewport
     * @note Provides real-time orientation feedback
     */
    void updateCubeOrientation();

    // ========================================================================
    // Hit Testing and Interaction Methods
    // ========================================================================
    
    /**
     * @brief Determine view type from mouse click position
     * 
     * Internal method that performs hit testing to determine which
     * standard view type corresponds to a mouse click position.
     * 
     * @param pos Mouse position in widget coordinates
     * 
     * @return View type corresponding to clicked cube region
     * 
     * @note Performs 3D hit testing against cube geometry
     * @note Returns most appropriate standard view for clicked region
     * @note Handles face, edge, and corner click disambiguation
     */
    ViewportWidget::ViewType getViewFromPosition(const QPoint& pos);
    
    /**
     * @brief Calculate highlighted region from mouse position
     * 
     * Internal method that determines which cube region should be
     * highlighted based on current mouse position for hover feedback.
     * 
     * @param pos Mouse position in widget coordinates
     * 
     * @return Region identifier for highlighting, or -1 if none
     * 
     * @note Provides visual feedback during mouse hover
     * @note Helps users understand click targets before clicking
     * @note Includes faces, edges, and corners in hit testing
     */
    int getHighlightedRegion(const QPoint& pos);
    
    /**
     * @brief Project 3D cube vertices to 2D screen coordinates
     * 
     * Internal method that projects 3D cube geometry to 2D screen
     * coordinates for rendering and hit testing operations.
     * 
     * @param vertices 3D vertices to project
     * 
     * @return 2D projected vertices in widget coordinates
     * 
     * @note Uses perspective projection for realistic 3D appearance
     * @note Projection parameters are optimized for cube rendering
     * @note Results are used for both rendering and hit testing
     */
    QList<QPointF> projectVertices(const QList<QVector3D>& vertices);
    
    /**
     * @brief Test if point is inside projected polygon
     * 
     * Internal method that performs 2D point-in-polygon testing
     * for hit testing against projected cube faces.
     * 
     * @param point Point to test in widget coordinates
     * @param polygon Projected polygon to test against
     * 
     * @return true if point is inside polygon, false otherwise
     * 
     * @note Uses standard point-in-polygon algorithm
     * @note Handles both convex and concave polygons correctly
     * @note Essential for accurate cube face hit testing
     */
    bool pointInPolygon(const QPointF& point, const QPolygonF& polygon);
};
