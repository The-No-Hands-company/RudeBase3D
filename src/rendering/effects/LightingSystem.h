#pragma once

#include "Common.h"
#include <QObject>
#include <QVector3D>
#include <QVector4D>
#include <memory>

// Forward declarations
class Renderer;

/**
 * @brief Industry-standard lighting system for 3D modeling applications
 * 
 * Implements professional three-point lighting setup with:
 * - Key Light: Primary directional light
 * - Fill Light: Secondary softer light to reduce harsh shadows
 * - Rim Light: Backlight for object separation
 * - Environment: Ambient lighting simulation
 */
class LightingSystem : public QObject
{
    Q_OBJECT

public:
    enum class LightingPreset {
        Studio,           // Professional studio lighting
        Maya,            // Maya-style default lighting  
        Blender,         // Blender-style default lighting
        Outdoor,         // Outdoor/daylight simulation
        Custom           // User-defined lighting
    };

    explicit LightingSystem(QObject* parent = nullptr);
    ~LightingSystem();

    // Lighting setup
    void setLightingPreset(LightingPreset preset);
    LightingPreset getCurrentPreset() const { return m_currentPreset; }

    // Key light control
    void setKeyLight(const QVector3D& direction, const QVector4D& color, float intensity = 1.0f);
    QVector3D getKeyLightDirection() const { return m_keyLightDirection; }
    QVector4D getKeyLightColor() const { return m_keyLightColor; }
    float getKeyLightIntensity() const { return m_keyLightIntensity; }

    // Fill light control  
    void setFillLight(const QVector3D& direction, const QVector4D& color, float intensity = 0.5f);
    QVector3D getFillLightDirection() const { return m_fillLightDirection; }
    QVector4D getFillLightColor() const { return m_fillLightColor; }
    float getFillLightIntensity() const { return m_fillLightIntensity; }

    // Rim light control
    void setRimLight(const QVector3D& direction, const QVector4D& color, float intensity = 0.3f);
    QVector3D getRimLightDirection() const { return m_rimLightDirection; }
    QVector4D getRimLightColor() const { return m_rimLightColor; }
    float getRimLightIntensity() const { return m_rimLightIntensity; }

    // Environment lighting
    void setAmbientLight(const QVector4D& color, float intensity = 0.2f);
    QVector4D getAmbientColor() const { return m_ambientColor; }
    float getAmbientIntensity() const { return m_ambientIntensity; }

    // Rendering integration
    void applyLighting(std::shared_ptr<Renderer> renderer, const QVector3D& cameraPosition);
    void updateUniforms(std::shared_ptr<Renderer> renderer);

    // Shadows (future expansion)
    void setShadowsEnabled(bool enabled) { m_shadowsEnabled = enabled; }
    bool areShadowsEnabled() const { return m_shadowsEnabled; }

signals:
    void lightingChanged();

private:
    void setupStudioLighting();
    void setupMayaLighting(); 
    void setupBlenderLighting();
    void setupOutdoorLighting();

    LightingPreset m_currentPreset;

    // Key light (primary)
    QVector3D m_keyLightDirection;
    QVector4D m_keyLightColor;
    float m_keyLightIntensity;

    // Fill light (secondary)
    QVector3D m_fillLightDirection;
    QVector4D m_fillLightColor;
    float m_fillLightIntensity;

    // Rim light (accent)
    QVector3D m_rimLightDirection;
    QVector4D m_rimLightColor;
    float m_rimLightIntensity;

    // Environment
    QVector4D m_ambientColor;
    float m_ambientIntensity;

    // Future features
    bool m_shadowsEnabled;
};
