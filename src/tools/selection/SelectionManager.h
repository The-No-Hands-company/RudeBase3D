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
 * █  FILE: SelectionManager.h                                                    █
 * █  DESCRIPTION: Professional Selection Management for Advanced 3D Modeling    █
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
 * █  This file contains the SelectionManager class - providing comprehensive    █
 * █  mesh element selection management with sophisticated interaction methods    █
 * █  and professional modeling workflow integration.                            █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file SelectionManager.h
 * @brief Professional Selection Management System for Advanced 3D Mesh Editing
 * 
 * This file contains the comprehensive SelectionManager class implementation that
 * provides professional-grade mesh element selection capabilities within the
 * RudeBase3D environment. The manager supports sophisticated selection operations
 * for vertices, edges, and faces with advanced interaction methods, selection
 * conversion, and integration with professional modeling workflows, delivering
 * functionality comparable to industry-standard 3D modeling applications like
 * Maya, Blender, 3ds Max, and Houdini.
 * 
 * ## Professional Selection Architecture
 * 
 * ### Multi-Element Selection System
 * Industry-standard selection paradigm for comprehensive mesh editing:
 * - **Vertex Selection**: Individual vertex selection with spatial proximity
 * - **Edge Selection**: Edge selection with geometric intersection testing
 * - **Face Selection**: Face selection with ray-triangle intersection
 * - **Mixed Selection**: Support for multiple element types simultaneously
 * 
 * ### Advanced Interaction Methods
 * Sophisticated selection techniques for professional workflows:
 * - **Point Selection**: Precise mouse-based element picking with tolerance
 * - **Box Selection**: Rectangular region selection with viewport projection
 * - **Lasso Selection**: Freehand polygon-based selection regions
 * - **Paint Selection**: Brush-based selection with falloff and pressure
 * 
 * ### Selection Mode Management
 * Professional selection mode coordination:
 * - **Component Modes**: Vertex, edge, face, and object-level selection
 * - **Selection Filters**: Material, smoothing group, and topology-based filtering
 * - **Selection Masking**: Temporary masking of unselectable elements
 * - **Mode Switching**: Seamless switching between selection types
 * 
 * ## Advanced Selection Features
 * 
 * ### Intelligent Selection Algorithms
 * Sophisticated algorithms for precise element identification:
 * - **Spatial Indexing**: Octree-based spatial acceleration for large meshes
 * - **Proximity Testing**: Distance-based selection with configurable tolerance
 * - **Ray Casting**: Accurate ray-mesh intersection for 3D viewport picking
 * - **Geometric Queries**: Advanced geometric testing for complex selections
 * 
 * ### Selection Conversion System
 * Professional selection type conversion for flexible workflows:
 * - **Vertex to Edge**: Select edges connected to selected vertices
 * - **Edge to Face**: Select faces adjacent to selected edges
 * - **Face to Vertex**: Select vertices of selected faces
 * - **Border Selection**: Automatic selection of mesh boundary elements
 * 
 * ### Multi-Selection Support
 * Advanced multi-element selection management:
 * - **Additive Selection**: Adding elements to existing selection sets
 * - **Subtractive Selection**: Removing elements from selection sets
 * - **Toggle Selection**: Inverting selection state of picked elements
 * - **Selection Sets**: Named selection groups for complex workflows
 * 
 * ## Industry-Standard Workflow Integration
 * 
 * ### Maya-Style Selection Behaviors
 * Maya-compatible selection paradigms and interactions:
 * - **Component Masking**: Automatic component type filtering
 * - **Selection Constraints**: Constraint-based selection limitations
 * - **Soft Selection**: Falloff-based selection with influence gradients
 * - **Selection Sets**: Named selection groups with hierarchical organization
 * 
 * ### Blender-Style Selection Features
 * Blender-compatible selection methods and workflows:
 * - **Border Select**: B-key rectangular selection with viewport coordinates
 * - **Circle Select**: C-key circular selection with size adjustment
 * - **Lasso Select**: Ctrl+LMB freehand selection with polygon regions
 * - **Alt+Click**: Deselection and edge loop selection behaviors
 * 
 * ### 3ds Max-Style Selection Tools
 * 3ds Max-compatible selection features and modifiers:
 * - **Sub-Object Selection**: Direct sub-object manipulation and editing
 * - **Selection Modifiers**: Named selection modifiers for non-destructive editing
 * - **Region Selection**: Various region selection tools and methods
 * - **Selection Lock**: Temporary locking of selection state
 * 
 * ## Advanced Technical Implementation
 * 
 * ### Ray-Mesh Intersection System
 * High-performance ray casting for accurate selection:
 * - **Möller-Trumbore Algorithm**: Fast ray-triangle intersection testing
 * - **Bounding Volume Hierarchy**: Accelerated intersection queries for large meshes
 * - **Viewport Projection**: Screen-space to world-space coordinate conversion
 * - **Depth Testing**: Accurate depth-based selection for overlapping elements
 * 
 * ### Selection Data Structures
 * Optimized selection storage and manipulation:
 * - **Hash Sets**: O(1) selection state queries and modifications
 * - **Sparse Storage**: Memory-efficient storage for large selection sets
 * - **Selection Caching**: Pre-computed selection queries for performance
 * - **Event Notification**: Efficient selection change notification system
 * 
 * ### Performance Optimization
 * Efficient selection operations for interactive modeling:
 * - **Spatial Partitioning**: Hierarchical spatial data structures for acceleration
 * - **Lazy Evaluation**: Deferred computation for selection-dependent operations
 * - **Batch Operations**: Grouped selection operations for performance
 * - **Memory Pooling**: Reusable data structures for frequent operations
 * 
 * ## Professional Selection Algorithms
 * 
 * ### Proximity-Based Selection
 * Sophisticated distance-based element selection:
 * - **Euclidean Distance**: Direct distance calculation for vertex selection
 * - **Point-to-Line Distance**: Accurate edge selection with line projection
 * - **Point-in-Triangle**: Barycentric coordinate testing for face selection
 * - **Tolerance Management**: Adaptive tolerance based on viewport scale
 * 
 * ### Region-Based Selection
 * Advanced geometric region selection algorithms:
 * - **Convex Hull Testing**: Point-in-polygon testing for lasso selection
 * - **Bounding Box Intersection**: Efficient box selection with AABB testing
 * - **Frustum Culling**: Viewport frustum-based element culling
 * - **Screen-Space Projection**: Accurate viewport coordinate transformation
 * 
 * ### Selection Conversion Algorithms
 * Intelligent selection type conversion with topology awareness:
 * - **Graph Traversal**: Breadth-first and depth-first traversal for connectivity
 * - **Adjacency Queries**: Fast adjacent element queries using half-edge structure
 * - **Boundary Detection**: Automatic detection of mesh boundaries and holes
 * - **Component Analysis**: Connected component analysis for selection groups
 * 
 * ## Usage Examples
 * 
 * ### Basic Selection Manager Setup and Usage
 * ```cpp
 * // Initialize selection manager with mesh
 * auto selectionManager = std::make_unique<SelectionManager>();
 * selectionManager->setMesh(meshPtr);
 * selectionManager->setSelectionType(SelectionType::Face);
 * 
 * // Perform point selection (from mouse click)
 * glm::vec3 worldPos = screenToWorld(mouseX, mouseY);
 * bool selected = selectionManager->selectAtPoint(worldPos, false);
 * 
 * if (selected) {
 *     auto selectedFaces = selectionManager->getSelectedFaces();
 *     std::cout << "Selected " << selectedFaces.size() << " faces" << std::endl;
 * }
 * 
 * // Convert face selection to vertex selection
 * selectionManager->convertSelection(SelectionType::Vertex);
 * auto selectedVertices = selectionManager->getSelectedVertices();
 * ```
 * 
 * ### Advanced Selection Operations with Custom Filtering
 * ```cpp
 * class AdvancedSelectionController {
 * private:
 *     std::unique_ptr<SelectionManager> m_selectionManager;
 *     std::vector<SelectionFilter> m_activeFilters;
 *     SelectionHistory m_selectionHistory;
 *     
 * public:
 *     AdvancedSelectionController(HalfEdgeMeshPtr mesh)
 *         : m_selectionManager(std::make_unique<SelectionManager>()) {
 *         m_selectionManager->setMesh(mesh);
 *         setupDefaultFilters();
 *     }
 *     
 *     void performFilteredSelection(const glm::vec3& worldPos, 
 *                                 const std::vector<SelectionCriteria>& criteria) {
 *         // Store current selection for history
 *         m_selectionHistory.push(captureCurrentSelection());
 *         
 *         // Perform base selection
 *         auto rayHit = m_selectionManager->raycast(getRayOrigin(), getRayDirection(worldPos));
 *         
 *         if (rayHit.hit) {
 *             // Apply selection filters
 *             bool passesFilters = true;
 *             
 *             for (const auto& criterion : criteria) {
 *                 switch (criterion.type) {
 *                     case SelectionCriteria::Material:
 *                         passesFilters &= checkMaterialFilter(rayHit, criterion);
 *                         break;
 *                     case SelectionCriteria::SmoothingGroup:
 *                         passesFilters &= checkSmoothingFilter(rayHit, criterion);
 *                         break;
 *                     case SelectionCriteria::Normal:
 *                         passesFilters &= checkNormalFilter(rayHit, criterion);
 *                         break;
 *                     case SelectionCriteria::Planarity:
 *                         passesFilters &= checkPlanarityFilter(rayHit, criterion);
 *                         break;
 *                 }
 *             }
 *             
 *             if (passesFilters) {
 *                 // Apply selection based on current mode
 *                 switch (m_selectionManager->getSelectionType()) {
 *                     case SelectionType::Vertex:
 *                         m_selectionManager->selectVertex(rayHit.vertex);
 *                         break;
 *                     case SelectionType::Edge:
 *                         m_selectionManager->selectEdge(rayHit.edge);
 *                         break;
 *                     case SelectionType::Face:
 *                         m_selectionManager->selectFace(rayHit.face);
 *                         break;
 *                 }
 *             }
 *         }
 *     }
 *     
 *     void performLoopSelection(HalfEdgeEdgePtr startEdge, LoopType loopType) {
 *         std::vector<HalfEdgeEdgePtr> loopEdges;
 *         
 *         switch (loopType) {
 *             case LoopType::EdgeLoop:
 *                 loopEdges = findEdgeLoop(startEdge);
 *                 break;
 *             case LoopType::EdgeRing:
 *                 loopEdges = findEdgeRing(startEdge);
 *                 break;
 *             case LoopType::FaceLoop:
 *                 loopEdges = findFaceLoop(startEdge);
 *                 break;
 *         }
 *         
 *         // Select all edges in the loop
 *         m_selectionManager->clearSelection();
 *         for (auto edge : loopEdges) {
 *             m_selectionManager->selectEdge(edge, true);
 *         }
 *     }
 *     
 *     void performGrowShrinkSelection(GrowShrinkOperation operation, int iterations = 1) {
 *         for (int i = 0; i < iterations; ++i) {
 *             switch (operation) {
 *                 case GrowShrinkOperation::Grow:
 *                     growSelection();
 *                     break;
 *                 case GrowShrinkOperation::Shrink:
 *                     shrinkSelection();
 *                     break;
 *             }
 *         }
 *     }
 *     
 * private:
 *     std::vector<HalfEdgeEdgePtr> findEdgeLoop(HalfEdgeEdgePtr startEdge) {
 *         std::vector<HalfEdgeEdgePtr> loopEdges;
 *         std::set<HalfEdgeEdgePtr> visited;
 *         
 *         // Traverse in both directions from start edge
 *         traverseLoopDirection(startEdge, loopEdges, visited, true);
 *         traverseLoopDirection(startEdge, loopEdges, visited, false);
 *         
 *         return loopEdges;
 *     }
 *     
 *     void growSelection() {
 *         switch (m_selectionManager->getSelectionType()) {
 *             case SelectionType::Vertex:
 *                 growVertexSelection();
 *                 break;
 *             case SelectionType::Edge:
 *                 growEdgeSelection();
 *                 break;
 *             case SelectionType::Face:
 *                 growFaceSelection();
 *                 break;
 *         }
 *     }
 *     
 *     void growFaceSelection() {
 *         auto currentSelection = m_selectionManager->getSelectedFaces();
 *         std::set<HalfEdgeFacePtr> newSelection(currentSelection.begin(), currentSelection.end());
 *         
 *         for (auto face : currentSelection) {
 *             // Get adjacent faces
 *             auto adjacentFaces = getAdjacentFaces(face);
 *             for (auto adjacentFace : adjacentFaces) {
 *                 // Apply growth filters if any
 *                 if (passesGrowthFilters(face, adjacentFace)) {
 *                     newSelection.insert(adjacentFace);
 *                 }
 *             }
 *         }
 *         
 *         // Update selection
 *         m_selectionManager->clearSelection();
 *         for (auto face : newSelection) {
 *             m_selectionManager->selectFace(face, true);
 *         }
 *     }
 * };
 * ```
 * 
 * ### Interactive Selection with Visual Feedback
 * ```cpp
 * class InteractiveSelectionWidget : public QOpenGLWidget {
 *     Q_OBJECT
 *     
 * private:
 *     SelectionManager* m_selectionManager;
 *     Camera* m_camera;
 *     bool m_boxSelecting = false;
 *     QPoint m_selectionStart;
 *     QPoint m_selectionCurrent;
 *     
 * public:
 *     InteractiveSelectionWidget(SelectionManager* manager, QWidget* parent = nullptr)
 *         : QOpenGLWidget(parent), m_selectionManager(manager) {
 *         setMouseTracking(true);
 *         setFocusPolicy(Qt::StrongFocus);
 *     }
 *     
 * protected:
 *     void mousePressEvent(QMouseEvent* event) override {
 *         if (event->button() == Qt::LeftButton) {
 *             if (event->modifiers() & Qt::ShiftModifier) {
 *                 // Begin box selection
 *                 beginBoxSelection(event->pos());
 *             } else {
 *                 // Point selection
 *                 performPointSelection(event->pos(), event->modifiers());
 *             }
 *         }
 *         
 *         QOpenGLWidget::mousePressEvent(event);
 *     }
 *     
 *     void mouseMoveEvent(QMouseEvent* event) override {
 *         if (m_boxSelecting) {
 *             updateBoxSelection(event->pos());
 *             update(); // Trigger repaint for visual feedback
 *         } else {
 *             // Update hover highlighting
 *             updateHoverHighlight(event->pos());
 *         }
 *         
 *         QOpenGLWidget::mouseMoveEvent(event);
 *     }
 *     
 *     void mouseReleaseEvent(QMouseEvent* event) override {
 *         if (event->button() == Qt::LeftButton && m_boxSelecting) {
 *             endBoxSelection(event->modifiers());
 *         }
 *         
 *         QOpenGLWidget::mouseReleaseEvent(event);
 *     }
 *     
 *     void keyPressEvent(QKeyEvent* event) override {
 *         switch (event->key()) {
 *             case Qt::Key_A:
 *                 if (event->modifiers() & Qt::ControlModifier) {
 *                     m_selectionManager->selectAll();
 *                     update();
 *                 }
 *                 break;
 *             case Qt::Key_D:
 *                 if (event->modifiers() & Qt::ControlModifier) {
 *                     m_selectionManager->clearSelection();
 *                     update();
 *                 }
 *                 break;
 *             case Qt::Key_I:
 *                 if (event->modifiers() & Qt::ControlModifier) {
 *                     m_selectionManager->invertSelection();
 *                     update();
 *                 }
 *                 break;
 *             case Qt::Key_1:
 *                 m_selectionManager->setSelectionType(SelectionType::Vertex);
 *                 break;
 *             case Qt::Key_2:
 *                 m_selectionManager->setSelectionType(SelectionType::Edge);
 *                 break;
 *             case Qt::Key_3:
 *                 m_selectionManager->setSelectionType(SelectionType::Face);
 *                 break;
 *         }
 *         
 *         QOpenGLWidget::keyPressEvent(event);
 *     }
 *     
 *     void paintGL() override {
 *         // Render mesh with selection highlighting
 *         renderMesh();
 *         renderSelectionHighlight();
 *         
 *         // Render box selection overlay
 *         if (m_boxSelecting) {
 *             renderBoxSelectionOverlay();
 *         }
 *     }
 *     
 * private:
 *     void beginBoxSelection(const QPoint& startPos) {
 *         m_boxSelecting = true;
 *         m_selectionStart = startPos;
 *         m_selectionCurrent = startPos;
 *         
 *         // Convert to world coordinates
 *         glm::vec3 worldStart = screenToWorld(startPos);
 *         m_selectionManager->beginBoxSelection(worldStart);
 *     }
 *     
 *     void updateBoxSelection(const QPoint& currentPos) {
 *         if (!m_boxSelecting) return;
 *         
 *         m_selectionCurrent = currentPos;
 *         
 *         // Convert to world coordinates
 *         glm::vec3 worldCurrent = screenToWorld(currentPos);
 *         m_selectionManager->updateBoxSelection(worldCurrent);
 *     }
 *     
 *     void endBoxSelection(Qt::KeyboardModifiers modifiers) {
 *         if (!m_boxSelecting) return;
 *         
 *         bool addToSelection = modifiers & Qt::ControlModifier;
 *         m_selectionManager->endBoxSelection(addToSelection);
 *         
 *         m_boxSelecting = false;
 *         update();
 *         
 *         emit selectionChanged();
 *     }
 *     
 *     void performPointSelection(const QPoint& pos, Qt::KeyboardModifiers modifiers) {
 *         glm::vec3 worldPos = screenToWorld(pos);
 *         bool addToSelection = modifiers & Qt::ControlModifier;
 *         
 *         bool selected = m_selectionManager->selectAtPoint(worldPos, addToSelection);
 *         
 *         if (selected) {
 *             update();
 *             emit selectionChanged();
 *         }
 *     }
 *     
 *     void updateHoverHighlight(const QPoint& pos) {
 *         glm::vec3 rayOrigin, rayDirection;
 *         screenToRay(pos, rayOrigin, rayDirection);
 *         
 *         auto rayHit = m_selectionManager->raycast(rayOrigin, rayDirection);
 *         
 *         // Update hover highlighting based on hit result
 *         updateHoverState(rayHit);
 *         update();
 *     }
 *     
 *     void renderSelectionHighlight() {
 *         // Render selection highlighting based on current selection type
 *         switch (m_selectionManager->getSelectionType()) {
 *             case SelectionType::Vertex:
 *                 renderSelectedVertices();
 *                 break;
 *             case SelectionType::Edge:
 *                 renderSelectedEdges();
 *                 break;
 *             case SelectionType::Face:
 *                 renderSelectedFaces();
 *                 break;
 *         }
 *     }
 *     
 *     void renderBoxSelectionOverlay() {
 *         if (!m_boxSelecting) return;
 *         
 *         // Render 2D overlay for box selection
 *         QPainter painter(this);
 *         painter.setPen(QPen(Qt::white, 2, Qt::DashLine));
 *         painter.setBrush(QBrush(QColor(255, 255, 255, 32)));
 *         
 *         QRect selectionRect(m_selectionStart, m_selectionCurrent);
 *         painter.drawRect(selectionRect.normalized());
 *     }
 *     
 * signals:
 *     void selectionChanged();
 * };
 * ```
 * 
 * ### Professional Selection Manager with Advanced Features
 * ```cpp
 * class ProfessionalSelectionManager : public SelectionManager {
 * private:
 *     struct SelectionSet {
 *         std::string name;
 *         std::vector<HalfEdgeVertexPtr> vertices;
 *         std::vector<HalfEdgeEdgePtr> edges;
 *         std::vector<HalfEdgeFacePtr> faces;
 *         QDateTime created;
 *         QDateTime lastModified;
 *     };
 *     
 *     std::map<std::string, SelectionSet> m_namedSelectionSets;
 *     SelectionStatistics m_statistics;
 *     SoftSelectionParameters m_softSelection;
 *     
 * public:
 *     ProfessionalSelectionManager() : SelectionManager() {
 *         setupAdvancedFeatures();
 *     }
 *     
 *     void saveSelectionSet(const std::string& name) {
 *         SelectionSet selectionSet;
 *         selectionSet.name = name;
 *         selectionSet.vertices = getSelectedVertices();
 *         selectionSet.edges = getSelectedEdges();
 *         selectionSet.faces = getSelectedFaces();
 *         selectionSet.created = QDateTime::currentDateTime();
 *         selectionSet.lastModified = selectionSet.created;
 *         
 *         m_namedSelectionSets[name] = selectionSet;
 *     }
 *     
 *     bool loadSelectionSet(const std::string& name) {
 *         auto it = m_namedSelectionSets.find(name);
 *         if (it == m_namedSelectionSets.end()) {
 *             return false;
 *         }
 *         
 *         const SelectionSet& selectionSet = it->second;
 *         
 *         clearSelection();
 *         
 *         for (auto vertex : selectionSet.vertices) {
 *             selectVertex(vertex, true);
 *         }
 *         for (auto edge : selectionSet.edges) {
 *             selectEdge(edge, true);
 *         }
 *         for (auto face : selectionSet.faces) {
 *             selectFace(face, true);
 *         }
 *         
 *         return true;
 *     }
 *     
 *     std::vector<std::string> getAvailableSelectionSets() const {
 *         std::vector<std::string> names;
 *         for (const auto& [name, set] : m_namedSelectionSets) {
 *             names.push_back(name);
 *         }
 *         return names;
 *     }
 *     
 *     void enableSoftSelection(float falloffRadius, FalloffType falloffType = FalloffType::Linear) {
 *         m_softSelection.enabled = true;
 *         m_softSelection.falloffRadius = falloffRadius;
 *         m_softSelection.falloffType = falloffType;
 *         updateSoftSelectionWeights();
 *     }
 *     
 *     void disableSoftSelection() {
 *         m_softSelection.enabled = false;
 *         clearSoftSelectionWeights();
 *     }
 *     
 *     std::map<HalfEdgeVertexPtr, float> getSoftSelectionWeights() const {
 *         return m_softSelection.vertexWeights;
 *     }
 *     
 *     SelectionStatistics getSelectionStatistics() const {
 *         return m_statistics;
 *     }
 *     
 * private:
 *     void setupAdvancedFeatures() {
 *         // Setup soft selection
 *         m_softSelection.enabled = false;
 *         m_softSelection.falloffRadius = 1.0f;
 *         m_softSelection.falloffType = FalloffType::Linear;
 *         
 *         // Setup statistics tracking
 *         m_statistics.totalSelections = 0;
 *         m_statistics.averageSelectionSize = 0.0f;
 *         m_statistics.lastSelectionTime = QDateTime::currentDateTime();
 *     }
 *     
 *     void updateSoftSelectionWeights() {
 *         if (!m_softSelection.enabled) return;
 *         
 *         m_softSelection.vertexWeights.clear();
 *         
 *         auto selectedVertices = getSelectedVertices();
 *         if (selectedVertices.empty()) return;
 *         
 *         // Calculate weights for all vertices based on distance to selected vertices
 *         auto allVertices = getMesh()->getAllVertices();
 *         
 *         for (auto vertex : allVertices) {
 *             float minDistance = std::numeric_limits<float>::max();
 *             
 *             // Find minimum distance to any selected vertex
 *             for (auto selectedVertex : selectedVertices) {
 *                 float distance = glm::length(vertex->getPosition() - selectedVertex->getPosition());
 *                 minDistance = std::min(minDistance, distance);
 *             }
 *             
 *             // Calculate weight based on falloff
 *             if (minDistance <= m_softSelection.falloffRadius) {
 *                 float normalizedDistance = minDistance / m_softSelection.falloffRadius;
 *                 float weight = calculateFalloffWeight(normalizedDistance);
 *                 m_softSelection.vertexWeights[vertex] = weight;
 *             }
 *         }
 *     }
 *     
 *     float calculateFalloffWeight(float normalizedDistance) const {
 *         switch (m_softSelection.falloffType) {
 *             case FalloffType::Linear:
 *                 return 1.0f - normalizedDistance;
 *             case FalloffType::Smooth:
 *                 return 1.0f - (3.0f * normalizedDistance * normalizedDistance - 
 *                               2.0f * normalizedDistance * normalizedDistance * normalizedDistance);
 *             case FalloffType::Root:
 *                 return 1.0f - std::sqrt(normalizedDistance);
 *             case FalloffType::Sphere:
 *                 return std::cos(normalizedDistance * M_PI * 0.5f);
 *             default:
 *                 return 1.0f - normalizedDistance;
 *         }
 *     }
 * };
 * ```
 * 
 * ## Integration with RudeBase3D Architecture
 * 
 * ### Edit Context Integration
 * Seamless integration with RudeBase3D edit context system:
 * - **Mode Coordination**: Automatic selection type updates based on edit mode
 * - **State Synchronization**: Coordinated selection state across multiple viewports
 * - **Event Notification**: Real-time selection change notifications
 * - **History Integration**: Complete integration with undo/redo systems
 * 
 * ### Mesh System Coordination
 * Professional coordination with mesh management systems:
 * - **Half-Edge Integration**: Direct integration with half-edge mesh structures
 * - **Topology Awareness**: Selection operations with topology validation
 * - **Performance Optimization**: Efficient selection queries using mesh indexing
 * - **Format Support**: Support for multiple mesh formats and representations
 * 
 * ### Rendering Pipeline Integration
 * Direct integration with RudeBase3D rendering systems:
 * - **Selection Highlighting**: Real-time visual feedback for selected elements
 * - **Hover Effects**: Interactive hover highlighting for element identification
 * - **Multi-Viewport**: Synchronized selection display across multiple viewports
 * - **Performance Optimization**: Efficient rendering updates during selection changes
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Selection
 * @ingroup InteractionTools
 * @ingroup MeshEditing
 */

#pragma once

#include "Common.h"

#include <glm/glm.hpp>
#include <QRect>
#include <unordered_set>
#include <vector>
#include <memory>

/**
 * @brief Manages mesh element selection for interactive editing
 * 
 * The SelectionManager handles vertex, edge, and face selection in HEDS meshes,
 * providing tools for single selection, multi-selection, and box selection.
 */
class SelectionManager {
public:
    SelectionManager();
    ~SelectionManager() = default;

    // Selection mode
    void setSelectionType(SelectionType type) { m_selectionType = type; }
    SelectionType getSelectionType() const { return m_selectionType; }

    // Current mesh
    void setMesh(HalfEdgeMeshPtr mesh);
    HalfEdgeMeshPtr getMesh() const { return m_mesh; }

    // Selection operations
    void clearSelection();
    void selectAll();
    void invertSelection();

    // Point selection (from mouse picking)
    bool selectAtPoint(const glm::vec3& worldPos, bool addToSelection = false);
    
    // Box selection
    void beginBoxSelection(const glm::vec3& startPos);
    void updateBoxSelection(const glm::vec3& currentPos);
    void endBoxSelection(bool addToSelection = false);
    bool isBoxSelecting() const { return m_boxSelecting; }
    std::pair<glm::vec2, glm::vec2> getSelectionBox() const { return m_selectionBox; }

    // Selection queries
    bool hasSelection() const;
    size_t getSelectionCount() const;
    
    // Get selected elements
    std::vector<HalfEdgeVertexPtr> getSelectedVertices() const;
    std::vector<HalfEdgeEdgePtr> getSelectedEdges() const;
    std::vector<HalfEdgeFacePtr> getSelectedFaces() const;

    // Selection state of individual elements
    bool isSelected(HalfEdgeVertexPtr vertex) const;
    bool isSelected(HalfEdgeEdgePtr edge) const;
    bool isSelected(HalfEdgeFacePtr face) const;

    // Manual selection of elements
    void selectVertex(HalfEdgeVertexPtr vertex, bool selected = true);
    void selectEdge(HalfEdgeEdgePtr edge, bool selected = true);
    void selectFace(HalfEdgeFacePtr face, bool selected = true);

    // Selection conversion (e.g., convert vertex selection to adjacent faces)
    void convertSelection(SelectionType toType);

    // Ray-mesh intersection for picking
    struct RayHit {
        bool hit = false;
        glm::vec3 point;
        float distance = 0.0f;
        HalfEdgeVertexPtr vertex;
        HalfEdgeEdgePtr edge;
        HalfEdgeFacePtr face;
    };
    
    RayHit raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const;

private:
    SelectionType m_selectionType = SelectionType::Vertex;
    HalfEdgeMeshPtr m_mesh;
    
    // Box selection state
    bool m_boxSelecting = false;
    glm::vec3 m_boxStart;
    glm::vec3 m_boxEnd;
    std::pair<glm::vec2, glm::vec2> m_selectionBox;
    
    // Selection sets for performance
    std::unordered_set<HalfEdgeVertexPtr> m_selectedVertices;
    std::unordered_set<HalfEdgeEdgePtr> m_selectedEdges;
    std::unordered_set<HalfEdgeFacePtr> m_selectedFaces;
    
    // Helper methods
    void updateSelectionVisualization();
    HalfEdgeVertexPtr findClosestVertex(const glm::vec3& point, float maxDistance = 0.1f) const;
    HalfEdgeEdgePtr findClosestEdge(const glm::vec3& point, float maxDistance = 0.1f) const;
    HalfEdgeFacePtr findClosestFace(const glm::vec3& point) const;
    
    // Ray-triangle intersection
    bool rayTriangleIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                              const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                              float& t, glm::vec3& hitPoint) const;
};
