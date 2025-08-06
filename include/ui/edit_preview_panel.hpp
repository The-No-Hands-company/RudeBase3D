#pragma once

#include "core/edit_preview_system.hpp"
#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QSlider>
#include <memory>

namespace RudeBase3D {
namespace UI {

class Scene;

/**
 * @brief Qt-based UI Panel for Edit Preview System
 * 
 * Provides user interface controls for real-time edit previews,
 * allowing users to see changes before committing them.
 */
class EditPreviewPanel : public QDockWidget {
    Q_OBJECT

public:
    EditPreviewPanel(std::shared_ptr<Core::EditPreviewSystem> previewSystem, QWidget* parent = nullptr);
    ~EditPreviewPanel() override = default;

    // Preview management
    void startPreviewForSelectedObject(Core::EditOperation operation);
    void refreshParameterUI();
    
private slots:
    void onOperationChanged(int index);
    void onParameterChanged();
    void onStartPreviewClicked();
    void onCommitPreviewClicked();
    void onCancelPreviewClicked();
    void onUndoPreviewClicked();
    void onRedoPreviewClicked();
    void onHistoryEnabledChanged(bool enabled);
    void onPreviewOpacityChanged(int value);
    void onShowOriginalChanged(bool show);
    void onWireframeOverlayChanged(bool enabled);
    
private:
    std::shared_ptr<Core::EditPreviewSystem> m_previewSystem;
    
    // Main widgets
    QWidget* m_centralWidget;
    QVBoxLayout* m_mainLayout;
    
    // Operation selection
    QGroupBox* m_operationGroup;
    QComboBox* m_operationCombo;
    QPushButton* m_startPreviewButton;
    
    // Parameter controls
    QGroupBox* m_parameterGroup;
    QWidget* m_parameterWidget;
    QVBoxLayout* m_parameterLayout;
    
    // Subdivision parameters
    QSpinBox* m_subdivisionLevels;
    QCheckBox* m_smoothSubdivision;
    
    // Extrude parameters
    QDoubleSpinBox* m_extrudeDistance;
    QDoubleSpinBox* m_extrudeDirectionX;
    QDoubleSpinBox* m_extrudeDirectionY;
    QDoubleSpinBox* m_extrudeDirectionZ;
    
    // Bevel parameters
    QDoubleSpinBox* m_bevelWidth;
    QSpinBox* m_bevelSegments;
    
    // Scale parameters
    QDoubleSpinBox* m_scaleFactorX;
    QDoubleSpinBox* m_scaleFactorY;
    QDoubleSpinBox* m_scaleFactorZ;
    QDoubleSpinBox* m_scalePivotX;
    QDoubleSpinBox* m_scalePivotY;
    QDoubleSpinBox* m_scalePivotZ;
    
    // Smooth parameters
    QDoubleSpinBox* m_smoothIntensity;
    
    // Preview controls
    QGroupBox* m_previewGroup;
    QPushButton* m_commitButton;
    QPushButton* m_cancelButton;
    QCheckBox* m_realTimeUpdate;
    
    // History controls
    QGroupBox* m_historyGroup;
    QPushButton* m_undoButton;
    QPushButton* m_redoButton;
    QCheckBox* m_historyEnabled;
    
    // Visual settings
    QGroupBox* m_visualGroup;
    QSlider* m_previewOpacitySlider;
    QLabel* m_previewOpacityLabel;
    QCheckBox* m_showOriginal;
    QCheckBox* m_wireframeOverlay;
    
    // UI state
    Core::EditPreviewParams m_uiParams;
    std::vector<Core::EditOperation> m_operations;
    
    // Selected entity tracking
    EntityID m_selectedEntity = INVALID_ENTITY_ID;
    
    // UI setup methods
    void setupUI();
    void setupOperationGroup();
    void setupParameterGroup();
    void setupPreviewGroup();
    void setupHistoryGroup();
    void setupVisualGroup();
    
    // Parameter UI management
    void showParametersForOperation(Core::EditOperation operation);
    void hideAllParameters();
    void updateParametersFromUI();
    void updateUIFromParameters();
    
    // Utility methods
    void updateSelectedEntity();
    void updatePreviewControlsState();
    void updateHistoryControlsState();
    
    // Event handlers
    void onPreviewStarted(EntityID entityId, Core::EditOperation operation);
    void onPreviewUpdated(EntityID entityId, Core::EditOperation operation);
    void onPreviewFinished(EntityID entityId, Core::EditOperation operation, bool committed);
};

} // namespace UI
} // namespace RudeBase3D
