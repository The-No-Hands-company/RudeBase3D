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

    qDebug() << "GridSystem::render() - Rendering grid and axes";
    
    // CRITICAL: Set up matrices before rendering
    renderer->setViewMatrix(viewMatrix);
    renderer->setProjectionMatrix(projMatrix);
    renderer->setModelMatrix(glm::mat4(1.0f)); // Identity for world coordinates
    
    // Set up basic rendering state for grid rendering
    renderer->enableDepthTest(false); // Grid always renders behind objects
    renderer->setLineWidth(20.0f);    // EXTREMELY thick lines for visibility
    
    // Draw world axes with HIGH CONTRAST colors
    qDebug() << "Rendering world axes...";
    
    // X axis (BRIGHT RED) - spans entire world
    renderer->renderLine(glm::vec3(-1000.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    
    // Y axis (BRIGHT GREEN) - spans entire world  
    renderer->renderLine(glm::vec3(0.0f, -1000.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    
    // Z axis (BRIGHT BLUE) - spans entire world
    renderer->renderLine(glm::vec3(0.0f, 0.0f, -1000.0f), glm::vec3(0.0f, 0.0f, 1000.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    
    // Draw a test cross right at the origin for reference
    float crossSize = 5.0f;
    renderer->renderLine(glm::vec3(-crossSize, 0.0f, 0.0f), glm::vec3(crossSize, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    renderer->renderLine(glm::vec3(0.0f, -crossSize, 0.0f), glm::vec3(0.0f, crossSize, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    renderer->renderLine(glm::vec3(0.0f, 0.0f, -crossSize), glm::vec3(0.0f, 0.0f, crossSize), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    
    // Draw grid lines on XZ plane (ground plane)
    qDebug() << "Rendering grid lines...";
    float gridExtent = 100.0f;
    float gridSpacing = 5.0f;
    glm::vec4 gridColor(0.5f, 0.5f, 0.5f, 1.0f); // Gray grid lines
    
    // Grid lines parallel to X axis (running along Z)
    for (float z = -gridExtent; z <= gridExtent; z += gridSpacing) {
        renderer->renderLine(glm::vec3(-gridExtent, 0.0f, z), glm::vec3(gridExtent, 0.0f, z), gridColor);
    }
    
    // Grid lines parallel to Z axis (running along X)
    for (float x = -gridExtent; x <= gridExtent; x += gridSpacing) {
        renderer->renderLine(glm::vec3(x, 0.0f, -gridExtent), glm::vec3(x, 0.0f, gridExtent), gridColor);
    }
    
    qDebug() << "GridSystem::render() - Grid rendering complete";
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
    // Maya-style: Much brighter for testing
    m_mainAxisColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);      // Very bright main axes
    m_gridLineColor = glm::vec4(0.8f, 0.8f, 0.8f, 0.9f);      // Very bright grid lines
    m_subdivisionColor = glm::vec4(0.6f, 0.6f, 0.6f, 0.8f);   // Bright subdivisions
    m_lineWidth = 2.0f;  // Thicker lines for visibility
    m_depthFading = false;  // Disable fading for testing
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
