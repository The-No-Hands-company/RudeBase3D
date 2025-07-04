#pragma once

#include "Common.h"
#include <QObject>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <memory>

// Forward declarations
class Mesh;
class Renderer;

/**
 * @brief Professional grid rendering system for 3D modeling applications
 * 
 * Provides industry-standard grid rendering with features like:
 * - Configurable grid size, divisions, and spacing
 * - Multiple grid styles (Maya, Blender, Studio)
 * - Axis highlighting and subdivision visualization
 * - Performance optimized line rendering
 * - Depth-aware rendering (grid behind objects)
 */
class GridSystem : public QObject
{
    Q_OBJECT

public:
    enum class GridStyle {
        Maya,           // Maya-style grid (subtle, professional)
        Blender,        // Blender-style grid (bright main axes)
        Studio,         // Photography studio grid (minimal)
        Technical,      // CAD-style precise grid
        Custom          // User-defined appearance
    };

    explicit GridSystem(QObject* parent = nullptr);
    ~GridSystem();

    // Grid configuration
    void setGridStyle(GridStyle style);
    GridStyle getGridStyle() const { return m_currentStyle; }

    void setGridSize(float size);
    float getGridSize() const { return m_gridSize; }

    void setGridDivisions(int divisions);
    int getGridDivisions() const { return m_gridDivisions; }

    void setSubdivisions(int subdivisions);
    int getSubdivisions() const { return m_subdivisions; }

    // Visibility and appearance
    void setVisible(bool visible);
    bool isVisible() const { return m_visible; }

    void setMainAxisColor(const QVector4D& color);
    QVector4D getMainAxisColor() const { return m_mainAxisColor; }

    void setGridLineColor(const QVector4D& color);
    QVector4D getGridLineColor() const { return m_gridLineColor; }

    void setSubdivisionColor(const QVector4D& color);
    QVector4D getSubdivisionColor() const { return m_subdivisionColor; }

    void setLineWidth(float width);
    float getLineWidth() const { return m_lineWidth; }

    void setFadeDistance(float distance);
    float getFadeDistance() const { return m_fadeDistance; }

    // Advanced features
    void setAdaptiveGrid(bool adaptive);
    bool isAdaptiveGrid() const { return m_adaptiveGrid; }

    void setDepthFading(bool enabled);
    bool isDepthFading() const { return m_depthFading; }

    // Rendering
    void render(std::shared_ptr<Renderer> renderer, const QMatrix4x4& viewMatrix, const QMatrix4x4& projMatrix);
    void updateGrid(); // Regenerate grid mesh when settings change

    // Grid plane
    void setGridPlane(const QVector3D& normal, float offset = 0.0f);
    QVector3D getGridPlane() const { return m_gridPlane; }
    float getGridOffset() const { return m_gridOffset; }

signals:
    void gridChanged();
    void visibilityChanged(bool visible);

private:
    void createGridMesh();
    void applyGridStyle();
    void setupMayaStyle();
    void setupBlenderStyle();
    void setupStudioStyle();
    void setupTechnicalStyle();

    // Grid configuration
    GridStyle m_currentStyle;
    float m_gridSize;
    int m_gridDivisions;
    int m_subdivisions;
    bool m_visible;

    // Appearance
    QVector4D m_mainAxisColor;     // X=0, Z=0 lines
    QVector4D m_gridLineColor;     // Major grid lines
    QVector4D m_subdivisionColor;  // Minor subdivision lines
    float m_lineWidth;
    float m_fadeDistance;

    // Advanced features
    bool m_adaptiveGrid;           // Adjust detail based on camera distance
    bool m_depthFading;            // Fade grid lines based on depth
    
    // Grid plane (for non-XZ grids)
    QVector3D m_gridPlane;         // Grid plane normal
    float m_gridOffset;            // Offset from origin

    // Rendering data
    MeshPtr m_gridMesh;
    bool m_meshNeedsUpdate;

    // Performance tracking
    int m_lastRenderedLines;
    float m_lastCameraDistance;
};
