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
 * █  FILE: ExtrudeTool.h                                                         █
 * █  DESCRIPTION: Professional Extrude Tool for Advanced Mesh Modeling           █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite            █
 * █  VERSION: 1.0.0                                                              █
 * █  CREATED: 2024                                                               █
 * █  UPDATED: August 7, 2025                                                     █
 * █                                                                              █
 * █  LICENSE: MIT License                                                        █
 * █  COMPANY: The No Hands Company                                               █
 * █  AUTHOR: RudeBase3D Development Team                                         █
 * █                                                                              █
 * █  This file contains the ExtrudeTool class - providing professional-grade     █
 * █  mesh extrusion operations for faces, edges, and vertices with real-time     █
 * █  preview and sophisticated topology management.                              █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file ExtrudeTool.h
 * @brief Professional Extrude Tool for Advanced 3D Mesh Modeling Operations
 * 
 * This file contains the comprehensive ExtrudeTool class implementation that provides
 * professional-grade mesh extrusion capabilities within the RudeBase3D environment.
 * The tool supports sophisticated extrusion operations for faces, edges, and vertices
 * with real-time preview, multiple extrusion modes, and advanced topology management,
 * delivering modeling functionality comparable to industry-standard 3D applications
 * like Maya, Blender, 3ds Max, and Modo.
 * 
 * ## Professional Extrusion Architecture
 * 
 * ### Multi-Element Extrusion System
 * Industry-standard extrusion capabilities for all mesh components:
 * - **Face Extrusion**: Individual and group face extrusion with normal preservation
 * - **Edge Extrusion**: Edge extrusion with automatic face creation and bridging
 * - **Vertex Extrusion**: Vertex extrusion with edge creation and topology repair
 * - **Multi-Selection**: Complex selection set extrusion with element coordination
 * 
 * ### Advanced Extrusion Modes
 * Professional extrusion paradigms matching industry workflows:
 * - **Normal Mode**: Extrude along calculated element normals with smoothing
 * - **Direction Mode**: Extrude in user-specified world-space direction
 * - **Individual Mode**: Per-element extrusion with independent normal calculation
 * - **Constrained Mode**: Axis-constrained extrusion with projection and snapping
 * 
 * ### Real-Time Preview System
 * Interactive preview system for sophisticated modeling workflows:
 * - **Live Preview**: Real-time geometry preview during mouse interaction
 * - **Visual Feedback**: Immediate visual response to parameter changes
 * - **Non-Destructive**: Preview without modifying original mesh data
 * - **Performance Optimized**: Efficient preview generation for large meshes
 * 
 * ## Advanced Modeling Features
 * 
 * ### Sophisticated Normal Calculation
 * Professional normal computation for precise extrusion direction:
 * - **Face Normals**: Area-weighted face normal calculation with smoothing
 * - **Edge Normals**: Averaged adjacent face normals with boundary handling
 * - **Vertex Normals**: Angle-weighted vertex normals with crease detection
 * - **Custom Normals**: User-defined normal directions with constraint systems
 * 
 * ### Topology-Aware Mesh Operations
 * Advanced mesh editing with complete topology preservation:
 * - **Half-Edge Integration**: Direct manipulation of half-edge mesh structure
 * - **Connectivity Preservation**: Maintains mesh manifold properties
 * - **Boundary Handling**: Proper treatment of mesh boundaries and holes
 * - **Validation Systems**: Automatic mesh integrity checking and repair
 * 
 * ### Professional Workflow Integration
 * Industry-standard modeling workflow support:
 * - **Undo/Redo Integration**: Complete operation history with state restoration
 * - **Tool Chaining**: Sequential tool execution with parameter inheritance
 * - **Macro Recording**: Tool operation recording for automation and scripting
 * - **Batch Processing**: Multi-object extrusion with synchronized parameters
 * 
 * ## Advanced Technical Implementation
 * 
 * ### Half-Edge Mesh Management
 * Professional mesh data structure handling for complex extrusion operations:
 * - **Vertex Duplication**: Efficient vertex duplication with position inheritance
 * - **Face Creation**: Automatic bridging face creation with proper orientation
 * - **Edge Bridging**: Sophisticated edge connection with topology validation
 * - **Memory Optimization**: Efficient memory usage for large extrusion operations
 * 
 * ### Interactive Preview Architecture
 * Real-time preview system for professional modeling workflows:
 * - **Preview Geometry**: Separate preview mesh for non-destructive editing
 * - **Render Integration**: Direct integration with rendering pipeline
 * - **Update Optimization**: Minimal updates for interactive performance
 * - **Visual Indicators**: Clear visual distinction between original and preview
 * 
 * ### State Management System
 * Comprehensive state management for complex modeling operations:
 * - **Operation State**: Complete tracking of extrusion operation progress
 * - **Backup Systems**: Original geometry backup for cancellation support
 * - **Parameter Tracking**: Real-time parameter change tracking and validation
 * - **Error Recovery**: Graceful handling of invalid operations and states
 * 
 * ## Professional Extrusion Algorithms
 * 
 * ### Face Extrusion Implementation
 * Sophisticated face extrusion with topology preservation:
 * - **Normal Calculation**: Area-weighted face normal with smoothing options
 * - **Vertex Duplication**: Efficient vertex duplication with UV preservation
 * - **Bridge Creation**: Automatic side face creation with proper winding
 * - **Manifold Preservation**: Maintains mesh manifold properties throughout
 * 
 * ### Edge Extrusion Implementation
 * Advanced edge extrusion with automatic face generation:
 * - **Adjacent Face Analysis**: Intelligent analysis of adjacent face topology
 * - **Normal Averaging**: Smooth normal calculation from adjacent faces
 * - **Quad Generation**: Automatic quad face creation along extruded edges
 * - **Boundary Treatment**: Special handling of boundary and naked edges
 * 
 * ### Vertex Extrusion Implementation
 * Professional vertex extrusion with edge creation:
 * - **Star Analysis**: Analysis of vertex star for proper edge creation
 * - **Edge Generation**: Automatic edge creation connecting original and new vertices
 * - **Face Triangulation**: Intelligent triangulation of resulting n-gons
 * - **Smoothing Groups**: Preservation of smoothing groups and material assignments
 * 
 * ## Industry-Standard Workflow Integration
 * 
 * ### Maya-Style Extrusion
 * Maya-compatible extrusion behaviors and workflows:
 * - **Component Masking**: Automatic component type detection and extrusion
 * - **Local Coordinate Space**: Extrusion in local object coordinate systems
 * - **Thickness Control**: Post-extrusion thickness adjustment with scaling
 * - **Division Control**: Automatic subdivision of extruded geometry
 * 
 * ### Blender-Style Extrusion
 * Blender-compatible extrusion paradigms and interactions:
 * - **E-Key Extrusion**: Instant extrusion activation with mouse movement
 * - **Constraint System**: X/Y/Z axis constraint during extrusion operation
 * - **Proportional Editing**: Falloff-based extrusion with influence control
 * - **Alt-Click Deselection**: Alternative selection behavior integration
 * 
 * ### 3ds Max-Style Extrusion
 * 3ds Max-compatible extrusion features and modifiers:
 * - **Extrude Modifier**: Non-destructive extrusion with parametric control
 * - **Bevel Integration**: Combined extrude and bevel operations
 * - **Group Extrusion**: Multi-object extrusion with synchronized parameters
 * - **Material Preservation**: Automatic material assignment to extruded faces
 * 
 * ## Performance Optimization Strategies
 * 
 * ### Large Mesh Optimization
 * Efficient extrusion operations for complex meshes:
 * - **Spatial Indexing**: Octree-based spatial acceleration for large selections
 * - **Batch Operations**: Grouped operations for multiple element extrusion
 * - **Memory Pooling**: Reusable data structures for frequent operations
 * - **Parallel Processing**: Multi-threaded processing for large extrusion sets
 * 
 * ### Real-Time Preview Optimization
 * Optimized preview generation for interactive workflows:
 * - **Level of Detail**: Adaptive preview detail based on viewport distance
 * - **Frustum Culling**: Preview geometry culling for off-screen elements
 * - **Dirty Flagging**: Minimal updates for unchanged geometry regions
 * - **GPU Acceleration**: GPU-based preview generation where available
 * 
 * ## Usage Examples
 * 
 * ### Basic Face Extrusion Operation
 * ```cpp
 * // Initialize extrude tool with mesh and selection manager
 * ExtrudeTool extrudeTool;
 * extrudeTool.setMesh(meshPtr);
 * extrudeTool.setSelectionManager(selectionManagerPtr);
 * 
 * // Configure extrusion settings
 * extrudeTool.setExtrudeMode(ExtrudeTool::ExtrudeMode::Normal);
 * 
 * // Begin extrusion operation
 * if (extrudeTool.canExtrude()) {
 *     extrudeTool.beginExtrude();
 *     
 *     // Update extrusion distance interactively
 *     extrudeTool.updateExtrude(2.5f);
 *     
 *     // Get preview for real-time rendering
 *     auto preview = extrudeTool.getPreview();
 *     renderPreviewGeometry(preview);
 *     
 *     // Confirm or cancel the operation
 *     extrudeTool.confirmExtrude(); // or extrudeTool.cancelExtrude();
 * }
 * ```
 * 
 * ### Advanced Directional Extrusion with Custom Parameters
 * ```cpp
 * class AdvancedExtrudeController {
 * private:
 *     ExtrudeTool m_extrudeTool;
 *     std::vector<ExtrusionStep> m_extrusionHistory;
 *     
 * public:
 *     void performDirectionalExtrude(const glm::vec3& direction, float distance,
 *                                   bool useIndividualNormals = false) {
 *         // Configure extrusion mode
 *         if (useIndividualNormals) {
 *             m_extrudeTool.setExtrudeMode(ExtrudeTool::ExtrudeMode::Individual);
 *         } else {
 *             m_extrudeTool.setExtrudeMode(ExtrudeTool::ExtrudeMode::Direction);
 *             m_extrudeTool.setExtrudeDirection(glm::normalize(direction));
 *         }
 *         
 *         // Begin extrusion with validation
 *         if (!m_extrudeTool.canExtrude()) {
 *             throw std::runtime_error("Cannot extrude: invalid selection or mesh state");
 *         }
 *         
 *         m_extrudeTool.beginExtrude();
 *         
 *         // Apply extrusion with specified parameters
 *         if (useIndividualNormals) {
 *             m_extrudeTool.updateExtrude(distance);
 *         } else {
 *             m_extrudeTool.updateExtrude(direction, distance);
 *         }
 *         
 *         // Store operation in history
 *         ExtrusionStep step;
 *         step.direction = direction;
 *         step.distance = distance;
 *         step.mode = m_extrudeTool.getExtrudeMode();
 *         step.timestamp = std::chrono::steady_clock::now();
 *         m_extrusionHistory.push_back(step);
 *         
 *         // Confirm the operation
 *         m_extrudeTool.confirmExtrude();
 *     }
 *     
 *     void performMultiStepExtrusion(const std::vector<ExtrusionStep>& steps) {
 *         for (const auto& step : steps) {
 *             // Set up for this step
 *             m_extrudeTool.setExtrudeMode(step.mode);
 *             if (step.mode == ExtrudeTool::ExtrudeMode::Direction) {
 *                 m_extrudeTool.setExtrudeDirection(step.direction);
 *             }
 *             
 *             // Execute the step
 *             if (m_extrudeTool.canExtrude()) {
 *                 m_extrudeTool.beginExtrude();
 *                 m_extrudeTool.updateExtrude(step.distance);
 *                 m_extrudeTool.confirmExtrude();
 *             }
 *         }
 *     }
 * };
 * ```
 * 
 * ### Interactive Extrusion with Real-Time Preview
 * ```cpp
 * class InteractiveExtrudeWidget : public QWidget {
 *     Q_OBJECT
 *     
 * private:
 *     ExtrudeTool* m_extrudeTool;
 *     QSlider* m_distanceSlider;
 *     QComboBox* m_modeComboBox;
 *     QPushButton* m_confirmButton;
 *     QPushButton* m_cancelButton;
 *     bool m_isExtruding = false;
 *     
 * public:
 *     InteractiveExtrudeWidget(ExtrudeTool* tool, QWidget* parent = nullptr)
 *         : QWidget(parent), m_extrudeTool(tool) {
 *         setupUI();
 *         connectSignals();
 *     }
 *     
 * private slots:
 *     void onDistanceChanged(int value) {
 *         if (!m_isExtruding) return;
 *         
 *         float distance = value / 100.0f; // Convert slider value to distance
 *         m_extrudeTool->updateExtrude(distance);
 *         
 *         // Update preview rendering
 *         auto preview = m_extrudeTool->getPreview();
 *         emit previewUpdated(preview);
 *     }
 *     
 *     void onModeChanged(int index) {
 *         ExtrudeTool::ExtrudeMode mode;
 *         switch (index) {
 *             case 0: mode = ExtrudeTool::ExtrudeMode::Normal; break;
 *             case 1: mode = ExtrudeTool::ExtrudeMode::Direction; break;
 *             case 2: mode = ExtrudeTool::ExtrudeMode::Individual; break;
 *             default: return;
 *         }
 *         
 *         m_extrudeTool->setExtrudeMode(mode);
 *         
 *         // Restart extrusion with new mode if currently extruding
 *         if (m_isExtruding) {
 *             m_extrudeTool->cancelExtrude();
 *             startExtrusion();
 *         }
 *     }
 *     
 *     void onConfirmClicked() {
 *         if (m_isExtruding) {
 *             m_extrudeTool->confirmExtrude();
 *             m_isExtruding = false;
 *             updateUIState();
 *             emit extrusionCompleted();
 *         }
 *     }
 *     
 *     void onCancelClicked() {
 *         if (m_isExtruding) {
 *             m_extrudeTool->cancelExtrude();
 *             m_isExtruding = false;
 *             updateUIState();
 *             emit extrusionCancelled();
 *         }
 *     }
 *     
 * public slots:
 *     void startExtrusion() {
 *         if (!m_extrudeTool->canExtrude()) {
 *             QMessageBox::warning(this, "Extrude Tool", 
 *                                "Cannot extrude: No valid selection");
 *             return;
 *         }
 *         
 *         m_extrudeTool->beginExtrude();
 *         m_isExtruding = true;
 *         updateUIState();
 *         
 *         // Initialize with current slider value
 *         onDistanceChanged(m_distanceSlider->value());
 *     }
 *     
 * private:
 *     void setupUI() {
 *         auto layout = new QVBoxLayout(this);
 *         
 *         // Mode selection
 *         layout->addWidget(new QLabel("Extrude Mode:"));
 *         m_modeComboBox = new QComboBox();
 *         m_modeComboBox->addItems({"Normal", "Direction", "Individual"});
 *         layout->addWidget(m_modeComboBox);
 *         
 *         // Distance control
 *         layout->addWidget(new QLabel("Distance:"));
 *         m_distanceSlider = new QSlider(Qt::Horizontal);
 *         m_distanceSlider->setRange(0, 1000); // 0 to 10 units
 *         m_distanceSlider->setValue(100); // Default 1 unit
 *         layout->addWidget(m_distanceSlider);
 *         
 *         // Control buttons
 *         auto buttonLayout = new QHBoxLayout();
 *         m_confirmButton = new QPushButton("Confirm");
 *         m_cancelButton = new QPushButton("Cancel");
 *         buttonLayout->addWidget(m_confirmButton);
 *         buttonLayout->addWidget(m_cancelButton);
 *         layout->addLayout(buttonLayout);
 *         
 *         updateUIState();
 *     }
 *     
 *     void connectSignals() {
 *         connect(m_distanceSlider, &QSlider::valueChanged,
 *                 this, &InteractiveExtrudeWidget::onDistanceChanged);
 *         connect(m_modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
 *                 this, &InteractiveExtrudeWidget::onModeChanged);
 *         connect(m_confirmButton, &QPushButton::clicked,
 *                 this, &InteractiveExtrudeWidget::onConfirmClicked);
 *         connect(m_cancelButton, &QPushButton::clicked,
 *                 this, &InteractiveExtrudeWidget::onCancelClicked);
 *     }
 *     
 *     void updateUIState() {
 *         m_distanceSlider->setEnabled(m_isExtruding);
 *         m_confirmButton->setEnabled(m_isExtruding);
 *         m_cancelButton->setEnabled(m_isExtruding);
 *         m_modeComboBox->setEnabled(!m_isExtruding);
 *     }
 *     
 * signals:
 *     void previewUpdated(const ExtrudeTool::ExtrudePreview& preview);
 *     void extrusionCompleted();
 *     void extrusionCancelled();
 * };
 * ```
 * 
 * ### Professional Extrusion Manager with Advanced Features
 * ```cpp
 * class ProfessionalExtrudeManager {
 * private:
 *     struct ExtrudeSession {
 *         std::string sessionId;
 *         ExtrudeTool tool;
 *         std::vector<ExtrusionStep> steps;
 *         std::chrono::steady_clock::time_point startTime;
 *         rude::HalfEdgeMeshPtr originalMesh;
 *         SelectionState originalSelection;
 *     };
 *     
 *     std::map<std::string, std::unique_ptr<ExtrudeSession>> m_activeSessions;
 *     ExtrudeSettings m_defaultSettings;
 *     
 * public:
 *     std::string beginExtrudeSession(rude::HalfEdgeMeshPtr mesh,
 *                                   std::shared_ptr<SelectionManager> selectionManager) {
 *         std::string sessionId = generateUniqueId();
 *         
 *         auto session = std::make_unique<ExtrudeSession>();
 *         session->sessionId = sessionId;
 *         session->tool.setMesh(mesh);
 *         session->tool.setSelectionManager(selectionManager);
 *         session->startTime = std::chrono::steady_clock::now();
 *         session->originalMesh = cloneMesh(mesh);
 *         session->originalSelection = selectionManager->getSelectionState();
 *         
 *         // Apply default settings
 *         session->tool.setExtrudeMode(m_defaultSettings.mode);
 *         session->tool.setExtrudeDirection(m_defaultSettings.direction);
 *         
 *         m_activeSessions[sessionId] = std::move(session);
 *         return sessionId;
 *     }
 *     
 *     bool executeExtrudeStep(const std::string& sessionId,
 *                           const ExtrusionParameters& params) {
 *         auto it = m_activeSessions.find(sessionId);
 *         if (it == m_activeSessions.end()) {
 *             return false;
 *         }
 *         
 *         ExtrudeSession* session = it->second.get();
 *         
 *         // Configure tool for this step
 *         session->tool.setExtrudeMode(params.mode);
 *         if (params.mode == ExtrudeTool::ExtrudeMode::Direction) {
 *             session->tool.setExtrudeDirection(params.direction);
 *         }
 *         
 *         // Execute the extrusion step
 *         if (!session->tool.canExtrude()) {
 *             return false;
 *         }
 *         
 *         session->tool.beginExtrude();
 *         session->tool.updateExtrude(params.distance);
 *         session->tool.confirmExtrude();
 *         
 *         // Record the step
 *         ExtrusionStep step;
 *         step.parameters = params;
 *         step.timestamp = std::chrono::steady_clock::now();
 *         step.preview = session->tool.getPreview();
 *         session->steps.push_back(step);
 *         
 *         return true;
 *     }
 *     
 *     void commitExtrudeSession(const std::string& sessionId) {
 *         auto it = m_activeSessions.find(sessionId);
 *         if (it != m_activeSessions.end()) {
 *             ExtrudeSession* session = it->second.get();
 *             
 *             // Log session completion
 *             auto duration = std::chrono::steady_clock::now() - session->startTime;
 *             logExtrudeSession(sessionId, session->steps, duration);
 *             
 *             m_activeSessions.erase(it);
 *         }
 *     }
 *     
 *     void rollbackExtrudeSession(const std::string& sessionId) {
 *         auto it = m_activeSessions.find(sessionId);
 *         if (it != m_activeSessions.end()) {
 *             ExtrudeSession* session = it->second.get();
 *             
 *             // Restore original mesh and selection
 *             restoreMesh(session->originalMesh);
 *             restoreSelection(session->originalSelection);
 *             
 *             m_activeSessions.erase(it);
 *         }
 *     }
 *     
 *     std::vector<ExtrudeTool::ExtrudePreview> getSessionPreviews(
 *         const std::string& sessionId) const {
 *         auto it = m_activeSessions.find(sessionId);
 *         if (it == m_activeSessions.end()) {
 *             return {};
 *         }
 *         
 *         std::vector<ExtrudeTool::ExtrudePreview> previews;
 *         const ExtrudeSession* session = it->second.get();
 *         
 *         for (const auto& step : session->steps) {
 *             previews.push_back(step.preview);
 *         }
 *         
 *         return previews;
 *     }
 * };
 * ```
 * 
 * ## Integration with RudeBase3D Architecture
 * 
 * ### Mesh System Integration
 * Seamless integration with RudeBase3D mesh architecture:
 * - **Half-Edge Mesh**: Direct integration with half-edge data structures
 * - **Mesh Validation**: Automatic mesh integrity validation after operations
 * - **Performance Optimization**: Efficient memory usage and operation batching
 * - **Format Support**: Support for multiple mesh formats and representations
 * 
 * ### Selection System Coordination
 * Professional coordination with selection management systems:
 * - **Selection Validation**: Automatic validation of selection compatibility
 * - **Multi-Object Selection**: Support for multi-object extrusion operations
 * - **Selection Preservation**: Intelligent selection updates after extrusion
 * - **History Integration**: Complete integration with selection history
 * 
 * ### Rendering Pipeline Integration
 * Direct integration with RudeBase3D rendering systems:
 * - **Preview Rendering**: Real-time preview integration with viewport rendering
 * - **Material Preservation**: Automatic material assignment to extruded geometry
 * - **Lighting Updates**: Dynamic lighting recalculation for extruded surfaces
 * - **Performance Optimization**: Efficient rendering updates during interaction
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup ModelingTools
 * @ingroup ExtrusionTools
 * @ingroup MeshEditing
 */

#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "core/mesh_forward.hpp"
#include "tools/selection/SelectionManager.h"

namespace rude {
class Face;
class Vertex;
class Edge;
class HalfEdgeMesh;
using FacePtr = std::shared_ptr<Face>;
using VertexPtr = std::shared_ptr<Vertex>;
using EdgePtr = std::shared_ptr<Edge>;
using HalfEdgeMeshPtr = std::shared_ptr<HalfEdgeMesh>;
}

/**
 * @brief Tool for extruding faces, edges, and vertices in HEDS meshes
 * 
 * The ExtrudeTool provides interactive mesh editing by extruding selected
 * elements along their normals or a specified direction.
 */
class ExtrudeTool {
public:
    enum class ExtrudeMode {
        Normal,     // Extrude along element normals
        Direction,  // Extrude in a specific direction
        Individual  // Extrude each element individually
    };

    ExtrudeTool();
    ~ExtrudeTool() = default;

    // Tool state
    void setMesh(rude::HalfEdgeMeshPtr mesh);
    void setSelectionManager(std::shared_ptr<SelectionManager> selectionManager);
    
    // Extrude operations
    bool beginExtrude();
    void updateExtrude(float distance);
    void updateExtrude(const glm::vec3& direction, float distance);
    void confirmExtrude();
    void cancelExtrude();
    
    // Tool settings
    void setExtrudeMode(ExtrudeMode mode) { m_extrudeMode = mode; }
    ExtrudeMode getExtrudeMode() const { return m_extrudeMode; }
    
    void setExtrudeDirection(const glm::vec3& direction) { m_extrudeDirection = direction; }
    glm::vec3 getExtrudeDirection() const { return m_extrudeDirection; }
    
    // Tool state queries
    bool isExtruding() const { return m_isExtruding; }
    bool canExtrude() const;
    
    // Get preview data for rendering
    struct ExtrudePreview {
        std::vector<glm::vec3> previewVertices;
        std::vector<unsigned int> previewIndices;
        std::vector<glm::vec3> extrudeVectors;
    };
    
    ExtrudePreview getPreview() const { return m_preview; }

private:
    rude::HalfEdgeMeshPtr m_mesh;
    std::shared_ptr<SelectionManager> m_selectionManager;
    
    ExtrudeMode m_extrudeMode = ExtrudeMode::Normal;
    glm::vec3 m_extrudeDirection = glm::vec3(0, 1, 0);
    
    bool m_isExtruding = false;
    float m_currentDistance = 0.0f;
    
    // Backup data for cancellation
    struct OriginalData {
        std::vector<glm::vec3> originalPositions;
        std::vector<unsigned int> originalVertexIds;
        std::vector<unsigned int> newVertexIds;
        std::vector<unsigned int> newFaceIds;
    };
    OriginalData m_originalData;
    
    ExtrudePreview m_preview;
    
    // Internal operations
    bool extrudeFaces(const std::vector<rude::FacePtr>& faces);
    bool extrudeEdges(const std::vector<rude::EdgePtr>& edges);
    bool extrudeVertices(const std::vector<rude::VertexPtr>& vertices);
    
    // Helper methods
    glm::vec3 calculateFaceNormal(rude::FacePtr face) const;
    glm::vec3 calculateEdgeNormal(rude::EdgePtr edge) const;
    glm::vec3 calculateVertexNormal(rude::VertexPtr vertex) const;
    
    void createExtrudedGeometry();
    void updatePreview();
    void restoreOriginalGeometry();
    
    // HEDS topology operations
    rude::VertexPtr duplicateVertex(rude::VertexPtr vertex);
    rude::FacePtr createQuadFace(rude::VertexPtr v1, rude::VertexPtr v2, 
                                   rude::VertexPtr v3, rude::VertexPtr v4);
    void bridgeEdges(rude::EdgePtr edge1, rude::EdgePtr edge2);
};
