#include "panels/selection_panel.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QToolButton>
#include <QButtonGroup>
#include <QDialog>

// Add namespace for the component types if needed
using namespace rude;

SelectionPanel::SelectionPanel(QWidget* parent)
    : QDockWidget("Selection", parent)
    , m_selectionManager(nullptr)
{
    // Create the main content widget
    auto* contentWidget = new QWidget(this);
    setWidget(contentWidget);
    
    setupUI();
}

void SelectionPanel::setSelectionManager(rude::SelectionManager* manager) {
    m_selectionManager = manager;
    updateSelectionInfo();
}

void SelectionPanel::setupUI() {
    auto* contentWidget = widget(); // Get the content widget we set in the constructor
    auto* mainLayout = new QVBoxLayout(contentWidget);

    // Selection Mode Group
    auto* modeGroup = new QGroupBox("Selection Mode", contentWidget);
    auto* modeLayout = new QHBoxLayout(modeGroup);
    
    m_selectionModeGroup = new QButtonGroup(this);
    
    m_entityModeButton = new QToolButton(this);
    m_entityModeButton->setText("Entity");
    m_entityModeButton->setToolTip("Select entities (1)");
    m_entityModeButton->setCheckable(true);
    m_entityModeButton->setChecked(true);
    
    m_vertexModeButton = new QToolButton(this);
    m_vertexModeButton->setText("Vertex");
    m_vertexModeButton->setToolTip("Select vertices (2)");
    m_vertexModeButton->setCheckable(true);
    
    m_edgeModeButton = new QToolButton(this);
    m_edgeModeButton->setText("Edge");
    m_edgeModeButton->setToolTip("Select edges (3)");
    m_edgeModeButton->setCheckable(true);
    
    m_faceModeButton = new QToolButton(this);
    m_faceModeButton->setText("Face");
    m_faceModeButton->setToolTip("Select faces (4)");
    m_faceModeButton->setCheckable(true);
    
    m_selectionModeGroup->addButton(m_entityModeButton, static_cast<int>(ComponentType::Entity));
    m_selectionModeGroup->addButton(m_vertexModeButton, static_cast<int>(ComponentType::Vertex));
    m_selectionModeGroup->addButton(m_edgeModeButton, static_cast<int>(ComponentType::Edge));
    m_selectionModeGroup->addButton(m_faceModeButton, static_cast<int>(ComponentType::Face));
    
    modeLayout->addWidget(m_entityModeButton);
    modeLayout->addWidget(m_vertexModeButton);
    modeLayout->addWidget(m_edgeModeButton);
    modeLayout->addWidget(m_faceModeButton);
    
    connect(m_selectionModeGroup, &QButtonGroup::idClicked,
            this, &SelectionPanel::onSelectionModeChanged);
    
    // Selection Info
    m_selectionInfoLabel = new QLabel("No selection", this);
    m_selectionInfoLabel->setWordWrap(true);
    
    // Mesh Operations Group
    auto* operationsGroup = new QGroupBox("Mesh Operations", contentWidget);
    auto* operationsLayout = new QVBoxLayout(operationsGroup);
    
    m_extrudeFaceButton = new QToolButton(this);
    m_extrudeFaceButton->setText("Extrude Face");
    m_extrudeFaceButton->setToolTip("Extrude selected faces");
    m_extrudeFaceButton->setEnabled(false);
    
    m_bevelEdgeButton = new QToolButton(this);
    m_bevelEdgeButton->setText("Bevel Edge");
    m_bevelEdgeButton->setToolTip("Bevel selected edges");
    m_bevelEdgeButton->setEnabled(false);
    
    m_subdivideFaceButton = new QToolButton(this);
    m_subdivideFaceButton->setText("Subdivide Face");
    m_subdivideFaceButton->setToolTip("Subdivide selected faces");
    m_subdivideFaceButton->setEnabled(false);
    
    operationsLayout->addWidget(m_extrudeFaceButton);
    operationsLayout->addWidget(m_bevelEdgeButton);
    operationsLayout->addWidget(m_subdivideFaceButton);
    
    connect(m_extrudeFaceButton, &QToolButton::clicked, this, &SelectionPanel::onExtrudeFaceClicked);
    connect(m_bevelEdgeButton, &QToolButton::clicked, this, &SelectionPanel::onBevelEdgeClicked);
    connect(m_subdivideFaceButton, &QToolButton::clicked, this, &SelectionPanel::onSubdivideFaceClicked);
    
    // Layout
    mainLayout->addWidget(modeGroup);
    mainLayout->addWidget(m_selectionInfoLabel);
    mainLayout->addWidget(operationsGroup);
    mainLayout->addStretch();
}

void SelectionPanel::updateSelectionInfo() {
    if (!m_selectionManager) {
        m_selectionInfoLabel->setText("No selection manager");
        return;
    }
    
    ComponentType primaryType = m_selectionManager->getPrimarySelectionType();
    QString typeText;
    
    switch (primaryType) {
        case ComponentType::Vertex:
            typeText = "vertices";
            break;
        case ComponentType::Edge:
            typeText = "edges";
            break;
        case ComponentType::Face:
            typeText = "faces";
            break;
        case ComponentType::Entity:
            typeText = "entities";
            break;
        default:
            typeText = "components";
            break;
    }
    
    // Pass primaryType to getSelectionCount
    size_t selectionCount = m_selectionManager->getSelectionCount(primaryType);
    
    m_selectionInfoLabel->setText(QString("%1 %2 selected").arg(selectionCount).arg(typeText));
    
    // Enable/disable operation buttons based on selection
    m_extrudeFaceButton->setEnabled(primaryType == ComponentType::Face && selectionCount > 0);
    m_bevelEdgeButton->setEnabled(primaryType == ComponentType::Edge && selectionCount > 0);
    m_subdivideFaceButton->setEnabled(primaryType == ComponentType::Face && selectionCount > 0);
}

void SelectionPanel::onSelectionModeChanged(int buttonId) {
    auto mode = static_cast<ComponentType>(buttonId);
    emit selectionModeChanged(mode);
}

void SelectionPanel::onExtrudeFaceClicked() {
    // TODO: Implement proper parameter dialog
    emit operationRequestedWithParams("extrude_face", 1.0); // Default extrude distance
}

void SelectionPanel::onBevelEdgeClicked() {
    // TODO: Implement proper parameter dialog  
    emit operationRequestedWithParams("bevel_edge", 0.1); // Default bevel distance
}

void SelectionPanel::onSubdivideFaceClicked() {
    // TODO: Implement proper parameter dialog
    emit operationRequestedWithParams("subdivide_face", 1.0); // Default subdivision level
}

void SelectionPanel::setCurrentMode(ComponentType mode) {
    // Find the button corresponding to the mode and click it
    if (auto button = m_selectionModeGroup->button(static_cast<int>(mode))) {
        button->setChecked(true);
        // Don't emit the signal since this is being set programmatically
    }
}

void SelectionPanel::onInsertEdgeLoopClicked() {
    // TODO: Implement proper parameter dialog
    emit operationRequestedWithParams("insert_edge_loop", 1.0); // Default number of cuts
}
