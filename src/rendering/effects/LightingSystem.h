#pragma once

#include "Common.h"
#include <QObject>
#include <glm/glm.hpp>
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
    void setKeyLight(const glm::vec3& direction, const glm::vec4& color, float intensity = 1.0f);
    glm::vec3 getKeyLightDirection() const { return m_keyLightDirection; }
    glm::vec4 getKeyLightColor() const { return m_keyLightColor; }
    float getKeyLightIntensity() const { return m_keyLightIntensity; }

    // Fill light control  
    void setFillLight(const glm::vec3& direction, const glm::vec4& color, float intensity = 0.5f);
    glm::vec3 getFillLightDirection() const { return m_fillLightDirection; }
    glm::vec4 getFillLightColor() const { return m_fillLightColor; }
    float getFillLightIntensity() const { return m_fillLightIntensity; }

    // Rim light control
    void setRimLight(const glm::vec3& direction, const glm::vec4& color, float intensity = 0.3f);
    glm::vec3 getRimLightDirection() const { return m_rimLightDirection; }
    glm::vec4 getRimLightColor() const { return m_rimLightColor; }
    float getRimLightIntensity() const { return m_rimLightIntensity; }

    // Environment lighting
    void setAmbientLight(const glm::vec4& color, float intensity = 0.2f);
    glm::vec4 getAmbientColor() const { return m_ambientColor; }
    float getAmbientIntensity() const { return m_ambientIntensity; }

    // Rendering integration
    void applyLighting(std::shared_ptr<Renderer> renderer, const glm::vec3& cameraPosition);
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
    glm::vec3 m_keyLightDirection;
    glm::vec4 m_keyLightColor;
    float m_keyLightIntensity;

    // Fill light (secondary)
    glm::vec3 m_fillLightDirection;
    glm::vec4 m_fillLightColor;
    float m_fillLightIntensity;

    // Rim light (accent)
    glm::vec3 m_rimLightDirection;
    glm::vec4 m_rimLightColor;
    float m_rimLightIntensity;

    // Environment
    glm::vec4 m_ambientColor;
    float m_ambientIntensity;

    // Future features
    bool m_shadowsEnabled;
};
