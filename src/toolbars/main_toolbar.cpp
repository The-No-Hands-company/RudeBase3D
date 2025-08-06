#include "toolbars/main_toolbar.hpp"
#include <QDebug>

MainToolbar::MainToolbar(QMainWindow* parent)
    : BaseToolbar("Main", parent)
    , m_activeSelectionTool("select")
    , m_activeTransformTool("move")
    , m_activeViewMode("solid")
{
    qDebug() << "[MainToolbar] Constructor started";
    setObjectName(getToolbarId());  // Safe to call virtual method after construction
    // Don't call virtual methods in constructor - use initialize() instead
    qDebug() << "[MainToolbar] About to call initialize()";
    initialize();
    qDebug() << "[MainToolbar] initialize() completed";
    updateToolStates();
    qDebug() << "[MainToolbar] Constructor completed";
}

void MainToolbar::createActions()
{
    qDebug() << "[MainToolbar] createActions() started";
    createSelectionActions();
    qDebug() << "[MainToolbar] Selection actions created";
    addToolbarSeparator();
    createTransformActions();
    qDebug() << "[MainToolbar] Transform actions created";
    addToolbarSeparator();
    createViewActions();
    qDebug() << "[MainToolbar] View actions created";
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
            this, &MainToolbar::onSelectionToolTriggered);
    connect(this, &BaseToolbar::actionTriggered,
            this, &MainToolbar::onTransformToolTriggered);
    connect(this, &BaseToolbar::actionTriggered,
            this, &MainToolbar::onViewModeTriggered);
}

void MainToolbar::createSelectionActions()
{
    // Selection tools
    QAction* selectAction = createAction("select", "Select", "Select objects and components", true);
    selectAction->setCheckable(true);
    selectAction->setChecked(true);
    
    QAction* boxSelectAction = createAction("box_select", "Box Select", "Select objects in a box region", true);
    boxSelectAction->setCheckable(true);
    
    QAction* lassoSelectAction = createAction("lasso_select", "Lasso Select", "Select objects with lasso", true);
    lassoSelectAction->setCheckable(true);
}

void MainToolbar::createTransformActions()
{
    // Transform tools
    QAction* moveAction = createAction("move", "Move", "Move selected objects", true);
    moveAction->setCheckable(true);
    moveAction->setChecked(true);
    
    QAction* rotateAction = createAction("rotate", "Rotate", "Rotate selected objects", true);
    rotateAction->setCheckable(true);
    
    QAction* scaleAction = createAction("scale", "Scale", "Scale selected objects", true);
    scaleAction->setCheckable(true);
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

void MainToolbar::setActiveSelectionTool(const QString& tool)
{
    if (m_activeSelectionTool != tool) {
        m_activeSelectionTool = tool;
        updateToolStates();
        emit selectionToolChanged(tool);
    }
}

void MainToolbar::setActiveTransformTool(const QString& tool)
{
    if (m_activeTransformTool != tool) {
        m_activeTransformTool = tool;
        updateToolStates();
        emit transformToolChanged(tool);
    }
}

void MainToolbar::onSelectionToolTriggered()
{
    QString actionId = qobject_cast<QObject*>(sender())->property("actionId").toString();
    
    // Check if it's a selection tool
    QStringList selectionTools = {"select", "box_select", "lasso_select"};
    if (selectionTools.contains(actionId)) {
        setActiveSelectionTool(actionId);
    }
}

void MainToolbar::onTransformToolTriggered()
{
    QString actionId = qobject_cast<QObject*>(sender())->property("actionId").toString();
    
    // Check if it's a transform tool
    QStringList transformTools = {"move", "rotate", "scale"};
    if (transformTools.contains(actionId)) {
        setActiveTransformTool(actionId);
    }
}

void MainToolbar::onViewModeTriggered()
{
    QString actionId = qobject_cast<QObject*>(sender())->property("actionId").toString();
    
    // Check if it's a view mode
    QStringList viewModes = {"solid", "wireframe", "material", "rendered"};
    if (viewModes.contains(actionId)) {
        m_activeViewMode = actionId;
        updateToolStates();
        emit viewModeChanged(actionId);
    }
}

void MainToolbar::updateToolStates()
{
    // Update selection tool states
    QStringList selectionTools = {"select", "box_select", "lasso_select"};
    for (const QString& tool : selectionTools) {
        if (m_actionMap.contains(tool)) {
            m_actionMap[tool]->setChecked(tool == m_activeSelectionTool);
        }
    }
    
    // Update transform tool states
    QStringList transformTools = {"move", "rotate", "scale"};
    for (const QString& tool : transformTools) {
        if (m_actionMap.contains(tool)) {
            m_actionMap[tool]->setChecked(tool == m_activeTransformTool);
        }
    }
    
    // Update view mode states
    QStringList viewModes = {"solid", "wireframe", "material", "rendered"};
    for (const QString& mode : viewModes) {
        if (m_actionMap.contains(mode)) {
            m_actionMap[mode]->setChecked(mode == m_activeViewMode);
        }
    }
}
