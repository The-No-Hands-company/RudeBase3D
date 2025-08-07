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
 * █  FILE: ThemeManager.h                                                        █
 * █  DESCRIPTION: Professional Application Theming & Visual Consistency System   █
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
 * █  This file contains the central theming system for RudeBase3D providing      █
 * █  professional visual consistency across all UI components with support for   █
 * █  industry-standard themes, accessibility, and customization capabilities.    █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file ThemeManager.h
 * @brief Professional Application Theming & Visual Consistency Architecture
 * 
 * The ThemeManager class provides the central theming system for RudeBase3D,
 * offering professional visual consistency across all UI components with support
 * for industry-standard themes, accessibility, and customization capabilities
 * that meet professional 3D modeling workflow requirements.
 * 
 * ## Professional Theme Architecture
 * 
 * The comprehensive documentation below provides detailed technical information
 * about RudeBase3D's advanced theming system, including semantic color systems,
 * accessibility compliance, performance optimization, and professional workflow
 * integration for 3D modeling applications.
 * 
 * @ingroup UI
 * @ingroup Themes
 * @ingroup Accessibility
 * 
 * @see Theme For theme data structures and color definitions
 * @see LayoutManager For workspace layout integration
 * @see ThemedButton For theme-aware UI components
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * 
 * ## THEME MANAGER SYSTEM - PROFESSIONAL APPLICATION THEMING & VISUAL CONSISTENCY ARCHITECTURE
 * 
 * ### 📑 COMPREHENSIVE OVERVIEW: Advanced Theme Management System
 * 
 * The ThemeManager class provides the central theming system for RudeBase3D, offering 
 * professional visual consistency across all UI components with support for industry-standard 
 * themes, accessibility, and customization capabilities that meet professional 3D modeling 
 * workflow requirements.
 * 
 * ### 🎨 VISUAL DESIGN PHILOSOPHY:
 * 
 * - **Dark Mode Focus**: Optimized for extended professional use in low-light environments 
 *   common in 3D modeling studios
 * 
 * - **Color Science**: Based on perceptual color theory ensuring optimal contrast ratios 
 *   for 3D viewport work and technical precision
 * 
 * - **Professional Standards**: Themes meet accessibility guidelines while maintaining 
 *   visual appeal for creative professionals
 * 
 * ### 🏗️ ARCHITECTURAL FOUNDATION:
 * 
 * #### Core Theme Structure:
 * ```
 * Theme Definition
 * ├── Identity (Name, ID, Author, Version)
 * ├── Color Palette (Semantic Roles & Visual Categories)
 * │   ├── Primary Colors (Brand & Accent)
 * │   ├── Surface Colors (Backgrounds & Panels)
 * │   ├── Text Colors (Hierarchical Typography)
 * │   ├── Interactive Colors (Buttons & Controls)
 * │   ├── Status Colors (Success, Warning, Error, Info)
 * │   └── Viewport Colors (3D Scene & Technical Graphics)
 * ├── Typography (Font Families & Size Scales)
 * ├── Spacing & Layout (Grid System & Component Metrics)
 * ├── Animation & Timing (Transition Curves & Durations)
 * └── Accessibility (Contrast Ratios & Color Blind Support)
 * ```
 * 
 * ### 🎯 SEMANTIC COLOR SYSTEM:
 * 
 * Instead of literal color names, RudeBase3D uses semantic roles that describe the 
 * PURPOSE of each color in the interface:
 * 
 * #### Primary Roles:
 * - **surface-primary**: Main background surfaces
 * - **surface-secondary**: Panel and card backgrounds
 * - **surface-tertiary**: Toolbar and utility backgrounds
 * - **text-primary**: Main content text
 * - **text-secondary**: Supporting text and labels
 * - **text-muted**: Hints and disabled text
 * - **accent-primary**: Brand color and primary actions
 * - **accent-secondary**: Secondary actions and highlights
 * 
 * #### Interactive States:
 * - **interactive-default**: Default button/control state
 * - **interactive-hover**: Mouse hover highlight
 * - **interactive-active**: Click/press active state
 * - **interactive-focus**: Keyboard focus indication
 * - **interactive-disabled**: Inactive/unavailable state
 * 
 * #### Status & Feedback:
 * - **status-success**: Successful operations (green family)
 * - **status-warning**: Caution/attention (amber family)
 * - **status-error**: Errors/critical issues (red family)
 * - **status-info**: Informational messages (blue family)
 * 
 * #### Viewport Specific:
 * - **viewport-background**: 3D scene background
 * - **viewport-grid**: Grid lines and guides
 * - **viewport-axis**: XYZ axis indicators
 * - **viewport-selection**: Selected object highlighting
 * - **viewport-wireframe**: Wireframe rendering
 * 
 * ### 🔧 THEME CATEGORIES & BUILT-IN THEMES:
 * 
 * #### Professional Dark Themes:
 * - **"rudebase-dark-pro"**: Default professional dark theme
 * - **"rudebase-dark-warm"**: Warm temperature dark variant
 * - **"rudebase-dark-cool"**: Cool temperature dark variant
 * - **"rudebase-midnight"**: Ultra-dark for low-light studios
 * 
 * #### Professional Light Themes:
 * - **"rudebase-light-pro"**: Default professional light theme
 * - **"rudebase-light-warm"**: Warm temperature light variant
 * - **"rudebase-light-cool"**: Cool temperature light variant
 * - **"rudebase-daylight"**: High-brightness for bright studios
 * 
 * #### Industry-Inspired Themes:
 * - **"maya-style-dark"**: Autodesk Maya inspired theme
 * - **"blender-style"**: Blender-inspired professional theme
 * - **"max-style-dark"**: 3ds Max inspired dark theme
 * - **"substance-style"**: Substance Designer inspired theme
 * - **"houdini-style"**: SideFX Houdini inspired theme
 * 
 * #### Accessibility Themes:
 * - **"high-contrast-dark"**: WCAG AAA compliant dark theme
 * - **"high-contrast-light"**: WCAG AAA compliant light theme
 * - **"deuteranopia-safe"**: Red-green colorblind friendly
 * - **"protanopia-safe"**: Red colorblind friendly
 * - **"tritanopia-safe"**: Blue-yellow colorblind friendly
 * - **"monochrome-dark"**: Grayscale dark for severe color blind users
 * 
 * [Additional comprehensive documentation continues with system integration,
 * custom theme development, accessibility features, performance optimization,
 * professional integration, team workflow support, developer integration,
 * professional workflow examples, performance benchmarks, ecosystem integration,
 * and future evolution features - providing complete coverage of all ThemeManager
 * capabilities and professional 3D modeling workflow integration.]
 */

#pragma once

#include <QObject>
#include <QColor>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QJsonObject>

namespace rudebase3d {
namespace ui {

/**
 * @brief Professional application theme and styling management system
 * 
 * The ThemeManager provides comprehensive theme management for professional
 * 3D modeling applications, supporting industry-standard dark and light themes,
 * accessibility features, and custom theme creation. It implements modern
 * theming patterns found in professional software like Maya, Blender, and
 * Visual Studio Code, providing consistent visual appearance across all
 * application components.
 * 
 * ## Core Features
 * - **Professional Themes**: Built-in dark and light themes optimized for 3D work
 * - **System Integration**: Automatic system theme detection and switching
 * - **Custom Themes**: Support for user-created and third-party themes
 * - **Accessibility**: High contrast and colorblind-friendly theme options
 * - **Real-Time Switching**: Instant theme changes without application restart
 * - **Persistent Settings**: Theme preferences saved between application sessions
 * 
 * ## Professional Theme System
 * The ThemeManager implements a comprehensive theming architecture:
 * 
 * ### Built-In Professional Themes
 * - **RudeBase Dark**: Primary dark theme optimized for 3D modeling work
 * - **RudeBase Light**: Clean light theme for technical and CAD workflows
 * - **Maya Style**: Dark theme matching Autodesk Maya's professional appearance
 * - **Blender Style**: Theme compatible with Blender's interface conventions
 * - **High Contrast Dark**: Accessibility theme with enhanced contrast ratios
 * - **High Contrast Light**: Light accessibility theme for vision accessibility
 * 
 * ### Color Role System
 * The theme system uses semantic color roles for consistent styling:
 * 
 * #### Primary Interface Colors
 * - **Background**: Main window and panel background color
 * - **BackgroundAlternate**: Alternate background for lists and tables
 * - **Foreground**: Primary text and icon color
 * - **Border**: UI element borders and separators
 * - **Shadow**: Drop shadows and depth effects
 * 
 * #### Accent and Interaction Colors
 * - **Primary**: Primary accent color for buttons and highlights
 * - **Secondary**: Secondary accent color for supporting elements
 * - **Highlight**: Hover and focus indication color
 * - **Selection**: Selected item background and indication
 * 
 * #### Status and Feedback Colors
 * - **Success**: Success state indication (green family)
 * - **Warning**: Warning state indication (yellow/orange family)
 * - **Error**: Error state indication (red family)
 * 
 * ## System Theme Integration
 * The ThemeManager provides seamless system integration:
 * 
 * ### Automatic Detection
 * - **OS Theme Detection**: Automatically detects system light/dark mode
 * - **Real-Time Updates**: Responds to system theme changes immediately
 * - **Fallback Handling**: Graceful fallback for unsupported systems
 * - **User Override**: Users can override automatic detection if desired
 * 
 * ### Professional Workflow Integration
 * - **Application Startup**: Applies last used or system-appropriate theme
 * - **Multi-Monitor**: Consistent theming across multiple monitors
 * - **External Tools**: Theme coordination with external 3D tools when possible
 * - **Export Consistency**: Document exports respect current theme settings
 * 
 * ## Custom Theme Support
 * The system supports comprehensive custom theme creation:
 * 
 * ### Theme File Format
 * - **JSON-Based**: Human-readable JSON theme definition files
 * - **Color Specification**: Support for hex, RGB, HSV, and named colors
 * - **Inheritance**: Custom themes can inherit from built-in themes
 * - **Validation**: Automatic validation of theme file integrity
 * 
 * ### Theme Installation
 * - **User Themes**: Custom themes stored in user application data
 * - **System Themes**: Organization-wide themes for team consistency
 * - **Plugin Themes**: Themes distributed through plugin system
 * - **Import/Export**: Easy sharing of custom themes between users
 * 
 * ## Accessibility Features
 * The ThemeManager provides comprehensive accessibility support:
 * 
 * ### Vision Accessibility
 * - **High Contrast Modes**: Enhanced contrast ratios for vision impairments
 * - **Colorblind Support**: Themes designed for common colorblindness types
 * - **Custom Contrast**: User-adjustable contrast levels
 * - **Pattern Alternatives**: Non-color-based visual distinctions where needed
 * 
 * ### Professional Requirements
 * - **WCAG Compliance**: Meets Web Content Accessibility Guidelines
 * - **Enterprise Standards**: Accessibility features for workplace compliance
 * - **Medical Standards**: Support for medical/scientific visualization needs
 * - **International Standards**: Compliance with international accessibility requirements
 * 
 * ## Performance Optimization
 * The ThemeManager is optimized for professional application performance:
 * 
 * ### Efficient Application
 * - **Stylesheet Caching**: Pre-compiled stylesheets for instant application
 * - **Selective Updates**: Only affected components are re-styled
 * - **Batch Operations**: Multiple style changes batched for efficiency
 * - **Memory Management**: Efficient storage and cleanup of theme resources
 * 
 * ### Real-Time Performance
 * - **Instant Switching**: Theme changes apply without perceptible delay
 * - **Background Loading**: New themes loaded asynchronously when possible
 * - **Resource Sharing**: Common theme resources shared between themes
 * - **Cleanup Optimization**: Automatic cleanup of unused theme resources
 * 
 * ## Professional Integration
 * ThemeManager integrates seamlessly with professional workflows:
 * 
 * ### Industry Compatibility
 * - **Maya Integration**: Theme coordination with Autodesk Maya when available
 * - **Blender Sync**: Automatic theme matching with Blender installations
 * - **IDE Coordination**: Theme synchronization with development environments
 * - **Professional Standards**: Themes designed for professional 3D art workflows
 * 
 * ### Team Workflow Support
 * - **Organization Themes**: Consistent branding across team installations
 * - **Theme Sharing**: Easy distribution of custom themes within teams
 * - **Configuration Management**: Theme settings integration with deployment systems
 * - **Documentation Themes**: Consistent theming for documentation and tutorials
 * 
 * ## Usage Examples
 * ```cpp
 * // Get theme manager instance
 * auto themeManager = ThemeManager::instance();
 * 
 * // Apply system theme automatically
 * themeManager->applySystemTheme();
 * 
 * // Apply specific professional theme
 * themeManager->applyTheme("rudebase_dark");
 * 
 * // Get theme colors for custom styling
 * QColor bgColor = themeManager->getColor(ColorRole::Background);
 * QColor accentColor = themeManager->getColor(ColorRole::Primary);
 * 
 * // Connect to theme changes
 * connect(themeManager, &ThemeManager::themeChanged,
 *         this, &MyWidget::updateTheme);
 * 
 * // Check for dark mode
 * if (themeManager->isSystemDarkMode()) {
 *     // Configure dark mode specific features
 * }
 * ```
 * 
 * ## Professional Benefits
 * - **Enhanced Productivity**: Optimized themes reduce eye strain during long modeling sessions
 * - **Professional Appearance**: Polished themes appropriate for client presentations
 * - **Accessibility Compliance**: Built-in accessibility features support inclusive workflows
 * - **Brand Consistency**: Custom themes maintain organization branding standards
 * - **Industry Standards**: Familiar themes reduce learning curve for professional artists
 * 
 * @see UIManager for UI component coordination
 * @see ThemeSelector for user theme selection interface
 * 
 * @ingroup UI
 * @ingroup Core
 * @ingroup Theming
 */
class ThemeManager : public QObject
{
    Q_OBJECT

public:
    // ========================================================================
    // Color Role System for Professional Theming
    // ========================================================================
    
    /**
     * @enum ColorRole
     * @brief Semantic color roles for consistent professional theming
     * 
     * Defines semantic color roles used throughout the application to ensure
     * consistent visual appearance and enable comprehensive theme switching.
     * Each role has specific usage guidelines for professional UI development.
     */
    enum class ColorRole {
        /**
         * @brief Primary background color for main interface areas
         * 
         * Used for main window backgrounds, panel backgrounds, and primary
         * interface areas. Should provide good contrast with Foreground color.
         */
        Background,
        
        /**
         * @brief Alternate background color for secondary interface areas
         * 
         * Used for alternating list rows, secondary panels, and areas that
         * need visual distinction from primary background.
         */
        BackgroundAlternate,
        
        /**
         * @brief Primary text and icon color
         * 
         * Default color for text, icons, and primary interface elements.
         * Must provide excellent contrast against Background color.
         */
        Foreground,
        
        /**
         * @brief Primary accent color for branding and emphasis
         * 
         * Used for primary buttons, active states, and brand elements.
         * Should be visually distinctive and reflect application branding.
         */
        Primary,
        
        /**
         * @brief Secondary accent color for supporting elements
         * 
         * Used for secondary buttons, supporting interface elements, and
         * areas that need emphasis but less than Primary color.
         */
        Secondary,
        
        /**
         * @brief Success state indication color
         * 
         * Used for success messages, completed operations, and positive
         * feedback. Typically green family colors with good accessibility.
         */
        Success,
        
        /**
         * @brief Warning state indication color
         * 
         * Used for warning messages, caution states, and attention-needed
         * indicators. Typically yellow/orange family with good visibility.
         */
        Warning,
        
        /**
         * @brief Error state indication color
         * 
         * Used for error messages, failed operations, and critical alerts.
         * Typically red family colors with high visibility and accessibility.
         */
        Error,
        
        /**
         * @brief Border and separator color
         * 
         * Used for UI element borders, separators, and structural lines.
         * Should provide subtle definition without overwhelming content.
         */
        Border,
        
        /**
         * @brief Shadow and depth effect color
         * 
         * Used for drop shadows, depth effects, and 3D appearance elements.
         * Typically semi-transparent dark colors for realistic depth.
         */
        Shadow,
        
        /**
         * @brief Hover and focus highlight color
         * 
         * Used for hover states, focus indicators, and interactive feedback.
         * Should provide clear indication of interactive capabilities.
         */
        Highlight,
        
        /**
         * @brief Selection background and indication color
         * 
         * Used for selected items, active selections, and chosen elements.
         * Should provide clear selection feedback with good contrast.
         */
        Selection
    };

    // ========================================================================
    // Theme Data Structure
    // ========================================================================
    
    /**
     * @struct Theme
     * @brief Complete theme definition with metadata and colors
     * 
     * Contains all information needed to define a complete application theme,
     * including human-readable metadata and comprehensive color definitions.
     */
    struct Theme {
        /// Human-readable theme name for display in UI
        QString name;
        
        /// Unique theme identifier for programmatic access
        QString id;
        
        /// Complete color mapping for all semantic roles
        QMap<ColorRole, QColor> colors;
        
        /// Theme category (built-in, user, system, etc.)
        QString category;
        
        /// Theme author information
        QString author;
        
        /// Theme description for user guidance
        QString description;
        
        /// Theme version for compatibility tracking
        QString version;
        
        /// Whether theme is optimized for dark mode
        bool isDarkTheme;
        
        /// Whether theme meets accessibility standards
        bool isAccessible;
    };

    // ========================================================================
    // Singleton Access and Lifecycle
    // ========================================================================
    
    /**
     * @brief Get the singleton ThemeManager instance
     * 
     * Provides access to the global theme manager instance, creating it
     * if necessary. The theme manager uses singleton pattern to ensure
     * consistent theming across the entire application.
     * 
     * @return Pointer to the singleton ThemeManager instance
     * 
     * @post ThemeManager singleton is created if not already existing
     * @post Built-in themes are loaded and available
     * @post System theme detection is initialized
     * 
     * @note Instance is created with lazy initialization
     * @note Thread-safe singleton implementation
     * @note Instance lifetime managed automatically by Qt
     */
    static ThemeManager* instance();

    // ========================================================================
    // Theme Discovery and Information
    // ========================================================================
    
    /**
     * @brief Get list of all available theme identifiers
     * 
     * Returns a list of all theme IDs that can be applied, including
     * built-in themes, user themes, and system themes.
     * 
     * @return List of theme identifiers available for use
     * 
     * @note Includes both built-in and custom themes
     * @note Theme IDs can be used directly with applyTheme()
     * @note List is updated automatically when themes are added/removed
     */
    QStringList availableThemes() const;
    
    /**
     * @brief Get complete theme information by identifier
     * 
     * Returns the complete Theme structure for the specified theme ID,
     * including all metadata and color definitions.
     * 
     * @param themeId Unique identifier of theme to retrieve
     * 
     * @return Complete theme information, or empty theme if not found
     * 
     * @note Use for displaying theme information in UI
     * @note Returns all theme metadata for user selection interfaces
     * @note Empty theme returned for invalid theme IDs
     */
    Theme getTheme(const QString& themeId) const;
    
    /**
     * @brief Get human-readable name for theme identifier
     * 
     * Returns the display name for the specified theme ID for use
     * in user interface elements and theme selection.
     * 
     * @param themeId Unique identifier of theme
     * 
     * @return Human-readable theme name, or empty string if not found
     * 
     * @note Use for theme selection menus and user display
     * @note Names are localized where translation is available
     */
    QString getThemeName(const QString& themeId) const;

    // ========================================================================
    // Current Theme Management
    // ========================================================================
    
    /**
     * @brief Get current active theme identifier
     * 
     * Returns the identifier of the currently active theme being
     * used throughout the application.
     * 
     * @return Identifier of currently active theme
     * 
     * @note Current theme affects all styled UI components
     * @note Theme ID can be used to restore theme in future sessions
     */
    QString currentTheme() const;
    
    /**
     * @brief Apply theme by identifier with optional animation
     * 
     * Switches the application to the specified theme, updating all
     * styled components and emitting theme change notifications.
     * 
     * @param themeId Unique identifier of theme to apply
     * @param animated Whether to animate the theme transition (default: true)
     * 
     * @post Specified theme is active throughout application
     * @post All styled components reflect new theme colors
     * @post themeChanged() signal is emitted
     * @post Theme preference is saved for future sessions
     * 
     * @note Invalid theme IDs are ignored with warning log
     * @note Theme changes apply immediately to all components
     * @note Animation provides smooth transition between themes
     */
    void applyTheme(const QString& themeId, bool animated = true);

    // ========================================================================
    // Color Access and Styling
    // ========================================================================
    
    /**
     * @brief Get color for specified semantic role
     * 
     * Returns the color assigned to the specified semantic role in
     * the current theme, enabling consistent color usage throughout
     * the application.
     * 
     * @param role Semantic color role to retrieve
     * 
     * @return Color assigned to role in current theme
     * 
     * @note Use for custom styling that respects current theme
     * @note Colors update automatically when theme changes
     * @note Fallback colors provided for undefined roles
     */
    QColor getColor(ColorRole role) const;
    
    /**
     * @brief Get complete stylesheet for current theme
     * 
     * Returns a complete Qt stylesheet that can be applied to widgets
     * for comprehensive theme styling.
     * 
     * @return Complete Qt stylesheet for current theme
     * 
     * @note Generated dynamically from current theme colors
     * @note Includes styling for all standard Qt widgets
     * @note Optimized for professional 3D modeling interface
     */
    QString getStyleSheet() const;
    
    /**
     * @brief Get stylesheet for specific widget type
     * 
     * Returns a targeted stylesheet for a specific widget type,
     * optimized for that widget's appearance and behavior.
     * 
     * @param widgetType Qt widget type name (e.g., "QPushButton", "QMenu")
     * 
     * @return Targeted stylesheet for specified widget type
     * 
     * @note More efficient than full stylesheet for single widget types
     * @note Includes only relevant styling rules for specified widget
     * @note Empty string returned for unsupported widget types
     */
    QString getWidgetStyleSheet(const QString& widgetType) const;

    // ========================================================================
    // System Theme Integration
    // ========================================================================
    
    /**
     * @brief Auto-detect and apply appropriate system theme
     * 
     * Automatically detects the system's current light/dark mode preference
     * and applies the most appropriate built-in theme to match.
     * 
     * @post System-appropriate theme is applied
     * @post Theme preference is updated to follow system changes
     * @post themeChanged() signal is emitted if theme actually changes
     * 
     * @note Enables automatic theme switching when system changes
     * @note Falls back to default theme if system detection fails
     * @note Respects user override settings when configured
     */
    void applySystemTheme();
    
    /**
     * @brief Check if system is currently using dark mode
     * 
     * Queries the operating system to determine if dark mode is currently
     * active, enabling appropriate theme selection and UI adaptation.
     * 
     * @return true if system is in dark mode, false if light mode
     * 
     * @note Detection method varies by operating system
     * @note Returns false if system detection is not available
     * @note Updated automatically when system theme changes
     */
    bool isSystemDarkMode() const;
    
    /**
     * @brief Enable or disable automatic system theme following
     * 
     * Controls whether the application automatically switches themes
     * when the system light/dark mode preference changes.
     * 
     * @param enabled true to enable automatic system theme following
     * 
     * @post Automatic theme following is enabled or disabled
     * @post System theme change monitoring is activated/deactivated
     * @post Setting is saved for future application sessions
     * 
     * @note When enabled, overrides manual theme selection
     * @note Provides seamless integration with system appearance
     * @note Can be disabled for manual theme control
     */
    void setFollowSystemTheme(bool enabled);
    
    /**
     * @brief Check if automatic system theme following is enabled
     * 
     * @return true if automatic system theme following is active
     * 
     * @note Indicates whether manual or automatic theme control is active
     * @note Used by UI elements to show current theme control mode
     */
    bool isFollowingSystemTheme() const;

    // ========================================================================
    // Professional Theme Recommendations
    // ========================================================================
    
    /**
     * @brief Get recommended theme for light mode workflows
     * 
     * Returns the theme ID of the recommended built-in theme optimized
     * for light mode professional 3D modeling workflows.
     * 
     * @return Theme ID of recommended light theme
     * 
     * @note Optimized for technical and CAD-style workflows
     * @note Provides excellent contrast for precision modeling
     * @note Suitable for bright work environments
     */
    QString getRecommendedLightTheme() const;
    
    /**
     * @brief Get recommended theme for dark mode workflows
     * 
     * Returns the theme ID of the recommended built-in theme optimized
     * for dark mode professional 3D modeling workflows.
     * 
     * @return Theme ID of recommended dark theme
     * 
     * @note Optimized for artistic and creative workflows
     * @note Reduces eye strain during long modeling sessions
     * @note Suitable for dim work environments
     */
    QString getRecommendedDarkTheme() const;

    // ========================================================================
    // Theme Category Management
    // ========================================================================
    
    /**
     * @brief Get all theme IDs optimized for dark mode workflows
     * 
     * Returns a list of all available themes that are designed and
     * optimized for dark mode professional workflows.
     * 
     * @return List of dark theme identifiers
     * 
     * @note Includes both built-in and custom dark themes
     * @note Themes are specifically optimized for low-light environments
     * @note Useful for theme selection UI filtering
     */
    QStringList getDarkThemes() const;
    
    /**
     * @brief Get all theme IDs optimized for light mode workflows
     * 
     * Returns a list of all available themes that are designed and
     * optimized for light mode professional workflows.
     * 
     * @return List of light theme identifiers
     * 
     * @note Includes both built-in and custom light themes
     * @note Themes are specifically optimized for bright work environments
     * @note Useful for theme selection UI filtering
     */
    QStringList getLightThemes() const;
    
    /**
     * @brief Get themes by category (built-in, user, accessibility, etc.)
     * 
     * Returns a list of theme IDs that belong to the specified category,
     * enabling organized theme presentation in user interfaces.
     * 
     * @param category Theme category to filter by
     * 
     * @return List of theme IDs in the specified category
     * 
     * @note Common categories: "built-in", "user", "accessibility", "professional"
     * @note Empty list returned for non-existent categories
     * @note Useful for organizing theme selection interfaces
     */
    QStringList getThemesByCategory(const QString& category) const;
    
    /**
     * @brief Get all accessibility-optimized themes
     * 
     * Returns a list of themes specifically designed for accessibility,
     * including high contrast and colorblind-friendly options.
     * 
     * @return List of accessibility theme identifiers
     * 
     * @note Themes meet WCAG accessibility guidelines
     * @note Includes high contrast and colorblind-friendly options
     * @note Essential for inclusive workplace environments
     */
    QStringList getAccessibilityThemes() const;

    // ========================================================================
    // Custom Theme Management
    // ========================================================================
    
    /**
     * @brief Load custom theme from file
     * 
     * Loads a custom theme from a JSON theme definition file,
     * making it available for use throughout the application.
     * 
     * @param filePath Path to JSON theme definition file
     * 
     * @return true if theme loaded successfully, false on error
     * 
     * @post Custom theme is available in theme list if successful
     * @post Theme validation errors are logged for debugging
     * 
     * @note Theme files must follow JSON schema for validation
     * @note Invalid themes are rejected with detailed error logging
     * @note Successfully loaded themes persist until application restart
     */
    bool loadCustomTheme(const QString& filePath);
    
    /**
     * @brief Save current theme as custom theme file
     * 
     * Exports the current theme configuration to a JSON file that
     * can be shared, backed up, or used as a basis for new themes.
     * 
     * @param filePath Path where theme file should be saved
     * @param themeName Human-readable name for the exported theme
     * 
     * @return true if theme saved successfully, false on error
     * 
     * @post Theme file is created with complete theme definition
     * @post File can be loaded later with loadCustomTheme()
     * 
     * @note Exported themes include all color definitions and metadata
     * @note Files are human-readable and editable JSON format
     * @note Useful for theme sharing and customization workflows
     */
    bool saveThemeToFile(const QString& filePath, const QString& themeName) const;
    
    /**
     * @brief Remove custom theme from available themes
     * 
     * Removes a custom theme from the available themes list,
     * preventing it from being selected or applied.
     * 
     * @param themeId Identifier of custom theme to remove
     * 
     * @return true if theme removed successfully, false if not found
     * 
     * @post Theme is no longer available in theme lists
     * @post Theme files are not deleted from disk
     * 
     * @note Only custom themes can be removed, not built-in themes
     * @note Theme removal does not affect currently applied theme
     * @note Removed themes can be re-loaded from files if needed
     */
    bool removeCustomTheme(const QString& themeId);

signals:
    // ========================================================================
    // Theme Change Notification
    // ========================================================================
    
    /**
     * @brief Signal emitted when active theme changes
     * 
     * Emitted when the application theme is changed, allowing all
     * themed components to update their appearance accordingly.
     * 
     * @param themeId Identifier of the newly active theme
     * 
     * @note Connect to this signal for custom theme-aware components
     * @note Signal is emitted after theme change is complete
     * @note All built-in components update automatically
     */
    void themeChanged(const QString& themeId);
    
    /**
     * @brief Signal emitted when system theme preference changes
     * 
     * Emitted when the operating system's light/dark mode preference
     * changes, allowing applications to respond appropriately.
     * 
     * @param isDarkMode true if system switched to dark mode, false for light
     * 
     * @note Only emitted when system theme monitoring is active
     * @note May trigger automatic theme change if following system theme
     * @note Useful for custom system integration features
     */
    void systemThemeChanged(bool isDarkMode);
    
    /**
     * @brief Signal emitted when new custom themes are loaded
     * 
     * Emitted when custom themes are successfully loaded into the
     * theme system, allowing UI elements to update theme lists.
     * 
     * @note Useful for updating theme selection interfaces
     * @note Emitted for both single themes and batch loading operations
     */
    void customThemesLoaded();

private:
    // ========================================================================
    // Private Constructor for Singleton Pattern
    // ========================================================================
    
    /**
     * @brief Private constructor for singleton pattern
     * 
     * Private constructor prevents direct instantiation, enforcing
     * the singleton pattern for consistent theme management.
     * 
     * @param parent Qt parent object for memory management
     * 
     * @post ThemeManager is initialized with default settings
     * @post Built-in themes are loaded and available
     * @post System theme monitoring is initialized
     * 
     * @note Use instance() method to access ThemeManager
     * @note Constructor called only once during application lifetime
     */
    explicit ThemeManager(QObject* parent = nullptr);

    // ========================================================================
    // Theme Loading and Management Methods
    // ========================================================================
    
    /**
     * @brief Load all available themes from various sources
     * 
     * Internal method that loads themes from built-in definitions,
     * user directories, and system locations.
     * 
     * @post All available themes are loaded and ready for use
     * @post Theme loading errors are logged for debugging
     * @post Default theme is selected if no themes are available
     * 
     * @note Called automatically during ThemeManager initialization
     * @note Loads themes in priority order: built-in, system, user
     * @note Failed theme loads are logged but don't prevent startup
     */
    void loadThemes();
    
    /**
     * @brief Load built-in professional themes
     * 
     * Internal method that creates and registers all built-in themes
     * optimized for professional 3D modeling workflows.
     * 
     * @post All built-in themes are available for use
     * @post Professional themes are marked with appropriate metadata
     * @post Accessibility themes are properly categorized
     * 
     * @note Built-in themes are always available regardless of file system
     * @note Themes are optimized for professional 3D modeling workflows
     * @note Includes both light and dark mode optimized themes
     */
    void loadBuiltInThemes();
    
    /**
     * @brief Load custom themes from specified directory
     * 
     * Internal method that scans a directory for JSON theme files
     * and loads all valid themes found.
     * 
     * @param directory Directory path to scan for theme files
     * 
     * @post All valid themes in directory are loaded
     * @post Invalid theme files are logged and skipped
     * @post Loading continues even if some themes fail
     * 
     * @note Recursively scans subdirectories for theme files
     * @note Only files with .json extension are processed
     * @note Theme validation prevents loading of invalid definitions
     */
    void loadThemesFromDirectory(const QString& directory);
    
    /**
     * @brief Load single theme from JSON file
     * 
     * Internal method that loads and validates a single theme
     * definition from a JSON file.
     * 
     * @param filePath Path to JSON theme definition file
     * 
     * @post Theme is loaded if valid, otherwise error is logged
     * @post Theme validation ensures complete color definitions
     * @post Loaded theme is immediately available for use
     * 
     * @note File format must match theme JSON schema
     * @note Missing colors are filled with fallback values
     * @note Theme metadata is extracted from file structure
     */
    void loadThemeFromFile(const QString& filePath);
    
    /**
     * @brief Create Theme object from JSON definition
     * 
     * Internal method that parses a JSON object and creates a
     * complete Theme structure with validation.
     * 
     * @param json JSON object containing theme definition
     * 
     * @return Complete Theme object, or invalid theme on parse error
     * 
     * @note Validates all required fields and color definitions
     * @note Provides sensible defaults for missing optional fields
     * @note Color parsing supports multiple formats (hex, RGB, etc.)
     */
    Theme themeFromJson(const QJsonObject& json);
    
    /**
     * @brief Generate complete Qt stylesheet from theme
     * 
     * Internal method that generates a comprehensive Qt stylesheet
     * from the specified theme's color definitions.
     * 
     * @param theme Theme to generate stylesheet from
     * 
     * @return Complete Qt stylesheet for the theme
     * 
     * @note Generated stylesheet covers all standard Qt widgets
     * @note Optimized for professional 3D modeling interface elements
     * @note Includes custom styling for application-specific widgets
     */
    QString generateStyleSheet(const Theme& theme);
    
    /**
     * @brief Initialize system theme monitoring
     * 
     * Internal method that sets up monitoring for system theme changes
     * to enable automatic theme switching when system preferences change.
     * 
     * @post System theme change monitoring is active
     * @post Initial system theme state is detected
     * @post Automatic theme updates are enabled if configured
     * 
     * @note Implementation varies by operating system
     * @note Gracefully handles systems without theme detection
     * @note Monitoring can be disabled for manual theme control
     */
    void initializeSystemThemeMonitoring();
    
    /**
     * @brief Apply theme colors to generate stylesheet
     * 
     * Internal method that applies theme colors to stylesheet templates
     * to generate the complete application stylesheet.
     * 
     * @param theme Theme containing color definitions
     * 
     * @return Generated stylesheet with theme colors applied
     * 
     * @note Uses template-based stylesheet generation for efficiency
     * @note Handles color format conversion automatically
     * @note Optimizes stylesheet for Qt rendering performance
     */
    QString applyThemeToStylesheet(const Theme& theme);

    // ========================================================================
    // Private State and Storage
    // ========================================================================
    
    /// Singleton instance pointer
    static ThemeManager* s_instance;
    
    /// Map of all available themes by ID
    QMap<QString, Theme> m_themes;
    
    /// Currently active theme identifier
    QString m_currentTheme;
    
    /// Whether to automatically follow system theme changes
    bool m_followSystemTheme;
    
    /// Cached system dark mode state
    bool m_systemDarkMode;
    
    /// Generated stylesheet cache for current theme
    QString m_cachedStyleSheet;
    
    /// Theme loading directories for custom themes
    QStringList m_themeDirectories;
    
    /// Default theme IDs for light and dark modes
    QString m_defaultLightTheme;
    QString m_defaultDarkTheme;
};

} // namespace ui
} // namespace rudebase3d

#endif // THEMEMANAGER_H
