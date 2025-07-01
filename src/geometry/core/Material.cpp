#include "Material.h"

Material::Material()
    : m_diffuseColor(0.8f, 0.8f, 0.8f, 1.0f)      // Industry standard gray
    , m_specularColor(0.2f, 0.2f, 0.2f, 1.0f)      // Subtle specular 
    , m_ambientColor(0.3f, 0.3f, 0.3f, 1.0f)       // Strong ambient
    , m_wireframeColor(1.0f, 1.0f, 1.0f, 1.0f)
    , m_shininess(32.0f)
    , m_metallic(0.0f)
    , m_roughness(0.5f)
    , m_wireframeWidth(1.0f)
{
}

MaterialPtr Material::createDefault()
{
    auto material = std::make_shared<Material>();
    // Industry standard 3D modeling gray - Blender's default gray
    material->setDiffuseColor(QVector4D(0.8f, 0.8f, 0.8f, 1.0f));  // Bright gray diffuse
    material->setSpecularColor(QVector4D(0.2f, 0.2f, 0.2f, 1.0f)); // Subtle specular
    material->setAmbientColor(QVector4D(0.3f, 0.3f, 0.3f, 1.0f));  // Strong ambient for visibility
    material->setShininess(32.0f);
    material->setMetallic(0.0f);
    material->setRoughness(0.5f);
    return material;
}

MaterialPtr Material::createMetal()
{
    auto material = std::make_shared<Material>();
    material->setDiffuseColor(QVector4D(0.7f, 0.7f, 0.8f, 1.0f));
    material->setSpecularColor(QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
    material->setAmbientColor(QVector4D(0.1f, 0.1f, 0.1f, 1.0f));
    material->setShininess(128.0f);
    material->setMetallic(1.0f);
    material->setRoughness(0.1f);
    return material;
}

MaterialPtr Material::createPlastic()
{
    auto material = std::make_shared<Material>();
    material->setDiffuseColor(QVector4D(0.8f, 0.2f, 0.2f, 1.0f));
    material->setSpecularColor(QVector4D(0.8f, 0.8f, 0.8f, 1.0f));
    material->setAmbientColor(QVector4D(0.2f, 0.05f, 0.05f, 1.0f));
    material->setShininess(64.0f);
    material->setMetallic(0.0f);
    material->setRoughness(0.3f);
    return material;
}

MaterialPtr Material::createGlass()
{
    auto material = std::make_shared<Material>();
    material->setDiffuseColor(QVector4D(0.9f, 0.9f, 0.9f, 0.3f));
    material->setSpecularColor(QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
    material->setAmbientColor(QVector4D(0.1f, 0.1f, 0.1f, 1.0f));
    material->setShininess(256.0f);
    material->setMetallic(0.0f);
    material->setRoughness(0.0f);
    return material;
}
