#include "toolbars/primitives_toolbar.hpp"
#include <QDebug>

PrimitivesToolbar::PrimitivesToolbar(QMainWindow* parent)
    : BaseToolbar("Primitives", parent)
{
    setObjectName(getToolbarId());  // Safe to call virtual method after construction
    initialize();
}

void PrimitivesToolbar::createActions()
{
    createBasicPrimitives();
    addToolbarSeparator();
    createAdvancedPrimitives();
    addToolbarSeparator();
    createCurveTools();
}

void PrimitivesToolbar::setupLayout()
{
    // Layout is handled by the order of action creation
    // Additional layout customization can be done here
}

void PrimitivesToolbar::connectSignals()
{
    // Connect the base toolbar's actionTriggered signal to our slot
    connect(this, &BaseToolbar::actionTriggered, 
            this, &PrimitivesToolbar::onActionTriggeredFromBase);
}

void PrimitivesToolbar::onActionTriggeredFromBase(const QString& actionId)
{
    qDebug() << "[PrimitivesToolbar] Action triggered:" << actionId;
    qDebug() << "[PrimitivesToolbar] About to emit primitiveRequested signal...";
    
    // Emit the appropriate signal based on the action ID
    if (actionId == "cube" || actionId == "sphere" || actionId == "cylinder" || 
        actionId == "plane" || actionId == "cone" || actionId == "torus" ||
        actionId == "icosphere" || actionId == "uv_sphere" || actionId == "monkey") {
        qDebug() << "[PrimitivesToolbar] Emitting primitiveRequested:" << actionId;
        emit primitiveRequested(actionId);
        qDebug() << "[PrimitivesToolbar] primitiveRequested signal emitted";
    } else if (actionId == "bezier_curve" || actionId == "circle" || actionId == "path") {
        qDebug() << "[PrimitivesToolbar] Emitting curveToolRequested:" << actionId;
        emit curveToolRequested(actionId);
    } else {
        qDebug() << "[PrimitivesToolbar] Unknown action ID:" << actionId;
    }
}

void PrimitivesToolbar::onPrimitiveRequested()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QString primitiveType = action->objectName(); // Use objectName which is set to the id
        emit primitiveRequested(primitiveType);
        qDebug() << "[PrimitivesToolbar] Primitive requested:" << primitiveType;
    } else {
        qDebug() << "[PrimitivesToolbar] onPrimitiveRequested: sender is not a QAction";
    }
}

void PrimitivesToolbar::onCurveToolRequested()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QString curveType = action->text().toLower().replace(' ', '_');
        emit curveToolRequested(curveType);
        qDebug() << "[PrimitivesToolbar] Curve tool requested:" << curveType;
    }
}

void PrimitivesToolbar::createBasicPrimitives()
{
    // Basic 3D primitives
    createAction("cube", "Cube", "Create a new cube primitive", true);
    createAction("sphere", "Sphere", "Create a new sphere primitive", true);
    createAction("cylinder", "Cylinder", "Create a new cylinder primitive", true);
    createAction("plane", "Plane", "Create a new plane primitive", true);
    createAction("cone", "Cone", "Create a new cone primitive", true);
}

void PrimitivesToolbar::createAdvancedPrimitives()
{
    // Advanced primitives
    createAction("torus", "Torus", "Create a new torus primitive", true);
    createAction("icosphere", "Icosphere", "Create a new icosphere primitive", true);
    createAction("uv_sphere", "UV Sphere", "Create a new UV sphere primitive", true);
    createAction("monkey", "Monkey", "Create Suzanne (monkey head) primitive", true);
}

void PrimitivesToolbar::createCurveTools()
{
    // Curve and surface tools
    createAction("bezier_curve", "Bezier Curve", "Create a new Bezier curve", true);
    createAction("circle", "Circle", "Create a new circle curve", true);
    createAction("path", "Path", "Create a new path curve", true);
}
