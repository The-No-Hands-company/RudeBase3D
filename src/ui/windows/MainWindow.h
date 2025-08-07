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
 * █  FILE: MainWindow.h                                                          █
 * █  DESCRIPTION: Primary Application Window & UI Framework                     █
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
 * █  This file contains the MainWindow class which serves as the central user   █
 * █  interface providing professional modeling environment with customizable    █
 * █  layouts, dockable panels, toolbars, and integrated 3D viewport system.    █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file MainWindow.h
 * @brief Primary application window and UI framework for RudeBase3D
 * 
 * This file contains the MainWindow class which serves as the central user
 * interface for the RudeBase3D 3D modeling application. It provides a complete
 * professional modeling environment with customizable layouts, dockable panels,
 * comprehensive toolbars, and integrated 3D viewport management.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup UI
 */

#pragma once

#include "Common.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QSplitter>
#include <QActionGroup>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include <memory>

// Forward declarations
class ViewportManager;
class ViewportWidget;
class Scene;
class SceneHierarchyPanel;
class PropertiesPanel;
class SceneManager;
class RenderSystem;
class AssetManager;
class UIManager;

namespace rude {
    class Scene;
    class SceneManager;
    class PropertiesPanel;
}

namespace rude {
    class PropertiesPanel;
}
class ExtrudeTool;
class ModelingToolManager;
class EditContext;
class LightingSystem;
class GridSystem;

// New forward declarations for panel system
class OutlinerPanel;
class SelectionPanel;
class ToolbarManager;

namespace RudeBase3D {
namespace UI {
    class EditPreviewPanel;
}
}

/**
 * @addtogroup UI
 * @{
 */

/**
 * @brief Primary application window and UI framework for RudeBase3D
 * 
 * The MainWindow class serves as the central hub for the RudeBase3D 3D modeling
 * application, providing a professional-grade user interface with extensive
 * customization capabilities. It orchestrates the interaction between the 3D
 * viewport, dockable panels, toolbars, menus, and various modeling systems.
 * 
 * ## Core Architecture
 * The MainWindow follows Qt's advanced docking system to provide:
 * - **Central Widget**: Primary 3D viewport for scene visualization and editing
 * - **Dockable Panels**: Moveable, resizable panels for different workflows
 * - **Menu System**: Comprehensive menu structure for all application features
 * - **Toolbar Integration**: Quick access to frequently used tools and commands
 * - **Status Bar**: Real-time feedback and application state information
 * 
 * ## Panel Management
 * The window manages several specialized panels:
 * - **Scene Hierarchy**: Tree view of scene objects and relationships
 * - **Properties Panel**: Object properties and material editing
 * - **Outliner Panel**: Advanced scene organization and filtering
 * - **Asset Browser**: File and resource management
 * - **Tool Panels**: Context-sensitive tool options and settings
 * 
 * ## Viewport Integration
 * - **Multi-Viewport Support**: Multiple 3D views with different camera angles
 * - **Viewport Splitting**: Horizontal/vertical viewport arrangements
 * - **Rendering Systems**: OpenGL integration with real-time preview
 * - **Camera Controls**: Orbit, pan, zoom, and predefined view angles
 * 
 * ## Workflow Support
 * - **Modeling Tools**: Complete set of 3D modeling operations
 * - **Selection Systems**: Advanced object selection and manipulation
 * - **Transform Tools**: Move, rotate, scale with precision controls
 * - **Material Editor**: Visual material creation and assignment
 * - **Animation Timeline**: Keyframe animation and timeline management
 * 
 * ## Customization Features
 * - **Layout Persistence**: Save and restore custom workspace layouts
 * - **Theme System**: Light/dark themes with customizable colors
 * - **Keyboard Shortcuts**: Fully customizable hotkey system
 * - **Toolbar Configuration**: Add, remove, and reorder toolbar buttons
 * - **Panel Visibility**: Show/hide panels based on current workflow
 * 
 * ## Professional Features
 * - **Multi-Document Interface**: Support for multiple open projects
 * - **Auto-Save System**: Automatic project backup and recovery
 * - **Plugin Architecture**: Extensible through custom plugins
 * - **Import/Export**: Support for industry-standard 3D file formats
 * - **Batch Operations**: Process multiple objects or files efficiently
 * 
 * ## Performance Optimization
 * - **Lazy Loading**: Panels and tools are created only when needed
 * - **Update Batching**: UI updates are batched to prevent flickering
 * - **Memory Management**: Efficient cleanup of large scenes and assets
 * - **Background Processing**: Long operations run without blocking the UI
 * 
 * ## Usage Examples
 * ```cpp
 * // Create and initialize main window
 * MainWindow mainWindow;
 * mainWindow.show();
 * 
 * // Customize panel layout
 * mainWindow.showSceneHierarchy(true);
 * mainWindow.showPropertiesPanel(true);
 * 
 * // Access active scene
 * Scene* scene = mainWindow.getCurrentScene();
 * 
 * // Set viewport mode
 * mainWindow.setViewportMode(ViewportMode::Perspective);
 * ```
 * 
 * ## Integration Points
 * The MainWindow coordinates with major subsystems:
 * - **Scene Management**: Loads, saves, and manages 3D scenes
 * - **Rendering System**: Displays scenes in real-time 3D viewport
 * - **Tool System**: Provides context for modeling and editing tools
 * - **Asset Pipeline**: Manages textures, models, and other resources
 * - **Plugin System**: Hosts third-party extensions and tools
 * 
 * @see ViewportManager for 3D rendering and camera control
 * @see SceneManager for scene lifecycle management
 * @see UIManager for theme and layout management
 * @see ToolManager for modeling tool integration
 * 
 * @ingroup UI
 */
 * viewport uses OpenGL for high-performance 3D rendering.
 * 
 * ## Workflow Integration
 * - File operations (New, Open, Save, Import, Export)
 * - Object creation and primitive generation
 * - Transform operations (Select, Move, Rotate, Scale)
 * - Modeling tools (Extrude, Inset, Bevel, Loop Cut)
 * - Viewport controls and visualization options
 * 
 * @see ViewportManager
 * @see SceneHierarchyPanel
 * @see PropertiesPanel
 * @see ToolbarManager
 * 
 * @ingroup UI
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window
     * 
     * Initializes the complete user interface including:
     * - Menu bar and all menus
     * - Toolbar system
     * - Dockable panels
     * - Central viewport
     * - Status bar
     * - Theme application
     * 
     * @param parent Parent widget (typically nullptr for main window)
     */
    explicit MainWindow(QWidget* parent = nullptr);
    
    /**
     * @brief Destroys the main window
     * 
     * Properly cleans up all UI components, saves user preferences,
     * and ensures clean shutdown of rendering systems.
     */
    ~MainWindow();

protected:
    /**
     * @brief Handles application close events
     * 
     * Prompts user to save unsaved changes, stores window state
     * and user preferences, and performs clean shutdown.
     * 
     * @param event Close event to accept or ignore
     */
    void closeEvent(QCloseEvent* event) override;

private slots:
    // File menu
    void newScene();
    void openScene();
    void saveScene();
    void saveSceneAs();
    void exitApplication();
    
    // Create menu
    void createCube();
    void createSphere();
    void createCylinder();
    void createPlane();
    void createCone();
    void createTorus();
    void createIcosphere();
    
    // View menu
    void resetCamera();
    void frameScene();
    void frameSelection();
    void toggleGrid(bool show);
    void setWireframeMode();
    void setSolidMode();
    void setSolidWireframeMode();
    
    // Transform menu
    void setSelectMode();
    void setTranslateMode();
    void setRotateMode();
    void setScaleMode();
    
    // Edit menu tools
    void beginExtrude();
    void beginInset();
    void beginLoopCut();
    void beginSubdivision();
    void beginBevel();
    void confirmEdit();
    void cancelEdit();
    
    // Selection type menu
    void setObjectSelection();
    void setVertexSelection();
    void setEdgeSelection();
    void setFaceSelection();
    
    // Help menu
    void showAbout();
    
    // Object selection
    void onObjectSelected(SceneObjectPtr object);
    void onTransformModeChanged(TransformMode mode);
    void onViewportChanged(ViewportWidget* viewport);

    // System updates
    void updateSystems();

private:
    // Core components
    std::shared_ptr<rude::Scene> m_scene;
    std::shared_ptr<rude::SceneManager> m_sceneManager;
    std::shared_ptr<RenderSystem> m_renderSystem;
    std::shared_ptr<AssetManager> m_assetManager;
    std::shared_ptr<UIManager> m_uiManager;
    std::shared_ptr<ExtrudeTool> m_extrudeTool;
    std::shared_ptr<ModelingToolManager> m_modelingToolManager;
    std::shared_ptr<EditContext> m_editContext;
    std::shared_ptr<LightingSystem> m_lightingSystem;
    std::shared_ptr<GridSystem> m_gridSystem;
    
    // Modern panel system
    OutlinerPanel* m_outlinerPanel = nullptr;
    PropertiesPanel* m_modernPropertiesPanel = nullptr;
    SelectionPanel* m_selectionPanel = nullptr;
    RudeBase3D::UI::EditPreviewPanel* m_editPreviewPanel = nullptr;
    std::unique_ptr<ToolbarManager> m_toolbarManager;
    
    // Legacy components during transition
    ViewportManager* m_viewportManager;
    SceneHierarchyPanel* m_hierarchyPanel;
    PropertiesPanel* m_propertiesPanel;
    
    // UI components
    QSplitter* m_centralSplitter;
    QSplitter* m_sidePanelSplitter;
    QDockWidget* m_hierarchyDock;
    QDockWidget* m_propertiesDock;
    
    // Menus
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_createMenu;
    QMenu* m_viewMenu;
    QMenu* m_transformMenu;
    QMenu* m_selectionMenu;
    QMenu* m_helpMenu;
    
    // Toolbars
    QToolBar* m_mainToolBar;
    QToolBar* m_transformToolBar;
    QToolBar* m_viewToolBar;
    
    // Actions
    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
    QAction* m_exitAction;
    
    QAction* m_createCubeAction;
    QAction* m_createSphereAction;
    QAction* m_createCylinderAction;
    QAction* m_createPlaneAction;
    QAction* m_createConeAction;
    QAction* m_createTorusAction;
    QAction* m_createIcosphereAction;
    
    QAction* m_resetCameraAction;
    QAction* m_frameSceneAction;
    QAction* m_frameSelectionAction;
    QAction* m_toggleGridAction;
    
    QActionGroup* m_renderModeGroup;
    QAction* m_wireframeModeAction;
    QAction* m_solidModeAction;
    QAction* m_solidWireframeModeAction;
    
    QActionGroup* m_transformModeGroup;
    QAction* m_selectModeAction;
    QAction* m_translateModeAction;
    QAction* m_rotateModeAction;
    QAction* m_scaleModeAction;
    
    // Selection type actions
    QActionGroup* m_selectionTypeGroup;
    QAction* m_objectSelectionAction;
    QAction* m_vertexSelectionAction;
    QAction* m_edgeSelectionAction;
    QAction* m_faceSelectionAction;
    
    // Edit tool actions
    QAction* m_extrudeAction;
    QAction* m_insetAction;
    QAction* m_loopCutAction;
    QAction* m_subdivisionAction;
    QAction* m_bevelAction;
    QAction* m_confirmEditAction;
    QAction* m_cancelEditAction;
    
    QAction* m_aboutAction;
    
    // Status bar
    QLabel* m_statusLabel;
    QLabel* m_objectCountLabel;
    QLabel* m_transformModeLabel;
    
    // Current file
    QString m_currentFile;
    bool m_sceneModified;
    
    // Update system
    QTimer* m_updateTimer;
    QElapsedTimer m_frameTimer;
    float m_deltaTime = 0.0f;
    
    // Helper methods
    void createPrimitiveHelper(const QString& primitiveType);
    
    // UI Setup methods
    void setupUI();
    void setupModernPanels();
    void setupModernToolbars();
    void connectSignals();
    
    void updateUI();
    void updateWindowTitle();
    void updateStatusBar();
    
    // Test scene creation
    void createTestScene();
    
    bool maybeSave();
    void setCurrentFile(const QString& fileName);
};
