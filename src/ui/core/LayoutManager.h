#pragma once

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QSettings>
#include <QDockWidget>

namespace RudeBase3D::UI {
    
    /**
     * @brief Manages dockable panels and layout customization
     * 
     * Handles panel registration, layout saving/loading, and provides
     * a flexible docking system for the application interface.
     */
    class LayoutManager : public QObject {
        Q_OBJECT
        
    public:
        explicit LayoutManager(QObject* parent = nullptr);
        
        // Panel management
        void registerPanel(const QString& name, QDockWidget* panel);
        void unregisterPanel(const QString& name);
        
        QDockWidget* getPanel(const QString& name) const;
        QStringList getPanelNames() const;
        
        // Visibility control
        void showPanel(const QString& name);
        void hidePanel(const QString& name);
        void togglePanel(const QString& name);
        bool isPanelVisible(const QString& name) const;
        
        // Layout management
        void saveLayout(const QString& layoutName = "default");
        void loadLayout(const QString& layoutName = "default");
        void resetToDefaultLayout();
        
        QStringList getAvailableLayouts() const;
        
        // Layout presets
        void applyModelingLayout();
        void applySculptingLayout();
        void applyAnimationLayout();
        void applyRenderingLayout();
        
    signals:
        void panelVisibilityChanged(const QString& panelName, bool visible);
        void layoutChanged(const QString& layoutName);
        
    public slots:
        void onPanelVisibilityChanged(bool visible);
        
    private:
        void setupDefaultLayout();
        QString getLayoutKey(const QString& layoutName) const;
        
        QMap<QString, QDockWidget*> m_panels;
        QSettings* m_settings;
        QString m_currentLayout;
    };
    
} // namespace RudeBase3D::UI
