#pragma once

#include <QObject>
#include <QToolBar>
#include <QMainWindow>
#include <QMap>
#include <QString>
#include <memory>

// Forward declarations
class MainToolbar;
class PrimitivesToolbar;
class SelectionToolbar;
class TransformToolbar;

/**
 * @brief Central manager for all application toolbars
 * 
 * This class manages the creation, organization, and lifecycle of all toolbars
 * in the VersaModeling application. It provides a centralized way to:
 * - Create and register toolbars
 * - Show/hide specific toolbars
 * - Organize toolbar layout
 * - Handle toolbar state persistence
 */
class ToolbarManager : public QObject
{
    Q_OBJECT

public:
    explicit ToolbarManager(QMainWindow* mainWindow, QObject* parent = nullptr);
    ~ToolbarManager();

    // Toolbar creation and management
    void createAllToolbars();
    void setupToolbarLayout();
    
    // Toolbar access
    QToolBar* getToolbar(const QString& name) const;
    MainToolbar* getMainToolbar() const { return m_mainToolbar.get(); }
    PrimitivesToolbar* getPrimitivesToolbar() const { return m_primitivesToolbar.get(); }
    SelectionToolbar* getSelectionToolbar() const { return m_selectionToolbar.get(); }
    TransformToolbar* getTransformToolbar() const { return m_transformToolbar.get(); }
    
    // Toolbar visibility
    void showToolbar(const QString& name, bool show = true);
    void hideToolbar(const QString& name);
    void toggleToolbar(const QString& name);
    
    // Layout management
    void setToolbarArea(const QString& name, Qt::ToolBarArea area);
    void insertToolbarBreak(const QString& beforeToolbar);
    
    // State management
    void saveToolbarState();
    void restoreToolbarState();
    void resetToDefaultLayout();

signals:
    void toolbarVisibilityChanged(const QString& toolbarName, bool visible);
    void toolbarAreaChanged(const QString& toolbarName, Qt::ToolBarArea area);

private slots:
    void onToolbarVisibilityChanged(bool visible);

private:
    void registerToolbar(const QString& name, QToolBar* toolbar);
    void connectToolbarSignals(QToolBar* toolbar);
    
    QMainWindow* m_mainWindow;
    
    // Toolbar instances
    std::unique_ptr<MainToolbar> m_mainToolbar;
    std::unique_ptr<PrimitivesToolbar> m_primitivesToolbar;
    std::unique_ptr<SelectionToolbar> m_selectionToolbar;
    std::unique_ptr<TransformToolbar> m_transformToolbar;
    
    // Toolbar registry
    QMap<QString, QToolBar*> m_toolbars;
    
    // Default layout configuration
    struct ToolbarConfig {
        Qt::ToolBarArea area;
        bool visible;
        bool breakBefore;
    };
    QMap<QString, ToolbarConfig> m_defaultConfig;
};
