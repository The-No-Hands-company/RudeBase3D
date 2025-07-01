#include "LightingSystem.h"
#include "Renderer.h"
#include <QDebug>

LightingSystem::LightingSystem(QObject* parent)
    : QObject(parent)
    , m_currentPreset(LightingPreset::Studio)
    , m_keyLightDirection(-0.4f, -0.8f, -0.6f)
    , m_keyLightColor(0.9f, 0.9f, 1.0f, 1.0f)
    , m_keyLightIntensity(1.0f)
    , m_fillLightDirection(0.6f, -0.3f, 0.8f)
    , m_fillLightColor(0.8f, 0.85f, 1.0f, 1.0f)
    , m_fillLightIntensity(0.4f)
    , m_rimLightDirection(0.2f, 0.8f, -0.9f)
    , m_rimLightColor(1.0f, 0.95f, 0.9f, 1.0f)
    , m_rimLightIntensity(0.3f)
    , m_ambientColor(0.2f, 0.22f, 0.25f, 1.0f)
    , m_ambientIntensity(0.2f)
    , m_shadowsEnabled(false)
{
    // Initialize with studio lighting preset
    setupStudioLighting();
}

LightingSystem::~LightingSystem() = default;

void LightingSystem::setLightingPreset(LightingPreset preset)
{
    if (m_currentPreset == preset) {
        return;
    }

    m_currentPreset = preset;

    switch (preset) {
        case LightingPreset::Studio:
            setupStudioLighting();
            break;
        case LightingPreset::Maya:
            setupMayaLighting();
            break;
        case LightingPreset::Blender:
            setupBlenderLighting();
            break;
        case LightingPreset::Outdoor:
            setupOutdoorLighting();
            break;
        case LightingPreset::Custom:
            // Don't change anything for custom
            break;
    }

    emit lightingChanged();
}

void LightingSystem::setKeyLight(const QVector3D& direction, const QVector4D& color, float intensity)
{
    m_keyLightDirection = direction.normalized();
    m_keyLightColor = color;
    m_keyLightIntensity = intensity;
    m_currentPreset = LightingPreset::Custom;
    emit lightingChanged();
}

void LightingSystem::setFillLight(const QVector3D& direction, const QVector4D& color, float intensity)
{
    m_fillLightDirection = direction.normalized();
    m_fillLightColor = color;
    m_fillLightIntensity = intensity;
    m_currentPreset = LightingPreset::Custom;
    emit lightingChanged();
}

void LightingSystem::setRimLight(const QVector3D& direction, const QVector4D& color, float intensity)
{
    m_rimLightDirection = direction.normalized();
    m_rimLightColor = color;
    m_rimLightIntensity = intensity;
    m_currentPreset = LightingPreset::Custom;
    emit lightingChanged();
}

void LightingSystem::setAmbientLight(const QVector4D& color, float intensity)
{
    m_ambientColor = color;
    m_ambientIntensity = intensity;
    m_currentPreset = LightingPreset::Custom;
    emit lightingChanged();
}

void LightingSystem::applyLighting(std::shared_ptr<Renderer> renderer, const QVector3D& cameraPosition)
{
    if (!renderer) {
        qWarning() << "LightingSystem: Invalid renderer";
        return;
    }

    // For now, use the key light as the primary light for the renderer
    // In the future, we can enhance the renderer to support multiple lights
    QVector4D finalKeyColor = m_keyLightColor * m_keyLightIntensity;
    renderer->setLighting(m_keyLightDirection, finalKeyColor);
    renderer->setViewPosition(cameraPosition);

    // TODO: When renderer supports multiple lights, also apply fill and rim lights
    // renderer->setFillLight(m_fillLightDirection, m_fillLightColor * m_fillLightIntensity);
    // renderer->setRimLight(m_rimLightDirection, m_rimLightColor * m_rimLightIntensity);
    // renderer->setAmbientLight(m_ambientColor, m_ambientIntensity);
}

void LightingSystem::updateUniforms(std::shared_ptr<Renderer> renderer)
{
    // This method can be used to update lighting uniforms without changing camera
    if (!renderer) {
        return;
    }

    QVector4D finalKeyColor = m_keyLightColor * m_keyLightIntensity;
    renderer->setLighting(m_keyLightDirection, finalKeyColor);
}

void LightingSystem::setupStudioLighting()
{
    // Professional 3-point studio lighting setup
    
    // Key Light: Primary light from upper front-right (45° up, 45° right)
    m_keyLightDirection = QVector3D(-0.4f, -0.8f, -0.6f).normalized();
    m_keyLightColor = QVector4D(0.9f, 0.9f, 1.0f, 1.0f);  // Slightly cool white
    m_keyLightIntensity = 1.0f;

    // Fill Light: Softer light from opposite side to reduce harsh shadows
    m_fillLightDirection = QVector3D(0.6f, -0.3f, 0.8f).normalized();
    m_fillLightColor = QVector4D(0.8f, 0.85f, 1.0f, 1.0f);  // Cooler fill
    m_fillLightIntensity = 0.4f;

    // Rim Light: Backlight for object separation
    m_rimLightDirection = QVector3D(0.2f, 0.8f, -0.9f).normalized();
    m_rimLightColor = QVector4D(1.0f, 0.95f, 0.9f, 1.0f);  // Warm rim
    m_rimLightIntensity = 0.3f;

    // Ambient: Cool ambient for professional contrast
    m_ambientColor = QVector4D(0.2f, 0.22f, 0.25f, 1.0f);
    m_ambientIntensity = 0.2f;
}

void LightingSystem::setupMayaLighting()
{
    // Maya-style default lighting (softer, more diffuse)
    
    m_keyLightDirection = QVector3D(-0.3f, -0.7f, -0.5f).normalized();
    m_keyLightColor = QVector4D(1.0f, 0.98f, 0.95f, 1.0f);  // Warm white
    m_keyLightIntensity = 0.8f;

    m_fillLightDirection = QVector3D(0.5f, -0.4f, 0.7f).normalized();
    m_fillLightColor = QVector4D(0.9f, 0.9f, 1.0f, 1.0f);   // Cool fill
    m_fillLightIntensity = 0.5f;

    m_rimLightDirection = QVector3D(0.1f, 0.6f, -0.8f).normalized();
    m_rimLightColor = QVector4D(1.0f, 0.9f, 0.8f, 1.0f);    // Warm rim
    m_rimLightIntensity = 0.25f;

    m_ambientColor = QVector4D(0.25f, 0.25f, 0.28f, 1.0f);
    m_ambientIntensity = 0.25f;
}

void LightingSystem::setupBlenderLighting()
{
    // Blender-style default lighting (neutral, technical)
    
    m_keyLightDirection = QVector3D(-0.35f, -0.75f, -0.55f).normalized();
    m_keyLightColor = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);    // Pure white
    m_keyLightIntensity = 0.9f;

    m_fillLightDirection = QVector3D(0.7f, -0.2f, 0.6f).normalized();
    m_fillLightColor = QVector4D(0.95f, 0.95f, 1.0f, 1.0f); // Slightly cool
    m_fillLightIntensity = 0.3f;

    m_rimLightDirection = QVector3D(0.3f, 0.7f, -0.7f).normalized();
    m_rimLightColor = QVector4D(1.0f, 1.0f, 0.95f, 1.0f);   // Slightly warm
    m_rimLightIntensity = 0.2f;

    m_ambientColor = QVector4D(0.2f, 0.2f, 0.2f, 1.0f);     // Neutral gray
    m_ambientIntensity = 0.2f;
}

void LightingSystem::setupOutdoorLighting()
{
    // Outdoor/daylight simulation (sun + sky)
    
    // Sun as key light
    m_keyLightDirection = QVector3D(-0.2f, -0.9f, -0.4f).normalized();
    m_keyLightColor = QVector4D(1.0f, 0.95f, 0.8f, 1.0f);   // Warm sunlight
    m_keyLightIntensity = 1.2f;

    // Sky as fill light
    m_fillLightDirection = QVector3D(0.3f, -0.1f, 0.9f).normalized();
    m_fillLightColor = QVector4D(0.7f, 0.8f, 1.0f, 1.0f);   // Blue sky
    m_fillLightIntensity = 0.6f;

    // Atmospheric rim
    m_rimLightDirection = QVector3D(0.5f, 0.5f, -0.7f).normalized();
    m_rimLightColor = QVector4D(0.9f, 0.85f, 0.7f, 1.0f);   // Atmospheric haze
    m_rimLightIntensity = 0.35f;

    // Environmental ambient
    m_ambientColor = QVector4D(0.3f, 0.35f, 0.4f, 1.0f);    // Blue-tinted ambient
    m_ambientIntensity = 0.3f;
}
