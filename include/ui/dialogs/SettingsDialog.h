#pragma once

#include <QDialog>

class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;
class QPushButton;
class QGroupBox;
class QCheckBox;
class QSlider;
class QSpinBox;
class QComboBox;
class QLabel;
class QLineEdit;

namespace rudebase3d::ui {
    class ThemeSelector;
}

/**
 * @brief Comprehensive settings dialog for RudeBase3D user experience customization
 * 
 * Provides extensive customization options across organized tabs:
 * - Appearance & Theme: Visual customization and theming
 * - Workflow & Productivity: Auto-save, project management, shortcuts
 * - Viewport & 3D: Camera, rendering, navigation controls
 * - Performance: Memory, CPU, GPU optimization settings
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog();

public slots:
    void applySettings();
    void resetToDefaults();

private slots:
    void onThemeChanged();
    void onSettingChanged();

private:
    void setupUI();
    void setupAppearanceTab();
    void setupWorkflowTab();
    void setupViewportTab();
    void setupPerformanceTab();
    void createButtons();
    void connectSignals();
    void loadSettings();
    void saveSettings();

    // Main layout
    QVBoxLayout* m_mainLayout;
    QTabWidget* m_tabWidget;
    QHBoxLayout* m_buttonLayout;

    // Tab widgets
    QWidget* m_appearanceTab;
    QWidget* m_workflowTab;
    QWidget* m_viewportTab;
    QWidget* m_performanceTab;

    // Appearance tab controls
    QGroupBox* m_themeGroup;
    rudebase3d::ui::ThemeSelector* m_themeSelector;
    QGroupBox* m_uiGroup;
    QSlider* m_uiScaleSlider;
    QLabel* m_uiScaleLabel;
    QCheckBox* m_animationsEnabled;
    QCheckBox* m_compactMode;
    QCheckBox* m_showTooltips;

    // Workflow tab controls
    QGroupBox* m_autoSaveGroup;
    QCheckBox* m_autoSaveEnabled;
    QSpinBox* m_autoSaveInterval;
    QGroupBox* m_projectGroup;
    QCheckBox* m_rememberLastProject;
    QCheckBox* m_confirmDeletes;
    QGroupBox* m_undoGroup;
    QCheckBox* m_undoLimitEnabled;
    QSpinBox* m_undoLimit;

    // Viewport tab controls
    QGroupBox* m_renderingGroup;
    QComboBox* m_antiAliasing;
    QCheckBox* m_vsyncEnabled;
    QGroupBox* m_cameraGroup;
    QSlider* m_fov;
    QLabel* m_fovLabel;
    QCheckBox* m_orbitAroundSelection;
    QCheckBox* m_invertY;
    QSlider* m_mouseWheelSensitivity;

    // Performance tab controls
    QGroupBox* m_memoryGroup;
    QSlider* m_cacheSize;
    QLabel* m_cacheSizeLabel;
    QGroupBox* m_cpuGroup;
    QSpinBox* m_maxThreads;
    QCheckBox* m_backgroundProcessing;
    QCheckBox* m_hardwareAcceleration;

    // Dialog buttons
    QPushButton* m_applyButton;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
    QPushButton* m_resetButton;

    // State tracking
    bool m_settingsChanged;
};
