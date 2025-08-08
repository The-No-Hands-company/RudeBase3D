/*
 * ██████╗ ██╗   ██╗██████╗ ███████╗██████╗  █████╗ ███████╗███████╗██████╗ ██████╗ 
 * ██╔══██╗██║   ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝╚════██╗██╔══██╗
 * ██████╔╝██║   ██║██║  ██║█████╗  ██████╔╝███████║███████╗█████╗   █████╔╝██║  ██║
 * ██╔══██╗██║   ██║██║  ██║██╔══╝  ██╔══██╗██╔══██║╚════██║██╔══╝   ╚═══██╗██║  ██║
 * ██║  ██║╚██████╔╝██████╔╝███████╗██████╔╝██║  ██║███████║███████╗██████╔╝██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═════╝ ╚═════╝ 
 * 
 * Professional 3D Modeling & Animation Software
 * 
 * ████████████████████████████████████████████████████████████████████████████████
 * █                                                                              █
 * █  FILE: ThemeSelector.h                                                       █
 * █  DESCRIPTION: UI Theme Selection Component & Style Management               █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite            █
 * █  VERSION: 1.0.0                                                              █
 * █  CREATED: 2024                                                               █
 * █  UPDATED: August 7, 2025                                                     █
 * █                                                                              █
 * █  LICENSE: MIT License                                                        █
 * █  COMPANY: The No Hands Company                                               █
 * █  AUTHOR: RudeBase3D Development Team                                         █
 * █                                                                              █
 * █  This file contains the ThemeSelector component providing user interface    █
 * █  for theme selection and style management in the professional 3D            █
 * █  modeling application environment.                                           █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

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

signals:
    /**
     * @brief Emitted when theme selection changes
     */
    void themeChanged(const QString& themeId);

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
