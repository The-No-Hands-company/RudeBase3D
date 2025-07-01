#pragma once

#include <QObject>
#include <QColor>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QJsonObject>

namespace rudebase3d {
namespace ui {

/**
 * @brief Manages application themes and styling
 * 
 * The ThemeManager provides a centralized system for managing application
 * themes, colors, and styling. It supports both built-in and custom themes.
 */
class ThemeManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Color roles for theming
     */
    enum class ColorRole {
        Background,
        BackgroundAlternate,
        Foreground,
        Primary,
        Secondary,
        Success,
        Warning,
        Error,
        Border,
        Shadow,
        Highlight,
        Selection
    };

    /**
     * @brief Theme data structure
     */
    struct Theme {
        QString name;
        QString id;
        QMap<ColorRole, QColor> colors;
    };

    /**
     * @brief Get the singleton instance
     */
    static ThemeManager* instance();

    /**
     * @brief Get list of available themes
     */
    QStringList availableThemes() const;

    /**
     * @brief Get current theme ID
     */
    QString currentTheme() const;

    /**
     * @brief Apply a theme by ID
     */
    void applyTheme(const QString& themeId);

    /**
     * @brief Get a color for the specified role
     */
    QColor getColor(ColorRole role) const;

    /**
     * @brief Auto-detect and apply system theme (light/dark)
     */
    void applySystemTheme();

    /**
     * @brief Check if system is using dark mode
     */
    bool isSystemDarkMode() const;

    /**
     * @brief Get recommended theme IDs for light/dark modes
     */
    QString getRecommendedLightTheme() const;
    QString getRecommendedDarkTheme() const;

    /**
     * @brief Get all dark theme IDs
     */
    QStringList getDarkThemes() const;

    /**
     * @brief Get all light theme IDs  
     */
    QStringList getLightThemes() const;

signals:
    /**
     * @brief Emitted when theme changes
     */
    void themeChanged(const QString& themeId);

private:
    explicit ThemeManager(QObject* parent = nullptr);
    
    void loadThemes();
    void loadBuiltInThemes();
    void loadThemesFromDirectory(const QString& directory);
    void loadThemeFromFile(const QString& filePath);
    Theme themeFromJson(const QJsonObject& json);
    QString generateStyleSheet(const Theme& theme);

    static ThemeManager* s_instance;
    QMap<QString, Theme> m_themes;
    QString m_currentTheme;
};

} // namespace ui
} // namespace rudebase3d
