#include "ui/dialogs/SettingsDialog.h"
#include "../components/ThemeSelector.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QSettings>
#include <QApplication>
#include <QMessageBox>
#include <QThread>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
    , m_mainLayout(nullptr)
    , m_tabWidget(nullptr)
    , m_buttonLayout(nullptr)
    , m_appearanceTab(nullptr)
    , m_workflowTab(nullptr)
    , m_viewportTab(nullptr)
    , m_performanceTab(nullptr)
    , m_themeGroup(nullptr)
    , m_themeSelector(nullptr)
    , m_uiGroup(nullptr)
    , m_uiScaleSlider(nullptr)
    , m_uiScaleLabel(nullptr)
    , m_animationsEnabled(nullptr)
    , m_compactMode(nullptr)
    , m_showTooltips(nullptr)
    , m_autoSaveGroup(nullptr)
    , m_autoSaveEnabled(nullptr)
    , m_autoSaveInterval(nullptr)
    , m_projectGroup(nullptr)
    , m_rememberLastProject(nullptr)
    , m_confirmDeletes(nullptr)
    , m_undoGroup(nullptr)
    , m_undoLimitEnabled(nullptr)
    , m_undoLimit(nullptr)
    , m_renderingGroup(nullptr)
    , m_antiAliasing(nullptr)
    , m_vsyncEnabled(nullptr)
    , m_cameraGroup(nullptr)
    , m_fov(nullptr)
    , m_fovLabel(nullptr)
    , m_orbitAroundSelection(nullptr)
    , m_invertY(nullptr)
    , m_mouseWheelSensitivity(nullptr)
    , m_memoryGroup(nullptr)
    , m_cacheSize(nullptr)
    , m_cacheSizeLabel(nullptr)
    , m_cpuGroup(nullptr)
    , m_maxThreads(nullptr)
    , m_backgroundProcessing(nullptr)
    , m_hardwareAcceleration(nullptr)
    , m_applyButton(nullptr)
    , m_okButton(nullptr)
    , m_cancelButton(nullptr)
    , m_resetButton(nullptr)
    , m_settingsChanged(false)
{
    setWindowTitle("RudeBase3D Settings & Preferences");
    setModal(true);
    resize(800, 600);
    
    setupUI();
    connectSignals();
    loadSettings();
}

SettingsDialog::~SettingsDialog()
{
    // Auto cleanup with Qt parent-child system
}

void SettingsDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // Create tab widget
    m_tabWidget = new QTabWidget(this);
    m_mainLayout->addWidget(m_tabWidget);
    
    // Setup all tabs
    setupAppearanceTab();
    setupWorkflowTab();
    setupViewportTab();
    setupPerformanceTab();
    
    // Create buttons
    createButtons();
}

void SettingsDialog::setupAppearanceTab()
{
    m_appearanceTab = new QWidget();
    m_tabWidget->addTab(m_appearanceTab, "Appearance & Theme");
    
    auto layout = new QVBoxLayout(m_appearanceTab);
    
    // Theme Group
    m_themeGroup = new QGroupBox("Theme Selection", m_appearanceTab);
    auto themeLayout = new QVBoxLayout(m_themeGroup);
    
    auto themeLabel = new QLabel("Choose your preferred theme for the best visual experience:");
    themeLabel->setWordWrap(true);
    themeLayout->addWidget(themeLabel);
    
    m_themeSelector = new rudebase3d::ui::ThemeSelector(m_appearanceTab);
    themeLayout->addWidget(m_themeSelector);
    
    layout->addWidget(m_themeGroup);
    
    // UI Settings Group
    m_uiGroup = new QGroupBox("Interface Settings", m_appearanceTab);
    auto uiLayout = new QFormLayout(m_uiGroup);
    
    // UI Scale
    auto scaleLayout = new QHBoxLayout();
    m_uiScaleSlider = new QSlider(Qt::Horizontal, m_uiGroup);
    m_uiScaleSlider->setRange(80, 200);
    m_uiScaleSlider->setValue(100);
    m_uiScaleSlider->setTickPosition(QSlider::TicksBelow);
    m_uiScaleSlider->setTickInterval(20);
    
    m_uiScaleLabel = new QLabel("100%", m_uiGroup);
    scaleLayout->addWidget(m_uiScaleSlider);
    scaleLayout->addWidget(m_uiScaleLabel);
    
    uiLayout->addRow("UI Scale:", scaleLayout);
    
    // UI Options
    m_animationsEnabled = new QCheckBox("Enable UI animations", m_uiGroup);
    m_animationsEnabled->setChecked(true);
    uiLayout->addRow("", m_animationsEnabled);
    
    m_compactMode = new QCheckBox("Compact interface mode", m_uiGroup);
    uiLayout->addRow("", m_compactMode);
    
    m_showTooltips = new QCheckBox("Show helpful tooltips", m_uiGroup);
    m_showTooltips->setChecked(true);
    uiLayout->addRow("", m_showTooltips);
    
    layout->addWidget(m_uiGroup);
    layout->addStretch();
}

void SettingsDialog::setupWorkflowTab()
{
    m_workflowTab = new QWidget();
    m_tabWidget->addTab(m_workflowTab, "Workflow & Productivity");
    
    auto layout = new QVBoxLayout(m_workflowTab);
    
    // Auto-save Group
    m_autoSaveGroup = new QGroupBox("Auto-Save & Backup", m_workflowTab);
    auto autoSaveLayout = new QFormLayout(m_autoSaveGroup);
    
    m_autoSaveEnabled = new QCheckBox("Enable automatic saving", m_autoSaveGroup);
    m_autoSaveEnabled->setChecked(true);
    autoSaveLayout->addRow("", m_autoSaveEnabled);
    
    m_autoSaveInterval = new QSpinBox(m_autoSaveGroup);
    m_autoSaveInterval->setRange(1, 60);
    m_autoSaveInterval->setValue(5);
    m_autoSaveInterval->setSuffix(" minutes");
    autoSaveLayout->addRow("Save interval:", m_autoSaveInterval);
    
    layout->addWidget(m_autoSaveGroup);
    
    // Project Management Group
    m_projectGroup = new QGroupBox("Project Management", m_workflowTab);
    auto projectLayout = new QVBoxLayout(m_projectGroup);
    
    m_rememberLastProject = new QCheckBox("Remember and reopen last project on startup", m_projectGroup);
    m_rememberLastProject->setChecked(true);
    projectLayout->addWidget(m_rememberLastProject);
    
    m_confirmDeletes = new QCheckBox("Confirm before deleting objects", m_projectGroup);
    m_confirmDeletes->setChecked(true);
    projectLayout->addWidget(m_confirmDeletes);
    
    layout->addWidget(m_projectGroup);
    
    // Undo/Redo Group
    m_undoGroup = new QGroupBox("Undo/Redo System", m_workflowTab);
    auto undoLayout = new QFormLayout(m_undoGroup);
    
    m_undoLimitEnabled = new QCheckBox("Limit undo history (recommended for performance)", m_undoGroup);
    m_undoLimitEnabled->setChecked(true);
    undoLayout->addRow("", m_undoLimitEnabled);
    
    m_undoLimit = new QSpinBox(m_undoGroup);
    m_undoLimit->setRange(10, 1000);
    m_undoLimit->setValue(100);
    undoLayout->addRow("Maximum undo steps:", m_undoLimit);
    
    layout->addWidget(m_undoGroup);
    layout->addStretch();
}

void SettingsDialog::setupViewportTab()
{
    m_viewportTab = new QWidget();
    m_tabWidget->addTab(m_viewportTab, "Viewport & 3D Navigation");
    
    auto layout = new QVBoxLayout(m_viewportTab);
    
    // Rendering Group
    m_renderingGroup = new QGroupBox("Rendering Quality", m_viewportTab);
    auto renderLayout = new QFormLayout(m_renderingGroup);
    
    m_antiAliasing = new QComboBox(m_renderingGroup);
    m_antiAliasing->addItems({"Off", "2x MSAA", "4x MSAA", "8x MSAA", "16x MSAA"});
    m_antiAliasing->setCurrentIndex(2);
    renderLayout->addRow("Anti-aliasing:", m_antiAliasing);
    
    m_vsyncEnabled = new QCheckBox("Enable V-Sync (reduces tearing)", m_renderingGroup);
    m_vsyncEnabled->setChecked(true);
    renderLayout->addRow("", m_vsyncEnabled);
    
    layout->addWidget(m_renderingGroup);
    
    // Camera Group
    m_cameraGroup = new QGroupBox("Camera Settings", m_viewportTab);
    auto cameraLayout = new QFormLayout(m_cameraGroup);
    
    auto fovLayout = new QHBoxLayout();
    m_fov = new QSlider(Qt::Horizontal, m_cameraGroup);
    m_fov->setRange(15, 120);
    m_fov->setValue(60);
    m_fovLabel = new QLabel("60°", m_cameraGroup);
    fovLayout->addWidget(m_fov);
    fovLayout->addWidget(m_fovLabel);
    cameraLayout->addRow("Field of View:", fovLayout);
    
    m_orbitAroundSelection = new QCheckBox("Orbit camera around selected object", m_cameraGroup);
    m_orbitAroundSelection->setChecked(true);
    cameraLayout->addRow("", m_orbitAroundSelection);
    
    m_invertY = new QCheckBox("Invert Y-axis for camera rotation", m_cameraGroup);
    cameraLayout->addRow("", m_invertY);
    
    m_mouseWheelSensitivity = new QSlider(Qt::Horizontal, m_cameraGroup);
    m_mouseWheelSensitivity->setRange(1, 10);
    m_mouseWheelSensitivity->setValue(5);
    cameraLayout->addRow("Mouse wheel sensitivity:", m_mouseWheelSensitivity);
    
    layout->addWidget(m_cameraGroup);
    layout->addStretch();
}

void SettingsDialog::setupPerformanceTab()
{
    m_performanceTab = new QWidget();
    m_tabWidget->addTab(m_performanceTab, "Performance & Resources");
    
    auto layout = new QVBoxLayout(m_performanceTab);
    
    // Memory Group
    m_memoryGroup = new QGroupBox("Memory Management", m_performanceTab);
    auto memoryLayout = new QFormLayout(m_memoryGroup);
    
    auto cacheLayout = new QHBoxLayout();
    m_cacheSize = new QSlider(Qt::Horizontal, m_memoryGroup);
    m_cacheSize->setRange(128, 4096);
    m_cacheSize->setValue(512);
    m_cacheSizeLabel = new QLabel("512 MB", m_memoryGroup);
    cacheLayout->addWidget(m_cacheSize);
    cacheLayout->addWidget(m_cacheSizeLabel);
    memoryLayout->addRow("Cache Size:", cacheLayout);
    
    layout->addWidget(m_memoryGroup);
    
    // CPU Group
    m_cpuGroup = new QGroupBox("CPU Utilization", m_performanceTab);
    auto cpuLayout = new QFormLayout(m_cpuGroup);
    
    m_maxThreads = new QSpinBox(m_cpuGroup);
    m_maxThreads->setRange(1, QThread::idealThreadCount());
    m_maxThreads->setValue(QThread::idealThreadCount());
    cpuLayout->addRow("Max Threads:", m_maxThreads);
    
    m_backgroundProcessing = new QCheckBox("Enable background processing", m_cpuGroup);
    m_backgroundProcessing->setChecked(true);
    cpuLayout->addRow("", m_backgroundProcessing);
    
    m_hardwareAcceleration = new QCheckBox("Enable hardware acceleration (recommended)", m_cpuGroup);
    m_hardwareAcceleration->setChecked(true);
    cpuLayout->addRow("", m_hardwareAcceleration);
    
    layout->addWidget(m_cpuGroup);
    layout->addStretch();
}

void SettingsDialog::createButtons()
{
    m_buttonLayout = new QHBoxLayout();
    
    m_resetButton = new QPushButton("Reset Tab", this);
    m_applyButton = new QPushButton("Apply", this);
    m_okButton = new QPushButton("OK", this);
    m_cancelButton = new QPushButton("Cancel", this);
    
    m_buttonLayout->addWidget(m_resetButton);
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_applyButton);
    m_buttonLayout->addWidget(m_okButton);
    m_buttonLayout->addWidget(m_cancelButton);
    
    m_mainLayout->addLayout(m_buttonLayout);
}

void SettingsDialog::connectSignals()
{
    // Button connections
    connect(m_applyButton, &QPushButton::clicked, this, &SettingsDialog::applySettings);
    connect(m_okButton, &QPushButton::clicked, [this]() {
        applySettings();
        accept();
    });
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_resetButton, &QPushButton::clicked, this, &SettingsDialog::resetToDefaults);
    
    // Theme change
    if (m_themeSelector) {
        connect(m_themeSelector, &rudebase3d::ui::ThemeSelector::themeChanged,
                this, &SettingsDialog::onThemeChanged);
    }
    
    // Settings change tracking
    auto trackChanges = [this]() { onSettingChanged(); };
    
    // UI Scale slider
    connect(m_uiScaleSlider, &QSlider::valueChanged, [this](int value) {
        m_uiScaleLabel->setText(QString("%1%").arg(value));
        onSettingChanged();
    });
    
    // FOV slider
    connect(m_fov, &QSlider::valueChanged, [this](int value) {
        m_fovLabel->setText(QString("%1°").arg(value));
        onSettingChanged();
    });
    
    // Cache size slider
    connect(m_cacheSize, &QSlider::valueChanged, [this](int value) {
        m_cacheSizeLabel->setText(QString("%1 MB").arg(value));
        onSettingChanged();
    });
    
    // Connect all checkboxes and spinboxes to change tracking
    connect(m_animationsEnabled, &QCheckBox::toggled, trackChanges);
    connect(m_compactMode, &QCheckBox::toggled, trackChanges);
    connect(m_showTooltips, &QCheckBox::toggled, trackChanges);
    connect(m_autoSaveEnabled, &QCheckBox::toggled, trackChanges);
    connect(m_rememberLastProject, &QCheckBox::toggled, trackChanges);
    connect(m_confirmDeletes, &QCheckBox::toggled, trackChanges);
    connect(m_undoLimitEnabled, &QCheckBox::toggled, trackChanges);
    connect(m_vsyncEnabled, &QCheckBox::toggled, trackChanges);
    connect(m_orbitAroundSelection, &QCheckBox::toggled, trackChanges);
    connect(m_invertY, &QCheckBox::toggled, trackChanges);
    connect(m_backgroundProcessing, &QCheckBox::toggled, trackChanges);
    connect(m_hardwareAcceleration, &QCheckBox::toggled, trackChanges);
}

void SettingsDialog::applySettings()
{
    saveSettings();
    m_settingsChanged = false;
    
    QMessageBox::information(this, "Settings Applied", 
                           "Your settings have been successfully applied.\n"
                           "Some changes may require a restart to take full effect.");
}

void SettingsDialog::resetToDefaults()
{
    int currentTab = m_tabWidget->currentIndex();
    QString tabName = m_tabWidget->tabText(currentTab);
    
    auto reply = QMessageBox::question(this, "Reset Settings",
                                      QString("Reset all settings on the '%1' tab to defaults?").arg(tabName),
                                      QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        switch (currentTab) {
            case 0: // Appearance
                if (m_themeSelector) m_themeSelector->setCurrentTheme("modern-dark");
                m_uiScaleSlider->setValue(100);
                m_uiScaleLabel->setText("100%");
                m_animationsEnabled->setChecked(true);
                m_compactMode->setChecked(false);
                m_showTooltips->setChecked(true);
                break;
            case 1: // Workflow
                m_autoSaveEnabled->setChecked(true);
                m_autoSaveInterval->setValue(5);
                m_rememberLastProject->setChecked(true);
                m_confirmDeletes->setChecked(true);
                m_undoLimitEnabled->setChecked(true);
                m_undoLimit->setValue(100);
                break;
            case 2: // Viewport
                m_antiAliasing->setCurrentIndex(2);
                m_vsyncEnabled->setChecked(true);
                m_fov->setValue(60);
                m_fovLabel->setText("60°");
                m_orbitAroundSelection->setChecked(true);
                m_invertY->setChecked(false);
                m_mouseWheelSensitivity->setValue(5);
                break;
            case 3: // Performance
                m_cacheSize->setValue(512);
                m_cacheSizeLabel->setText("512 MB");
                m_maxThreads->setValue(QThread::idealThreadCount());
                m_backgroundProcessing->setChecked(true);
                m_hardwareAcceleration->setChecked(true);
                break;
        }
        m_settingsChanged = true;
    }
}

void SettingsDialog::onThemeChanged()
{
    m_settingsChanged = true;
}

void SettingsDialog::onSettingChanged()
{
    m_settingsChanged = true;
}

void SettingsDialog::loadSettings()
{
    QSettings settings;
    
    // Load appearance settings
    m_uiScaleSlider->setValue(settings.value("UI/Scale", 100).toInt());
    m_uiScaleLabel->setText(QString("%1%").arg(m_uiScaleSlider->value()));
    m_animationsEnabled->setChecked(settings.value("UI/Animations", true).toBool());
    m_compactMode->setChecked(settings.value("UI/CompactMode", false).toBool());
    m_showTooltips->setChecked(settings.value("UI/ShowTooltips", true).toBool());
    
    // Load workflow settings
    m_autoSaveEnabled->setChecked(settings.value("Workflow/AutoSave", true).toBool());
    m_autoSaveInterval->setValue(settings.value("Workflow/AutoSaveInterval", 5).toInt());
    m_rememberLastProject->setChecked(settings.value("Workflow/RememberLastProject", true).toBool());
    m_confirmDeletes->setChecked(settings.value("Workflow/ConfirmDeletes", true).toBool());
    m_undoLimitEnabled->setChecked(settings.value("Workflow/UndoLimitEnabled", true).toBool());
    m_undoLimit->setValue(settings.value("Workflow/UndoLimit", 100).toInt());
    
    // Load viewport settings
    m_antiAliasing->setCurrentIndex(settings.value("Viewport/AntiAliasing", 2).toInt());
    m_vsyncEnabled->setChecked(settings.value("Viewport/VSync", true).toBool());
    m_fov->setValue(settings.value("Viewport/FOV", 60).toInt());
    m_fovLabel->setText(QString("%1°").arg(m_fov->value()));
    m_orbitAroundSelection->setChecked(settings.value("Viewport/OrbitAroundSelection", true).toBool());
    m_invertY->setChecked(settings.value("Viewport/InvertY", false).toBool());
    m_mouseWheelSensitivity->setValue(settings.value("Viewport/MouseWheelSensitivity", 5).toInt());
    
    // Load performance settings
    m_cacheSize->setValue(settings.value("Performance/CacheSize", 512).toInt());
    m_cacheSizeLabel->setText(QString("%1 MB").arg(m_cacheSize->value()));
    m_maxThreads->setValue(settings.value("Performance/MaxThreads", QThread::idealThreadCount()).toInt());
    m_backgroundProcessing->setChecked(settings.value("Performance/BackgroundProcessing", true).toBool());
    m_hardwareAcceleration->setChecked(settings.value("Performance/HardwareAcceleration", true).toBool());
}

void SettingsDialog::saveSettings()
{
    QSettings settings;
    
    // Save appearance settings
    settings.setValue("UI/Scale", m_uiScaleSlider->value());
    settings.setValue("UI/Animations", m_animationsEnabled->isChecked());
    settings.setValue("UI/CompactMode", m_compactMode->isChecked());
    settings.setValue("UI/ShowTooltips", m_showTooltips->isChecked());
    
    // Save workflow settings
    settings.setValue("Workflow/AutoSave", m_autoSaveEnabled->isChecked());
    settings.setValue("Workflow/AutoSaveInterval", m_autoSaveInterval->value());
    settings.setValue("Workflow/RememberLastProject", m_rememberLastProject->isChecked());
    settings.setValue("Workflow/ConfirmDeletes", m_confirmDeletes->isChecked());
    settings.setValue("Workflow/UndoLimitEnabled", m_undoLimitEnabled->isChecked());
    settings.setValue("Workflow/UndoLimit", m_undoLimit->value());
    
    // Save viewport settings
    settings.setValue("Viewport/AntiAliasing", m_antiAliasing->currentIndex());
    settings.setValue("Viewport/VSync", m_vsyncEnabled->isChecked());
    settings.setValue("Viewport/FOV", m_fov->value());
    settings.setValue("Viewport/OrbitAroundSelection", m_orbitAroundSelection->isChecked());
    settings.setValue("Viewport/InvertY", m_invertY->isChecked());
    settings.setValue("Viewport/MouseWheelSensitivity", m_mouseWheelSensitivity->value());
    
    // Save performance settings
    settings.setValue("Performance/CacheSize", m_cacheSize->value());
    settings.setValue("Performance/MaxThreads", m_maxThreads->value());
    settings.setValue("Performance/BackgroundProcessing", m_backgroundProcessing->isChecked());
    settings.setValue("Performance/HardwareAcceleration", m_hardwareAcceleration->isChecked());
}
