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
#include <memory>

// Forward declarations
class Viewport3D;
class Scene;
class SceneHierarchyPanel;
class PropertiesPanel;
class SceneManager;
class CameraController;
class InputController;
class RenderSystem;
class AssetManager;
class UIManager;
class ExtrudeTool; // Add ExtrudeTool forward declaration
class LightingSystem;
class GridSystem;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
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

private:
    // Core components
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<SceneManager> m_sceneManager;
    std::shared_ptr<CameraController> m_cameraController;
    std::shared_ptr<InputController> m_inputController;
    std::shared_ptr<RenderSystem> m_renderSystem;
    std::shared_ptr<AssetManager> m_assetManager;
    std::shared_ptr<UIManager> m_uiManager;
    std::shared_ptr<ExtrudeTool> m_extrudeTool; // Add ExtrudeTool member
    std::shared_ptr<LightingSystem> m_lightingSystem;
    std::shared_ptr<GridSystem> m_gridSystem;
    
    Viewport3D* m_viewport;
    SceneHierarchyPanel* m_hierarchyPanel;
    PropertiesPanel* m_propertiesPanel;
    
    // UI components
    QSplitter* m_centralSplitter;
    QSplitter* m_sidePanelSplitter;
    QDockWidget* m_hierarchyDock;
    QDockWidget* m_propertiesDock;
    
    // Menus
    QMenu* m_fileMenu;
    QMenu* m_editMenu; // Add Edit menu
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
    
    void setupUI();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createActions();
    void createDockWidgets();
    void connectSignals();
    
    void updateUI();
    void updateWindowTitle();
    void updateStatusBar();
    
    // Test scene creation
    void createTestScene();
    
    bool maybeSave();
    void setCurrentFile(const QString& fileName);
};
