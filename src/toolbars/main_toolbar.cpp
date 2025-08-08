#include "toolbars/main_toolbar.hpp"
#include <QDebug>

MainToolbar::MainToolbar(QMainWindow* parent)
    : BaseToolbar("Main", parent)
    , m_activeViewMode("solid")
{
    qDebug() << "[MainToolbar] Constructor started";
    setObjectName(getToolbarId());  // Safe to call virtual method after construction
    // Don't call virtual methods in constructor - use initialize() instead
    qDebug() << "[MainToolbar] About to call initialize()";
    initialize();
    qDebug() << "[MainToolbar] initialize() completed";
    updateViewModeStates();
    qDebug() << "[MainToolbar] Constructor completed";
}

void MainToolbar::createActions()
{
    qDebug() << "[MainToolbar] createActions() started";
    createFileActions();
    qDebug() << "[MainToolbar] File actions created";
    addToolbarSeparator();
    createViewActions();
    qDebug() << "[MainToolbar] View actions created";
    addToolbarSeparator();
    createCameraActions();
    qDebug() << "[MainToolbar] Camera actions created";
    qDebug() << "[MainToolbar] createActions() completed";
}

void MainToolbar::setupLayout()
{
    // Layout is handled by the order of action creation
}

void MainToolbar::connectSignals()
{
    // Connect to base toolbar action signal
    connect(this, &BaseToolbar::actionTriggered,
            this, &MainToolbar::onViewModeTriggered);
    connect(this, &BaseToolbar::actionTriggered,
            this, &MainToolbar::onCameraActionTriggered);
}

void MainToolbar::createFileActions()
{
    // File operations
    QAction* newAction = createAction("new", "New", "Create a new project", true);
    QAction* openAction = createAction("open", "Open", "Open an existing project", true);
    QAction* saveAction = createAction("save", "Save", "Save current project", true);
    QAction* undoAction = createAction("undo", "Undo", "Undo last action", true);
    QAction* redoAction = createAction("redo", "Redo", "Redo last action", true);
}

void MainToolbar::createCameraActions()
{
    // Camera controls
    QAction* resetCameraAction = createAction("reset_camera", "Reset Camera", "Reset camera to default view", true);
    QAction* frameSelectedAction = createAction("frame_selected", "Frame Selected", "Frame selected objects in view", true);
    QAction* frameSceneAction = createAction("frame_scene", "Frame Scene", "Frame entire scene in view", true);
    QAction* gridAction = createAction("grid", "Grid", "Toggle viewport grid", true);
    gridAction->setCheckable(true);
    gridAction->setChecked(true);
}

void MainToolbar::createViewActions()
{
    // View modes
    QAction* solidAction = createAction("solid", "Solid", "Solid shading mode", true);
    solidAction->setCheckable(true);
    solidAction->setChecked(true);
    
    QAction* wireframeAction = createAction("wireframe", "Wireframe", "Wireframe shading mode", true);
    wireframeAction->setCheckable(true);
    
    QAction* materialAction = createAction("material", "Material", "Material preview mode", true);
    materialAction->setCheckable(true);
    
    QAction* renderedAction = createAction("rendered", "Rendered", "Rendered preview mode", true);
    renderedAction->setCheckable(true);
}

void MainToolbar::onViewModeTriggered()
{
    QString actionId = qobject_cast<QObject*>(sender())->property("actionId").toString();
    
    // Check if it's a view mode
    QStringList viewModes = {"solid", "wireframe", "material", "rendered"};
    if (viewModes.contains(actionId)) {
        m_activeViewMode = actionId;
        updateViewModeStates();
        emit viewModeChanged(actionId);
    }
}

void MainToolbar::onCameraActionTriggered()
{
    QString actionId = qobject_cast<QObject*>(sender())->property("actionId").toString();
    
    // Handle camera actions
    if (actionId == "reset_camera") {
        emit cameraResetRequested();
    } else if (actionId == "frame_selected") {
        emit frameSelectedRequested();
    } else if (actionId == "frame_scene") {
        emit frameSceneRequested();
    } else if (actionId == "grid") {
        emit gridToggleRequested();
    }
}

void MainToolbar::setActiveViewMode(const QString& mode)
{
    if (m_activeViewMode != mode) {
        m_activeViewMode = mode;
        updateViewModeStates();
        emit viewModeChanged(mode);
    }
}

void MainToolbar::updateViewModeStates()
{
    // Update view mode button states
    QStringList viewModes = {"solid", "wireframe", "material", "rendered"};
    
    for (const QString& mode : viewModes) {
        if (m_actionMap.contains(mode)) {
            QAction* action = m_actionMap[mode];
            action->setChecked(mode == m_activeViewMode);
        }
    }
}
