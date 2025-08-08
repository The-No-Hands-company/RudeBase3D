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
    // Basic primitives
    QAction* cube = new QAction("Cube", this);
    cube->setObjectName("cube");
    cube->setToolTip("Create a new cube primitive");
    addAction(cube);
    QAction* sphere = new QAction("Sphere", this);
    sphere->setObjectName("sphere");
    sphere->setToolTip("Create a new sphere primitive");
    addAction(sphere);
    QAction* cylinder = new QAction("Cylinder", this);
    cylinder->setObjectName("cylinder");
    cylinder->setToolTip("Create a new cylinder primitive");
    addAction(cylinder);
    QAction* plane = new QAction("Plane", this);
    plane->setObjectName("plane");
    plane->setToolTip("Create a new plane primitive");
    addAction(plane);
    QAction* cone = new QAction("Cone", this);
    cone->setObjectName("cone");
    cone->setToolTip("Create a new cone primitive");
    addAction(cone);
    addSeparator();
    // Advanced primitives
    QAction* torus = new QAction("Torus", this);
    torus->setObjectName("torus");
    torus->setToolTip("Create a new torus primitive");
    addAction(torus);
    QAction* icosphere = new QAction("Icosphere", this);
    icosphere->setObjectName("icosphere");
    icosphere->setToolTip("Create a new icosphere primitive");
    addAction(icosphere);
    QAction* uvSphere = new QAction("UV Sphere", this);
    uvSphere->setObjectName("uv_sphere");
    uvSphere->setToolTip("Create a new UV sphere primitive");
    addAction(uvSphere);
    QAction* monkey = new QAction("Monkey", this);
    monkey->setObjectName("monkey");
    monkey->setToolTip("Create Suzanne (monkey head) primitive");
    addAction(monkey);
    addSeparator();
    // Curve tools
    QAction* bezier = new QAction("Bezier Curve", this);
    bezier->setObjectName("bezier_curve");
    bezier->setToolTip("Create a new Bezier curve");
    addAction(bezier);
    QAction* circle = new QAction("Circle", this);
    circle->setObjectName("circle");
    circle->setToolTip("Create a new circle curve");
    addAction(circle);
    QAction* path = new QAction("Path", this);
    path->setObjectName("path");
    path->setToolTip("Create a new path curve");
    addAction(path);
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
