#pragma once

#include <QObject>
#include <QColor>
#include <QFont>
#include <QString>
#include <QMap>

namespace RudeBase3D::UI {
    
    /**
     * @brief Manages application themes and styling
     * 
     * Provides centralized theme management with support for light/dark modes
     * and consistent styling across all UI components.
     */
    class ThemeManager : public QObject {
        Q_OBJECT
        
    public:
        enum class Theme {
            Light,
            Dark,
            Auto  // Follows system preference
        };
        
        static ThemeManager& instance();
        
        // Theme management
        void setTheme(Theme theme);
        Theme currentTheme() const { return m_currentTheme; }
        
        // Color system
        QColor color(const QString& colorName) const;
        QColor primaryColor() const;
        QColor backgroundColor() const;
        QColor surfaceColor() const;
        QColor textColor() const;
        QColor borderColor() const;
        
        // Typography
        QFont font(const QString& fontName) const;
        QFont headerFont() const;
        QFont bodyFont() const;
        QFont codeFont() const;
        
        // Spacing system
        int spacing(const QString& sizeName) const;
        int spacingXS() const { return 4; }
        int spacingS() const { return 8; }
        int spacingM() const { return 16; }
        int spacingL() const { return 24; }
        int spacingXL() const { return 32; }
        
        // Style sheets
        QString getComponentStyle(const QString& componentName) const;
        QString getGlobalStyleSheet() const;
        
    signals:
        void themeChanged(Theme newTheme);
        
    private:
        ThemeManager();
        void loadTheme(Theme theme);
        void setupColorPalette();
        void setupFonts();
        void applyGlobalStyleSheet();
        
        Theme m_currentTheme = Theme::Dark;
        QMap<QString, QColor> m_colors;
        QMap<QString, QFont> m_fonts;
        QMap<QString, QString> m_styleSheets;
    };
    
} // namespace RudeBase3D::UI
