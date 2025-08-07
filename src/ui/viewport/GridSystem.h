//==============================================================================
// RudeBase3D Engine - GridSystem.h
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

/**
 * @file GridSystem.h
 * @brief Professional reference grid system for 3D modeling workflows
 * 
 * This file contains the GridSystem class which provides industry-standard
 * reference grid rendering for 3D modeling applications. The system offers
 * multiple grid styles matching professional software standards, adaptive
 * performance optimization, and comprehensive visual customization options.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup UI
 * @ingroup Rendering
 */

#pragma once

#include "../../Common.h"
#include <QObject>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

// Forward declarations
class Mesh;
class Renderer;

/**
 * @addtogroup UI
 * @{
 */

/**
 * @addtogroup Rendering
 * @{
 */

/**
 * @brief Professional reference grid system for 3D modeling and animation workflows
 * 
 * The GridSystem provides industry-standard reference grid rendering designed
 * for professional 3D modeling applications. It implements multiple grid styles
 * matching popular software packages while offering advanced features like
 * adaptive performance optimization, depth-based fading, and customizable
 * visual appearance for optimal modeling workflows.
 * 
 * ## Grid Architecture
 * The system implements a hierarchical grid structure:
 * 
 * ### Main Axes (X=0, Z=0)
 * - **Purpose**: Primary reference lines for world coordinate alignment
 * - **Appearance**: Bold, high-contrast lines typically in red/green/blue
 * - **Visibility**: Always visible when grid is enabled
 * - **Function**: Essential for object positioning and world orientation
 * 
 * ### Major Grid Lines
 * - **Purpose**: Primary spatial reference with larger spacing
 * - **Spacing**: Configurable major divisions (typically 1, 5, or 10 units)
 * - **Appearance**: Medium contrast lines in neutral colors
 * - **Function**: Primary spatial measurement and alignment reference
 * 
 * ### Minor Subdivisions
 * - **Purpose**: Fine detail reference between major grid lines
 * - **Spacing**: Configurable subdivisions of major grid spacing
 * - **Appearance**: Subtle, low-contrast lines that fade with distance
 * - **Function**: Precise positioning and fine alignment control
 * 
 * ## Professional Grid Styles
 * The system includes preset styles matching industry-standard software:
 * 
 * ### Maya Style
 * - **Characteristics**: Subtle, professional appearance with muted colors
 * - **Main Axes**: Red (X), Green (Y), Blue (Z) with moderate intensity
 * - **Grid Lines**: Light gray with good contrast balance
 * - **Subdivisions**: Very subtle gray for detail without clutter
 * - **Use Case**: Animation and film industry workflows
 * 
 * ### Blender Style
 * - **Characteristics**: Higher contrast with prominent main axes
 * - **Main Axes**: Bright red/green/blue for clear orientation
 * - **Grid Lines**: Medium gray with clear visibility
 * - **Subdivisions**: Darker subdivisions for technical precision
 * - **Use Case**: Artistic modeling and game development
 * 
 * ### Studio Style
 * - **Characteristics**: Minimal, clean appearance for photography
 * - **Main Axes**: Subtle colored axes that don't dominate
 * - **Grid Lines**: Very light gray for non-intrusive reference
 * - **Subdivisions**: Nearly invisible for clean presentation
 * - **Use Case**: Product visualization and photography setups
 * 
 * ### Technical Style
 * - **Characteristics**: Precise, CAD-like appearance for engineering
 * - **Main Axes**: Bold, high-contrast axes for technical accuracy
 * - **Grid Lines**: Clear, well-defined lines for measurement
 * - **Subdivisions**: Visible minor divisions for precision work
 * - **Use Case**: CAD modeling and technical design
 * 
 * ## Advanced Performance Features
 * The GridSystem includes sophisticated optimizations for professional use:
 * 
 * ### Adaptive Grid Resolution
 * - **Purpose**: Automatically adjusts grid detail based on camera distance
 * - **Near View**: Full subdivision detail for precise modeling
 * - **Mid View**: Balanced grid density for general work
 * - **Far View**: Simplified grid to maintain performance
 * - **Benefits**: Consistent visual density and optimal performance
 * 
 * ### Depth-Based Fading
 * - **Purpose**: Prevents grid lines from interfering with 3D objects
 * - **Implementation**: Lines fade based on distance from camera
 * - **Visual Result**: Grid provides reference without visual clutter
 * - **Artistic Control**: Fade distance fully configurable
 * 
 * ### Performance Optimization
 * - **Line Batching**: Efficient GPU rendering of grid geometry
 * - **Frustum Culling**: Only visible grid sections are rendered
 * - **Level-of-Detail**: Dynamic grid simplification for performance
 * - **Caching**: Grid mesh cached until settings change
 * 
 * ## Coordinate System Support
 * The grid system supports multiple coordinate orientations:
 * 
 * ### Standard XZ Plane (Y-Up)
 * - **Default**: Horizontal grid with Y-axis pointing up
 * - **Use Case**: Most 3D modeling applications and games
 * - **Axes**: X (right), Y (up), Z (forward/back)
 * 
 * ### Custom Plane Orientation
 * - **Flexibility**: Grid can be oriented on any plane
 * - **Use Case**: Specialized workflows and non-standard orientations
 * - **Implementation**: Configurable plane normal and offset
 * 
 * ## Integration with Modeling Tools
 * The GridSystem integrates seamlessly with modeling workflows:
 * - **Snap-to-Grid**: Visual feedback for snap-to-grid operations
 * - **Unit Display**: Grid spacing matches scene units for measurement
 * - **Real-time Updates**: Immediate visual feedback for setting changes
 * - **Performance Scaling**: Adapts to viewport size and performance needs
 * 
 * ## Usage Examples
 * ```cpp
 * // Create and configure grid system
 * auto gridSystem = std::make_shared<GridSystem>();
 * 
 * // Apply professional Maya-style grid
 * gridSystem->setGridStyle(GridSystem::GridStyle::Maya);
 * 
 * // Configure grid spacing for metric modeling
 * gridSystem->setGridSize(10.0f);        // 10 unit major divisions
 * gridSystem->setGridDivisions(10);      // 10 major lines per side
 * gridSystem->setSubdivisions(5);        // 5 subdivisions per major division
 * 
 * // Enable advanced features
 * gridSystem->setAdaptiveGrid(true);     // Auto-adjust detail
 * gridSystem->setDepthFading(true);      // Fade with distance
 * gridSystem->setFadeDistance(50.0f);    // Fade beyond 50 units
 * 
 * // Customize appearance for specific workflow
 * gridSystem->setMainAxisColor(glm::vec4(1.0f, 0.3f, 0.3f, 1.0f)); // Red X-axis
 * gridSystem->setGridLineColor(glm::vec4(0.5f, 0.5f, 0.5f, 0.7f));   // Gray grid
 * 
 * // Render in viewport
 * gridSystem->render(renderer, viewMatrix, projectionMatrix);
 * ```
 * 
 * ## Performance Considerations
 * - **Line Count**: Grid complexity scales with divisions and subdivisions
 * - **Camera Distance**: Adaptive features reduce complexity at distance
 * - **Update Frequency**: Grid mesh cached until settings change
 * - **GPU Usage**: Efficient line rendering with minimal draw calls
 * 
 * @see Renderer for low-level line rendering operations
 * @see RenderSystem for viewport integration
 * @see LightingSystem for depth-aware rendering coordination
 * 
 * @ingroup UI
 * @ingroup Rendering
 */
class GridSystem : public QObject
{
    Q_OBJECT

public:
    // ========================================================================
    // Grid Style Types
    // ========================================================================
    
    /**
     * @enum GridStyle
     * @brief Professional grid appearance presets matching industry standards
     * 
     * Predefined grid styles that match the visual appearance of popular
     * professional 3D modeling and animation software packages.
     */
    enum class GridStyle {
        /**
         * @brief Autodesk Maya-style subtle professional grid
         * 
         * Muted colors with balanced contrast optimized for animation
         * and film industry workflows. Features subtle main axes and
         * well-balanced grid line visibility.
         */
        Maya,
        
        /**
         * @brief Blender-style high-contrast artistic grid
         * 
         * Bright main axes with clear grid visibility optimized for
         * artistic modeling and game development. Higher contrast for
         * better visual feedback during creative work.
         */
        Blender,
        
        /**
         * @brief Photography studio minimal clean grid
         * 
         * Minimal, non-intrusive appearance optimized for product
         * visualization and photography setups. Very subtle lines
         * that provide reference without visual interference.
         */
        Studio,
        
        /**
         * @brief CAD-style precise technical grid
         * 
         * High precision appearance with clear measurements optimized
         * for technical design and engineering workflows. Bold lines
         * and visible subdivisions for accurate modeling.
         */
        Technical,
        
        /**
         * @brief User-defined custom grid appearance
         * 
         * Indicates that grid appearance has been manually customized
         * beyond the standard preset configurations.
         */
        Custom
    };

    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct grid system with Qt parent
     * 
     * Creates a new GridSystem with default Maya-style appearance and
     * standard grid configuration suitable for general 3D modeling work.
     * 
     * @param parent Qt parent object for memory management
     * 
     * @post GridSystem is created with Maya preset active
     * @post Grid is configured with sensible default values
     * @post System is ready for immediate rendering
     */
    explicit GridSystem(QObject* parent = nullptr);
    
    /**
     * @brief Destructor with automatic cleanup
     * 
     * Cleans up grid mesh resources and disconnects from the Qt
     * object hierarchy for proper memory management.
     */
    ~GridSystem();

    // ========================================================================
    // Grid Style and Appearance Management
    // ========================================================================
    
    /**
     * @brief Apply professional grid style preset
     * 
     * Configures all grid appearance parameters to match the specified
     * professional software style, instantly updating colors, line weights,
     * and visual characteristics.
     * 
     * @param style Grid style preset to apply
     * 
     * @post All grid appearance parameters match specified style
     * @post Current style is updated to specified value
     * @post Grid mesh is regenerated with new appearance
     * @post gridChanged() signal is emitted
     * 
     * @note Overwrites any custom appearance adjustments
     * @note Changes take effect on next render call
     * 
     * @see GridStyle for available style descriptions
     */
    void setGridStyle(GridStyle style);
    
    /**
     * @brief Get currently active grid style
     * 
     * @return Currently active grid style preset
     * 
     * @note Returns GridStyle::Custom if appearance has been manually adjusted
     */
    GridStyle getGridStyle() const { return m_currentStyle; }

    // ========================================================================
    // Grid Geometry Configuration
    // ========================================================================
    
    /**
     * @brief Set overall grid size (extent from center)
     * 
     * Configures the total extent of the grid from the center point.
     * The grid extends this distance in both positive and negative
     * directions along each axis.
     * 
     * @param size Grid extent in world units (typically 10-100)
     * 
     * @pre size must be > 0
     * 
     * @post Grid geometry extends ±size units from center
     * @post Grid mesh is marked for regeneration
     * @post gridChanged() signal is emitted
     * 
     * @note Larger sizes may impact performance with high subdivision counts
     * @note Size should match typical object scales in the scene
     */
    void setGridSize(float size);
    
    /**
     * @brief Get current grid size
     * 
     * @return Grid extent in world units
     */
    float getGridSize() const { return m_gridSize; }

    /**
     * @brief Set number of major grid divisions per side
     * 
     * Configures how many major grid lines appear from center to edge.
     * This controls the density of the primary grid structure.
     * 
     * @param divisions Number of major divisions (typically 5-20)
     * 
     * @pre divisions must be > 0
     * 
     * @post Grid has specified number of major lines per quadrant
     * @post Grid mesh is marked for regeneration
     * @post gridChanged() signal is emitted
     * 
     * @note Higher division counts create denser grids
     * @note Performance scales with division count
     */
    void setGridDivisions(int divisions);
    
    /**
     * @brief Get current number of grid divisions
     * 
     * @return Number of major grid divisions per side
     */
    int getGridDivisions() const { return m_gridDivisions; }

    /**
     * @brief Set subdivision count between major grid lines
     * 
     * Configures how many minor subdivision lines appear between
     * each pair of major grid lines for fine detail reference.
     * 
     * @param subdivisions Number of minor lines between major lines (typically 1-10)
     * 
     * @pre subdivisions must be >= 0
     * 
     * @post Grid has specified subdivisions between major lines
     * @post Grid mesh is marked for regeneration
     * @post gridChanged() signal is emitted
     * 
     * @note Subdivisions provide fine positioning reference
     * @note Higher subdivision counts significantly impact performance
     * @note Set to 0 to disable subdivisions entirely
     */
    void setSubdivisions(int subdivisions);
    
    /**
     * @brief Get current subdivision count
     * 
     * @return Number of subdivisions between major grid lines
     */
    int getSubdivisions() const { return m_subdivisions; }

    // ========================================================================
    // Visibility and Display Control
    // ========================================================================
    
    /**
     * @brief Control grid visibility in viewport
     * 
     * Enables or disables grid rendering while preserving all
     * configuration settings for quick show/hide operations.
     * 
     * @param visible true to show grid, false to hide
     * 
     * @post Grid visibility state is updated
     * @post visibilityChanged() signal is emitted
     * 
     * @note Hidden grids are not rendered but retain all settings
     * @note Useful for clean presentation or performance optimization
     */
    void setVisible(bool visible);
    
    /**
     * @brief Check if grid is currently visible
     * 
     * @return true if grid will be rendered when render() is called
     */
    bool isVisible() const { return m_visible; }

    // ========================================================================
    // Color and Appearance Customization
    // ========================================================================
    
    /**
     * @brief Set main coordinate axes color
     * 
     * Configures the color of the primary coordinate axes (X=0, Z=0 lines)
     * that provide world coordinate reference. These are typically the
     * most prominent lines in the grid.
     * 
     * @param color RGBA color for main axes (often red/green for X/Z)
     * 
     * @post Main axes color is updated
     * @post Current style is set to Custom
     * @post Grid mesh is marked for regeneration
     * @post gridChanged() signal is emitted
     * 
     * @note Main axes should be most prominent for clear orientation
     * @note Common colors: red (1,0,0,1) for X, green (0,1,0,1) for Z
     */
    void setMainAxisColor(const glm::vec4& color);
    
    /**
     * @brief Get current main axes color
     * 
     * @return RGBA color vector for main coordinate axes
     */
    glm::vec4 getMainAxisColor() const { return m_mainAxisColor; }

    /**
     * @brief Set major grid lines color
     * 
     * Configures the color of major grid lines that form the primary
     * spatial reference structure. These should be visible but not
     * overpowering compared to the main axes.
     * 
     * @param color RGBA color for major grid lines
     * 
     * @post Major grid line color is updated
     * @post Current style is set to Custom
     * @post Grid mesh is marked for regeneration
     * @post gridChanged() signal is emitted
     * 
     * @note Should be less prominent than main axes
     * @note Typical colors: medium gray (0.5,0.5,0.5,0.7)
     */
    void setGridLineColor(const glm::vec4& color);
    
    /**
     * @brief Get current grid line color
     * 
     * @return RGBA color vector for major grid lines
     */
    glm::vec4 getGridLineColor() const { return m_gridLineColor; }

    /**
     * @brief Set subdivision lines color
     * 
     * Configures the color of minor subdivision lines that provide
     * fine detail reference. These should be subtle to avoid visual
     * clutter while still providing useful reference.
     * 
     * @param color RGBA color for subdivision lines
     * 
     * @post Subdivision line color is updated
     * @post Current style is set to Custom
     * @post Grid mesh is marked for regeneration
     * @post gridChanged() signal is emitted
     * 
     * @note Should be most subtle of all grid elements
     * @note Typical colors: light gray (0.7,0.7,0.7,0.3)
     */
    void setSubdivisionColor(const glm::vec4& color);
    
    /**
     * @brief Get current subdivision color
     * 
     * @return RGBA color vector for subdivision lines
     */
    glm::vec4 getSubdivisionColor() const { return m_subdivisionColor; }

    /**
     * @brief Set line rendering width
     * 
     * Controls the pixel width of all grid lines including main axes,
     * major lines, and subdivisions. Affects the overall visual weight
     * of the grid in the viewport.
     * 
     * @param width Line width in pixels (typically 1.0-3.0)
     * 
     * @post Line width is updated for all grid elements
     * @post Current style is set to Custom if not default
     * @post gridChanged() signal is emitted
     * 
     * @note Very wide lines may not be supported on all hardware
     * @note Line width affects legibility at different zoom levels
     */
    void setLineWidth(float width);
    
    /**
     * @brief Get current line width
     * 
     * @return Line width in pixels
     */
    float getLineWidth() const { return m_lineWidth; }

    /**
     * @brief Set distance-based fade range
     * 
     * Configures the distance from camera at which grid lines begin
     * to fade out. This prevents distant grid lines from creating
     * visual noise and performance issues.
     * 
     * @param distance Fade start distance in world units
     * 
     * @post Fade distance is updated
     * @post Grid lines fade beyond specified distance
     * @post gridChanged() signal is emitted
     * 
     * @note Larger distances show more grid detail at cost of performance
     * @note Should be set based on typical modeling scale and camera range
     */
    void setFadeDistance(float distance);
    
    /**
     * @brief Get current fade distance
     * 
     * @return Distance at which grid fading begins
     */
    float getFadeDistance() const { return m_fadeDistance; }

    // ========================================================================
    // Advanced Grid Features
    // ========================================================================
    
    /**
     * @brief Enable adaptive grid detail adjustment
     * 
     * Enables automatic adjustment of grid detail based on camera distance.
     * When enabled, subdivisions and grid density adapt to maintain
     * optimal visual density and performance across zoom levels.
     * 
     * @param adaptive true to enable adaptive behavior, false for static grid
     * 
     * @post Adaptive grid state is updated
     * @post Grid detail adjusts automatically during camera movement
     * @post gridChanged() signal is emitted
     * 
     * @note Adaptive mode improves performance during camera animation
     * @note Maintains consistent visual density across zoom levels
     * @note May cause subtle grid changes during camera movement
     */
    void setAdaptiveGrid(bool adaptive);
    
    /**
     * @brief Check if adaptive grid is enabled
     * 
     * @return true if grid detail adapts to camera distance
     */
    bool isAdaptiveGrid() const { return m_adaptiveGrid; }

    /**
     * @brief Enable depth-based line fading
     * 
     * Enables fading of grid lines based on their distance from the camera
     * to prevent grid lines from interfering with 3D object visibility.
     * 
     * @param enabled true to enable depth fading, false for uniform opacity
     * 
     * @post Depth fading state is updated
     * @post Grid lines fade with distance on next render
     * @post gridChanged() signal is emitted
     * 
     * @note Depth fading improves 3D object visibility
     * @note Prevents grid from dominating the visual space
     * @note Fade distance controlled by setFadeDistance()
     */
    void setDepthFading(bool enabled);
    
    /**
     * @brief Check if depth fading is enabled
     * 
     * @return true if grid lines fade based on distance
     */
    bool isDepthFading() const { return m_depthFading; }

    // ========================================================================
    // Core Rendering Operations
    // ========================================================================
    
    /**
     * @brief Render grid to current framebuffer
     * 
     * Performs complete grid rendering using the current configuration,
     * view matrices, and appearance settings. This is the main entry
     * point for grid visualization in the rendering pipeline.
     * 
     * @param renderer Shared pointer to renderer for line drawing operations
     * @param viewMatrix Camera view transformation matrix
     * @param projMatrix Camera projection transformation matrix
     * 
     * @pre Renderer must be initialized and ready for line rendering
     * @pre View and projection matrices must be valid
     * @pre Grid must be visible for rendering to occur
     * 
     * @post Grid is rendered to current framebuffer
     * @post Performance metrics are updated
     * 
     * @note Only renders if grid visibility is enabled
     * @note Performance scales with grid complexity and camera position
     * @note Adaptive features may adjust detail during render
     */
    void render(std::shared_ptr<Renderer> renderer, const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
    
    /**
     * @brief Force grid mesh regeneration
     * 
     * Immediately regenerates the grid mesh using current settings.
     * Typically called automatically when settings change, but can
     * be called manually for advanced control.
     * 
     * @post Grid mesh is completely regenerated
     * @post Mesh update flag is cleared
     * @post Performance counters are reset
     * 
     * @note Usually called automatically when needed
     * @note May cause brief performance impact during regeneration
     */
    void updateGrid(); // Regenerate grid mesh when settings change

    // ========================================================================
    // Grid Plane Configuration
    // ========================================================================
    
    /**
     * @brief Configure grid plane orientation
     * 
     * Sets the orientation and position of the grid plane for non-standard
     * orientations. Default is XZ plane (Y-up), but any orientation is supported.
     * 
     * @param normal Normalized normal vector of the grid plane
     * @param offset Distance offset from world origin along normal
     * 
     * @pre normal vector should be normalized
     * 
     * @post Grid plane orientation is updated
     * @post Grid mesh is marked for regeneration
     * @post gridChanged() signal is emitted
     * 
     * @note Default XZ plane uses normal (0,1,0) with offset 0
     * @note Custom orientations useful for specialized workflows
     * @note Offset allows positioning grid away from world origin
     */
    void setGridPlane(const glm::vec3& normal, float offset = 0.0f);
    
    /**
     * @brief Get current grid plane normal
     * 
     * @return Normalized normal vector of the grid plane
     */
    glm::vec3 getGridPlane() const { return m_gridPlane; }
    
    /**
     * @brief Get current grid plane offset
     * 
     * @return Distance offset from origin along plane normal
     */
    float getGridOffset() const { return m_gridOffset; }

signals:
    /**
     * @brief Signal emitted when grid configuration changes
     * 
     * Emitted whenever grid geometry, appearance, or behavior settings
     * are modified, allowing connected systems to update accordingly.
     * 
     * @note Connect to this signal for automatic viewport updates
     * @note Useful for UI synchronization with grid changes
     * @note Emitted before mesh regeneration occurs
     */
    void gridChanged();
    
    /**
     * @brief Signal emitted when grid visibility changes
     * 
     * Emitted specifically when grid visibility is toggled, allowing
     * UI elements to update visibility controls and status indicators.
     * 
     * @param visible New visibility state of the grid
     * 
     * @note Separate from gridChanged() for specific visibility handling
     * @note Useful for toolbar button state synchronization
     */
    void visibilityChanged(bool visible);

private:
    // ========================================================================
    // Internal Grid Management Methods
    // ========================================================================
    
    /**
     * @brief Generate grid mesh geometry
     * 
     * Internal method that creates the complete grid mesh including
     * main axes, major grid lines, and subdivisions based on current
     * configuration settings.
     * 
     * @post Grid mesh is completely regenerated
     * @post Mesh update flag is cleared
     * @post Line count statistics are updated
     * 
     * @note Called automatically when grid settings change
     * @note Performance scales with grid complexity
     */
    void createGridMesh();
    
    /**
     * @brief Apply current grid style settings
     * 
     * Internal method that configures all appearance parameters
     * to match the currently selected grid style preset.
     * 
     * @post All colors and appearance match current style
     * @post Grid mesh is marked for regeneration
     * 
     * @note Called internally by setGridStyle()
     */
    void applyGridStyle();
    
    /**
     * @brief Configure Maya-style grid appearance
     * 
     * Internal method that sets up subtle, professional grid appearance
     * matching Autodesk Maya's default viewport configuration.
     */
    void setupMayaStyle();
    
    /**
     * @brief Configure Blender-style grid appearance
     * 
     * Internal method that sets up high-contrast artistic grid appearance
     * matching Blender's default viewport configuration.
     */
    void setupBlenderStyle();
    
    /**
     * @brief Configure studio-style grid appearance
     * 
     * Internal method that sets up minimal, clean grid appearance
     * optimized for photography and product visualization.
     */
    void setupStudioStyle();
    
    /**
     * @brief Configure technical-style grid appearance
     * 
     * Internal method that sets up precise, CAD-like grid appearance
     * optimized for engineering and technical design workflows.
     */
    void setupTechnicalStyle();

    // ========================================================================
    // Grid Configuration State
    // ========================================================================
    
    /// Currently active grid style preset
    GridStyle m_currentStyle;
    
    /// Overall grid extent from center in world units
    float m_gridSize;
    
    /// Number of major grid divisions per side
    int m_gridDivisions;
    
    /// Number of subdivisions between major lines
    int m_subdivisions;
    
    /// Whether grid is currently visible
    bool m_visible;

    // ========================================================================
    // Visual Appearance Parameters
    // ========================================================================
    
    /// RGBA color of main coordinate axes (X=0, Z=0 lines)
    glm::vec4 m_mainAxisColor;
    
    /// RGBA color of major grid lines
    glm::vec4 m_gridLineColor;
    
    /// RGBA color of subdivision lines
    glm::vec4 m_subdivisionColor;
    
    /// Line width in pixels for all grid elements
    float m_lineWidth;
    
    /// Distance at which grid lines begin to fade
    float m_fadeDistance;

    // ========================================================================
    // Advanced Feature State
    // ========================================================================
    
    /// Whether grid detail adapts to camera distance
    bool m_adaptiveGrid;
    
    /// Whether grid lines fade based on distance from camera
    bool m_depthFading;
    
    // ========================================================================
    // Grid Plane Configuration
    // ========================================================================
    
    /// Normal vector of the grid plane (default: Y-up)
    glm::vec3 m_gridPlane;
    
    /// Offset distance from origin along plane normal
    float m_gridOffset;

    // ========================================================================
    // Rendering Resources and State
    // ========================================================================
    
    /// Mesh containing all grid line geometry
    rude::MeshPtr m_gridMesh;
    
    /// Flag indicating mesh needs regeneration
    bool m_meshNeedsUpdate;

    // ========================================================================
    // Performance Tracking
    // ========================================================================
    
    /// Number of lines rendered in last frame (for performance monitoring)
    int m_lastRenderedLines;
    
    /// Camera distance during last render (for adaptive grid calculations)
    float m_lastCameraDistance;
};

/**
 * @}
 */

/**
 * @}
 */
