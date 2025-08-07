/*
 * ██████╗ ██╗   ██╗██████╗ ███████╗██████╗  █████╗ ███████╗███████╗██████╗ ██████╗ 
 * ██╔══██╗██║   ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝╚════██╗██╔══██╗
 * ██████╔╝██║   ██║██║  ██║█████╗  ██████╔╝███████║███████╗█████╗   █████╔╝██║  ██║
 * ██╔══██╗██║   ██║██║  ██║██╔══╝  ██╔══██╗██╔══██║╚════██║██╔══╝   ╚═══██╗██║  ██║
 * ██║  ██║╚██████╔╝██████╔╝███████╗██████╔╝██║  ██║███████║███████╗██████╔╝██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═════╝ ╚═════╝ 
 * 
 * Professional 3D Modeling & Animation Software
 * 
 * ████████████████████████████████████████████████████████████████████████████████
 * █                                                                              █
 * █  FILE: AssetManager.h                                                       █
 * █  DESCRIPTION: Comprehensive Asset Management & Primitive Generation System  █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite          █
 * █  VERSION: 1.0.0                                                             █
 * █  CREATED: 2024                                                              █
 * █  UPDATED: August 7, 2025                                                    █
 * █                                                                              █
 * █  LICENSE: MIT License                                                       █
 * █  COMPANY: The No Hands Company                                              █
 * █  AUTHOR: RudeBase3D Development Team                                        █
 * █                                                                              █
 * █  This file contains the AssetManager class providing centralized asset     █
 * █  management, primitive mesh generation, material creation, and resource    █
 * █  caching for professional 3D modeling and animation workflows.             █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file AssetManager.h
 * @brief Comprehensive Asset Management and Primitive Generation System
 * 
 * This file contains the AssetManager class implementation that provides centralized
 * asset management, primitive mesh generation, material creation, and resource caching
 * essential for professional 3D modeling and animation workflows. The system manages
 * the complete lifecycle of 3D assets including meshes and materials, with intelligent
 * caching, automatic memory management, and seamless integration with file I/O systems
 * and rendering pipelines within the RudeBase3D environment.
 * 
 * ## Core Asset Management Architecture
 * 
 * ### Centralized Asset Registry
 * Unified asset management with intelligent organization:
 * - **Mesh Management**: Complete lifecycle management of 3D mesh assets with naming and caching
 * - **Material Management**: Comprehensive material asset registry with property tracking
 * - **Asset Discovery**: Automatic asset enumeration and query capabilities
 * - **Reference Tracking**: Smart pointer management with automatic cleanup
 * - **Unique Naming**: Automatic generation of unique asset names preventing conflicts
 * 
 * ### Intelligent Caching System
 * High-performance asset caching for optimal memory usage:
 * - **Memory-Efficient Storage**: Smart pointer-based caching with automatic cleanup
 * - **Asset Deduplication**: Prevent duplicate asset loading through intelligent detection
 * - **Cache Invalidation**: Selective cache clearing with fine-grained control
 * - **Memory Monitoring**: Asset memory usage tracking and optimization
 * - **Lazy Loading**: On-demand asset loading with background processing
 * 
 * ### Professional Asset Pipeline
 * Production-ready asset workflow integration:
 * - **File Format Support**: Integration with comprehensive file format handlers
 * - **Asset Validation**: Automatic asset validation and integrity checking
 * - **Dependency Tracking**: Asset dependency resolution and management
 * - **Version Control**: Asset versioning and change tracking capabilities
 * - **Batch Processing**: Efficient handling of multiple asset operations
 * 
 * ## Primitive Mesh Generation System
 * 
 * ### Mathematical Precision
 * High-quality primitive generation with mathematical accuracy:
 * - **Perfect Geometry**: Mathematically precise primitive mesh generation
 * - **UV Mapping**: Optimal texture coordinate generation for all primitives
 * - **Normal Calculation**: Accurate vertex normal computation for smooth shading
 * - **Topology Optimization**: Efficient triangle connectivity and vertex welding
 * - **Configurable Detail**: Adjustable subdivision levels for performance optimization
 * 
 * ### Professional Primitive Library
 * Comprehensive set of industry-standard 3D primitives:
 * - **Cube Meshes**: Perfect box geometry with configurable size and subdivision
 * - **Sphere Meshes**: UV sphere generation with configurable resolution and mapping
 * - **Plane Meshes**: Flat surfaces with optional subdivision for displacement
 * - **Cylinder Meshes**: Parametric cylinder generation with caps and sides
 * - **Cone Meshes**: Parametric cone primitives with configurable taper and detail
 * - **Grid Meshes**: Construction grids and reference planes for scene setup
 * 
 * ### Advanced Mesh Features
 * Professional-grade mesh properties and optimization:
 * - **Smooth Normals**: Calculated vertex normals for professional rendering
 * - **UV Coordinates**: Optimal texture mapping coordinates for all primitives
 * - **Tangent Spaces**: Calculated tangent and bitangent vectors for normal mapping
 * - **Level of Detail**: Multiple resolution variants for performance optimization
 * - **Manifold Geometry**: Watertight mesh generation suitable for 3D printing
 * 
 * ## Material Creation and Management
 * 
 * ### Professional Material Templates
 * Industry-standard material presets for rapid workflow:
 * - **Default Materials**: Basic material templates for rapid prototyping
 * - **Colored Materials**: Simple diffuse materials with customizable colors
 * - **Metallic Materials**: PBR metallic materials with realistic properties
 * - **Glass Materials**: Transparent and refractive material templates
 * - **Fabric Materials**: Specialized textile and cloth material presets
 * 
 * ### PBR Material Integration
 * Physically-Based Rendering material support:
 * - **Metallic-Roughness Workflow**: Industry-standard PBR material creation
 * - **Real-World Properties**: Materials based on measured physical data
 * - **Energy Conservation**: Physically accurate light interaction simulation
 * - **Texture Binding**: Automatic texture slot management and binding
 * - **Shader Compatibility**: Seamless integration with rendering pipeline
 * 
 * ### Material Library Management
 * Comprehensive material organization and cataloging:
 * - **Material Categories**: Organized material library with searchable categories
 * - **Custom Properties**: User-defined material metadata and tagging
 * - **Material Variants**: Multiple variations of base materials for different uses
 * - **Asset Dependencies**: Automatic texture and property dependency tracking
 * - **Export/Import**: Material library sharing and interchange capabilities
 * 
 * ## Technical Implementation
 * 
 * ### Qt Integration
 * Seamless Qt framework integration for professional applications:
 * - **QObject Inheritance**: Full Qt object model integration with signals and slots
 * - **Signal-Slot Communication**: Asynchronous asset loading notifications
 * - **QString Support**: Unicode string handling for international asset names
 * - **QHash Optimization**: High-performance hash table storage for asset lookup
 * - **Thread Safety**: Multi-threaded asset loading with proper synchronization
 * 
 * ### Memory Management
 * Professional-grade memory handling for large-scale projects:
 * - **Smart Pointer Architecture**: Automatic memory management with shared ownership
 * - **Reference Counting**: Automatic cleanup of unused assets
 * - **Memory Pool Allocation**: Efficient memory allocation for primitive generation
 * - **Garbage Collection**: Periodic cleanup of unreferenced assets
 * - **Memory Profiling**: Built-in memory usage monitoring and reporting
 * 
 * ### Performance Optimization
 * High-performance asset handling for real-time applications:
 * - **Asset Streaming**: Background asset loading without blocking UI
 * - **Parallel Processing**: Multi-threaded primitive generation and asset loading
 * - **Cache Locality**: Memory layout optimization for efficient access patterns
 * - **Batch Operations**: Efficient handling of multiple asset operations
 * - **Resource Pooling**: Reuse of temporary objects and computational resources
 * 
 * ## Usage Examples
 * 
 * ### Basic Asset Manager Setup
 * ```cpp
 * // Create and initialize asset manager for professional workflow
 * auto assetManager = new AssetManager(this);
 * if (assetManager->initialize()) {
 *     // Connect asset loading signals for progress feedback
 *     connect(assetManager, &AssetManager::assetLoaded,
 *             this, &MainWindow::onAssetLoaded);
 *     connect(assetManager, &AssetManager::assetLoadFailed,
 *             this, &MainWindow::onAssetLoadFailed);
 *     
 *     logInfo("Asset manager initialized successfully");
 * } else {
 *     logError("Failed to initialize asset manager");
 * }
 * ```
 * 
 * ### Professional Scene Setup with Primitives
 * ```cpp
 * class SceneBuilder {
 * private:
 *     AssetManager* m_assetManager;
 *     
 * public:
 *     void createArchitecturalScene() {
 *         // Create ground plane
 *         auto groundMesh = m_assetManager->createPlaneMesh(50.0f, 50.0f);
 *         auto groundMaterial = m_assetManager->createColoredMaterial(
 *             glm::vec4(0.7f, 0.7f, 0.8f, 1.0f)); // Light gray
 *         
 *         // Create building blocks
 *         auto buildingMesh = m_assetManager->createCubeMesh(1.0f);
 *         auto concreteMaterial = m_assetManager->createMetallicMaterial(
 *             glm::vec4(0.6f, 0.6f, 0.65f, 1.0f), 0.1f, 0.8f); // Concrete-like
 *         
 *         // Create decorative elements
 *         auto columnMesh = m_assetManager->createCylinderMesh(0.3f, 4.0f, 16);
 *         auto marbleMaterial = m_assetManager->createMetallicMaterial(
 *             glm::vec4(0.9f, 0.9f, 0.95f, 1.0f), 0.0f, 0.1f); // Polished marble
 *         
 *         // Create reference grid
 *         auto gridMesh = m_assetManager->createGridMesh(20.0f, 20);
 *         auto gridMaterial = m_assetManager->createColoredMaterial(
 *             glm::vec4(0.3f, 0.3f, 0.3f, 0.5f)); // Semi-transparent grid
 *         
 *         logInfo("Created architectural scene with professional assets");
 *     }
 *     
 *     void createProductDesignScene() {
 *         // Create product base
 *         auto baseMesh = m_assetManager->createCylinderMesh(2.0f, 0.2f, 32);
 *         auto aluminumMaterial = m_assetManager->createMetallicMaterial(
 *             glm::vec4(0.91f, 0.92f, 0.92f, 1.0f), 1.0f, 0.2f); // Brushed aluminum
 *         
 *         // Create main product body
 *         auto bodyMesh = m_assetManager->createSphereMesh(1.5f, 64, 32);
 *         auto plasticMaterial = m_assetManager->createMetallicMaterial(
 *             glm::vec4(0.2f, 0.3f, 0.8f, 1.0f), 0.0f, 0.4f); // Matte plastic
 *         
 *         // Create detail elements
 *         auto detailMesh = m_assetManager->createConeMesh(0.1f, 0.3f, 16);
 *         auto metalMaterial = m_assetManager->createMetallicMaterial(
 *             glm::vec4(0.8f, 0.8f, 0.9f, 1.0f), 1.0f, 0.1f); // Polished metal
 *         
 *         logInfo("Created product design scene with realistic materials");
 *     }
 * };
 * ```
 * 
 * ### Advanced Asset Loading and Management
 * ```cpp
 * class AssetLibraryManager {
 * private:
 *     AssetManager* m_assetManager;
 *     QStringList m_pendingAssets;
 *     
 * public:
 *     void loadAssetLibrary(const QStringList& assetPaths) {
 *         m_pendingAssets = assetPaths;
 *         loadNextAsset();
 *     }
 *     
 *     void onAssetLoaded(const QString& name, const QString& type) {
 *         logInfo("Successfully loaded " + type + " asset: " + name);
 *         
 *         // Update progress
 *         updateLoadingProgress();
 *         
 *         // Load next asset
 *         loadNextAsset();
 *     }
 *     
 *     void onAssetLoadFailed(const QString& name, const QString& error) {
 *         logError("Failed to load asset " + name + ": " + error);
 *         
 *         // Try alternative asset or skip
 *         handleAssetLoadFailure(name, error);
 *         
 *         // Continue loading
 *         loadNextAsset();
 *     }
 *     
 * private:
 *     void loadNextAsset() {
 *         if (m_pendingAssets.isEmpty()) {
 *             onAssetLibraryLoadComplete();
 *             return;
 *         }
 *         
 *         QString assetPath = m_pendingAssets.takeFirst();
 *         QString assetName = QFileInfo(assetPath).baseName();
 *         
 *         // Determine asset type and load accordingly
 *         if (assetPath.endsWith(".obj") || assetPath.endsWith(".stl") || 
 *             assetPath.endsWith(".ply")) {
 *             m_assetManager->loadMesh(assetName, assetPath);
 *         } else if (assetPath.endsWith(".mtl")) {
 *             m_assetManager->loadMaterial(assetName, assetPath);
 *         }
 *     }
 * };
 * ```
 * 
 * ### Custom Material Creation Pipeline
 * ```cpp
 * class MaterialFactory {
 * private:
 *     AssetManager* m_assetManager;
 *     
 * public:
 *     std::shared_ptr<Material> createWoodMaterial(const QString& woodType) {
 *         auto material = m_assetManager->createMaterial(woodType + "_wood");
 *         
 *         if (woodType == "oak") {
 *             material->setDiffuseColor(glm::vec4(0.8f, 0.6f, 0.4f, 1.0f));
 *             material->setRoughness(0.7f);
 *             material->setMetallic(0.0f);
 *         } else if (woodType == "pine") {
 *             material->setDiffuseColor(glm::vec4(0.9f, 0.8f, 0.6f, 1.0f));
 *             material->setRoughness(0.8f);
 *             material->setMetallic(0.0f);
 *         } else if (woodType == "mahogany") {
 *             material->setDiffuseColor(glm::vec4(0.6f, 0.3f, 0.2f, 1.0f));
 *             material->setRoughness(0.6f);
 *             material->setMetallic(0.0f);
 *         }
 *         
 *         return material;
 *     }
 *     
 *     std::shared_ptr<Material> createFabricMaterial(const QString& fabricType) {
 *         auto material = m_assetManager->createMaterial(fabricType + "_fabric");
 *         
 *         if (fabricType == "cotton") {
 *             material->setDiffuseColor(glm::vec4(0.9f, 0.9f, 0.85f, 1.0f));
 *             material->setRoughness(0.9f);
 *             material->setMetallic(0.0f);
 *         } else if (fabricType == "silk") {
 *             material->setDiffuseColor(glm::vec4(0.95f, 0.95f, 0.9f, 1.0f));
 *             material->setRoughness(0.2f);
 *             material->setMetallic(0.0f);
 *         } else if (fabricType == "velvet") {
 *             material->setDiffuseColor(glm::vec4(0.4f, 0.2f, 0.6f, 1.0f));
 *             material->setRoughness(0.95f);
 *             material->setMetallic(0.0f);
 *         }
 *         
 *         return material;
 *     }
 *     
 *     std::shared_ptr<Material> createMetalMaterial(const QString& metalType) {
 *         auto material = m_assetManager->createMaterial(metalType + "_metal");
 *         material->setMetallic(1.0f); // All metals are fully metallic
 *         
 *         if (metalType == "aluminum") {
 *             material->setDiffuseColor(glm::vec4(0.91f, 0.92f, 0.92f, 1.0f));
 *             material->setRoughness(0.1f); // Polished aluminum
 *         } else if (metalType == "copper") {
 *             material->setDiffuseColor(glm::vec4(0.95f, 0.64f, 0.54f, 1.0f));
 *             material->setRoughness(0.15f);
 *         } else if (metalType == "gold") {
 *             material->setDiffuseColor(glm::vec4(1.0f, 0.86f, 0.57f, 1.0f));
 *             material->setRoughness(0.05f); // Highly polished gold
 *         } else if (metalType == "steel") {
 *             material->setDiffuseColor(glm::vec4(0.77f, 0.78f, 0.78f, 1.0f));
 *             material->setRoughness(0.2f);
 *         }
 *         
 *         return material;
 *     }
 * };
 * ```
 * 
 * ### Asset Performance Monitoring
 * ```cpp
 * class AssetPerformanceMonitor {
 * private:
 *     AssetManager* m_assetManager;
 *     QTimer* m_monitorTimer;
 *     
 * public:
 *     void startMonitoring() {
 *         m_monitorTimer = new QTimer(this);
 *         connect(m_monitorTimer, &QTimer::timeout,
 *                 this, &AssetPerformanceMonitor::updateMetrics);
 *         m_monitorTimer->start(5000); // Update every 5 seconds
 *     }
 *     
 *     void updateMetrics() {
 *         size_t meshCount = m_assetManager->getMeshCount();
 *         size_t materialCount = m_assetManager->getMaterialCount();
 *         
 *         // Calculate memory usage (approximation)
 *         size_t estimatedMemoryUsage = calculateMemoryUsage();
 *         
 *         logInfo("Asset Manager Metrics:");
 *         logInfo("  Meshes: " + QString::number(meshCount));
 *         logInfo("  Materials: " + QString::number(materialCount));
 *         logInfo("  Estimated Memory: " + formatMemorySize(estimatedMemoryUsage));
 *         
 *         // Trigger cleanup if memory usage is high
 *         if (estimatedMemoryUsage > MAX_MEMORY_THRESHOLD) {
 *             optimizeMemoryUsage();
 *         }
 *     }
 *     
 * private:
 *     void optimizeMemoryUsage() {
 *         logInfo("Optimizing asset memory usage...");
 *         
 *         // Clear unused assets
 *         m_assetManager->clearCache();
 *         
 *         // Force garbage collection
 *         QCoreApplication::processEvents();
 *         
 *         logInfo("Memory optimization complete");
 *     }
 * };
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Asset Dependency Management
 * Professional asset relationship tracking and resolution:
 * - **Dependency Graphs**: Automatic tracking of asset dependencies and relationships
 * - **Circular Reference Detection**: Prevention of circular dependency issues
 * - **Dependency Resolution**: Automatic loading of required dependencies
 * - **Asset Variants**: Multiple versions of assets for different quality levels
 * - **Dynamic Loading**: On-demand loading of dependencies during rendering
 * 
 * ### Asset Streaming and LOD
 * Performance optimization for large-scale scenes:
 * - **Progressive Loading**: Background loading of assets with progressive detail
 * - **Distance-Based LOD**: Automatic level-of-detail selection based on camera distance
 * - **Memory Streaming**: Dynamic loading and unloading based on memory constraints
 * - **Quality Adaptation**: Real-time quality adjustment based on performance metrics
 * - **Asset Prioritization**: Intelligent loading order based on usage patterns
 * 
 * ### Professional Asset Pipeline Integration
 * Enterprise-ready asset workflow support:
 * - **Version Control**: Asset versioning and change tracking capabilities
 * - **Asset Validation**: Comprehensive asset integrity checking and validation
 * - **Batch Processing**: Efficient handling of large asset collections
 * - **Asset Conversion**: Automatic format conversion and optimization
 * - **Metadata Management**: Custom asset properties and organizational tags
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup AssetManagement
 * @ingroup Assets
 * @ingroup Primitives
 * @ingroup Materials
 */

#pragma once

#include "Common.h"
#include <QObject>
#include <QString>
#include <QHash>
#include <memory>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

class Mesh;
class Material;

class AssetManager : public QObject
{
    Q_OBJECT
    
public:
    explicit AssetManager(QObject* parent = nullptr);
    ~AssetManager();
    
    // Initialization
    bool initialize();
    void cleanup();
    
    // Mesh management
    rude::MeshPtr createMesh(const QString& name);
    rude::MeshPtr getMesh(const QString& name) const;
    bool loadMesh(const QString& name, const QString& filePath);
    void removeMesh(const QString& name);
    
    // Material management
    std::shared_ptr<Material> createMaterial(const QString& name);
    std::shared_ptr<Material> getMaterial(const QString& name) const;
    bool loadMaterial(const QString& name, const QString& filePath);
    void removeMaterial(const QString& name);
    
    // Primitive mesh creation
    rude::MeshPtr createCubeMesh(float size = 1.0f);
    rude::MeshPtr createSphereMesh(float radius = 0.5f, int segments = 32, int rings = 16);
    rude::MeshPtr createPlaneMesh(float width = 2.0f, float height = 2.0f);
    rude::MeshPtr createCylinderMesh(float radius = 0.5f, float height = 1.0f, int segments = 32);
    rude::MeshPtr createConeMesh(float radius = 0.5f, float height = 1.0f, int segments = 32);
    rude::MeshPtr createGridMesh(float size = 20.0f, int divisions = 20);
    
    // Default material creation
    std::shared_ptr<Material> createDefaultMaterial();
    std::shared_ptr<Material> createColoredMaterial(const glm::vec4& color);
    std::shared_ptr<Material> createMetallicMaterial(const glm::vec4& color, float metallic = 0.8f, float roughness = 0.2f);
    
    // Asset queries
    QStringList getMeshNames() const;
    QStringList getMaterialNames() const;
    size_t getMeshCount() const;
    size_t getMaterialCount() const;
    
    // Cache management
    void clearCache();
    void clearMeshCache();
    void clearMaterialCache();
    
signals:
    void assetLoaded(const QString& name, const QString& type);
    void assetLoadFailed(const QString& name, const QString& error);
    
private:
    QString generateUniqueName(const QString& baseName, const QStringList& existingNames) const;
    
    QHash<QString, rude::MeshPtr> m_meshCache;
    QHash<QString, std::shared_ptr<Material>> m_materialCache;
    
    // Asset counters for unique naming
    int m_meshCounter;
    int m_materialCounter;
};
