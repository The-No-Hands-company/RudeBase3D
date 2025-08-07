#include "toolbars/selection_toolbar.hpp"
#include <QActionGroup>

SelectionToolbar::SelectionToolbar(QMainWindow* parent)
    : BaseToolbar("Selection", parent)
    , m_currentMode(rude::ComponentType::Entity)
    , m_currentModifier("Replace")
    , m_currentTool("Direct")
{
    setObjectName(getToolbarId());
    initialize();
}

void SelectionToolbar::createActions()
{
    createModeActions();
    createModifierActions();
    createToolActions();
    createOperationActions();
    
    setupLayout();
    connectSignals();
}

void SelectionToolbar::setupLayout()
{
    // Add mode actions
    addAction(m_actionMap["mode_entity"]);
    addAction(m_actionMap["mode_vertex"]);
    addAction(m_actionMap["mode_edge"]);
    addAction(m_actionMap["mode_face"]);
    
    addSeparator();
    
    // Add modifier actions
    addAction(m_actionMap["modifier_replace"]);
    addAction(m_actionMap["modifier_add"]);
    addAction(m_actionMap["modifier_subtract"]);
    addAction(m_actionMap["modifier_intersect"]);
    
    addSeparator();
    
    // Add tool actions
    addAction(m_actionMap["tool_direct"]);
    addAction(m_actionMap["tool_box"]);
    addAction(m_actionMap["tool_lasso"]);
    addAction(m_actionMap["tool_brush"]);
    
    addSeparator();
    
    // Add operation actions
    addAction(m_actionMap["operation_select_all"]);
    addAction(m_actionMap["operation_deselect_all"]);
    addAction(m_actionMap["operation_invert"]);
}

void SelectionToolbar::connectSignals()
{
    // Connect mode actions
    connect(m_actionMap["mode_entity"], &QAction::triggered, this, &SelectionToolbar::onSelectionModeChanged);
    connect(m_actionMap["mode_vertex"], &QAction::triggered, this, &SelectionToolbar::onSelectionModeChanged);
    connect(m_actionMap["mode_edge"], &QAction::triggered, this, &SelectionToolbar::onSelectionModeChanged);
    connect(m_actionMap["mode_face"], &QAction::triggered, this, &SelectionToolbar::onSelectionModeChanged);
    
    // Connect modifier actions
    connect(m_actionMap["modifier_replace"], &QAction::triggered, this, &SelectionToolbar::onSelectionModifierChanged);
    connect(m_actionMap["modifier_add"], &QAction::triggered, this, &SelectionToolbar::onSelectionModifierChanged);
    connect(m_actionMap["modifier_subtract"], &QAction::triggered, this, &SelectionToolbar::onSelectionModifierChanged);
    connect(m_actionMap["modifier_intersect"], &QAction::triggered, this, &SelectionToolbar::onSelectionModifierChanged);
    
    // Connect tool actions
    connect(m_actionMap["tool_direct"], &QAction::triggered, this, &SelectionToolbar::onSelectionToolChanged);
    connect(m_actionMap["tool_box"], &QAction::triggered, this, &SelectionToolbar::onSelectionToolChanged);
    connect(m_actionMap["tool_lasso"], &QAction::triggered, this, &SelectionToolbar::onSelectionToolChanged);
    connect(m_actionMap["tool_brush"], &QAction::triggered, this, &SelectionToolbar::onSelectionToolChanged);
    
    // Connect operation actions
    connect(m_actionMap["operation_select_all"], &QAction::triggered, this, &SelectionToolbar::onSelectionOperationRequested);
    connect(m_actionMap["operation_deselect_all"], &QAction::triggered, this, &SelectionToolbar::onSelectionOperationRequested);
    connect(m_actionMap["operation_invert"], &QAction::triggered, this, &SelectionToolbar::onSelectionOperationRequested);
}

void SelectionToolbar::createModeActions()
{
    // Create action group for exclusive selection
    auto modeGroup = new QActionGroup(this);
    
    // Entity selection mode
    auto entityAction = createAction("mode_entity", "Entity", "Select entire entities", true);
    entityAction->setCheckable(true);
    entityAction->setChecked(true);
    modeGroup->addAction(entityAction);
    
    // Vertex selection mode
    auto vertexAction = createAction("mode_vertex", "Vertex", "Select individual vertices", true);
    vertexAction->setCheckable(true);
    modeGroup->addAction(vertexAction);
    
    // Edge selection mode
    auto edgeAction = createAction("mode_edge", "Edge", "Select edges", true);
    edgeAction->setCheckable(true);
    modeGroup->addAction(edgeAction);
    
    // Face selection mode
    auto faceAction = createAction("mode_face", "Face", "Select faces", true);
    faceAction->setCheckable(true);
    modeGroup->addAction(faceAction);
}

void SelectionToolbar::createModifierActions()
{
    // Create action group for exclusive selection
    auto modifierGroup = new QActionGroup(this);
    
    // Replace selection
    auto replaceAction = createAction("modifier_replace", "Replace", "Replace current selection", true);
    replaceAction->setCheckable(true);
    replaceAction->setChecked(true);
    modifierGroup->addAction(replaceAction);
    
    // Add to selection
    auto addAction = createAction("modifier_add", "Add", "Add to current selection (Shift)", true);
    addAction->setCheckable(true);
    modifierGroup->addAction(addAction);
    
    // Subtract from selection
    auto subtractAction = createAction("modifier_subtract", "Subtract", "Remove from current selection (Ctrl)", true);
    subtractAction->setCheckable(true);
    modifierGroup->addAction(subtractAction);
    
    // Intersect selection
    auto intersectAction = createAction("modifier_intersect", "Intersect", "Intersect with current selection", true);
    intersectAction->setCheckable(true);
    modifierGroup->addAction(intersectAction);
}

void SelectionToolbar::createToolActions()
{
    // Create action group for exclusive selection
    auto toolGroup = new QActionGroup(this);
    
    // Direct selection
    auto directAction = createAction("tool_direct", "Direct", "Direct click selection", true);
    directAction->setCheckable(true);
    directAction->setChecked(true);
    toolGroup->addAction(directAction);
    
    // Box selection
    auto boxAction = createAction("tool_box", "Box", "Box selection tool", true);
    boxAction->setCheckable(true);
    toolGroup->addAction(boxAction);
    
    // Lasso selection
    auto lassoAction = createAction("tool_lasso", "Lasso", "Lasso selection tool", true);
    lassoAction->setCheckable(true);
    toolGroup->addAction(lassoAction);
    
    // Paint selection
    auto paintAction = createAction("tool_paint", "Paint", "Paint selection tool", true);
    paintAction->setCheckable(true);
    toolGroup->addAction(paintAction);
}

void SelectionToolbar::createOperationActions()
{
    // Select All
    auto selectAllAction = createAction("operation_select_all", "Select All", "Select all entities", true);
    selectAllAction->setShortcut(QKeySequence("Ctrl+A"));
    
    // Deselect All
    auto deselectAllAction = createAction("operation_deselect_all", "Deselect All", "Clear selection", true);
    deselectAllAction->setShortcut(QKeySequence("Ctrl+D"));
    
    // Invert Selection
    auto invertAction = createAction("operation_invert", "Invert", "Invert current selection", true);
    invertAction->setShortcut(QKeySequence("Ctrl+I"));
    
    // Select Similar
    auto selectSimilarAction = createAction("operation_select_similar", "Select Similar", "Select similar entities", true);
    // TODO: Connect selectSimilarAction to selection manager when implemented
    (void)selectSimilarAction; // Acknowledge action until similar selection is implemented
    
    // Grow Selection
    auto growAction = createAction("operation_grow", "Grow", "Grow selection to connected elements", true);
    // TODO: Connect growAction to selection manager when implemented
    (void)growAction; // Acknowledge action until grow selection is implemented
    
    // Shrink Selection
    auto shrinkAction = createAction("operation_shrink", "Shrink", "Shrink selection", true);
    // TODO: Connect shrinkAction to selection manager when implemented
    (void)shrinkAction; // Acknowledge action until shrink selection is implemented
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
