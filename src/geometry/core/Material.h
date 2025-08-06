#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

class Material {
public:
    Material();
    ~Material() = default;

    // Color properties
    void setDiffuseColor(const glm::vec4& color) { m_diffuseColor = color; }
    const glm::vec4& getDiffuseColor() const { return m_diffuseColor; }
    
    void setSpecularColor(const glm::vec4& color) { m_specularColor = color; }
    const glm::vec4& getSpecularColor() const { return m_specularColor; }
    
    void setAmbientColor(const glm::vec4& color) { m_ambientColor = color; }
    const glm::vec4& getAmbientColor() const { return m_ambientColor; }
    
    // Material properties
    void setShininess(float shininess) { m_shininess = shininess; }
    float getShininess() const { return m_shininess; }
    
    void setMetallic(float metallic) { m_metallic = metallic; }
    float getMetallic() const { return m_metallic; }
    
    void setRoughness(float roughness) { m_roughness = roughness; }
    float getRoughness() const { return m_roughness; }
    
    // Wireframe properties
    void setWireframeColor(const glm::vec4& color) { m_wireframeColor = color; }
    const glm::vec4& getWireframeColor() const { return m_wireframeColor; }
    
    void setWireframeWidth(float width) { m_wireframeWidth = width; }
    float getWireframeWidth() const { return m_wireframeWidth; }

    // Preset materials
    static MaterialPtr createDefault();
    static MaterialPtr createMetal();
    static MaterialPtr createPlastic();
    static MaterialPtr createGlass();

private:
    glm::vec4 m_diffuseColor;
    glm::vec4 m_specularColor;
    glm::vec4 m_ambientColor;
    glm::vec4 m_wireframeColor;
    
    float m_shininess;
    float m_metallic;
    float m_roughness;
    float m_wireframeWidth;
};
