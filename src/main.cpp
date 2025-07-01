#include <QApplication>
#include <QDebug>
#include "ui/windows/MainWindow.h"
#include "ui/core/ThemeManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("RudeBase3D");
    app.setApplicationVersion("2.0.0");  // Version 2.0 with modern hybrid geometry
    app.setOrganizationName("RudeBase3D");
    app.setApplicationDisplayName("RudeBase3D - Professional 3D Modeling Suite");
    
    // Initialize theme system and apply system theme
    auto* themeManager = rudebase3d::ui::ThemeManager::instance();
    themeManager->applySystemTheme(); // Auto-detect and apply dark/light mode
    
    qDebug() << "Creating main window...";
    
    // Create and show main window
    MainWindow window;
    
    qDebug() << "Showing main window...";
    window.show();
    
    qDebug() << "Window shown, starting event loop...";
    int result = app.exec();
    qDebug() << "Event loop ended with result:" << result;
    
    return result;
}
