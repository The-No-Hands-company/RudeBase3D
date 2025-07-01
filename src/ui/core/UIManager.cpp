#include "UIManager.h"
#include "Viewport3D.h"
#include "SceneHierarchyPanel.h"
#include "PropertiesPanel.h"
#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QActionGroup>
#include <QLabel>
#include <QApplication>
#include <QKeySequence>

UIManager::UIManager(QMainWindow* mainWindow, QObject* parent)
    : QObject(parent)
    , m_mainWindow(mainWindow)
    , m_mainSplitter(nullptr)
    , m_rightSplitter(nullptr)
    , m_viewport(nullptr)
    , m_sceneHierarchy(nullptr)
    , m_propertiesPanel(nullptr)
{
}

void UIManager::setupUI()
{
    if (!m_mainWindow) return;
    
    createActions();
    setupMenus();
    setupToolbars();
    setupStatusBar();
    setupCentralWidget();
    connectSignals();
    
    m_mainWindow->setWindowTitle("RudeBase3D - 3D Modeling Application");
    m_mainWindow->resize(1200, 800);
}

void UIManager::setupMenus()
{
    createFileMenu();
    createEditMenu();
    createCreateMenu();
    createViewMenu();
    createHelpMenu();
}

void UIManager::setupToolbars()
{
    createMainToolbar();
    createViewToolbar();
    createTransformToolbar();
}

void UIManager::setupStatusBar()
{
    auto statusBar = m_mainWindow->statusBar();
    statusBar->showMessage("Ready");
}

void UIManager::setupCentralWidget()
{
    // Create main splitter (horizontal)
    m_mainSplitter = new QSplitter(Qt::Horizontal, m_mainWindow);
    
    // Create viewport
    m_viewport = new Viewport3D(m_mainSplitter);
    
    // Create right splitter (vertical) for panels
    m_rightSplitter = new QSplitter(Qt::Vertical, m_mainSplitter);
    
    // Create panels
    m_sceneHierarchy = new SceneHierarchyPanel(m_rightSplitter);
    m_propertiesPanel = new PropertiesPanel(m_rightSplitter);
    
    // Add panels to right splitter
    m_rightSplitter->addWidget(m_sceneHierarchy);
    m_rightSplitter->addWidget(m_propertiesPanel);
    m_rightSplitter->setSizes({400, 400});
    
    // Add viewport and panels to main splitter
    m_mainSplitter->addWidget(m_viewport);
    m_mainSplitter->addWidget(m_rightSplitter);
    m_mainSplitter->setSizes({800, 300});
    
    m_mainWindow->setCentralWidget(m_mainSplitter);
}

void UIManager::createActions()
{
    // File actions
    m_newAction = new QAction("&New", this);
    m_newAction->setShortcut(QKeySequence::New);
    m_newAction->setStatusTip("Create a new scene");
    
    m_openAction = new QAction("&Open...", this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip("Open an existing scene");
    
    m_saveAction = new QAction("&Save", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip("Save the current scene");
    
    m_saveAsAction = new QAction("Save &As...", this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAsAction->setStatusTip("Save the scene with a new name");
    
    m_exportAction = new QAction("&Export...", this);
    m_exportAction->setStatusTip("Export the scene to a file");
    
    m_quitAction = new QAction("&Quit", this);
    m_quitAction->setShortcut(QKeySequence::Quit);
    m_quitAction->setStatusTip("Exit the application");
    
    // Edit actions
    m_undoAction = new QAction("&Undo", this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_undoAction->setStatusTip("Undo the last action");
    m_undoAction->setEnabled(false);
    
    m_redoAction = new QAction("&Redo", this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    m_redoAction->setStatusTip("Redo the last undone action");
    m_redoAction->setEnabled(false);
    
    m_deleteAction = new QAction("&Delete", this);
    m_deleteAction->setShortcut(QKeySequence::Delete);
    m_deleteAction->setStatusTip("Delete selected objects");
    
    m_duplicateAction = new QAction("Du&plicate", this);
    m_duplicateAction->setShortcut(QKeySequence("Ctrl+D"));
    m_duplicateAction->setStatusTip("Duplicate selected objects");
    
    m_selectAllAction = new QAction("Select &All", this);
    m_selectAllAction->setShortcut(QKeySequence::SelectAll);
    m_selectAllAction->setStatusTip("Select all objects");
    
    m_deselectAllAction = new QAction("&Deselect All", this);
    m_deselectAllAction->setShortcut(QKeySequence("Ctrl+Shift+A"));
    m_deselectAllAction->setStatusTip("Deselect all objects");
    
    // Create actions
    m_createCubeAction = new QAction("&Cube", this);
    m_createCubeAction->setStatusTip("Create a cube");
    
    m_createSphereAction = new QAction("&Sphere", this);
    m_createSphereAction->setStatusTip("Create a sphere");
    
    m_createPlaneAction = new QAction("&Plane", this);
    m_createPlaneAction->setStatusTip("Create a plane");
    
    m_createCylinderAction = new QAction("C&ylinder", this);
    m_createCylinderAction->setStatusTip("Create a cylinder");
    
    m_createConeAction = new QAction("Co&ne", this);
    m_createConeAction->setStatusTip("Create a cone");
    
    m_createTorusAction = new QAction("&Torus", this);
    m_createTorusAction->setStatusTip("Create a torus");
    
    m_createIcosphereAction = new QAction("&Icosphere", this);
    m_createIcosphereAction->setStatusTip("Create an icosphere");
    
    // View actions
    m_resetCameraAction = new QAction("&Reset Camera", this);
    m_resetCameraAction->setShortcut(QKeySequence("Home"));
    m_resetCameraAction->setStatusTip("Reset camera to default position");
    
    m_frameSceneAction = new QAction("&Frame Scene", this);
    m_frameSceneAction->setShortcut(QKeySequence("F"));
    m_frameSceneAction->setStatusTip("Frame the entire scene");
    
    m_frameSelectedAction = new QAction("Frame &Selected", this);
    m_frameSelectedAction->setShortcut(QKeySequence("Shift+F"));
    m_frameSelectedAction->setStatusTip("Frame selected objects");
    
    m_toggleGridAction = new QAction("&Grid", this);
    m_toggleGridAction->setShortcut(QKeySequence("G"));
    m_toggleGridAction->setStatusTip("Toggle grid visibility");
    m_toggleGridAction->setCheckable(true);
    m_toggleGridAction->setChecked(true);
    
    // Render mode actions
    m_wireframeAction = new QAction("&Wireframe", this);
    m_wireframeAction->setShortcut(QKeySequence("1"));
    m_wireframeAction->setStatusTip("Wireframe rendering mode");
    m_wireframeAction->setCheckable(true);
    
    m_solidAction = new QAction("&Solid", this);
    m_solidAction->setShortcut(QKeySequence("2"));
    m_solidAction->setStatusTip("Solid rendering mode");
    m_solidAction->setCheckable(true);
    m_solidAction->setChecked(true);
    
    m_solidWireframeAction = new QAction("Solid + &Wireframe", this);
    m_solidWireframeAction->setShortcut(QKeySequence("3"));
    m_solidWireframeAction->setStatusTip("Solid with wireframe overlay");
    m_solidWireframeAction->setCheckable(true);
    
    // Group render mode actions
    auto renderModeGroup = new QActionGroup(this);
    renderModeGroup->addAction(m_wireframeAction);
    renderModeGroup->addAction(m_solidAction);
    renderModeGroup->addAction(m_solidWireframeAction);
    
    // Transform mode actions
    m_selectModeAction = new QAction("&Select", this);
    m_selectModeAction->setShortcut(QKeySequence("Q"));
    m_selectModeAction->setStatusTip("Selection mode");
    m_selectModeAction->setCheckable(true);
    m_selectModeAction->setChecked(true);
    
    m_translateModeAction = new QAction("&Move", this);
    m_translateModeAction->setShortcut(QKeySequence("W"));
    m_translateModeAction->setStatusTip("Translation mode");
    m_translateModeAction->setCheckable(true);
    
    m_rotateModeAction = new QAction("&Rotate", this);
    m_rotateModeAction->setShortcut(QKeySequence("E"));
    m_rotateModeAction->setStatusTip("Rotation mode");
    m_rotateModeAction->setCheckable(true);
    
    m_scaleModeAction = new QAction("&Scale", this);
    m_scaleModeAction->setShortcut(QKeySequence("R"));
    m_scaleModeAction->setStatusTip("Scale mode");
    m_scaleModeAction->setCheckable(true);
    
    // Group transform mode actions
    auto transformModeGroup = new QActionGroup(this);
    transformModeGroup->addAction(m_selectModeAction);
    transformModeGroup->addAction(m_translateModeAction);
    transformModeGroup->addAction(m_rotateModeAction);
    transformModeGroup->addAction(m_scaleModeAction);
}

void UIManager::createFileMenu()
{
    m_fileMenu = m_mainWindow->menuBar()->addMenu("&File");
    
    m_fileMenu->addAction(m_newAction);
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exportAction);
    m_fileMenu->addSeparator();
    
    // Recent files submenu
    m_recentFilesMenu = m_fileMenu->addMenu("Recent &Files");
    for (int i = 0; i < MaxRecentFiles; ++i) {
        auto action = new QAction(this);
        action->setVisible(false);
        connect(action, &QAction::triggered, this, &UIManager::onRecentFileTriggered);
        m_recentFileActions.append(action);
        m_recentFilesMenu->addAction(action);
    }
    
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAction);
}

void UIManager::createEditMenu()
{
    m_editMenu = m_mainWindow->menuBar()->addMenu("&Edit");
    
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_deleteAction);
    m_editMenu->addAction(m_duplicateAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_selectAllAction);
    m_editMenu->addAction(m_deselectAllAction);
}

void UIManager::createCreateMenu()
{
    m_createMenu = m_mainWindow->menuBar()->addMenu("&Create");
    
    m_createMenu->addAction(m_createCubeAction);
    m_createMenu->addAction(m_createSphereAction);
    m_createMenu->addAction(m_createCylinderAction);
    m_createMenu->addAction(m_createConeAction);
    m_createMenu->addAction(m_createPlaneAction);
    m_createMenu->addSeparator();
    m_createMenu->addAction(m_createTorusAction);
    m_createMenu->addAction(m_createIcosphereAction);
}

void UIManager::createViewMenu()
{
    m_viewMenu = m_mainWindow->menuBar()->addMenu("&View");
    
    m_viewMenu->addAction(m_resetCameraAction);
    m_viewMenu->addAction(m_frameSceneAction);
    m_viewMenu->addAction(m_frameSelectedAction);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_toggleGridAction);
    m_viewMenu->addSeparator();
    
    auto renderModeMenu = m_viewMenu->addMenu("&Render Mode");
    renderModeMenu->addAction(m_wireframeAction);
    renderModeMenu->addAction(m_solidAction);
    renderModeMenu->addAction(m_solidWireframeAction);
    
    m_viewMenu->addSeparator();
    
    auto transformModeMenu = m_viewMenu->addMenu("&Transform Mode");
    transformModeMenu->addAction(m_selectModeAction);
    transformModeMenu->addAction(m_translateModeAction);
    transformModeMenu->addAction(m_rotateModeAction);
    transformModeMenu->addAction(m_scaleModeAction);
}

void UIManager::createHelpMenu()
{
    m_helpMenu = m_mainWindow->menuBar()->addMenu("&Help");
    
    auto aboutAction = new QAction("&About", this);
    aboutAction->setStatusTip("About RudeBase3D");
    m_helpMenu->addAction(aboutAction);
    
    connect(aboutAction, &QAction::triggered, [this]() {
        showStatusMessage("RudeBase3D - A modern 3D modeling application");
    });
}

void UIManager::createMainToolbar()
{
    m_mainToolBar = m_mainWindow->addToolBar("Main");
    
    m_mainToolBar->addAction(m_newAction);
    m_mainToolBar->addAction(m_openAction);
    m_mainToolBar->addAction(m_saveAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_undoAction);
    m_mainToolBar->addAction(m_redoAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_createCubeAction);
    m_mainToolBar->addAction(m_createSphereAction);
    m_mainToolBar->addAction(m_createPlaneAction);
}

void UIManager::createViewToolbar()
{
    m_viewToolBar = m_mainWindow->addToolBar("View");
    
    m_viewToolBar->addAction(m_resetCameraAction);
    m_viewToolBar->addAction(m_frameSceneAction);
    m_viewToolBar->addAction(m_frameSelectedAction);
    m_viewToolBar->addSeparator();
    m_viewToolBar->addAction(m_toggleGridAction);
    m_viewToolBar->addSeparator();
    m_viewToolBar->addAction(m_wireframeAction);
    m_viewToolBar->addAction(m_solidAction);
    m_viewToolBar->addAction(m_solidWireframeAction);
}

void UIManager::createTransformToolbar()
{
    m_transformToolBar = m_mainWindow->addToolBar("Transform");
    
    m_transformToolBar->addAction(m_selectModeAction);
    m_transformToolBar->addAction(m_translateModeAction);
    m_transformToolBar->addAction(m_rotateModeAction);
    m_transformToolBar->addAction(m_scaleModeAction);
}

void UIManager::connectSignals()
{
    // File operations
    connect(m_newAction, &QAction::triggered, this, &UIManager::newScene);
    connect(m_openAction, &QAction::triggered, this, &UIManager::openScene);
    connect(m_saveAction, &QAction::triggered, this, &UIManager::saveScene);
    connect(m_saveAsAction, &QAction::triggered, this, &UIManager::saveSceneAs);
    connect(m_exportAction, &QAction::triggered, this, &UIManager::exportScene);
    connect(m_quitAction, &QAction::triggered, this, &UIManager::quit);
    
    // Edit operations
    connect(m_undoAction, &QAction::triggered, this, &UIManager::undo);
    connect(m_redoAction, &QAction::triggered, this, &UIManager::redo);
    connect(m_deleteAction, &QAction::triggered, this, &UIManager::deleteSelected);
    connect(m_duplicateAction, &QAction::triggered, this, &UIManager::duplicateSelected);
    connect(m_selectAllAction, &QAction::triggered, this, &UIManager::selectAll);
    connect(m_deselectAllAction, &QAction::triggered, this, &UIManager::deselectAll);
    
    // Create operations
    connect(m_createCubeAction, &QAction::triggered, this, &UIManager::createCube);
    connect(m_createSphereAction, &QAction::triggered, this, &UIManager::createSphere);
    connect(m_createPlaneAction, &QAction::triggered, this, &UIManager::createPlane);
    connect(m_createCylinderAction, &QAction::triggered, this, &UIManager::createCylinder);
    connect(m_createConeAction, &QAction::triggered, this, &UIManager::createCone);
    connect(m_createTorusAction, &QAction::triggered, this, &UIManager::createTorus);
    connect(m_createIcosphereAction, &QAction::triggered, this, &UIManager::createIcosphere);
    
    // View operations
    connect(m_resetCameraAction, &QAction::triggered, this, &UIManager::resetCamera);
    connect(m_frameSceneAction, &QAction::triggered, this, &UIManager::frameScene);
    connect(m_frameSelectedAction, &QAction::triggered, this, &UIManager::frameSelected);
    connect(m_toggleGridAction, &QAction::triggered, this, &UIManager::toggleGrid);
    
    // Render mode
    connect(m_wireframeAction, &QAction::triggered, [this]() {
        emit renderModeChanged(RenderMode::Wireframe);
    });
    connect(m_solidAction, &QAction::triggered, [this]() {
        emit renderModeChanged(RenderMode::Solid);
    });
    connect(m_solidWireframeAction, &QAction::triggered, [this]() {
        emit renderModeChanged(RenderMode::SolidWireframe);
    });
    
    // Transform mode
    connect(m_selectModeAction, &QAction::triggered, [this]() {
        emit transformModeChanged(TransformMode::Select);
    });
    connect(m_translateModeAction, &QAction::triggered, [this]() {
        emit transformModeChanged(TransformMode::Translate);
    });
    connect(m_rotateModeAction, &QAction::triggered, [this]() {
        emit transformModeChanged(TransformMode::Rotate);
    });
    connect(m_scaleModeAction, &QAction::triggered, [this]() {
        emit transformModeChanged(TransformMode::Scale);
    });
}

void UIManager::updateRecentFiles(const QStringList& files)
{
    int numRecentFiles = qMin(files.size(), MaxRecentFiles);
    
    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = QString("&%1 %2").arg(i + 1).arg(files[i]);
        m_recentFileActions[i]->setText(text);
        m_recentFileActions[i]->setData(files[i]);
        m_recentFileActions[i]->setVisible(true);
    }
    
    for (int i = numRecentFiles; i < MaxRecentFiles; ++i) {
        m_recentFileActions[i]->setVisible(false);
    }
}

void UIManager::setTransformMode(TransformMode mode)
{
    switch (mode) {
        case TransformMode::Select:
            m_selectModeAction->setChecked(true);
            break;
        case TransformMode::Translate:
            m_translateModeAction->setChecked(true);
            break;
        case TransformMode::Rotate:
            m_rotateModeAction->setChecked(true);
            break;
        case TransformMode::Scale:
            m_scaleModeAction->setChecked(true);
            break;
    }
}

void UIManager::setRenderMode(RenderMode mode)
{
    switch (mode) {
        case RenderMode::Wireframe:
            m_wireframeAction->setChecked(true);
            break;
        case RenderMode::Solid:
            m_solidAction->setChecked(true);
            break;
        case RenderMode::SolidWireframe:
            m_solidWireframeAction->setChecked(true);
            break;
    }
}

void UIManager::showStatusMessage(const QString& message, int timeout)
{
    if (m_mainWindow) {
        m_mainWindow->statusBar()->showMessage(message, timeout);
    }
}

void UIManager::updateObjectCount(int count)
{
    showStatusMessage(QString("Objects: %1").arg(count));
}

void UIManager::updateSelectionInfo(const QString& info)
{
    showStatusMessage(info);
}

void UIManager::onRecentFileTriggered()
{
    auto action = qobject_cast<QAction*>(sender());
    if (action) {
        QString fileName = action->data().toString();
        // TODO: Open the recent file
        showStatusMessage(QString("Opening: %1").arg(fileName));
    }
}
