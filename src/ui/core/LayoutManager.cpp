#include "LayoutManager.h"
#include <QApplication>
#include <QScreen>
#include <QSettings>
#include <QSplitter>
#include <QDockWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>

namespace rudebase3d {
namespace ui {

LayoutManager* LayoutManager::s_instance = nullptr;

LayoutManager::LayoutManager(QObject* parent)
    : QObject(parent)
    , m_currentLayout("default")
{
    loadDefaultLayouts();
}

LayoutManager* LayoutManager::instance()
{
    if (!s_instance) {
        s_instance = new LayoutManager();
    }
    return s_instance;
}

void LayoutManager::registerMainWindow(QMainWindow* mainWindow)
{
    m_mainWindow = mainWindow;
    setupDefaultLayout();
}

void LayoutManager::loadDefaultLayouts()
{
    // Modeling Layout
    LayoutConfiguration modeling;
    modeling.name = "Modeling";
    modeling.id = "modeling";
    modeling.description = "Optimized for 3D modeling tasks";
    modeling.dockAreas[DockArea::Left] = {"Hierarchy", "Properties"};
    modeling.dockAreas[DockArea::Right] = {"Materials", "Textures"};
    modeling.dockAreas[DockArea::Bottom] = {"Tools", "Console"};
    modeling.viewportConfig.mode = ViewportMode::Single;
    modeling.viewportConfig.camera = CameraMode::Perspective;
    
    m_layouts[modeling.id] = modeling;
    
    // Animation Layout
    LayoutConfiguration animation;
    animation.name = "Animation";
    animation.id = "animation";
    animation.description = "Timeline and animation tools";
    animation.dockAreas[DockArea::Left] = {"Hierarchy", "Properties"};
    animation.dockAreas[DockArea::Right] = {"Animation", "Keyframes"};
    animation.dockAreas[DockArea::Bottom] = {"Timeline", "Graph Editor"};
    animation.viewportConfig.mode = ViewportMode::Single;
    animation.viewportConfig.camera = CameraMode::Perspective;
    
    m_layouts[animation.id] = animation;
    
    // Rendering Layout
    LayoutConfiguration rendering;
    rendering.name = "Rendering";
    rendering.id = "rendering";
    rendering.description = "Material and lighting setup";
    rendering.dockAreas[DockArea::Left] = {"Scene", "Lights"};
    rendering.dockAreas[DockArea::Right] = {"Materials", "Render Settings"};
    rendering.dockAreas[DockArea::Bottom] = {"Render Queue", "Console"};
    rendering.viewportConfig.mode = ViewportMode::Split;
    rendering.viewportConfig.camera = CameraMode::Perspective;
    
    m_layouts[rendering.id] = rendering;
    
    // Sculpting Layout
    LayoutConfiguration sculpting;
    sculpting.name = "Sculpting";
    sculpting.id = "sculpting";
    sculpting.description = "Full-screen sculpting workspace";
    sculpting.dockAreas[DockArea::Left] = {"Brushes", "Symmetry"};
    sculpting.dockAreas[DockArea::Right] = {"Layers", "Properties"};
    sculpting.dockAreas[DockArea::Bottom] = {}; // Minimal bottom panel
    sculpting.viewportConfig.mode = ViewportMode::Single;
    sculpting.viewportConfig.camera = CameraMode::Perspective;
    
    m_layouts[sculpting.id] = sculpting;
}

QStringList LayoutManager::availableLayouts() const
{
    QStringList layouts;
    for (auto it = m_layouts.constBegin(); it != m_layouts.constEnd(); ++it) {
        layouts << it.value().name;
    }
    return layouts;
}

QString LayoutManager::currentLayout() const
{
    return m_currentLayout;
}

void LayoutManager::applyLayout(const QString& layoutId)
{
    if (!m_layouts.contains(layoutId) || !m_mainWindow) {
        qWarning() << "Layout not found or main window not registered:" << layoutId;
        return;
    }
    
    m_currentLayout = layoutId;
    const LayoutConfiguration& layout = m_layouts[layoutId];
    
    // Hide all dock widgets first
    hideAllDockWidgets();
    
    // Apply dock configuration
    applyDockConfiguration(layout);
    
    // Apply viewport configuration
    applyViewportConfiguration(layout.viewportConfig);
    
    // Save layout to settings
    saveLayoutToSettings(layoutId);
    
    emit layoutChanged(layoutId);
}

void LayoutManager::hideAllDockWidgets()
{
    if (!m_mainWindow) return;
    
    QList<QDockWidget*> dockWidgets = m_mainWindow->findChildren<QDockWidget*>();
    for (QDockWidget* dock : dockWidgets) {
        dock->hide();
    }
}

void LayoutManager::applyDockConfiguration(const LayoutConfiguration& layout)
{
    if (!m_mainWindow) return;
    
    // Apply left dock area
    if (!layout.dockAreas.value(DockArea::Left).isEmpty()) {
        setupDockArea(Qt::LeftDockWidgetArea, layout.dockAreas.value(DockArea::Left));
    }
    
    // Apply right dock area
    if (!layout.dockAreas.value(DockArea::Right).isEmpty()) {
        setupDockArea(Qt::RightDockWidgetArea, layout.dockAreas.value(DockArea::Right));
    }
    
    // Apply bottom dock area
    if (!layout.dockAreas.value(DockArea::Bottom).isEmpty()) {
        setupDockArea(Qt::BottomDockWidgetArea, layout.dockAreas.value(DockArea::Bottom));
    }
    
    // Apply top dock area (if specified)
    if (!layout.dockAreas.value(DockArea::Top).isEmpty()) {
        setupDockArea(Qt::TopDockWidgetArea, layout.dockAreas.value(DockArea::Top));
    }
}

void LayoutManager::setupDockArea(Qt::DockWidgetArea area, const QStringList& panelNames)
{
    if (!m_mainWindow || panelNames.isEmpty()) return;
    
    QList<QDockWidget*> dockWidgets;
    
    // Find or create dock widgets
    for (const QString& panelName : panelNames) {
        QDockWidget* dock = findOrCreateDockWidget(panelName);
        if (dock) {
            dockWidgets.append(dock);
            m_mainWindow->addDockWidget(area, dock);
            dock->show();
        }
    }
    
    // Tabify multiple dock widgets in the same area
    if (dockWidgets.size() > 1) {
        for (int i = 1; i < dockWidgets.size(); ++i) {
            m_mainWindow->tabifyDockWidget(dockWidgets[0], dockWidgets[i]);
        }
        // Bring the first dock to front
        dockWidgets[0]->raise();
    }
}

QDockWidget* LayoutManager::findOrCreateDockWidget(const QString& name)
{
    if (!m_mainWindow) return nullptr;
    
    // Try to find existing dock widget
    QList<QDockWidget*> dockWidgets = m_mainWindow->findChildren<QDockWidget*>();
    for (QDockWidget* dock : dockWidgets) {
        if (dock->windowTitle() == name || dock->objectName() == name) {
            return dock;
        }
    }
    
    // Create new dock widget if not found
    QDockWidget* dock = new QDockWidget(name, m_mainWindow);
    dock->setObjectName(name);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    
    // Create placeholder content
    QWidget* content = createPanelContent(name);
    dock->setWidget(content);
    
    return dock;
}

QWidget* LayoutManager::createPanelContent(const QString& panelName)
{
    QWidget* content = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(content);
    
    // Create appropriate content based on panel name
    if (panelName == "Hierarchy") {
        // Scene hierarchy tree widget
        layout->addWidget(createPlaceholderWidget("Scene Hierarchy"));
    }
    else if (panelName == "Properties") {
        // Properties panel
        layout->addWidget(createPlaceholderWidget("Object Properties"));
    }
    else if (panelName == "Materials") {
        // Material browser
        layout->addWidget(createPlaceholderWidget("Material Browser"));
    }
    else if (panelName == "Tools") {
        // Tool palette
        layout->addWidget(createPlaceholderWidget("Tool Palette"));
    }
    else if (panelName == "Console") {
        // Output console
        layout->addWidget(createPlaceholderWidget("Console Output"));
    }
    else if (panelName == "Timeline") {
        // Animation timeline
        layout->addWidget(createPlaceholderWidget("Animation Timeline"));
    }
    else {
        // Generic panel
        layout->addWidget(createPlaceholderWidget(panelName));
    }
    
    return content;
}

QWidget* LayoutManager::createPlaceholderWidget(const QString& text)
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QLabel* label = new QLabel(text);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: #888; font-size: 14px; padding: 20px;");
    
    layout->addWidget(label);
    
    return widget;
}

void LayoutManager::applyViewportConfiguration(const ViewportConfiguration& config)
{
    // This would be implemented based on your viewport system
    // For now, just emit a signal that viewport configuration should change
    emit viewportConfigurationChanged(config);
}

void LayoutManager::saveLayoutToSettings(const QString& layoutId)
{
    if (!m_mainWindow) return;
    
    QSettings settings;
    settings.beginGroup("Layout");
    settings.setValue("CurrentLayout", layoutId);
    settings.setValue("MainWindowGeometry", m_mainWindow->saveGeometry());
    settings.setValue("MainWindowState", m_mainWindow->saveState());
    settings.endGroup();
}

void LayoutManager::loadLayoutFromSettings()
{
    if (!m_mainWindow) return;
    
    QSettings settings;
    settings.beginGroup("Layout");
    
    QString layoutId = settings.value("CurrentLayout", "modeling").toString();
    QByteArray geometry = settings.value("MainWindowGeometry").toByteArray();
    QByteArray state = settings.value("MainWindowState").toByteArray();
    
    if (!geometry.isEmpty()) {
        m_mainWindow->restoreGeometry(geometry);
    }
    
    if (!state.isEmpty()) {
        m_mainWindow->restoreState(state);
    }
    
    settings.endGroup();
    
    // Apply the saved layout
    if (m_layouts.contains(layoutId)) {
        applyLayout(layoutId);
    }
}

void LayoutManager::setupDefaultLayout()
{
    if (!m_mainWindow) return;
    
    // Set up default window size and position
    QScreen* screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int width = screenGeometry.width() * 0.8;
        int height = screenGeometry.height() * 0.8;
        int x = (screenGeometry.width() - width) / 2;
        int y = (screenGeometry.height() - height) / 2;
        
        m_mainWindow->setGeometry(x, y, width, height);
    }
    
    // Try to load from settings first
    loadLayoutFromSettings();
    
    // If no layout was restored, apply default
    if (m_currentLayout.isEmpty()) {
        applyLayout("modeling");
    }
}

void LayoutManager::resetToDefault()
{
    applyLayout("modeling");
}

void LayoutManager::saveCurrentLayout()
{
    saveLayoutToSettings(m_currentLayout);
}

} // namespace ui
} // namespace rudebase3d
