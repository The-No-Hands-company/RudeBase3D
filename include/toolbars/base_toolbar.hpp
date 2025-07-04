#pragma once

#include <QToolBar>
#include <QAction>
#include <QMainWindow>
#include <QMap>
#include <memory>
#include <vector>

/**
 * @brief Base class for all application toolbars
 * 
 * Provides common functionality and interface for all toolbars:
 * - Standard toolbar setup
 * - Icon management
 * - Tooltip and status tip handling
 * - Signal connections
 */
class BaseToolbar : public QToolBar
{
    Q_OBJECT

public:
    explicit BaseToolbar(const QString& title, QMainWindow* parent = nullptr);
    virtual ~BaseToolbar() = default;

    // Toolbar identification
    virtual QString getToolbarId() const = 0;
    virtual QString getToolbarDescription() const = 0;
    
    // Toolbar state
    void setToolbarVisible(bool visible);
    bool isToolbarVisible() const;
    
    void addToolbarSeparator();

signals:
    void actionTriggered(const QString& actionId);
    void toolbarVisibilityChanged(bool visible);

protected:
    // Initialization
    void initialize();
    
    // Virtual methods for subclasses to implement
    virtual void createActions() = 0;
    virtual void setupLayout() = 0;
    virtual void connectSignals() = 0;
    
    // Helper methods
    void setupToolbarStyle();
    QIcon loadIcon(const QString& iconPath, const QSize& size = QSize(24, 24));
    QAction* createAction(const QString& id, const QString& text, 
                         const QString& tooltip, bool addToActions = true);
    
    // Action storage
    QMap<QString, QAction*> m_actionMap;
    std::vector<std::unique_ptr<QAction>> m_actions;
    
    QMainWindow* m_mainWindow;

private slots:
    void onActionTriggered();
    void onVisibilityChanged(bool visible);

private:
    QString m_toolbarId;
    QString m_description;
};
