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

class UIManager : public QObject
{
    Q_OBJECT
    
public:
    explicit UIManager(QMainWindow* mainWindow, QObject* parent = nullptr);
    
    // UI setup
    void setupUI();
    void setupMenus();
    void setupToolbars();
    void setupStatusBar();
    void setupDockWidgets();
    
    // Component access
    ViewportManager* getViewportManager() const { return m_viewportManager; }
    SceneHierarchyPanel* getSceneHierarchy() const { return m_sceneHierarchy; }
    PropertiesPanel* getPropertiesPanel() const { return m_propertiesPanel; }
    
    // UI state management
    void updateRecentFiles(const QStringList& files);
    void setTransformMode(TransformMode mode);
    void setRenderMode(RenderMode mode);
    
    // Status updates
    void showStatusMessage(const QString& message, int timeout = 2000);
    void updateObjectCount(int count);
    void updateSelectionInfo(const QString& info);
    
signals:
    // File operations
    void newScene();
    void openScene();
    void saveScene();
    void saveSceneAs();
    void exportScene();
    void quit();
    
    // Edit operations
    void undo();
    void redo();
    void deleteSelected();
    void duplicateSelected();
    void selectAll();
    void deselectAll();
    
    // Modeling operations
    void extrudeSelected();
    void insetSelected();
    void loopCutSelected();
    void subdivideSelected();
    void bevelSelected();
    
    // Object creation
    void createCube();
    void createSphere();
    void createPlane();
    void createCylinder();
    void createCone();
    void createTorus();
    void createIcosphere();
    
    // View operations
    void resetCamera();
    void frameScene();
    void frameSelected();
    void toggleGrid();
    void toggleWireframe();
    
    // Transform operations
    void transformModeChanged(TransformMode mode);
    void renderModeChanged(RenderMode mode);
    
private slots:
    void onRecentFileTriggered();
    
private:
    void createActions();
    void createFileMenu();
    void createEditMenu();
    void createCreateMenu();
    void createViewMenu();
    void createHelpMenu();
    void createMainToolbar();
    void createViewToolbar();
    void createTransformToolbar();
    void setupCentralWidget();
    void connectSignals();
    
    QMainWindow* m_mainWindow;
    
    // Central widget and layout
    QSplitter* m_mainSplitter;
    QSplitter* m_rightSplitter;
    ViewportManager* m_viewportManager;
    SceneHierarchyPanel* m_sceneHierarchy;
    PropertiesPanel* m_propertiesPanel;
    
    // Menus
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_createMenu;
    QMenu* m_viewMenu;
    QMenu* m_helpMenu;
    QMenu* m_recentFilesMenu;
    
    // Toolbars
    QToolBar* m_mainToolBar;
    QToolBar* m_viewToolBar;
    QToolBar* m_transformToolBar;
    
    // File actions
    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
    QAction* m_exportAction;
    QAction* m_quitAction;
    
    // Edit actions
    QAction* m_undoAction;
    QAction* m_redoAction;
    QAction* m_deleteAction;
    QAction* m_duplicateAction;
    QAction* m_selectAllAction;
    QAction* m_deselectAllAction;
    
    // Modeling actions
    QAction* m_extrudeAction;
    QAction* m_insetAction;
    QAction* m_loopCutAction;
    QAction* m_subdivisionAction;
    QAction* m_bevelAction;
    
    // Create actions
    QAction* m_createCubeAction;
    QAction* m_createSphereAction;
    QAction* m_createPlaneAction;
    QAction* m_createCylinderAction;
    QAction* m_createConeAction;
    QAction* m_createTorusAction;
    QAction* m_createIcosphereAction;
    
    // View actions
    QAction* m_resetCameraAction;
    QAction* m_frameSceneAction;
    QAction* m_frameSelectedAction;
    QAction* m_toggleGridAction;
    QAction* m_wireframeAction;
    QAction* m_solidAction;
    QAction* m_solidWireframeAction;
    
    // Transform actions
    QAction* m_selectModeAction;
    QAction* m_translateModeAction;
    QAction* m_rotateModeAction;
    QAction* m_scaleModeAction;
    
    // Recent files
    QList<QAction*> m_recentFileActions;
    static const int MaxRecentFiles = 5;
    
    // Theme system
    rudebase3d::ui::ThemeSelector* m_themeSelector;
};
