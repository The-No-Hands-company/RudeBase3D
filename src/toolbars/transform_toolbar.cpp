#include "toolbars/transform_toolbar.hpp"
#include <QActionGroup>

TransformToolbar::TransformToolbar(QMainWindow* parent)
    : BaseToolbar("Transform", parent)
    , m_currentMode("Move")
    , m_coordinateSystem("Global")
    , m_axisConstraint("None")
    , m_snapEnabled(false)
    , m_proportionalEditingEnabled(false)
{
    setObjectName(getToolbarId());  // Use virtual method instead of hardcoded string
    initialize();
}

void TransformToolbar::createActions()
{
    createModeActions();
    createCoordinateActions();
    createConstraintActions();
    createOptionActions();
}

void TransformToolbar::setupLayout()
{
    // Add mode actions
    for (const auto& action : m_actions) {
        if (action->objectName().startsWith("mode_")) {
            addAction(action.get());
        }
    }
    
    addSeparator();
    
    // Add coordinate system actions
    for (const auto& action : m_actions) {
        if (action->objectName().startsWith("coord_")) {
            addAction(action.get());
        }
    }
    
    addSeparator();
    
    // Add constraint actions
    for (const auto& action : m_actions) {
        if (action->objectName().startsWith("constraint_")) {
            addAction(action.get());
        }
    }
    
    addSeparator();
    
    // Add option actions
    for (const auto& action : m_actions) {
        if (action->objectName().startsWith("option_")) {
            addAction(action.get());
        }
    }
}

void TransformToolbar::connectSignals()
{
    // Connect all actions to their respective slots
    for (const auto& action : m_actions) {
        if (action->objectName().startsWith("mode_")) {
            connect(action.get(), &QAction::triggered, this, &TransformToolbar::onTransformModeChanged);
        } else if (action->objectName().startsWith("coord_")) {
            connect(action.get(), &QAction::triggered, this, &TransformToolbar::onCoordinateSystemChanged);
        } else if (action->objectName().startsWith("constraint_")) {
            connect(action.get(), &QAction::triggered, this, &TransformToolbar::onAxisConstraintChanged);
        } else if (action->objectName().startsWith("option_snap")) {
            connect(action.get(), &QAction::toggled, this, &TransformToolbar::onSnapToggled);
        } else if (action->objectName().startsWith("option_proportional")) {
            connect(action.get(), &QAction::toggled, this, &TransformToolbar::onProportionalEditingToggled);
        }
    }
}

void TransformToolbar::createModeActions()
{
    // Create action group for exclusive selection
    auto modeGroup = new QActionGroup(this);
    
    // Move mode
    auto moveAction = createAction("mode_move", "Move", "Move selected objects (G)", true);
    moveAction->setCheckable(true);
    moveAction->setChecked(true);
    moveAction->setShortcut(QKeySequence("G"));
    modeGroup->addAction(moveAction);
    
    // Rotate mode
    auto rotateAction = createAction("mode_rotate", "Rotate", "Rotate selected objects (R)", true);
    rotateAction->setCheckable(true);
    rotateAction->setShortcut(QKeySequence("R"));
    modeGroup->addAction(rotateAction);
    
    // Scale mode
    auto scaleAction = createAction("mode_scale", "Scale", "Scale selected objects (S)", true);
    scaleAction->setCheckable(true);
    scaleAction->setShortcut(QKeySequence("S"));
    modeGroup->addAction(scaleAction);
    
    // Free transform mode
    auto freeAction = createAction("mode_free", "Free", "Free transform with gizmo", true);
    freeAction->setCheckable(true);
    modeGroup->addAction(freeAction);
}

void TransformToolbar::createCoordinateActions()
{
    // Create action group for exclusive selection
    auto coordGroup = new QActionGroup(this);
    
    // Global coordinates
    auto globalAction = createAction("coord_global", "Global", "Use global coordinate system", true);
    globalAction->setCheckable(true);
    globalAction->setChecked(true);
    coordGroup->addAction(globalAction);
    
    // Local coordinates
    auto localAction = createAction("coord_local", "Local", "Use local coordinate system", true);
    localAction->setCheckable(true);
    coordGroup->addAction(localAction);
    
    // View coordinates
    auto viewAction = createAction("coord_view", "View", "Use view coordinate system", true);
    viewAction->setCheckable(true);
    coordGroup->addAction(viewAction);
    
    // Normal coordinates (for face/surface alignment)
    auto normalAction = createAction("coord_normal", "Normal", "Use surface normal coordinate system", true);
    normalAction->setCheckable(true);
    coordGroup->addAction(normalAction);
}

void TransformToolbar::createConstraintActions()
{
    // Create action group for exclusive selection (including None)
    auto constraintGroup = new QActionGroup(this);
    
    // No constraint
    auto noneAction = createAction("constraint_none", "Free", "No axis constraint", true);
    noneAction->setCheckable(true);
    noneAction->setChecked(true);
    constraintGroup->addAction(noneAction);
    
    // X-axis constraint
    auto xAction = createAction("constraint_x", "X", "Constrain to X-axis", true);
    xAction->setCheckable(true);
    xAction->setShortcut(QKeySequence("X"));
    constraintGroup->addAction(xAction);
    
    // Y-axis constraint
    auto yAction = createAction("constraint_y", "Y", "Constrain to Y-axis", true);
    yAction->setCheckable(true);
    yAction->setShortcut(QKeySequence("Y"));
    constraintGroup->addAction(yAction);
    
    // Z-axis constraint
    auto zAction = createAction("constraint_z", "Z", "Constrain to Z-axis", true);
    zAction->setCheckable(true);
    zAction->setShortcut(QKeySequence("Z"));
    constraintGroup->addAction(zAction);
    
    // XY-plane constraint
    auto xyAction = createAction("constraint_xy", "XY", "Constrain to XY-plane", true);
    xyAction->setCheckable(true);
    constraintGroup->addAction(xyAction);
    
    // XZ-plane constraint
    auto xzAction = createAction("constraint_xz", "XZ", "Constrain to XZ-plane", true);
    xzAction->setCheckable(true);
    constraintGroup->addAction(xzAction);
    
    // YZ-plane constraint
    auto yzAction = createAction("constraint_yz", "YZ", "Constrain to YZ-plane", true);
    yzAction->setCheckable(true);
    constraintGroup->addAction(yzAction);
}

void TransformToolbar::createOptionActions()
{
    // Snap toggle
    auto snapAction = createAction("option_snap", "Snap", "Enable/disable snapping", false);
    snapAction->setCheckable(true);
    snapAction->setChecked(false);
    
    // Proportional editing toggle
    auto propAction = createAction("option_proportional", "Proportional", "Enable/disable proportional editing", false);
    propAction->setCheckable(true);
    propAction->setChecked(false);
    propAction->setShortcut(QKeySequence("O"));
    
    // Snap settings (opens snap settings dialog)
    auto snapSettingsAction = createAction("option_snap_settings", "Snap Settings", "Configure snap settings", false);
    
    // Transform orientation settings
    auto orientationAction = createAction("option_orientation", "Orientation", "Transform orientation settings", false);
    
    // Pivot settings
    auto pivotAction = createAction("option_pivot", "Pivot", "Set transform pivot point", false);
}

void TransformToolbar::setTransformMode(const QString& mode)
{
    if (m_currentMode != mode) {
        m_currentMode = mode;
        updateModeButtons();
        emit transformModeChanged(mode);
    }
}

void TransformToolbar::setCoordinateSystem(const QString& system)
{
    if (m_coordinateSystem != system) {
        m_coordinateSystem = system;
        emit coordinateSystemChanged(system);
    }
}

void TransformToolbar::setAxisConstraint(const QString& axis)
{
    if (m_axisConstraint != axis) {
        m_axisConstraint = axis;
        updateConstraintButtons();
        emit axisConstraintChanged(axis);
    }
}

void TransformToolbar::updateModeButtons()
{
    // Update button states based on current mode
    for (auto& action : m_actions) {
        if (action->objectName().startsWith("mode_")) {
            bool shouldCheck = false;
            
            if (action->objectName() == "mode_move" && m_currentMode == "Move") {
                shouldCheck = true;
            } else if (action->objectName() == "mode_rotate" && m_currentMode == "Rotate") {
                shouldCheck = true;
            } else if (action->objectName() == "mode_scale" && m_currentMode == "Scale") {
                shouldCheck = true;
            } else if (action->objectName() == "mode_free" && m_currentMode == "Free") {
                shouldCheck = true;
            }
            
            action->setChecked(shouldCheck);
        }
    }
}

void TransformToolbar::updateConstraintButtons()
{
    // Update constraint button states
    for (auto& action : m_actions) {
        if (action->objectName().startsWith("constraint_")) {
            bool shouldCheck = false;
            
            if (action->objectName() == "constraint_none" && m_axisConstraint == "None") {
                shouldCheck = true;
            } else if (action->objectName() == "constraint_x" && m_axisConstraint == "X") {
                shouldCheck = true;
            } else if (action->objectName() == "constraint_y" && m_axisConstraint == "Y") {
                shouldCheck = true;
            } else if (action->objectName() == "constraint_z" && m_axisConstraint == "Z") {
                shouldCheck = true;
            } else if (action->objectName() == "constraint_xy" && m_axisConstraint == "XY") {
                shouldCheck = true;
            } else if (action->objectName() == "constraint_xz" && m_axisConstraint == "XZ") {
                shouldCheck = true;
            } else if (action->objectName() == "constraint_yz" && m_axisConstraint == "YZ") {
                shouldCheck = true;
            }
            
            action->setChecked(shouldCheck);
        }
    }
}

void TransformToolbar::onTransformModeChanged()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    QString mode = "Move";
    
    if (action->objectName() == "mode_move") {
        mode = "Move";
    } else if (action->objectName() == "mode_rotate") {
        mode = "Rotate";
    } else if (action->objectName() == "mode_scale") {
        mode = "Scale";
    } else if (action->objectName() == "mode_free") {
        mode = "Free";
    }
    
    setTransformMode(mode);
}

void TransformToolbar::onCoordinateSystemChanged()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    QString system = "Global";
    
    if (action->objectName() == "coord_global") {
        system = "Global";
    } else if (action->objectName() == "coord_local") {
        system = "Local";
    } else if (action->objectName() == "coord_view") {
        system = "View";
    } else if (action->objectName() == "coord_normal") {
        system = "Normal";
    }
    
    setCoordinateSystem(system);
}

void TransformToolbar::onAxisConstraintChanged()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    QString constraint = "None";
    
    if (action->objectName() == "constraint_none") {
        constraint = "None";
    } else if (action->objectName() == "constraint_x") {
        constraint = "X";
    } else if (action->objectName() == "constraint_y") {
        constraint = "Y";
    } else if (action->objectName() == "constraint_z") {
        constraint = "Z";
    } else if (action->objectName() == "constraint_xy") {
        constraint = "XY";
    } else if (action->objectName() == "constraint_xz") {
        constraint = "XZ";
    } else if (action->objectName() == "constraint_yz") {
        constraint = "YZ";
    }
    
    setAxisConstraint(constraint);
}

void TransformToolbar::onSnapToggled()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    m_snapEnabled = action->isChecked();
    emit snapToggled(m_snapEnabled);
}

void TransformToolbar::onProportionalEditingToggled()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    m_proportionalEditingEnabled = action->isChecked();
    emit proportionalEditingToggled(m_proportionalEditingEnabled);
}
