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
    // Transform modes
    QAction* move = new QAction("Move", this);
    move->setObjectName("mode_move");
    move->setToolTip("Move selected objects (G)");
    move->setCheckable(true);
    move->setChecked(true);
    move->setShortcut(QKeySequence("G"));
    addAction(move);
    QAction* rotate = new QAction("Rotate", this);
    rotate->setObjectName("mode_rotate");
    rotate->setToolTip("Rotate selected objects (R)");
    rotate->setCheckable(true);
    rotate->setShortcut(QKeySequence("R"));
    addAction(rotate);
    QAction* scale = new QAction("Scale", this);
    scale->setObjectName("mode_scale");
    scale->setToolTip("Scale selected objects (S)");
    scale->setCheckable(true);
    scale->setShortcut(QKeySequence("S"));
    addAction(scale);
    QAction* free = new QAction("Free", this);
    free->setObjectName("mode_free");
    free->setToolTip("Free transform with gizmo");
    free->setCheckable(true);
    addAction(free);
    addSeparator();
    // Coordinate systems
    QAction* global = new QAction("Global", this);
    global->setObjectName("coord_global");
    global->setToolTip("Use global coordinate system");
    global->setCheckable(true);
    global->setChecked(true);
    addAction(global);
    QAction* local = new QAction("Local", this);
    local->setObjectName("coord_local");
    local->setToolTip("Use local coordinate system");
    local->setCheckable(true);
    addAction(local);
    QAction* view = new QAction("View", this);
    view->setObjectName("coord_view");
    view->setToolTip("Use view coordinate system");
    view->setCheckable(true);
    addAction(view);
    QAction* normal = new QAction("Normal", this);
    normal->setObjectName("coord_normal");
    normal->setToolTip("Use surface normal coordinate system");
    normal->setCheckable(true);
    addAction(normal);
    addSeparator();
    // Axis constraints
    QAction* none = new QAction("Free", this);
    none->setObjectName("constraint_none");
    none->setToolTip("No axis constraint");
    none->setCheckable(true);
    none->setChecked(true);
    addAction(none);
    QAction* x = new QAction("X", this);
    x->setObjectName("constraint_x");
    x->setToolTip("Constrain to X-axis");
    x->setCheckable(true);
    x->setShortcut(QKeySequence("X"));
    addAction(x);
    QAction* y = new QAction("Y", this);
    y->setObjectName("constraint_y");
    y->setToolTip("Constrain to Y-axis");
    y->setCheckable(true);
    y->setShortcut(QKeySequence("Y"));
    addAction(y);
    QAction* z = new QAction("Z", this);
    z->setObjectName("constraint_z");
    z->setToolTip("Constrain to Z-axis");
    z->setCheckable(true);
    z->setShortcut(QKeySequence("Z"));
    addAction(z);
    QAction* xy = new QAction("XY", this);
    xy->setObjectName("constraint_xy");
    xy->setToolTip("Constrain to XY-plane");
    xy->setCheckable(true);
    addAction(xy);
    QAction* xz = new QAction("XZ", this);
    xz->setObjectName("constraint_xz");
    xz->setToolTip("Constrain to XZ-plane");
    xz->setCheckable(true);
    addAction(xz);
    QAction* yz = new QAction("YZ", this);
    yz->setObjectName("constraint_yz");
    yz->setToolTip("Constrain to YZ-plane");
    yz->setCheckable(true);
    addAction(yz);
    addSeparator();
    // Options
    QAction* snap = new QAction("Snap", this);
    snap->setObjectName("option_snap");
    snap->setToolTip("Enable/disable snapping");
    snap->setCheckable(true);
    snap->setChecked(false);
    addAction(snap);
    QAction* prop = new QAction("Proportional", this);
    prop->setObjectName("option_proportional");
    prop->setToolTip("Enable/disable proportional editing");
    prop->setCheckable(true);
    prop->setChecked(false);
    prop->setShortcut(QKeySequence("O"));
    addAction(prop);
    QAction* snapSettings = new QAction("Snap Settings", this);
    snapSettings->setObjectName("option_snap_settings");
    snapSettings->setToolTip("Configure snap settings");
    addAction(snapSettings);
    QAction* orientation = new QAction("Orientation", this);
    orientation->setObjectName("option_orientation");
    orientation->setToolTip("Transform orientation settings");
    addAction(orientation);
    QAction* pivot = new QAction("Pivot", this);
    pivot->setObjectName("option_pivot");
    pivot->setToolTip("Set transform pivot point");
    addAction(pivot);
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
