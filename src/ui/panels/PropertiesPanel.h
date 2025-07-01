#pragma once

#include "Common.h"
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
#include <QColorDialog>
#include <memory>

// Forward declarations
class Scene;

class PropertiesPanel : public QWidget {
    Q_OBJECT

public:
    explicit PropertiesPanel(QWidget* parent = nullptr);
    ~PropertiesPanel() = default;

    void setScene(std::shared_ptr<Scene> scene);
    std::shared_ptr<Scene> getScene() const { return m_scene; }
    
    void refreshProperties();
    void setSelectedObject(SceneObjectPtr object);

private slots:
    void onNameChanged();
    void onVisibilityChanged(bool visible);
    
    // Transform slots
    void onPositionChanged();
    void onRotationChanged();
    void onScaleChanged();
    void onResetTransform();
    
    // Material slots
    void onDiffuseColorChanged();
    void onSpecularColorChanged();
    void onAmbientColorChanged();
    void onShininessChanged(double value);
    void onMetallicChanged(double value);
    void onRoughnessChanged(double value);
    void onWireframeColorChanged();
    void onWireframeWidthChanged(double value);
    void onMaterialPresetChanged(int index);

private:
    std::shared_ptr<Scene> m_scene;
    SceneObjectPtr m_selectedObject;
    
    QVBoxLayout* m_mainLayout;
    
    // Object properties group
    QGroupBox* m_objectGroup;
    QLineEdit* m_nameEdit;
    QCheckBox* m_visibleCheckBox;
    
    // Transform group
    QGroupBox* m_transformGroup;
    QDoubleSpinBox* m_positionX;
    QDoubleSpinBox* m_positionY;
    QDoubleSpinBox* m_positionZ;
    QDoubleSpinBox* m_rotationX;
    QDoubleSpinBox* m_rotationY;
    QDoubleSpinBox* m_rotationZ;
    QDoubleSpinBox* m_scaleX;
    QDoubleSpinBox* m_scaleY;
    QDoubleSpinBox* m_scaleZ;
    QPushButton* m_resetTransformButton;
    
    // Material group
    QGroupBox* m_materialGroup;
    QPushButton* m_diffuseColorButton;
    QPushButton* m_specularColorButton;
    QPushButton* m_ambientColorButton;
    QDoubleSpinBox* m_shininessSpinBox;
    QDoubleSpinBox* m_metallicSpinBox;
    QDoubleSpinBox* m_roughnessSpinBox;
    QPushButton* m_wireframeColorButton;
    QDoubleSpinBox* m_wireframeWidthSpinBox;
    QComboBox* m_materialPresetCombo;
    
    bool m_updateInProgress;
    
    void setupUI();
    void setupObjectGroup();
    void setupTransformGroup();
    void setupMaterialGroup();
    void connectSignals();
    
    void updateObjectProperties();
    void updateTransformProperties();
    void updateMaterialProperties();
    
    void setColorButton(QPushButton* button, const QVector4D& color);
    QVector4D getColorFromButton(QPushButton* button);
    void openColorDialog(QPushButton* button);
    
    QDoubleSpinBox* createDoubleSpinBox(double min = -999999.0, double max = 999999.0, 
                                       double step = 0.1, int decimals = 3);
};
