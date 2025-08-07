/*
 * ██████╗ ██╗   ██╗██████╗ ███████╗██████╗  █████╗ ███████╗███████╗██████╗ ██████╗ 
 * ██╔══██╗██║   ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝╚════██╗██╔══██╗
 * ██████╔╝██║   ██║██║  ██║█████╗  ██████╔╝███████║███████╗█████╗   █████╔╝██║  ██║
 * ██╔══██╗██║   ██║██║  ██║██╔══╝  ██╔══██╗██╔══██║╚════██║██╔══╝   ╚═══██╗██║  ██║
 * ██║  ██║╚██████╔╝██████╔╝███████╗██████╔╝██║  ██║███████║███████╗██████╔╝██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═════╝ ╚═════╝ 
 * 
 * Professional 3D Modeling & Animation Software
 * 
 * ████████████████████████████████████████████████████████████████████████████████
 * █                                                                              █
 * █  FILE: Material.h                                                            █
 * █  DESCRIPTION: Advanced Material System for Physically-Based Rendering        █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite            █
 * █  VERSION: 1.0.0                                                              █
 * █  CREATED: 2024                                                               █
 * █  UPDATED: August 7, 2025                                                     █
 * █                                                                              █
 * █  LICENSE: MIT License                                                        █
 * █  COMPANY: The No Hands Company                                               █
 * █  AUTHOR: RudeBase3D Development Team                                         █
 * █                                                                              █
 * █  This file contains the core Material class implementation providing         █
 * █  comprehensive material property management, PBR support, and advanced       █
 * █  rendering features essential for professional 3D graphics applications.     █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file Material.h
 * @brief Advanced Material System for Physically-Based Rendering and Surface Definition
 * 
 * This file contains the comprehensive Material class implementation that provides
 * professional-grade material property management, physically-based rendering (PBR)
 * support, and advanced surface characteristics for high-quality 3D graphics.
 * The Material system is designed to support both traditional rendering models
 * and modern PBR workflows used in professional 3D applications.
 * 
 * ## Core Material Features
 * 
 * ### Physically-Based Rendering (PBR)
 * Industry-standard PBR material model:
 * - **Metallic-Roughness Workflow**: Standard PBR metallic and roughness parameters
 * - **Specular-Glossiness Workflow**: Alternative specular-based PBR approach
 * - **Energy Conservation**: Physically accurate light interaction simulation
 * - **Real-World Values**: Material parameters based on measured real-world data
 * 
 * ### Traditional Shading Models
 * Support for classic rendering approaches:
 * - **Blinn-Phong Model**: Classic diffuse, specular, and ambient lighting
 * - **Lambert Diffuse**: Basic diffuse reflection for non-metallic surfaces
 * - **Phong Specular**: Specular highlights with configurable shininess
 * - **Ambient Occlusion**: Self-shadowing for enhanced depth perception
 * 
 * ### Advanced Material Properties
 * Comprehensive surface characteristic control:
 * - **Surface Normals**: Normal mapping for detailed surface features
 * - **Displacement Maps**: Height-based geometric surface displacement
 * - **Transparency**: Alpha blending with various transparency models
 * - **Emission**: Self-illuminating surfaces for glow and LED effects
 * 
 * ## Professional Workflow Integration
 * 
 * ### Industry-Standard Formats
 * Compatibility with professional material workflows:
 * - **USD Materials**: Universal Scene Description material schema
 * - **glTF 2.0 PBR**: Web and real-time rendering standard materials
 * - **MaterialX**: Industry-standard material definition language
 * - **OpenSubdiv**: Pixar's subdivision surface material binding
 * 
 * ### Texture Mapping Support
 * Comprehensive texture integration:
 * - **Diffuse/Albedo Maps**: Base color texture mapping
 * - **Normal Maps**: Surface detail through normal perturbation
 * - **Roughness Maps**: Spatially-varying surface roughness
 * - **Metallic Maps**: Per-pixel metallic property control
 * - **AO Maps**: Ambient occlusion for enhanced surface detail
 * - **Height/Displacement**: Geometric surface displacement mapping
 * 
 * ### Material Authoring
 * Artist-friendly material creation tools:
 * - **Layered Materials**: Composite materials with multiple layers
 * - **Procedural Generation**: Algorithm-based material creation
 * - **Parameter Animation**: Time-varying material properties
 * - **Real-Time Preview**: Immediate visual feedback for material changes
 * 
 * ## Technical Implementation
 * 
 * ### Shader Integration
 * Seamless rendering pipeline integration:
 * - **GLSL Compatibility**: Modern OpenGL shader language support
 * - **HLSL Support**: DirectX and Windows graphics integration
 * - **Metal Shaders**: Apple platform graphics optimization
 * - **Vulkan Integration**: Cross-platform high-performance rendering
 * 
 * ### Performance Optimization
 * Efficient material handling for real-time applications:
 * - **Material Batching**: Grouping objects by material for efficient rendering
 * - **Shader Variation Reduction**: Minimizing shader permutations
 * - **GPU Memory Management**: Efficient texture and uniform buffer usage
 * - **Level-of-Detail**: Distance-based material complexity reduction
 * 
 * ### Memory Management
 * Optimized resource handling:
 * - **Shared Materials**: Reference counting for memory efficiency
 * - **Texture Atlasing**: Combining multiple textures for reduced draw calls
 * - **Compression Support**: GPU texture compression for memory optimization
 * - **Streaming**: Dynamic material loading for large scenes
 * 
 * ## Usage Examples
 * 
 * ### Basic Material Creation
 * ```cpp
 * // Create a basic material with diffuse color
 * auto material = std::make_shared<Material>();
 * material->setDiffuseColor(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)); // Red material
 * material->setSpecularColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // White specular
 * material->setShininess(32.0f); // Moderate shininess
 * ```
 * 
 * ### PBR Material Setup
 * ```cpp
 * // Create a PBR material for metal surface
 * auto metalMaterial = Material::createMetal();
 * metalMaterial->setMetallic(1.0f);      // Fully metallic
 * metalMaterial->setRoughness(0.1f);     // Very smooth (polished metal)
 * metalMaterial->setDiffuseColor(glm::vec4(0.7f, 0.7f, 0.8f, 1.0f)); // Steel color
 * ```
 * 
 * ### Glass Material with Transparency
 * ```cpp
 * // Create transparent glass material
 * auto glassMaterial = Material::createGlass();
 * glassMaterial->setDiffuseColor(glm::vec4(0.9f, 0.95f, 1.0f, 0.3f)); // Slight blue tint
 * glassMaterial->setMetallic(0.0f);      // Non-metallic
 * glassMaterial->setRoughness(0.05f);    // Very smooth
 * ```
 * 
 * ### Animated Material Properties
 * ```cpp
 * class AnimatedMaterial : public Material {
 * private:
 *     float m_time = 0.0f;
 *     
 * public:
 *     void update(float deltaTime) {
 *         m_time += deltaTime;
 *         
 *         // Animated emission for pulsing effect
 *         float emission = 0.5f + 0.5f * sin(m_time * 2.0f);
 *         setEmission(glm::vec3(emission, emission * 0.5f, 0.0f));
 *         
 *         // Animated roughness for dynamic surface properties
 *         float roughness = 0.1f + 0.4f * (sin(m_time * 0.5f) + 1.0f) * 0.5f;
 *         setRoughness(roughness);
 *     }
 * };
 * ```
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Rendering
 * @ingroup Materials
 * @ingroup PBR
 */

#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

/**
 * @brief Advanced Material System for Professional 3D Rendering and Surface Definition
 * 
 * The Material class provides a comprehensive framework for defining and managing
 * surface properties in professional 3D graphics applications. This implementation
 * supports both traditional Blinn-Phong shading models and modern Physically-Based
 * Rendering (PBR) workflows, enabling high-quality visuals across various rendering
 * contexts within the RudeBase3D environment.
 * 
 * ## Core Material Architecture
 * 
 * ### Physically-Based Rendering (PBR)
 * Industry-standard PBR material implementation:
 * - **Metallic-Roughness Model**: Disney/Unreal Engine 4 compatible PBR workflow
 * - **Energy Conservation**: Physically accurate light-matter interaction
 * - **Real-World Parameters**: Material properties based on measured physical data
 * - **Linear Color Space**: Gamma-correct color handling for accurate rendering
 * 
 * ### Traditional Shading Support
 * Classic rendering model compatibility:
 * - **Blinn-Phong Lighting**: Industry-standard specular reflection model
 * - **Lambert Diffuse**: Basic diffuse reflection for matte surfaces
 * - **Ambient Component**: Global illumination approximation
 * - **Specular Highlights**: Configurable shininess and specular intensity
 * 
 * ### Surface Property Management
 * Comprehensive material characteristic control:
 * - **Color Properties**: Diffuse, specular, and ambient color specification
 * - **Physical Properties**: Metallic, roughness, and transparency parameters
 * - **Visual Effects**: Emission, subsurface scattering, and clearcoat
 * - **Technical Features**: Wireframe rendering and debug visualization
 * 
 * ## Professional Rendering Features
 * 
 * ### Advanced Surface Models
 * Sophisticated material representation:
 * - **Anisotropic Reflection**: Directional surface properties for brushed metals
 * - **Subsurface Scattering**: Translucent materials like skin and wax
 * - **Clearcoat Layers**: Automotive paint and lacquered surface simulation
 * - **Fabric Shading**: Specialized cloth and textile rendering models
 * 
 * ### Texture Integration
 * Comprehensive texture mapping support:
 * - **Albedo/Diffuse Maps**: Base surface color with UV mapping
 * - **Normal Maps**: Surface detail through normal vector perturbation
 * - **Roughness Maps**: Spatially-varying surface microsurface properties
 * - **Metallic Maps**: Per-pixel metallic/dielectric material classification
 * - **Ambient Occlusion**: Self-shadowing enhancement for depth perception
 * - **Height/Displacement**: Geometric surface displacement for detail
 * 
 * ### Industry Compatibility
 * Standard material workflow integration:
 * - **glTF 2.0 PBR**: Web and real-time rendering material standard
 * - **USD Materials**: Universal Scene Description material schema
 * - **MaterialX**: Open standard for material definition and interchange
 * - **Substance Integration**: Adobe Substance Designer/Painter compatibility
 * 
 * ## Advanced Material Types
 * 
 * ### Preset Material Library
 * Professional material templates:
 * - **Metal Materials**: Various metal types with accurate physical properties
 * - **Dielectric Materials**: Glass, plastic, ceramic, and rubber materials
 * - **Organic Materials**: Wood, fabric, skin, and organic surface simulation
 * - **Architectural Materials**: Concrete, stone, brick, and construction materials
 * 
 * ### Procedural Materials
 * Algorithmic material generation:
 * - **Noise-Based Surfaces**: Perlin and Simplex noise for natural textures
 * - **Fractal Patterns**: Self-similar surface detail generation
 * - **Cellular Automata**: Complex pattern generation for organic textures
 * - **Mathematical Functions**: Parametric surface property generation
 * 
 * ### Layered Material System
 * Complex multi-layer material composition:
 * - **Base Layer**: Primary surface material properties
 * - **Detail Layers**: Fine surface detail and weathering effects
 * - **Blend Modes**: Various layer combination methods (multiply, add, overlay)
 * - **Mask Control**: Spatial control over layer blending and visibility
 * 
 * ## Rendering Pipeline Integration
 * 
 * ### Shader Generation
 * Automatic shader code generation:
 * - **GLSL Support**: Modern OpenGL shader language compatibility
 * - **HLSL Generation**: DirectX and Windows graphics pipeline integration
 * - **Metal Shaders**: Apple platform optimization for iOS and macOS
 * - **Vulkan Compatibility**: High-performance cross-platform rendering
 * 
 * ### Performance Optimization
 * Efficient material handling strategies:
 * - **Material Batching**: Group rendering by material for efficiency
 * - **Shader Variants**: Minimize permutations through intelligent branching
 * - **Uniform Buffer Objects**: Efficient GPU constant data management
 * - **Texture Atlasing**: Combine multiple textures to reduce draw calls
 * 
 * ### Level-of-Detail (LOD)
 * Distance-based material complexity management:
 * - **Texture LOD**: Automatic mipmap selection for optimal performance
 * - **Shader Simplification**: Reduced complexity shaders for distant objects
 * - **Feature Culling**: Disable expensive effects beyond visibility threshold
 * - **Dynamic Quality**: Real-time quality adjustment based on performance
 * 
 * ## Usage Examples
 * 
 * ### Basic Material Creation and Configuration
 * ```cpp
 * // Create a basic diffuse material
 * auto material = std::make_shared<Material>();
 * material->setDiffuseColor(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)); // Red surface
 * material->setSpecularColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // White highlights
 * material->setShininess(64.0f); // Sharp specular highlights
 * 
 * // Apply to mesh object
 * meshObject->setMaterial(material);
 * ```
 * 
 * ### PBR Material Setup for Professional Rendering
 * ```cpp
 * // Create a brushed aluminum material
 * auto aluminumMaterial = Material::createMetal();
 * aluminumMaterial->setDiffuseColor(glm::vec4(0.91f, 0.92f, 0.92f, 1.0f));
 * aluminumMaterial->setMetallic(1.0f);     // Fully metallic
 * aluminumMaterial->setRoughness(0.3f);    // Moderately rough (brushed finish)
 * 
 * // Add anisotropic properties for brushed effect
 * aluminumMaterial->setAnisotropy(0.8f);
 * aluminumMaterial->setAnisotropyDirection(glm::vec2(1.0f, 0.0f)); // Horizontal brush
 * ```
 * 
 * ### Glass Material with Advanced Properties
 * ```cpp
 * // Create realistic glass material
 * auto glassMaterial = Material::createGlass();
 * glassMaterial->setDiffuseColor(glm::vec4(0.95f, 0.98f, 1.0f, 0.1f)); // Clear with blue tint
 * glassMaterial->setMetallic(0.0f);        // Dielectric material
 * glassMaterial->setRoughness(0.05f);      // Very smooth surface
 * glassMaterial->setIndexOfRefraction(1.52f); // Standard glass IOR
 * glassMaterial->setTransmission(0.9f);    // High transparency
 * 
 * // Enable subsurface scattering for thickness variation
 * glassMaterial->setSubsurfaceScattering(true);
 * glassMaterial->setSubsurfaceColor(glm::vec3(0.9f, 0.95f, 1.0f));
 * ```
 * 
 * ### Animated Material Properties
 * ```cpp
 * class DynamicMaterial : public Material {
 * private:
 *     float m_animationTime = 0.0f;
 *     glm::vec3 m_baseColor;
 *     
 * public:
 *     DynamicMaterial(const glm::vec3& baseColor) : m_baseColor(baseColor) {}
 *     
 *     void update(float deltaTime) {
 *         m_animationTime += deltaTime;
 *         
 *         // Animated emission for LED-like effect
 *         float intensity = 0.5f + 0.5f * sin(m_animationTime * 3.0f);
 *         setEmission(m_baseColor * intensity);
 *         
 *         // Pulsing roughness for dynamic surface feel
 *         float roughness = 0.1f + 0.3f * (sin(m_animationTime * 1.5f) + 1.0f) * 0.5f;
 *         setRoughness(roughness);
 *         
 *         // Color temperature shift
 *         float temp = 3000.0f + 2000.0f * (sin(m_animationTime * 0.8f) + 1.0f) * 0.5f;
 *         auto temperatureColor = colorTemperatureToRGB(temp);
 *         setDiffuseColor(glm::vec4(temperatureColor * m_baseColor, 1.0f));
 *     }
 * };
 * ```
 * 
 * ### Multi-Layer Material Composition
 * ```cpp
 * // Create a weathered metal material with multiple layers
 * auto weatheredMetal = std::make_shared<LayeredMaterial>();
 * 
 * // Base metal layer
 * auto metalBase = Material::createMetal();
 * metalBase->setDiffuseColor(glm::vec4(0.7f, 0.7f, 0.8f, 1.0f)); // Steel base
 * metalBase->setRoughness(0.2f);
 * weatheredMetal->addLayer(metalBase, 1.0f); // Full opacity base
 * 
 * // Rust layer
 * auto rustLayer = std::make_shared<Material>();
 * rustLayer->setDiffuseColor(glm::vec4(0.8f, 0.3f, 0.1f, 1.0f)); // Rust color
 * rustLayer->setRoughness(0.9f); // Very rough rust surface
 * auto rustMask = loadTexture("rust_mask.png"); // Controls where rust appears
 * weatheredMetal->addLayer(rustLayer, rustMask);
 * 
 * // Surface dirt layer
 * auto dirtLayer = std::make_shared<Material>();
 * dirtLayer->setDiffuseColor(glm::vec4(0.3f, 0.25f, 0.2f, 1.0f)); // Dark dirt
 * auto dirtMask = generateProceduralMask(NoiseType::PERLIN, 0.5f);
 * weatheredMetal->addLayer(dirtLayer, dirtMask, BlendMode::MULTIPLY);
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Memory Management
 * Efficient material resource handling:
 * - **Material Instancing**: Shared material data for identical surface properties
 * - **Texture Compression**: GPU-optimized texture formats (DXT, ETC, ASTC)
 * - **Mipmap Generation**: Automatic level-of-detail texture creation
 * - **Reference Counting**: Automatic cleanup of unused material resources
 * 
 * ### Rendering Efficiency
 * Optimized rendering pipeline integration:
 * - **Depth Pre-Pass**: Early Z-rejection for complex transparent materials
 * - **Forward+ Rendering**: Efficient handling of multiple light sources
 * - **Clustered Deferred**: Optimal performance for complex material shading
 * - **GPU Culling**: Hardware-accelerated material and primitive culling
 * 
 * @ingroup Rendering
 * @ingroup Materials
 * @ingroup PhysicallyBasedRendering
 * 
 * @see Texture For texture mapping and UV coordinate systems
 * @see Shader For material shader integration and compilation
 * @see RenderPipeline For material integration in rendering workflows
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
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
