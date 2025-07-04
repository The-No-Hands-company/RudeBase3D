#include "toolbars/base_toolbar.hpp"
#include <QAction>
#include <QIcon>
#include <QDebug>

BaseToolbar::BaseToolbar(const QString& title, QMainWindow* parent)
    : QToolBar(title, parent)
    , m_mainWindow(parent)
{
    qDebug() << "[BaseToolbar] Constructor started for:" << title;
    setupToolbarStyle();
    qDebug() << "[BaseToolbar] setupToolbarStyle() completed";
    
    // Connect visibility changes
    connect(this, &QToolBar::visibilityChanged, 
            this, &BaseToolbar::onVisibilityChanged);
    qDebug() << "[BaseToolbar] Visibility signal connected";
    
    qDebug() << "[BaseToolbar] Created:" << title;
}

void BaseToolbar::initialize()
{
    createActions();
    setupLayout();
    connectSignals();
}

void BaseToolbar::setupToolbarStyle()
{
    // Set toolbar properties
    setMovable(true);
    setFloatable(true);
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setIconSize(QSize(24, 24));
    
    // Note: setObjectName should be set in derived class constructors
    // since getToolbarId() is virtual and not safe to call during base construction
    
    // Set toolbar style
    setStyleSheet(R"(
        QToolBar {
            background: #f0f0f0;
            border: 1px solid #d0d0d0;
            spacing: 2px;
            padding: 2px;
        }
        QToolBar::separator {
            background: #d0d0d0;
            width: 1px;
            margin: 4px 2px;
        }
        QToolButton {
            background: transparent;
            border: 1px solid transparent;
            border-radius: 3px;
            padding: 3px;
            margin: 1px;
        }
        QToolButton:hover {
            background: #e0e0e0;
            border: 1px solid #c0c0c0;
        }
        QToolButton:pressed {
            background: #d0d0d0;
            border: 1px solid #a0a0a0;
        }
        QToolButton:checked {
            background: #b0d4f1;
            border: 1px solid #4a90e2;
        }
    )");
}

void BaseToolbar::addToolbarSeparator()
{
    addSeparator();
}

QIcon BaseToolbar::loadIcon(const QString& iconPath, const QSize& size)
{
    QIcon icon(iconPath);
    if (icon.isNull()) {
        qDebug() << "[BaseToolbar] Warning: Could not load icon:" << iconPath;
        // Create a simple colored square as fallback
        QPixmap pixmap(size);
        pixmap.fill(Qt::gray);
        icon = QIcon(pixmap);
    }
    return icon;
}

QAction* BaseToolbar::createAction(const QString& id, const QString& text, 
                                  const QString& tooltip, bool addToActions)
{
    auto action = std::make_unique<QAction>(text, this);
    action->setObjectName(id);
    action->setToolTip(tooltip);
    action->setStatusTip(tooltip);
    
    QAction* actionPtr = action.get();
    
    // Connect the action to our signal handler
    connect(actionPtr, &QAction::triggered, this, &BaseToolbar::onActionTriggered);
    
    if (addToActions) {
        m_actions.push_back(std::move(action));
        m_actionMap[id] = actionPtr;
        
        // Add the action to the toolbar
        addAction(actionPtr);
    }
    
    return actionPtr;
}

void BaseToolbar::setToolbarVisible(bool visible)
{
    setVisible(visible);
}

bool BaseToolbar::isToolbarVisible() const
{
    return isVisible();
}

void BaseToolbar::onActionTriggered()
{
    qDebug() << "[BaseToolbar] *** ACTION TRIGGERED ***";
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        qDebug() << "[BaseToolbar] Action found, objectName:" << action->objectName() << "text:" << action->text();
        // Find action in our collection
        for (const auto& actionPtr : m_actions) {
            if (actionPtr.get() == action) {
                qDebug() << "[BaseToolbar] Action found in collection, emitting actionTriggered signal:" << action->objectName();
                emit actionTriggered(action->objectName());
                qDebug() << "[BaseToolbar] Signal emitted successfully";
                break;
            }
        }
    } else {
        qDebug() << "[BaseToolbar] ERROR: sender is not a QAction";
    }
}

void BaseToolbar::onVisibilityChanged(bool visible)
{
    emit toolbarVisibilityChanged(visible);
    qDebug() << "[BaseToolbar]" << getToolbarId() << "visibility changed:" << visible;
}
