#pragma once

#include "BaseComponent.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace rudebase3d {
namespace ui {

class ThemeManager;

/**
 * @brief Theme selector widget for easy theme switching
 * 
 * Provides a user-friendly interface for selecting and applying themes,
 * including automatic dark mode detection.
 */
class ThemeSelector : public BaseComponent
{
    Q_OBJECT

public:
    explicit ThemeSelector(QWidget* parent = nullptr);

    /**
     * @brief Set current theme
     */
    void setCurrentTheme(const QString& themeId);

    /**
     * @brief Enable/disable system theme auto-detection
     */
    void setAutoDetectEnabled(bool enabled);

    /**
     * @brief Check if auto-detect is enabled
     */
    bool isAutoDetectEnabled() const;

protected:
    void applyTheme() override;

private slots:
    void onThemeChanged(int index);
    void onAutoDetectClicked();
    void onSystemThemeChanged();

private:
    void setupUI();
    void populateThemes();
    void updateAutoDetectButton();

    ThemeManager* m_themeManager;
    QHBoxLayout* m_layout;
    QLabel* m_label;
    QComboBox* m_themeCombo;
    QPushButton* m_autoDetectButton;
    bool m_autoDetectEnabled = false;
    bool m_updating = false;
};

} // namespace ui
} // namespace rudebase3d
