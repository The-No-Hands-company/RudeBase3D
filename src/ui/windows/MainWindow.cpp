#include "MainWindow.h"
#include "UIManager.h"
#include "ViewportManager.h"
#include "SceneHierarchyPanel.h"
#include "PropertiesPanel.h"
#include "RenderSystem.h"
#include "LightingSystem.h"
#include "GridSystem.h"
#include "Camera.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Material.h"
#include "AssetManager.h"
#include "FileFormatHandlers.h"
#include "ExtrudeTool.h"
#include "ModelingToolManager.h"
#include "EditContext.h"
#include "GeometryConverter.h"
#include "core/core_system.hpp"
#include "core/scene_manager.hpp"
#include "core/entity.hpp"
#include "core/selection_manager.hpp"
#include "panels/outliner_panel.hpp"
#include "panels/properties_panel.hpp"
#include "panels/selection_panel.hpp"
#include "toolbars/toolbar_manager.hpp"
#include "toolbars/main_toolbar.hpp"
#include "toolbars/primitives_toolbar.hpp"
#include "toolbars/selection_toolbar.hpp"
#include "toolbars/transform_toolbar.hpp"
#include "gizmo/gizmo_manager.hpp"
#include "ui/edit_preview_panel.hpp"

#include <QFileDialog>
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QSplitter>
#include <QLabel>
#include <QActionGroup>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_sceneManager(std::make_shared<rude::SceneManager>())
    , m_renderSystem(std::make_shared<RenderSystem>())
    , m_assetManager(std::make_shared<AssetManager>())
    , m_uiManager(std::make_shared<UIManager>(this, this))
    , m_extrudeTool(std::make_shared<ExtrudeTool>())
    , m_modelingToolManager(std::make_shared<ModelingToolManager>())
    , m_editContext(std::make_shared<EditContext>())
    , m_lightingSystem(std::make_shared<LightingSystem>())
    , m_gridSystem(std::make_shared<GridSystem>())
    , m_sceneModified(false)
{
    printf("[CONSTRUCTOR] MainWindow constructor started\n");
    qDebug() << "MainWindow constructor started";
    
    // Use CoreSystem for scene management instead of local scene manager
    auto& coreSystem = CoreSystem::getInstance();
    auto* coreSceneManager = coreSystem.getSceneManager();
    
    // Get the scene from CoreSystem instead of local manager
    if (coreSceneManager) {
        m_scene = coreSceneManager->getScene();
    } else {
        // Fallback to legacy scene manager during transition
        m_scene = m_sceneManager->getScene();
    }
    
    // Set up component dependencies
    m_renderSystem->setScene(m_scene);
    
    // Initialize and configure lighting system
    m_lightingSystem->setLightingPreset(LightingSystem::LightingPreset::Studio);
    
    // Initialize and configure grid system  
    m_gridSystem->setGridStyle(GridSystem::GridStyle::Maya);
    m_gridSystem->setGridSize(20.0f);
    m_gridSystem->setGridDivisions(20);
    m_gridSystem->setVisible(true);
    
    // Setup update timer for ECS system
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateSystems);
    m_updateTimer->start(16); // ~60 FPS (16ms)
    m_frameTimer.start();
    
    qDebug() << "Setting up UI...";
    setupUI();
    
    qDebug() << "Setting window properties...";
    // Set initial window properties
    setWindowTitle("RudeBase3D - Professional 3D Modeling Suite");
    setMinimumSize(1024, 768);
    resize(1280, 800);
    
    qDebug() << "Updating UI...";
    updateUI();
    
    // Add a test cube to verify viewport is working
    qDebug() << "Adding test cube to scene...";
    addTestPrimitive();
    
    qDebug() << "MainWindow constructor completed";
}

void MainWindow::setupUI()
{
    printf("[MAIN DEBUG] setupUI() called\n");
    // Set up UI using UIManager
    m_uiManager->setupUI();
    
    // Get the viewport manager and panels from UIManager
    m_viewportManager = m_uiManager->getViewportManager();
    printf("[MAIN DEBUG] ViewportManager obtained: %p\n", m_viewportManager);
    
    m_hierarchyPanel = m_uiManager->getSceneHierarchy();
    // TODO: Fix type mismatch - UIManager returns PropertiesPanel* but we need rude::PropertiesPanel*
    // m_propertiesPanel = m_uiManager->getPropertiesPanel();
    
    // Set up component integrations with ViewportManager
    printf("[MAIN DEBUG] Setting scene, lighting, grid, and render systems\n");
    m_viewportManager->setScene(m_scene);
    m_viewportManager->setLightingSystem(m_lightingSystem);
    printf("[MAIN DEBUG] About to call setGridSystem with: %p\n", m_gridSystem.get());
    m_viewportManager->setGridSystem(m_gridSystem);
    printf("[MAIN DEBUG] setGridSystem completed\n");
    m_viewportManager->setRenderSystem(m_renderSystem);
    printf("[MAIN DEBUG] All systems set\n");
    
    // Set camera controller type (Maya style by default)
    m_viewportManager->setCameraControllerType("Maya");
    
    // Set up modern panel system
    setupModernPanels();
    
    // Set up modern toolbar system
    setupModernToolbars();
    
    // Connect lighting and grid systems to viewport and render system
    // These are now handled by ViewportManager automatically
    
    // Note: ViewportManager now manages camera controllers internally
    // Legacy controller setup is no longer needed
    
    // Set up modeling tools
    m_modelingToolManager->setEditContext(m_editContext.get());
    
    // Connect component signals
    connectSignals();
}

void MainWindow::connectSignals()
{
    // UIManager now handles action connections and emits signals
    // Connect UIManager signals to MainWindow slots
    connect(m_uiManager.get(), &UIManager::newScene, this, &MainWindow::newScene);
    connect(m_uiManager.get(), &UIManager::openScene, this, &MainWindow::openScene);
    connect(m_uiManager.get(), &UIManager::saveScene, this, &MainWindow::saveScene);
    connect(m_uiManager.get(), &UIManager::saveSceneAs, this, &MainWindow::saveSceneAs);
    connect(m_uiManager.get(), &UIManager::quit, this, &MainWindow::exitApplication);
    
    // Create operations
    connect(m_uiManager.get(), &UIManager::createCube, this, &MainWindow::createCube);
    connect(m_uiManager.get(), &UIManager::createSphere, this, &MainWindow::createSphere);
    connect(m_uiManager.get(), &UIManager::createPlane, this, &MainWindow::createPlane);
    connect(m_uiManager.get(), &UIManager::createCylinder, this, &MainWindow::createCylinder);
    connect(m_uiManager.get(), &UIManager::createCone, this, &MainWindow::createCone);
    connect(m_uiManager.get(), &UIManager::createTorus, this, &MainWindow::createTorus);
    connect(m_uiManager.get(), &UIManager::createIcosphere, this, &MainWindow::createIcosphere);
    
    // Modeling tool connections
    connect(m_uiManager.get(), &UIManager::extrudeSelected, this, &MainWindow::beginExtrude);
    connect(m_uiManager.get(), &UIManager::insetSelected, this, &MainWindow::beginInset);
    connect(m_uiManager.get(), &UIManager::loopCutSelected, this, &MainWindow::beginLoopCut);
    connect(m_uiManager.get(), &UIManager::subdivideSelected, this, &MainWindow::beginSubdivision);
    connect(m_uiManager.get(), &UIManager::bevelSelected, this, &MainWindow::beginBevel);
    
    // View operations
    connect(m_uiManager.get(), &UIManager::resetCamera, this, &MainWindow::resetCamera);
    connect(m_uiManager.get(), &UIManager::frameScene, this, &MainWindow::frameScene);
    connect(m_uiManager.get(), &UIManager::frameSelected, this, &MainWindow::frameSelection);
    connect(m_uiManager.get(), &UIManager::toggleGrid, this, [this]() {
        // Toggle the current grid state
        bool currentState = m_gridSystem ? m_gridSystem->isVisible() : true;
        toggleGrid(!currentState);
    });
    
    // Render and transform modes
    connect(m_uiManager.get(), &UIManager::renderModeChanged, this, [this](RenderMode mode) {
        if (m_viewportManager) m_viewportManager->setGlobalRenderMode(mode);
    });
    connect(m_uiManager.get(), &UIManager::transformModeChanged, this, [this](TransformMode mode) {
        (void)mode; // Suppress unused parameter warning
        // TODO: Implement transform mode on ViewportManager
        // For now, this will be handled per viewport
    });
    
    // Panel visibility connections
    connect(m_uiManager.get(), &UIManager::showOutlinerPanel, this, [this](bool show) {
        if (m_outlinerPanel) m_outlinerPanel->setVisible(show);
    });
    connect(m_uiManager.get(), &UIManager::showPropertiesPanel, this, [this](bool show) {
        if (m_modernPropertiesPanel) m_modernPropertiesPanel->setVisible(show);
    });
    connect(m_uiManager.get(), &UIManager::showSelectionPanel, this, [this](bool show) {
        if (m_selectionPanel) m_selectionPanel->setVisible(show);
    });
    connect(m_uiManager.get(), &UIManager::showEditPreviewPanel, this, [this](bool show) {
        if (m_editPreviewPanel) m_editPreviewPanel->setVisible(show);
    });
    
    // Scene and viewport connections
    if (m_viewportManager) {
        connect(m_viewportManager, &ViewportManager::activeViewportChanged, 
                this, &MainWindow::onViewportChanged);
        // TODO: Connect individual viewport signals through the manager
    }
    
    // Scene manager connections
    if (m_sceneManager) {
        // TODO: rude::SceneManager doesn't have sceneChanged signal - need to implement or find alternative
        // connect(m_sceneManager.get(), &rude::SceneManager::sceneChanged, this, &MainWindow::updateStatusBar);
    }
    
    // Edit tool connections will be handled by UIManager
    // UIManager should emit signals for these edit operations
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newScene()
{
    qDebug() << "newScene() started";
    if (maybeSave()) {
        qDebug() << "maybeSave() returned true, creating new scene";
        
        // Use CoreSystem for scene management
        auto& coreSystem = CoreSystem::getInstance();
        auto* coreSceneManager = coreSystem.getSceneManager();
        auto* selectionManager = coreSystem.getSelectionManager();
        
        if (coreSceneManager) {
            // Clear current selection first
            if (selectionManager) {
                selectionManager->clearSelection();
            }
            // Create new scene through CoreSystem
            auto newScene = std::make_shared<rude::Scene>();
            coreSceneManager->setScene(newScene);
            m_scene = newScene;
            qDebug() << "Created new scene through CoreSystem";
        } else {
            // Fallback: create a new scene and assign to manager
            auto newScene = std::make_shared<rude::Scene>();
            m_sceneManager->setScene(newScene);
            m_scene = newScene;
            qDebug() << "Created new scene through fallback SceneManager";
        }
        // Update component references to new scene
        m_viewportManager->setScene(m_scene);
        qDebug() << "Set scene on viewport manager";
        m_renderSystem->setScene(m_scene);
        qDebug() << "Set scene on render system";
        
        setCurrentFile("");
        qDebug() << "Set current file";
        
        m_sceneModified = false;
        qDebug() << "About to call updateUI from newScene";
        updateUI();
        
        // ViewportManager now handles camera control and scene framing
        
        qDebug() << "newScene() completed";
    } else {
        qDebug() << "maybeSave() returned false";
    }
}

void MainWindow::openScene()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Import 3D Model", "",
        "3D Models (*.obj *.stl *.ply);;OBJ Files (*.obj);;STL Files (*.stl);;PLY Files (*.ply);;All Files (*)");
    
    if (!fileName.isEmpty()) {
        // Try to import the file using FileFormatHandlers
        std::string fileNameStd = fileName.toStdString();
        if (fileName.endsWith(".obj", Qt::CaseInsensitive)) {
            auto result = OBJFileHandler::importFromFile(fileNameStd);
            if (result.success && !result.meshes.empty()) {
                // Add all imported meshes to the scene
                for (size_t i = 0; i < result.meshes.size(); ++i) {
                    auto mesh = result.meshes[i];
                    QString objectName = (i < result.meshNames.size()) ? QString::fromStdString(result.meshNames[i]) : QString("Imported_Object_%1").arg(i);
                    std::string entityName = objectName.toStdString();
                    // TODO: Fix Entity type conflict - rude::Entity* vs Entity*
                    // Entity* entity = m_sceneManager->createEntity(entityName);
                    // TODO: Stub - SceneManager::createEntity not implemented yet
                    // auto rudeEntity = m_sceneManager->createEntity(entityName);
                    rude::Entity* rudeEntity = nullptr; // Stub for now
                    if (rudeEntity) {
                        // TODO: Calling setMesh on incomplete rude::Entity type - need proper include or type unification
                        // rudeEntity->setMesh(mesh);
                        // TODO: Assign material if your ECS supports it
                    }
                }
                
                if (!result.meshes.empty()) {
                    setCurrentFile(fileName);
                    m_sceneModified = false;
                    updateUI();
                    
                    // Frame the imported objects
                    frameScene();
                    
                    statusBar()->showMessage(QString("Imported %1 objects from %2")
                        .arg(result.meshes.size()).arg(QFileInfo(fileName).fileName()), 2000);
                }
            } else {
                QMessageBox::warning(this, "Import Error", 
                    QString("Failed to import file: %1\n\nError: %2").arg(fileName, result.errorMessage));
            }
        } else if (fileName.endsWith(".stl", Qt::CaseInsensitive)) {
            auto result = STLFileHandler::importFromFile(fileNameStd);
            if (result.success && result.mesh) {
                auto mesh = result.mesh;
                std::string entityName = QFileInfo(fileName).baseName().toStdString();
                // TODO: Fix Entity type conflict - rude::Entity* vs Entity*
                // Entity* entity = m_sceneManager->createEntity(entityName);
                // TODO: Stub - SceneManager::createEntity not implemented yet
                // auto rudeEntity = m_sceneManager->createEntity(entityName);
                rude::Entity* rudeEntity = nullptr; // Stub for now
                if (rudeEntity) {
                    // TODO: Calling setMesh on incomplete rude::Entity type - need proper include or type unification
                    // rudeEntity->setMesh(mesh);
                    // TODO: Assign material if your ECS supports it
                }
                setCurrentFile(fileName);
                m_sceneModified = false;
                updateUI();
                frameScene();
                statusBar()->showMessage(QString("Imported %1").arg(QFileInfo(fileName).fileName()), 2000);
            } else {
                QMessageBox::warning(this, "Import Error", 
                    QString("Failed to import STL file: %1\n\nError: %2").arg(fileName, result.errorMessage));
            }
        } else if (fileName.endsWith(".ply", Qt::CaseInsensitive)) {
            auto result = PLYFileHandler::importFromFile(fileNameStd);
            if (result.success && result.mesh) {
                auto mesh = result.mesh;
                std::string entityName = QFileInfo(fileName).baseName().toStdString();
                // TODO: Fix Entity type conflict - rude::Entity* vs Entity*
                // Entity* entity = m_sceneManager->createEntity(entityName);
                // TODO: Stub - SceneManager::createEntity not implemented yet
                // auto rudeEntity = m_sceneManager->createEntity(entityName);
                rude::Entity* rudeEntity = nullptr; // Stub for now
                if (rudeEntity) {
                    // TODO: Calling setMesh on incomplete rude::Entity type - need proper include or type unification
                    // rudeEntity->setMesh(mesh);
                    // TODO: Assign material if your ECS supports it
                }
                setCurrentFile(fileName);
                m_sceneModified = false;
                updateUI();
                frameScene();
                statusBar()->showMessage(QString("Imported %1").arg(QFileInfo(fileName).fileName()), 2000);
            } else {
                QMessageBox::warning(this, "Import Error", 
                    QString("Failed to import PLY file: %1\n\nError: %2").arg(fileName, result.errorMessage));
            }
        } else {
            QMessageBox::information(this, "Unsupported Format", 
                "Unsupported file format. Please select an OBJ, STL, or PLY file.");
        }
    }
}

void MainWindow::saveScene()
{
    if (m_currentFile.isEmpty()) {
        saveSceneAs();
    } else {
        // Quick save to current file
        // Use SelectionManager to get selected entity
        auto& coreSystem = CoreSystem::getInstance();
        auto* selectionManager = coreSystem.getSelectionManager();
        rude::Entity* selectedEntity = nullptr;
        if (selectionManager) {
            const auto& selectedEntities = selectionManager->getSelectedEntities();
            if (!selectedEntities.empty()) {
                selectedEntity = *selectedEntities.begin();
            }
        }
        if (!selectedEntity || !selectedEntity->getMesh()) {
            QMessageBox::information(this, "Save", 
                "Please select an object to save.\n\nNote: Currently only individual object export is supported.");
            return;
        }
        bool success = false;
        std::string currentFileStd = m_currentFile.toStdString();
        if (m_currentFile.endsWith(".obj", Qt::CaseInsensitive)) {
            success = OBJFileHandler::exportToFile(currentFileStd, selectedEntity->getMesh());
        } else if (m_currentFile.endsWith(".stl", Qt::CaseInsensitive)) {
            success = STLFileHandler::exportToFile(currentFileStd, selectedEntity->getMesh());
        }
        if (success) {
            m_sceneModified = false;
            updateUI();
            statusBar()->showMessage(QString("Saved %1").arg(QFileInfo(m_currentFile).fileName()), 2000);
        } else {
            QMessageBox::warning(this, "Save Error", 
                QString("Failed to save to file: %1").arg(m_currentFile));
        }
    }
}

void MainWindow::saveSceneAs()
{
    // Use SelectionManager to get selected entity
    auto& coreSystem = CoreSystem::getInstance();
    auto* selectionManager = coreSystem.getSelectionManager();
    rude::Entity* selectedEntity = nullptr;
    if (selectionManager) {
        const auto& selectedEntities = selectionManager->getSelectedEntities();
        if (!selectedEntities.empty()) {
            selectedEntity = *selectedEntities.begin();
        }
    }
    if (!selectedEntity || !selectedEntity->getMesh()) {
        QMessageBox::information(this, "Export", 
            "Please select an object to export.\n\nNote: Currently only individual object export is supported.");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this,
        "Export 3D Model", QString::fromStdString(selectedEntity->getName()),
        "OBJ Files (*.obj);;STL Files (*.stl);;All Files (*)");
    if (!fileName.isEmpty()) {
        bool success = false;
        std::string fileNameStd = fileName.toStdString();
        if (fileName.endsWith(".obj", Qt::CaseInsensitive)) {
            success = OBJFileHandler::exportToFile(fileNameStd, selectedEntity->getMesh());
        } else if (fileName.endsWith(".stl", Qt::CaseInsensitive)) {
            success = STLFileHandler::exportToFile(fileNameStd, selectedEntity->getMesh());
        } else if (fileName.endsWith(".ply", Qt::CaseInsensitive)) {
            success = PLYFileHandler::exportToFile(fileNameStd, selectedEntity->getMesh());
        } else {
            // Default to OBJ if no extension specified
            if (!fileName.contains('.')) {
                fileName += ".obj";
                fileNameStd = fileName.toStdString();
            }
            success = OBJFileHandler::exportToFile(fileNameStd, selectedEntity->getMesh());
        }
        if (success) {
            statusBar()->showMessage(QString("Exported %1 to %2")
                .arg(QString::fromStdString(selectedEntity->getName()), QFileInfo(fileName).fileName()), 2000);
        } else {
            QMessageBox::warning(this, "Export Error", 
                QString("Failed to export to file: %1").arg(fileName));
        }
    }
}

void MainWindow::exitApplication()
{
    close();
}

void MainWindow::createCube()
{
    createPrimitiveHelper("cube");
}

void MainWindow::createSphere()
{
    createPrimitiveHelper("sphere");
}

void MainWindow::createCylinder()
{
    createPrimitiveHelper("cylinder");
}

void MainWindow::createPlane()
{
    createPrimitiveHelper("plane");
}

void MainWindow::createCone()
{
    createPrimitiveHelper("cone");
}

void MainWindow::createTorus()
{
    createPrimitiveHelper("torus");
}

void MainWindow::createIcosphere()
{
    createPrimitiveHelper("icosphere");
}

void MainWindow::resetCamera()
{
    m_viewportManager->resetAllCameras();
}

void MainWindow::frameScene()
{
    m_viewportManager->frameSceneAll();
}

void MainWindow::frameSelection()
{
    m_viewportManager->frameSelectionAll();
}

void MainWindow::toggleGrid(bool show)
{
    if (m_gridSystem) {
        m_gridSystem->setVisible(show);
    }
}

void MainWindow::setWireframeMode()
{
    m_viewportManager->setGlobalRenderMode(RenderMode::Wireframe);
}

void MainWindow::setSolidMode()
{
    m_viewportManager->setGlobalRenderMode(RenderMode::Solid);
}

void MainWindow::setSolidWireframeMode()
{
    m_viewportManager->setGlobalRenderMode(RenderMode::SolidWireframe);
}

void MainWindow::setSelectMode()
{
    // TODO: Implement transform mode on ViewportManager
    // For now, this will be handled per viewport
    if (auto activeViewport = m_viewportManager->getActiveViewport()) {
        // activeViewport->setTransformMode(TransformMode::Select);
    }
}

void MainWindow::setTranslateMode()
{
    // TODO: Implement transform mode on ViewportManager
    if (auto activeViewport = m_viewportManager->getActiveViewport()) {
        // activeViewport->setTransformMode(TransformMode::Translate);
    }
}

void MainWindow::setRotateMode()
{
    // TODO: Implement transform mode on ViewportManager
    if (auto activeViewport = m_viewportManager->getActiveViewport()) {
        // activeViewport->setTransformMode(TransformMode::Rotate);
    }
}

void MainWindow::setScaleMode()
{
    // TODO: Implement transform mode on ViewportManager
    if (auto activeViewport = m_viewportManager->getActiveViewport()) {
        // activeViewport->setTransformMode(TransformMode::Scale);
    }
}

void MainWindow::setObjectSelection()
{
    // TODO: Implement selection type on ViewportManager
    if (auto activeViewport = m_viewportManager->getActiveViewport()) {
        // activeViewport->setSelectionType(SelectionType::Object);
    }
}

void MainWindow::setVertexSelection()
{
    // TODO: Implement selection type on ViewportManager
    if (auto activeViewport = m_viewportManager->getActiveViewport()) {
        // activeViewport->setSelectionType(SelectionType::Vertex);
    }
}

void MainWindow::setEdgeSelection()
{
    // TODO: Implement selection type on ViewportManager
    if (auto activeViewport = m_viewportManager->getActiveViewport()) {
        // activeViewport->setSelectionType(SelectionType::Edge);
    }
}

void MainWindow::setFaceSelection()
{
    // TODO: Implement selection type on ViewportManager
    if (auto activeViewport = m_viewportManager->getActiveViewport()) {
        // activeViewport->setSelectionType(SelectionType::Face);
    }
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About RudeBase3D",
                      "<h2>RudeBase3D</h2>"
                      "<p>Version 1.0.0</p>"
                      "<p>A rudimentary 3D modeling application built with C++ and Qt.</p>"
                      "<p>Features:</p>"
                      "<ul>"
                      "<li>Basic 3D primitives (Cube, Sphere, Cylinder, Plane)</li>"
                      "<li>3D viewport with OpenGL rendering</li>"
                      "<li>Camera navigation (orbit, pan, zoom)</li>"
                      "<li>Object transformation tools</li>"
                      "<li>Scene hierarchy management</li>"
                      "<li>Properties panel for object editing</li>"
                      "<li>Wireframe and solid rendering modes</li>"
                      "<li>Basic material system</li>"
                      "</ul>"
                      "<p>Copyright © 2025 RudeBase3D</p>");
}

void MainWindow::onObjectSelected(SceneObjectPtr object)
{
    Q_UNUSED(object)
    updateUI();
}

void MainWindow::onTransformModeChanged(TransformMode mode)
{
    // Transform mode changes are now handled by UIManager
    // UIManager will update the appropriate action states
    Q_UNUSED(mode)
}

void MainWindow::updateUI()
{
    updateWindowTitle();
    // Status bar updates are now handled by UIManager
}

void MainWindow::updateWindowTitle()
{
    QString title = "RudeBase3D - 3D Modeling Application";
    
    if (!m_currentFile.isEmpty()) {
        title = QString("%1 - %2").arg(QFileInfo(m_currentFile).baseName(), title);
    } else {
        title = QString("Untitled - %1").arg(title);
    }
    
    if (m_sceneModified) {
        title += " *";
    }
    
    setWindowTitle(title);
}

void MainWindow::updateStatusBar()
{
    // Status bar updates are now handled by UIManager
    // UIManager will manage the status bar labels and updates
}

bool MainWindow::maybeSave()
{
    if (m_sceneModified) {
        QMessageBox::StandardButton ret = QMessageBox::warning(this, "RudeBase3D",
                     "The scene has been modified.\n"
                     "Do you want to save your changes?",
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        
        if (ret == QMessageBox::Save) {
            return true; // Would normally save here
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::setCurrentFile(const QString& fileName)
{
    m_currentFile = fileName;
    m_sceneModified = false;
    updateWindowTitle();
}

void MainWindow::createTestScene()
{
    // TODO: Re-implement test scene creation using SceneManager
    qDebug() << "Test scene creation temporarily disabled";
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destructor called";
    
    // Disconnect all signals to prevent callback issues during destruction
    if (m_uiManager) {
        disconnect(m_uiManager.get(), nullptr, this, nullptr);
    }
    
    // Critical: Clear viewport manager's shared_ptr references before MainWindow's 
    // shared_ptr members are destroyed to prevent use-after-free
    if (m_viewportManager) {
        // Clear all shared_ptr references in ViewportManager
        m_viewportManager->setScene(nullptr);
        m_viewportManager->setLightingSystem(nullptr);
        m_viewportManager->setGridSystem(nullptr);
        m_viewportManager->setRenderSystem(nullptr);
    }
    
    // Clear panel references
    if (m_hierarchyPanel) {
        m_hierarchyPanel->setScene(nullptr);
    }
    if (m_propertiesPanel) {
        m_propertiesPanel->setEntity(nullptr);
    }
    
    // Clear other cross-references
    if (m_renderSystem) {
        m_renderSystem->setScene(nullptr);
        m_renderSystem->setCamera(nullptr);
    }
    
    // Clear modeling tool manager and edit context
    if (m_modelingToolManager) {
        m_modelingToolManager->setEditContext(nullptr);
    }
    if (m_editContext) {
        m_editContext->setActiveObject(nullptr);
        m_editContext->clearSelection();
    }
    
    // Explicitly reset shared_ptrs to control destruction order
    // Reset in reverse dependency order
    m_modelingToolManager.reset();
    m_editContext.reset();
    m_extrudeTool.reset();
    m_lightingSystem.reset();
    m_gridSystem.reset();
    m_renderSystem.reset();
    m_assetManager.reset();
    
    // Reset UI manager last since other components may depend on it
    m_uiManager.reset();
    m_sceneManager.reset();
    
    qDebug() << "MainWindow destructor completed";
    // Note: m_scene is owned by SceneManager, so it will be cleaned up when SceneManager is destroyed
}

// Edit tool methods
void MainWindow::beginExtrude()
{
    if (!m_extrudeTool || !m_viewportManager) {
        qDebug() << "Cannot begin extrude: missing tools";
        return;
    }
    
    // Get the current mesh from the selected object
    // Use SelectionManager to get selected entity
    auto& coreSystem = CoreSystem::getInstance();
    auto* selectionManager = coreSystem.getSelectionManager();
    rude::Entity* selectedEntity = nullptr;
    if (selectionManager) {
        const auto& selectedEntities = selectionManager->getSelectedEntities();
        if (!selectedEntities.empty()) {
            selectedEntity = *selectedEntities.begin();
        }
    }
    if (!selectedEntity || !selectedEntity->getMesh()) {
        qDebug() << "Cannot extrude: no mesh selected";
        return;
    }
    // Convert to HalfEdgeMesh for editing
    auto halfEdgeMesh = GeometryConverter::toHalfEdge(selectedEntity->getMesh());
    if (!halfEdgeMesh) {
        qDebug() << "Failed to convert mesh to HalfEdge format";
        return;
    }
    // Set up the extrude tool
    m_extrudeTool->setMesh(halfEdgeMesh);
    // TODO: Update selection manager access for ViewportManager
    // m_extrudeTool->setSelectionManager(m_viewportManager->getActiveViewport()->getSelectionManager());
    // Begin the extrude operation
    if (m_extrudeTool->beginExtrude()) {
        qDebug() << "Extrude operation started";
        statusBar()->showMessage("Extrude active - Move mouse to set distance, Enter to confirm, Escape to cancel");
    }
}

void MainWindow::confirmEdit()
{
    if (!m_extrudeTool) return;
    
    if (m_extrudeTool->isExtruding()) {
        m_extrudeTool->confirmExtrude();
        
        qDebug() << "Edit operation confirmed";
        statusBar()->showMessage("Edit operation confirmed", 2000);
        
        // Update the viewport
        if (auto activeViewport = m_viewportManager->getActiveViewport()) {
            activeViewport->update();
        }
    }
}

void MainWindow::cancelEdit()
{
    if (!m_extrudeTool) return;
    
    if (m_extrudeTool->isExtruding()) {
        m_extrudeTool->cancelExtrude();
        
        qDebug() << "Edit operation cancelled";
        statusBar()->showMessage("Edit operation cancelled", 2000);
        
        // Update the viewport
        if (auto activeViewport = m_viewportManager->getActiveViewport()) {
            activeViewport->update();
        }
    }
}

void MainWindow::beginInset()
{
    if (!m_modelingToolManager) {
        qDebug() << "Cannot begin inset: ModelingToolManager not available";
        return;
    }
    
    // TODO: Set up EditContext with current selection
    if (m_modelingToolManager->executeInset()) {
        qDebug() << "Inset operation started";
        statusBar()->showMessage("Inset operation completed", 2000);
        if (auto activeViewport = m_viewportManager->getActiveViewport()) {
            activeViewport->update();
        }
    } else {
        qDebug() << "Failed to execute inset operation";
        statusBar()->showMessage("Inset failed - ensure faces are selected", 2000);
    }
}

void MainWindow::beginLoopCut()
{
    if (!m_modelingToolManager) {
        qDebug() << "Cannot begin loop cut: ModelingToolManager not available";
        return;
    }
    
    // TODO: Set up EditContext with current selection
    if (m_modelingToolManager->executeLoopCut()) {
        qDebug() << "Loop cut operation started";
        statusBar()->showMessage("Loop cut operation completed", 2000);
        if (auto activeViewport = m_viewportManager->getActiveViewport()) {
            activeViewport->update();
        }
    } else {
        qDebug() << "Failed to execute loop cut operation";
        statusBar()->showMessage("Loop cut failed - ensure edges are selected", 2000);
    }
}

void MainWindow::beginSubdivision()
{
    if (!m_modelingToolManager) {
        qDebug() << "Cannot begin subdivision: ModelingToolManager not available";
        return;
    }
    
    // TODO: Set up EditContext with current selection
    if (m_modelingToolManager->executeSubdivision()) {
        qDebug() << "Subdivision operation started";
        statusBar()->showMessage("Subdivision operation completed", 2000);
        if (auto activeViewport = m_viewportManager->getActiveViewport()) {
            activeViewport->update();
        }
    } else {
        qDebug() << "Failed to execute subdivision operation";
        statusBar()->showMessage("Subdivision failed - ensure faces are selected", 2000);
    }
}

void MainWindow::beginBevel()
{
    if (!m_modelingToolManager) {
        qDebug() << "Cannot begin bevel: ModelingToolManager not available";
        return;
    }
    
    // TODO: Set up EditContext with current selection
    if (m_modelingToolManager->executeBevel()) {
        qDebug() << "Bevel operation started";
        statusBar()->showMessage("Bevel operation completed", 2000);
        if (auto activeViewport = m_viewportManager->getActiveViewport()) {
            activeViewport->update();
        }
    } else {
        qDebug() << "Failed to execute bevel operation";
        statusBar()->showMessage("Bevel failed - ensure edges or vertices are selected", 2000);
    }
}

void MainWindow::onViewportChanged(ViewportWidget* viewport)
{
    // Handle viewport activation change
    if (viewport) {
        qDebug() << "Active viewport changed to:" << viewport->getViewName();
        this->updateStatusBar();
    }
}

void MainWindow::setupModernPanels()
{
    qDebug() << "Setting up modern panel system...";
    
    // Get reference to core system for scene and selection management
    auto& coreSystem = CoreSystem::getInstance();
    auto* sceneManager = coreSystem.getSceneManager();
    auto* selectionManager = coreSystem.getSelectionManager();
    
    // Create the modern outliner panel
    m_outlinerPanel = new OutlinerPanel(this);
    m_outlinerPanel->setWindowTitle("Outliner");
    m_outlinerPanel->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    // Set the scene in the outliner panel
    if (sceneManager) {
        // Note: OutlinerPanel expects a raw Scene pointer, not shared_ptr
        // This is a temporary adaptation during the transition
        m_outlinerPanel->setScene(sceneManager->getScene().get());
    }
    
    // Create the modern properties panel
    m_modernPropertiesPanel = new PropertiesPanel(this);
    m_modernPropertiesPanel->setWindowTitle("Properties");
    m_modernPropertiesPanel->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    // Create the selection panel
    m_selectionPanel = new SelectionPanel(this);
    m_selectionPanel->setWindowTitle("Selection");
    m_selectionPanel->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    // Create the edit preview panel
    auto* editPreviewSystem = coreSystem.getEditPreviewSystem();
    if (editPreviewSystem) {
        // Note: Using shared_ptr with no-op deleter is safe here because:
        // 1. EditPreviewSystem is owned by CoreSystem singleton (same lifetime as application)
        // 2. EditPreviewPanel is destroyed before CoreSystem during application shutdown
        // 3. This avoids exposing raw pointer interface in EditPreviewPanel constructor
        std::shared_ptr<RudeBase3D::Core::EditPreviewSystem> sharedPreviewSystem(editPreviewSystem, [](RudeBase3D::Core::EditPreviewSystem*){});
        m_editPreviewPanel = new RudeBase3D::UI::EditPreviewPanel(sharedPreviewSystem, this);
        m_editPreviewPanel->setWindowTitle("Edit Preview");
        m_editPreviewPanel->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    }
    
    // Connect the selection panel to the core system's selection manager
    if (selectionManager) {
        m_selectionPanel->setSelectionManager(selectionManager);
    }
    
    // Add panels to the right dock area
    addDockWidget(Qt::RightDockWidgetArea, m_outlinerPanel);
    addDockWidget(Qt::RightDockWidgetArea, m_modernPropertiesPanel);
    addDockWidget(Qt::RightDockWidgetArea, m_selectionPanel);
    if (m_editPreviewPanel) {
        addDockWidget(Qt::RightDockWidgetArea, m_editPreviewPanel);
    }
    
    // Tabify the panels
    tabifyDockWidget(m_outlinerPanel, m_modernPropertiesPanel);
    tabifyDockWidget(m_modernPropertiesPanel, m_selectionPanel);
    if (m_editPreviewPanel) {
        tabifyDockWidget(m_selectionPanel, m_editPreviewPanel);
    }
    
    // Ensure outliner is visible by default
    m_outlinerPanel->raise();
    
    // Connect panel signals to appropriate slots
    connect(m_outlinerPanel, &OutlinerPanel::selectionChanged, this, [this, selectionManager](rude::Entity* entity) {
        if (selectionManager && entity) {
            // Update selection in the selection manager
            // This is a temporary solution during the transition period
            // We'll need to adapt the selection system properly
            rude::SelectionData selData;
            selData.type = rude::ComponentType::Entity;
            selData.entity = entity;
            selectionManager->clearSelection();
            selectionManager->selectEntity(entity, rude::SelectionMode::Replace);
            
            // Also update properties panel
            if (m_modernPropertiesPanel) {
                m_modernPropertiesPanel->setEntity(entity);
            }
        }
    });
    
    // Connect selection panel signals to core system
    connect(m_selectionPanel, &SelectionPanel::selectionModeChanged, this, [this, selectionManager](rude::ComponentType mode) {
        qDebug() << "Selection mode changed to:" << static_cast<int>(mode);
        // Update the selection manager's current mode
        if (selectionManager) {
            // Set the selection mode in the manager - this would be implemented in SelectionManager
            // selectionManager->setSelectionMode(mode);
        }
        
        // Update the viewport to reflect the new selection mode
        if (m_viewportManager) {
            // This would update any selection visualization in the viewport
            // m_viewportManager->setSelectionMode(mode);
        }
    });
    
    connect(m_selectionPanel, &SelectionPanel::operationRequested, this, [this](const QString& operation) {
        qDebug() << "Operation requested:" << operation;
        // Route operation to appropriate handler
        if (operation == "extrude") {
            beginExtrude();
        } else if (operation == "bevel") {
            beginBevel();
        } else if (operation == "subdivide") {
            beginSubdivision();
        }
    });
    
    qDebug() << "Modern panel system setup complete";
}

void MainWindow::setupModernToolbars()
{
    qDebug() << "Setting up modern toolbar system...";
    
    // Create the toolbar manager
    m_toolbarManager = std::make_unique<ToolbarManager>(this);
    
    // Create all toolbars
    m_toolbarManager->createAllToolbars();
    
    // Set up the toolbar layout
    m_toolbarManager->setupToolbarLayout();
    
    // Connect toolbar actions to the appropriate slots
    auto* mainToolbar = m_toolbarManager->getMainToolbar();
    auto* primitivesToolbar = m_toolbarManager->getPrimitivesToolbar();
    auto* selectionToolbar = m_toolbarManager->getSelectionToolbar();
    auto* transformToolbar = m_toolbarManager->getTransformToolbar();
    
    // Connect main toolbar signals (view modes and camera controls)
    if (mainToolbar) {
        connect(mainToolbar, &MainToolbar::viewModeChanged, this, [this](const QString& mode) {
            qDebug() << "View mode changed to:" << mode;
            // Update viewport rendering mode
            if (m_viewportManager) {
                // Map string mode to RenderMode enum
                RenderMode renderMode = RenderMode::Solid;
                if (mode == "wireframe") {
                    renderMode = RenderMode::Wireframe;
                } else if (mode == "solid") {
                    renderMode = RenderMode::Solid;
                } else if (mode == "material" || mode == "rendered") {
                    renderMode = RenderMode::SolidWireframe;  // Use combined mode for advanced views
                }
                
                for (int i = 0; i < m_viewportManager->getViewportCount(); ++i) {
                    auto* viewport = m_viewportManager->getViewport(i);
                    if (viewport) {
                        viewport->setRenderMode(renderMode);
                    }
                }
            }
        });
        
        connect(mainToolbar, &MainToolbar::cameraResetRequested, this, [this]() {
            qDebug() << "Camera reset requested";
            if (m_viewportManager) {
                auto* viewport = m_viewportManager->getActiveViewport();
                if (viewport) {
                    viewport->resetCamera();
                }
            }
        });
        
        connect(mainToolbar, &MainToolbar::frameSelectedRequested, this, [this]() {
            qDebug() << "Frame selected requested";
            if (m_viewportManager) {
                auto* viewport = m_viewportManager->getActiveViewport();
                if (viewport) {
                    viewport->frameSelection();
                }
            }
        });
        
        connect(mainToolbar, &MainToolbar::frameSceneRequested, this, [this]() {
            qDebug() << "Frame scene requested";
            if (m_viewportManager) {
                auto* viewport = m_viewportManager->getActiveViewport();
                if (viewport) {
                    viewport->frameScene();
                }
            }
        });
        
        connect(mainToolbar, &MainToolbar::gridToggleRequested, this, [this]() {
            qDebug() << "Grid toggle requested";
            if (m_viewportManager) {
                for (int i = 0; i < m_viewportManager->getViewportCount(); ++i) {
                    auto* viewport = m_viewportManager->getViewport(i);
                    if (viewport) {
                        // Toggle grid visibility - need to track current state
                        static bool gridVisible = true;
                        gridVisible = !gridVisible;
                        viewport->setShowGrid(gridVisible);
                    }
                }
            }
        });
    }
    
    // Connect selection toolbar signals (selection and component modes)
    if (selectionToolbar) {
        // These connections will be implemented once SelectionToolbar is refactored
        // connect(selectionToolbar, &SelectionToolbar::selectionModeChanged, ...);
        // connect(selectionToolbar, &SelectionToolbar::selectionToolChanged, ...);
    }
    
    // Connect transform toolbar signals (transformation tools)
    if (transformToolbar) {
        // These connections will be implemented once TransformToolbar is refactored
        // connect(transformToolbar, &TransformToolbar::transformToolChanged, ...);
    }
    
    // Connect primitives toolbar to create functions
    if (primitivesToolbar) {
        // These connections would be implemented to hook up primitive creation
        // to existing create methods
        // connect(primitivesToolbar, &PrimitivesToolbar::createPrimitiveRequested, 
        //         this, &MainWindow::handlePrimitiveCreation);
    }
    
    // Connect selection toolbar to selection system
    if (selectionToolbar) {
        // connect(selectionToolbar, &SelectionToolbar::selectionModeChanged,
        //         this, &MainWindow::handleSelectionModeChange);
    }
    
    // Connect transform toolbar to transform system
    if (transformToolbar) {
        // connect(transformToolbar, &TransformToolbar::transformModeChanged,
        //         this, &MainWindow::handleTransformModeChange);
    }
    
    qDebug() << "Modern toolbar system setup complete";
}

void MainWindow::createPrimitiveHelper(const QString& primitiveType)
{
    // Use CoreSystem for primitive creation
    auto& coreSystem = CoreSystem::getInstance();
    auto* coreSceneManager = coreSystem.getSceneManager();
    (void)coreSceneManager; // Suppress unused variable warning - TODO: Use when Entity types are unified
    auto* selectionManager = coreSystem.getSelectionManager();

    std::string primitiveTypeName = primitiveType.toStdString();
    // TODO: Fix Entity type conflict - rude::Entity* vs Entity*
    // Entity* entity = m_sceneManager->createPrimitive(primitiveTypeName);
    // TODO: Stub - SceneManager::createPrimitive not implemented yet
    // auto rudeEntity = m_sceneManager->createPrimitive(primitiveTypeName);
    rude::Entity* rudeEntity = nullptr; // Stub for now
    if (rudeEntity) {
        if (selectionManager) {
            selectionManager->clearSelection();
            // TODO: Fix Entity type mismatch - selectEntity expects rude::Entity* but we have Entity*
            // selectionManager->selectEntity(entity, rude::SelectionMode::Replace);
        }
        // m_sceneManager->setSelectedObject(entity); // Selection is now handled by SelectionManager
        m_sceneModified = true;
        updateUI();
        if (m_outlinerPanel) {
            m_outlinerPanel->updateEntityList();
        }
        qDebug() << "Created" << primitiveType << "primitive";
    }
}

void MainWindow::updateSystems() {
    // Calculate delta time
    qint64 elapsed = m_frameTimer.restart();
    m_deltaTime = elapsed / 1000.0f; // Convert to seconds
    
    // Clamp delta time to prevent huge jumps during debugging or when window is inactive
    if (m_deltaTime > 0.033f) { // Max 30 FPS to prevent huge steps
        m_deltaTime = 0.033f;
    }
    
    // Update core systems through CoreSystem
    auto& coreSystem = CoreSystem::getInstance();
    coreSystem.update(m_deltaTime);
    
    // You could also update other systems here if needed
    // For example, viewport updates, UI animations, etc.
}

void MainWindow::addTestPrimitive()
{
    // Add a test cube to the scene to verify viewport functionality
    auto* coreSceneManager = CoreSystem::getInstance().getSceneManager();
    if (coreSceneManager) {
        auto* testCube = coreSceneManager->createPrimitive("cube", "TestCube");
        if (testCube) {
            qDebug() << "Test cube created successfully - Entity ID:" << testCube->getId();
        } else {
            qDebug() << "Failed to create test cube";
        }
    } else {
        qDebug() << "No scene manager available for test cube";
    }
}
