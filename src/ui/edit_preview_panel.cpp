#include "ui/edit_preview_panel.hpp"
#include "scene.hpp"
#include "selection_manager.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <iostream>

namespace RudeBase3D {
namespace UI {

EditPreviewPanel::EditPreviewPanel(std::shared_ptr<Core::EditPreviewSystem> previewSystem, QWidget* parent)
    : QDockWidget("Edit Preview", parent)
    , m_previewSystem(previewSystem)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr) {
    
    // Initialize operations
    m_operations = {
        Core::EditOperation::Subdivision,
        Core::EditOperation::Extrude,
        Core::EditOperation::Bevel,
        Core::EditOperation::Scale,
        Core::EditOperation::Smooth,
        Core::EditOperation::Decimate
    };
    
    setupUI();
    
    // Setup event handlers
    if (m_previewSystem) {
        m_previewSystem->onPreviewStarted = [this](EntityID entityId, Core::EditOperation operation) {
            onPreviewStarted(entityId, operation);
        };
        
        m_previewSystem->onPreviewUpdated = [this](EntityID entityId, Core::EditOperation operation) {
            onPreviewUpdated(entityId, operation);
        };
        
        m_previewSystem->onPreviewFinished = [this](EntityID entityId, Core::EditOperation operation, bool committed) {
            onPreviewFinished(entityId, operation, committed);
        };
    }
    
    // Set initial state
    updatePreviewControlsState();
    updateHistoryControlsState();
    
    std::cout << "EditPreviewPanel initialized" << std::endl;
}

void EditPreviewPanel::setupUI() {
    // Create central widget and main layout
    m_centralWidget = new QWidget();
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_mainLayout->setSpacing(8);
    m_mainLayout->setContentsMargins(8, 8, 8, 8);
    
    // Setup groups
    setupOperationGroup();
    setupParameterGroup();
    setupPreviewGroup();
    setupHistoryGroup();
    setupVisualGroup();
    
    // Add stretch to push everything to the top
    m_mainLayout->addStretch();
    
    // Set the central widget
    setWidget(m_centralWidget);
    
    // Configure dock widget properties
    setFeatures(QDockWidget::DockWidgetMovable | 
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetClosable);
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
}

void EditPreviewPanel::setupOperationGroup() {
    m_operationGroup = new QGroupBox("Operation");
    QVBoxLayout* layout = new QVBoxLayout(m_operationGroup);
    
    // Operation combo box
    m_operationCombo = new QComboBox();
    m_operationCombo->addItems({
        "Subdivision",
        "Extrude",
        "Bevel", 
        "Scale",
        "Smooth",
        "Decimate"
    });
    
    connect(m_operationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EditPreviewPanel::onOperationChanged);
    
    // Start preview button
    m_startPreviewButton = new QPushButton("Start Preview");
    connect(m_startPreviewButton, &QPushButton::clicked,
            this, &EditPreviewPanel::onStartPreviewClicked);
    
    layout->addWidget(new QLabel("Select Operation:"));
    layout->addWidget(m_operationCombo);
    layout->addWidget(m_startPreviewButton);
    
    m_mainLayout->addWidget(m_operationGroup);
}

void EditPreviewPanel::setupParameterGroup() {
    m_parameterGroup = new QGroupBox("Parameters");
    m_parameterLayout = new QVBoxLayout(m_parameterGroup);
    
    // Create parameter widgets but don't add them yet
    // They will be shown/hidden based on selected operation
    
    // Subdivision parameters
    QWidget* subdivisionWidget = new QWidget();
    QGridLayout* subdivisionLayout = new QGridLayout(subdivisionWidget);
    
    subdivisionLayout->addWidget(new QLabel("Levels:"), 0, 0);
    m_subdivisionLevels = new QSpinBox();
    m_subdivisionLevels->setRange(1, 4);
    m_subdivisionLevels->setValue(1);
    connect(m_subdivisionLevels, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &EditPreviewPanel::onParameterChanged);
    subdivisionLayout->addWidget(m_subdivisionLevels, 0, 1);
    
    m_smoothSubdivision = new QCheckBox("Smooth Subdivision");
    m_smoothSubdivision->setChecked(true);
    connect(m_smoothSubdivision, &QCheckBox::toggled,
            this, &EditPreviewPanel::onParameterChanged);
    subdivisionLayout->addWidget(m_smoothSubdivision, 1, 0, 1, 2);
    
    // Extrude parameters
    QWidget* extrudeWidget = new QWidget();
    QGridLayout* extrudeLayout = new QGridLayout(extrudeWidget);
    
    extrudeLayout->addWidget(new QLabel("Distance:"), 0, 0);
    m_extrudeDistance = new QDoubleSpinBox();
    m_extrudeDistance->setRange(0.0, 10.0);
    m_extrudeDistance->setSingleStep(0.1);
    m_extrudeDistance->setValue(1.0);
    connect(m_extrudeDistance, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EditPreviewPanel::onParameterChanged);
    extrudeLayout->addWidget(m_extrudeDistance, 0, 1);
    
    extrudeLayout->addWidget(new QLabel("Direction X:"), 1, 0);
    m_extrudeDirectionX = new QDoubleSpinBox();
    m_extrudeDirectionX->setRange(-1.0, 1.0);
    m_extrudeDirectionX->setSingleStep(0.1);
    m_extrudeDirectionX->setValue(0.0);
    connect(m_extrudeDirectionX, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EditPreviewPanel::onParameterChanged);
    extrudeLayout->addWidget(m_extrudeDirectionX, 1, 1);
    
    extrudeLayout->addWidget(new QLabel("Direction Y:"), 2, 0);
    m_extrudeDirectionY = new QDoubleSpinBox();
    m_extrudeDirectionY->setRange(-1.0, 1.0);
    m_extrudeDirectionY->setSingleStep(0.1);
    m_extrudeDirectionY->setValue(0.0);
    connect(m_extrudeDirectionY, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EditPreviewPanel::onParameterChanged);
    extrudeLayout->addWidget(m_extrudeDirectionY, 2, 1);
    
    extrudeLayout->addWidget(new QLabel("Direction Z:"), 3, 0);
    m_extrudeDirectionZ = new QDoubleSpinBox();
    m_extrudeDirectionZ->setRange(-1.0, 1.0);
    m_extrudeDirectionZ->setSingleStep(0.1);
    m_extrudeDirectionZ->setValue(1.0);
    connect(m_extrudeDirectionZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EditPreviewPanel::onParameterChanged);
    extrudeLayout->addWidget(m_extrudeDirectionZ, 3, 1);
    
    // Scale parameters
    QWidget* scaleWidget = new QWidget();
    QGridLayout* scaleLayout = new QGridLayout(scaleWidget);
    
    scaleLayout->addWidget(new QLabel("Scale X:"), 0, 0);
    m_scaleFactorX = new QDoubleSpinBox();
    m_scaleFactorX->setRange(0.1, 5.0);
    m_scaleFactorX->setSingleStep(0.1);
    m_scaleFactorX->setValue(1.0);
    connect(m_scaleFactorX, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EditPreviewPanel::onParameterChanged);
    scaleLayout->addWidget(m_scaleFactorX, 0, 1);
    
    scaleLayout->addWidget(new QLabel("Scale Y:"), 1, 0);
    m_scaleFactorY = new QDoubleSpinBox();
    m_scaleFactorY->setRange(0.1, 5.0);
    m_scaleFactorY->setSingleStep(0.1);
    m_scaleFactorY->setValue(1.0);
    connect(m_scaleFactorY, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EditPreviewPanel::onParameterChanged);
    scaleLayout->addWidget(m_scaleFactorY, 1, 1);
    
    scaleLayout->addWidget(new QLabel("Scale Z:"), 2, 0);
    m_scaleFactorZ = new QDoubleSpinBox();
    m_scaleFactorZ->setRange(0.1, 5.0);
    m_scaleFactorZ->setSingleStep(0.1);
    m_scaleFactorZ->setValue(1.0);
    connect(m_scaleFactorZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EditPreviewPanel::onParameterChanged);
    scaleLayout->addWidget(m_scaleFactorZ, 2, 1);
    
    // Smooth parameters
    QWidget* smoothWidget = new QWidget();
    QGridLayout* smoothLayout = new QGridLayout(smoothWidget);
    
    smoothLayout->addWidget(new QLabel("Intensity:"), 0, 0);
    m_smoothIntensity = new QDoubleSpinBox();
    m_smoothIntensity->setRange(0.1, 3.0);
    m_smoothIntensity->setSingleStep(0.1);
    m_smoothIntensity->setValue(1.0);
    connect(m_smoothIntensity, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &EditPreviewPanel::onParameterChanged);
    smoothLayout->addWidget(m_smoothIntensity, 0, 1);
    
    // Store widgets for show/hide
    subdivisionWidget->setObjectName("subdivision");
    extrudeWidget->setObjectName("extrude");
    scaleWidget->setObjectName("scale");
    smoothWidget->setObjectName("smooth");
    
    m_parameterLayout->addWidget(subdivisionWidget);
    m_parameterLayout->addWidget(extrudeWidget);
    m_parameterLayout->addWidget(scaleWidget);
    m_parameterLayout->addWidget(smoothWidget);
    
    // Initially hide all parameter widgets
    hideAllParameters();
    
    m_mainLayout->addWidget(m_parameterGroup);
}

void EditPreviewPanel::setupPreviewGroup() {
    m_previewGroup = new QGroupBox("Preview Controls");
    QVBoxLayout* layout = new QVBoxLayout(m_previewGroup);
    
    // Action buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_commitButton = new QPushButton("Apply Changes");
    m_commitButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; }");
    connect(m_commitButton, &QPushButton::clicked,
            this, &EditPreviewPanel::onCommitPreviewClicked);
    
    m_cancelButton = new QPushButton("Cancel");
    m_cancelButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; }");
    connect(m_cancelButton, &QPushButton::clicked,
            this, &EditPreviewPanel::onCancelPreviewClicked);
    
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->addWidget(m_cancelButton);
    
    // Real-time update checkbox
    m_realTimeUpdate = new QCheckBox("Real-time Update");
    m_realTimeUpdate->setChecked(true);
    // TODO: Connect to actual real-time update functionality
    
    layout->addLayout(buttonLayout);
    layout->addWidget(m_realTimeUpdate);
    
    m_mainLayout->addWidget(m_previewGroup);
}

void EditPreviewPanel::setupHistoryGroup() {
    m_historyGroup = new QGroupBox("History");
    QVBoxLayout* layout = new QVBoxLayout(m_historyGroup);
    
    // History buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_undoButton = new QPushButton("Undo");
    connect(m_undoButton, &QPushButton::clicked,
            this, &EditPreviewPanel::onUndoPreviewClicked);
    
    m_redoButton = new QPushButton("Redo");
    connect(m_redoButton, &QPushButton::clicked,
            this, &EditPreviewPanel::onRedoPreviewClicked);
    
    buttonLayout->addWidget(m_undoButton);
    buttonLayout->addWidget(m_redoButton);
    
    // History enabled checkbox
    m_historyEnabled = new QCheckBox("Enable History");
    connect(m_historyEnabled, &QCheckBox::toggled,
            this, &EditPreviewPanel::onHistoryEnabledChanged);
    
    layout->addLayout(buttonLayout);
    layout->addWidget(m_historyEnabled);
    
    m_mainLayout->addWidget(m_historyGroup);
}

void EditPreviewPanel::setupVisualGroup() {
    m_visualGroup = new QGroupBox("Visual Settings");
    QVBoxLayout* layout = new QVBoxLayout(m_visualGroup);
    
    // Preview opacity
    QHBoxLayout* opacityLayout = new QHBoxLayout();
    opacityLayout->addWidget(new QLabel("Preview Opacity:"));
    
    m_previewOpacitySlider = new QSlider(Qt::Horizontal);
    m_previewOpacitySlider->setRange(10, 100);
    m_previewOpacitySlider->setValue(70);
    connect(m_previewOpacitySlider, &QSlider::valueChanged,
            this, &EditPreviewPanel::onPreviewOpacityChanged);
    
    m_previewOpacityLabel = new QLabel("70%");
    m_previewOpacityLabel->setMinimumWidth(30);
    
    opacityLayout->addWidget(m_previewOpacitySlider);
    opacityLayout->addWidget(m_previewOpacityLabel);
    
    // Visual options
    m_showOriginal = new QCheckBox("Show Original");
    m_showOriginal->setChecked(true);
    connect(m_showOriginal, &QCheckBox::toggled,
            this, &EditPreviewPanel::onShowOriginalChanged);
    
    m_wireframeOverlay = new QCheckBox("Wireframe Overlay");
    m_wireframeOverlay->setChecked(true);
    connect(m_wireframeOverlay, &QCheckBox::toggled,
            this, &EditPreviewPanel::onWireframeOverlayChanged);
    
    layout->addLayout(opacityLayout);
    layout->addWidget(m_showOriginal);
    layout->addWidget(m_wireframeOverlay);
    
    m_mainLayout->addWidget(m_visualGroup);
}

void EditPreviewPanel::startPreviewForSelectedObject(Core::EditOperation operation) {
    updateSelectedEntity();
    
    if (m_selectedEntity == INVALID_ENTITY_ID || !m_previewSystem) {
        return;
    }
    
    // Update UI parameters
    updateParametersFromUI();
    
    m_previewSystem->startPreview(m_selectedEntity, operation, m_uiParams);
}

void EditPreviewPanel::refreshParameterUI() {
    if (m_previewSystem && m_previewSystem->hasActivePreview()) {
        m_uiParams = m_previewSystem->getCurrentParams();
        updateUIFromParameters();
    }
}

void EditPreviewPanel::onOperationChanged(int index) {
    if (index >= 0 && index < static_cast<int>(m_operations.size())) {
        showParametersForOperation(m_operations[index]);
    }
}

void EditPreviewPanel::onParameterChanged() {
    if (m_previewSystem && m_previewSystem->hasActivePreview() && m_realTimeUpdate->isChecked()) {
        updateParametersFromUI();
        m_previewSystem->updatePreview(m_uiParams);
    }
}

void EditPreviewPanel::onStartPreviewClicked() {
    int index = m_operationCombo->currentIndex();
    if (index >= 0 && index < static_cast<int>(m_operations.size())) {
        startPreviewForSelectedObject(m_operations[index]);
    }
}

void EditPreviewPanel::onCommitPreviewClicked() {
    if (m_previewSystem) {
        m_previewSystem->commitPreview();
    }
}

void EditPreviewPanel::onCancelPreviewClicked() {
    if (m_previewSystem) {
        m_previewSystem->cancelPreview();
    }
}

void EditPreviewPanel::onUndoPreviewClicked() {
    if (m_previewSystem) {
        m_previewSystem->undoPreview();
        refreshParameterUI();
    }
}

void EditPreviewPanel::onRedoPreviewClicked() {
    if (m_previewSystem) {
        m_previewSystem->redoPreview();
        refreshParameterUI();
    }
}

void EditPreviewPanel::onHistoryEnabledChanged(bool enabled) {
    if (m_previewSystem) {
        m_previewSystem->enablePreviewHistory(enabled);
        updateHistoryControlsState();
    }
}

void EditPreviewPanel::onPreviewOpacityChanged(int value) {
    float opacity = value / 100.0f;
    m_previewOpacityLabel->setText(QString("%1%").arg(value));
    
    if (m_previewSystem) {
        m_previewSystem->setPreviewOpacity(opacity);
    }
}

void EditPreviewPanel::onShowOriginalChanged(bool show) {
    if (m_previewSystem) {
        m_previewSystem->setShowOriginal(show);
    }
}

void EditPreviewPanel::onWireframeOverlayChanged(bool enabled) {
    if (m_previewSystem) {
        m_previewSystem->setWireframeOverlay(enabled);
    }
}

void EditPreviewPanel::showParametersForOperation(Core::EditOperation operation) {
    hideAllParameters();
    
    QWidget* widgetToShow = nullptr;
    
    switch (operation) {
        case Core::EditOperation::Subdivision:
            widgetToShow = m_parameterGroup->findChild<QWidget*>("subdivision");
            break;
        case Core::EditOperation::Extrude:
            widgetToShow = m_parameterGroup->findChild<QWidget*>("extrude");
            break;
        case Core::EditOperation::Scale:
            widgetToShow = m_parameterGroup->findChild<QWidget*>("scale");
            break;
        case Core::EditOperation::Smooth:
            widgetToShow = m_parameterGroup->findChild<QWidget*>("smooth");
            break;
        default:
            break;
    }
    
    if (widgetToShow) {
        widgetToShow->setVisible(true);
    }
}

void EditPreviewPanel::hideAllParameters() {
    auto children = m_parameterGroup->findChildren<QWidget*>();
    for (auto* child : children) {
        if (child->objectName().contains("subdivision") ||
            child->objectName().contains("extrude") ||
            child->objectName().contains("scale") ||
            child->objectName().contains("smooth")) {
            child->setVisible(false);
        }
    }
}

void EditPreviewPanel::updateParametersFromUI() {
    // Update subdivision parameters
    m_uiParams.subdivisionLevels = m_subdivisionLevels->value();
    m_uiParams.smoothSubdivision = m_smoothSubdivision->isChecked();
    
    // Update extrude parameters
    m_uiParams.extrudeDistance = m_extrudeDistance->value();
    m_uiParams.extrudeDirection = glm::vec3(
        m_extrudeDirectionX->value(),
        m_extrudeDirectionY->value(),
        m_extrudeDirectionZ->value()
    );
    
    // Update scale parameters
    m_uiParams.scaleFactors = glm::vec3(
        m_scaleFactorX->value(),
        m_scaleFactorY->value(),
        m_scaleFactorZ->value()
    );
    
    // Update smooth parameters
    m_uiParams.intensity = m_smoothIntensity->value();
}

void EditPreviewPanel::updateUIFromParameters() {
    // Update subdivision UI
    m_subdivisionLevels->setValue(m_uiParams.subdivisionLevels);
    m_smoothSubdivision->setChecked(m_uiParams.smoothSubdivision);
    
    // Update extrude UI
    m_extrudeDistance->setValue(m_uiParams.extrudeDistance);
    m_extrudeDirectionX->setValue(m_uiParams.extrudeDirection.x);
    m_extrudeDirectionY->setValue(m_uiParams.extrudeDirection.y);
    m_extrudeDirectionZ->setValue(m_uiParams.extrudeDirection.z);
    
    // Update scale UI
    m_scaleFactorX->setValue(m_uiParams.scaleFactors.x);
    m_scaleFactorY->setValue(m_uiParams.scaleFactors.y);
    m_scaleFactorZ->setValue(m_uiParams.scaleFactors.z);
    
    // Update smooth UI
    m_smoothIntensity->setValue(m_uiParams.intensity);
}

void EditPreviewPanel::updateSelectedEntity() {
    // TODO: Get selected entity from selection manager
    // For now, we'll use a placeholder
    // m_selectedEntity = SelectionManager::getInstance().getSelectedEntity();
}

void EditPreviewPanel::updatePreviewControlsState() {
    bool hasActivePreview = m_previewSystem && m_previewSystem->hasActivePreview();
    bool hasValidSelection = (m_selectedEntity != INVALID_ENTITY_ID);
    
    // Enable/disable start preview button
    m_startPreviewButton->setEnabled(hasValidSelection && !hasActivePreview);
    
    // Enable/disable preview action buttons
    m_commitButton->setEnabled(hasActivePreview);
    m_cancelButton->setEnabled(hasActivePreview);
    
    // Enable/disable parameter controls
    m_parameterGroup->setEnabled(hasActivePreview || hasValidSelection);
}

void EditPreviewPanel::updateHistoryControlsState() {
    if (!m_previewSystem) {
        m_undoButton->setEnabled(false);
        m_redoButton->setEnabled(false);
        return;
    }
    
    m_undoButton->setEnabled(m_previewSystem->canUndoPreview());
    m_redoButton->setEnabled(m_previewSystem->canRedoPreview());
}

void EditPreviewPanel::onPreviewStarted(EntityID entityId, Core::EditOperation operation) {
    std::cout << "Preview started for entity " << entityId << std::endl;
    refreshParameterUI();
    updatePreviewControlsState();
    updateHistoryControlsState();
    
    // Update operation combo to match the started operation
    for (size_t i = 0; i < m_operations.size(); ++i) {
        if (m_operations[i] == operation) {
            m_operationCombo->setCurrentIndex(static_cast<int>(i));
            showParametersForOperation(operation);
            break;
        }
    }
}

void EditPreviewPanel::onPreviewUpdated(EntityID entityId, Core::EditOperation operation) {
    updateHistoryControlsState();
}

void EditPreviewPanel::onPreviewFinished(EntityID entityId, Core::EditOperation operation, bool committed) {
    std::cout << "Preview " << (committed ? "committed" : "cancelled") << " for entity " << entityId << std::endl;
    updatePreviewControlsState();
    updateHistoryControlsState();
    hideAllParameters();
}

} // namespace UI
} // namespace RudeBase3D

#include "edit_preview_panel.moc"
