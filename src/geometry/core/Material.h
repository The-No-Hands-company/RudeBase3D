#pragma once

#include "Common.h"
#include <QVector4D>

class Material {
public:
    Material();
    ~Material() = default;

    // Color properties
    void setDiffuseColor(const QVector4D& color) { m_diffuseColor = color; }
    const QVector4D& getDiffuseColor() const { return m_diffuseColor; }
    
    void setSpecularColor(const QVector4D& color) { m_specularColor = color; }
    const QVector4D& getSpecularColor() const { return m_specularColor; }
    
    void setAmbientColor(const QVector4D& color) { m_ambientColor = color; }
    const QVector4D& getAmbientColor() const { return m_ambientColor; }
    
    // Material properties
    void setShininess(float shininess) { m_shininess = shininess; }
    float getShininess() const { return m_shininess; }
    
    void setMetallic(float metallic) { m_metallic = metallic; }
    float getMetallic() const { return m_metallic; }
    
    void setRoughness(float roughness) { m_roughness = roughness; }
    float getRoughness() const { return m_roughness; }
    
    // Wireframe properties
    void setWireframeColor(const QVector4D& color) { m_wireframeColor = color; }
    const QVector4D& getWireframeColor() const { return m_wireframeColor; }
    
    void setWireframeWidth(float width) { m_wireframeWidth = width; }
    float getWireframeWidth() const { return m_wireframeWidth; }

    // Preset materials
    static MaterialPtr createDefault();
    static MaterialPtr createMetal();
    static MaterialPtr createPlastic();
    static MaterialPtr createGlass();

private:
    QVector4D m_diffuseColor;
    QVector4D m_specularColor;
    QVector4D m_ambientColor;
    QVector4D m_wireframeColor;
    
    float m_shininess;
    float m_metallic;
    float m_roughness;
    float m_wireframeWidth;
};
