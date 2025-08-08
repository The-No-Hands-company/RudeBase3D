#include "toolbars/selection_toolbar.hpp"
#include <QActionGroup>

SelectionToolbar::SelectionToolbar(QMainWindow* parent)
    : BaseToolbar("Selection", parent)
    , m_currentMode(rude::ComponentType::Entity)
    , m_currentModifier("Replace")
    , m_currentTool("Direct")
{
    qDebug() << "[SelectionToolbar] Constructor started";
    setObjectName(getToolbarId());
    qDebug() << "[SelectionToolbar] About to call initialize()";
    initialize();
    qDebug() << "[SelectionToolbar] Constructor completed";
}

void SelectionToolbar::createActions()
{
    qDebug() << "[SelectionToolbar] createActions() started";
    
    qDebug() << "[SelectionToolbar] Creating mode actions...";
    createModeActions();
    qDebug() << "[SelectionToolbar] Mode actions created";
    
    qDebug() << "[SelectionToolbar] Creating modifier actions...";
    createModifierActions();
    qDebug() << "[SelectionToolbar] Modifier actions created";
    
    qDebug() << "[SelectionToolbar] Creating tool actions...";
    createToolActions();
    qDebug() << "[SelectionToolbar] Tool actions created";
    
    qDebug() << "[SelectionToolbar] Creating operation actions...";
    createOperationActions();
    qDebug() << "[SelectionToolbar] Operation actions created";
    
    qDebug() << "[SelectionToolbar] Setting up layout...";
    setupLayout();
    qDebug() << "[SelectionToolbar] Layout setup completed";
    
    qDebug() << "[SelectionToolbar] Connecting signals...";
    connectSignals();
    qDebug() << "[SelectionToolbar] createActions() completed";
}

void SelectionToolbar::setupLayout()
{
    qDebug() << "[SelectionToolbar] setupLayout() started";
    
    // Add mode actions first
    for (auto it = m_actionMap.begin(); it != m_actionMap.end(); ++it) {
        if (it.key().startsWith("mode_") && it.value()) {
            qDebug() << "[SelectionToolbar] Adding mode action:" << it.key();
            addAction(it.value());
        }
    }
    
    addSeparator();
    
    // Add modifier actions
    for (auto it = m_actionMap.begin(); it != m_actionMap.end(); ++it) {
        if (it.key().startsWith("modifier_") && it.value()) {
            qDebug() << "[SelectionToolbar] Adding modifier action:" << it.key();
            addAction(it.value());
        }
    }
    
    addSeparator();
    
    // Add tool actions
    for (auto it = m_actionMap.begin(); it != m_actionMap.end(); ++it) {
        if (it.key().startsWith("tool_") && it.value()) {
            qDebug() << "[SelectionToolbar] Adding tool action:" << it.key();
            addAction(it.value());
        }
    }
    
    addSeparator();
    
    // Add operation actions
    for (auto it = m_actionMap.begin(); it != m_actionMap.end(); ++it) {
        if (it.key().startsWith("operation_") && it.value()) {
            qDebug() << "[SelectionToolbar] Adding operation action:" << it.key();
            addAction(it.value());
        }
    }
    
    qDebug() << "[SelectionToolbar] setupLayout() completed";
}

void SelectionToolbar::connectSignals()
{
    // Connect mode actions with null checks
    if (m_actionMap["mode_entity"])
        connect(m_actionMap["mode_entity"], &QAction::triggered, this, &SelectionToolbar::onSelectionModeChanged);
    if (m_actionMap["mode_vertex"])
        connect(m_actionMap["mode_vertex"], &QAction::triggered, this, &SelectionToolbar::onSelectionModeChanged);
    if (m_actionMap["mode_edge"])
        connect(m_actionMap["mode_edge"], &QAction::triggered, this, &SelectionToolbar::onSelectionModeChanged);
    if (m_actionMap["mode_face"])
        connect(m_actionMap["mode_face"], &QAction::triggered, this, &SelectionToolbar::onSelectionModeChanged);
    
    // Connect modifier actions with null checks
    if (m_actionMap["modifier_replace"])
        connect(m_actionMap["modifier_replace"], &QAction::triggered, this, &SelectionToolbar::onSelectionModifierChanged);
    if (m_actionMap["modifier_add"])
        connect(m_actionMap["modifier_add"], &QAction::triggered, this, &SelectionToolbar::onSelectionModifierChanged);
    if (m_actionMap["modifier_subtract"])
        connect(m_actionMap["modifier_subtract"], &QAction::triggered, this, &SelectionToolbar::onSelectionModifierChanged);
    if (m_actionMap["modifier_intersect"])
        connect(m_actionMap["modifier_intersect"], &QAction::triggered, this, &SelectionToolbar::onSelectionModifierChanged);
    
    // Connect tool actions with null checks
    if (m_actionMap["tool_direct"])
        connect(m_actionMap["tool_direct"], &QAction::triggered, this, &SelectionToolbar::onSelectionToolChanged);
    if (m_actionMap["tool_box"])
        connect(m_actionMap["tool_box"], &QAction::triggered, this, &SelectionToolbar::onSelectionToolChanged);
    if (m_actionMap["tool_lasso"])
        connect(m_actionMap["tool_lasso"], &QAction::triggered, this, &SelectionToolbar::onSelectionToolChanged);
    if (m_actionMap["tool_brush"])
        connect(m_actionMap["tool_brush"], &QAction::triggered, this, &SelectionToolbar::onSelectionToolChanged);
    
    // Connect operation actions with null checks
    if (m_actionMap["operation_select_all"])
        connect(m_actionMap["operation_select_all"], &QAction::triggered, this, &SelectionToolbar::onSelectionOperationRequested);
    if (m_actionMap["operation_deselect_all"])
        connect(m_actionMap["operation_deselect_all"], &QAction::triggered, this, &SelectionToolbar::onSelectionOperationRequested);
    if (m_actionMap["operation_invert"])
        connect(m_actionMap["operation_invert"], &QAction::triggered, this, &SelectionToolbar::onSelectionOperationRequested);
}

void SelectionToolbar::createModeActions()
{
    qDebug() << "[SelectionToolbar] createModeActions() started";
    // Entity
    auto entityAction = new QAction("Entity", this);
    entityAction->setObjectName("mode_entity");
    entityAction->setToolTip("Select entire entities");
    entityAction->setCheckable(true);
    entityAction->setChecked(true);
    m_actionMap["mode_entity"] = entityAction;
    // Vertex
    auto vertexAction = new QAction("Vertex", this);
    vertexAction->setObjectName("mode_vertex");
    vertexAction->setToolTip("Select individual vertices");
    vertexAction->setCheckable(true);
    m_actionMap["mode_vertex"] = vertexAction;
    // Edge
    auto edgeAction = new QAction("Edge", this);
    edgeAction->setObjectName("mode_edge");
    edgeAction->setToolTip("Select edges");
    edgeAction->setCheckable(true);
    m_actionMap["mode_edge"] = edgeAction;
    // Face
    auto faceAction = new QAction("Face", this);
    faceAction->setObjectName("mode_face");
    faceAction->setToolTip("Select faces");
    faceAction->setCheckable(true);
    m_actionMap["mode_face"] = faceAction;
    qDebug() << "[SelectionToolbar] Mode actions created directly";
    qDebug() << "[SelectionToolbar] createModeActions() completed";
}

void SelectionToolbar::createModifierActions()
{
    qDebug() << "[SelectionToolbar] createModifierActions() started";
    // Replace
    auto replaceAction = new QAction("Replace", this);
    replaceAction->setObjectName("modifier_replace");
    replaceAction->setToolTip("Replace current selection");
    replaceAction->setCheckable(true);
    replaceAction->setChecked(true);
    m_actionMap["modifier_replace"] = replaceAction;
    // Add
    auto addAction = new QAction("Add", this);
    addAction->setObjectName("modifier_add");
    addAction->setToolTip("Add to current selection (Shift)");
    addAction->setCheckable(true);
    m_actionMap["modifier_add"] = addAction;
    // Subtract
    auto subtractAction = new QAction("Subtract", this);
    subtractAction->setObjectName("modifier_subtract");
    subtractAction->setToolTip("Remove from current selection (Ctrl)");
    subtractAction->setCheckable(true);
    m_actionMap["modifier_subtract"] = subtractAction;
    // Intersect
    auto intersectAction = new QAction("Intersect", this);
    intersectAction->setObjectName("modifier_intersect");
    intersectAction->setToolTip("Intersect with current selection");
    intersectAction->setCheckable(true);
    m_actionMap["modifier_intersect"] = intersectAction;
    qDebug() << "[SelectionToolbar] Modifier actions created directly";
    qDebug() << "[SelectionToolbar] createModifierActions() completed";
}

void SelectionToolbar::createToolActions()
{
    qDebug() << "[SelectionToolbar] createToolActions() started";
    // Direct selection tool
    auto directAction = new QAction("Direct", this);
    directAction->setObjectName("tool_direct");
    directAction->setToolTip("Direct selection tool");
    directAction->setCheckable(true);
    directAction->setChecked(true);
    m_actionMap["tool_direct"] = directAction;
    // Box selection tool
    auto boxAction = new QAction("Box", this);
    boxAction->setObjectName("tool_box");
    boxAction->setToolTip("Box selection tool");
    boxAction->setCheckable(true);
    m_actionMap["tool_box"] = boxAction;
    // Lasso selection tool
    auto lassoAction = new QAction("Lasso", this);
    lassoAction->setObjectName("tool_lasso");
    lassoAction->setToolTip("Lasso selection tool");
    lassoAction->setCheckable(true);
    m_actionMap["tool_lasso"] = lassoAction;
    // Brush selection tool
    auto brushAction = new QAction("Brush", this);
    brushAction->setObjectName("tool_brush");
    brushAction->setToolTip("Brush selection tool");
    brushAction->setCheckable(true);
    m_actionMap["tool_brush"] = brushAction;
    qDebug() << "[SelectionToolbar] Tool actions created directly";
    qDebug() << "[SelectionToolbar] createToolActions() completed";
}

void SelectionToolbar::createOperationActions()
{
    qDebug() << "[SelectionToolbar] createOperationActions() started";
    // Select All
    auto selectAllAction = new QAction("Select All", this);
    selectAllAction->setObjectName("operation_select_all");
    selectAllAction->setToolTip("Select all entities");
    selectAllAction->setShortcut(QKeySequence("Ctrl+A"));
    m_actionMap["operation_select_all"] = selectAllAction;
    // Deselect All
    auto deselectAllAction = new QAction("Deselect All", this);
    deselectAllAction->setObjectName("operation_deselect_all");
    deselectAllAction->setToolTip("Clear selection");
    deselectAllAction->setShortcut(QKeySequence("Ctrl+D"));
    m_actionMap["operation_deselect_all"] = deselectAllAction;
    // Invert selection
    auto invertAction = new QAction("Invert", this);
    invertAction->setObjectName("operation_invert");
    invertAction->setToolTip("Invert current selection");
    m_actionMap["operation_invert"] = invertAction;
    qDebug() << "[SelectionToolbar] Operation actions created directly";
    qDebug() << "[SelectionToolbar] createOperationActions() completed";
}

void SelectionToolbar::setSelectionMode(rude::ComponentType mode)
{
    if (m_currentMode != mode) {
        m_currentMode = mode;
        updateModeButtons();
        emit selectionModeChanged(mode);
    }
}

void SelectionToolbar::updateModeButtons()
{
    // Update button states based on current mode
    for (auto& action : m_actions) {
        if (action->objectName().startsWith("mode_")) {
            bool shouldCheck = false;
            
            if (action->objectName() == "mode_entity" && m_currentMode == rude::ComponentType::Entity) {
                shouldCheck = true;
            } else if (action->objectName() == "mode_vertex" && m_currentMode == rude::ComponentType::Vertex) {
                shouldCheck = true;
            } else if (action->objectName() == "mode_edge" && m_currentMode == rude::ComponentType::Edge) {
                shouldCheck = true;
            } else if (action->objectName() == "mode_face" && m_currentMode == rude::ComponentType::Face) {
                shouldCheck = true;
            }
            
            action->setChecked(shouldCheck);
        }
    }
}

void SelectionToolbar::onSelectionModeChanged()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    rude::ComponentType newMode = rude::ComponentType::Entity;
    
    if (action->objectName() == "mode_entity") {
        newMode = rude::ComponentType::Entity;
    } else if (action->objectName() == "mode_vertex") {
        newMode = rude::ComponentType::Vertex;
    } else if (action->objectName() == "mode_edge") {
        newMode = rude::ComponentType::Edge;
    } else if (action->objectName() == "mode_face") {
        newMode = rude::ComponentType::Face;
    }
    
    setSelectionMode(newMode);
}

void SelectionToolbar::onSelectionModifierChanged()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    QString modifier = "Replace";
    
    if (action->objectName() == "modifier_replace") {
        modifier = "Replace";
    } else if (action->objectName() == "modifier_add") {
        modifier = "Add";
    } else if (action->objectName() == "modifier_subtract") {
        modifier = "Subtract";
    } else if (action->objectName() == "modifier_intersect") {
        modifier = "Intersect";
    }
    
    if (m_currentModifier != modifier) {
        m_currentModifier = modifier;
        emit selectionModifierChanged(modifier);
    }
}

void SelectionToolbar::onSelectionToolChanged()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    QString tool = "Direct";
    
    if (action->objectName() == "tool_direct") {
        tool = "Direct";
    } else if (action->objectName() == "tool_box") {
        tool = "Box";
    } else if (action->objectName() == "tool_lasso") {
        tool = "Lasso";
    } else if (action->objectName() == "tool_paint") {
        tool = "Paint";
    }
    
    if (m_currentTool != tool) {
        m_currentTool = tool;
        emit selectionToolChanged(tool);
    }
}

void SelectionToolbar::onSelectionOperationRequested()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    QString operation;
    
    if (action->objectName() == "operation_select_all") {
        operation = "SelectAll";
    } else if (action->objectName() == "operation_deselect_all") {
        operation = "DeselectAll";
    } else if (action->objectName() == "operation_invert") {
        operation = "Invert";
    } else if (action->objectName() == "operation_select_similar") {
        operation = "SelectSimilar";
    } else if (action->objectName() == "operation_grow") {
        operation = "Grow";
    } else if (action->objectName() == "operation_shrink") {
        operation = "Shrink";
    }
    
    if (!operation.isEmpty()) {
        emit selectionOperationRequested(operation);
    }
}
