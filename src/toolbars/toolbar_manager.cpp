#include "toolbars/toolbar_manager.hpp"
#include "toolbars/main_toolbar.hpp"
#include "toolbars/primitives_toolbar.hpp"
#include "toolbars/selection_toolbar.hpp"
#include "toolbars/transform_toolbar.hpp"
#include <QMainWindow>
#include <QSettings>

ToolbarManager::ToolbarManager(QMainWindow* mainWindow, QObject* parent)
    : QObject(parent)
    , m_mainWindow(mainWindow)
{
    // Setup default configuration
    m_defaultConfig["main"] = {Qt::TopToolBarArea, true, false};
    m_defaultConfig["primitives"] = {Qt::TopToolBarArea, true, false};
    m_defaultConfig["selection"] = {Qt::LeftToolBarArea, true, false};
    m_defaultConfig["transform"] = {Qt::LeftToolBarArea, true, false};
}

ToolbarManager::~ToolbarManager()
{
    // Unique pointers will handle cleanup automatically
}

void ToolbarManager::createAllToolbars()
{
    // Create main toolbar
    auto mainToolbar = new MainToolbar(m_mainWindow);
    m_mainToolbar = std::unique_ptr<MainToolbar>(mainToolbar);
    registerToolbar("main", m_mainToolbar.get());
    
    // Create primitives toolbar
    auto primitives = new PrimitivesToolbar(m_mainWindow);
    m_primitivesToolbar = std::unique_ptr<PrimitivesToolbar>(primitives);
    registerToolbar("primitives", m_primitivesToolbar.get());
    
    // Create selection toolbar - TEMPORARILY DISABLED
    // auto selection = new SelectionToolbar(m_mainWindow);
    // m_selectionToolbar = std::unique_ptr<SelectionToolbar>(selection);
    // registerToolbar("selection", m_selectionToolbar.get());
    
    // Create transform toolbar - TEMPORARILY DISABLED
    // auto transform = new TransformToolbar(m_mainWindow);
    // m_transformToolbar = std::unique_ptr<TransformToolbar>(transform);
    // registerToolbar("transform", m_transformToolbar.get());
}

void ToolbarManager::setupToolbarLayout()
{
    // Setup default layout according to configuration
    for (auto it = m_defaultConfig.begin(); it != m_defaultConfig.end(); ++it) {
        const QString& name = it.key();
        const ToolbarConfig& config = it.value();
        
        if (m_toolbars.contains(name)) {
            QToolBar* toolbar = m_toolbars[name];
            
            // Add to main window
            m_mainWindow->addToolBar(config.area, toolbar);
            
            // Set visibility
            toolbar->setVisible(config.visible);
            
            // Add separator after toolbar if specified
            if (config.breakBefore) {
                m_mainWindow->addToolBarBreak(config.area);
            }
        }
    }
}

void ToolbarManager::registerToolbar(const QString& name, QToolBar* toolbar)
{
    m_toolbars[name] = toolbar;
    connectToolbarSignals(toolbar);
}

void ToolbarManager::connectToolbarSignals(QToolBar* toolbar)
{
    // Connect visibility change signals
    connect(toolbar, &QToolBar::visibilityChanged,
            this, &ToolbarManager::onToolbarVisibilityChanged);
}

void ToolbarManager::restoreToolbarState()
{
    QSettings settings;
    settings.beginGroup("Toolbars");
    
    for (auto it = m_toolbars.begin(); it != m_toolbars.end(); ++it) {
        const QString& name = it.key();
        QToolBar* toolbar = it.value();
        
        if (settings.contains(name + "/visible")) {
            bool visible = settings.value(name + "/visible").toBool();
            toolbar->setVisible(visible);
        }
    }
    
    settings.endGroup();
}

QToolBar* ToolbarManager::getToolbar(const QString& name) const
{
    return m_toolbars.value(name, nullptr);
}

void ToolbarManager::showToolbar(const QString& name, bool show)
{
    if (m_toolbars.contains(name)) {
        m_toolbars[name]->setVisible(show);
    }
}

void ToolbarManager::hideToolbar(const QString& name)
{
    showToolbar(name, false);
}

void ToolbarManager::toggleToolbar(const QString& name)
{
    if (m_toolbars.contains(name)) {
        QToolBar* toolbar = m_toolbars[name];
        toolbar->setVisible(!toolbar->isVisible());
    }
}

void ToolbarManager::setToolbarArea(const QString& name, Qt::ToolBarArea area)
{
    if (m_toolbars.contains(name)) {
        m_mainWindow->addToolBar(area, m_toolbars[name]);
    }
}

void ToolbarManager::insertToolbarBreak(const QString& beforeToolbar)
{
    if (m_toolbars.contains(beforeToolbar)) {
        QToolBar* toolbar = m_toolbars[beforeToolbar];
        Qt::ToolBarArea area = m_mainWindow->toolBarArea(toolbar);
        
        // area could be used for area-specific break insertion logic
        // For now, acknowledge variable until area-specific functionality is needed
        (void)area;
        
        m_mainWindow->insertToolBarBreak(toolbar);
    }
}

void ToolbarManager::saveToolbarState()
{
    QSettings settings;
    settings.beginGroup("Toolbars");
    
    for (auto it = m_toolbars.begin(); it != m_toolbars.end(); ++it) {
        const QString& name = it.key();
        QToolBar* toolbar = it.value();
        
        settings.setValue(name + "/visible", toolbar->isVisible());
        settings.setValue(name + "/area", static_cast<int>(m_mainWindow->toolBarArea(toolbar)));
    }
    
    settings.endGroup();
}

void ToolbarManager::resetToDefaultLayout()
{
    // Reset all toolbars to default configuration
    for (auto it = m_defaultConfig.begin(); it != m_defaultConfig.end(); ++it) {
        const QString& name = it.key();
        const ToolbarConfig& config = it.value();
        
        if (m_toolbars.contains(name)) {
            QToolBar* toolbar = m_toolbars[name];
            m_mainWindow->addToolBar(config.area, toolbar);
            toolbar->setVisible(config.visible);
        }
    }
}

void ToolbarManager::onToolbarVisibilityChanged(bool visible)
{
    QToolBar* toolbar = qobject_cast<QToolBar*>(sender());
    if (toolbar) {
        // Find the toolbar name
        for (auto it = m_toolbars.begin(); it != m_toolbars.end(); ++it) {
            if (it.value() == toolbar) {
                emit toolbarVisibilityChanged(it.key(), visible);
                break;
            }
        }
    }
}
