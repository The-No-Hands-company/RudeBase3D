#include "GridSystem.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshGenerator.h"
#include <QDebug>
#include <QMatrix4x4>
#include <cmath>

GridSystem::GridSystem(QObject* parent)
    : QObject(parent)
    , m_currentStyle(GridStyle::Maya)
    , m_gridSize(20.0f)
    , m_gridDivisions(20)
    , m_subdivisions(5)
    , m_visible(true)
    , m_mainAxisColor(0.8f, 0.8f, 0.8f, 0.9f)
    , m_gridLineColor(0.4f, 0.4f, 0.4f, 0.7f)
    , m_subdivisionColor(0.25f, 0.25f, 0.25f, 0.5f)
    , m_lineWidth(1.0f)
    , m_fadeDistance(50.0f)
    , m_adaptiveGrid(true)
    , m_depthFading(true)
    , m_gridPlane(0.0f, 1.0f, 0.0f) // XZ plane by default
    , m_gridOffset(0.0f)
    , m_meshNeedsUpdate(true)
    , m_lastRenderedLines(0)
    , m_lastCameraDistance(0.0f)
{
    // Initialize with Maya-style grid
    setupMayaStyle();
    createGridMesh();
}

GridSystem::~GridSystem() = default;

void GridSystem::setGridStyle(GridStyle style)
{
    if (m_currentStyle == style) {
        return;
    }

    m_currentStyle = style;
    applyGridStyle();
    m_meshNeedsUpdate = true;
    
    emit gridChanged();
}

void GridSystem::setGridSize(float size)
{
    if (qFuzzyCompare(m_gridSize, size)) {
        return;
    }

    m_gridSize = qMax(0.1f, size);
    m_meshNeedsUpdate = true;
    emit gridChanged();
}

void GridSystem::setGridDivisions(int divisions)
{
    if (m_gridDivisions == divisions) {
        return;
    }

    m_gridDivisions = qMax(2, divisions);
    m_meshNeedsUpdate = true;
    emit gridChanged();
}

void GridSystem::setSubdivisions(int subdivisions)
{
    if (m_subdivisions == subdivisions) {
        return;
    }

    m_subdivisions = qMax(1, subdivisions);
    m_meshNeedsUpdate = true;
    emit gridChanged();
}

void GridSystem::setVisible(bool visible)
{
    if (m_visible == visible) {
        return;
    }

    m_visible = visible;
    emit visibilityChanged(visible);
}

void GridSystem::setMainAxisColor(const glm::vec4& color)
{
    m_mainAxisColor = color;
    m_currentStyle = GridStyle::Custom;
    emit gridChanged();
}

void GridSystem::setGridLineColor(const glm::vec4& color)
{
    m_gridLineColor = color;
    m_currentStyle = GridStyle::Custom;
    emit gridChanged();
}

void GridSystem::setSubdivisionColor(const glm::vec4& color)
{
    m_subdivisionColor = color;
    m_currentStyle = GridStyle::Custom;
    emit gridChanged();
}

void GridSystem::setLineWidth(float width)
{
    m_lineWidth = qMax(0.1f, width);
    emit gridChanged();
}

void GridSystem::setFadeDistance(float distance)
{
    m_fadeDistance = qMax(1.0f, distance);
    emit gridChanged();
}

void GridSystem::setAdaptiveGrid(bool adaptive)
{
    m_adaptiveGrid = adaptive;
    emit gridChanged();
}

void GridSystem::setDepthFading(bool enabled)
{
    m_depthFading = enabled;
    emit gridChanged();
}

void GridSystem::setGridPlane(const glm::vec3& normal, float offset)
{
    m_gridPlane = glm::normalize(normal);
    m_gridOffset = offset;
    m_meshNeedsUpdate = true;
    emit gridChanged();
}

void GridSystem::render(std::shared_ptr<Renderer> renderer, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
    if (!m_visible || !renderer) {
        return;
    }

    // PROFESSIONAL GRID RENDERING
    renderer->setViewMatrix(viewMatrix);
    renderer->setProjectionMatrix(projMatrix);
    renderer->setModelMatrix(glm::mat4(1.0f));
    
    // Professional grid settings
    renderer->enableDepthTest(false);
    
    // === PROFESSIONAL WORLD AXES ===
    float axisLength = 100.0f;
    float axisThickness = 2.0f;
    
    // World axes with professional colors (like Maya/Blender)
    // X-axis: Red
    renderer->renderLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(axisLength, 0.0f, 0.0f), 
                        glm::vec4(0.9f, 0.2f, 0.2f, 0.9f));
    
    // Y-axis: Green  
    renderer->renderLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, axisLength, 0.0f), 
                        glm::vec4(0.4f, 0.8f, 0.2f, 0.9f));
    
    // Z-axis: Blue
    renderer->renderLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, axisLength), 
                        glm::vec4(0.2f, 0.4f, 0.9f, 0.9f));
    
    // === PROFESSIONAL GRID SYSTEM ===
    float majorGridSize = 10.0f;     // Major grid lines every 10 units
    float minorGridSize = 1.0f;      // Minor grid lines every 1 unit
    float gridExtent = 100.0f;       // Grid extends 100 units in each direction
    
    // Professional color scheme
    glm::vec4 majorGridColor(0.4f, 0.4f, 0.4f, 0.6f);  // Subtle gray for major lines
    glm::vec4 minorGridColor(0.25f, 0.25f, 0.25f, 0.3f); // Very subtle for minor lines
    glm::vec4 originColor(0.6f, 0.6f, 0.6f, 0.8f);     // Slightly brighter for origin lines
    
    // Draw minor grid lines (every 1 unit)
    for (float i = -gridExtent; i <= gridExtent; i += minorGridSize) {
        if (fmod(i, majorGridSize) == 0.0f) continue; // Skip major grid positions
        
        // Lines parallel to X-axis
        renderer->renderLine(glm::vec3(-gridExtent, 0.0f, i), 
                           glm::vec3(gridExtent, 0.0f, i), minorGridColor);
        
        // Lines parallel to Z-axis
        renderer->renderLine(glm::vec3(i, 0.0f, -gridExtent), 
                           glm::vec3(i, 0.0f, gridExtent), minorGridColor);
    }
    
    // Draw major grid lines (every 10 units)
    for (float i = -gridExtent; i <= gridExtent; i += majorGridSize) {
        if (i == 0.0f) continue; // Skip origin lines
        
        // Lines parallel to X-axis
        renderer->renderLine(glm::vec3(-gridExtent, 0.0f, i), 
                           glm::vec3(gridExtent, 0.0f, i), majorGridColor);
        
        // Lines parallel to Z-axis
        renderer->renderLine(glm::vec3(i, 0.0f, -gridExtent), 
                           glm::vec3(i, 0.0f, gridExtent), majorGridColor);
    }
    
    // Draw origin lines (X=0 and Z=0) with special highlighting
    renderer->renderLine(glm::vec3(-gridExtent, 0.0f, 0.0f), 
                        glm::vec3(gridExtent, 0.0f, 0.0f), originColor);
    renderer->renderLine(glm::vec3(0.0f, 0.0f, -gridExtent), 
                        glm::vec3(0.0f, 0.0f, gridExtent), originColor);
}

void GridSystem::updateGrid()
{
    createGridMesh();
    m_meshNeedsUpdate = false;
}

void GridSystem::createGridMesh()
{
    // Use MeshGenerator to create the basic grid
    m_gridMesh = MeshGenerator::generateGrid(m_gridSize, m_gridDivisions);
    
    if (!m_gridMesh) {
        qWarning() << "GridSystem: Failed to create grid mesh";
        return;
    }

    // TODO: For non-XZ planes, transform the grid vertices according to m_gridPlane and m_gridOffset
    // This would allow grids on arbitrary planes (YZ, XY, or custom orientations)
    
    m_lastRenderedLines = (m_gridDivisions + 1) * 4; // Approximate line count
}

void GridSystem::applyGridStyle()
{
    switch (m_currentStyle) {
        case GridStyle::Maya:
            setupMayaStyle();
            break;
        case GridStyle::Blender:
            setupBlenderStyle();
            break;
        case GridStyle::Studio:
            setupStudioStyle();
            break;
        case GridStyle::Technical:
            setupTechnicalStyle();
            break;
        case GridStyle::Custom:
            // Don't change colors for custom style
            break;
    }
}

void GridSystem::setupMayaStyle()
{
    // Professional Maya-style grid settings
    m_mainAxisColor = glm::vec4(0.6f, 0.6f, 0.6f, 0.8f);      // Subtle main axes
    m_gridLineColor = glm::vec4(0.3f, 0.3f, 0.3f, 0.5f);      // Professional subtle grid
    m_subdivisionColor = glm::vec4(0.2f, 0.2f, 0.2f, 0.3f);   // Very subtle subdivisions
    m_lineWidth = 1.0f;          // Professional thin lines
    m_depthFading = true;        // Enable depth fading for realism
    m_adaptiveGrid = true;       // Adaptive grid based on zoom level
}

void GridSystem::setupBlenderStyle()
{
    // Blender-style: Bright main axes, clear grid
    m_mainAxisColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);      // Bright white axes
    m_gridLineColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.8f);      // Clear grid lines
    m_subdivisionColor = glm::vec4(0.3f, 0.3f, 0.3f, 0.6f);   // Visible subdivisions
    m_lineWidth = 1.2f;
    m_depthFading = true;
}

void GridSystem::setupStudioStyle()
{
    // Studio-style: Minimal, professional
    m_mainAxisColor = glm::vec4(0.7f, 0.7f, 0.7f, 0.8f);      // Subdued axes
    m_gridLineColor = glm::vec4(0.3f, 0.3f, 0.3f, 0.6f);      // Minimal grid
    m_subdivisionColor = glm::vec4(0.2f, 0.2f, 0.2f, 0.4f);   // Barely visible
    m_lineWidth = 0.8f;
    m_depthFading = true;
}

void GridSystem::setupTechnicalStyle()
{
    // Technical/CAD-style: High contrast, precise
    m_mainAxisColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);      // Red X, will need separate Z color
    m_gridLineColor = glm::vec4(0.6f, 0.6f, 0.6f, 0.9f);      // High contrast grid
    m_subdivisionColor = glm::vec4(0.4f, 0.4f, 0.4f, 0.7f);   // Clear subdivisions
    m_lineWidth = 1.0f;
    m_depthFading = false; // Always visible in technical mode
}
