#pragma once

#include <QWidget>
#include "ui/core/ThemeManager.h"

namespace RudeBase3D::UI {
    
    /**
     * @brief Base class for all custom UI components
     * 
     * Provides theme integration, consistent styling, and common functionality
     * for all UI components in RudeBase3D.
     */
    class BaseComponent : public QWidget {
        Q_OBJECT
        
    public:
        explicit BaseComponent(QWidget* parent = nullptr);
        virtual ~BaseComponent() = default;
        
    protected:
        // Theme integration
        virtual void updateTheme();
        ThemeManager* themeManager() const { return m_themeManager; }
        
        // Common styling helpers
        void setThemedStyleSheet(const QString& componentName);
        QColor getThemeColor(const QString& colorName) const;
        QFont getThemeFont(const QString& fontName) const;
        int getThemeSpacing(const QString& sizeName) const;
        
        // Event handling
        void paintEvent(QPaintEvent* event) override;
        void changeEvent(QEvent* event) override;
        
    private slots:
        void onThemeChanged(ThemeManager::Theme newTheme);
        
    protected:
        ThemeManager* m_themeManager;
        bool m_themeAware = true;
    };
    
} // namespace RudeBase3D::UI
