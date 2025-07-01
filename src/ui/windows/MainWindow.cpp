#include "MainWindow.h"
#include "UIManager.h"
#include "SceneManager.h"
#include "Viewport3D.h"
#include "SceneHierarchyPanel.h"
#include "PropertiesPanel.h"
#include "InputController.h"
#include "RenderSystem.h"
#include "LightingSystem.h"
#include "GridSystem.h"
#include "Camera.h"
#include "CameraController.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Material.h"
#include "AssetManager.h"
#include "FileFormatHandlers.h"
#include "ExtrudeTool.h"
#include "GeometryConverter.h" // Add GeometryConverter include
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
    , m_sceneManager(std::make_shared<SceneManager>(this))
    , m_cameraController(std::make_shared<CameraController>(this))
    , m_inputController(std::make_shared<InputController>(this))
    , m_renderSystem(std::make_shared<RenderSystem>(this))
    , m_assetManager(std::make_shared<AssetManager>(this))
    , m_uiManager(std::make_shared<UIManager>(this, this))
    , m_extrudeTool(std::make_shared<ExtrudeTool>())
    , m_lightingSystem(std::make_shared<LightingSystem>())
    , m_gridSystem(std::make_shared<GridSystem>())
    , m_sceneModified(false)
{
    qDebug() << "MainWindow constructor started";
    
    // Create initial scene through SceneManager
    m_sceneManager->newScene();
    m_scene = m_sceneManager->getScene();
    
    // Set up component dependencies
    m_cameraController->setScene(m_scene);
    m_renderSystem->setScene(m_scene);
    
    // Initialize and configure lighting system
    m_lightingSystem->setLightingPreset(LightingSystem::LightingPreset::Studio);
    
    // Initialize and configure grid system  
    m_gridSystem->setGridStyle(GridSystem::GridStyle::Maya);
    m_gridSystem->setGridSize(20.0f);
    m_gridSystem->setGridDivisions(20);
    m_gridSystem->setVisible(true);
    
    qDebug() << "Setting up UI...";
    setupUI();
    
    qDebug() << "Setting window properties...";
    // Set initial window properties
    setWindowTitle("RudeBase3D - Professional 3D Modeling Suite");
    setMinimumSize(1024, 768);
    resize(1280, 800);
    
    qDebug() << "Updating UI...";
    updateUI();
    
    qDebug() << "MainWindow constructor completed";
}

void MainWindow::setupUI()
{
    // Set up UI using UIManager
    m_uiManager->setupUI();
    
    // Get the viewport and panels from UIManager
    m_viewport = m_uiManager->getViewport();
    m_hierarchyPanel = m_uiManager->getSceneHierarchy();
    m_propertiesPanel = m_uiManager->getPropertiesPanel();
    
    // Set up component integrations - use MainWindow's controllers
    m_viewport->setScene(m_scene);
    m_viewport->setCameraController(m_cameraController);
    m_viewport->setInputController(m_inputController);
    
    // Connect lighting and grid systems to viewport and render system
    m_viewport->setLightingSystem(m_lightingSystem);
    m_viewport->setGridSystem(m_gridSystem);
    m_renderSystem->setLightingSystem(m_lightingSystem);
    m_renderSystem->setGridSystem(m_gridSystem);
    
    // Set up MainWindow's controllers
    m_cameraController->setCamera(m_viewport->getCamera());
    m_cameraController->setScene(m_scene);
    m_inputController->setCameraController(m_cameraController);
    m_inputController->setScene(m_scene);
    m_inputController->setViewport(m_viewport);
    m_inputController->setSelectionManager(m_viewport->getSelectionManager()); // Connect SelectionManager to InputController
    m_renderSystem->setCamera(m_viewport->getCamera());
    
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
        if (m_viewport) m_viewport->setRenderMode(mode);
    });
    connect(m_uiManager.get(), &UIManager::transformModeChanged, this, [this](TransformMode mode) {
        if (m_viewport) m_viewport->setTransformMode(mode);
    });
    
    // Scene and viewport connections
    if (m_viewport && m_scene) {
        connect(m_viewport, &Viewport3D::objectSelected, this, &MainWindow::onObjectSelected);
        connect(m_viewport, &Viewport3D::transformModeChanged, this, &MainWindow::onTransformModeChanged);
    }
    
    // Scene manager connections
    if (m_sceneManager) {
        connect(m_sceneManager.get(), &SceneManager::sceneChanged, this, &MainWindow::updateStatusBar);
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
        m_sceneManager->newScene();
        qDebug() << "SceneManager newScene() completed";
        
        m_scene = m_sceneManager->getScene();
        qDebug() << "Got scene from SceneManager";
        
        // Update component references to new scene
        m_viewport->setScene(m_scene);
        qDebug() << "Set scene on viewport";
        
        m_cameraController->setScene(m_scene);
        qDebug() << "Set scene on camera controller";
        
        m_renderSystem->setScene(m_scene);
        qDebug() << "Set scene on render system";
        
        m_inputController->setScene(m_scene);
        qDebug() << "Set scene on input controller";
        
        setCurrentFile("");
        qDebug() << "Set current file";
        
        m_sceneModified = false;
        qDebug() << "About to call updateUI from newScene";
        updateUI();
        
        // Frame the scene to show all objects properly
        if (m_cameraController) {
            qDebug() << "Auto-framing scene after creation";
            m_cameraController->frameScene();
        }
        
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
        if (fileName.endsWith(".obj", Qt::CaseInsensitive)) {
            auto result = OBJFileHandler::importFromFile(fileName);
            if (result.success && !result.meshes.empty()) {
                // Add all imported meshes to the scene
                for (size_t i = 0; i < result.meshes.size(); ++i) {
                    auto mesh = result.meshes[i];
                    QString objectName = (i < result.meshNames.size()) ? result.meshNames[i] : QString("Imported_Object_%1").arg(i);
                    
                    auto object = std::make_shared<SceneObject>(objectName);
                    object->setMesh(mesh);
                    
                    // Create a default material
                    auto material = std::make_shared<Material>();
                    material->setDiffuseColor(QVector4D(0.7f, 0.7f, 0.7f, 1.0f));
                    material->setSpecularColor(QVector4D(0.3f, 0.3f, 0.3f, 1.0f));
                    material->setShininess(32.0f);
                    object->setMaterial(material);
                    
                    m_sceneManager->addObject(object);
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
            auto result = STLFileHandler::importFromFile(fileName);
            if (result.success && result.mesh) {
                auto mesh = result.mesh; // STL contains one mesh
                
                auto object = std::make_shared<SceneObject>(QFileInfo(fileName).baseName());
                object->setMesh(mesh);
                
                auto material = std::make_shared<Material>();
                material->setDiffuseColor(QVector4D(0.7f, 0.7f, 0.7f, 1.0f));
                material->setSpecularColor(QVector4D(0.3f, 0.3f, 0.3f, 1.0f));
                material->setShininess(32.0f);
                object->setMaterial(material);
                
                m_sceneManager->addObject(object);
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
            auto result = PLYFileHandler::importFromFile(fileName);
            if (result.success && result.mesh) {
                auto mesh = result.mesh; // PLY contains one mesh
                
                auto object = std::make_shared<SceneObject>(QFileInfo(fileName).baseName());
                object->setMesh(mesh);
                
                auto material = std::make_shared<Material>();
                material->setDiffuseColor(QVector4D(0.7f, 0.7f, 0.7f, 1.0f));
                material->setSpecularColor(QVector4D(0.3f, 0.3f, 0.3f, 1.0f));
                material->setShininess(32.0f);
                object->setMaterial(material);
                
                m_sceneManager->addObject(object);
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
        auto selectedObject = m_sceneManager->getSelectedObject();
        if (!selectedObject || !selectedObject->getMesh()) {
            QMessageBox::information(this, "Save", 
                "Please select an object to save.\n\nNote: Currently only individual object export is supported.");
            return;
        }
        
        bool success = false;
        if (m_currentFile.endsWith(".obj", Qt::CaseInsensitive)) {
            success = OBJFileHandler::exportToFile(m_currentFile, selectedObject->getMesh());
        } else if (m_currentFile.endsWith(".stl", Qt::CaseInsensitive)) {
            success = STLFileHandler::exportToFile(m_currentFile, selectedObject->getMesh());
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
    auto selectedObject = m_sceneManager->getSelectedObject();
    if (!selectedObject || !selectedObject->getMesh()) {
        QMessageBox::information(this, "Export", 
            "Please select an object to export.\n\nNote: Currently only individual object export is supported.");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this,
        "Export 3D Model", selectedObject->getName(),
        "OBJ Files (*.obj);;STL Files (*.stl);;All Files (*)");
    
    if (!fileName.isEmpty()) {
        bool success = false;
        
        if (fileName.endsWith(".obj", Qt::CaseInsensitive)) {
            success = OBJFileHandler::exportToFile(fileName, selectedObject->getMesh());
        } else if (fileName.endsWith(".stl", Qt::CaseInsensitive)) {
            success = STLFileHandler::exportToFile(fileName, selectedObject->getMesh());
        } else if (fileName.endsWith(".ply", Qt::CaseInsensitive)) {
            success = PLYFileHandler::exportToFile(fileName, selectedObject->getMesh());
        } else {
            // Default to OBJ if no extension specified
            if (!fileName.contains('.')) {
                fileName += ".obj";
            }
            success = OBJFileHandler::exportToFile(fileName, selectedObject->getMesh());
        }
        
        if (success) {
            statusBar()->showMessage(QString("Exported %1 to %2")
                .arg(selectedObject->getName(), QFileInfo(fileName).fileName()), 2000);
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
    auto cube = m_sceneManager->createCube();
    m_sceneManager->selectObject(cube);
    m_sceneModified = true;
    updateUI();
}

void MainWindow::createSphere()
{
    auto sphere = m_sceneManager->createSphere();
    m_sceneManager->selectObject(sphere);
    m_sceneModified = true;
    updateUI();
}

void MainWindow::createCylinder()
{
    auto cylinder = m_sceneManager->createCylinder();
    m_sceneManager->selectObject(cylinder);
    m_sceneModified = true;
    updateUI();
}

void MainWindow::createPlane()
{
    auto plane = m_sceneManager->createPlane();
    m_sceneManager->selectObject(plane);
    m_sceneModified = true;
    updateUI();
}

void MainWindow::createCone()
{
    auto cone = m_sceneManager->createCone();
    m_sceneManager->selectObject(cone);
    m_sceneModified = true;
    updateUI();
}

void MainWindow::createTorus()
{
    auto torus = m_sceneManager->createTorus();
    m_sceneManager->selectObject(torus);
    m_sceneModified = true;
    updateUI();
}

void MainWindow::createIcosphere()
{
    auto icosphere = m_sceneManager->createIcosphere();
    m_sceneManager->selectObject(icosphere);
    m_sceneModified = true;
    updateUI();
}

void MainWindow::resetCamera()
{
    m_viewport->resetCamera();
}

void MainWindow::frameScene()
{
    m_viewport->frameScene();
}

void MainWindow::frameSelection()
{
    m_viewport->frameSelectedObject();
}

void MainWindow::toggleGrid(bool show)
{
    if (m_gridSystem) {
        m_gridSystem->setVisible(show);
    }
}

void MainWindow::setWireframeMode()
{
    m_viewport->setRenderMode(RenderMode::Wireframe);
}

void MainWindow::setSolidMode()
{
    m_viewport->setRenderMode(RenderMode::Solid);
}

void MainWindow::setSolidWireframeMode()
{
    m_viewport->setRenderMode(RenderMode::SolidWireframe);
}

void MainWindow::setSelectMode()
{
    m_viewport->setTransformMode(TransformMode::Select);
}

void MainWindow::setTranslateMode()
{
    m_viewport->setTransformMode(TransformMode::Translate);
}

void MainWindow::setRotateMode()
{
    m_viewport->setTransformMode(TransformMode::Rotate);
}

void MainWindow::setScaleMode()
{
    m_viewport->setTransformMode(TransformMode::Scale);
}

void MainWindow::setObjectSelection()
{
    if (m_viewport) {
        m_viewport->setSelectionType(SelectionType::Object);
    }
}

void MainWindow::setVertexSelection()
{
    if (m_viewport) {
        m_viewport->setSelectionType(SelectionType::Vertex);
    }
}

void MainWindow::setEdgeSelection()
{
    if (m_viewport) {
        m_viewport->setSelectionType(SelectionType::Edge);
    }
}

void MainWindow::setFaceSelection()
{
    if (m_viewport) {
        m_viewport->setSelectionType(SelectionType::Face);
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
    // Critical: Clear viewport's shared_ptr references before MainWindow's 
    // shared_ptr members are destroyed to prevent use-after-free
    if (m_viewport) {
        // Clear all shared_ptr references in Viewport3D
        m_viewport->setScene(nullptr);
        m_viewport->setCameraController(nullptr);
        m_viewport->setInputController(nullptr);
    }
    
    // Clear panel references
    if (m_hierarchyPanel) {
        m_hierarchyPanel->setScene(nullptr);
    }
    if (m_propertiesPanel) {
        m_propertiesPanel->setSelectedObject(nullptr);
    }
    
    // Clear other cross-references
    if (m_inputController) {
        m_inputController->setScene(nullptr);
        m_inputController->setViewport(nullptr);
    }
    if (m_cameraController) {
        m_cameraController->setScene(nullptr);
        m_cameraController->setCamera(nullptr);
    }
    if (m_renderSystem) {
        m_renderSystem->setScene(nullptr);
        m_renderSystem->setCamera(nullptr);
    }
}

// Edit tool methods
void MainWindow::beginExtrude()
{
    if (!m_extrudeTool || !m_viewport) {
        qDebug() << "Cannot begin extrude: missing tools";
        return;
    }
    
    // Get the current mesh from the selected object
    auto selectedObject = m_scene->getSelectedObject();
    if (!selectedObject || !selectedObject->getMesh()) {
        qDebug() << "Cannot extrude: no mesh selected";
        return;
    }
    
    // Convert to HalfEdgeMesh for editing
    auto halfEdgeMesh = GeometryConverter::toHalfEdge(selectedObject->getMesh());
    if (!halfEdgeMesh) {
        qDebug() << "Failed to convert mesh to HalfEdge format";
        return;
    }
    
    // Set up the extrude tool
    m_extrudeTool->setMesh(halfEdgeMesh);
    m_extrudeTool->setSelectionManager(m_viewport->getSelectionManager());
    
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
        m_viewport->update();
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
        m_viewport->update();
    }
}
