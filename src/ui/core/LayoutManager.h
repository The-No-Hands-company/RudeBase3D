#pragma once

#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>
#include <QLabel>

namespace rudebase3d {
namespace ui {

/**
 * @brief Manages application layout configurations
 * 
 * The LayoutManager provides preset workspace layouts optimized for different
 * tasks like modeling, animation, rendering, and sculpting.
 */
class LayoutManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Dock widget areas
     */
    enum class DockArea {
        Left,
        Right,
        Top,
        Bottom
    };

    /**
     * @brief Viewport display modes
     */
    enum class ViewportMode {
        Single,     // Single viewport
        Split,      // Split viewport
        Quad        // Quad viewport
    };

    /**
     * @brief Camera projection modes
     */
    enum class CameraMode {
        Perspective,
        Orthographic,
        Mixed       // Multiple camera types
    };

    /**
     * @brief Viewport configuration
     */
    struct ViewportConfiguration {
        ViewportMode mode = ViewportMode::Single;
        CameraMode camera = CameraMode::Perspective;
        bool showGrid = true;
        bool showGizmos = true;
        bool showStats = false;
    };

    /**
     * @brief Layout configuration
     */
    struct LayoutConfiguration {
        QString name;
        QString id;
        QString description;
        QMap<DockArea, QStringList> dockAreas;
        ViewportConfiguration viewportConfig;
    };

    /**
     * @brief Get the singleton instance
     */
    static LayoutManager* instance();

    /**
     * @brief Register the main window for layout management
     */
    void registerMainWindow(QMainWindow* mainWindow);

    /**
     * @brief Get list of available layouts
     */
    QStringList availableLayouts() const;

    /**
     * @brief Get current layout ID
     */
    QString currentLayout() const;

    /**
     * @brief Apply a layout by ID
     */
    void applyLayout(const QString& layoutId);

    /**
     * @brief Reset to default layout
     */
    void resetToDefault();

    /**
     * @brief Save current layout state
     */
    void saveCurrentLayout();

signals:
    /**
     * @brief Emitted when layout changes
     */
    void layoutChanged(const QString& layoutId);

    /**
     * @brief Emitted when viewport configuration changes
     */
    void viewportConfigurationChanged(const ViewportConfiguration& config);

private:
    explicit LayoutManager(QObject* parent = nullptr);
    
    void loadDefaultLayouts();
    void setupDefaultLayout();
    void hideAllDockWidgets();
    void applyDockConfiguration(const LayoutConfiguration& layout);
    void setupDockArea(Qt::DockWidgetArea area, const QStringList& panelNames);
    QDockWidget* findOrCreateDockWidget(const QString& name);
    QWidget* createPanelContent(const QString& panelName);
    QWidget* createPlaceholderWidget(const QString& text);
    void applyViewportConfiguration(const ViewportConfiguration& config);
    void saveLayoutToSettings(const QString& layoutId);
    void loadLayoutFromSettings();

    static LayoutManager* s_instance;
    QMainWindow* m_mainWindow = nullptr;
    QMap<QString, LayoutConfiguration> m_layouts;
    QString m_currentLayout;
};

} // namespace ui
} // namespace rudebase3d
