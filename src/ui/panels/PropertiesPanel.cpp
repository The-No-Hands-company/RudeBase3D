#include "PropertiesPanel.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Material.h"
#include <QColorDialog>

PropertiesPanel::PropertiesPanel(QWidget* parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_updateInProgress(false)
{
    setupUI();
    connectSignals();
}

void PropertiesPanel::setScene(std::shared_ptr<Scene> scene)
{
    // Disconnect from previous scene
    if (m_scene) {
        disconnect(m_scene.get(), nullptr, this, nullptr);
    }
    
    m_scene = scene;
    
    // Connect to new scene
    if (m_scene) {
        connect(m_scene.get(), &Scene::selectionChanged, this, &PropertiesPanel::setSelectedObject);
    }
    
    setSelectedObject(m_scene ? m_scene->getSelectedObject() : nullptr);
}

void PropertiesPanel::refreshProperties()
{
    if (m_updateInProgress) {
        return;
    }
    
    updateObjectProperties();
    updateTransformProperties();
    updateMaterialProperties();
}

void PropertiesPanel::setSelectedObject(SceneObjectPtr object)
{
    m_selectedObject = object;
    
    bool hasSelection = (object != nullptr);
    setEnabled(hasSelection);
    
    if (hasSelection) {
        refreshProperties();
    }
}

void PropertiesPanel::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    setupObjectGroup();
    setupTransformGroup();
    setupMaterialGroup();
    
    m_mainLayout->addStretch();
}

void PropertiesPanel::setupObjectGroup()
{
    m_objectGroup = new QGroupBox("Object", this);
    auto layout = new QGridLayout(m_objectGroup);
    
    // Name
    layout->addWidget(new QLabel("Name:"), 0, 0);
    m_nameEdit = new QLineEdit(m_objectGroup);
    layout->addWidget(m_nameEdit, 0, 1);
    
    // Visibility
    m_visibleCheckBox = new QCheckBox("Visible", m_objectGroup);
    layout->addWidget(m_visibleCheckBox, 1, 0, 1, 2);
    
    m_mainLayout->addWidget(m_objectGroup);
}

void PropertiesPanel::setupTransformGroup()
{
    m_transformGroup = new QGroupBox("Transform", this);
    auto layout = new QGridLayout(m_transformGroup);
    
    // Position
    layout->addWidget(new QLabel("Position:"), 0, 0);
    m_positionX = createDoubleSpinBox();
    m_positionY = createDoubleSpinBox();
    m_positionZ = createDoubleSpinBox();
    layout->addWidget(new QLabel("X:"), 0, 1);
    layout->addWidget(m_positionX, 0, 2);
    layout->addWidget(new QLabel("Y:"), 0, 3);
    layout->addWidget(m_positionY, 0, 4);
    layout->addWidget(new QLabel("Z:"), 0, 5);
    layout->addWidget(m_positionZ, 0, 6);
    
    // Rotation
    layout->addWidget(new QLabel("Rotation:"), 1, 0);
    m_rotationX = createDoubleSpinBox(-360, 360, 1, 1);
    m_rotationY = createDoubleSpinBox(-360, 360, 1, 1);
    m_rotationZ = createDoubleSpinBox(-360, 360, 1, 1);
    layout->addWidget(new QLabel("X:"), 1, 1);
    layout->addWidget(m_rotationX, 1, 2);
    layout->addWidget(new QLabel("Y:"), 1, 3);
    layout->addWidget(m_rotationY, 1, 4);
    layout->addWidget(new QLabel("Z:"), 1, 5);
    layout->addWidget(m_rotationZ, 1, 6);
    
    // Scale
    layout->addWidget(new QLabel("Scale:"), 2, 0);
    m_scaleX = createDoubleSpinBox(0.001, 1000, 0.1, 3);
    m_scaleY = createDoubleSpinBox(0.001, 1000, 0.1, 3);
    m_scaleZ = createDoubleSpinBox(0.001, 1000, 0.1, 3);
    layout->addWidget(new QLabel("X:"), 2, 1);
    layout->addWidget(m_scaleX, 2, 2);
    layout->addWidget(new QLabel("Y:"), 2, 3);
    layout->addWidget(m_scaleY, 2, 4);
    layout->addWidget(new QLabel("Z:"), 2, 5);
    layout->addWidget(m_scaleZ, 2, 6);
    
    // Reset button
    m_resetTransformButton = new QPushButton("Reset Transform", m_transformGroup);
    layout->addWidget(m_resetTransformButton, 3, 0, 1, 7);
    
    m_mainLayout->addWidget(m_transformGroup);
}

void PropertiesPanel::setupMaterialGroup()
{
    m_materialGroup = new QGroupBox("Material", this);
    auto layout = new QGridLayout(m_materialGroup);
    
    // Material preset
    layout->addWidget(new QLabel("Preset:"), 0, 0);
    m_materialPresetCombo = new QComboBox(m_materialGroup);
    m_materialPresetCombo->addItems({"Default", "Metal", "Plastic", "Glass"});
    layout->addWidget(m_materialPresetCombo, 0, 1, 1, 2);
    
    // Diffuse color
    layout->addWidget(new QLabel("Diffuse:"), 1, 0);
    m_diffuseColorButton = new QPushButton(m_materialGroup);
    m_diffuseColorButton->setMinimumHeight(30);
    layout->addWidget(m_diffuseColorButton, 1, 1, 1, 2);
    
    // Specular color
    layout->addWidget(new QLabel("Specular:"), 2, 0);
    m_specularColorButton = new QPushButton(m_materialGroup);
    m_specularColorButton->setMinimumHeight(30);
    layout->addWidget(m_specularColorButton, 2, 1, 1, 2);
    
    // Ambient color
    layout->addWidget(new QLabel("Ambient:"), 3, 0);
    m_ambientColorButton = new QPushButton(m_materialGroup);
    m_ambientColorButton->setMinimumHeight(30);
    layout->addWidget(m_ambientColorButton, 3, 1, 1, 2);
    
    // Shininess
    layout->addWidget(new QLabel("Shininess:"), 4, 0);
    m_shininessSpinBox = createDoubleSpinBox(1, 256, 1, 1);
    layout->addWidget(m_shininessSpinBox, 4, 1, 1, 2);
    
    // Metallic
    layout->addWidget(new QLabel("Metallic:"), 5, 0);
    m_metallicSpinBox = createDoubleSpinBox(0, 1, 0.1, 2);
    layout->addWidget(m_metallicSpinBox, 5, 1, 1, 2);
    
    // Roughness
    layout->addWidget(new QLabel("Roughness:"), 6, 0);
    m_roughnessSpinBox = createDoubleSpinBox(0, 1, 0.1, 2);
    layout->addWidget(m_roughnessSpinBox, 6, 1, 1, 2);
    
    // Wireframe color
    layout->addWidget(new QLabel("Wireframe:"), 7, 0);
    m_wireframeColorButton = new QPushButton(m_materialGroup);
    m_wireframeColorButton->setMinimumHeight(30);
    layout->addWidget(m_wireframeColorButton, 7, 1, 1, 2);
    
    // Wireframe width
    layout->addWidget(new QLabel("Wire Width:"), 8, 0);
    m_wireframeWidthSpinBox = createDoubleSpinBox(0.1, 10, 0.1, 1);
    layout->addWidget(m_wireframeWidthSpinBox, 8, 1, 1, 2);
    
    m_mainLayout->addWidget(m_materialGroup);
}

void PropertiesPanel::connectSignals()
{
    // Object properties
    connect(m_nameEdit, &QLineEdit::textChanged, this, &PropertiesPanel::onNameChanged);
    connect(m_visibleCheckBox, &QCheckBox::toggled, this, &PropertiesPanel::onVisibilityChanged);
    
    // Transform properties
    connect(m_positionX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onPositionChanged);
    connect(m_positionY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onPositionChanged);
    connect(m_positionZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onPositionChanged);
    
    connect(m_rotationX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onRotationChanged);
    connect(m_rotationY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onRotationChanged);
    connect(m_rotationZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onRotationChanged);
    
    connect(m_scaleX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onScaleChanged);
    connect(m_scaleY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onScaleChanged);
    connect(m_scaleZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onScaleChanged);
    
    connect(m_resetTransformButton, &QPushButton::clicked, this, &PropertiesPanel::onResetTransform);
    
    // Material properties
    connect(m_materialPresetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &PropertiesPanel::onMaterialPresetChanged);
    connect(m_diffuseColorButton, &QPushButton::clicked, this, &PropertiesPanel::onDiffuseColorChanged);
    connect(m_specularColorButton, &QPushButton::clicked, this, &PropertiesPanel::onSpecularColorChanged);
    connect(m_ambientColorButton, &QPushButton::clicked, this, &PropertiesPanel::onAmbientColorChanged);
    connect(m_wireframeColorButton, &QPushButton::clicked, this, &PropertiesPanel::onWireframeColorChanged);
    
    connect(m_shininessSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, &PropertiesPanel::onShininessChanged);
    connect(m_metallicSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, &PropertiesPanel::onMetallicChanged);
    connect(m_roughnessSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, &PropertiesPanel::onRoughnessChanged);
    connect(m_wireframeWidthSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, &PropertiesPanel::onWireframeWidthChanged);
}

void PropertiesPanel::updateObjectProperties()
{
    if (!m_selectedObject) {
        return;
    }
    
    m_updateInProgress = true;
    
    m_nameEdit->setText(m_selectedObject->getName());
    m_visibleCheckBox->setChecked(m_selectedObject->isVisible());
    
    m_updateInProgress = false;
}

void PropertiesPanel::updateTransformProperties()
{
    if (!m_selectedObject) {
        return;
    }
    
    m_updateInProgress = true;
    
    const Transform& transform = m_selectedObject->getTransform();
    
    // Position
    QVector3D position = transform.getPosition();
    m_positionX->setValue(position.x());
    m_positionY->setValue(position.y());
    m_positionZ->setValue(position.z());
    
    // Rotation (Euler angles)
    QVector3D rotation = transform.getEulerAngles();
    m_rotationX->setValue(rotation.x());
    m_rotationY->setValue(rotation.y());
    m_rotationZ->setValue(rotation.z());
    
    // Scale
    QVector3D scale = transform.getScale();
    m_scaleX->setValue(scale.x());
    m_scaleY->setValue(scale.y());
    m_scaleZ->setValue(scale.z());
    
    m_updateInProgress = false;
}

void PropertiesPanel::updateMaterialProperties()
{
    if (!m_selectedObject || !m_selectedObject->getMaterial()) {
        return;
    }
    
    m_updateInProgress = true;
    
    auto material = m_selectedObject->getMaterial();
    
    // Set color buttons
    setColorButton(m_diffuseColorButton, material->getDiffuseColor());
    setColorButton(m_specularColorButton, material->getSpecularColor());
    setColorButton(m_ambientColorButton, material->getAmbientColor());
    setColorButton(m_wireframeColorButton, material->getWireframeColor());
    
    // Set numeric values
    m_shininessSpinBox->setValue(material->getShininess());
    m_metallicSpinBox->setValue(material->getMetallic());
    m_roughnessSpinBox->setValue(material->getRoughness());
    m_wireframeWidthSpinBox->setValue(material->getWireframeWidth());
    
    m_updateInProgress = false;
}

void PropertiesPanel::onNameChanged()
{
    if (m_updateInProgress || !m_selectedObject) {
        return;
    }
    
    m_selectedObject->setName(m_nameEdit->text());
}

void PropertiesPanel::onVisibilityChanged(bool visible)
{
    if (m_updateInProgress || !m_selectedObject) {
        return;
    }
    
    m_selectedObject->setVisible(visible);
}

void PropertiesPanel::onPositionChanged()
{
    if (m_updateInProgress || !m_selectedObject) {
        return;
    }
    
    QVector3D position(m_positionX->value(), m_positionY->value(), m_positionZ->value());
    m_selectedObject->getTransform().setPosition(position);
}

void PropertiesPanel::onRotationChanged()
{
    if (m_updateInProgress || !m_selectedObject) {
        return;
    }
    
    QVector3D rotation(m_rotationX->value(), m_rotationY->value(), m_rotationZ->value());
    m_selectedObject->getTransform().setEulerAngles(rotation);
}

void PropertiesPanel::onScaleChanged()
{
    if (m_updateInProgress || !m_selectedObject) {
        return;
    }
    
    QVector3D scale(m_scaleX->value(), m_scaleY->value(), m_scaleZ->value());
    m_selectedObject->getTransform().setScale(scale);
}

void PropertiesPanel::onResetTransform()
{
    if (!m_selectedObject) {
        return;
    }
    
    m_selectedObject->getTransform().reset();
    updateTransformProperties();
}

void PropertiesPanel::onDiffuseColorChanged()
{
    openColorDialog(m_diffuseColorButton);
    if (m_selectedObject && m_selectedObject->getMaterial()) {
        m_selectedObject->getMaterial()->setDiffuseColor(getColorFromButton(m_diffuseColorButton));
    }
}

void PropertiesPanel::onSpecularColorChanged()
{
    openColorDialog(m_specularColorButton);
    if (m_selectedObject && m_selectedObject->getMaterial()) {
        m_selectedObject->getMaterial()->setSpecularColor(getColorFromButton(m_specularColorButton));
    }
}

void PropertiesPanel::onAmbientColorChanged()
{
    openColorDialog(m_ambientColorButton);
    if (m_selectedObject && m_selectedObject->getMaterial()) {
        m_selectedObject->getMaterial()->setAmbientColor(getColorFromButton(m_ambientColorButton));
    }
}

void PropertiesPanel::onWireframeColorChanged()
{
    openColorDialog(m_wireframeColorButton);
    if (m_selectedObject && m_selectedObject->getMaterial()) {
        m_selectedObject->getMaterial()->setWireframeColor(getColorFromButton(m_wireframeColorButton));
    }
}

void PropertiesPanel::onShininessChanged(double value)
{
    if (m_updateInProgress || !m_selectedObject || !m_selectedObject->getMaterial()) {
        return;
    }
    
    m_selectedObject->getMaterial()->setShininess(value);
}

void PropertiesPanel::onMetallicChanged(double value)
{
    if (m_updateInProgress || !m_selectedObject || !m_selectedObject->getMaterial()) {
        return;
    }
    
    m_selectedObject->getMaterial()->setMetallic(value);
}

void PropertiesPanel::onRoughnessChanged(double value)
{
    if (m_updateInProgress || !m_selectedObject || !m_selectedObject->getMaterial()) {
        return;
    }
    
    m_selectedObject->getMaterial()->setRoughness(value);
}

void PropertiesPanel::onWireframeWidthChanged(double value)
{
    if (m_updateInProgress || !m_selectedObject || !m_selectedObject->getMaterial()) {
        return;
    }
    
    m_selectedObject->getMaterial()->setWireframeWidth(value);
}

void PropertiesPanel::onMaterialPresetChanged(int index)
{
    if (m_updateInProgress || !m_selectedObject) {
        return;
    }
    
    MaterialPtr material;
    switch (index) {
        case 0: material = Material::createDefault(); break;
        case 1: material = Material::createMetal(); break;
        case 2: material = Material::createPlastic(); break;
        case 3: material = Material::createGlass(); break;
        default: return;
    }
    
    m_selectedObject->setMaterial(material);
    updateMaterialProperties();
}

void PropertiesPanel::setColorButton(QPushButton* button, const QVector4D& color)
{
    QColor qcolor;
    qcolor.setRgbF(color.x(), color.y(), color.z(), color.w());
    
    QString styleSheet = QString("background-color: %1").arg(qcolor.name());
    button->setStyleSheet(styleSheet);
    button->setProperty("color", qcolor);
}

QVector4D PropertiesPanel::getColorFromButton(QPushButton* button)
{
    QColor color = button->property("color").value<QColor>();
    return QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void PropertiesPanel::openColorDialog(QPushButton* button)
{
    QColor currentColor = button->property("color").value<QColor>();
    QColor newColor = QColorDialog::getColor(currentColor, this, "Select Color");
    
    if (newColor.isValid()) {
        setColorButton(button, QVector4D(newColor.redF(), newColor.greenF(), newColor.blueF(), newColor.alphaF()));
    }
}

QDoubleSpinBox* PropertiesPanel::createDoubleSpinBox(double min, double max, double step, int decimals)
{
    auto spinBox = new QDoubleSpinBox(this);
    spinBox->setRange(min, max);
    spinBox->setSingleStep(step);
    spinBox->setDecimals(decimals);
    spinBox->setMinimumWidth(80);
    return spinBox;
}
