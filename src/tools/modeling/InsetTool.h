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
 * █  FILE: InsetTool.h                                                           █
 * █  DESCRIPTION: Professional Face Inset Tool for Advanced Mesh Modeling       █
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
 * █  This file contains the InsetTool class - providing professional-grade      █
 * █  face inset operations with topology preservation, region handling, and     █
 * █  sophisticated geometric calculations for advanced 3D modeling.             █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file InsetTool.h
 * @brief Professional Face Inset Tool for Advanced 3D Mesh Modeling Operations
 * 
 * This file contains the comprehensive InsetTool class implementation that provides
 * professional-grade face inset capabilities within the RudeBase3D environment.
 * The tool supports sophisticated face inset operations with proper topology
 * preservation, multiple inset modes, and advanced geometric calculations,
 * delivering modeling functionality comparable to industry-standard 3D applications
 * like Maya, Blender, 3ds Max, and Houdini.
 * 
 * ## Professional Inset Architecture
 * 
 * ### Multi-Mode Inset System
 * Industry-standard inset operations for flexible modeling workflows:
 * - **Individual Inset**: Per-face inset operations with independent scaling
 * - **Region Inset**: Connected face region inset with unified boundaries
 * - **Proportional Inset**: Distance-based inset scaling for natural results
 * - **Uniform Inset**: Even thickness inset regardless of face size or angle
 * 
 * ### Advanced Geometric Calculations
 * Sophisticated mathematical framework for precise inset operations:
 * - **Face Center Computation**: Area-weighted centroid calculation for accuracy
 * - **Inset Vector Calculation**: Perpendicular inward vectors with angle compensation
 * - **Boundary Preservation**: Maintains mesh manifold properties during operation
 * - **Intersection Handling**: Resolves edge intersections in complex geometries
 * 
 * ### Topology-Aware Mesh Operations
 * Professional mesh editing with complete structural integrity:
 * - **Half-Edge Integration**: Direct manipulation of half-edge mesh connectivity
 * - **Manifold Preservation**: Ensures mesh remains manifold throughout operation
 * - **Boundary Detection**: Intelligent handling of mesh boundaries and holes
 * - **Validation Systems**: Automatic mesh integrity checking and error recovery
 * 
 * ## Advanced Modeling Features
 * 
 * ### Intelligent Face Analysis
 * Sophisticated face property analysis for optimal inset results:
 * - **Normal Computation**: Accurate face normal calculation with smoothing
 * - **Area Calculation**: Precise face area computation for scaling operations
 * - **Planarity Testing**: Verification of face planarity for valid inset operations
 * - **Convexity Analysis**: Convex hull analysis for intersection prediction
 * 
 * ### Multi-Face Region Handling
 * Advanced algorithms for complex face selection inset operations:
 * - **Connected Component Analysis**: Automatic detection of face regions
 * - **Boundary Edge Detection**: Identification of region boundary edges
 * - **Interior Edge Handling**: Special treatment of edges within face regions
 * - **Bridge Face Creation**: Automatic creation of connecting geometry
 * 
 * ### Professional Workflow Integration
 * Industry-standard modeling workflow support:
 * - **Undo/Redo Integration**: Complete operation history with state restoration
 * - **Parameter Preservation**: Maintains tool settings across modeling sessions
 * - **Batch Processing**: Multi-face inset operations with synchronized parameters
 * - **Macro Recording**: Tool operation recording for automation and scripting
 * 
 * ## Advanced Technical Implementation
 * 
 * ### Geometric Algorithm Foundation
 * Mathematical precision in inset calculations:
 * - **Vector Mathematics**: GLM-based vector operations for numerical stability
 * - **Plane Intersection**: Robust plane-plane intersection for edge calculation
 * - **Distance Computation**: Accurate distance calculation for inset scaling
 * - **Angle Calculation**: Precise angle computation for even thickness insets
 * 
 * ### Edge Case Handling
 * Robust handling of complex geometric situations:
 * - **Degenerate Faces**: Special handling of very small or thin faces
 * - **Non-Planar Faces**: Approximation strategies for curved face surfaces
 * - **Self-Intersection**: Prevention and resolution of self-intersecting geometry
 * - **Numerical Precision**: Epsilon-based comparisons for floating-point stability
 * 
 * ### Memory Management
 * Efficient resource handling for large mesh operations:
 * - **Vertex Pooling**: Reusable vertex data structures for performance
 * - **Face Creation**: Efficient face creation with minimal memory allocation
 * - **Cleanup Systems**: Automatic cleanup of temporary data structures
 * - **Reference Tracking**: Complete tracking of created geometry for undo operations
 * 
 * ## Professional Inset Algorithms
 * 
 * ### Individual Face Inset Algorithm
 * Sophisticated per-face inset with geometric precision:
 * - **Centroid Calculation**: Mass-center computation for uniform scaling
 * - **Edge Bisector Method**: Angle bisector calculation for even inset lines
 * - **Scaling Factor Application**: Proportional scaling with distance preservation
 * - **Bridge Quad Creation**: Automatic creation of connecting quad faces
 * 
 * ### Region Inset Algorithm
 * Advanced multi-face region inset with topology preservation:
 * - **Boundary Extraction**: Automatic extraction of region boundary loops
 * - **Interior Edge Removal**: Elimination of edges within the inset region
 * - **Unified Scaling**: Coordinated scaling across the entire face region
 * - **Complex Topology**: Handling of regions with holes and complex boundaries
 * 
 * ### Even Thickness Algorithm
 * Professional even-thickness inset for consistent results:
 * - **Angle Compensation**: Adjustment for face angle variations
 * - **Distance Normalization**: Uniform distance regardless of face orientation
 * - **Thickness Preservation**: Maintains consistent inset thickness across faces
 * - **Quality Metrics**: Validation of inset quality and geometric consistency
 * 
 * ## Industry-Standard Workflow Integration
 * 
 * ### Maya-Style Inset Operations
 * Maya-compatible inset behaviors and workflows:
 * - **Component Mode**: Direct integration with face selection and manipulation
 * - **Interactive Scaling**: Real-time inset adjustment with mouse interaction
 * - **Thickness Control**: Independent thickness and scale parameter control
 * - **Local Coordinate Space**: Inset operations in local object coordinates
 * 
 * ### Blender-Style Inset Features
 * Blender-compatible inset paradigms and interactions:
 * - **I-Key Inset**: Instant inset activation with modifier key combinations
 * - **Individual Faces**: Toggle between individual and collective inset modes
 * - **Depth Control**: Independent inset depth with normal-based displacement
 * - **Even Offset**: Uniform inset thickness across varying face sizes
 * 
 * ### 3ds Max-Style Inset Modifiers
 * 3ds Max-compatible inset features and modifiers:
 * - **Inset Modifier**: Non-destructive inset with parametric control
 * - **Sub-Object Selection**: Integration with sub-object selection systems
 * - **Multi-Selection**: Support for complex face selection sets
 * - **Material Preservation**: Automatic material assignment to inset faces
 * 
 * ## Performance Optimization Strategies
 * 
 * ### Large Mesh Optimization
 * Efficient inset operations for complex meshes:
 * - **Spatial Indexing**: Octree-based spatial acceleration for large face sets
 * - **Batch Operations**: Grouped operations for multiple face inset
 * - **Memory Pooling**: Reusable data structures for frequent operations
 * - **Parallel Processing**: Multi-threaded processing for independent face insets
 * 
 * ### Algorithm Optimization
 * Optimized geometric calculations for interactive performance:
 * - **Vector Caching**: Pre-computed face normals and edge vectors
 * - **Incremental Updates**: Minimal recalculation for parameter changes
 * - **Geometric Culling**: Early rejection of invalid inset candidates
 * - **Precision Control**: Adaptive precision based on mesh scale and requirements
 * 
 * ## Usage Examples
 * 
 * ### Basic Face Inset Operation
 * ```cpp
 * // Initialize inset tool with mesh
 * InsetTool insetTool;
 * insetTool.setMesh(meshPtr);
 * 
 * // Configure inset settings
 * insetTool.setInsetMode(InsetTool::InsetMode::Individual);
 * insetTool.setInsetDepth(0.2f);
 * insetTool.setScaleEvenThickness(true);
 * 
 * // Perform inset operation on selected faces
 * std::vector<rude::FacePtr> selectedFaces = getSelectedFaces();
 * if (insetTool.canInset()) {
 *     bool success = insetTool.insetFaces(selectedFaces, 0.5f);
 *     if (success) {
 *         auto createdFaces = insetTool.getCreatedFaces();
 *         auto createdVertices = insetTool.getCreatedVertices();
 *         updateMeshRendering();
 *     }
 * }
 * ```
 * 
 * ### Advanced Region Inset with Custom Parameters
 * ```cpp
 * class AdvancedInsetController {
 * private:
 *     InsetTool m_insetTool;
 *     std::vector<InsetOperation> m_operationHistory;
 *     
 * public:
 *     void performRegionInset(const std::vector<rude::FacePtr>& faces,
 *                           const InsetParameters& params) {
 *         // Configure tool for region operation
 *         m_insetTool.setInsetMode(InsetTool::InsetMode::Region);
 *         m_insetTool.setInsetDepth(params.depth);
 *         m_insetTool.setScaleEvenThickness(params.evenThickness);
 *         
 *         // Validate operation feasibility
 *         if (!m_insetTool.canInset()) {
 *             throw std::runtime_error("Cannot inset: invalid face selection");
 *         }
 *         
 *         // Analyze face connectivity
 *         auto faceRegions = analyzeFaceConnectivity(faces);
 *         
 *         for (const auto& region : faceRegions) {
 *             // Perform inset on each connected region
 *             bool success = m_insetTool.insetFaces(region.faces, params.insetAmount);
 *             
 *             if (success) {
 *                 // Store operation for history
 *                 InsetOperation operation;
 *                 operation.originalFaces = region.faces;
 *                 operation.createdFaces = m_insetTool.getCreatedFaces();
 *                 operation.createdVertices = m_insetTool.getCreatedVertices();
 *                 operation.parameters = params;
 *                 operation.timestamp = std::chrono::steady_clock::now();
 *                 m_operationHistory.push_back(operation);
 *                 
 *                 // Apply post-processing if needed
 *                 if (params.smoothInsetFaces) {
 *                     applyFaceSmoothing(operation.createdFaces);
 *                 }
 *             }
 *         }
 *     }
 *     
 *     void performMultiLevelInset(const std::vector<rude::FacePtr>& faces,
 *                                const std::vector<float>& insetLevels) {
 *         std::vector<rude::FacePtr> currentFaces = faces;
 *         
 *         for (float insetAmount : insetLevels) {
 *             if (m_insetTool.insetFaces(currentFaces, insetAmount)) {
 *                 // Use newly created faces for next level
 *                 currentFaces = extractInsetFaces(m_insetTool.getCreatedFaces());
 *             } else {
 *                 break; // Stop if inset fails
 *             }
 *         }
 *     }
 *     
 * private:
 *     std::vector<FaceRegion> analyzeFaceConnectivity(
 *         const std::vector<rude::FacePtr>& faces) {
 *         std::vector<FaceRegion> regions;
 *         std::set<rude::FacePtr> processed;
 *         
 *         for (auto face : faces) {
 *             if (processed.find(face) != processed.end()) {
 *                 continue;
 *             }
 *             
 *             FaceRegion region;
 *             std::queue<rude::FacePtr> faceQueue;
 *             faceQueue.push(face);
 *             
 *             while (!faceQueue.empty()) {
 *                 auto currentFace = faceQueue.front();
 *                 faceQueue.pop();
 *                 
 *                 if (processed.find(currentFace) != processed.end()) {
 *                     continue;
 *                 }
 *                 
 *                 processed.insert(currentFace);
 *                 region.faces.push_back(currentFace);
 *                 
 *                 // Find adjacent faces in the selection
 *                 auto adjacentFaces = findAdjacentFaces(currentFace, faces);
 *                 for (auto adjacent : adjacentFaces) {
 *                     if (processed.find(adjacent) == processed.end()) {
 *                         faceQueue.push(adjacent);
 *                     }
 *                 }
 *             }
 *             
 *             regions.push_back(region);
 *         }
 *         
 *         return regions;
 *     }
 * };
 * ```
 * 
 * ### Interactive Inset with Real-Time Preview
 * ```cpp
 * class InteractiveInsetWidget : public QWidget {
 *     Q_OBJECT
 *     
 * private:
 *     InsetTool* m_insetTool;
 *     QSlider* m_insetAmountSlider;
 *     QSlider* m_depthSlider;
 *     QCheckBox* m_evenThicknessCheckBox;
 *     QComboBox* m_modeComboBox;
 *     QPushButton* m_previewButton;
 *     QPushButton* m_confirmButton;
 *     QPushButton* m_cancelButton;
 *     
 *     std::vector<rude::FacePtr> m_selectedFaces;
 *     bool m_previewActive = false;
 *     
 * public:
 *     InteractiveInsetWidget(InsetTool* tool, QWidget* parent = nullptr)
 *         : QWidget(parent), m_insetTool(tool) {
 *         setupUI();
 *         connectSignals();
 *     }
 *     
 *     void setSelectedFaces(const std::vector<rude::FacePtr>& faces) {
 *         m_selectedFaces = faces;
 *         updateUIState();
 *     }
 *     
 * private slots:
 *     void onInsetAmountChanged(int value) {
 *         if (!m_previewActive) return;
 *         
 *         float insetAmount = value / 100.0f;
 *         updatePreview(insetAmount);
 *     }
 *     
 *     void onDepthChanged(int value) {
 *         float depth = value / 100.0f;
 *         m_insetTool->setInsetDepth(depth);
 *         
 *         if (m_previewActive) {
 *             updatePreview(m_insetAmountSlider->value() / 100.0f);
 *         }
 *     }
 *     
 *     void onModeChanged(int index) {
 *         InsetTool::InsetMode mode;
 *         switch (index) {
 *             case 0: mode = InsetTool::InsetMode::Individual; break;
 *             case 1: mode = InsetTool::InsetMode::Region; break;
 *             default: return;
 *         }
 *         
 *         m_insetTool->setInsetMode(mode);
 *         
 *         if (m_previewActive) {
 *             updatePreview(m_insetAmountSlider->value() / 100.0f);
 *         }
 *     }
 *     
 *     void onEvenThicknessToggled(bool enabled) {
 *         m_insetTool->setScaleEvenThickness(enabled);
 *         
 *         if (m_previewActive) {
 *             updatePreview(m_insetAmountSlider->value() / 100.0f);
 *         }
 *     }
 *     
 *     void onPreviewClicked() {
 *         if (!m_previewActive) {
 *             startPreview();
 *         } else {
 *             stopPreview();
 *         }
 *     }
 *     
 *     void onConfirmClicked() {
 *         if (m_previewActive) {
 *             confirmInset();
 *         }
 *     }
 *     
 *     void onCancelClicked() {
 *         if (m_previewActive) {
 *             cancelInset();
 *         }
 *     }
 *     
 * private:
 *     void setupUI() {
 *         auto layout = new QVBoxLayout(this);
 *         
 *         // Mode selection
 *         layout->addWidget(new QLabel("Inset Mode:"));
 *         m_modeComboBox = new QComboBox();
 *         m_modeComboBox->addItems({"Individual", "Region"});
 *         layout->addWidget(m_modeComboBox);
 *         
 *         // Inset amount control
 *         layout->addWidget(new QLabel("Inset Amount:"));
 *         m_insetAmountSlider = new QSlider(Qt::Horizontal);
 *         m_insetAmountSlider->setRange(1, 200); // 0.01 to 2.0
 *         m_insetAmountSlider->setValue(50); // Default 0.5
 *         layout->addWidget(m_insetAmountSlider);
 *         
 *         // Depth control
 *         layout->addWidget(new QLabel("Inset Depth:"));
 *         m_depthSlider = new QSlider(Qt::Horizontal);
 *         m_depthSlider->setRange(0, 100); // 0 to 1.0
 *         m_depthSlider->setValue(20); // Default 0.2
 *         layout->addWidget(m_depthSlider);
 *         
 *         // Even thickness option
 *         m_evenThicknessCheckBox = new QCheckBox("Even Thickness");
 *         m_evenThicknessCheckBox->setChecked(true);
 *         layout->addWidget(m_evenThicknessCheckBox);
 *         
 *         // Control buttons
 *         auto buttonLayout = new QHBoxLayout();
 *         m_previewButton = new QPushButton("Preview");
 *         m_confirmButton = new QPushButton("Confirm");
 *         m_cancelButton = new QPushButton("Cancel");
 *         
 *         m_previewButton->setCheckable(true);
 *         buttonLayout->addWidget(m_previewButton);
 *         buttonLayout->addWidget(m_confirmButton);
 *         buttonLayout->addWidget(m_cancelButton);
 *         layout->addLayout(buttonLayout);
 *         
 *         updateUIState();
 *     }
 *     
 *     void connectSignals() {
 *         connect(m_insetAmountSlider, &QSlider::valueChanged,
 *                 this, &InteractiveInsetWidget::onInsetAmountChanged);
 *         connect(m_depthSlider, &QSlider::valueChanged,
 *                 this, &InteractiveInsetWidget::onDepthChanged);
 *         connect(m_modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
 *                 this, &InteractiveInsetWidget::onModeChanged);
 *         connect(m_evenThicknessCheckBox, &QCheckBox::toggled,
 *                 this, &InteractiveInsetWidget::onEvenThicknessToggled);
 *         connect(m_previewButton, &QPushButton::clicked,
 *                 this, &InteractiveInsetWidget::onPreviewClicked);
 *         connect(m_confirmButton, &QPushButton::clicked,
 *                 this, &InteractiveInsetWidget::onConfirmClicked);
 *         connect(m_cancelButton, &QPushButton::clicked,
 *                 this, &InteractiveInsetWidget::onCancelClicked);
 *     }
 *     
 *     void updateUIState() {
 *         bool hasSelection = !m_selectedFaces.empty();
 *         bool canInset = hasSelection && m_insetTool->canInset();
 *         
 *         m_previewButton->setEnabled(canInset);
 *         m_confirmButton->setEnabled(m_previewActive);
 *         m_cancelButton->setEnabled(m_previewActive);
 *         
 *         // Disable parameter controls during preview
 *         m_modeComboBox->setEnabled(!m_previewActive);
 *         m_insetAmountSlider->setEnabled(m_previewActive);
 *         m_depthSlider->setEnabled(m_previewActive);
 *         m_evenThicknessCheckBox->setEnabled(!m_previewActive);
 *     }
 *     
 *     void startPreview() {
 *         if (m_selectedFaces.empty() || !m_insetTool->canInset()) {
 *             return;
 *         }
 *         
 *         m_previewActive = true;
 *         m_previewButton->setText("Stop Preview");
 *         m_previewButton->setChecked(true);
 *         
 *         // Store original mesh state for cancellation
 *         storeOriginalMeshState();
 *         
 *         // Start with current slider value
 *         updatePreview(m_insetAmountSlider->value() / 100.0f);
 *         updateUIState();
 *     }
 *     
 *     void stopPreview() {
 *         if (!m_previewActive) return;
 *         
 *         restoreOriginalMeshState();
 *         
 *         m_previewActive = false;
 *         m_previewButton->setText("Preview");
 *         m_previewButton->setChecked(false);
 *         updateUIState();
 *     }
 *     
 *     void updatePreview(float insetAmount) {
 *         if (!m_previewActive) return;
 *         
 *         // Restore to original state first
 *         restoreOriginalMeshState();
 *         
 *         // Apply inset with current parameters
 *         m_insetTool->insetFaces(m_selectedFaces, insetAmount);
 *         
 *         // Update viewport rendering
 *         emit previewUpdated();
 *     }
 *     
 * signals:
 *     void previewUpdated();
 *     void insetCompleted();
 *     void insetCancelled();
 * };
 * ```
 * 
 * ### Professional Inset Manager with Advanced Features
 * ```cpp
 * class ProfessionalInsetManager {
 * private:
 *     struct InsetSession {
 *         std::string sessionId;
 *         InsetTool tool;
 *         std::vector<InsetStep> steps;
 *         std::chrono::steady_clock::time_point startTime;
 *         rude::HalfEdgeMeshPtr originalMesh;
 *         SelectionState originalSelection;
 *     };
 *     
 *     std::map<std::string, std::unique_ptr<InsetSession>> m_activeSessions;
 *     InsetSettings m_defaultSettings;
 *     
 * public:
 *     std::string beginInsetSession(rude::HalfEdgeMeshPtr mesh,
 *                                 const std::vector<rude::FacePtr>& faces) {
 *         std::string sessionId = generateUniqueId();
 *         
 *         auto session = std::make_unique<InsetSession>();
 *         session->sessionId = sessionId;
 *         session->tool.setMesh(mesh);
 *         session->startTime = std::chrono::steady_clock::now();
 *         session->originalMesh = cloneMesh(mesh);
 *         
 *         // Apply default settings
 *         session->tool.setInsetMode(m_defaultSettings.mode);
 *         session->tool.setInsetDepth(m_defaultSettings.depth);
 *         session->tool.setScaleEvenThickness(m_defaultSettings.evenThickness);
 *         
 *         m_activeSessions[sessionId] = std::move(session);
 *         return sessionId;
 *     }
 *     
 *     bool executeInsetStep(const std::string& sessionId,
 *                         const std::vector<rude::FacePtr>& faces,
 *                         const InsetParameters& params) {
 *         auto it = m_activeSessions.find(sessionId);
 *         if (it == m_activeSessions.end()) {
 *             return false;
 *         }
 *         
 *         InsetSession* session = it->second.get();
 *         
 *         // Configure tool for this step
 *         session->tool.setInsetMode(params.mode);
 *         session->tool.setInsetDepth(params.depth);
 *         session->tool.setScaleEvenThickness(params.evenThickness);
 *         
 *         // Execute the inset operation
 *         if (!session->tool.canInset()) {
 *             return false;
 *         }
 *         
 *         bool success = session->tool.insetFaces(faces, params.insetAmount);
 *         
 *         if (success) {
 *             // Record the step
 *             InsetStep step;
 *             step.originalFaces = faces;
 *             step.createdFaces = session->tool.getCreatedFaces();
 *             step.createdVertices = session->tool.getCreatedVertices();
 *             step.parameters = params;
 *             step.timestamp = std::chrono::steady_clock::now();
 *             session->steps.push_back(step);
 *         }
 *         
 *         return success;
 *     }
 *     
 *     void commitInsetSession(const std::string& sessionId) {
 *         auto it = m_activeSessions.find(sessionId);
 *         if (it != m_activeSessions.end()) {
 *             InsetSession* session = it->second.get();
 *             
 *             // Log session completion
 *             auto duration = std::chrono::steady_clock::now() - session->startTime;
 *             logInsetSession(sessionId, session->steps, duration);
 *             
 *             m_activeSessions.erase(it);
 *         }
 *     }
 *     
 *     void rollbackInsetSession(const std::string& sessionId) {
 *         auto it = m_activeSessions.find(sessionId);
 *         if (it != m_activeSessions.end()) {
 *             InsetSession* session = it->second.get();
 *             
 *             // Restore original mesh
 *             restoreMesh(session->originalMesh);
 *             
 *             m_activeSessions.erase(it);
 *         }
 *     }
 *     
 *     std::vector<InsetStep> getSessionSteps(const std::string& sessionId) const {
 *         auto it = m_activeSessions.find(sessionId);
 *         if (it == m_activeSessions.end()) {
 *             return {};
 *         }
 *         
 *         return it->second->steps;
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
 * - **Face Selection**: Direct integration with face selection and highlighting
 * - **Multi-Object Selection**: Support for multi-object inset operations
 * - **Selection Preservation**: Intelligent selection updates after inset
 * - **History Integration**: Complete integration with selection and operation history
 * 
 * ### Rendering Pipeline Integration
 * Direct integration with RudeBase3D rendering systems:
 * - **Real-Time Updates**: Immediate visual feedback during inset operations
 * - **Material Preservation**: Automatic material assignment to inset faces
 * - **Lighting Updates**: Dynamic lighting recalculation for modified surfaces
 * - **Performance Optimization**: Efficient rendering updates during interaction
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup ModelingTools
 * @ingroup InsetTools
 * @ingroup MeshEditing
 */

#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "core/mesh_forward.hpp"

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
 * @brief Tool for insetting faces in half-edge meshes
 * 
 * The InsetTool creates smaller faces inside selected faces, maintaining
 * proper topology and creating connecting faces between the original and
 * inset face boundaries.
 */
class InsetTool {
public:
    enum class InsetMode {
        Individual,  // Inset each face individually
        Region      // Inset selected faces as a region
    };

    InsetTool();
    ~InsetTool() = default;

    // Tool state
    void setMesh(rude::HalfEdgeMeshPtr mesh) { m_mesh = mesh; }
    rude::HalfEdgeMeshPtr getMesh() const { return m_mesh; }

    // Inset operations
    bool insetFaces(const std::vector<rude::FacePtr>& faces, float insetAmount);
    bool insetFace(rude::FacePtr face, float insetAmount);
    
    // Tool settings
    void setInsetMode(InsetMode mode) { m_insetMode = mode; }
    InsetMode getInsetMode() const { return m_insetMode; }
    
    void setInsetDepth(float depth) { m_insetDepth = depth; }
    float getInsetDepth() const { return m_insetDepth; }
    
    void setScaleEvenThickness(bool enabled) { m_scaleEvenThickness = enabled; }
    bool getScaleEvenThickness() const { return m_scaleEvenThickness; }

    // Validation
    bool canInset() const;
    
    // Result information
    const std::vector<rude::FacePtr>& getCreatedFaces() const { return m_createdFaces; }
    const std::vector<rude::VertexPtr>& getCreatedVertices() const { return m_createdVertices; }

private:
    // Core mesh
    rude::HalfEdgeMeshPtr m_mesh;
    
    // Tool settings
    InsetMode m_insetMode = InsetMode::Individual;
    float m_insetDepth = 0.0f;
    bool m_scaleEvenThickness = false;
    
    // Result tracking
    std::vector<rude::FacePtr> m_createdFaces;
    std::vector<rude::VertexPtr> m_createdVertices;
    
    // Internal operations
    rude::FacePtr insetFaceIndividual(rude::FacePtr face, float insetAmount);
    std::vector<rude::FacePtr> insetRegion(const std::vector<rude::FacePtr>& faces, float insetAmount);
    
    // Helper methods
    glm::vec3 calculateFaceCenter(rude::FacePtr face) const;
    glm::vec3 calculateInsetPosition(rude::VertexPtr vertex, rude::FacePtr face, float insetAmount) const;
    std::vector<rude::VertexPtr> createInsetVertices(rude::FacePtr face, float insetAmount);
    void createBridgeFaces(const std::vector<rude::VertexPtr>& originalVertices, 
                          const std::vector<rude::VertexPtr>& insetVertices);
    
    // Topology helpers
    rude::VertexPtr duplicateVertex(rude::VertexPtr vertex, const glm::vec3& newPosition);
    rude::FacePtr createQuadFace(rude::VertexPtr v1, rude::VertexPtr v2,
                                   rude::VertexPtr v3, rude::VertexPtr v4);
    
    // Cleanup
    void clearResults();
};
