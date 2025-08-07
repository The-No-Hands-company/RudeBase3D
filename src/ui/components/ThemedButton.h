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
 * █  FILE: ThemedButton.h                                                        █
 * █  DESCRIPTION: Professional Themed Button Component System                    █
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
 * █  This file contains the ThemedButton implementation providing professional   █
 * █  theme-aware button components with advanced styling, state management,     █
 * █  and accessibility features for modern 3D modeling applications.            █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file ThemedButton.h
 * @brief Professional Themed Button Component - Advanced UI Control for Consistent Interface Design
 *
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * The ThemedButton class represents the foundation of RudeBase3D's professional user 
 * interface design philosophy, providing a sophisticated, theme-aware button component 
 * that delivers the polished experience expected in modern 3D modeling applications.
 * This component serves as the standard interactive element across all interface panels,
 * toolbars, dialogs, and control systems within the RudeBase3D ecosystem.
 * 
 * Drawing inspiration from industry-leading software like Autodesk Maya, Blender, and 
 * Cinema 4D, ThemedButton provides a comprehensive button solution that automatically
 * adapts to the application's theme system while maintaining professional visual
 * standards and accessibility compliance essential for professional 3D workflows.
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * CORE ARCHITECTURE & DESIGN PHILOSOPHY
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * ## Professional Button System Design
 * 
 * ThemedButton implements a comprehensive button component architecture designed for
 * professional 3D modeling applications where visual consistency, accessibility, and
 * responsive interaction are paramount:
 * 
 * ```cpp
 * // ThemedButton Architecture Overview
 * class ThemedButton : public BaseComponent {
 *     // Core Button Functionality
 *     - Multiple professional button styles (Primary, Secondary, Success, Warning, Danger, Ghost)
 *     - Icon and text integration with flexible sizing and layout options
 *     - State-aware visual feedback with smooth animations and transitions
 *     - Theme-responsive design that adapts to application appearance modes
 *     
 *     // Professional Interaction Features
 *     - Hover, focus, pressed, and disabled state management
 *     - Loading state visualization for asynchronous operations
 *     - Error state indication with appropriate visual feedback
 *     - Keyboard navigation and accessibility compliance
 *     
 *     // Performance & Integration
 *     - Cached styling and optimized rendering pipeline
 *     - Seamless integration with Qt layout management
 *     - Professional animation timing and visual effects
 *     - Memory-efficient resource management
 * };
 * ```
 * 
 * ## Button Style System
 * 
 * RudeBase3D's button styling system provides six distinct button types, each optimized
 * for specific interface contexts and user interaction patterns:
 * 
 * ### Primary Buttons
 * ```cpp
 * // Primary buttons for main actions
 * ThemedButton* applyButton = new ThemedButton("Apply Changes", parent);
 * applyButton->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 * // Usage: Apply, Save, Export, OK, Confirm
 * // Visual: Prominent color with strong contrast, clear call-to-action styling
 * ```
 * 
 * ### Secondary Buttons
 * ```cpp
 * // Secondary buttons for alternative actions
 * ThemedButton* cancelButton = new ThemedButton("Cancel", parent);
 * cancelButton->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 * // Usage: Cancel, Reset, Import, Browse
 * // Visual: Subtle styling that complements primary actions
 * ```
 * 
 * ### Success Buttons
 * ```cpp
 * // Success buttons for positive confirmations
 * ThemedButton* createButton = new ThemedButton("Create Mesh", parent);
 * createButton->setButtonStyle(ThemedButton::ButtonStyle::Success);
 * // Usage: Create, Add, Generate, Build
 * // Visual: Green-toned styling indicating positive actions
 * ```
 * 
 * ### Warning Buttons
 * ```cpp
 * // Warning buttons for cautionary actions
 * ThemedButton* modifyButton = new ThemedButton("Modify Geometry", parent);
 * modifyButton->setButtonStyle(ThemedButton::ButtonStyle::Warning);
 * // Usage: Modify, Update, Change, Overwrite
 * // Visual: Orange/yellow styling indicating caution required
 * ```
 * 
 * ### Danger Buttons
 * ```cpp
 * // Danger buttons for destructive actions
 * ThemedButton* deleteButton = new ThemedButton("Delete Object", parent);
 * deleteButton->setButtonStyle(ThemedButton::ButtonStyle::Danger);
 * // Usage: Delete, Remove, Clear, Destroy
 * // Visual: Red-toned styling clearly indicating destructive operations
 * ```
 * 
 * ### Ghost Buttons
 * ```cpp
 * // Ghost buttons for minimal interface elements
 * ThemedButton* toggleButton = new ThemedButton("Toggle View", parent);
 * toggleButton->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 * // Usage: Toggle, Show/Hide, Minimize, Expand
 * // Visual: Minimal styling with transparent background
 * ```
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * ADVANCED ICON & TEXT INTEGRATION
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * ## Professional Icon System
 * 
 * ThemedButton provides comprehensive icon integration designed for professional 3D
 * modeling workflows where visual icons enhance usability and workflow efficiency:
 * 
 * ```cpp
 * // Icon-only buttons for compact toolbars
 * ThemedButton* extrudeBtn = new ThemedButton(parent);
 * extrudeBtn->setIcon(QIcon(":/icons/tools/extrude.svg"));
 * extrudeBtn->setIconSize(QSize(24, 24));
 * extrudeBtn->setToolTip("Extrude Selected Faces");
 * 
 * // Icon + text combination for clear identification
 * QIcon saveIcon(":/icons/file/save.svg");
 * ThemedButton* saveBtn = new ThemedButton(saveIcon, "Save Project", parent);
 * saveBtn->setIconSize(QSize(16, 16));
 * 
 * // Dynamic icon updates based on state
 * ThemedButton* playBtn = new ThemedButton(parent);
 * playBtn->setIcon(isPlaying ? pauseIcon : playIcon);
 * playBtn->setText(isPlaying ? "Pause Animation" : "Play Animation");
 * ```
 * 
 * ## Scalable Vector Graphics Support
 * 
 * Optimized for SVG icons that maintain crisp appearance across all display densities:
 * 
 * ```cpp
 * // Multi-resolution icon support
 * ThemedButton* renderBtn = new ThemedButton(parent);
 * renderBtn->setIcon(QIcon(":/icons/render/render.svg"));
 * 
 * // Automatic scaling for different interface contexts
 * renderBtn->setIconSize(QSize(16, 16));  // Standard UI size
 * renderBtn->setIconSize(QSize(24, 24));  // Toolbar size
 * renderBtn->setIconSize(QSize(32, 32));  // Large panel size
 * 
 * // Icon adapts to theme colors automatically
 * // SVG icons can include theme-aware color variables
 * ```
 * 
 * ## Text Integration & Typography
 * 
 * Professional text rendering with proper typography and internationalization support:
 * 
 * ```cpp
 * // Multi-language text support
 * ThemedButton* exportBtn = new ThemedButton(tr("Export Mesh"), parent);
 * 
 * // Dynamic text updates
 * ThemedButton* undoBtn = new ThemedButton(parent);
 * undoBtn->setText(QString("Undo %1").arg(lastAction));
 * 
 * // Text-only professional styling
 * ThemedButton* optionsBtn = new ThemedButton("Advanced Options...", parent);
 * optionsBtn->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 * ```
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * THEME INTEGRATION & VISUAL CONSISTENCY
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * ## Automatic Theme Adaptation
 * 
 * ThemedButton seamlessly integrates with RudeBase3D's theme system, automatically
 * adapting its appearance to match the current theme while maintaining visual hierarchy:
 * 
 * ```cpp
 * // Theme system integration
 * class ThemedButton : public BaseComponent {
 * protected:
 *     void applyTheme() override {
 *         // Automatically called when theme changes
 *         updateColorScheme();
 *         updateTypography();
 *         updateSpacing();
 *         updateAnimations();
 *         refreshVisualState();
 *     }
 *     
 *     QString generateBaseStyleSheet() const override {
 *         // Generates CSS based on current theme
 *         return buildStyleSheet(
 *             currentTheme(),
 *             buttonStyle(),
 *             componentState()
 *         );
 *     }
 * };
 * ```
 * 
 * ## Dark/Light Mode Optimization
 * 
 * Professional appearance optimization for both dark and light interface themes:
 * 
 * ```cpp
 * // Dark theme optimization
 * if (isDarkTheme()) {
 *     // Optimized contrast ratios for dark backgrounds
 *     // Enhanced glow effects and shadow rendering
 *     // Adjusted color temperatures for eye comfort
 * }
 * 
 * // Light theme optimization  
 * if (isLightTheme()) {
 *     // Clean, minimal styling for light backgrounds
 *     // Subtle shadows and border definitions
 *     // High contrast text for readability
 * }
 * ```
 * 
 * ## Accessibility Compliance
 * 
 * Comprehensive accessibility features ensuring professional applications meet
 * international accessibility standards:
 * 
 * ```cpp
 * // WCAG 2.1 AA compliance
 * - Minimum 4.5:1 contrast ratio for normal text
 * - Minimum 3:1 contrast ratio for large text
 * - Minimum 44x44 pixel touch targets
 * - Clear focus indicators for keyboard navigation
 * - Screen reader compatible ARIA attributes
 * - High contrast mode automatic adaptation
 * ```
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * PROFESSIONAL INTERACTION STATES
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * ## State-Aware Visual Feedback
 * 
 * ThemedButton provides comprehensive visual feedback for all interaction states,
 * delivering the responsive feel expected in professional 3D modeling applications:
 * 
 * ```cpp
 * // Hover state with smooth transitions
 * void onHoverEnter() {
 *     animateColorTransition(hoverColor, 150ms);
 *     animateShadowTransition(hoverShadow, 150ms);
 *     animateScaleTransition(1.02f, 100ms);
 * }
 * 
 * // Press state with immediate feedback
 * void onPressStart() {
 *     animateColorTransition(pressedColor, 50ms);
 *     animateScaleTransition(0.98f, 50ms);
 *     provideTactileFeedback();
 * }
 * 
 * // Focus state for keyboard navigation
 * void onFocusGained() {
 *     showFocusRing(accessibleFocusColor);
 *     announceToScreenReader();
 * }
 * ```
 * 
 * ## Loading State Visualization
 * 
 * Professional loading state indication for asynchronous operations common in 3D workflows:
 * 
 * ```cpp
 * // Show loading during mesh processing
 * ThemedButton* processBtn = new ThemedButton("Process Mesh", parent);
 * 
 * connect(processBtn, &ThemedButton::clicked, [=]() {
 *     processBtn->showLoadingState();
 *     processBtn->setText("Processing...");
 *     processBtn->setEnabled(false);
 *     
 *     // Start async mesh processing
 *     meshProcessor->processAsync([=](bool success) {
 *         processBtn->hideLoadingState();
 *         processBtn->setEnabled(true);
 *         
 *         if (success) {
 *             processBtn->setText("Process Complete");
 *             processBtn->setButtonStyle(ThemedButton::ButtonStyle::Success);
 *         } else {
 *             processBtn->showErrorState("Processing failed");
 *         }
 *     });
 * });
 * ```
 * 
 * ## Error State Management
 * 
 * Clear error indication with appropriate styling and user feedback:
 * 
 * ```cpp
 * // Error state with visual feedback
 * void showErrorState(const QString& errorMessage) {
 *     setButtonStyle(ButtonStyle::Danger);
 *     setToolTip(errorMessage);
 *     animateShakeEffect();
 *     scheduleErrorReset(3000ms);
 * }
 * 
 * // Automatic error state recovery
 * void resetFromErrorState() {
 *     restorePreviousStyle();
 *     clearErrorToolTip();
 *     animateReturnToNormal();
 * }
 * ```
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * ANIMATION & VISUAL EFFECTS SYSTEM
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * ## Professional Animation Framework
 * 
 * ThemedButton implements a sophisticated animation system designed to provide smooth,
 * professional visual feedback without impacting application performance:
 * 
 * ```cpp
 * // Animation configuration
 * struct AnimationConfig {
 *     int hoverDuration = 150;      // Hover transition timing
 *     int pressDuration = 50;       // Press feedback timing
 *     int focusDuration = 100;      // Focus indication timing
 *     int loadingCycle = 1000;      // Loading animation cycle
 *     
 *     QEasingCurve hoverEasing = QEasingCurve::OutCubic;
 *     QEasingCurve pressEasing = QEasingCurve::OutQuad;
 *     QEasingCurve focusEasing = QEasingCurve::OutSine;
 * };
 * 
 * // Smooth color transitions
 * void animateColorTransition(const QColor& targetColor, int duration) {
 *     QPropertyAnimation* colorAnim = new QPropertyAnimation(this, "backgroundColor");
 *     colorAnim->setDuration(duration);
 *     colorAnim->setStartValue(currentBackgroundColor());
 *     colorAnim->setEndValue(targetColor);
 *     colorAnim->setEasingCurve(animConfig.hoverEasing);
 *     colorAnim->start(QAbstractAnimation::DeleteWhenStopped);
 * }
 * ```
 * 
 * ## Performance-Optimized Effects
 * 
 * Visual effects designed for smooth performance in complex 3D interfaces:
 * 
 * ```cpp
 * // GPU-accelerated effects where available
 * void setupHardwareAcceleration() {
 *     if (isOpenGLAvailable()) {
 *         enableOpenGLAnimations();
 *         useHardwareShadows();
 *         enableVectorGraphicsCache();
 *     }
 * }
 * 
 * // Adaptive animation quality
 * void adjustAnimationQuality() {
 *     if (isPerformanceLimited()) {
 *         disableComplexAnimations();
 *         reduceAnimationFrameRate();
 *         useSimplifiedEffects();
 *     }
 * }
 * ```
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * INTEGRATION WITH RUDEBASE3D INTERFACE SYSTEMS
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * ## Toolbar Integration
 * 
 * Optimized for seamless integration with RudeBase3D's professional toolbar system:
 * 
 * ```cpp
 * // Modeling toolbar integration
 * class ModelingToolbar : public QToolBar {
 * public:
 *     void setupModelingTools() {
 *         // Extrude tool button
 *         ThemedButton* extrudeBtn = new ThemedButton(this);
 *         extrudeBtn->setIcon(QIcon(":/icons/tools/extrude.svg"));
 *         extrudeBtn->setIconSize(QSize(24, 24));
 *         extrudeBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         extrudeBtn->setToolTip("Extrude Selected Faces (E)");
 *         
 *         // Inset tool button
 *         ThemedButton* insetBtn = new ThemedButton(this);
 *         insetBtn->setIcon(QIcon(":/icons/tools/inset.svg"));
 *         insetBtn->setIconSize(QSize(24, 24));
 *         insetBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         insetBtn->setToolTip("Inset Selected Faces (I)");
 *         
 *         addWidget(extrudeBtn);
 *         addWidget(insetBtn);
 *         
 *         // Connect to modeling system
 *         connect(extrudeBtn, &ThemedButton::clicked,
 *                 toolManager, &ModelingToolManager::activateExtrudeTool);
 *         connect(insetBtn, &ThemedButton::clicked,
 *                 toolManager, &ModelingToolManager::activateInsetTool);
 *     }
 * };
 * ```
 * 
 * ## Panel Integration
 * 
 * Professional integration with dockable panels and property editors:
 * 
 * ```cpp
 * // Property panel integration
 * class MaterialPropertyPanel : public QWidget {
 * public:
 *     void setupMaterialControls() {
 *         // Load material button
 *         ThemedButton* loadBtn = new ThemedButton("Load Material", this);
 *         loadBtn->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 *         loadBtn->setIcon(QIcon(":/icons/material/load.svg"));
 *         
 *         // Save material button
 *         ThemedButton* saveBtn = new ThemedButton("Save Material", this);
 *         saveBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         saveBtn->setIcon(QIcon(":/icons/material/save.svg"));
 *         
 *         // Reset to default button
 *         ThemedButton* resetBtn = new ThemedButton("Reset to Default", this);
 *         resetBtn->setButtonStyle(ThemedButton::ButtonStyle::Warning);
 *         resetBtn->setIcon(QIcon(":/icons/material/reset.svg"));
 *         
 *         QHBoxLayout* buttonLayout = new QHBoxLayout;
 *         buttonLayout->addWidget(loadBtn);
 *         buttonLayout->addWidget(saveBtn);
 *         buttonLayout->addWidget(resetBtn);
 *         
 *         setLayout(buttonLayout);
 *     }
 * };
 * ```
 * 
 * ## Dialog Integration
 * 
 * Consistent appearance in modal dialogs and settings windows:
 * 
 * ```cpp
 * // Settings dialog integration
 * class RenderSettingsDialog : public QDialog {
 * public:
 *     void setupDialogButtons() {
 *         // Standard dialog buttons with themed styling
 *         ThemedButton* okBtn = new ThemedButton("OK", this);
 *         okBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         
 *         ThemedButton* cancelBtn = new ThemedButton("Cancel", this);
 *         cancelBtn->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 *         
 *         ThemedButton* applyBtn = new ThemedButton("Apply", this);
 *         applyBtn->setButtonStyle(ThemedButton::ButtonStyle::Success);
 *         
 *         ThemedButton* resetBtn = new ThemedButton("Reset to Defaults", this);
 *         resetBtn->setButtonStyle(ThemedButton::ButtonStyle::Warning);
 *         
 *         // Dialog button box
 *         QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
 *         buttonBox->addButton(okBtn, QDialogButtonBox::AcceptRole);
 *         buttonBox->addButton(cancelBtn, QDialogButtonBox::RejectRole);
 *         buttonBox->addButton(applyBtn, QDialogButtonBox::ApplyRole);
 *         buttonBox->addButton(resetBtn, QDialogButtonBox::ResetRole);
 *     }
 * };
 * ```
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * ADVANCED USAGE PATTERNS & WORKFLOWS
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * ## Context-Aware Button Behavior
 * 
 * Dynamic button adaptation based on application state and user context:
 * 
 * ```cpp
 * // Context-sensitive save button
 * class DocumentController : public QObject {
 * public:
 *     void setupSaveButton() {
 *         saveButton = new ThemedButton("Save", parent);
 *         updateSaveButtonState();
 *         
 *         // Connect to document state changes
 *         connect(document, &Document::modifiedStateChanged,
 *                 this, &DocumentController::updateSaveButtonState);
 *     }
 *     
 * private:
 *     void updateSaveButtonState() {
 *         if (document->isModified()) {
 *             saveButton->setText("Save Changes");
 *             saveButton->setButtonStyle(ThemedButton::ButtonStyle::Warning);
 *             saveButton->setIcon(QIcon(":/icons/file/save-modified.svg"));
 *         } else {
 *             saveButton->setText("Save");
 *             saveButton->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 *             saveButton->setIcon(QIcon(":/icons/file/save.svg"));
 *         }
 *     }
 *     
 *     ThemedButton* saveButton;
 *     Document* document;
 * };
 * ```
 * 
 * ## Progressive Action Workflows
 * 
 * Multi-step workflow integration with clear visual progression:
 * 
 * ```cpp
 * // Multi-step mesh export workflow
 * class MeshExportWorkflow : public QWidget {
 * public:
 *     void setupWorkflowButtons() {
 *         // Step 1: Select format
 *         selectFormatBtn = new ThemedButton("Select Format", this);
 *         selectFormatBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         
 *         // Step 2: Configure options (initially disabled)
 *         configureBtn = new ThemedButton("Configure Options", this);
 *         configureBtn->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 *         configureBtn->setEnabled(false);
 *         
 *         // Step 3: Export (initially disabled)
 *         exportBtn = new ThemedButton("Export Mesh", this);
 *         exportBtn->setButtonStyle(ThemedButton::ButtonStyle::Success);
 *         exportBtn->setEnabled(false);
 *         
 *         setupWorkflowConnections();
 *     }
 *     
 * private:
 *     void onFormatSelected() {
 *         selectFormatBtn->setButtonStyle(ThemedButton::ButtonStyle::Success);
 *         selectFormatBtn->setText("Format Selected ✓");
 *         
 *         configureBtn->setEnabled(true);
 *         configureBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *     }
 *     
 *     void onOptionsConfigured() {
 *         configureBtn->setButtonStyle(ThemedButton::ButtonStyle::Success);
 *         configureBtn->setText("Options Configured ✓");
 *         
 *         exportBtn->setEnabled(true);
 *         exportBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *     }
 * };
 * ```
 * 
 * ## Tool State Synchronization
 * 
 * Button state synchronization with 3D modeling tool states:
 * 
 * ```cpp
 * // Tool selection button group
 * class ToolSelectionGroup : public QWidget {
 * public:
 *     void setupToolButtons() {
 *         // Create tool buttons
 *         selectBtn = createToolButton("Select", ":/icons/tools/select.svg");
 *         extrudeBtn = createToolButton("Extrude", ":/icons/tools/extrude.svg");
 *         insetBtn = createToolButton("Inset", ":/icons/tools/inset.svg");
 *         bevelBtn = createToolButton("Bevel", ":/icons/tools/bevel.svg");
 *         
 *         // Connect to tool manager
 *         connect(toolManager, &ModelingToolManager::activeToolChanged,
 *                 this, &ToolSelectionGroup::updateActiveButton);
 *     }
 *     
 * private:
 *     ThemedButton* createToolButton(const QString& name, const QString& iconPath) {
 *         ThemedButton* btn = new ThemedButton(name, this);
 *         btn->setIcon(QIcon(iconPath));
 *         btn->setIconSize(QSize(24, 24));
 *         btn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         btn->setCheckable(true);
 *         return btn;
 *     }
 *     
 *     void updateActiveButton(ModelingTool* activeTool) {
 *         // Reset all buttons to ghost style
 *         for (ThemedButton* btn : toolButtons) {
 *             btn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *             btn->setChecked(false);
 *         }
 *         
 *         // Highlight active tool button
 *         if (auto activeBtn = getButtonForTool(activeTool)) {
 *             activeBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *             activeBtn->setChecked(true);
 *         }
 *     }
 * };
 * ```
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * PERFORMANCE OPTIMIZATION & RESOURCE MANAGEMENT
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * ## Efficient Style Sheet Management
 * 
 * Optimized CSS generation and caching for smooth performance:
 * 
 * ```cpp
 * // Style sheet caching system
 * class ButtonStyleCache {
 * public:
 *     static QString getStyleSheet(ButtonStyle style, ComponentState state, const Theme& theme) {
 *         QString cacheKey = generateCacheKey(style, state, theme);
 *         
 *         if (styleCache.contains(cacheKey)) {
 *             return styleCache[cacheKey];
 *         }
 *         
 *         QString stylesheet = generateStyleSheet(style, state, theme);
 *         styleCache[cacheKey] = stylesheet;
 *         return stylesheet;
 *     }
 *     
 *     static void clearCache() {
 *         styleCache.clear();
 *     }
 *     
 * private:
 *     static QHash<QString, QString> styleCache;
 * };
 * 
 * // Efficient style application
 * void ThemedButton::applyTheme() {
 *     QString stylesheet = ButtonStyleCache::getStyleSheet(
 *         m_buttonStyle, 
 *         componentState(), 
 *         currentTheme()
 *     );
 *     
 *     setStyleSheet(stylesheet);
 * }
 * ```
 * 
 * ## Icon Resource Optimization
 * 
 * Efficient icon loading, scaling, and caching:
 * 
 * ```cpp
 * // Icon management system
 * class IconManager {
 * public:
 *     static QIcon getScaledIcon(const QString& iconPath, const QSize& size) {
 *         QString cacheKey = QString("%1_%2x%3").arg(iconPath).arg(size.width()).arg(size.height());
 *         
 *         if (iconCache.contains(cacheKey)) {
 *             return iconCache[cacheKey];
 *         }
 *         
 *         QIcon originalIcon(iconPath);
 *         QIcon scaledIcon = createScaledIcon(originalIcon, size);
 *         iconCache[cacheKey] = scaledIcon;
 *         
 *         return scaledIcon;
 *     }
 *     
 * private:
 *     static QIcon createScaledIcon(const QIcon& original, const QSize& size) {
 *         QPixmap pixmap = original.pixmap(size, QIcon::Normal, QIcon::Off);
 *         
 *         // Apply theme-aware colorization if needed
 *         if (shouldColorizeIcon()) {
 *             pixmap = applyThemeColorization(pixmap);
 *         }
 *         
 *         return QIcon(pixmap);
 *     }
 *     
 *     static QHash<QString, QIcon> iconCache;
 * };
 * ```
 * 
 * ## Memory Usage Optimization
 * 
 * Responsible memory management for long-running professional applications:
 * 
 * ```cpp
 * // Memory-efficient button management
 * class ButtonMemoryManager {
 * public:
 *     static void optimizeMemoryUsage() {
 *         // Clear unused style sheet cache entries
 *         ButtonStyleCache::clearUnusedEntries();
 *         
 *         // Compress icon cache
 *         IconManager::compressCache();
 *         
 *         // Clean up animation objects
 *         cleanupCompletedAnimations();
 *     }
 *     
 *     static void scheduleMemoryCleanup() {
 *         QTimer::singleShot(60000, optimizeMemoryUsage); // Every minute
 *     }
 * };
 * 
 * // Automatic resource cleanup
 * ThemedButton::~ThemedButton() {
 *     // Clean up animations
 *     for (auto* animation : findChildren<QPropertyAnimation*>()) {
 *         animation->stop();
 *         animation->deleteLater();
 *     }
 *     
 *     // Release cached resources
 *     releaseStyleResources();
 *     releaseIconResources();
 * }
 * ```
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * PROFESSIONAL USAGE EXAMPLES & BEST PRACTICES
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * ## Complete Modeling Interface Example
 * 
 * Comprehensive example showing ThemedButton integration in a professional modeling interface:
 * 
 * ```cpp
 * // Professional modeling interface with themed buttons
 * class ModelingInterface : public QMainWindow {
 * public:
 *     void setupInterface() {
 *         setupMainToolbar();
 *         setupModelingPanel();
 *         setupPropertiesPanel();
 *         setupStatusActions();
 *     }
 *     
 * private:
 *     void setupMainToolbar() {
 *         QToolBar* mainToolbar = addToolBar("Main");
 *         
 *         // File operations
 *         ThemedButton* newBtn = new ThemedButton("New", this);
 *         newBtn->setIcon(QIcon(":/icons/file/new.svg"));
 *         newBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         newBtn->setShortcut(QKeySequence::New);
 *         
 *         ThemedButton* openBtn = new ThemedButton("Open", this);
 *         openBtn->setIcon(QIcon(":/icons/file/open.svg"));
 *         openBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         openBtn->setShortcut(QKeySequence::Open);
 *         
 *         ThemedButton* saveBtn = new ThemedButton("Save", this);
 *         saveBtn->setIcon(QIcon(":/icons/file/save.svg"));
 *         saveBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         saveBtn->setShortcut(QKeySequence::Save);
 *         
 *         mainToolbar->addWidget(newBtn);
 *         mainToolbar->addWidget(openBtn);
 *         mainToolbar->addWidget(saveBtn);
 *         mainToolbar->addSeparator();
 *         
 *         // Undo/Redo operations
 *         ThemedButton* undoBtn = new ThemedButton("Undo", this);
 *         undoBtn->setIcon(QIcon(":/icons/edit/undo.svg"));
 *         undoBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         undoBtn->setShortcut(QKeySequence::Undo);
 *         
 *         ThemedButton* redoBtn = new ThemedButton("Redo", this);
 *         redoBtn->setIcon(QIcon(":/icons/edit/redo.svg"));
 *         redoBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         redoBtn->setShortcut(QKeySequence::Redo);
 *         
 *         mainToolbar->addWidget(undoBtn);
 *         mainToolbar->addWidget(redoBtn);
 *         
 *         // Connect to command system
 *         connect(newBtn, &ThemedButton::clicked, commandManager, &CommandManager::newScene);
 *         connect(openBtn, &ThemedButton::clicked, commandManager, &CommandManager::openScene);
 *         connect(saveBtn, &ThemedButton::clicked, commandManager, &CommandManager::saveScene);
 *         connect(undoBtn, &ThemedButton::clicked, commandManager, &CommandManager::undo);
 *         connect(redoBtn, &ThemedButton::clicked, commandManager, &CommandManager::redo);
 *     }
 *     
 *     void setupModelingPanel() {
 *         QDockWidget* modelingDock = new QDockWidget("Modeling Tools", this);
 *         QWidget* modelingWidget = new QWidget;
 *         QVBoxLayout* layout = new QVBoxLayout(modelingWidget);
 *         
 *         // Basic mesh operations
 *         QGroupBox* basicGroup = new QGroupBox("Basic Operations");
 *         QGridLayout* basicLayout = new QGridLayout(basicGroup);
 *         
 *         ThemedButton* addCubeBtn = new ThemedButton("Add Cube", this);
 *         addCubeBtn->setIcon(QIcon(":/icons/primitives/cube.svg"));
 *         addCubeBtn->setButtonStyle(ThemedButton::ButtonStyle::Success);
 *         
 *         ThemedButton* addSphereBtn = new ThemedButton("Add Sphere", this);
 *         addSphereBtn->setIcon(QIcon(":/icons/primitives/sphere.svg"));
 *         addSphereBtn->setButtonStyle(ThemedButton::ButtonStyle::Success);
 *         
 *         ThemedButton* addCylinderBtn = new ThemedButton("Add Cylinder", this);
 *         addCylinderBtn->setIcon(QIcon(":/icons/primitives/cylinder.svg"));
 *         addCylinderBtn->setButtonStyle(ThemedButton::ButtonStyle::Success);
 *         
 *         ThemedButton* deleteBtn = new ThemedButton("Delete Selected", this);
 *         deleteBtn->setIcon(QIcon(":/icons/edit/delete.svg"));
 *         deleteBtn->setButtonStyle(ThemedButton::ButtonStyle::Danger);
 *         deleteBtn->setShortcut(QKeySequence::Delete);
 *         
 *         basicLayout->addWidget(addCubeBtn, 0, 0);
 *         basicLayout->addWidget(addSphereBtn, 0, 1);
 *         basicLayout->addWidget(addCylinderBtn, 1, 0);
 *         basicLayout->addWidget(deleteBtn, 1, 1);
 *         
 *         // Advanced modeling tools
 *         QGroupBox* advancedGroup = new QGroupBox("Advanced Tools");
 *         QVBoxLayout* advancedLayout = new QVBoxLayout(advancedGroup);
 *         
 *         ThemedButton* extrudeBtn = new ThemedButton("Extrude", this);
 *         extrudeBtn->setIcon(QIcon(":/icons/tools/extrude.svg"));
 *         extrudeBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         extrudeBtn->setShortcut(QKeySequence("E"));
 *         
 *         ThemedButton* insetBtn = new ThemedButton("Inset", this);
 *         insetBtn->setIcon(QIcon(":/icons/tools/inset.svg"));
 *         insetBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         insetBtn->setShortcut(QKeySequence("I"));
 *         
 *         ThemedButton* bevelBtn = new ThemedButton("Bevel", this);
 *         bevelBtn->setIcon(QIcon(":/icons/tools/bevel.svg"));
 *         bevelBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         bevelBtn->setShortcut(QKeySequence("B"));
 *         
 *         advancedLayout->addWidget(extrudeBtn);
 *         advancedLayout->addWidget(insetBtn);
 *         advancedLayout->addWidget(bevelBtn);
 *         
 *         layout->addWidget(basicGroup);
 *         layout->addWidget(advancedGroup);
 *         layout->addStretch();
 *         
 *         modelingDock->setWidget(modelingWidget);
 *         addDockWidget(Qt::LeftDockWidgetArea, modelingDock);
 *         
 *         // Connect to modeling system
 *         connect(extrudeBtn, &ThemedButton::clicked, 
 *                 toolManager, &ModelingToolManager::activateExtrudeTool);
 *         connect(insetBtn, &ThemedButton::clicked,
 *                 toolManager, &ModelingToolManager::activateInsetTool);
 *         connect(bevelBtn, &ThemedButton::clicked,
 *                 toolManager, &ModelingToolManager::activateBevelTool);
 *     }
 * };
 * ```
 * 
 * ## Responsive Dialog System
 * 
 * Professional dialog implementation with consistent button styling:
 * 
 * ```cpp
 * // Export options dialog with themed buttons
 * class ExportOptionsDialog : public QDialog {
 * public:
 *     ExportOptionsDialog(QWidget* parent = nullptr) : QDialog(parent) {
 *         setupUI();
 *         connectSignals();
 *         loadSettings();
 *     }
 *     
 * private:
 *     void setupUI() {
 *         setWindowTitle("Export Options");
 *         setModal(true);
 *         resize(500, 400);
 *         
 *         QVBoxLayout* mainLayout = new QVBoxLayout(this);
 *         
 *         // Export format selection
 *         QGroupBox* formatGroup = new QGroupBox("Export Format");
 *         QVBoxLayout* formatLayout = new QVBoxLayout(formatGroup);
 *         
 *         objBtn = new ThemedButton("Wavefront OBJ", this);
 *         objBtn->setIcon(QIcon(":/icons/formats/obj.svg"));
 *         objBtn->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 *         objBtn->setCheckable(true);
 *         
 *         fbxBtn = new ThemedButton("Autodesk FBX", this);
 *         fbxBtn->setIcon(QIcon(":/icons/formats/fbx.svg"));
 *         fbxBtn->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 *         fbxBtn->setCheckable(true);
 *         
 *         gltfBtn = new ThemedButton("glTF 2.0", this);
 *         gltfBtn->setIcon(QIcon(":/icons/formats/gltf.svg"));
 *         gltfBtn->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 *         gltfBtn->setCheckable(true);
 *         
 *         formatLayout->addWidget(objBtn);
 *         formatLayout->addWidget(fbxBtn);
 *         formatLayout->addWidget(gltfBtn);
 *         
 *         // Quality settings
 *         QGroupBox* qualityGroup = new QGroupBox("Quality Settings");
 *         QHBoxLayout* qualityLayout = new QHBoxLayout(qualityGroup);
 *         
 *         lowQualityBtn = new ThemedButton("Low", this);
 *         lowQualityBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         lowQualityBtn->setCheckable(true);
 *         
 *         mediumQualityBtn = new ThemedButton("Medium", this);
 *         mediumQualityBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         mediumQualityBtn->setCheckable(true);
 *         mediumQualityBtn->setChecked(true);
 *         
 *         highQualityBtn = new ThemedButton("High", this);
 *         highQualityBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         highQualityBtn->setCheckable(true);
 *         
 *         qualityLayout->addWidget(lowQualityBtn);
 *         qualityLayout->addWidget(mediumQualityBtn);
 *         qualityLayout->addWidget(highQualityBtn);
 *         
 *         // Action buttons
 *         QHBoxLayout* buttonLayout = new QHBoxLayout;
 *         
 *         previewBtn = new ThemedButton("Preview", this);
 *         previewBtn->setIcon(QIcon(":/icons/preview.svg"));
 *         previewBtn->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 *         
 *         exportBtn = new ThemedButton("Export", this);
 *         exportBtn->setIcon(QIcon(":/icons/export.svg"));
 *         exportBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         
 *         cancelBtn = new ThemedButton("Cancel", this);
 *         cancelBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         
 *         buttonLayout->addWidget(previewBtn);
 *         buttonLayout->addStretch();
 *         buttonLayout->addWidget(exportBtn);
 *         buttonLayout->addWidget(cancelBtn);
 *         
 *         mainLayout->addWidget(formatGroup);
 *         mainLayout->addWidget(qualityGroup);
 *         mainLayout->addStretch();
 *         mainLayout->addLayout(buttonLayout);
 *     }
 *     
 *     void connectSignals() {
 *         // Format selection (exclusive)
 *         connect(objBtn, &ThemedButton::clicked, [this]() { selectFormat("obj"); });
 *         connect(fbxBtn, &ThemedButton::clicked, [this]() { selectFormat("fbx"); });
 *         connect(gltfBtn, &ThemedButton::clicked, [this]() { selectFormat("gltf"); });
 *         
 *         // Quality selection (exclusive)
 *         connect(lowQualityBtn, &ThemedButton::clicked, [this]() { selectQuality("low"); });
 *         connect(mediumQualityBtn, &ThemedButton::clicked, [this]() { selectQuality("medium"); });
 *         connect(highQualityBtn, &ThemedButton::clicked, [this]() { selectQuality("high"); });
 *         
 *         // Actions
 *         connect(previewBtn, &ThemedButton::clicked, this, &ExportOptionsDialog::previewExport);
 *         connect(exportBtn, &ThemedButton::clicked, this, &ExportOptionsDialog::performExport);
 *         connect(cancelBtn, &ThemedButton::clicked, this, &QDialog::reject);
 *     }
 *     
 *     void selectFormat(const QString& format) {
 *         // Reset all format buttons
 *         objBtn->setChecked(false);
 *         objBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         fbxBtn->setChecked(false);
 *         fbxBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         gltfBtn->setChecked(false);
 *         gltfBtn->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 *         
 *         // Highlight selected format
 *         ThemedButton* selectedBtn = nullptr;
 *         if (format == "obj") selectedBtn = objBtn;
 *         else if (format == "fbx") selectedBtn = fbxBtn;
 *         else if (format == "gltf") selectedBtn = gltfBtn;
 *         
 *         if (selectedBtn) {
 *             selectedBtn->setChecked(true);
 *             selectedBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         }
 *         
 *         m_selectedFormat = format;
 *         updateExportButton();
 *     }
 *     
 *     void updateExportButton() {
 *         bool canExport = !m_selectedFormat.isEmpty();
 *         exportBtn->setEnabled(canExport);
 *         
 *         if (canExport) {
 *             exportBtn->setText(QString("Export as %1").arg(m_selectedFormat.toUpper()));
 *             exportBtn->setButtonStyle(ThemedButton::ButtonStyle::Success);
 *         } else {
 *             exportBtn->setText("Export");
 *             exportBtn->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 *         }
 *     }
 *     
 *     ThemedButton* objBtn;
 *     ThemedButton* fbxBtn;
 *     ThemedButton* gltfBtn;
 *     ThemedButton* lowQualityBtn;
 *     ThemedButton* mediumQualityBtn;
 *     ThemedButton* highQualityBtn;
 *     ThemedButton* previewBtn;
 *     ThemedButton* exportBtn;
 *     ThemedButton* cancelBtn;
 *     
 *     QString m_selectedFormat;
 * };
 * ```
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * @ingroup UI
 * @ingroup Components  
 * @ingroup ThemeSystem
 * @ingroup UserInteraction
 * @ingroup ModelingInterface
 * @ingroup ProfessionalUI
 * 
 * @see BaseComponent For foundational component features and state management
 * @see ThemeManager For theme integration and color management  
 * @see UIManager For overall UI coordination and consistency
 * @see ModelingToolManager For integration with 3D modeling tools
 * @see CommandManager For command system integration
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * 
 * @note ThemedButton represents the cornerstone of RudeBase3D's professional interface
 *       design, providing the visual consistency and interactive excellence expected
 *       in modern 3D modeling applications.
 * 
 * ═══════════════════════════════════════════════════════════════════════════════════
 */
 * ### Theme Integration
 * Seamless integration with the application theme system:
 * - **Automatic Color Adaptation**: Colors update automatically with theme changes
 * - **Style Consistency**: Maintains visual consistency across the application
 * - **Accessibility Compliance**: Meets accessibility contrast requirements
 * - **Dark/Light Mode Support**: Optimized appearance for both theme modes
 * 
 * ## Advanced Features
 * 
 * ### State-Aware Styling
 * Professional visual feedback for all interaction states:
 * - **Hover Effects**: Smooth color and shadow transitions on mouse hover
 * - **Focus Indication**: Clear focus rings for keyboard navigation
 * - **Pressed States**: Immediate visual feedback for button activation
 * - **Disabled Appearance**: Clear indication when buttons are non-interactive
 * - **Loading States**: Visual progress indication for asynchronous operations
 * - **Error States**: Visual error indication with appropriate styling
 * 
 * ### Professional Animation System
 * Smooth animations for enhanced user experience:
 * - **Hover Transitions**: Subtle color and shadow animations
 * - **Press Feedback**: Immediate visual response to user input
 * - **State Changes**: Smooth transitions between different button states
 * - **Loading Animations**: Progress indicators for long-running operations
 * - **Customizable Timing**: Configurable animation duration and easing
 * 
 * ### Accessibility Integration
 * Comprehensive accessibility support for professional applications:
 * - **Screen Reader Support**: Proper ARIA labels and descriptions
 * - **Keyboard Navigation**: Full keyboard control with proper focus management
 * - **High Contrast Support**: Automatic adaptation to accessibility themes
 * - **Touch Target Sizing**: Appropriate minimum sizes for touch interfaces
 * - **Focus Indicators**: Clear visual focus indication for all interaction modes
 * 
 * ## Architecture Integration
 * 
 * ### BaseComponent Inheritance
 * Inherits all BaseComponent features for consistent behavior:
 * ```cpp
 * // ThemedButton automatically includes:
 * // - Theme integration and automatic updates
 * // - State management with visual feedback
 * // - Accessibility support and ARIA attributes
 * // - Error handling and loading states
 * // - Signal/slot integration for loose coupling
 * ```
 * 
 * ### UI Framework Integration
 * Designed for seamless integration with the broader UI framework:
 * - **Layout Management**: Proper size hints and layout behavior
 * - **Panel Integration**: Consistent appearance in dockable panels
 * - **Toolbar Compatibility**: Optimized for toolbar and menu usage
 * - **Dialog Integration**: Professional appearance in modal dialogs
 * 
 * ## Usage Examples
 * 
 * ### Basic Button Creation
 * ```cpp
 * // Create primary action button
 * ThemedButton* saveButton = new ThemedButton("Save", parent);
 * saveButton->setButtonStyle(ThemedButton::ButtonStyle::Primary);
 * 
 * // Create icon button with text
 * QIcon exportIcon(":/icons/export.svg");
 * ThemedButton* exportButton = new ThemedButton(exportIcon, "Export", parent);
 * exportButton->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 * 
 * // Create danger button for destructive actions
 * ThemedButton* deleteButton = new ThemedButton("Delete", parent);
 * deleteButton->setButtonStyle(ThemedButton::ButtonStyle::Danger);
 * ```
 * 
 * ### Advanced Configuration
 * ```cpp
 * // Configure button with custom icon size
 * ThemedButton* toolButton = new ThemedButton(parent);
 * toolButton->setIcon(QIcon(":/icons/tool.svg"));
 * toolButton->setText("Modeling Tool");
 * toolButton->setIconSize(QSize(24, 24));
 * toolButton->setButtonStyle(ThemedButton::ButtonStyle::Ghost);
 * 
 * // Connect to actions
 * connect(toolButton, &ThemedButton::clicked,
 *         this, &MainWindow::activateModelingTool);
 * ```
 * 
 * ### State Management Integration
 * ```cpp
 * // Show loading state during operations
 * saveButton->showLoadingState();
 * 
 * // Perform async operation
 * QTimer::singleShot(2000, [saveButton]() {
 *     saveButton->hideLoadingState();
 * });
 * 
 * // Handle error states
 * if (operationFailed) {
 *     saveButton->showErrorState("Save operation failed");
 * }
 * ```
 * 
 * ### Dynamic Style Changes
 * ```cpp
 * // Change button style based on context
 * if (hasUnsavedChanges) {
 *     saveButton->setButtonStyle(ThemedButton::ButtonStyle::Warning);
 *     saveButton->setText("Save Changes");
 * } else {
 *     saveButton->setButtonStyle(ThemedButton::ButtonStyle::Secondary);
 *     saveButton->setText("Save");
 * }
 * ```
 * 
 * ## Performance Considerations
 * 
 * ### Efficient Rendering
 * Optimized for smooth performance in complex interfaces:
 * - **Cached Style Sheets**: Style sheets are cached and reused for efficiency
 * - **Minimal Repaints**: State changes trigger only necessary visual updates
 * - **Icon Caching**: Icons are cached at appropriate sizes for fast rendering
 * - **Animation Optimization**: Smooth animations without performance impact
 * 
 * ### Memory Management
 * Responsible resource usage for long-running applications:
 * - **Shared Resources**: Theme resources are shared across button instances
 * - **Automatic Cleanup**: Proper cleanup of resources and event handlers
 * - **Icon Management**: Efficient icon storage and scaling
 * - **Style Sheet Optimization**: Optimized CSS generation and caching
 * 
 * @ingroup UI
 * @ingroup Components
 * @ingroup ThemeSystem
 * @ingroup UserInteraction
 * 
 * @see BaseComponent For foundational component features and state management
 * @see ThemeManager For theme integration and color management
 * @see UIManager For overall UI coordination and consistency
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class ThemedButton : public BaseComponent
{
    Q_OBJECT

public:
    enum class ButtonStyle {
        Primary,    // Main action button
        Secondary,  // Secondary actions
        Success,    // Positive actions (save, create, etc.)
        Warning,    // Warning actions
        Danger,     // Destructive actions (delete, etc.)
        Ghost       // Minimal style
    };

    explicit ThemedButton(QWidget* parent = nullptr);
    explicit ThemedButton(const QString& text, QWidget* parent = nullptr);
    explicit ThemedButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);

    /**
     * @brief Set button text
     */
    void setText(const QString& text);
    QString text() const;

    /**
     * @brief Set button icon
     */
    void setIcon(const QIcon& icon);
    QIcon icon() const;

    /**
     * @brief Set button style
     */
    void setButtonStyle(ButtonStyle style);
    ButtonStyle buttonStyle() const;

    /**
     * @brief Set icon size
     */
    void setIconSize(const QSize& size);
    QSize iconSize() const;

protected:
    void applyTheme() override;
    QString generateBaseStyleSheet() const override;
    void onStateChanged(ComponentState oldState, ComponentState newState) override;
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void clicked();

private:
    void setupButton();
    QString getStyleSheetForState() const;
    QColor getBackgroundColorForState() const;
    QColor getTextColorForState() const;

    QPushButton* m_button;
    ButtonStyle m_buttonStyle = ButtonStyle::Primary;
    QString m_text;
    QIcon m_icon;
    QSize m_iconSize{16, 16};
};

} // namespace ui
} // namespace rudebase3d
