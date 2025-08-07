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
 * █  FILE: BaseComponent.h                                                       █
 * █  DESCRIPTION: Foundation Base Class for All UI Components                    █
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
 * █  This file contains the BaseComponent class - the foundational base class    █
 * █  for all custom UI components providing theme integration, common            █
 * █  functionality, and consistent behavior across the application interface.    █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

#pragma once

#include <QWidget>
#include <QString>
#include <QEvent>
#include <QColor>
#include <QPalette>
#include "../core/ThemeManager.h"

namespace rudebase3d {
namespace ui {

/**
 * @brief Foundation base class for all UI components in the RudeBase3D framework
 * 
 * The BaseComponent class serves as the foundational base class for all custom UI
 * components within the RudeBase3D 3D modeling application. It provides essential
 * functionality including comprehensive theme integration, state management,
 * accessibility support, and consistent behavioral patterns across the entire
 * user interface.
 * 
 * ## Core Functionality
 * 
 * ### Theme Integration System
 * Deep integration with the RudeBase3D theme management system:
 * - **Automatic Theme Updates**: Components automatically respond to theme changes
 * - **Color Role Management**: Access to semantic color roles (primary, secondary, etc.)
 * - **Style Sheet Generation**: Automatic generation of theme-aware CSS styling
 * - **Dark/Light Mode Support**: Seamless switching between light and dark themes
 * - **Custom Theme Properties**: Support for component-specific theme customizations
 * 
 * ### Component State Management
 * Comprehensive visual state system for professional UI feedback:
 * - **Normal State**: Default component appearance for standard interaction
 * - **Hover State**: Visual feedback when mouse cursor hovers over component
 * - **Focus State**: Keyboard navigation and accessibility focus indication
 * - **Pressed State**: Active interaction feedback for button-like components
 * - **Disabled State**: Clear visual indication when component is non-interactive
 * - **Loading State**: Progress indication for asynchronous operations
 * - **Error State**: Visual error indication with optional error messaging
 * 
 * ### Accessibility Foundation
 * Built-in accessibility support for professional applications:
 * - **Screen Reader Support**: Proper ARIA attributes and semantic markup
 * - **Keyboard Navigation**: Full keyboard accessibility with proper tab order
 * - **High Contrast Support**: Automatic adaptation to system accessibility settings
 * - **Tooltip Management**: Context-sensitive tooltips with state-aware content
 * - **Focus Management**: Proper focus handling for complex UI interactions
 * 
 * ## Professional Features
 * 
 * ### Consistent Behavior Patterns
 * Standardized behavior across all derived components:
 * - **Event Handling**: Consistent mouse, keyboard, and focus event processing
 * - **Animation Support**: Smooth state transitions and hover effects
 * - **Size Adaptation**: Responsive sizing and layout adaptation
 * - **Performance Optimization**: Efficient rendering and update strategies
 * 
 * ### Error Handling and Feedback
 * Professional error indication and user feedback:
 * - **Visual Error States**: Clear error indication without disrupting workflow
 * - **Error Messaging**: Optional error messages with proper accessibility
 * - **Recovery Mechanisms**: Built-in error recovery and state restoration
 * - **Validation Support**: Foundation for input validation and feedback
 * 
 * ### Developer-Friendly Architecture
 * Designed for easy extension and customization:
 * - **Virtual Hook Methods**: Override points for custom behavior
 * - **Protected Utilities**: Helper methods for common UI operations
 * - **Signal System**: Qt signal/slot integration for loose coupling
 * - **Style Customization**: Easy style sheet override and customization
 * 
 * ## Architecture Integration
 * 
 * ### Theme Manager Integration
 * Seamless integration with the application theme system:
 * ```cpp
 * // Components automatically receive theme updates
 * connect(ThemeManager::instance(), &ThemeManager::themeChanged,
 *         this, &BaseComponent::updateTheme);
 * 
 * // Access theme colors with semantic roles
 * QColor primaryColor = getThemeColor(ThemeManager::ColorRole::Primary);
 * QColor backgroundolor = getThemeColor(ThemeManager::ColorRole::Background);
 * ```
 * 
 * ### UI Manager Coordination
 * Works with the UI management system for consistent experience:
 * - **Layout Management**: Proper integration with layout management systems
 * - **Panel Coordination**: Consistent behavior in dockable panels
 * - **Menu Integration**: Proper menu and toolbar component behavior
 * - **Modal Dialog Support**: Consistent behavior in modal contexts
 * 
 * ## Usage Examples
 * 
 * ### Creating Custom Components
 * ```cpp
 * class CustomButton : public BaseComponent {
 *     Q_OBJECT
 * 
 * public:
 *     CustomButton(QWidget* parent = nullptr) : BaseComponent(parent) {
 *         // Component automatically has theme integration
 *         setComponentState(ComponentState::Normal);
 *     }
 * 
 * protected:
 *     void onStateChanged(ComponentState oldState, ComponentState newState) override {
 *         BaseComponent::onStateChanged(oldState, newState);
 *         // Custom state change handling
 *         updateCustomAppearance();
 *     }
 * 
 *     QString generateBaseStyleSheet() const override {
 *         QString baseStyle = BaseComponent::generateBaseStyleSheet();
 *         // Add custom styling
 *         return baseStyle + "CustomButton { border-radius: 4px; }";
 *     }
 * };
 * ```
 * 
 * ### State Management
 * ```cpp
 * // Set component states based on interaction
 * component->setComponentState(BaseComponent::ComponentState::Hover);
 * component->setComponentState(BaseComponent::ComponentState::Pressed);
 * component->setComponentState(BaseComponent::ComponentState::Disabled);
 * 
 * // Show error state with message
 * component->showErrorState("Invalid input value");
 * 
 * // Clear error and return to normal
 * component->clearErrorState();
 * ```
 * 
 * ### Theme Integration
 * ```cpp
 * // Components automatically update when theme changes
 * void CustomComponent::updateTheme() {
 *     QColor bg = getThemeColor(ThemeManager::ColorRole::Background);
 *     QColor fg = getThemeColor(ThemeManager::ColorRole::Foreground);
 *     
 *     // Apply theme colors to custom elements
 *     setStyleSheet(QString("background-color: %1; color: %2;")
 *                   .arg(bg.name()).arg(fg.name()));
 * }
 * ```
 * 
 * ## Performance Considerations
 * 
 * ### Efficient Updates
 * Optimized for smooth user interface performance:
 * - **Minimal Repaints**: State changes trigger only necessary visual updates
 * - **Cached Styling**: Style sheets are cached and reused when possible
 * - **Event Batching**: Multiple rapid state changes are batched for efficiency
 * - **Lazy Evaluation**: Complex operations are deferred until actually needed
 * 
 * ### Memory Management
 * Responsible resource management for long-running applications:
 * - **RAII Principles**: Automatic cleanup of resources using C++ RAII
 * - **Qt Parent System**: Proper integration with Qt's parent-child ownership
 * - **Theme Resource Sharing**: Shared theme resources to minimize memory usage
 * - **Event Handler Cleanup**: Automatic cleanup of event handlers and connections
 * 
 * @ingroup UI
 * @ingroup Components
 * @ingroup ThemeSystem
 * 
 * @see ThemeManager For theme integration and color management
 * @see UIManager For overall UI coordination and management
 * @see LayoutManager For layout and workspace integration
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class BaseComponent : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Comprehensive visual state enumeration for professional UI feedback
     * 
     * Defines all possible visual states that a UI component can display to provide
     * clear feedback to users about the component's current status and interaction
     * capabilities. These states are designed to match professional application
     * standards and accessibility requirements.
     */
    enum class ComponentState {
        Normal,    ///< Default state - component is ready for interaction
        Hover,     ///< Mouse cursor is hovering over the component
        Focus,     ///< Component has keyboard focus for accessibility
        Pressed,   ///< Component is currently being pressed/activated
        Disabled,  ///< Component is disabled and non-interactive
        Loading,   ///< Component is performing an asynchronous operation
        Error      ///< Component is in an error state with visual indication
    };

    /**
     * @brief Constructs a new BaseComponent with theme integration
     * 
     * Creates a new base component with automatic theme integration, proper
     * state initialization, and accessibility support. The component is
     * immediately ready for use and will respond to theme changes.
     * 
     * @param parent Parent widget for proper Qt ownership hierarchy. Can be
     *               nullptr for top-level components.
     * 
     * The constructor:
     * - Initializes component state to Normal
     * - Connects to theme manager for automatic theme updates
     * - Sets up accessibility attributes
     * - Configures base styling and appearance
     * - Enables proper event handling for state management
     */
    explicit BaseComponent(QWidget* parent = nullptr);

    /**
     * @brief Sets the visual state of the component
     * 
     * Changes the component's visual state, triggering appropriate visual
     * updates and accessibility notifications. State changes are animated
     * smoothly and trigger the onStateChanged() virtual method for custom
     * handling in derived classes.
     * 
     * @param state New component state to apply. Must be a valid ComponentState
     *              value. Invalid states are ignored.
     * 
     * The method:
     * - Validates the new state
     * - Calls onStateChanged() for custom handling
     * - Updates visual appearance automatically
     * - Emits stateChanged() signal for external observers
     * - Updates accessibility properties as needed
     * - Refreshes tooltips based on the new state
     * 
     * @see ComponentState For available state values
     * @see onStateChanged() For custom state change handling
     */
    void setComponentState(ComponentState state);

    /**
     * @brief Gets the current visual state of the component
     * 
     * Returns the component's current visual state, which can be used for
     * conditional logic, state validation, or external state monitoring.
     * 
     * @return Current ComponentState value
     * 
     * @note The returned state always reflects the actual visual state and
     *       may differ from the last state set if the component has internal
     *       state management logic.
     */
    ComponentState componentState() const;

    /**
     * @brief Sets the disabled state with proper state management integration
     * 
     * Overrides the standard QWidget::setDisabled() method to ensure that
     * the component's state management system is properly updated when the
     * widget is enabled or disabled.
     * 
     * @param disabled True to disable the component, false to enable it
     * 
     * When disabled:
     * - Component state is set to ComponentState::Disabled
     * - Visual appearance updates to show disabled state
     * - Interaction events are ignored
     * - Accessibility properties are updated
     * - Disabled tooltip is shown if configured
     * 
     * When enabled:
     * - Component state is restored to ComponentState::Normal
     * - Full interactivity is restored
     * - Standard tooltip is restored
     */
    void setDisabled(bool disabled);

    /**
     * @brief Shows a loading state with visual feedback
     * 
     * Transitions the component to loading state, providing visual indication
     * that an asynchronous operation is in progress. This prevents user
     * interaction while maintaining clear feedback about the component's status.
     * 
     * The loading state:
     * - Sets component state to ComponentState::Loading
     * - May show loading animations or progress indicators
     * - Disables user interaction temporarily
     * - Updates accessibility properties for screen readers
     * - Can be customized in derived classes through onStateChanged()
     * 
     * @see hideLoadingState() To return to normal state
     * @see ComponentState::Loading For the loading state definition
     */
    void showLoadingState();
    
    /**
     * @brief Hides the loading state and returns to normal operation
     * 
     * Transitions the component back to normal state from loading state,
     * restoring full interactivity and normal visual appearance.
     * 
     * @see showLoadingState() To enter loading state
     */
    void hideLoadingState();
    
    /**
     * @brief Shows an error state with optional error messaging
     * 
     * Transitions the component to error state, providing clear visual
     * indication that an error has occurred. Optionally displays an
     * error message through tooltips or other feedback mechanisms.
     * 
     * @param errorMessage Optional error message to display. If provided,
     *                     the message is shown through the tooltip system
     *                     or other appropriate feedback mechanisms.
     * 
     * The error state:
     * - Sets component state to ComponentState::Error
     * - Applies error visual styling (typically red indicators)
     * - Shows error message if provided
     * - Updates accessibility properties for error indication
     * - Maintains component interactivity unless specifically disabled
     * 
     * @see clearErrorState() To return to normal state
     * @see setErrorTooltip() To configure error message display
     */
    void showErrorState(const QString& errorMessage = QString());
    
    /**
     * @brief Clears the error state and returns to normal operation
     * 
     * Transitions the component back to normal state from error state,
     * clearing any error messages and restoring normal visual appearance.
     * 
     * @see showErrorState() To enter error state
     */
    void clearErrorState();

    /**
     * @brief Sets the base tooltip shown in normal state
     * 
     * Configures the tooltip text that is displayed when the component is
     * in normal state and the user hovers over it. This tooltip provides
     * helpful information about the component's function.
     * 
     * @param tooltip Tooltip text to display. Can be empty to disable tooltips.
     * 
     * @see setDisabledTooltip() For disabled state tooltips
     * @see setErrorTooltip() For error state tooltips
     */
    void setBaseTooltip(const QString& tooltip);
    
    /**
     * @brief Sets the tooltip shown when component is disabled
     * 
     * Configures the tooltip text displayed when the component is disabled.
     * This should typically explain why the component is disabled and what
     * conditions need to be met to enable it.
     * 
     * @param tooltip Tooltip text for disabled state
     */
    void setDisabledTooltip(const QString& tooltip);
    
    /**
     * @brief Sets the tooltip shown when component is in error state
     * 
     * Configures the tooltip text displayed when the component is in error
     * state. This should provide information about the error and potential
     * solutions or recovery actions.
     * 
     * @param tooltip Tooltip text for error state
     */
    void setErrorTooltip(const QString& tooltip);

protected:
    /**
     * @brief Applies the current theme settings to the component
     * 
     * Virtual method that applies theme settings to the component's visual
     * appearance. This method is called automatically when the theme changes
     * and can be overridden in derived classes to implement custom theming.
     * 
     * The base implementation:
     * - Generates base style sheet using generateBaseStyleSheet()
     * - Applies the style sheet to the widget
     * - Updates color properties from theme manager
     * - Triggers visual refresh as needed
     * 
     * Override this method in derived classes to implement custom theme
     * application logic while still calling the base implementation.
     */
    virtual void applyTheme();

    /**
     * @brief Generates the base CSS style sheet for the component
     * 
     * Virtual method that creates the CSS style sheet for the component
     * based on the current theme and component state. This method is used
     * by applyTheme() and can be overridden to customize appearance.
     * 
     * @return QString containing the CSS style sheet for the component
     * 
     * The base implementation includes:
     * - State-specific color schemes
     * - Theme-aware background and foreground colors
     * - Accessibility-compliant contrast ratios
     * - Professional styling for hover and focus states
     * 
     * Derived classes should call the base implementation and append
     * their custom styling:
     * ```cpp
     * QString CustomComponent::generateBaseStyleSheet() const {
     *     QString base = BaseComponent::generateBaseStyleSheet();
     *     return base + "CustomComponent { border-radius: 4px; }";
     * }
     * ```
     */
    virtual QString generateBaseStyleSheet() const;

    /**
     * @brief Called when component state changes for custom handling
     * 
     * Virtual hook method that is called whenever the component's state
     * changes, allowing derived classes to implement custom state-specific
     * behavior and appearance updates.
     * 
     * @param oldState The previous state of the component
     * @param newState The new state the component is transitioning to
     * 
     * This method is called before the visual appearance is updated,
     * allowing derived classes to perform custom logic based on state
     * transitions. Common uses include:
     * - Starting or stopping animations
     * - Updating custom visual elements
     * - Managing child component states
     * - Triggering state-specific operations
     * 
     * Always call the base implementation when overriding:
     * ```cpp
     * void CustomComponent::onStateChanged(ComponentState oldState, ComponentState newState) {
     *     BaseComponent::onStateChanged(oldState, newState);
     *     // Custom state change logic here
     * }
     * ```
     */
    virtual void onStateChanged(ComponentState oldState, ComponentState newState);

    /**
     * @brief Updates the visual appearance based on current state
     * 
     * Virtual method that updates the component's visual appearance to match
     * its current state. This is called automatically after state changes
     * and can be overridden to implement custom appearance logic.
     * 
     * The base implementation:
     * - Applies theme-based styling
     * - Updates color schemes based on state
     * - Refreshes accessibility properties
     * - Triggers repainting as needed
     */
    virtual void updateAppearance();

    /**
     * @brief Gets a theme color for the specified semantic role
     * 
     * Utility method that retrieves colors from the theme manager using
     * semantic color roles. This ensures consistent color usage across
     * all components and automatic adaptation to theme changes.
     * 
     * @param role Semantic color role from the theme system
     * @return QColor object for the specified role in the current theme
     * 
     * Common color roles include:
     * - Primary: Main accent color for important elements
     * - Secondary: Secondary accent color for supporting elements
     * - Background: Background color for the component
     * - Foreground: Text and icon color
     * - Error: Color for error states and warnings
     * - Success: Color for success states and confirmations
     * 
     * @see ThemeManager::ColorRole For available color roles
     */
    QColor getThemeColor(ThemeManager::ColorRole role) const;

    /**
     * @brief Handles events for state management and accessibility
     * 
     * Overrides QWidget::event() to provide automatic state management
     * based on user interaction events. Handles hover, focus, and other
     * events that should trigger state changes.
     * 
     * @param event The event to process
     * @return True if the event was handled, false otherwise
     * 
     * The method automatically handles:
     * - Mouse enter/leave events for hover state
     * - Focus in/out events for focus state
     * - Mouse press/release events for pressed state
     * - Enable/disable events for disabled state
     * 
     * Derived classes can override this method but should call the base
     * implementation to maintain proper state management.
     */
    bool event(QEvent* event) override;

signals:
    /**
     * @brief Signal emitted when the component's state changes
     * 
     * This signal is emitted whenever the component transitions to a new
     * state, allowing external objects to respond to state changes. The
     * signal is emitted after the state change is complete and visual
     * updates have been applied.
     * 
     * @param state The new state of the component
     * 
     * Connect to this signal to implement custom logic based on component
     * state changes:
     * ```cpp
     * connect(component, &BaseComponent::stateChanged,
     *         this, [](BaseComponent::ComponentState state) {
     *     if (state == BaseComponent::ComponentState::Error) {
     *         // Handle error state
     *     }
     * });
     * ```
     */
    void stateChanged(ComponentState state);

private slots:
    /**
     * @brief Handles theme change notifications from theme manager
     * 
     * Private slot that responds to theme changes by applying the new
     * theme to the component. This ensures automatic theme updates
     * without manual intervention.
     * 
     * @param themeId Identifier of the new theme being applied
     */
    void onThemeChanged(const QString& themeId);

private:
    /**
     * @brief Updates the tooltip based on current component state
     * 
     * Internal method that updates the component's tooltip to match
     * the current state, using the appropriate tooltip text for the
     * current state (base, disabled, or error).
     */
    void updateTooltip();

    // === Private Member Variables ===
    
    /**
     * @brief Pointer to the theme manager for color and style access
     */
    ThemeManager* m_themeManager = nullptr;
    
    /**
     * @brief Current visual state of the component
     */
    ComponentState m_state = ComponentState::Normal;
    
    /**
     * @brief Tooltip text for normal state
     */
    QString m_baseTooltip;
    
    /**
     * @brief Tooltip text for disabled state
     */
    QString m_disabledTooltip;
    
    /**
     * @brief Tooltip text for error state
     */
    QString m_errorTooltip;
};

} // namespace ui
} // namespace rudebase3d

// Make ComponentState available for Qt's meta-object system
Q_DECLARE_METATYPE(rudebase3d::ui::BaseComponent::ComponentState)
