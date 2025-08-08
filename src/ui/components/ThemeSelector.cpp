#include "ThemeSelector.h"
#include "../core/ThemeManager.h"
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QIcon>

namespace rudebase3d {
namespace ui {

ThemeSelector::ThemeSelector(QWidget* parent)
    : BaseComponent(parent)
    , m_themeManager(ThemeManager::instance())
{
    setupUI();
    populateThemes();
    
    // Connect to theme manager
    connect(m_themeManager, &ThemeManager::themeChanged,
            this, &ThemeSelector::onSystemThemeChanged);
    
    // Set initial theme
    setCurrentTheme(m_themeManager->currentTheme());
}

void ThemeSelector::setupUI()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(8, 4, 8, 4);
    m_layout->setSpacing(8);
    
    // Theme label
    m_label = new QLabel("Theme:", this);
    m_label->setMinimumWidth(50);
    m_layout->addWidget(m_label);
    
    // Theme selection combo box
    m_themeCombo = new QComboBox(this);
    m_themeCombo->setMinimumWidth(150);
    m_themeCombo->setToolTip("Select application theme");
    connect(m_themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ThemeSelector::onThemeChanged);
    m_layout->addWidget(m_themeCombo);
    
    // Auto-detect button
    m_autoDetectButton = new QPushButton("Auto", this);
    m_autoDetectButton->setCheckable(true);
    m_autoDetectButton->setToolTip("Automatically follow system theme");
    m_autoDetectButton->setMaximumWidth(60);
    connect(m_autoDetectButton, &QPushButton::clicked,
            this, &ThemeSelector::onAutoDetectClicked);
    m_layout->addWidget(m_autoDetectButton);
    
    m_layout->addStretch();
    
    setBaseTooltip("Theme selection and customization");
}

void ThemeSelector::populateThemes()
{
    m_updating = true;
    
    m_themeCombo->clear();
    
    // Add themes with user-friendly names and categories
    m_themeCombo->addItem("🌞 Light", "light");
    m_themeCombo->addItem("🌙 Dark", "dark");
    m_themeCombo->addItem("🌆 Modern Dark", "modern-dark");
    m_themeCombo->addItem("🔥 Warm Dark", "warm-dark");
    m_themeCombo->addItem("🔮 Purple Dark", "purple-dark");
    m_themeCombo->addItem("💼 Professional Blue", "professional-blue");
    m_themeCombo->addItem("⚫ High Contrast Dark", "high-contrast-dark");
    
    m_updating = false;
}

void ThemeSelector::setCurrentTheme(const QString& themeId)
{
    if (m_updating) return;
    
    m_updating = true;
    
    // Find and select the theme in the combo box
    for (int i = 0; i < m_themeCombo->count(); ++i) {
        if (m_themeCombo->itemData(i).toString() == themeId) {
            m_themeCombo->setCurrentIndex(i);
            break;
        }
    }
    
    m_updating = false;
}

void ThemeSelector::setAutoDetectEnabled(bool enabled)
{
    m_autoDetectEnabled = enabled;
    m_autoDetectButton->setChecked(enabled);
    updateAutoDetectButton();
    
    if (enabled) {
        m_themeManager->applySystemTheme();
    }
}

bool ThemeSelector::isAutoDetectEnabled() const
{
    return m_autoDetectEnabled;
}

void ThemeSelector::onThemeChanged(int index)
{
    if (m_updating) return;
    
    QString themeId = m_themeCombo->itemData(index).toString();
    if (!themeId.isEmpty()) {
        // Disable auto-detect when manually selecting theme
        if (m_autoDetectEnabled) {
            m_autoDetectEnabled = false;
            m_autoDetectButton->setChecked(false);
            updateAutoDetectButton();
        }
        
        m_themeManager->applyTheme(themeId);
        emit themeChanged(themeId);
    }
}

void ThemeSelector::onAutoDetectClicked()
{
    setAutoDetectEnabled(m_autoDetectButton->isChecked());
}

void ThemeSelector::onSystemThemeChanged()
{
    if (!m_autoDetectEnabled) {
        // Update selector to reflect the current theme
        setCurrentTheme(m_themeManager->currentTheme());
    }
}

void ThemeSelector::updateAutoDetectButton()
{
    if (m_autoDetectEnabled) {
        m_autoDetectButton->setText("Auto ✓");
        m_autoDetectButton->setToolTip("Auto-detect enabled - following system theme");
        m_themeCombo->setEnabled(false);
    } else {
        m_autoDetectButton->setText("Auto");
        m_autoDetectButton->setToolTip("Click to automatically follow system theme");
        m_themeCombo->setEnabled(true);
    }
}

void ThemeSelector::applyTheme()
{
    BaseComponent::applyTheme();
    
    // Update component styling based on current theme
    if (m_label && m_themeCombo && m_autoDetectButton) {
        QString labelStyle = QString("QLabel { color: %1; }")
            .arg(getThemeColor(ThemeManager::ColorRole::Foreground).name());
        m_label->setStyleSheet(labelStyle);
        
        QString comboStyle = QString(R"(
            QComboBox {
                background-color: %1;
                color: %2;
                border: 1px solid %3;
                border-radius: 4px;
                padding: 4px 8px;
            }
            QComboBox::drop-down {
                border: none;
                width: 20px;
            }
            QComboBox::down-arrow {
                border: none;
                width: 8px;
                height: 8px;
            }
            QComboBox QAbstractItemView {
                background-color: %1;
                color: %2;
                border: 1px solid %3;
                selection-background-color: %4;
            }
        )")
        .arg(getThemeColor(ThemeManager::ColorRole::BackgroundAlternate).name())
        .arg(getThemeColor(ThemeManager::ColorRole::Foreground).name())
        .arg(getThemeColor(ThemeManager::ColorRole::Border).name())
        .arg(getThemeColor(ThemeManager::ColorRole::Highlight).name());
        
        m_themeCombo->setStyleSheet(comboStyle);
        
        QString buttonStyle = QString(R"(
            QPushButton {
                background-color: %1;
                color: %2;
                border: 1px solid %3;
                border-radius: 4px;
                padding: 4px 8px;
                font-weight: 500;
            }
            QPushButton:hover {
                background-color: %4;
            }
            QPushButton:checked {
                background-color: %5;
                color: white;
            }
        )")
        .arg(getThemeColor(ThemeManager::ColorRole::BackgroundAlternate).name())
        .arg(getThemeColor(ThemeManager::ColorRole::Foreground).name())
        .arg(getThemeColor(ThemeManager::ColorRole::Border).name())
        .arg(getThemeColor(ThemeManager::ColorRole::Highlight).name())
        .arg(getThemeColor(ThemeManager::ColorRole::Primary).name());
        
        m_autoDetectButton->setStyleSheet(buttonStyle);
    }
}

} // namespace ui
} // namespace rudebase3d
