//==============================================================================
// RudeBase3D Engine - main.cpp
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

/**
 * @file main.cpp
 * @brief Application entry point for RudeBase3D
 * 
 * This file contains the main() function and application initialization logic.
 * It sets up the Qt application, initializes core systems, applies themes,
 * and creates the main window.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * @copyright Copyright (c) 2025 The-No-Hands-company
 * 
 * ## Initialization Sequence
 * 1. Create QApplication instance
 * 2. Set application metadata
 * 3. Initialize theme system with auto-detection
 * 4. Initialize core systems
 * 5. Create and show main window
 * 6. Run event loop
 * 7. Clean shutdown of core systems
 * 
 * ## Architecture Notes
 * - Uses singleton pattern for CoreSystem access
 * - Automatic theme detection based on system preferences
 * - Proper resource cleanup on application exit
 */

#include <QApplication>
#include <QDebug>
#include "ui/windows/MainWindow.h"
#include "ui/core/ThemeManager.h"
#include "core/core_system.hpp"

/**
 * @brief Helper function to access CoreSystem singleton
 * 
 * Provides convenient access to the CoreSystem singleton instance
 * throughout the application.
 * 
 * @return Reference to the CoreSystem singleton
 * 
 * @see CoreSystem::getInstance()
 */
CoreSystem& core() {
    return CoreSystem::getInstance();
}

/**
 * @brief Main application entry point
 * 
 * Initializes the RudeBase3D application, sets up all core systems,
 * and runs the main event loop.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * 
 * @return Application exit code (0 for success, non-zero for error)
 * 
 * @note This function handles complete application lifecycle:
 *       - Qt application setup and configuration
 *       - Theme system initialization with auto-detection
 *       - Core system initialization (ECS, Scene, Selection, etc.)
 *       - Main window creation and display
 *       - Event loop execution
 *       - Graceful shutdown and cleanup
 * 
 * ## Error Handling
 * The function includes comprehensive logging at each stage to help
 * with debugging initialization issues.
 * 
 * ## Memory Management
 * Uses RAII principles - all resources are automatically cleaned up
 * when the function exits, including explicit core system shutdown.
 */
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
    
    // Initialize the core system
    qDebug() << "Initializing core systems...";
    CoreSystem& coreSys = core();
    coreSys.initialize();
    
    qDebug() << "Creating main window...";
    
    // Create and show main window
    MainWindow window;
    
    qDebug() << "Showing main window...";
    window.show();
    
    qDebug() << "Window shown, starting event loop...";
    int result = app.exec();
    qDebug() << "Event loop ended with result:" << result;
    
    // Shutdown core systems
    qDebug() << "Shutting down core systems...";
    coreSys.shutdown();
    
    return result;
}
