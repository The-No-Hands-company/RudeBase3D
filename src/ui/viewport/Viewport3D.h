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
 * █  FILE: Viewport3D.h                                                          █
 * █  DESCRIPTION: Professional 3D Viewport & OpenGL Rendering System             █
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
 * █  This file contains the Viewport3D implementation providing professional     █
 * █  3D scene visualization, camera controls, and interactive rendering for      █
 * █  modeling, animation, and visualization workflows in 3D applications.        █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file Viewport3D.h
 * @brief Primary 3D viewport widget for professional modeling interface
 * 
 * This file contains the Viewport3D class which serves as the main 3D visualization
 * widget in RudeBase3D. It integrates OpenGL rendering, user interaction, camera
 * control, lighting, grid systems, and selection management into a cohesive
 * professional 3D modeling interface optimized for productivity workflows.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup UI
 * @ingroup Rendering
 */

#pragma once

#include "Common.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <memory>

// Forward declarations
namespace rude { class Scene; }
class Camera;
class Renderer;
class ICameraController;
class InputController;
class SelectionManager;
class LightingSystem;
class GridSystem;

/**
 * @addtogroup UI
 * @{
 */

/**
 * @brief Professional 3D viewport widget integrating rendering and interaction systems
 * 
 * The Viewport3D class is the primary 3D visualization widget in RudeBase3D, providing
 * a complete professional modeling interface that combines OpenGL rendering, user
 * interaction, camera navigation, lighting management, and selection tools into a
 * unified widget optimized for 3D modeling workflows.
 * 
 * ## Core Architecture
 * The viewport integrates multiple specialized systems:
 * - **Rendering Pipeline**: High-performance OpenGL rendering with modern shaders
 * - **Camera System**: Professional camera navigation with orbit, pan, zoom controls
 * - **Lighting System**: Industry-standard three-point lighting with real-time adjustment
 * - **Grid System**: Professional reference grids with adaptive detail levels
 * - **Selection System**: Multi-mode object selection with visual feedback
 * - **Input Management**: Intuitive mouse and keyboard interaction handling
 * 
 * ## Professional Features
 * The viewport provides features expected in professional 3D modeling software:
 * 
 * ### Multiple Render Modes
 * - **Solid Shading**: Full material rendering with lighting and shadows
 * - **Wireframe**: Edge-only topology visualization for mesh analysis
 * - **Solid+Wireframe**: Combined view showing both surfaces and edges
 * - **Points**: Vertex-only display for detailed topology inspection
 * 
 * ### Advanced Camera Controls
 * - **Orbit Navigation**: Smooth rotation around focus point or selection
 * - **Pan Movement**: Lateral camera translation for precise positioning
 * - **Zoom Control**: Distance-based scaling with configurable sensitivity
 * - **Frame Operations**: Automatic framing of scenes or selected objects
 * - **View Presets**: Quick access to standard orthographic and perspective views
 * 
 * ### Professional Lighting
 * - **Three-Point Setup**: Key, fill, and rim lights following industry standards
 * - **Preset Configurations**: Maya, Blender, Studio, and Outdoor lighting modes
 * - **Real-Time Adjustment**: Interactive lighting parameter modification
 * - **Color Temperature**: Realistic lighting with temperature-based color control
 * 
 * ### Reference Grid System
 * - **Multiple Grid Styles**: Maya, Blender, Studio, and Technical presets
 * - **Adaptive Detail**: Automatic grid resolution based on camera distance
 * - **Measurement Support**: Precise unit-based spacing for technical modeling
 * - **Custom Configuration**: Full control over colors, spacing, and appearance
 * 
 * ## Transform and Selection
 * The viewport provides sophisticated object manipulation capabilities:
 * 
 * ### Selection Modes
 * - **Object Selection**: Click-to-select individual objects with highlight feedback
 * - **Multi-Selection**: Extended selection with modifier keys (Ctrl/Shift)
 * - **Box Selection**: Drag-to-select multiple objects within rectangular regions
 * - **Component Selection**: Vertex, edge, and face selection for detailed editing
 * 
 * ### Transform Gizmos
 * - **Translation Gizmo**: Visual handles for precise object positioning
 * - **Rotation Gizmo**: Circular controls for object rotation around axes
 * - **Scale Gizmo**: Size adjustment handles with uniform and non-uniform scaling
 * - **Snapping Support**: Grid-based and object-based snapping for precise alignment
 * 
 * ## Performance Optimization
 * The viewport is optimized for real-time professional workflows:
 * 
 * ### Rendering Efficiency
 * - **Frustum Culling**: Objects outside view are automatically excluded
 * - **Level of Detail**: Distance-based geometry simplification for complex scenes
 * - **State Caching**: Minimal OpenGL state changes for maximum performance
 * - **Batch Rendering**: Grouped draw calls for similar geometry types
 * 
 * ### Memory Management
 * - **Resource Pooling**: Efficient management of OpenGL resources
 * - **Texture Streaming**: On-demand loading of high-resolution textures
 * - **Geometry Caching**: Optimized vertex buffer management
 * - **Smart Updates**: Only changed elements are re-rendered
 * 
 * ## User Interaction Workflow
 * The viewport provides intuitive interaction patterns familiar to 3D artists:
 * 
 * ### Mouse Controls
 * 1. **Left Click**: Object selection and tool activation
 * 2. **Middle Mouse**: Camera orbit navigation (with drag)
 * 3. **Right Click**: Context menu access for object operations
 * 4. **Scroll Wheel**: Zoom in/out with configurable sensitivity
 * 5. **Shift+Middle**: Pan camera laterally across the scene
 * 
 * ### Keyboard Shortcuts
 * - **Space**: Reset camera to home position for scene overview
 * - **F**: Frame selected objects or entire scene if nothing selected
 * - **G/R/S**: Activate grab (translate), rotate, or scale transform modes
 * - **Tab**: Toggle between object and edit modes for modeling
 * - **Ctrl+A**: Select all objects in the current scene
 * 
 * ## OpenGL Integration
 * The viewport manages the complete OpenGL rendering context:
 * 
 * ### Context Management
 * - **Multi-Sample Anti-Aliasing**: High-quality edge smoothing for professional output
 * - **Depth Testing**: Accurate 3D depth sorting for complex scenes
 * - **Alpha Blending**: Transparent material support with proper depth ordering
 * - **Error Handling**: Comprehensive OpenGL error detection and reporting
 * 
 * ### Shader Pipeline
 * - **Modern GLSL**: Vertex and fragment shaders optimized for 3D modeling
 * - **Uniform Management**: Efficient parameter passing to GPU programs
 * - **Multi-Pass Rendering**: Support for advanced lighting and post-processing
 * - **Debug Visualization**: Special shaders for wireframe and normal display
 * 
 * ## Event System Integration
 * The viewport participates in the application's event-driven architecture:
 * 
 * ### Signals and Slots
 * - **objectSelected()**: Emitted when user selects objects for external tools
 * - **transformModeChanged()**: Notifies UI when transform mode switches
 * - **cameraChanged()**: Updates dependent systems when camera moves
 * - **selectionChanged()**: Synchronizes selection state across application
 * 
 * ### External Communication
 * - **Property Panels**: Selection changes update object property displays
 * - **Outliner Sync**: Selected objects are highlighted in scene hierarchy
 * - **Tool Activation**: Selection state enables/disables appropriate tools
 * - **Status Updates**: Viewport statistics displayed in status bar
 * 
 * ## Threading and Performance
 * The viewport is designed for main-thread operation with performance safeguards:
 * 
 * ### Main Thread Operations
 * - All OpenGL operations occur on the main UI thread for stability
 * - Scene updates are synchronized to prevent rendering corruption
 * - User input is processed immediately for responsive interaction
 * 
 * ### Background Processing
 * - Geometry processing can occur on worker threads when possible
 * - Texture loading and processing utilizes background threads
 * - Scene graph updates are batched for efficient main thread application
 * 
 * ## Extensibility Framework
 * The viewport provides extension points for custom functionality:
 * 
 * ### Custom Render Modes
 * - Plugin architecture for additional visualization modes
 * - Custom shader integration for specialized rendering effects
 * - Post-processing pipeline for advanced visual effects
 * 
 * ### Tool Integration
 * - Custom tool support through standardized interaction interfaces
 * - Gizmo system extensible for specialized manipulation tools
 * - Context menu system supports tool-specific options
 * 
 * ## Usage Examples
 * ```cpp
 * // Create and configure 3D viewport
 * auto viewport = std::make_unique<Viewport3D>(parent);
 * 
 * // Set up scene and systems
 * viewport->setScene(sceneManager->getCurrentScene());
 * viewport->setLightingSystem(lightingSystem);
 * viewport->setGridSystem(gridSystem);
 * 
 * // Configure rendering
 * viewport->setRenderMode(RenderMode::Solid);
 * viewport->setShowGrid(true);
 * 
 * // Connect to application events
 * connect(viewport.get(), &Viewport3D::objectSelected,
 *         propertyPanel, &PropertyPanel::showObjectProperties);
 * 
 * // Enable professional camera controls
 * auto cameraController = std::make_shared<OrbitCameraController>();
 * viewport->setCameraController(cameraController);
 * ```
 * 
 * ## Professional Workflow Integration
 * The viewport integrates seamlessly with professional 3D modeling workflows:
 * - **Maya/Blender Compatibility**: Familiar navigation and interaction patterns
 * - **Industry Standards**: Lighting and grid presets matching professional software
 * - **Efficiency Focus**: Optimized for rapid iteration and real-time feedback
 * - **Precision Tools**: Accurate measurement and alignment capabilities
 * 
 * @see Camera for view and projection management
 * @see LightingSystem for professional illumination control
 * @see GridSystem for reference grid functionality
 * @see RenderSystem for rendering pipeline coordination
 * @see SelectionManager for object selection and manipulation
 * 
 * @ingroup UI
 * @ingroup Rendering
 */
class Viewport3D : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct 3D viewport widget with Qt parent
     * 
     * Creates a new Viewport3D widget with complete 3D rendering capabilities,
     * professional camera controls, and integrated lighting and grid systems.
     * The viewport is ready for immediate use in professional modeling workflows.
     * 
     * @param parent Qt parent widget for memory management
     * 
     * @post Viewport is created with default camera, lighting, and grid systems
     * @post OpenGL context is configured for modern 3D rendering
     * @post Render mode is set to Solid with transform gizmos enabled
     * @post Professional lighting preset (Studio) is applied
     * 
     * @note OpenGL context creation may fail on systems without proper drivers
     * @note Viewport requires OpenGL 3.3 Core Profile or higher
     */
    explicit Viewport3D(QWidget* parent = nullptr);
    
    /**
     * @brief Destructor with automatic resource cleanup
     * 
     * Properly cleans up OpenGL resources, disconnects from Qt object
     * hierarchy, and releases all managed subsystems for memory safety.
     */
    ~Viewport3D();

    // ========================================================================
    // Scene and Camera Management
    // ========================================================================
    
    /**
     * @brief Assign scene for 3D visualization
     * 
     * Sets the 3D scene that contains all objects, lighting, and materials
     * to be rendered in this viewport. The scene provides the complete
     * 3D world state for visualization and interaction.
     * 
     * @param scene Shared pointer to the scene to visualize
     * 
     * @post Scene is assigned and becomes visible in viewport
     * @post Previous scene reference is released
     * @post Viewport automatically frames the new scene content
     * 
     * @note Passing nullptr clears the viewport (empty scene)
     * @note Scene changes take effect immediately
     */
    void setScene(std::shared_ptr<rude::Scene> scene);
    
    /**
     * @brief Get currently assigned scene
     * 
     * @return Shared pointer to current scene, or nullptr if none assigned
     * 
     * @note Use for external scene manipulation and synchronization
     */
    std::shared_ptr<rude::Scene> getScene() const { return m_scene; }
    
    /**
     * @brief Get viewport camera for external control
     * 
     * @return Shared pointer to the viewport camera
     * 
     * @note Camera provides view and projection matrices for rendering
     * @note Use for external camera control and synchronization
     */
    std::shared_ptr<Camera> getCamera() const { return m_camera; }

    // ========================================================================
    // Rendering Configuration
    // ========================================================================
    
    /**
     * @brief Set primary visualization render mode
     * 
     * Controls how 3D geometry is displayed in the viewport.
     * Different modes optimize for different modeling and analysis workflows.
     * 
     * @param mode New render mode to apply
     * 
     * @post Render mode is updated and takes effect immediately
     * @post Viewport re-renders with new visualization style
     * 
     * ## Available Modes
     * - **Solid**: Full surface rendering with lighting and materials
     * - **Wireframe**: Edge-only display for topology analysis
     * - **SolidWireframe**: Combined view showing surfaces and edges
     * - **Points**: Vertex-only display for detailed mesh inspection
     * 
     * @note Mode changes affect rendering performance differently
     * @note Wireframe modes are optimized for interactive feedback
     */
    void setRenderMode(RenderMode mode);
    
    /**
     * @brief Get current render mode
     * 
     * @return Currently active render mode
     */
    RenderMode getRenderMode() const { return m_renderMode; }
    
    /**
     * @brief Set active object transformation mode
     * 
     * Controls which type of transformation gizmo is displayed for
     * selected objects, enabling different manipulation workflows.
     * 
     * @param mode New transform mode to activate
     * 
     * @post Transform mode is updated and gizmos change appearance
     * @post transformModeChanged() signal is emitted for UI synchronization
     * 
     * ## Available Modes
     * - **Translate**: Position adjustment with directional handles
     * - **Rotate**: Orientation adjustment with circular controls
     * - **Scale**: Size adjustment with scaling handles
     * - **None**: No transform gizmo displayed
     * 
     * @note Transform mode affects mouse interaction behavior
     * @note Gizmo visibility depends on object selection state
     */
    void setTransformMode(TransformMode mode);
    
    /**
     * @brief Get current transform mode
     * 
     * @return Currently active transform mode
     */
    TransformMode getTransformMode() const { return m_transformMode; }

    // ========================================================================
    // System Integration and Controllers
    // ========================================================================
    
    /**
     * @brief Assign camera controller for navigation
     * 
     * Sets the camera controller that handles user input for camera
     * navigation including orbit, pan, zoom, and view presets.
     * 
     * @param controller Shared pointer to camera controller
     * 
     * @post Camera controller is assigned and handles navigation input
     * @post Previous controller reference is released
     * 
     * @note Different controllers provide different navigation styles
     * @note OrbitController is recommended for general 3D modeling
     * @note FlyController is suited for architectural walkthroughs
     */
    void setCameraController(std::shared_ptr<ICameraController> controller);
    
    /**
     * @brief Assign input controller for tool interaction
     * 
     * Sets the input controller that manages mouse and keyboard input
     * for object selection, tool activation, and viewport interaction.
     * 
     * @param controller Shared pointer to input controller
     * 
     * @post Input controller handles user interaction events
     * @post Previous controller reference is released
     * 
     * @note Input controller determines selection and tool behavior
     * @note Controller provides context-sensitive interaction modes
     */
    void setInputController(std::shared_ptr<InputController> controller);
    
    /**
     * @brief Assign lighting system for scene illumination
     * 
     * Sets the lighting system that manages scene illumination including
     * three-point lighting setups, presets, and real-time adjustments.
     * 
     * @param lightingSystem Shared pointer to lighting system
     * 
     * @post Lighting system provides scene illumination
     * @post Professional lighting presets become available
     * @post Previous lighting system reference is released
     * 
     * @note Lighting system affects rendering quality and performance
     * @note Real-time lighting changes are immediately visible
     */
    void setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem);
    
    /**
     * @brief Assign grid system for reference overlays
     * 
     * Sets the grid system that manages reference grids, measurement
     * guides, and construction aids for precise modeling workflows.
     * 
     * @param gridSystem Shared pointer to grid system
     * 
     * @post Grid system provides reference overlays in viewport
     * @post Professional grid presets become available
     * @post Previous grid system reference is released
     * 
     * @note Grid system supports adaptive detail and custom styling
     * @note Grid visibility can be toggled independently
     */
    void setGridSystem(std::shared_ptr<GridSystem> gridSystem);
    
    /**
     * @brief Get current camera controller
     * 
     * @return Shared pointer to camera controller, or nullptr if none assigned
     * 
     * @note Use for external camera control configuration
     */
    std::shared_ptr<ICameraController> getCameraController() const { return m_cameraController; }
    
    /**
     * @brief Get current input controller
     * 
     * @return Shared pointer to input controller, or nullptr if none assigned
     * 
     * @note Use for external input handling configuration
     */
    std::shared_ptr<InputController> getInputController() const { return m_inputController; }
    
    /**
     * @brief Get current lighting system
     * 
     * @return Shared pointer to lighting system, or nullptr if none assigned
     * 
     * @note Use for external lighting configuration and real-time adjustment
     */
    std::shared_ptr<LightingSystem> getLightingSystem() const { return m_lightingSystem; }
    
    /**
     * @brief Get current grid system
     * 
     * @return Shared pointer to grid system, or nullptr if none assigned
     * 
     * @note Use for external grid configuration and visibility control
     */
    std::shared_ptr<GridSystem> getGridSystem() const { return m_gridSystem; }

    // ========================================================================
    // Selection Management
    // ========================================================================
    
    /**
     * @brief Assign selection manager for object manipulation
     * 
     * Sets the selection manager that handles object selection, multi-selection,
     * selection highlighting, and selection-based tool activation.
     * 
     * @param selectionManager Shared pointer to selection manager
     * 
     * @post Selection manager handles object selection in viewport
     * @post Selection highlighting is enabled for selected objects
     * @post Previous selection manager reference is released
     * 
     * @note Selection manager coordinates with transform gizmos
     * @note Multi-selection modes depend on manager configuration
     */
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);
    
    /**
     * @brief Get current selection manager
     * 
     * @return Shared pointer to selection manager, or nullptr if none assigned
     * 
     * @note Use for external selection state queries and manipulation
     */
    std::shared_ptr<SelectionManager> getSelectionManager() const { return m_selectionManager; }
    
    /**
     * @brief Set selection interaction mode
     * 
     * Controls what type of scene elements can be selected and how
     * selection behaves during user interaction.
     * 
     * @param type New selection type to activate
     * 
     * @post Selection type is updated and affects click behavior
     * @post Selection highlighting adapts to new selection type
     * 
     * ## Selection Types
     * - **Object**: Select entire objects for transformation
     * - **Component**: Select vertices, edges, or faces for editing
     * - **Material**: Select objects by material assignment
     * - **Group**: Select object groups for batch operations
     * 
     * @note Selection type affects available manipulation tools
     * @note Component selection requires edit mode activation
     */
    void setSelectionType(SelectionType type);
    
    /**
     * @brief Get current selection type
     * 
     * @return Currently active selection type
     */
    SelectionType getSelectionType() const;

    // ========================================================================
    // Camera Navigation and View Control
    // ========================================================================
    
    /**
     * @brief Reset camera to default home position
     * 
     * Moves camera to the default home position providing an optimal
     * overview of the entire scene for general modeling workflows.
     * 
     * @post Camera position is reset to default home location
     * @post Camera orientation provides scene overview
     * @post Zoom level is adjusted to frame entire scene
     * 
     * @note Home position is optimized for general 3D modeling
     * @note Animation may be applied for smooth transition
     */
    void resetCamera();
    
    /**
     * @brief Frame entire scene in viewport
     * 
     * Automatically positions and zooms camera to show the entire scene
     * content with appropriate padding for optimal visualization.
     * 
     * @post Camera position frames all scene objects
     * @post Zoom level shows entire scene with comfortable padding
     * @post Camera orientation is optimized for scene content
     * 
     * @note Framing calculates optimal viewing distance automatically
     * @note Empty scenes default to standard modeling distance
     */
    void frameScene();
    
    /**
     * @brief Frame currently selected objects
     * 
     * Automatically positions and zooms camera to optimally frame
     * the currently selected objects for detailed inspection or editing.
     * 
     * @post Camera frames selected objects with appropriate zoom level
     * @post Camera position provides optimal viewing angle
     * @post Unselected objects may be visible but not focal
     * 
     * @note Requires active selection for operation
     * @note Falls back to scene framing if nothing selected
     * @note Multiple selected objects are framed as a group
     */
    void frameSelectedObject();

    // ========================================================================
    // Grid and Visual Aids
    // ========================================================================
    
    /**
     * @brief Toggle reference grid visibility
     * 
     * Controls whether the construction grid is visible in the viewport
     * for spatial reference and measurement during modeling workflows.
     * 
     * @param show true to show grid, false to hide
     * 
     * @post Grid visibility is updated in viewport
     * @post Grid rendering is enabled/disabled for performance
     * 
     * @note Grid visibility is independent of grid system assignment
     * @note Hidden grid still provides snapping functionality if enabled
     * @note Grid settings are managed by assigned GridSystem
     */
    void setShowGrid(bool show);
    
    /**
     * @brief Check if reference grid is currently visible
     * 
     * @return true if grid is visible, false if hidden
     * 
     * @note Grid visibility state is managed independently of grid system
     */
    bool isGridVisible() const;

signals:
    // ========================================================================
    // UI Communication Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when user selects objects in viewport
     * 
     * Emitted whenever the user clicks to select objects in the 3D viewport,
     * allowing external systems to respond to selection changes for property
     * display, tool activation, and UI synchronization.
     * 
     * @param object Shared pointer to the selected scene object
     * 
     * @note Emitted for both single and multi-selection operations
     * @note Connect to property panels for automatic object details display
     * @note Selection state is also available through SelectionManager
     */
    void objectSelected(SceneObjectPtr object);
    
    /**
     * @brief Signal emitted when transform mode changes
     * 
     * Emitted when the active transformation mode changes (translate, rotate,
     * scale), allowing UI elements to update tool buttons and displays.
     * 
     * @param mode New transform mode that has been activated
     * 
     * @note Connect to toolbar buttons for visual state synchronization
     * @note Transform mode affects gizmo appearance and mouse behavior
     */
    void transformModeChanged(TransformMode mode);

protected:
    // ========================================================================
    // OpenGL Lifecycle and Rendering
    // ========================================================================
    
    /**
     * @brief Initialize OpenGL resources and rendering state
     * 
     * Called automatically by Qt when the OpenGL context is ready.
     * Sets up OpenGL state, loads shaders, initializes rendering systems,
     * and prepares the viewport for 3D rendering operations.
     * 
     * @post OpenGL context is configured for 3D rendering
     * @post Shaders are loaded and ready for use
     * @post Rendering systems are initialized
     * @post Viewport is ready for scene rendering
     * 
     * @note Called automatically by Qt framework
     * @note Failure here prevents viewport functionality
     * @note OpenGL errors are logged for debugging
     */
    void initializeGL() override;
    
    /**
     * @brief Handle viewport size changes
     * 
     * Called automatically when the widget is resized. Updates viewport
     * dimensions, camera aspect ratio, and adjusts rendering parameters
     * to maintain correct 3D visualization.
     * 
     * @param w New width of the viewport in pixels
     * @param h New height of the viewport in pixels
     * 
     * @post Viewport dimensions are updated
     * @post Camera aspect ratio is recalculated
     * @post Projection matrix is updated for new dimensions
     * 
     * @note Called automatically by Qt framework
     * @note Maintains proper aspect ratio for undistorted 3D view
     */
    void resizeGL(int w, int h) override;
    
    /**
     * @brief Render complete 3D scene frame
     * 
     * Called automatically by Qt to render the 3D scene. Performs complete
     * frame rendering including scene geometry, lighting, grid overlays,
     * transform gizmos, and selection highlighting.
     * 
     * @post Complete 3D frame is rendered to viewport
     * @post All visual elements are updated and displayed
     * 
     * @note Called automatically by Qt framework during repaints
     * @note Performance-critical method optimized for real-time rendering
     * @note Rendering order ensures correct depth and transparency
     */
    void paintGL() override;

    // ========================================================================
    // User Input Event Handling
    // ========================================================================
    
    /**
     * @brief Handle mouse button press events
     * 
     * Processes mouse button press events for object selection, camera
     * navigation initiation, and tool activation. Routes events to
     * appropriate controllers based on button and modifier keys.
     * 
     * @param event Qt mouse event containing button, position, and modifiers
     * 
     * @post Mouse interaction is initiated based on button and context
     * @post Appropriate controller receives event for processing
     * 
     * @note Left click typically handles selection and tool activation
     * @note Middle click initiates camera navigation
     * @note Right click shows context menus
     */
    void mousePressEvent(QMouseEvent* event) override;
    
    /**
     * @brief Handle mouse movement events
     * 
     * Processes mouse movement for camera navigation, object manipulation,
     * and real-time visual feedback. Coordinates with active controllers
     * to provide smooth, responsive interaction.
     * 
     * @param event Qt mouse event containing position and movement delta
     * 
     * @post Mouse movement is processed by active controller
     * @post Visual feedback is updated for ongoing operations
     * 
     * @note Camera navigation requires active mouse button press
     * @note Object manipulation uses transform gizmo interaction
     * @note Movement provides real-time preview of operations
     */
    void mouseMoveEvent(QMouseEvent* event) override;
    
    /**
     * @brief Handle mouse button release events
     * 
     * Processes mouse button release to complete interactions, finalize
     * operations, and update state. Ensures proper completion of camera
     * navigation and object manipulation operations.
     * 
     * @param event Qt mouse event containing button and final position
     * 
     * @post Active mouse interaction is completed
     * @post Final state is applied to scene or camera
     * 
     * @note Completes camera navigation movements
     * @note Finalizes object transformation operations
     * @note Updates selection state for click-to-select
     */
    void mouseReleaseEvent(QMouseEvent* event) override;
    
    /**
     * @brief Handle mouse wheel scroll events
     * 
     * Processes mouse wheel input for camera zoom control with configurable
     * sensitivity and smooth scaling behavior for precision navigation.
     * 
     * @param event Qt wheel event containing scroll delta and position
     * 
     * @post Camera zoom level is adjusted based on wheel input
     * @post Zoom maintains focus point for intuitive navigation
     * 
     * @note Positive delta zooms in, negative delta zooms out
     * @note Zoom speed is configurable through camera controller
     * @note Focus point typically follows mouse cursor position
     */
    void wheelEvent(QWheelEvent* event) override;
    
    /**
     * @brief Handle keyboard input events
     * 
     * Processes keyboard shortcuts for camera control, tool activation,
     * and viewport navigation. Provides rapid access to common operations
     * for efficient modeling workflows.
     * 
     * @param event Qt keyboard event containing key code and modifiers
     * 
     * @post Keyboard shortcut is processed and appropriate action taken
     * @post Camera, selection, or tool state may be updated
     * 
     * @note Space bar typically resets camera to home position
     * @note F key frames selected objects or entire scene
     * @note G/R/S keys activate grab/rotate/scale transform modes
     */
    void keyPressEvent(QKeyEvent* event) override;

private:
    // ========================================================================
    // Core System Components
    // ========================================================================
    
    /// Scene containing all 3D objects, lights, and materials for rendering
    std::shared_ptr<rude::Scene> m_scene;
    
    /// Camera managing view and projection transformations
    std::shared_ptr<Camera> m_camera;
    
    /// Low-level OpenGL renderer for graphics operations
    std::shared_ptr<Renderer> m_renderer;
    
    /// Camera controller handling navigation input and behavior
    std::shared_ptr<ICameraController> m_cameraController;
    
    /// Input controller managing user interaction and tool activation
    std::shared_ptr<InputController> m_inputController;
    
    /// Selection manager handling object selection and highlighting
    std::shared_ptr<SelectionManager> m_selectionManager;
    
    /// Lighting system providing professional three-point lighting
    std::shared_ptr<LightingSystem> m_lightingSystem;
    
    /// Grid system managing reference grids and construction aids
    std::shared_ptr<GridSystem> m_gridSystem;

    // ========================================================================
    // Rendering and Interaction State
    // ========================================================================
    
    /// Current primary visualization mode for scene rendering
    RenderMode m_renderMode;
    
    /// Active transformation mode for selected object manipulation
    TransformMode m_transformMode;

    // ========================================================================
    // Mouse Interaction State
    // ========================================================================
    
    /// Last recorded mouse position for delta calculation
    QPoint m_lastMousePos;
    
    /// Currently pressed mouse button for interaction context
    Qt::MouseButton m_mouseButton;
    
    bool m_isDragging;
    
    // Transform gizmo
    bool m_showTransformGizmo;
    
    // Helper methods
    void updateCamera();
    
    // Rendering helpers
    void renderScene();
    void renderTransformGizmo();
    void renderSelection(); // Selection visualization
};

/**
 * @}
 */
