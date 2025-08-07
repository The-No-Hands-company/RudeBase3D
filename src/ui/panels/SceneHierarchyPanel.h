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
 * █  FILE: SceneHierarchyPanel.h                                                 █
 * █  DESCRIPTION: Professional Scene Hierarchy & Outliner Panel                  █
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
 * █  This file contains the SceneHierarchyPanel implementation providing         █
 * █  professional scene outliner functionality, object hierarchy management,     █
 * █  and advanced 3D scene organization tools for modeling workflows.            █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

#pragma once

#include "Common.h"
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QBrush>
#include <QList>
#include <QString>
#include <QPoint>
#include <memory>

// Forward declarations
namespace rude { 
    class Scene; 
    class Entity;
}

/**
 * @brief Professional scene hierarchy and outliner panel for 3D object management
 * 
 * The SceneHierarchyPanel provides a comprehensive tree-based interface for
 * managing 3D scene objects, hierarchical relationships, and object properties.
 * It implements industry-standard outliner functionality found in professional
 * 3D modeling applications like Maya, Blender, and 3ds Max, enabling efficient
 * scene organization and object management workflows.
 * 
 * ## Core Features
 * - **Hierarchical Object Tree**: Visual representation of scene object hierarchy
 * - **Interactive Selection**: Two-way selection synchronization with 3D viewport
 * - **Object Management**: Rename, delete, duplicate operations through intuitive interface
 * - **Context-Sensitive Menus**: Right-click operations for streamlined object manipulation
 * - **Real-Time Updates**: Automatic updates when scene structure changes
 * - **Professional Appearance**: Industry-standard visual design and interaction patterns
 * 
 * ## Professional Scene Management
 * The panel implements comprehensive scene management functionality:
 * 
 * ### Object Hierarchy Display
 * - **Tree Structure**: Clear hierarchical display of parent-child relationships
 * - **Visual Grouping**: Indentation and visual cues for hierarchy levels
 * - **Object Icons**: Type-specific icons for different object types (mesh, light, camera)
 * - **Visibility Indicators**: Visual state for object visibility and selection status
 * - **Naming Display**: Clear object names with inline editing capabilities
 * 
 * ### Interactive Object Selection
 * - **Click Selection**: Single-click selection with immediate viewport feedback
 * - **Multi-Selection**: Ctrl+click and Shift+click for multiple object selection
 * - **Range Selection**: Shift+click for selecting object ranges in hierarchy
 * - **Viewport Sync**: Two-way synchronization with 3D viewport selection
 * - **Selection Feedback**: Visual highlighting of selected objects in tree
 * 
 * ### Object Management Operations
 * - **Inline Renaming**: Double-click or F2 for immediate object renaming
 * - **Drag-and-Drop**: Intuitive parent-child relationship modification
 * - **Context Operations**: Right-click menu for delete, duplicate, properties
 * - **Keyboard Shortcuts**: Professional keyboard shortcuts for common operations
 * - **Undo Integration**: All operations properly integrated with undo system
 * 
 * ## Professional Workflow Integration
 * The panel integrates seamlessly with professional 3D modeling workflows:
 * 
 * ### Industry Compatibility
 * - **Maya-Style Interface**: Familiar outliner patterns matching Autodesk Maya
 * - **Blender Integration**: Compatible with Blender outliner conventions
 * - **3ds Max Patterns**: Scene explorer functionality similar to 3ds Max
 * - **Standard Shortcuts**: Industry-standard keyboard shortcuts and conventions
 * 
 * ### Object Type Support
 * - **Mesh Objects**: Geometric objects with appropriate mesh icons and properties
 * - **Light Objects**: Lighting objects with light-specific icons and controls
 * - **Camera Objects**: Camera objects with camera icons and viewport integration
 * - **Group Objects**: Grouping objects for scene organization and batch operations
 * - **Custom Objects**: Support for application-specific object types
 * 
 * ### Advanced Features
 * - **Object Filtering**: Search and filter objects by name, type, or properties
 * - **Bulk Operations**: Select and operate on multiple objects simultaneously
 * - **Object Properties**: Quick access to object properties and settings
 * - **Layer Management**: Object organization through layer assignments
 * - **Import/Export**: Hierarchy-aware import and export operations
 * 
 * ## User Interface Design
 * The panel provides a professional and intuitive user interface:
 * 
 * ### Visual Design
 * - **Professional Theming**: Consistent with application theme system
 * - **Clear Typography**: Readable object names and hierarchy indicators
 * - **Intuitive Icons**: Clear visual representation of different object types
 * - **Proper Spacing**: Optimized spacing for dense scene hierarchies
 * - **Focus Indicators**: Clear visual feedback for focus and selection states
 * 
 * ### Interaction Patterns
 * - **Standard Tree Navigation**: Familiar tree widget interaction patterns
 * - **Context-Sensitive Menus**: Appropriate menu options based on selection
 * - **Drag-and-Drop Feedback**: Visual feedback during hierarchy reorganization
 * - **Hover Effects**: Subtle hover feedback for interactive elements
 * - **Keyboard Navigation**: Full keyboard navigation support for accessibility
 * 
 * ## Performance Optimization
 * The panel is optimized for large scene management:
 * 
 * ### Efficient Updates
 * - **Selective Refresh**: Only affected tree items are updated when changes occur
 * - **Lazy Loading**: Large hierarchies are loaded incrementally for responsiveness
 * - **Change Detection**: Intelligent detection of scene changes requiring UI updates
 * - **Batch Operations**: Multiple changes are batched for efficient processing
 * 
 * ### Memory Management
 * - **Smart Pointers**: Automatic memory management for scene references
 * - **Weak References**: Prevents circular references in object hierarchies
 * - **Resource Cleanup**: Proper cleanup when scenes are changed or closed
 * - **Cache Optimization**: Efficient caching of frequently accessed object data
 * 
 * ## Signal Integration
 * The panel provides comprehensive signal integration for application coordination:
 * 
 * ### Selection Coordination
 * - **Entity Selection**: Signals when objects are selected in the hierarchy
 * - **Viewport Sync**: Receives selection changes from 3D viewport
 * - **Multi-Component**: Coordinates with property panels and tool systems
 * - **Real-Time Updates**: Immediate reflection of selection changes
 * 
 * ### Object Management Signals
 * - **Rename Operations**: Signals when objects are renamed through the interface
 * - **Delete Operations**: Coordinates object deletion with scene management
 * - **Creation Events**: Handles new object creation and hierarchy placement
 * - **Property Changes**: Signals when object properties are modified
 * 
 * ## Usage Examples
 * ```cpp
 * // Create and configure scene hierarchy panel
 * auto hierarchyPanel = new SceneHierarchyPanel(parent);
 * hierarchyPanel->setScene(sceneManager->getCurrentScene());
 * 
 * // Connect to selection system
 * connect(hierarchyPanel, &SceneHierarchyPanel::entitySelected,
 *         viewportManager, &ViewportManager::selectEntity);
 * 
 * // Connect to object management
 * connect(hierarchyPanel, &SceneHierarchyPanel::entityDeleted,
 *         sceneManager, &SceneManager::deleteEntity);
 * connect(hierarchyPanel, &SceneHierarchyPanel::entityRenamed,
 *         sceneManager, &SceneManager::renameEntity);
 * 
 * // Update hierarchy when scene changes
 * connect(sceneManager, &SceneManager::sceneChanged,
 *         hierarchyPanel, &SceneHierarchyPanel::refreshHierarchy);
 * ```
 * 
 * ## Professional Benefits
 * - **Efficient Scene Navigation**: Quick access to any object in complex scenes
 * - **Organized Workflows**: Hierarchical organization supports clean modeling practices
 * - **Familiar Interface**: Industry-standard patterns reduce learning curve
 * - **Batch Operations**: Efficient management of multiple objects simultaneously
 * - **Visual Clarity**: Clear representation of scene structure and relationships
 * 
 * @see Entity for individual scene object representation
 * @see Scene for scene management and coordination
 * @see PropertiesPanel for object property editing
 * @see UIManager for panel integration and coordination
 * 
 * @ingroup UI
 * @ingroup Panels
 * @ingroup SceneManagement
 */
class SceneHierarchyPanel : public QWidget {
    Q_OBJECT

public:
    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct scene hierarchy panel
     * 
     * Creates a new scene hierarchy panel with professional tree widget
     * and context menu system ready for 3D scene management workflows.
     * 
     * @param parent Qt parent widget for memory management
     * 
     * @post Panel is created with empty hierarchy tree
     * @post Context menu system is initialized and functional
     * @post UI layout is configured for professional appearance
     * @post Signal connections are established for interaction
     * 
     * @note Call setScene() to populate the hierarchy with scene objects
     * @note Panel adapts to application theme automatically
     */
    explicit SceneHierarchyPanel(QWidget* parent = nullptr);
    
    /**
     * @brief Destructor with automatic cleanup
     * 
     * Properly cleans up all UI components and disconnects from
     * scene references for clean shutdown.
     */
    ~SceneHierarchyPanel() = default;

    // ========================================================================
    // Scene Management and Configuration
    // ========================================================================
    
    /**
     * @brief Set scene for hierarchy display
     * 
     * Assigns the 3D scene to be displayed in the hierarchy panel,
     * populating the tree with all scene objects and establishing
     * connections for real-time updates.
     * 
     * @param scene Shared pointer to the scene to display
     * 
     * @post Hierarchy tree is populated with scene objects
     * @post Scene change monitoring is established
     * @post Object selection synchronization is active
     * @post Panel is ready for interactive scene management
     * 
     * @note Previous scene connections are automatically cleaned up
     * @note Null scene pointer clears the hierarchy display
     * @note Scene changes trigger automatic hierarchy refresh
     */
    void setScene(std::shared_ptr<rude::Scene> scene);
    
    /**
     * @brief Get currently displayed scene
     * 
     * @return Shared pointer to currently displayed scene, or nullptr if none
     * 
     * @note Use to check if panel has an active scene assigned
     * @note Returned pointer may be null if no scene is set
     */
    std::shared_ptr<rude::Scene> getScene() const { return m_scene; }

    // ========================================================================
    // Hierarchy Update and Refresh
    // ========================================================================
    
    /**
     * @brief Refresh complete hierarchy display
     * 
     * Rebuilds the entire hierarchy tree from the current scene state,
     * ensuring all objects, relationships, and properties are current.
     * 
     * @post Hierarchy tree reflects current scene state
     * @post Object names, hierarchy, and properties are updated
     * @post Selection states are preserved where possible
     * @post Visual appearance matches current object states
     * 
     * @note Call when scene structure has changed significantly
     * @note Preserves expansion states of tree branches where possible
     * @note Expensive operation for large scenes, use selectively
     */
    void refreshHierarchy();
    
    /**
     * @brief Update specific object in hierarchy
     * 
     * Updates the hierarchy display for a specific object without
     * rebuilding the entire tree, providing efficient updates.
     * 
     * @param entity Pointer to entity that has been modified
     * 
     * @post Specified entity's display is updated in hierarchy
     * @post Object name, icon, and properties reflect current state
     * @post Hierarchy structure is updated if parent changed
     * 
     * @note More efficient than full refresh for single object changes
     * @note Automatically handles hierarchy changes and reparenting
     * @note No effect if entity is not currently displayed
     */
    void updateEntity(rude::Entity* entity);
    
    /**
     * @brief Add new entity to hierarchy display
     * 
     * Adds a newly created entity to the hierarchy tree at the
     * appropriate location based on its parent-child relationships.
     * 
     * @param entity Pointer to new entity to add to hierarchy
     * 
     * @post Entity appears in hierarchy tree at correct location
     * @post Entity is automatically selected in hierarchy
     * @post Tree expansion is adjusted to show new entity
     * 
     * @note Entity must already exist in the scene
     * @note Placement determined by entity's parent relationship
     * @note New entity becomes immediately available for interaction
     */
    void addEntity(rude::Entity* entity);
    
    /**
     * @brief Remove entity from hierarchy display
     * 
     * Removes the specified entity from the hierarchy tree display,
     * typically called when an entity is deleted from the scene.
     * 
     * @param entity Pointer to entity to remove from hierarchy
     * 
     * @post Entity is removed from hierarchy tree
     * @post Selection is updated if removed entity was selected
     * @post Child entities are handled according to hierarchy rules
     * 
     * @note Does not delete entity from scene, only from display
     * @note Child handling depends on scene hierarchy implementation
     * @note No effect if entity is not currently displayed
     */
    void removeEntity(rude::Entity* entity);

    // ========================================================================
    // Selection Management
    // ========================================================================
    
    /**
     * @brief Set selected entities in hierarchy
     * 
     * Updates the hierarchy selection to match the provided list of
     * entities, typically called from viewport selection changes.
     * 
     * @param entities List of entities to select in hierarchy
     * 
     * @post Specified entities are selected in hierarchy tree
     * @post Visual selection highlighting is updated
     * @post Selection change signals are suppressed to prevent loops
     * 
     * @note Clears existing selection before applying new selection
     * @note Entities not in hierarchy are ignored silently
     * @note Expands tree branches to show selected entities if needed
     */
    void setSelectedEntities(const QList<rude::Entity*>& entities);
    
    /**
     * @brief Get currently selected entities from hierarchy
     * 
     * @return List of entities currently selected in the hierarchy tree
     * 
     * @note Selection may be empty if no objects are selected
     * @note Use for synchronization with other components
     * @note Selection order matches tree order, not selection order
     */
    QList<rude::Entity*> getSelectedEntities() const;
    
    /**
     * @brief Clear all selection in hierarchy
     * 
     * Removes all selection from the hierarchy tree, typically used
     * when clearing selection throughout the application.
     * 
     * @post No entities are selected in hierarchy tree
     * @post Selection highlighting is cleared
     * @post entitySelected() signal is emitted with nullptr
     * 
     * @note Triggers selection change signals for coordination
     * @note Provides clean selection state for new operations
     */
    void clearSelection();

signals:
    // ========================================================================
    // Object Selection Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when entity selection changes
     * 
     * Emitted when the user selects objects in the hierarchy tree,
     * allowing other components to synchronize their selection state.
     * 
     * @param entity Pointer to newly selected entity, or nullptr if cleared
     * 
     * @note Connect to viewport manager for selection synchronization
     * @note Multiple selections may emit multiple signals
     * @note nullptr indicates selection was cleared
     */
    void entitySelected(rude::Entity* entity);
    
    /**
     * @brief Signal emitted when multiple entities are selected
     * 
     * Emitted when multiple entities are selected simultaneously,
     * providing efficient batch selection notification.
     * 
     * @param entities List of all currently selected entities
     * 
     * @note Use for batch operations and multi-selection workflows
     * @note Emitted for both user selection and programmatic selection
     * @note Empty list indicates all selection was cleared
     */
    void entitiesSelected(const QList<rude::Entity*>& entities);

    // ========================================================================
    // Object Management Signals
    // ========================================================================
    
    /**
     * @brief Signal emitted when entity is renamed through hierarchy
     * 
     * Emitted when the user renames an object through inline editing
     * or context menu operations in the hierarchy.
     * 
     * @param entity Pointer to entity that was renamed
     * @param newName New name assigned to the entity
     * 
     * @note Connect to scene manager for name change processing
     * @note Name validation should be performed by receiving component
     * @note Operation is undoable through command system integration
     */
    void entityRenamed(rude::Entity* entity, const QString& newName);
    
    /**
     * @brief Signal emitted when entity deletion is requested
     * 
     * Emitted when the user requests entity deletion through context
     * menu or keyboard shortcuts in the hierarchy.
     * 
     * @param entity Pointer to entity requested for deletion
     * 
     * @note Connect to scene manager for deletion processing
     * @note Deletion confirmation may be handled by receiving component
     * @note Operation should be undoable through command system
     */
    void entityDeleted(rude::Entity* entity);
    
    /**
     * @brief Signal emitted when entity duplication is requested
     * 
     * Emitted when the user requests entity duplication through context
     * menu operations in the hierarchy.
     * 
     * @param entity Pointer to entity to be duplicated
     * 
     * @note Connect to scene manager for duplication processing
     * @note New entity typically created near original entity
     * @note Duplication includes all entity properties and relationships
     */
    void entityDuplicated(rude::Entity* entity);
    
    /**
     * @brief Signal emitted when entity hierarchy changes
     * 
     * Emitted when the user modifies object parent-child relationships
     * through drag-and-drop operations in the hierarchy.
     * 
     * @param entity Pointer to entity whose parent changed
     * @param newParent Pointer to new parent entity, or nullptr for root
     * 
     * @note Connect to scene manager for hierarchy update processing
     * @note Hierarchy changes affect transformation inheritance
     * @note Operation should be undoable through command system
     */
    void entityReparented(rude::Entity* entity, rude::Entity* newParent);

private slots:
    // ========================================================================
    // Internal Event Handlers
    // ========================================================================
    
    /**
     * @brief Handle tree widget selection changes
     * 
     * Internal slot called when tree widget selection changes,
     * coordinating selection state and emitting appropriate signals.
     * 
     * @post Selection signals are emitted for external coordination
     * @post Internal selection state is updated
     * @post Multi-selection is handled appropriately
     * 
     * @note Connected to tree widget selection changed signal
     * @note Handles both single and multiple selection scenarios
     */
    void onItemSelectionChanged();
    
    /**
     * @brief Handle tree item editing completion
     * 
     * Internal slot called when inline editing of tree items is completed,
     * processing name changes and validation.
     * 
     * @param item Tree widget item that was edited
     * @param column Column that was edited (typically 0 for name)
     * 
     * @post Entity rename signal is emitted if name changed
     * @post Invalid names are rejected with user feedback
     * @post Item display is updated to reflect final name
     * 
     * @note Connected to tree widget itemChanged signal
     * @note Performs basic name validation before accepting changes
     */
    void onItemChanged(QTreeWidgetItem* item, int column);
    
    /**
     * @brief Handle context menu requests
     * 
     * Internal slot called when user requests context menu through
     * right-click operations on tree items.
     * 
     * @param pos Position where context menu was requested
     * 
     * @post Context menu is displayed at appropriate location
     * @post Menu content reflects current selection and capabilities
     * @post Menu actions are enabled/disabled based on context
     * 
     * @note Connected to tree widget customContextMenuRequested signal
     * @note Menu content adapts to selected entity types and states
     */
    void onCustomContextMenu(const QPoint& pos);
    
    /**
     * @brief Handle delete operation from context menu
     * 
     * Internal slot called when user selects delete from context menu,
     * processing entity deletion requests.
     * 
     * @post entityDeleted signal is emitted for selected entities
     * @post Deletion confirmation may be requested from user
     * @post Multiple selected entities are handled in batch
     * 
     * @note Connected to delete action triggered signal
     * @note Processes all currently selected entities
     */
    void onDeleteSelectedObject();
    
    /**
     * @brief Handle rename operation from context menu
     * 
     * Internal slot called when user selects rename from context menu,
     * initiating inline editing mode for selected entity.
     * 
     * @post Inline editing mode is activated for selected item
     * @post Current entity name is selected for easy replacement
     * @post Editing completion is handled by onItemChanged
     * 
     * @note Connected to rename action triggered signal
     * @note Only processes single selected entity
     */
    void onRenameSelectedObject();
    
    /**
     * @brief Handle duplicate operation from context menu
     * 
     * Internal slot called when user selects duplicate from context menu,
     * processing entity duplication requests.
     * 
     * @post entityDuplicated signal is emitted for selected entities
     * @post Multiple selected entities are duplicated in batch
     * @post New entities are positioned appropriately in hierarchy
     * 
     * @note Connected to duplicate action triggered signal
     * @note Processes all currently selected entities
     */
    void onDuplicateSelectedObject();

private:
    // ========================================================================
    // Scene and Data Management
    // ========================================================================
    
    /// Shared pointer to currently displayed scene
    std::shared_ptr<rude::Scene> m_scene;

    // ========================================================================
    // UI Component References
    // ========================================================================
    
    /// Main vertical layout for panel organization
    QVBoxLayout* m_layout;
    
    /// Tree widget displaying the hierarchy structure
    QTreeWidget* m_treeWidget;
    
    /// Horizontal layout for action buttons
    QHBoxLayout* m_buttonLayout;
    
    /// Delete button for quick object deletion
    QPushButton* m_deleteButton;
    
    // ========================================================================
    // Context Menu System
    // ========================================================================
    
    /// Context menu for right-click operations
    QMenu* m_contextMenu;
    
    /// Delete action for context menu
    QAction* m_deleteAction;
    
    /// Rename action for context menu
    QAction* m_renameAction;
    
    /// Duplicate action for context menu
    QAction* m_duplicateAction;
    
    /// Add child action for creating child objects
    QAction* m_addChildAction;
    
    /// Properties action for object property access
    QAction* m_propertiesAction;

    // ========================================================================
    // Internal UI Setup and Configuration
    // ========================================================================
    
    /**
     * @brief Set up complete user interface layout
     * 
     * Internal method that creates and configures all UI components
     * including tree widget, buttons, and layout management.
     * 
     * @post Complete UI is created and configured
     * @post Tree widget is configured for hierarchy display
     * @post Action buttons are created and positioned
     * @post Layout is optimized for professional appearance
     * 
     * @note Called automatically during construction
     * @note Creates all necessary UI components and styling
     */
    void setupUI();
    
    /**
     * @brief Set up context menu system
     * 
     * Internal method that creates the context menu and all associated
     * actions for right-click operations on hierarchy items.
     * 
     * @post Context menu is created with all standard actions
     * @post Action shortcuts and icons are configured
     * @post Menu is ready for display on right-click events
     * 
     * @note Called automatically during UI setup
     * @note Actions are context-sensitive based on selection
     */
    void setupContextMenu();
    
    /**
     * @brief Connect all internal signals and slots
     * 
     * Internal method that establishes signal-slot connections between
     * UI components and internal handler methods.
     * 
     * @post All UI signals are connected to appropriate handlers
     * @post Tree widget events are properly routed
     * @post Context menu actions are functional
     * 
     * @note Called automatically during construction
     * @note Ensures all UI interactions are properly handled
     */
    void connectSignals();

    // ========================================================================
    // Tree Management and Entity Mapping
    // ========================================================================
    
    /**
     * @brief Find tree item corresponding to entity ID
     * 
     * Internal method that searches the tree hierarchy to find the
     * tree widget item representing the specified entity.
     * 
     * @param entityId Unique identifier of entity to find
     * 
     * @return Pointer to tree widget item, or nullptr if not found
     * 
     * @note Performs recursive search through entire tree hierarchy
     * @note Returns first matching item if multiple items exist (shouldn't happen)
     * @note Efficient lookup for entity-to-item mapping
     */
    QTreeWidgetItem* findItemByEntityId(unsigned int entityId);
    
    /**
     * @brief Get entity associated with tree widget item
     * 
     * Internal method that retrieves the entity pointer stored
     * in the specified tree widget item's user data.
     * 
     * @param item Tree widget item to query for entity
     * 
     * @return Pointer to associated entity, or nullptr if none
     * 
     * @note Entity pointers are stored in item user data during creation
     * @note Returns nullptr for invalid items or items without entity data
     * @note Essential for mapping tree interactions to scene objects
     */
    rude::Entity* getEntityFromItem(QTreeWidgetItem* item);
    
    /**
     * @brief Update tree item to reflect entity state
     * 
     * Internal method that updates a tree widget item's appearance
     * and properties to match the current state of its associated entity.
     * 
     * @param item Tree widget item to update
     * @param entity Entity whose state should be reflected
     * 
     * @post Item text reflects entity name
     * @post Item icon reflects entity type
     * @post Item appearance reflects entity state (visibility, selection, etc.)
     * 
     * @note Called when entity properties change
     * @note Handles all visual aspects of entity representation
     * @note Updates are optimized to only change what's necessary
     */
    void updateItemFromEntity(QTreeWidgetItem* item, rude::Entity* entity);
    
    /**
     * @brief Create tree item for entity with proper configuration
     * 
     * Internal method that creates a new tree widget item for the
     * specified entity with appropriate styling and data.
     * 
     * @param entity Entity to create item for
     * @param parent Parent tree item, or nullptr for root level
     * 
     * @return Pointer to newly created tree widget item
     * 
     * @post Tree item is created with entity name and icon
     * @post Entity pointer is stored in item user data
     * @post Item is configured for hierarchy interaction
     * 
     * @note Item is not automatically added to tree, caller must position it
     * @note All entity properties are reflected in initial item state
     * @note Item supports inline editing and context menu operations
     */
    QTreeWidgetItem* createItemForEntity(rude::Entity* entity, QTreeWidgetItem* parent = nullptr);
    
    /**
     * @brief Build complete tree hierarchy from scene
     * 
     * Internal method that builds the complete tree widget hierarchy
     * from the current scene's object structure.
     * 
     * @post Tree widget contains all scene entities in proper hierarchy
     * @post Parent-child relationships are correctly represented
     * @post Tree is ready for user interaction
     * 
     * @note Called during scene assignment and hierarchy refresh
     * @note Clears existing tree content before building new hierarchy
     * @note Preserves expansion states where possible during refresh
     */
    void buildTreeFromScene();
    
    /**
     * @brief Recursively build tree branch for entity and children
     * 
     * Internal method that recursively creates tree items for an entity
     * and all of its child entities.
     * 
     * @param entity Root entity for this branch
     * @param parentItem Parent tree item to add new items to
     * 
     * @post Entity and all children are represented in tree
     * @post Hierarchy relationships are properly maintained
     * @post Tree branch is ready for interaction
     * 
     * @note Handles arbitrary depth hierarchies through recursion
     * @note Maintains entity relationships during tree construction
     * @note Called by buildTreeFromScene for complete hierarchy building
     */
    void buildTreeBranch(rude::Entity* entity, QTreeWidgetItem* parentItem);
    
    /**
     * @brief Apply theme-appropriate styling to tree widget
     * 
     * Internal method that applies current theme styling to the tree
     * widget and all its items for consistent appearance.
     * 
     * @post Tree widget appearance matches current application theme
     * @post Item colors and styling are theme-appropriate
     * @post Professional appearance is maintained
     * 
     * @note Called when theme changes or during initialization
     * @note Adapts to both light and dark theme modes
     * @note Ensures accessibility and readability in all themes
     */
    void applyThemeToTree();
    
    /**
     * @brief Update context menu based on current selection
     * 
     * Internal method that enables or disables context menu actions
     * based on the current selection state and entity capabilities.
     * 
     * @post Context menu actions reflect current selection capabilities
     * @post Inappropriate actions are disabled or hidden
     * @post Menu provides only relevant operations for selection
     * 
     * @note Called before displaying context menu
     * @note Action availability depends on selected entity types
     * @note Prevents invalid operations through UI design
     */
    void updateContextMenuForSelection();
};
