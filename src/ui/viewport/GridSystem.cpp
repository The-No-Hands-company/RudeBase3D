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

void GridSystem::setMainAxisColor(const QVector4D& color)
{
    m_mainAxisColor = color;
    m_currentStyle = GridStyle::Custom;
    emit gridChanged();
}

void GridSystem::setGridLineColor(const QVector4D& color)
{
    m_gridLineColor = color;
    m_currentStyle = GridStyle::Custom;
    emit gridChanged();
}

void GridSystem::setSubdivisionColor(const QVector4D& color)
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

void GridSystem::setGridPlane(const QVector3D& normal, float offset)
{
    m_gridPlane = normal.normalized();
    m_gridOffset = offset;
    m_meshNeedsUpdate = true;
    emit gridChanged();
}

void GridSystem::render(std::shared_ptr<Renderer> renderer, const QMatrix4x4& viewMatrix, const QMatrix4x4& projMatrix)
{
    if (!m_visible || !renderer || !m_gridMesh) {
        return;
    }

    // Update grid mesh if needed
    if (m_meshNeedsUpdate) {
        updateGrid();
    }

    // Save current OpenGL state
    float currentLineWidth;
    glGetFloatv(GL_LINE_WIDTH, &currentLineWidth);
    GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);

    // Set up grid rendering state
    glDisable(GL_DEPTH_TEST); // Grid always renders behind objects
    glLineWidth(m_lineWidth);

    // Set identity model matrix for world-space grid
    renderer->setModelMatrix(QMatrix4x4());

    // Get grid vertices for manual line rendering
    auto vertices = m_gridMesh->getVertices();

    // Render grid lines with appropriate colors
    for (size_t i = 0; i < vertices.size(); i += 2) {
        if (i + 1 < vertices.size()) {
            QVector3D start = vertices[i].position;
            QVector3D end = vertices[i + 1].position;

            // Determine line type and color
            QVector4D color = m_gridLineColor;

            // Major axis lines (X=0 and Z=0 for XZ plane)
            if ((qAbs(start.x()) < 0.01f && qAbs(end.x()) < 0.01f) ||  // Z-axis line
                (qAbs(start.z()) < 0.01f && qAbs(end.z()) < 0.01f)) {  // X-axis line
                color = m_mainAxisColor;
            }
            // Subdivision lines (every Nth line based on subdivision setting)
            else if (m_subdivisions > 1) {
                float stepSize = m_gridSize / static_cast<float>(m_gridDivisions);
                float subdivStepSize = stepSize / static_cast<float>(m_subdivisions);
                
                bool isSubdivision = false;
                // Check if this line aligns with subdivision
                if (qAbs(fmod(qAbs(start.x()), subdivStepSize)) < 0.01f ||
                    qAbs(fmod(qAbs(start.z()), subdivStepSize)) < 0.01f) {
                    isSubdivision = true;
                }
                
                if (isSubdivision) {
                    color = m_subdivisionColor;
                }
            }

            // Apply depth fading if enabled
            if (m_depthFading) {
                QVector3D midPoint = (start + end) * 0.5f;
                QVector4D clipPos = projMatrix * viewMatrix * QVector4D(midPoint, 1.0f);
                float depth = clipPos.z() / clipPos.w();
                float fadeAlpha = 1.0f - qMax(0.0f, qMin(1.0f, depth / m_fadeDistance));
                color.setW(color.w() * fadeAlpha);
            }

            renderer->renderLine(start, end, color);
        }
    }

    // Restore OpenGL state
    if (depthTestEnabled) glEnable(GL_DEPTH_TEST);
    glLineWidth(currentLineWidth);
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
    // Maya-style: Subtle grid with bright main axes
    m_mainAxisColor = QVector4D(0.8f, 0.8f, 0.8f, 0.9f);      // Bright main axes
    m_gridLineColor = QVector4D(0.4f, 0.4f, 0.4f, 0.7f);      // Subtle grid lines
    m_subdivisionColor = QVector4D(0.25f, 0.25f, 0.25f, 0.5f); // Very subtle subdivisions
    m_lineWidth = 1.0f;
    m_depthFading = true;
}

void GridSystem::setupBlenderStyle()
{
    // Blender-style: Bright main axes, clear grid
    m_mainAxisColor = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);      // Bright white axes
    m_gridLineColor = QVector4D(0.5f, 0.5f, 0.5f, 0.8f);      // Clear grid lines
    m_subdivisionColor = QVector4D(0.3f, 0.3f, 0.3f, 0.6f);   // Visible subdivisions
    m_lineWidth = 1.2f;
    m_depthFading = true;
}

void GridSystem::setupStudioStyle()
{
    // Studio-style: Minimal, professional
    m_mainAxisColor = QVector4D(0.7f, 0.7f, 0.7f, 0.8f);      // Subdued axes
    m_gridLineColor = QVector4D(0.3f, 0.3f, 0.3f, 0.6f);      // Minimal grid
    m_subdivisionColor = QVector4D(0.2f, 0.2f, 0.2f, 0.4f);   // Barely visible
    m_lineWidth = 0.8f;
    m_depthFading = true;
}

void GridSystem::setupTechnicalStyle()
{
    // Technical/CAD-style: High contrast, precise
    m_mainAxisColor = QVector4D(1.0f, 0.0f, 0.0f, 1.0f);      // Red X, will need separate Z color
    m_gridLineColor = QVector4D(0.6f, 0.6f, 0.6f, 0.9f);      // High contrast grid
    m_subdivisionColor = QVector4D(0.4f, 0.4f, 0.4f, 0.7f);   // Clear subdivisions
    m_lineWidth = 1.0f;
    m_depthFading = false; // Always visible in technical mode
}
