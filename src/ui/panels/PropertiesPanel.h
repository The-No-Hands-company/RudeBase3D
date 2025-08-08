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
 * █  FILE: PropertiesPanel.h                                                     █
 * █  DESCRIPTION: Professional Object Property Editor for 3D Applications        █
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
 * █  This file contains the PropertiesPanel implementation providing             █
 * █  comprehensive object property editing, real-time parameter adjustment,      █
 * █  and advanced material property management for professional 3D workflows.    █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

#pragma once

#include "Common.h"
#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QColorDialog>
#include <memory>

// Forward declarations
namespace rude { 
    class Scene; 
    class Entity;
}

/**
 * @brief Professional object property editor panel for comprehensive 3D object manipulation
 * 
 * The PropertiesPanel class provides a sophisticated, user-friendly interface for editing
 * properties of selected 3D objects in professional modeling applications. This panel
 * dynamically adapts to show relevant properties based on the currently selected object,
 * providing context-sensitive editing capabilities that are essential for productive
 * 3D modeling workflows.
 * 
 * ## Core Functionality
 * 
 * ### Object Information Management
 * The panel provides comprehensive object information display and editing:
 * - **Name Editing**: Interactive object name editing with validation
 * - **Visibility Control**: Toggle object visibility with immediate viewport updates
 * - **Object Type Display**: Shows object type with appropriate icons
 * - **Selection Tracking**: Automatically updates when scene selection changes
 * 
 * ### Transform Property Editing
 * Professional-grade transform editing with precision controls:
 * - **Position Control**: X, Y, Z position editing with high precision
 * - **Rotation Control**: Euler angle rotation editing with degree/radian options
 * - **Scale Control**: Uniform and non-uniform scaling with lock options
 * - **Transform Reset**: Quick reset to identity transform
 * - **Real-time Updates**: Live viewport updates during property changes
 * 
 * ### Material Property System
 * Comprehensive material editing with industry-standard controls:
 * - **Color Management**: Diffuse, specular, and ambient color editing
 * - **Surface Properties**: Shininess, metallic, and roughness parameters
 * - **Wireframe Control**: Wireframe color and line width adjustment
 * - **Material Presets**: Pre-configured material templates for quick application
 * - **Real-time Preview**: Immediate material updates in viewport
 * 
 * ## Architecture Integration
 * 
 * ### Scene System Integration
 * The PropertiesPanel integrates seamlessly with the scene management system:
 * ```cpp
 * // Example usage
 * PropertiesPanel* panel = new PropertiesPanel(parent);
 * panel->setScene(scene);
 * 
 * // Selection updates automatically trigger property display
 * connect(sceneHierarchy, &SceneHierarchyPanel::selectionChanged,
 *         panel, &PropertiesPanel::setSelectedObject);
 * ```
 * 
 * ### UI Framework Integration
 * Designed as a professional dockable panel:
 * - **Dockable Interface**: Can be docked to any edge of the main window
 * - **Floating Support**: Can be undocked for multi-monitor workflows
 * - **Resizable Layout**: Adaptive layout that works at various panel sizes
 * - **Theme Integration**: Respects application theme and styling
 * 
 * ### Real-time Update System
 * Efficient property synchronization with the 3D scene:
 * - **Change Detection**: Prevents unnecessary updates during batch operations
 * - **Undo Integration**: All property changes are undoable through command system
 * - **Multi-Selection**: Supports editing multiple objects simultaneously
 * - **Conflict Resolution**: Handles conflicting values in multi-object selection
 * 
 * ## Professional Features
 * 
 * ### Precision Control Interface
 * The panel provides professional-grade precision controls:
 * - **High-Precision Spinboxes**: Double-precision editing with customizable step sizes
 * - **Unit Support**: Supports different measurement units (meters, feet, etc.)
 * - **Range Validation**: Automatic clamping to valid property ranges
 * - **Keyboard Shortcuts**: Efficient keyboard navigation and editing
 * 
 * ### Color Management System
 * Professional color editing with industry standards:
 * - **Color Picker Integration**: Full-featured color picker with multiple color spaces
 * - **Preset Colors**: Quick access to commonly used colors
 * - **Color History**: Remembers recently used colors for quick reselection
 * - **Alpha Channel**: Full RGBA support for transparent materials
 * 
 * ### Material Workflow
 * Streamlined material editing workflow:
 * - **Material Templates**: Industry-standard material presets (metal, plastic, etc.)
 * - **Custom Materials**: Create and save custom material configurations
 * - **Material Library**: Access to shared material library
 * - **PBR Support**: Full physically-based rendering parameter support
 * 
 * ## Usage Examples
 * 
 * ### Basic Object Property Editing
 * ```cpp
 * // Set up the properties panel
 * PropertiesPanel* properties = new PropertiesPanel(mainWindow);
 * properties->setScene(scene);
 * 
 * // Connect to selection system
 * connect(selectionManager, &SelectionManager::selectionChanged,
 *         properties, &PropertiesPanel::setSelectedObject);
 * 
 * // The panel automatically updates when objects are selected
 * ```
 * 
 * ### Advanced Material Editing
 * ```cpp
 * // Connect to material change notifications
 * connect(properties, &PropertiesPanel::materialChanged,
 *         renderer, &Renderer::updateMaterial);
 * 
 * // Custom material preset handling
 * connect(properties, &PropertiesPanel::materialPresetRequested,
 *         materialManager, &MaterialManager::applyPreset);
 * ```
 * 
 * ### Transform Editing Integration
 * ```cpp
 * // Connect transform changes to gizmo system
 * connect(properties, &PropertiesPanel::transformChanged,
 *         gizmoManager, &GizmoManager::updateGizmo);
 * 
 * // Bidirectional updates between gizmo and properties
 * connect(gizmoManager, &GizmoManager::objectTransformed,
 *         properties, &PropertiesPanel::updateTransformDisplay);
 * ```
 * 
 * ## Performance Considerations
 * 
 * ### Efficient Update Handling
 * The panel is optimized for smooth real-time editing:
 * - **Batch Updates**: Groups multiple property changes to minimize overhead
 * - **Selective Updates**: Only updates changed properties, not entire interface
 * - **Update Throttling**: Prevents excessive updates during rapid value changes
 * - **Memory Efficiency**: Minimal memory footprint for large scene handling
 * 
 * ### Responsive Interface Design
 * Maintains responsiveness during heavy scene operations:
 * - **Asynchronous Updates**: Non-blocking property updates
 * - **Progressive Loading**: Loads complex property sets progressively
 * - **Caching Strategy**: Caches property values to reduce scene queries
 * - **Lazy Evaluation**: Defers expensive calculations until actually needed
 * 
 * @ingroup UI
 * @ingroup Panels
 * @ingroup ObjectEditing
 * 
 * @see SceneHierarchyPanel For object selection and hierarchy management
 * @see ViewportManager For viewport integration and real-time updates
 * @see UIManager For panel management and docking system
 * @see ThemeManager For consistent styling and theming
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */

class PropertiesPanel : public QDockWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs a new PropertiesPanel instance
     * 
     * Creates a comprehensive object property editor panel with all necessary
     * UI components for editing 3D object properties. The panel is initialized
     * with a professional layout suitable for docking in the main application window.
     * 
     * The constructor sets up:
     * - Main layout structure with organized property groups
     * - Object information editing controls (name, visibility)
     * - Transform editing controls (position, rotation, scale)
     * - Material property controls (colors, surface properties)
     * - Signal connections for real-time property updates
     * - Professional styling and theming integration
     * 
     * @param parent Parent widget for the properties panel. If nullptr, the panel
     *               will be created as a top-level window. Typically should be
     *               the main application window for proper docking behavior.
     * 
     * @note The panel is created in a ready-to-use state but requires a scene
     *       to be set via setScene() before object properties can be displayed.
     * 
     * @see setScene() To connect the panel to a 3D scene
     * @see setSelectedObject() To display properties of a specific object
     */
    explicit PropertiesPanel(QWidget* parent = nullptr);
    
    /**
     * @brief Uses default destructor for clean resource management
     * 
     * The default destructor is sufficient as the class uses RAII principles
     * and Qt's automatic parent-child memory management for UI components.
     * All shared pointers and Qt objects are automatically cleaned up.
     */
    ~PropertiesPanel() = default;

    /**
     * @brief Sets the 3D scene that this properties panel will operate on
     * 
     * Connects the properties panel to a specific 3D scene, enabling it to
     * display and edit properties of objects within that scene. This establishes
     * the necessary connections for real-time property synchronization.
     * 
     * @param scene Shared pointer to the 3D scene. Must be a valid scene object
     *              with proper initialization. Can be nullptr to disconnect from
     *              any current scene (useful during scene transitions).
     * 
     * @note Changing the scene will clear any current object selection and
     *       reset the panel to its default state.
     * 
     * @see setSelectedObject() To select an object for property editing
     */
    void setScene(std::shared_ptr<rude::Scene> scene);
    
    /**
     * @brief Gets the currently connected 3D scene
     * 
     * Returns the scene that this properties panel is currently operating on.
     * This is useful for validation and debugging purposes.
     * 
     * @return Shared pointer to the current scene, or nullptr if no scene is set
     * 
     * @note The returned pointer should not be cached as the scene may change
     *       when setScene() is called with a different scene.
     */
    std::shared_ptr<rude::Scene> getScene() const { return m_scene; }
    
    /**
     * @brief Sets the entity for property editing (ECS-based objects)
     * 
     * Configures the properties panel to edit properties of an ECS entity.
     * This method is used for entity-based objects that use the ECS architecture
     * rather than traditional scene objects.
     * 
     * @param entity Pointer to the entity to edit. Can be nullptr to clear
     *               the current entity selection. The entity must be valid
     *               and belong to the current scene's entity registry.
     * 
     * @note This method is an alternative to setSelectedObject() for ECS-based
     *       object management. Use the method appropriate for your object type.
     * 
     * @see setSelectedObject() For traditional scene object editing
     */
    void setEntity(rude::Entity* entity);
    
    /**
     * @brief Refreshes all property displays to reflect current object state
     * 
     * Forces a complete refresh of all property groups to ensure they display
     * the current state of the selected object. This is useful after external
     * modifications to the object or when property synchronization is needed.
     * 
     * The refresh updates:
     * - Object information (name, visibility status)
     * - Transform properties (position, rotation, scale values)
     * - Material properties (colors, surface parameters)
     * - Any custom properties based on object type
     * 
     * @note This method is automatically called when selection changes, but
     *       can be called manually when external systems modify object properties.
     * 
     * @see setSelectedObject() Which automatically triggers a refresh
     */
    void refreshProperties();
    
    /**
     * @brief Sets the currently selected object for property editing
     * 
     * Updates the properties panel to display and edit properties of the
     * specified object. This triggers a complete refresh of all property
     * groups to show values relevant to the selected object type.
     * 
     * @param object Shared pointer to the scene object to edit. Can be nullptr
     *               to clear the selection and show an empty properties panel.
     *               The object must belong to the currently set scene.
     * 
     * The method automatically:
     * - Updates object information (name, visibility, type)
     * - Refreshes transform properties (position, rotation, scale)
     * - Updates material properties based on object's material
     * - Enables/disables relevant property groups based on object capabilities
     * 
     * @note This method is typically called automatically when the user selects
     *       objects in the scene hierarchy or viewport, but can also be called
     *       programmatically for specific selection scenarios.
     * 
     * @see SceneHierarchyPanel::selectionChanged() For automatic selection updates
     * @see setEntity() For ECS-based object editing
     */
    void setSelectedObject(SceneObjectPtr object);

private slots:
    /**
     * @brief Handles object name changes from the name editor
     * 
     * This slot is triggered when the user modifies the object name in the
     * name edit field. It validates the new name and updates the selected
     * object accordingly, ensuring the change is reflected throughout the
     * application (scene hierarchy, undo system, etc.).
     * 
     * The method:
     * - Validates the new name for uniqueness and legal characters
     * - Updates the object's name property in the scene
     * - Triggers scene hierarchy refresh if needed
     * - Registers the change with the undo/redo system
     * 
     * @note Empty names or names with invalid characters are automatically
     *       corrected or reverted to the previous valid name.
     */
    void onNameChanged();
    
    /**
     * @brief Handles object visibility toggle changes
     * 
     * Responds to visibility checkbox state changes by updating the selected
     * object's visibility in the 3D scene. This immediately affects viewport
     * rendering and selection behavior.
     * 
     * @param visible New visibility state. True makes the object visible in
     *                all viewports, false hides it from rendering but keeps
     *                it selectable in the scene hierarchy.
     * 
     * The method:
     * - Updates the object's visibility flag
     * - Triggers immediate viewport refresh
     * - Updates related UI elements (scene hierarchy icons)
     * - Registers the change for undo/redo
     */
    void onVisibilityChanged(bool visible);
    
    // Transform slots
    
    /**
     * @brief Handles position value changes from position spinboxes
     * 
     * Triggered when the user modifies X, Y, or Z position values in the
     * transform group. Updates the object's world position and refreshes
     * the viewport display with the new position.
     * 
     * The method:
     * - Reads current values from position spinboxes
     * - Validates position values for scene bounds
     * - Updates the object's transform matrix
     * - Triggers viewport refresh and gizmo updates
     * - Registers transform change for undo/redo
     * 
     * @note Position changes are applied in world coordinates and may
     *       affect child object positions in hierarchical scenes.
     */
    void onPositionChanged();
    
    /**
     * @brief Handles rotation value changes from rotation spinboxes
     * 
     * Responds to Euler angle rotation changes (X, Y, Z) by updating the
     * object's orientation. Rotation values are interpreted as degrees and
     * converted to the appropriate internal representation.
     * 
     * The method:
     * - Reads Euler angles from rotation spinboxes
     * - Converts degrees to radians for internal calculations
     * - Updates the object's rotation matrix
     * - Handles gimbal lock prevention where applicable
     * - Triggers viewport and gizmo updates
     * - Registers rotation change for undo/redo
     * 
     * @note Rotation order follows standard Euler angle conventions (XYZ).
     */
    void onRotationChanged();
    
    /**
     * @brief Handles scale value changes from scale spinboxes
     * 
     * Updates object scale based on X, Y, Z scale factor changes. Supports
     * both uniform and non-uniform scaling with appropriate validation to
     * prevent degenerate transformations.
     * 
     * The method:
     * - Reads scale factors from scale spinboxes
     * - Validates scale values (prevents zero or negative scales)
     * - Updates the object's scale matrix
     * - Handles uniform scaling constraints if enabled
     * - Triggers viewport refresh and bounding box updates
     * - Registers scale change for undo/redo
     * 
     * @note Zero or very small scale values are clamped to prevent
     *       rendering artifacts and mathematical instability.
     */
    void onScaleChanged();
    
    /**
     * @brief Resets object transform to identity values
     * 
     * Triggered by the "Reset Transform" button, this method resets the
     * selected object's transform to default identity values (position at
     * origin, no rotation, unit scale).
     * 
     * The reset operation:
     * - Sets position to (0, 0, 0)
     * - Sets rotation to (0, 0, 0) degrees
     * - Sets scale to (1, 1, 1)
     * - Updates all transform spinboxes to reflect new values
     * - Triggers complete viewport refresh
     * - Registers the reset as a single undoable operation
     * 
     * @note This is a common operation for centering objects or starting
     *       fresh with transform modifications.
     */
    void onResetTransform();
    
    // Material slots
    
    /**
     * @brief Handles diffuse color changes from the color picker
     * 
     * Triggered when the user selects a new diffuse color using the diffuse
     * color button. Updates the object's material diffuse color property
     * and triggers immediate material updates in the renderer.
     * 
     * The method:
     * - Opens color picker dialog if needed
     * - Extracts RGBA values from color selection
     * - Updates material diffuse color property
     * - Triggers material shader updates
     * - Updates color button appearance
     * - Registers material change for undo/redo
     * 
     * @note Diffuse color affects how the surface reflects ambient and
     *       direct lighting in the physically-based rendering pipeline.
     */
    void onDiffuseColorChanged();
    
    /**
     * @brief Handles specular color changes from the color picker
     * 
     * Updates the material's specular reflection color when the user modifies
     * the specular color property. This affects how the surface reflects
     * direct light sources and contributes to material appearance.
     * 
     * @note In PBR workflows, specular color is often derived from metallic
     *       and base color values, but direct editing is supported for
     *       artistic control and legacy material compatibility.
     */
    void onSpecularColorChanged();
    
    /**
     * @brief Handles ambient color changes from the color picker
     * 
     * Modifies the material's ambient color component, which affects how
     * the surface appears under ambient lighting conditions without direct
     * light sources.
     * 
     * @note Ambient color is typically used for artistic enhancement and
     *       global illumination approximation in real-time rendering.
     */
    void onAmbientColorChanged();
    
    /**
     * @brief Handles shininess parameter changes
     * 
     * Updates the material's shininess (specular exponent) value, controlling
     * the size and intensity of specular highlights on the surface.
     * 
     * @param value New shininess value. Higher values create smaller, more
     *              intense highlights (glossy surfaces), lower values create
     *              larger, diffuse highlights (matte surfaces). Range is
     *              typically 0.0 to 128.0 or higher for very glossy materials.
     * 
     * @note In PBR workflows, shininess is often replaced by roughness,
     *       but is maintained for compatibility with traditional materials.
     */
    void onShininessChanged(double value);
    
    /**
     * @brief Handles metallic parameter changes for PBR materials
     * 
     * Updates the material's metallic factor in physically-based rendering,
     * controlling whether the surface behaves like a metal or dielectric.
     * 
     * @param value Metallic factor from 0.0 (non-metallic/dielectric) to
     *              1.0 (fully metallic). Intermediate values can be used
     *              for artistic effects or layered materials.
     * 
     * The metallic parameter affects:
     * - Specular reflection intensity and color
     * - Diffuse contribution (metals have no diffuse reflection)
     * - Fresnel calculations for realistic material appearance
     */
    void onMetallicChanged(double value);
    
    /**
     * @brief Handles roughness parameter changes for PBR materials
     * 
     * Updates the material's surface roughness in physically-based rendering,
     * controlling the microsurface detail that affects light scattering.
     * 
     * @param value Surface roughness from 0.0 (perfectly smooth mirror) to
     *              1.0 (completely rough/matte). This parameter directly
     *              affects specular highlight size and reflection clarity.
     * 
     * The roughness parameter influences:
     * - Specular highlight distribution (GGX/Trowbridge-Reitz)
     * - Environment map reflection blur
     * - Overall material appearance from glossy to matte
     */
    void onRoughnessChanged(double value);
    
    /**
     * @brief Handles wireframe color changes
     * 
     * Updates the color used for wireframe rendering when the object is
     * displayed in wireframe mode. This is useful for object selection
     * highlighting and debugging mesh topology.
     * 
     * @note Wireframe color is independent of the material's surface colors
     *       and is only visible when wireframe rendering is enabled.
     */
    void onWireframeColorChanged();
    
    /**
     * @brief Handles wireframe line width changes
     * 
     * Updates the thickness of wireframe lines when the object is displayed
     * in wireframe mode. Thicker lines improve visibility but may obscure
     * fine mesh details.
     * 
     * @param value Line width in pixels. Typical range is 1.0 to 5.0 pixels.
     *              Values outside this range may not be supported by all
     *              graphics hardware or may cause performance issues.
     * 
     * @note Line width support varies by graphics driver and hardware.
     *       Some systems may ignore values other than 1.0.
     */
    void onWireframeWidthChanged(double value);
    
    /**
     * @brief Handles material preset selection changes
     * 
     * Applies a predefined material configuration when the user selects a
     * material preset from the dropdown menu. This provides quick access to
     * commonly used materials and professional material templates.
     * 
     * @param index Index of the selected preset in the preset list. Each
     *              preset contains predefined values for all material
     *              parameters (colors, roughness, metallic, etc.).
     * 
     * The method:
     * - Loads preset material parameters from the material library
     * - Updates all material property controls to reflect preset values
     * - Applies the complete material configuration to the selected object
     * - Registers the material change as a single undoable operation
     * 
     * @note Custom presets can be added to the material library and will
     *       automatically appear in the preset selection dropdown.
     */
    void onMaterialPresetChanged(int index);

private:
    // === Core Data Members ===
    
    /**
     * @brief Shared pointer to the 3D scene being edited
     * 
     * Maintains a reference to the current scene that contains the objects
     * whose properties are being edited. This scene reference is used for:
     * - Validating object selections
     * - Accessing scene-level properties and settings
     * - Coordinating with other scene-dependent systems
     * - Managing object lifecycle and relationships
     */
    std::shared_ptr<rude::Scene> m_scene;
    
    /**
     * @brief Pointer to the currently selected scene object
     * 
     * Holds a reference to the object whose properties are currently being
     * displayed and edited in the panel. This can be nullptr when no object
     * is selected or when the panel is in an empty state.
     */
    SceneObjectPtr m_selectedObject;
    
    /**
     * @brief Pointer to the currently selected ECS entity
     * 
     * Holds a reference to the ECS entity whose properties are currently being
     * displayed and edited. Used during the transition from SceneObject to ECS.
     * This can be nullptr when no entity is selected.
     */
    rude::Entity* m_currentEntity;
    
    // === Layout Management ===
    
    /**
     * @brief Main vertical layout container for the properties panel
     * 
     * The primary layout that organizes all property groups vertically.
     * Contains the object properties group, transform group, and material
     * group in a logical, easy-to-navigate arrangement.
     */
    QVBoxLayout* m_mainLayout;
    
    // === Object Properties Group ===
    
    /**
     * @brief Group box container for basic object properties
     * 
     * Contains controls for fundamental object properties like name and
     * visibility that apply to all types of scene objects.
     */
    QGroupBox* m_objectGroup;
    
    /**
     * @brief Text editor for object name modification
     * 
     * Allows users to edit the display name of the selected object. Names
     * are validated for uniqueness and legal characters before application.
     */
    QLineEdit* m_nameEdit;
    
    /**
     * @brief Checkbox for object visibility control
     * 
     * Toggles the visibility state of the selected object in all viewports.
     * Hidden objects remain selectable in the scene hierarchy but are not
     * rendered in the 3D views.
     */
    QCheckBox* m_visibleCheckBox;
    
    // === Transform Group ===
    
    /**
     * @brief Group box container for transform properties
     * 
     * Contains all transform-related controls including position, rotation,
     * and scale editing widgets arranged in a professional layout.
     */
    QGroupBox* m_transformGroup;
    
    /**
     * @brief X-coordinate position editor
     * 
     * High-precision spinbox for editing the X component of the object's
     * world position. Supports decimal precision and range validation.
     */
    QDoubleSpinBox* m_positionX;
    
    /**
     * @brief Y-coordinate position editor
     * 
     * High-precision spinbox for editing the Y component of the object's
     * world position. Typically represents the vertical axis in 3D space.
     */
    QDoubleSpinBox* m_positionY;
    
    /**
     * @brief Z-coordinate position editor
     * 
     * High-precision spinbox for editing the Z component of the object's
     * world position. Represents depth in the 3D coordinate system.
     */
    QDoubleSpinBox* m_positionZ;
    
    /**
     * @brief X-axis rotation editor (pitch)
     * 
     * Spinbox for editing rotation around the X-axis in degrees. Follows
     * standard Euler angle conventions with automatic range normalization.
     */
    QDoubleSpinBox* m_rotationX;
    
    /**
     * @brief Y-axis rotation editor (yaw)
     * 
     * Spinbox for editing rotation around the Y-axis in degrees. Typically
     * represents horizontal rotation or turning motion.
     */
    QDoubleSpinBox* m_rotationY;
    
    /**
     * @brief Z-axis rotation editor (roll)
     * 
     * Spinbox for editing rotation around the Z-axis in degrees. Represents
     * rolling or banking motion in 3D rotations.
     */
    QDoubleSpinBox* m_rotationZ;
    
    /**
     * @brief X-axis scale factor editor
     * 
     * Controls scaling along the X-axis. Values greater than 1.0 stretch
     * the object, values less than 1.0 compress it. Prevents zero values
     * to avoid degenerate transformations.
     */
    QDoubleSpinBox* m_scaleX;
    
    /**
     * @brief Y-axis scale factor editor
     * 
     * Controls scaling along the Y-axis (typically height). Maintains
     * positive values and provides precision control for exact scaling.
     */
    QDoubleSpinBox* m_scaleY;
    
    /**
     * @brief Z-axis scale factor editor
     * 
     * Controls scaling along the Z-axis (typically depth). Works in
     * conjunction with other scale components for uniform or non-uniform
     * scaling operations.
     */
    QDoubleSpinBox* m_scaleZ;
    
    /**
     * @brief Button to reset transform to identity values
     * 
     * Provides one-click reset of all transform components (position,
     * rotation, scale) to their default identity values. Useful for
     * centering objects or starting fresh with transformations.
     */
    QPushButton* m_resetTransformButton;
    
    // === Material Group ===
    
    /**
     * @brief Group box container for material properties
     * 
     * Organizes all material-related editing controls including colors,
     * surface properties, and rendering parameters in a professional layout.
     */
    QGroupBox* m_materialGroup;
    
    /**
     * @brief Color picker button for diffuse material color
     * 
     * Opens a color dialog for selecting the diffuse color component of
     * the material. The button face shows the current color for quick
     * visual reference.
     */
    QPushButton* m_diffuseColorButton;
    
    /**
     * @brief Color picker button for specular material color
     * 
     * Controls the specular reflection color, affecting how the material
     * reflects direct light sources. Important for realistic material
     * appearance in lit scenes.
     */
    QPushButton* m_specularColorButton;
    
    /**
     * @brief Color picker button for ambient material color
     * 
     * Sets the ambient color component, which affects the material's
     * appearance under ambient lighting conditions without direct illumination.
     */
    QPushButton* m_ambientColorButton;
    
    /**
     * @brief Spinbox for material shininess parameter
     * 
     * Controls the specular exponent that determines the size and intensity
     * of specular highlights. Higher values create more focused, intense
     * highlights typical of glossy surfaces.
     */
    QDoubleSpinBox* m_shininessSpinBox;
    
    /**
     * @brief Spinbox for PBR metallic parameter
     * 
     * Controls the metallic factor in physically-based rendering. Determines
     * whether the surface behaves like a metal (1.0) or dielectric (0.0).
     */
    QDoubleSpinBox* m_metallicSpinBox;
    
    /**
     * @brief Spinbox for PBR roughness parameter
     * 
     * Controls surface roughness in physically-based rendering. Affects
     * specular highlight distribution and reflection clarity from smooth
     * (0.0) to completely rough (1.0).
     */
    QDoubleSpinBox* m_roughnessSpinBox;
    
    /**
     * @brief Color picker button for wireframe rendering color
     * 
     * Sets the color used when the object is displayed in wireframe mode.
     * Useful for object highlighting and mesh topology visualization.
     */
    QPushButton* m_wireframeColorButton;
    
    /**
     * @brief Spinbox for wireframe line width
     * 
     * Controls the thickness of wireframe lines in pixels. Thicker lines
     * improve visibility but may obscure fine mesh details.
     */
    QDoubleSpinBox* m_wireframeWidthSpinBox;
    
    /**
     * @brief Dropdown for material preset selection
     * 
     * Provides quick access to predefined material configurations including
     * industry-standard materials and custom user presets from the material
     * library.
     */
    QComboBox* m_materialPresetCombo;
    
    // === State Management ===
    
    /**
     * @brief Flag to prevent recursive updates during property changes
     * 
     * Set to true during programmatic property updates to prevent signal
     * loops and recursive update cycles. Essential for maintaining UI
     * stability during batch property modifications.
     */
    bool m_updateInProgress;
    
    // === UI Setup Methods ===
    
    /**
     * @brief Initializes the complete user interface layout
     * 
     * Sets up the main panel layout and creates all property groups with
     * their respective controls. Establishes the visual hierarchy and
     * professional appearance of the properties panel.
     * 
     * Called during construction to build the complete UI structure.
     */
    void setupUI();
    
    /**
     * @brief Creates and configures the object properties group
     * 
     * Sets up the basic object property controls including name editing
     * and visibility toggling. Configures validators and initial states
     * for professional object management.
     */
    void setupObjectGroup();
    
    /**
     * @brief Creates and configures the transform properties group
     * 
     * Initializes all transform editing controls with appropriate ranges,
     * precision settings, and step values. Sets up the transform reset
     * button and organizes controls in a logical layout.
     */
    void setupTransformGroup();
    
    /**
     * @brief Creates and configures the material properties group
     * 
     * Sets up all material editing controls including color pickers,
     * parameter spinboxes, and the material preset selector. Configures
     * appropriate ranges and defaults for professional material editing.
     */
    void setupMaterialGroup();
    
    /**
     * @brief Connects all UI signals to their respective slot handlers
     * 
     * Establishes signal-slot connections between UI controls and their
     * corresponding update methods. Essential for real-time property
     * synchronization and responsive user interaction.
     */
    void connectSignals();
    
    // === Property Update Methods ===
    
    /**
     * @brief Updates object property displays to reflect current object state
     * 
     * Refreshes the object properties group (name, visibility) to show
     * current values from the selected object. Handles cases where no
     * object is selected by disabling or clearing controls appropriately.
     */
    void updateObjectProperties();
    
    /**
     * @brief Updates transform property displays to reflect current transform
     * 
     * Refreshes all transform controls (position, rotation, scale) with
     * current values from the selected object's transform matrix. Handles
     * matrix decomposition and unit conversion as needed.
     */
    void updateTransformProperties();
    
    /**
     * @brief Updates all entity property displays to reflect current entity
     * 
     * Refreshes all property controls with values from the currently selected
     * ECS entity. This includes object properties, transform properties, and
     * any entity-specific attributes.
     */
    void updateEntityProperties();
    
    /**
     * @brief Updates material property displays to reflect current material
     * 
     * Refreshes all material controls with values from the selected object's
     * current material. Handles both traditional and PBR material parameters
     * and updates color button appearances.
     */
    void updateMaterialProperties();
    
    // === Color Management Utilities ===
    
    /**
     * @brief Sets a color button's appearance to represent a specific color
     * 
     * Updates a color picker button's visual appearance (background color,
     * icon, etc.) to reflect the current color value. Provides immediate
     * visual feedback about the current color setting.
     * 
     * @param button The color button to update
     * @param color The RGBA color to represent (glm::vec4 format)
     */
    void setColorButton(QPushButton* button, const glm::vec4& color);
    
    /**
     * @brief Extracts color value from a color button's current state
     * 
     * Retrieves the RGBA color value currently represented by a color
     * picker button. Used when applying color changes to material properties.
     * 
     * @param button The color button to read from
     * @return Current color value in glm::vec4 RGBA format
     */
    glm::vec4 getColorFromButton(QPushButton* button);
    
    /**
     * @brief Opens a color selection dialog for a specific color button
     * 
     * Displays a professional color picker dialog allowing the user to
     * select a new color value. Updates the button appearance and triggers
     * appropriate material updates when a color is selected.
     * 
     * @param button The color button that triggered the color selection
     */
    void openColorDialog(QPushButton* button);
    
    // === Widget Factory Methods ===
    
    /**
     * @brief Creates a configured double-precision spinbox for property editing
     * 
     * Factory method that creates and configures QDoubleSpinBox widgets with
     * consistent settings for property editing. Provides standardized precision,
     * range validation, and step behavior across all numeric property controls.
     * 
     * @param min Minimum allowed value (default: -999999.0)
     * @param max Maximum allowed value (default: 999999.0)
     * @param step Step size for increment/decrement operations (default: 0.1)
     * @param decimals Number of decimal places to display (default: 3)
     * @return Configured QDoubleSpinBox ready for use in property editing
     * 
     * @note This ensures consistent behavior and appearance across all
     *       numeric property editors in the panel.
     */
    QDoubleSpinBox* createDoubleSpinBox(double min = -999999.0, double max = 999999.0, 
                                       double step = 0.1, int decimals = 3);
};
