//==============================================================================
// RudeBase3D Engine
// 
// File: UIManager.h
// Description: Professional UI management system for 3D modeling application
// 
// Copyright (c) 2024 RudeBase3D Development Team
// Licensed under the MIT License. See LICENSE file in the project root.
//==============================================================================

#pragma once

#include "Common.h"
#include <QObject>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

class QMainWindow;
class QSplitter;
class QVBoxLayout;
class QHBoxLayout;
class ViewportManager;
class SceneHierarchyPanel;
class PropertiesPanel;
class SceneManager;

namespace rudebase3d {
namespace ui {
class ThemeSelector;
}
}

/**
 * @brief Professional UI management system for 3D modeling applications
 * 
 * The UIManager class provides comprehensive management of the main application
 * user interface, coordinating menus, toolbars, panels, and viewport layouts
 * to create a professional 3D modeling environment. It implements industry-standard
 * UI patterns found in Maya, Blender, and 3ds Max, providing familiar workflows
 * for professional 3D artists and technical users.
 * 
 * ## Core Features
 * - **Professional Menu System**: Complete menu structure with standard 3D modeling operations
 * - **Context-Sensitive Toolbars**: Dynamic toolbars adapting to current tool and selection
 * - **Dockable Panels**: Flexible panel layout with outliner, properties, and preview panels
 * - **Viewport Integration**: Seamless integration with multi-viewport layout system
 * - **Recent Files Management**: Professional recent files handling with persistent storage
 * - **Theme System Integration**: Coordinated theming across all UI components
 * 
 * ## UI Architecture
 * The UIManager implements a professional UI architecture:
 * 
 * ### Main Window Layout
 * - **Central Viewport Area**: Primary viewport or multi-viewport layout
 * - **Left Panel**: Scene hierarchy and outliner with object tree
 * - **Right Panel**: Properties panel for selected objects and tools
 * - **Top Menu Bar**: Standard application menus (File, Edit, Create, View, Help)
 * - **Toolbar Area**: Context-sensitive toolbars for common operations
 * - **Status Bar**: Real-time status information and progress feedback
 * 
 * ### Professional Menu Structure
 * The menu system follows industry standards:
 * 
 * #### File Menu
 * - **Scene Management**: New, Open, Save, Save As operations
 * - **Import/Export**: Support for various 3D file formats
 * - **Recent Files**: Quick access to recently opened projects
 * - **Application Control**: Preferences and quit functionality
 * 
 * #### Edit Menu
 * - **Undo/Redo**: Full undo stack with operation descriptions
 * - **Selection Operations**: Select All, Deselect, Invert Selection
 * - **Object Operations**: Delete, Duplicate, Copy, Paste
 * - **Preferences**: Application and tool settings
 * 
 * #### Create Menu
 * - **Primitive Objects**: Cube, Sphere, Plane, Cylinder, Cone, Torus
 * - **Complex Geometry**: Icosphere, Mesh primitives with customization
 * - **Lighting Objects**: Point lights, directional lights, spot lights
 * - **Camera Objects**: Perspective and orthographic cameras
 * 
 * #### View Menu
 * - **Camera Operations**: Frame Scene, Frame Selected, Reset Camera
 * - **Viewport Options**: Grid toggle, wireframe modes, shading options
 * - **Panel Visibility**: Show/hide outliner, properties, and tool panels
 * - **Layout Management**: Viewport layout switching and customization
 * 
 * ### Professional Toolbar System
 * Context-sensitive toolbars provide immediate access to common operations:
 * 
 * #### Main Toolbar
 * - **File Operations**: Quick access to new, open, save operations
 * - **Undo/Redo**: Visual undo stack with operation history
 * - **Transform Tools**: Select, Move, Rotate, Scale tool selection
 * - **Snap Settings**: Grid snap, vertex snap, edge snap controls
 * 
 * #### View Toolbar
 * - **Render Modes**: Wireframe, Solid, Solid+Wireframe, Textured
 * - **Viewport Options**: Grid visibility, gizmo display, overlay controls
 * - **Camera Controls**: Frame operations and viewport reset
 * - **Selection Modes**: Object, face, edge, vertex selection modes
 * 
 * #### Transform Toolbar
 * - **Tool Selection**: Transform mode buttons with visual feedback
 * - **Coordinate Systems**: Local, Global, View coordinate system selection
 * - **Pivot Points**: Object center, world center, 3D cursor pivot options
 * - **Snap Controls**: Increment snap values and snap mode toggles
 * 
 * ## Panel Management
 * The UIManager coordinates professional panel layouts:
 * 
 * ### Scene Hierarchy Panel
 * - **Object Tree**: Hierarchical display of scene objects and relationships
 * - **Visibility Controls**: Object visibility and rendering toggles
 * - **Selection Integration**: Two-way selection sync with viewport
 * - **Context Menus**: Right-click operations for object management
 * 
 * ### Properties Panel
 * - **Object Properties**: Transform, material, and rendering properties
 * - **Tool Settings**: Current tool parameters and options
 * - **Dynamic Content**: Panel content adapts to current selection
 * - **Real-Time Updates**: Immediate feedback for property changes
 * 
 * ### Flexible Docking
 * - **Dockable Windows**: Panels can be docked, undocked, and repositioned
 * - **Layout Persistence**: Panel arrangements saved between sessions
 * - **Multi-Monitor Support**: Panels can span multiple monitors
 * - **Custom Layouts**: User-defined panel arrangements for specific workflows
 * 
 * ## Signal Coordination
 * The UIManager provides comprehensive signal coordination:
 * 
 * ### File Operation Signals
 * - **Scene Management**: newScene(), openScene(), saveScene() signals
 * - **Import/Export**: Coordinated file import and export operations
 * - **Recent Files**: Automatic recent file menu updates
 * - **Error Handling**: User feedback for file operation failures
 * 
 * ### Edit Operation Signals
 * - **Undo System**: undo(), redo() signals with operation descriptions
 * - **Selection Management**: selectAll(), deselectAll(), deleteSelected() signals
 * - **Object Operations**: duplicateSelected() and other object operations
 * - **Command Integration**: Integration with application command system
 * 
 * ### Modeling Operation Signals
 * - **Mesh Operations**: extrudeSelected(), insetSelected(), loopCutSelected() signals
 * - **Surface Operations**: subdivideSelected(), bevelSelected() operations
 * - **Tool Coordination**: Tool-specific operation signals and parameters
 * - **Preview Integration**: Real-time operation preview coordination
 * 
 * ## Professional Integration
 * UIManager integrates with professional 3D modeling workflows:
 * 
 * ### Industry Compatibility
 * - **Maya-Style Interface**: Familiar layout and operation patterns
 * - **Blender Integration**: Support for Blender-style workflows and shortcuts
 * - **3ds Max Compatibility**: Standard 3ds Max interface conventions
 * - **CAD Integration**: Technical modeling UI patterns for engineering workflows
 * 
 * ### Theme System Integration
 * - **Professional Themes**: Dark and light themes optimized for 3D work
 * - **Consistent Styling**: Coordinated appearance across all UI elements
 * - **Accessibility**: High contrast options and colorblind-friendly palettes
 * - **Customization**: User-customizable theme parameters and colors
 * 
 * ## Performance Optimization
 * The UIManager is optimized for responsive professional workflows:
 * 
 * ### Efficient Updates
 * - **Selective Redraws**: Only affected UI elements are updated
 * - **Lazy Loading**: Panel content loaded on demand
 * - **Batch Operations**: Multiple UI updates batched for efficiency
 * - **Thread Safety**: UI updates properly coordinated across threads
 * 
 * ### Memory Management
 * - **Smart Pointers**: Automatic memory management for UI components
 * - **Resource Sharing**: Efficient sharing of icons and resources
 * - **Dynamic Loading**: UI elements created only when needed
 * - **Cleanup Coordination**: Proper resource cleanup during shutdown
 * 
 * ## Usage Examples
 * ```cpp
 * // Create UI manager for main window
 * auto uiManager = std::make_unique<UIManager>(mainWindow);
 * uiManager->setupUI();
 * 
 * // Connect to application systems
 * connect(uiManager.get(), &UIManager::newScene,
 *         sceneManager, &SceneManager::createNewScene);
 * connect(uiManager.get(), &UIManager::openScene,
 *         sceneManager, &SceneManager::openScene);
 * 
 * // Update UI state
 * uiManager->setTransformMode(TransformMode::Translate);
 * uiManager->setRenderMode(RenderMode::Solid);
 * uiManager->showStatusMessage("Ready for modeling", 3000);
 * 
 * // Access UI components
 * auto viewport = uiManager->getViewportManager();
 * auto outliner = uiManager->getSceneHierarchy();
 * auto properties = uiManager->getPropertiesPanel();
 * ```
 * 
 * ## Professional Benefits
 * - **Familiar Interface**: Industry-standard UI patterns reduce learning curve
 * - **Efficient Workflows**: Optimized tool access and operation sequences
 * - **Flexible Layouts**: Customizable interface adapts to different workflows
 * - **Professional Appearance**: Polished interface appropriate for commercial use
 * - **Accessibility**: Multiple interaction methods accommodate different user preferences
 * 
 * @see ViewportManager for viewport layout coordination
 * @see SceneHierarchyPanel for object tree management
 * @see PropertiesPanel for object property editing
 * @see ThemeManager for UI theming and appearance
 * 
 * @ingroup UI
 * @ingroup Core
 */
class UIManager : public QObject
{
    Q_OBJECT
    
public:
    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct UI manager for main window
     * 
     * Creates a new UIManager that will coordinate the user interface
     * for the specified main window, setting up professional 3D modeling
     * interface components and layouts.
     * 
     * @param mainWindow Main application window to manage
     * @param parent Qt parent object for memory management
     * 
     * @post UIManager is created and ready for setupUI() call
     * @post Main window reference is stored for UI coordination
     * @post Component creation is prepared but not yet executed
     * 
     * @note Call setupUI() after construction to create the interface
     * @note Main window must remain valid for UIManager lifetime
     */
    explicit UIManager(QMainWindow* mainWindow, QObject* parent = nullptr);

    // ========================================================================
    // UI Setup and Initialization
    // ========================================================================
    
    /**
     * @brief Set up complete user interface
     * 
     * Creates and configures all UI components including menus, toolbars,
     * panels, and viewport layout for professional 3D modeling workflows.
     * 
     * @post Complete UI is created and configured
     * @post All menus, toolbars, and panels are functional
     * @post Signal connections are established
     * @post UI is ready for user interaction
     * 
     * @note This is a comprehensive setup that creates the entire interface
     * @note Should be called once after UIManager construction
     * @note Creates all necessary UI components and connections
     */
    void setupUI();
    
    /**
     * @brief Set up application menu system
     * 
     * Creates and configures the main menu bar with standard 3D modeling
     * menus including File, Edit, Create, View, and Help menus.
     * 
     * @post Menu bar is created with all standard menus
     * @post Menu actions are created and configured
     * @post Recent files menu is initialized
     * @post Menu signals are connected to appropriate slots
     * 
     * @note Creates professional menu structure matching industry standards
     * @note Recent files functionality is integrated automatically
     */
    void setupMenus();
    
    /**
     * @brief Set up application toolbar system
     * 
     * Creates and configures context-sensitive toolbars providing quick
     * access to common 3D modeling operations and tools.
     * 
     * @post Main toolbar is created with essential operations
     * @post View toolbar is created with visualization controls
     * @post Transform toolbar is created with modeling tools
     * @post Toolbar signals are connected to appropriate slots
     * 
     * @note Toolbars adapt to current tool selection and context
     * @note Provides immediate access to frequently used operations
     */
    void setupToolbars();
    
    /**
     * @brief Set up application status bar
     * 
     * Creates and configures the status bar with real-time information
     * display for modeling operations and application state.
     * 
     * @post Status bar is created and configured
     * @post Object count display is initialized
     * @post Selection info display is ready
     * @post Progress indicators are available
     * 
     * @note Status bar provides real-time feedback during operations
     * @note Includes permanent and temporary message areas
     */
    void setupStatusBar();
    
    /**
     * @brief Set up dockable panel widgets
     * 
     * Creates and configures dockable panels including scene hierarchy,
     * properties panel, and other workflow-specific panels.
     * 
     * @post Scene hierarchy panel is created and docked
     * @post Properties panel is created and docked
     * @post Panel visibility controls are configured
     * @post Panel layouts are optimized for professional workflows
     * 
     * @note Panels can be undocked, repositioned, and customized
     * @note Layout preferences are saved between application sessions
     */
    void setupDockWidgets();

    // ========================================================================
    // Component Access and Management
    // ========================================================================
    
    /**
     * @brief Get viewport manager for multi-viewport coordination
     * 
     * @return Pointer to the viewport manager instance
     * 
     * @note Use for direct viewport manipulation and layout control
     * @note Viewport manager coordinates all viewport-related operations
     */
    ViewportManager* getViewportManager() const { return m_viewportManager; }
    
    /**
     * @brief Get scene hierarchy panel for object tree management
     * 
     * @return Pointer to the scene hierarchy panel instance
     * 
     * @note Use for scene tree manipulation and object organization
     * @note Panel provides hierarchical view of scene objects
     */
    SceneHierarchyPanel* getSceneHierarchy() const { return m_sceneHierarchy; }
    
    /**
     * @brief Get properties panel for object property editing
     * 
     * @return Pointer to the properties panel instance
     * 
     * @note Use for object property display and modification
     * @note Panel content adapts to current selection automatically
     */
    PropertiesPanel* getPropertiesPanel() const { return m_propertiesPanel; }

    // ========================================================================
    // UI State Management
    // ========================================================================
    
    /**
     * @brief Update recent files menu with new file list
     * 
     * Updates the recent files menu with the provided list of recently
     * opened files, maintaining professional recent files behavior.
     * 
     * @param files List of recent file paths to display
     * 
     * @post Recent files menu is updated with provided list
     * @post Menu actions are created for each recent file
     * @post File paths are stored for menu action handling
     * 
     * @note Maximum number of recent files is limited by MaxRecentFiles
     * @note File list is automatically persisted between sessions
     * @note Non-existent files are automatically filtered from the list
     */
    void updateRecentFiles(const QStringList& files);
    
    /**
     * @brief Set current transform mode for modeling tools
     * 
     * Updates the UI to reflect the current transform mode, updating
     * toolbar button states and tool configurations.
     * 
     * @param mode Transform mode to activate (Select, Translate, Rotate, Scale)
     * 
     * @post Transform toolbar reflects the specified mode
     * @post Only the appropriate transform mode button is pressed
     * @post Transform mode signal is emitted for tool coordination
     * 
     * @note Transform mode affects viewport tool behavior
     * @note Mode changes are reflected in all relevant UI components
     */
    void setTransformMode(TransformMode mode);
    
    /**
     * @brief Set current render mode for viewport visualization
     * 
     * Updates the UI to reflect the current render mode, updating
     * toolbar button states and viewport visualization.
     * 
     * @param mode Render mode to activate (Wireframe, Solid, etc.)
     * 
     * @post View toolbar reflects the specified render mode
     * @post Only the appropriate render mode button is pressed
     * @post Render mode signal is emitted for viewport coordination
     * 
     * @note Render mode affects viewport appearance across all viewports
     * @note Mode changes provide immediate visual feedback
     */
    void setRenderMode(RenderMode mode);

    // ========================================================================
    // Status Display and User Feedback
    // ========================================================================
    
    /**
     * @brief Display status message to user with optional timeout
     * 
     * Shows a status message in the status bar for user feedback,
     * with automatic clearing after the specified timeout.
     * 
     * @param message Text message to display to user
     * @param timeout Time in milliseconds before message is cleared (default: 2000)
     * 
     * @post Message is displayed in status bar
     * @post Message will be automatically cleared after timeout
     * @post Previous temporary messages are replaced
     * 
     * @note Use for operation feedback and user guidance
     * @note Permanent status information uses separate status bar areas
     * @note Messages are automatically truncated if too long for status bar
     */
    void showStatusMessage(const QString& message, int timeout = 2000);
    
    /**
     * @brief Update object count display in status bar
     * 
     * Updates the permanent object count display showing the number
     * of objects in the current scene.
     * 
     * @param count Current number of objects in the scene
     * 
     * @post Object count display is updated in status bar
     * @post Count is formatted for professional appearance
     * 
     * @note Provides constant awareness of scene complexity
     * @note Count updates automatically as objects are added/removed
     */
    void updateObjectCount(int count);
    
    /**
     * @brief Update selection information display in status bar
     * 
     * Updates the selection information display showing details
     * about currently selected objects or components.
     * 
     * @param info Formatted selection information string
     * 
     * @post Selection info display is updated in status bar
     * @post Information is formatted for clear user feedback
     * 
     * @note Provides immediate feedback about current selection
     * @note Info includes selection type and count details
     * @note Cleared automatically when selection becomes empty
     */
    void updateSelectionInfo(const QString& info);

signals:
    // ========================================================================
    // File Operation Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when user requests new scene creation
     * 
     * Emitted when the user selects File > New or uses the new scene
     * toolbar button to create a fresh modeling scene.
     * 
     * @note Connect to SceneManager for scene creation coordination
     * @note Triggers new scene workflow with user confirmation if needed
     * @note Current scene state may need to be saved before proceeding
     */
    void newScene();
    
    /**
     * @brief Signal emitted when user requests scene file opening
     * 
     * Emitted when the user selects File > Open or uses the open
     * toolbar button to load an existing scene file.
     * 
     * @note Connect to SceneManager for file loading coordination
     * @note Triggers file dialog and scene loading workflow
     * @note Current scene state may need to be saved before proceeding
     */
    void openScene();
    
    /**
     * @brief Signal emitted when user requests scene save operation
     * 
     * Emitted when the user selects File > Save or uses the save
     * toolbar button to save the current scene.
     * 
     * @note Connect to SceneManager for scene saving coordination
     * @note Uses current scene filename if available, otherwise prompts
     * @note Provides user feedback on save operation success/failure
     */
    void saveScene();
    
    /**
     * @brief Signal emitted when user requests save as operation
     * 
     * Emitted when the user selects File > Save As to save the
     * current scene with a new filename.
     * 
     * @note Connect to SceneManager for scene saving coordination
     * @note Always prompts for new filename regardless of current state
     * @note Updates current scene filename after successful save
     */
    void saveSceneAs();
    
    /**
     * @brief Signal emitted when user requests scene export operation
     * 
     * Emitted when the user selects File > Export to export the
     * scene to various 3D file formats.
     * 
     * @note Connect to export system for format-specific export
     * @note Triggers export dialog with format selection options
     * @note Export operation does not change current scene filename
     */
    void exportScene();
    
    /**
     * @brief Signal emitted when user requests application exit
     * 
     * Emitted when the user selects File > Quit or closes the
     * main window to exit the application.
     * 
     * @note Connect to main application for controlled shutdown
     * @note Triggers save confirmation if scene has unsaved changes
     * @note Allows cleanup operations before application termination
     */
    void quit();

    // ========================================================================
    // Edit Operation Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when user requests undo operation
     * 
     * Emitted when the user selects Edit > Undo or uses Ctrl+Z
     * to undo the last modeling operation.
     * 
     * @note Connect to command system for undo stack coordination
     * @note Provides user feedback about undone operation
     * @note Updates UI state to reflect post-undo state
     */
    void undo();
    
    /**
     * @brief Signal emitted when user requests redo operation
     * 
     * Emitted when the user selects Edit > Redo or uses Ctrl+Y
     * to redo the last undone operation.
     * 
     * @note Connect to command system for redo stack coordination
     * @note Provides user feedback about redone operation
     * @note Updates UI state to reflect post-redo state
     */
    void redo();
    
    /**
     * @brief Signal emitted when user requests delete selected objects
     * 
     * Emitted when the user presses Delete key or selects Edit > Delete
     * to remove currently selected objects from the scene.
     * 
     * @note Connect to scene manager for object deletion
     * @note Operation is undoable through the command system
     * @note Updates selection state after deletion
     */
    void deleteSelected();
    
    /**
     * @brief Signal emitted when user requests duplicate selected objects
     * 
     * Emitted when the user selects Edit > Duplicate or uses Shift+D
     * to create copies of currently selected objects.
     * 
     * @note Connect to scene manager for object duplication
     * @note New objects are typically positioned with slight offset
     * @note Duplicated objects become the new selection
     */
    void duplicateSelected();
    
    /**
     * @brief Signal emitted when user requests select all operation
     * 
     * Emitted when the user selects Edit > Select All or uses Ctrl+A
     * to select all objects in the current scene.
     * 
     * @note Connect to selection manager for all-object selection
     * @note Selection includes all visible and selectable objects
     * @note Updates UI to reflect expanded selection state
     */
    void selectAll();
    
    /**
     * @brief Signal emitted when user requests deselect all operation
     * 
     * Emitted when the user selects Edit > Deselect All or uses Alt+A
     * to clear the current object selection.
     * 
     * @note Connect to selection manager for selection clearing
     * @note Removes all objects from current selection
     * @note Updates UI to reflect empty selection state
     */
    void deselectAll();

    // ========================================================================
    // Modeling Operation Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when user requests extrude operation
     * 
     * Emitted when the user selects modeling tool > Extrude to
     * extrude selected faces or edges in the mesh.
     * 
     * @note Connect to modeling system for extrude operation
     * @note Operation typically enters interactive mode for user guidance
     * @note Works on selected faces, edges, or vertices depending on selection mode
     */
    void extrudeSelected();
    
    /**
     * @brief Signal emitted when user requests inset operation
     * 
     * Emitted when the user selects modeling tool > Inset to
     * create inset faces within selected face regions.
     * 
     * @note Connect to modeling system for inset operation
     * @note Operation creates new faces within existing face boundaries
     * @note Provides interactive control over inset distance and properties
     */
    void insetSelected();
    
    /**
     * @brief Signal emitted when user requests loop cut operation
     * 
     * Emitted when the user selects modeling tool > Loop Cut to
     * create edge loops across selected mesh geometry.
     * 
     * @note Connect to modeling system for loop cut operation
     * @note Operation provides interactive preview and positioning
     * @note Creates new edge loops for enhanced topology control
     */
    void loopCutSelected();
    
    /**
     * @brief Signal emitted when user requests subdivide operation
     * 
     * Emitted when the user selects modeling tool > Subdivide to
     * increase mesh resolution through subdivision algorithms.
     * 
     * @note Connect to modeling system for subdivision operation
     * @note Multiple subdivision algorithms may be available
     * @note Operation increases mesh complexity for detail modeling
     */
    void subdivideSelected();
    
    /**
     * @brief Signal emitted when user requests bevel operation
     * 
     * Emitted when the user selects modeling tool > Bevel to
     * create beveled edges or corners on selected geometry.
     * 
     * @note Connect to modeling system for bevel operation
     * @note Operation provides interactive control over bevel parameters
     * @note Creates smooth transitions between adjacent faces
     */
    void bevelSelected();

    // ========================================================================
    // Object Creation Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when user requests cube primitive creation
     * 
     * Emitted when the user selects Create > Cube to add a
     * new cube primitive to the scene.
     * 
     * @note Connect to primitive creation system
     * @note Cube is created at scene origin or 3D cursor position
     * @note New cube becomes the current selection
     */
    void createCube();
    
    /**
     * @brief Signal emitted when user requests sphere primitive creation
     * 
     * Emitted when the user selects Create > Sphere to add a
     * new sphere primitive to the scene.
     * 
     * @note Connect to primitive creation system
     * @note Sphere is created with default subdivision parameters
     * @note New sphere becomes the current selection
     */
    void createSphere();
    
    /**
     * @brief Signal emitted when user requests plane primitive creation
     * 
     * Emitted when the user selects Create > Plane to add a
     * new plane primitive to the scene.
     * 
     * @note Connect to primitive creation system
     * @note Plane is created in XY plane at scene origin
     * @note New plane becomes the current selection
     */
    void createPlane();
    
    /**
     * @brief Signal emitted when user requests cylinder primitive creation
     * 
     * Emitted when the user selects Create > Cylinder to add a
     * new cylinder primitive to the scene.
     * 
     * @note Connect to primitive creation system
     * @note Cylinder is created with default radius and height
     * @note New cylinder becomes the current selection
     */
    void createCylinder();
    
    /**
     * @brief Signal emitted when user requests cone primitive creation
     * 
     * Emitted when the user selects Create > Cone to add a
     * new cone primitive to the scene.
     * 
     * @note Connect to primitive creation system
     * @note Cone is created with default base radius and height
     * @note New cone becomes the current selection
     */
    void createCone();
    
    /**
     * @brief Signal emitted when user requests torus primitive creation
     * 
     * Emitted when the user selects Create > Torus to add a
     * new torus primitive to the scene.
     * 
     * @note Connect to primitive creation system
     * @note Torus is created with default major and minor radii
     * @note New torus becomes the current selection
     */
    void createTorus();
    
    /**
     * @brief Signal emitted when user requests icosphere primitive creation
     * 
     * Emitted when the user selects Create > Icosphere to add a
     * new icospheric primitive to the scene.
     * 
     * @note Connect to primitive creation system
     * @note Icosphere provides more uniform triangulation than UV sphere
     * @note New icosphere becomes the current selection
     */
    void createIcosphere();

    // ========================================================================
    // View Operation Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when user requests camera reset
     * 
     * Emitted when the user selects View > Reset Camera to
     * return the viewport camera to its default position.
     * 
     * @note Connect to viewport manager for camera coordination
     * @note Reset position depends on current viewport type
     * @note Operation may be animated for smooth transition
     */
    void resetCamera();
    
    /**
     * @brief Signal emitted when user requests frame scene operation
     * 
     * Emitted when the user selects View > Frame Scene or presses
     * Home key to frame all scene objects in the viewport.
     * 
     * @note Connect to viewport manager for camera framing
     * @note Camera position is calculated to show entire scene
     * @note Operation may be animated for smooth transition
     */
    void frameScene();
    
    /**
     * @brief Signal emitted when user requests frame selected operation
     * 
     * Emitted when the user selects View > Frame Selected or presses
     * Period key to frame selected objects in the viewport.
     * 
     * @note Connect to viewport manager for camera framing
     * @note Camera position is calculated to show selected objects
     * @note Falls back to frame scene if no objects are selected
     */
    void frameSelected();
    
    /**
     * @brief Signal emitted when user toggles grid visibility
     * 
     * Emitted when the user selects View > Toggle Grid to
     * show or hide the reference grid in viewports.
     * 
     * @note Connect to grid system for visibility control
     * @note Grid visibility state is toggled from current state
     * @note All viewports are affected by grid visibility changes
     */
    void toggleGrid();
    
    /**
     * @brief Signal emitted when user toggles wireframe display
     * 
     * Emitted when the user selects View > Toggle Wireframe to
     * switch between solid and wireframe rendering modes.
     * 
     * @note Connect to viewport manager for render mode control
     * @note Wireframe mode shows mesh topology clearly
     * @note Toggle switches between wireframe and previous solid mode
     */
    void toggleWireframe();

    // ========================================================================
    // Transform Mode and Rendering Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when transform mode is changed
     * 
     * Emitted when the user changes the current transform tool
     * through toolbar buttons or keyboard shortcuts.
     * 
     * @param mode New transform mode (Select, Translate, Rotate, Scale)
     * 
     * @note Connect to tool system for transform mode coordination
     * @note Mode change affects viewport tool behavior and gizmo display
     * @note Keyboard shortcuts provide quick mode switching
     */
    void transformModeChanged(TransformMode mode);
    
    /**
     * @brief Signal emitted when render mode is changed
     * 
     * Emitted when the user changes the viewport rendering mode
     * through toolbar buttons or menu selections.
     * 
     * @param mode New render mode (Wireframe, Solid, Textured, etc.)
     * 
     * @note Connect to viewport manager for render mode coordination
     * @note Mode change affects all viewports unless overridden individually
     * @note Render mode affects performance and visual clarity
     */
    void renderModeChanged(RenderMode mode);

    // ========================================================================
    // Panel Visibility Control Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when outliner panel visibility changes
     * 
     * Emitted when the user toggles the scene hierarchy/outliner
     * panel visibility through the View menu.
     * 
     * @param show true to show panel, false to hide panel
     * 
     * @note Connect to dock widget system for panel management
     * @note Panel state is saved for next application session
     * @note Hidden panels can be restored through View menu
     */
    void showOutlinerPanel(bool show);
    
    /**
     * @brief Signal emitted when properties panel visibility changes
     * 
     * Emitted when the user toggles the properties panel
     * visibility through the View menu.
     * 
     * @param show true to show panel, false to hide panel
     * 
     * @note Connect to dock widget system for panel management
     * @note Panel state is saved for next application session
     * @note Properties panel shows details for selected objects
     */
    void showPropertiesPanel(bool show);
    
    /**
     * @brief Signal emitted when selection panel visibility changes
     * 
     * Emitted when the user toggles the selection tools panel
     * visibility through the View menu.
     * 
     * @param show true to show panel, false to hide panel
     * 
     * @note Connect to dock widget system for panel management
     * @note Selection panel provides advanced selection tools
     * @note Panel can be docked or floating as needed
     */
    void showSelectionPanel(bool show);
    
    /**
     * @brief Signal emitted when edit preview panel visibility changes
     * 
     * Emitted when the user toggles the edit operation preview
     * panel visibility through the View menu.
     * 
     * @param show true to show panel, false to hide panel
     * 
     * @note Connect to dock widget system for panel management
     * @note Preview panel shows real-time operation previews
     * @note Useful for complex modeling operations with parameters
     */
    void showEditPreviewPanel(bool show);

private slots:
    // ========================================================================
    // Internal Event Handlers
    // ========================================================================
    
    /**
     * @brief Handle recent file menu action triggered
     * 
     * Internal slot called when a recent file menu item is activated,
     * coordinating the file opening process.
     * 
     * @post Recent file opening process is initiated
     * @post Current scene save confirmation is handled if needed
     * @post File loading feedback is provided to user
     * 
     * @note Connected to all recent file menu actions automatically
     * @note Handles non-existent files gracefully with user notification
     */
    void onRecentFileTriggered();

private:
    // ========================================================================
    // Internal UI Creation Methods
    // ========================================================================
    
    /**
     * @brief Create all UI actions for menus and toolbars
     * 
     * Internal method that creates QAction objects for all menu items
     * and toolbar buttons with appropriate icons, shortcuts, and tooltips.
     * 
     * @post All UI actions are created and configured
     * @post Keyboard shortcuts are assigned and functional
     * @post Action icons are loaded and applied
     * @post Tooltips are set for user guidance
     * 
     * @note Actions are shared between menus and toolbars for consistency
     * @note Shortcuts follow industry standards (Ctrl+N, Ctrl+O, etc.)
     */
    void createActions();
    
    /**
     * @brief Create File menu with standard file operations
     * 
     * Internal method that creates the File menu with New, Open, Save,
     * Export, Recent Files, and Quit operations.
     * 
     * @post File menu is created and populated with actions
     * @post Recent files submenu is created and configured
     * @post Menu separators are added for logical grouping
     * @post File operations are immediately functional
     */
    void createFileMenu();
    
    /**
     * @brief Create Edit menu with editing and selection operations
     * 
     * Internal method that creates the Edit menu with Undo, Redo,
     * selection operations, and object manipulation commands.
     * 
     * @post Edit menu is created and populated with actions
     * @post Undo/redo actions are connected to command system
     * @post Selection operations are immediately functional
     * @post Menu reflects current operation availability
     */
    void createEditMenu();
    
    /**
     * @brief Create Create menu with primitive and object creation
     * 
     * Internal method that creates the Create menu with primitive
     * objects, lights, cameras, and other scene elements.
     * 
     * @post Create menu is created and populated with actions
     * @post Primitive creation operations are immediately functional
     * @post Advanced object creation options are available
     * @post Menu is organized by object categories
     */
    void createCreateMenu();
    
    /**
     * @brief Create View menu with viewport and visualization controls
     * 
     * Internal method that creates the View menu with camera operations,
     * render modes, panel visibility, and viewport controls.
     * 
     * @post View menu is created and populated with actions
     * @post Camera operations are connected to viewport system
     * @post Panel visibility controls are functional
     * @post Render mode options are immediately available
     */
    void createViewMenu();
    
    /**
     * @brief Create Help menu with documentation and about information
     * 
     * Internal method that creates the Help menu with user documentation,
     * tutorials, and application information.
     * 
     * @post Help menu is created and populated with actions
     * @post Documentation links are functional
     * @post About dialog is accessible
     * @post Help system integration is complete
     */
    void createHelpMenu();
    
    /**
     * @brief Create main toolbar with essential operations
     * 
     * Internal method that creates the main toolbar with file operations,
     * undo/redo, and transform tool selection.
     * 
     * @post Main toolbar is created and populated with actions
     * @post Essential operations are immediately accessible
     * @post Toolbar layout is optimized for common workflows
     * @post Icon consistency is maintained throughout
     */
    void createMainToolbar();
    
    /**
     * @brief Create view toolbar with visualization controls
     * 
     * Internal method that creates the view toolbar with render modes,
     * viewport options, and camera controls.
     * 
     * @post View toolbar is created and populated with actions
     * @post Render mode buttons provide immediate visual feedback
     * @post Viewport controls are organized logically
     * @post Toggle buttons show current state clearly
     */
    void createViewToolbar();
    
    /**
     * @brief Create transform toolbar with modeling tools
     * 
     * Internal method that creates the transform toolbar with tool
     * selection, coordinate systems, and snap settings.
     * 
     * @post Transform toolbar is created and populated with actions
     * @post Tool selection buttons are mutually exclusive
     * @post Coordinate system selection is immediately functional
     * @post Snap settings provide real-time feedback
     */
    void createTransformToolbar();
    
    /**
     * @brief Set up central widget with viewport and panel layout
     * 
     * Internal method that creates the main window's central widget
     * with viewport manager and dockable panels.
     * 
     * @post Central widget layout is created and configured
     * @post Viewport manager is embedded and functional
     * @post Panel docking areas are established
     * @post Layout proportions are optimized for 3D modeling
     */
    void setupCentralWidget();
    
    /**
     * @brief Connect all UI signals to appropriate slots
     * 
     * Internal method that establishes signal-slot connections between
     * UI actions and their corresponding functionality.
     * 
     * @post All UI actions are connected to appropriate signals
     * @post Menu and toolbar interactions are fully functional
     * @post Signal routing is established for external coordination
     * @post Connection integrity is verified for all critical operations
     */
    void connectSignals();

    // ========================================================================
    // UI Component Storage
    // ========================================================================
    
    /// Main window reference for UI coordination
    QMainWindow* m_mainWindow;
    
    // Central widget and layout components
    /// Primary splitter for main layout organization
    QSplitter* m_mainSplitter;
    
    /// Right-side splitter for panel organization
    QSplitter* m_rightSplitter;
    
    /// Viewport manager for 3D visualization
    ViewportManager* m_viewportManager;
    
    /// Scene hierarchy panel for object tree management
    SceneHierarchyPanel* m_sceneHierarchy;
    
    /// Properties panel for object editing
    PropertiesPanel* m_propertiesPanel;
    
    // Menu system components
    /// File operations menu
    QMenu* m_fileMenu;
    
    /// Edit operations menu
    QMenu* m_editMenu;
    
    /// Object creation menu
    QMenu* m_createMenu;
    
    /// View and visualization menu
    QMenu* m_viewMenu;
    
    /// Help and documentation menu
    QMenu* m_helpMenu;
    
    /// Recent files submenu
    QMenu* m_recentFilesMenu;
    
    // Toolbar system components
    /// Main operations toolbar
    QToolBar* m_mainToolBar;
    
    /// View and rendering toolbar
    QToolBar* m_viewToolBar;
    
    
    /// Transform tools toolbar
    QToolBar* m_transformToolBar;
    
    // ========================================================================
    // File Operation Actions
    // ========================================================================
    
    /// New scene creation action
    QAction* m_newAction;
    
    /// Scene file opening action
    QAction* m_openAction;
    
    /// Scene save action
    QAction* m_saveAction;
    
    /// Scene save as action
    QAction* m_saveAsAction;
    
    /// Scene export action
    QAction* m_exportAction;
    
    /// Application quit action
    QAction* m_quitAction;
    
    // ========================================================================
    // Edit Operation Actions
    // ========================================================================
    
    /// Undo last operation action
    QAction* m_undoAction;
    
    /// Redo last undone operation action
    QAction* m_redoAction;
    
    /// Delete selected objects action
    QAction* m_deleteAction;
    
    /// Duplicate selected objects action
    QAction* m_duplicateAction;
    
    /// Select all objects action
    QAction* m_selectAllAction;
    
    /// Deselect all objects action
    QAction* m_deselectAllAction;
    
    // ========================================================================
    // Modeling Operation Actions
    // ========================================================================
    
    /// Extrude selected geometry action
    QAction* m_extrudeAction;
    
    /// Inset selected faces action
    QAction* m_insetAction;
    
    /// Loop cut operation action
    QAction* m_loopCutAction;
    
    /// Subdivision surface action
    QAction* m_subdivisionAction;
    
    /// Bevel edges/vertices action
    QAction* m_bevelAction;
    
    // ========================================================================
    // Primitive Creation Actions
    // ========================================================================
    
    /// Create cube primitive action
    QAction* m_createCubeAction;
    
    /// Create sphere primitive action
    QAction* m_createSphereAction;
    
    /// Create plane primitive action
    QAction* m_createPlaneAction;
    
    /// Create cylinder primitive action
    QAction* m_createCylinderAction;
    
    /// Create cone primitive action
    QAction* m_createConeAction;
    
    /// Create torus primitive action
    QAction* m_createTorusAction;
    
    /// Create icosphere primitive action
    QAction* m_createIcosphereAction;
    
    // ========================================================================
    // View and Camera Actions
    // ========================================================================
    
    /// Reset camera to default position action
    QAction* m_resetCameraAction;
    
    /// Frame entire scene in viewport action
    QAction* m_frameSceneAction;
    
    /// Frame selected objects in viewport action
    QAction* m_frameSelectedAction;
    
    /// Toggle reference grid visibility action
    QAction* m_toggleGridAction;
    
    /// Wireframe rendering mode action
    QAction* m_wireframeAction;
    
    /// Solid rendering mode action
    QAction* m_solidAction;
    
    /// Solid with wireframe overlay mode action
    QAction* m_solidWireframeAction;
    
    // ========================================================================
    // Transform Tool Actions
    // ========================================================================
    
    /// Selection tool mode action
    QAction* m_selectModeAction;
    
    /// Translation/move tool mode action
    QAction* m_translateModeAction;
    
    /// Rotation tool mode action
    QAction* m_rotateModeAction;
    
    /// Scale tool mode action
    QAction* m_scaleModeAction;
    
    // ========================================================================
    // Recent Files Management
    // ========================================================================
    
    /// List of recent file actions for dynamic menu
    QList<QAction*> m_recentFileActions;
    
    /// Maximum number of recent files to track
    static const int MaxRecentFiles = 5;
    
    // ========================================================================
    // Theme System Integration
    // ========================================================================
    
    /// Theme selector widget for appearance customization
    rudebase3d::ui::ThemeSelector* m_themeSelector;
};

#endif // UIMANAGER_H
