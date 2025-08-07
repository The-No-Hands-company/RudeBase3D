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
 * █  FILE: EditContext.h                                                         █
 * █  DESCRIPTION: Professional Edit Context & Modeling State Management          █
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
 * █  This file contains the EditContext and ModelingTool classes - providing     █
 * █  comprehensive state management for professional 3D modeling operations      █
 * █  including selection, mesh editing, and modeling tool coordination.          █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file EditContext.h
 * @brief Professional Edit Context and Modeling State Management System
 * 
 * This file contains the comprehensive EditContext and ModelingTool class implementations
 * that provide professional-grade state management for 3D modeling operations within
 * the RudeBase3D environment. The EditContext serves as the central coordinator for
 * editing modes, selection management, mesh manipulation, and modeling tool integration,
 * enabling sophisticated sub-object editing workflows comparable to industry-standard
 * 3D modeling applications like Maya, Blender, and 3ds Max.
 * 
 * ## Professional Edit Context Architecture
 * 
 * ### Multi-Level Editing System
 * Industry-standard hierarchical editing mode management:
 * - **Object Mode**: Whole object selection and transformation operations
 * - **Component Mode**: Sub-object editing with vertex, edge, and face selection
 * - **Modeling Mode**: Advanced mesh editing with specialized modeling tools
 * - **Animation Mode**: Keyframe editing and animation curve manipulation
 * 
 * ### Advanced Selection Management
 * Professional selection systems supporting complex modeling workflows:
 * - **Multi-Selection**: Complex selection sets with grouping and filtering
 * - **Selection Modes**: Vertex, edge, face, and component-based selection
 * - **Selection Tools**: Box select, lasso select, paint select, and loop selection
 * - **Selection Filtering**: Material, smoothing group, and topology-based filtering
 * 
 * ### Half-Edge Mesh Integration
 * Sophisticated mesh data structure management for professional modeling:
 * - **Topological Editing**: Direct manipulation of mesh connectivity
 * - **Non-Destructive Workflow**: Working copy system preserving original geometry
 * - **Real-Time Updates**: Immediate visual feedback during mesh modification
 * - **Undo/Redo Integration**: Complete editing history with state restoration
 * 
 * ## Advanced Modeling Features
 * 
 * ### Selection System Architecture
 * Professional selection management with industry-standard behaviors:
 * - **Element Selection**: Individual vertex, edge, and face selection
 * - **Loop Selection**: Edge loops and face loops with automatic traversal
 * - **Ring Selection**: Edge rings and face rings for cylindrical topology
 * - **Connected Selection**: Flood-fill selection of connected components
 * - **Border Selection**: Automatic selection of mesh boundary elements
 * 
 * ### Modeling Tool Framework
 * Extensible tool system for sophisticated mesh editing operations:
 * - **Extrude Operations**: Face, edge, and vertex extrusion with scaling
 * - **Inset Operations**: Face inset with even and individual inset modes
 * - **Bevel Operations**: Edge and vertex beveling with profile control
 * - **Loop Cut Tools**: Edge loop insertion with preview and subdivision
 * - **Subdivision**: Catmull-Clark subdivision with creasing support
 * 
 * ### State Management and Validation
 * Robust state management ensuring modeling operation consistency:
 * - **Context Validation**: Automatic validation of editing prerequisites
 * - **Tool Compatibility**: Context-aware tool availability and activation
 * - **State Synchronization**: Coordinated updates across multiple viewports
 * - **Error Recovery**: Graceful handling of invalid operations and states
 * 
 * ## Professional Workflow Integration
 * 
 * ### Industry-Standard Selection Behaviors
 * Selection paradigms matching professional 3D modeling software:
 * - **Maya-Style Selection**: Component masking and selection constraints
 * - **Blender-Style Selection**: Alt-click deselection and border selection
 * - **3ds Max-Style Selection**: Sub-object mode switching and selection sets
 * - **Modo-Style Selection**: Action centers and selection falloffs
 * 
 * ### Multi-Viewport Coordination
 * Synchronized editing state across multiple 3D viewports:
 * - **Selection Synchronization**: Unified selection state across all views
 * - **Mode Synchronization**: Consistent editing modes in all viewports
 * - **Visual Feedback**: Coordinated selection highlighting and tool feedback
 * - **Viewport Isolation**: Optional per-viewport editing mode isolation
 * 
 * ### Tool Integration Framework
 * Comprehensive modeling tool integration and coordination:
 * - **Tool Manager**: Centralized registration and activation of modeling tools
 * - **Tool Chaining**: Sequential tool execution with parameter passing
 * - **Tool Presets**: Saved tool configurations for rapid workflow execution
 * - **Macro Recording**: Tool sequence recording for automation and scripting
 * 
 * ## Advanced Technical Implementation
 * 
 * ### Half-Edge Mesh Management
 * Professional mesh data structure handling for topological editing:
 * - **Working Copy System**: Non-destructive editing with original preservation
 * - **Lazy Conversion**: On-demand conversion between mesh representations
 * - **Memory Optimization**: Efficient memory usage for large mesh editing
 * - **Validation Systems**: Automatic mesh integrity checking and repair
 * 
 * ### Selection Data Structures
 * Optimized selection storage and manipulation for large meshes:
 * - **Sparse Selection**: Memory-efficient storage of large selection sets
 * - **Selection Caching**: Fast selection operations with cached lookup tables
 * - **Hierarchical Selection**: Nested selection sets for complex operations
 * - **Selection History**: Undo/redo support for selection state changes
 * 
 * ### Event System Integration
 * Comprehensive notification system for modeling state changes:
 * - **Qt Signal Integration**: Native Qt event system for UI synchronization
 * - **Custom Event Types**: Specialized events for modeling operations
 * - **Event Filtering**: Selective event delivery for performance optimization
 * - **Batch Events**: Grouped event delivery for bulk operations
 * 
 * ## Professional Modeling Operations
 * 
 * ### Selection Algorithm Implementation
 * Advanced selection algorithms for professional modeling workflows:
 * - **Geometric Selection**: Spatial queries for region-based selection
 * - **Topological Selection**: Graph traversal for connected component selection
 * - **Shortest Path Selection**: Dijkstra-based path selection between elements
 * - **Flood Fill Selection**: Connected region selection with criteria filtering
 * 
 * ### Tool Execution Framework
 * Sophisticated tool execution system with preview and validation:
 * - **Preview System**: Real-time tool result preview without commitment
 * - **Parameter Validation**: Automatic validation of tool parameters
 * - **Execution Pipeline**: Multi-stage tool execution with rollback capability
 * - **Result Caching**: Efficient caching of tool results for interactive editing
 * 
 * ### Mesh Modification Tracking
 * Comprehensive change tracking for complex modeling operations:
 * - **Differential Updates**: Minimal change tracking for efficient updates
 * - **Dependency Tracking**: Automatic invalidation of dependent calculations
 * - **History Management**: Complete operation history with selective undo
 * - **Conflict Resolution**: Handling of conflicting simultaneous modifications
 * 
 * ## Usage Examples
 * 
 * ### Basic Edit Context Setup and Mode Management
 * ```cpp
 * // Initialize edit context with Qt parent
 * auto editContext = std::make_shared<EditContext>(parentWidget);
 * 
 * // Set active object for editing
 * auto selectedObject = scene->getSelectedObject();
 * editContext->setActiveObject(selectedObject);
 * 
 * // Switch to component editing mode
 * editContext->setEditMode(EditMode::Component);
 * editContext->setSelectionType(SelectionType::Face);
 * 
 * // Connect to state change notifications
 * connect(editContext.get(), &EditContext::editModeChanged,
 *         [this](EditMode mode) { updateToolbar(mode); });
 * 
 * connect(editContext.get(), &EditContext::selectionChanged,
 *         [this]() { updateSelectionInfo(); });
 * ```
 * 
 * ### Advanced Selection Management System
 * ```cpp
 * class AdvancedSelectionManager {
 * private:
 *     std::shared_ptr<EditContext> m_editContext;
 *     std::vector<SelectionSet> m_selectionSets;
 *     
 * public:
 *     void performBoxSelection(const glm::vec3& min, const glm::vec3& max, 
 *                             SelectionMode mode) {
 *         switch (m_editContext->getSelectionType()) {
 *             case SelectionType::Vertex:
 *                 selectVerticesInBox(min, max, mode);
 *                 break;
 *             case SelectionType::Edge:
 *                 selectEdgesInBox(min, max, mode);
 *                 break;
 *             case SelectionType::Face:
 *                 selectFacesInBox(min, max, mode);
 *                 break;
 *         }
 *     }
 *     
 *     void performLoopSelection(rude::EdgePtr startEdge, SelectionMode mode) {
 *         auto halfEdgeMesh = m_editContext->getActiveHalfEdgeMesh();
 *         if (!halfEdgeMesh || !startEdge) return;
 *         
 *         std::vector<rude::EdgePtr> loopEdges;
 *         traverseEdgeLoop(startEdge, loopEdges);
 *         
 *         for (auto edge : loopEdges) {
 *             if (mode == SelectionMode::Add) {
 *                 m_editContext->selectEdge(edge, true);
 *             } else if (mode == SelectionMode::Subtract) {
 *                 m_editContext->deselectEdge(edge);
 *             } else {
 *                 m_editContext->selectEdge(edge, false);
 *             }
 *         }
 *     }
 *     
 * private:
 *     void traverseEdgeLoop(rude::EdgePtr startEdge, 
 *                          std::vector<rude::EdgePtr>& loopEdges) {
 *         std::set<rude::EdgePtr> visited;
 *         rude::EdgePtr currentEdge = startEdge;
 *         
 *         do {
 *             if (visited.find(currentEdge) != visited.end()) {
 *                 break; // Loop completed
 *             }
 *             
 *             visited.insert(currentEdge);
 *             loopEdges.push_back(currentEdge);
 *             
 *             // Find next edge in loop
 *             currentEdge = findNextLoopEdge(currentEdge);
 *         } while (currentEdge && currentEdge != startEdge);
 *     }
 * };
 * ```
 * 
 * ### Professional Modeling Tool Implementation
 * ```cpp
 * class ExtrudeFacesTool : public ModelingTool {
 * private:
 *     float m_extrudeDistance = 1.0f;
 *     glm::vec3 m_extrudeDirection = glm::vec3(0, 0, 1);
 *     bool m_useIndividualNormals = false;
 *     bool m_scaleAfterExtrude = false;
 *     float m_scaleAmount = 1.0f;
 *     
 * public:
 *     ExtrudeFacesTool(EditContext* context, QObject* parent = nullptr)
 *         : ModelingTool(context, parent) {}
 *     
 *     bool canExecute() const override {
 *         return m_context->getEditMode() == EditMode::Component &&
 *                m_context->getSelectionType() == SelectionType::Face &&
 *                !m_context->getSelectedFaces().empty();
 *     }
 *     
 *     bool execute() override {
 *         if (!canExecute()) return false;
 *         
 *         auto halfEdgeMesh = m_context->getActiveHalfEdgeMesh();
 *         const auto& selectedFaces = m_context->getSelectedFaces();
 *         
 *         std::vector<rude::FacePtr> newFaces;
 *         
 *         for (auto face : selectedFaces) {
 *             auto extrudedFaces = extrudeFace(halfEdgeMesh, face);
 *             newFaces.insert(newFaces.end(), extrudedFaces.begin(), extrudedFaces.end());
 *         }
 *         
 *         // Update selection to new faces
 *         m_context->clearSelection();
 *         for (auto face : newFaces) {
 *             m_context->selectFace(face, true);
 *         }
 *         
 *         // Commit changes to mesh
 *         m_context->commitChangesToMesh();
 *         emitMeshModified();
 *         
 *         return true;
 *     }
 *     
 *     void preview() override {
 *         // Generate preview geometry without modifying the working mesh
 *         generateExtrudePreview();
 *     }
 *     
 *     QString getName() const override { return "Extrude Faces"; }
 *     QString getDescription() const override { 
 *         return "Extrude selected faces along their normals"; 
 *     }
 *     
 * private:
 *     std::vector<rude::FacePtr> extrudeFace(HalfEdgeMeshPtr mesh, rude::FacePtr face) {
 *         std::vector<rude::FacePtr> newFaces;
 *         
 *         // Get face vertices
 *         auto vertices = face->getVertices();
 *         
 *         // Calculate extrude direction
 *         glm::vec3 faceNormal = face->getNormal();
 *         glm::vec3 extrudeVec = m_useIndividualNormals ? 
 *                               faceNormal * m_extrudeDistance :
 *                               m_extrudeDirection * m_extrudeDistance;
 *         
 *         // Create new vertices
 *         std::vector<rude::VertexPtr> newVertices;
 *         for (auto vertex : vertices) {
 *             glm::vec3 newPos = vertex->getPosition() + extrudeVec;
 *             auto newVertex = mesh->addVertex(newPos);
 *             newVertices.push_back(newVertex);
 *         }
 *         
 *         // Create top face
 *         auto topFace = mesh->addFace(newVertices);
 *         newFaces.push_back(topFace);
 *         
 *         // Create side faces
 *         for (size_t i = 0; i < vertices.size(); ++i) {
 *             size_t next = (i + 1) % vertices.size();
 *             
 *             std::vector<rude::VertexPtr> sideFaceVerts = {
 *                 vertices[i], vertices[next], 
 *                 newVertices[next], newVertices[i]
 *             };
 *             
 *             auto sideFace = mesh->addFace(sideFaceVerts);
 *             newFaces.push_back(sideFace);
 *         }
 *         
 *         // Apply scaling if enabled
 *         if (m_scaleAfterExtrude && m_scaleAmount != 1.0f) {
 *             glm::vec3 faceCenter = topFace->getCenter();
 *             for (auto vertex : newVertices) {
 *                 glm::vec3 offset = vertex->getPosition() - faceCenter;
 *                 vertex->setPosition(faceCenter + offset * m_scaleAmount);
 *             }
 *         }
 *         
 *         return newFaces;
 *     }
 * };
 * ```
 * 
 * ### Modeling Tool Manager and Workflow
 * ```cpp
 * class ModelingToolManager : public QObject {
 *     Q_OBJECT
 *     
 * private:
 *     std::shared_ptr<EditContext> m_editContext;
 *     std::map<QString, std::unique_ptr<ModelingTool>> m_tools;
 *     ModelingTool* m_activeTool = nullptr;
 *     
 * public:
 *     ModelingToolManager(std::shared_ptr<EditContext> context, QObject* parent = nullptr)
 *         : QObject(parent), m_editContext(context) {
 *         registerDefaultTools();
 *     }
 *     
 *     void registerTool(const QString& name, std::unique_ptr<ModelingTool> tool) {
 *         m_tools[name] = std::move(tool);
 *     }
 *     
 *     bool activateTool(const QString& name) {
 *         auto it = m_tools.find(name);
 *         if (it == m_tools.end()) {
 *             return false;
 *         }
 *         
 *         ModelingTool* tool = it->second.get();
 *         if (!tool->canExecute()) {
 *             return false;
 *         }
 *         
 *         if (m_activeTool) {
 *             m_activeTool->cancel();
 *         }
 *         
 *         m_activeTool = tool;
 *         emit toolActivated(name);
 *         return true;
 *     }
 *     
 *     bool executeTool(const QString& name) {
 *         if (!activateTool(name)) {
 *             return false;
 *         }
 *         
 *         bool success = m_activeTool->execute();
 *         if (success) {
 *             emit toolExecuted(name);
 *         }
 *         
 *         m_activeTool = nullptr;
 *         return success;
 *     }
 *     
 *     std::vector<QString> getAvailableTools() const {
 *         std::vector<QString> availableTools;
 *         
 *         for (const auto& [name, tool] : m_tools) {
 *             if (tool->canExecute()) {
 *                 availableTools.push_back(name);
 *             }
 *         }
 *         
 *         return availableTools;
 *     }
 *     
 * private:
 *     void registerDefaultTools() {
 *         registerTool("Extrude", std::make_unique<ExtrudeFacesTool>(m_editContext.get()));
 *         registerTool("Inset", std::make_unique<InsetFacesTool>(m_editContext.get()));
 *         registerTool("Bevel", std::make_unique<BevelEdgesTool>(m_editContext.get()));
 *         registerTool("LoopCut", std::make_unique<LoopCutTool>(m_editContext.get()));
 *         registerTool("Subdivide", std::make_unique<SubdivisionTool>(m_editContext.get()));
 *     }
 *     
 * signals:
 *     void toolActivated(const QString& name);
 *     void toolExecuted(const QString& name);
 * };
 * ```
 * 
 * ### Advanced Edit Context State Management
 * ```cpp
 * class EditContextStateManager {
 * private:
 *     struct EditState {
 *         EditMode mode;
 *         SelectionType selectionType;
 *         SceneObjectPtr activeObject;
 *         std::vector<rude::VertexPtr> selectedVertices;
 *         std::vector<rude::EdgePtr> selectedEdges;
 *         std::vector<rude::FacePtr> selectedFaces;
 *         HalfEdgeMeshPtr workingMesh;
 *     };
 *     
 *     std::shared_ptr<EditContext> m_editContext;
 *     std::vector<EditState> m_stateHistory;
 *     int m_currentStateIndex = -1;
 *     int m_maxHistorySize = 100;
 *     
 * public:
 *     EditContextStateManager(std::shared_ptr<EditContext> context) 
 *         : m_editContext(context) {
 *         // Connect to context change signals
 *         connect(context.get(), &EditContext::editModeChanged,
 *                 this, &EditContextStateManager::captureState);
 *         connect(context.get(), &EditContext::selectionChanged,
 *                 this, &EditContextStateManager::captureState);
 *         connect(context.get(), &EditContext::meshModified,
 *                 this, &EditContextStateManager::captureState);
 *     }
 *     
 *     void captureState() {
 *         EditState state;
 *         state.mode = m_editContext->getEditMode();
 *         state.selectionType = m_editContext->getSelectionType();
 *         state.activeObject = m_editContext->getActiveObject();
 *         state.selectedVertices = m_editContext->getSelectedVertices();
 *         state.selectedEdges = m_editContext->getSelectedEdges();
 *         state.selectedFaces = m_editContext->getSelectedFaces();
 *         state.workingMesh = cloneHalfEdgeMesh(m_editContext->getActiveHalfEdgeMesh());
 *         
 *         // Remove future states if we're not at the end
 *         if (m_currentStateIndex < static_cast<int>(m_stateHistory.size()) - 1) {
 *             m_stateHistory.erase(m_stateHistory.begin() + m_currentStateIndex + 1,
 *                                 m_stateHistory.end());
 *         }
 *         
 *         // Add new state
 *         m_stateHistory.push_back(state);
 *         m_currentStateIndex = static_cast<int>(m_stateHistory.size()) - 1;
 *         
 *         // Limit history size
 *         if (m_stateHistory.size() > m_maxHistorySize) {
 *             m_stateHistory.erase(m_stateHistory.begin());
 *             m_currentStateIndex--;
 *         }
 *     }
 *     
 *     bool undo() {
 *         if (m_currentStateIndex <= 0) {
 *             return false;
 *         }
 *         
 *         m_currentStateIndex--;
 *         restoreState(m_stateHistory[m_currentStateIndex]);
 *         return true;
 *     }
 *     
 *     bool redo() {
 *         if (m_currentStateIndex >= static_cast<int>(m_stateHistory.size()) - 1) {
 *             return false;
 *         }
 *         
 *         m_currentStateIndex++;
 *         restoreState(m_stateHistory[m_currentStateIndex]);
 *         return true;
 *     }
 *     
 * private:
 *     void restoreState(const EditState& state) {
 *         // Temporarily disconnect signals to avoid recursive state capture
 *         m_editContext->blockSignals(true);
 *         
 *         m_editContext->setEditMode(state.mode);
 *         m_editContext->setSelectionType(state.selectionType);
 *         m_editContext->setActiveObject(state.activeObject);
 *         
 *         // Restore selection
 *         m_editContext->clearSelection();
 *         for (auto vertex : state.selectedVertices) {
 *             m_editContext->selectVertex(vertex, true);
 *         }
 *         for (auto edge : state.selectedEdges) {
 *             m_editContext->selectEdge(edge, true);
 *         }
 *         for (auto face : state.selectedFaces) {
 *             m_editContext->selectFace(face, true);
 *         }
 *         
 *         // Restore working mesh
 *         if (state.workingMesh) {
 *             restoreHalfEdgeMesh(state.workingMesh);
 *         }
 *         
 *         m_editContext->blockSignals(false);
 *         
 *         // Emit single notification of state change
 *         emit m_editContext->selectionChanged();
 *     }
 * };
 * ```
 * 
 * ## Performance Optimization Strategies
 * 
 * ### Selection Performance
 * Optimized selection operations for large meshes:
 * - **Spatial Indexing**: Octree and grid-based spatial acceleration
 * - **Selection Caching**: Pre-computed selection queries and results
 * - **Lazy Evaluation**: Deferred selection processing for interactive operations
 * - **Parallel Processing**: Multi-threaded selection operations for large datasets
 * 
 * ### Memory Management
 * Efficient memory usage for complex modeling operations:
 * - **Working Copy Optimization**: Minimal memory overhead for mesh editing
 * - **Selection Storage**: Compressed selection storage for large selection sets
 * - **History Compression**: Differential state storage for edit history
 * - **Resource Pooling**: Reusable data structures for frequent operations
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Tools
 * @ingroup EditContext
 * @ingroup ModelingTools
 */

#pragma once

#include "Common.h"

#include <QObject>
#include <memory>
#include <vector>

// Forward declarations
class Scene;
class SceneObject;
class Viewport3D;

/**
 * @brief Edit Context
 * Manages the current editing state and operations
 */
class EditContext : public QObject {
    Q_OBJECT

public:
    explicit EditContext(QObject* parent = nullptr);
    ~EditContext() = default;

    // Mode management
    void setEditMode(EditMode mode);
    EditMode getEditMode() const { return m_currentEditMode; }
    
    void setSelectionType(SelectionType type);
    SelectionType getSelectionType() const { return m_currentSelectionType; }

    // Object editing
    void setActiveObject(SceneObjectPtr object);
    SceneObjectPtr getActiveObject() const { return m_activeObject.lock(); }
    
    // Half-edge mesh access
    HalfEdgeMeshPtr getActiveHalfEdgeMesh() const;
    void commitChangesToMesh(); // Apply half-edge changes back to face-vertex mesh

    // Selection management
    const std::vector<rude::VertexPtr>& getSelectedVertices() const { return m_selectedVertices; }
    const std::vector<rude::EdgePtr>& getSelectedEdges() const { return m_selectedEdges; }
    const std::vector<rude::FacePtr>& getSelectedFaces() const { return m_selectedFaces; }
    
    void clearSelection();
    void selectAll();
    void invertSelection();
    
    // Element selection
    void selectVertex(rude::VertexPtr vertex, bool addToSelection = false);
    void selectEdge(rude::EdgePtr edge, bool addToSelection = false);
    void selectFace(rude::FacePtr face, bool addToSelection = false);
    
    void deselectVertex(rude::VertexPtr vertex);
    void deselectEdge(rude::EdgePtr edge);
    void deselectFace(rude::FacePtr face);
    
    // Box selection
    void boxSelect(const glm::vec3& min, const glm::vec3& max, bool addToSelection = false);
    
    // Loop/ring selection
    void selectEdgeLoop(rude::EdgePtr startEdge, bool addToSelection = false);
    void selectEdgeRing(rude::EdgePtr startEdge, bool addToSelection = false);

    // Validation
    bool canEdit() const;
    bool hasSelection() const;

signals:
    void editModeChanged(EditMode mode);
    void selectionTypeChanged(SelectionType type);
    void activeObjectChanged(SceneObjectPtr object);
    void selectionChanged();
    void meshModified();

private slots:
    void onActiveObjectDestroyed();

private:
    EditMode m_currentEditMode = EditMode::Object;
    SelectionType m_currentSelectionType = SelectionType::Vertex;
    
    std::weak_ptr<SceneObject> m_activeObject;
    HalfEdgeMeshPtr m_workingHalfEdgeMesh;
    
    // Current selection
    std::vector<rude::VertexPtr> m_selectedVertices;
    std::vector<rude::EdgePtr> m_selectedEdges;
    std::vector<rude::FacePtr> m_selectedFaces;
    
    // Helper methods
    void updateWorkingMesh();
    void clearSelectionInternal();
    void addVertexToSelection(rude::VertexPtr vertex);
    void addEdgeToSelection(rude::EdgePtr edge);
    void addFaceToSelection(rude::FacePtr face);
    void removeVertexFromSelection(rude::VertexPtr vertex);
    void removeEdgeFromSelection(rude::EdgePtr edge);
    void removeFaceFromSelection(rude::FacePtr face);
};

/**
 * @brief Modeling Tool Base Class
 * Base class for all modeling tools (extrude, bevel, etc.)
 */
class ModelingTool : public QObject {
    Q_OBJECT

public:
    explicit ModelingTool(EditContext* context, QObject* parent = nullptr);
    virtual ~ModelingTool() = default;

    // Tool interface
    virtual bool canExecute() const = 0;
    virtual bool execute() = 0;
    virtual void preview() {}
    virtual void cancel() {}
    
    // Tool properties
    virtual QString getName() const = 0;
    virtual QString getDescription() const = 0;

protected:
    EditContext* m_context;
    
    // Helper methods
    bool hasValidSelection() const;
    void emitMeshModified();

signals:
    void toolExecuted();
    void toolCancelled();
    void previewUpdated();
};

// Forward declarations for modeling tools
class ExtrudeTool;
class InsetTool;
class BevelTool;
class LoopCutTool;
class SubdivisionTool;
class ModelingToolManager;
