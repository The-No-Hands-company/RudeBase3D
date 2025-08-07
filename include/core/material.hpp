//==============================================================================
// RudeBase3D Engine - material.hpp
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

/**
 * @file material.hpp
 * @brief Surface material properties for realistic 3D rendering
 * 
 * This file contains the Material class which defines surface appearance
 * properties for 3D objects. Materials control how surfaces interact with
 * light to create realistic visual effects including color, reflectivity,
 * transparency, and texture mapping.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup Core
 */

#pragma once
#include <glm/glm.hpp>
#include <string>

/**
 * @addtogroup Core
 * @{
 */

/**
 * @brief Surface material properties for physically-based rendering
 * 
 * The Material class encapsulates surface appearance properties that determine
 * how light interacts with 3D object surfaces. It implements a simplified
 * physically-based rendering (PBR) model suitable for real-time 3D applications
 * while maintaining compatibility with traditional Phong/Blinn-Phong shading.
 * 
 * ## Core Properties
 * - **Diffuse Color**: Base color of the material under uniform lighting
 * - **Specular Color**: Color and intensity of reflective highlights
 * - **Shininess**: Controls the size and sharpness of specular highlights
 * - **Opacity**: Controls transparency and alpha blending behavior
 * 
 * ## Shading Models
 * The material supports multiple shading approaches:
 * 
 * ### Phong/Blinn-Phong Shading
 * - **Diffuse**: Lambertian diffuse reflection for matte surfaces
 * - **Specular**: Directional reflection based on surface smoothness
 * - **Shininess**: Exponential falloff for highlight size control
 * 
 * ### Physically-Based Rendering (Future)
 * - **Albedo**: Base color without lighting information
 * - **Metallic**: Metallic vs dielectric surface classification
 * - **Roughness**: Surface microsurface detail affecting reflections
 * - **Normal Maps**: Surface detail without geometry complexity
 * 
 * ## Texture Integration
 * Materials support texture-based property variation:
 * - **Diffuse/Albedo Maps**: Spatial color variation
 * - **Normal Maps**: Surface detail through normal perturbation
 * - **Specular/Metallic Maps**: Spatially-varying reflectance properties
 * 
 * ## Usage Examples
 * ```cpp
 * // Create basic colored material
 * Material red(glm::vec3(0.8f, 0.2f, 0.2f), glm::vec3(1.0f), 64.0f);
 * 
 * // Create metallic material
 * Material metal;
 * metal.diffuse = glm::vec3(0.7f, 0.7f, 0.8f);
 * metal.specular = glm::vec3(0.9f, 0.9f, 1.0f);
 * metal.shininess = 128.0f;
 * 
 * // Create glass-like material
 * Material glass;
 * glass.diffuse = glm::vec3(0.1f, 0.3f, 0.6f);
 * glass.specular = glm::vec3(1.0f);
 * glass.shininess = 96.0f;
 * glass.opacity = 0.3f;
 * 
 * // Add texture support
 * metal.diffuseTexture = "assets/textures/metal_diffuse.jpg";
 * metal.normalTexture = "assets/textures/metal_normal.jpg";
 * ```
 * 
 * ## Material Presets
 * Common material configurations:
 * - **Matte Plastic**: Low specular, medium diffuse, low shininess
 * - **Polished Metal**: High specular, colored diffuse, high shininess
 * - **Rough Metal**: Medium specular, colored diffuse, medium shininess
 * - **Glass/Crystal**: Low diffuse, high specular, very high shininess, low opacity
 * - **Wood/Fabric**: Medium diffuse, low specular, low shininess
 * 
 * ## Performance Considerations
 * - Materials with opacity < 1.0 require alpha blending (slower rendering)
 * - High shininess values create sharper but more expensive highlights
 * - Texture loading should be managed for memory efficiency
 * - Material switching can impact rendering performance
 * 
 * @see Mesh for geometry association with materials
 * @see Shader for material property implementation
 * @see Texture for texture mapping details
 * 
 * @ingroup Core
 */
class Material {
public:
    // ========================================================================
    // Construction and Initialization
    // ========================================================================
    
    /**
     * @brief Default constructor
     * 
     * Creates a material with neutral gray appearance suitable for basic
     * 3D object rendering. The default values provide a matte plastic-like
     * appearance that works well for prototype and debug visualization.
     * 
     * @post Material has neutral gray diffuse color (0.8, 0.8, 0.8)
     * @post Material has white specular color (1.0, 1.0, 1.0)
     * @post Material has medium shininess (32.0)
     * @post Material is fully opaque (opacity = 1.0)
     * @post No textures are assigned
     */
    Material();
    
    /**
     * @brief Construct material with specific shading properties
     * 
     * Creates a material with custom diffuse and specular colors and
     * shininess value. This constructor provides quick setup for common
     * material types without texture mapping.
     * 
     * @param diffuse Base color of the material (RGB, each component 0-1)
     * @param specular Reflective highlight color (RGB, each component 0-1)
     * @param shininess Specular highlight sharpness (typically 1-128)
     * 
     * @pre All color components should be in range [0, 1]
     * @pre shininess should be > 0 (typically 1-128 for realistic materials)
     * 
     * @post Material uses specified diffuse and specular colors
     * @post Material uses specified shininess value
     * @post Material is fully opaque (opacity = 1.0)
     * @post No textures are assigned
     * 
     * @note Higher shininess values create smaller, sharper highlights
     * @note Shininess values above 128 may not provide visually significant improvement
     */
    Material(const glm::vec3& diffuse, const glm::vec3& specular, float shininess);

    // ========================================================================
    // Basic Material Properties
    // ========================================================================
    
    /**
     * @brief Base color of the material surface
     * 
     * Defines the intrinsic color of the material under uniform lighting
     * conditions. This color is most visible on surfaces facing light sources
     * and represents the material's absorption/reflection characteristics.
     * 
     * ## Color Guidelines
     * - **Metals**: Typically colored (gold: warm yellow, copper: orange-red)
     * - **Dielectrics**: Often neutral (white, gray) or colored (plastic, paint)
     * - **Natural Materials**: Earth tones, organic colors
     * 
     * ## Value Ranges
     * - Each component (R, G, B) should be in range [0, 1]
     * - Values near 0 create dark, light-absorbing surfaces
     * - Values near 1 create bright, light-reflecting surfaces
     * 
     * @note This property corresponds to kd (diffuse coefficient) in Phong shading
     * @note For PBR workflows, this becomes the albedo color
     */
    glm::vec3 diffuse{0.8f, 0.8f, 0.8f};
    
    /**
     * @brief Reflective highlight color and intensity
     * 
     * Controls the color and brightness of specular reflections that create
     * shiny highlights on the surface. For realistic materials, this is often
     * white or tinted toward the material's characteristic reflection color.
     * 
     * ## Material-Specific Guidelines
     * - **Metals**: Colored specular matching the metal's reflection (gold: warm, silver: cool)
     * - **Dielectrics**: Usually white or very light gray (most non-metals)
     * - **Plastics**: White to slightly colored based on surface coating
     * 
     * ## Intensity Control
     * - Higher values create more prominent reflections
     * - Lower values create subtle, matte appearances
     * - Black (0,0,0) completely disables specular reflection
     * 
     * @note This property corresponds to ks (specular coefficient) in Phong shading
     * @note For metals, specular color should match or complement diffuse color
     */
    glm::vec3 specular{1.0f, 1.0f, 1.0f};
    
    /**
     * @brief Surface smoothness and highlight sharpness
     * 
     * Controls the size and sharpness of specular highlights. Higher values
     * create smaller, sharper highlights (smooth surfaces), while lower values
     * create larger, softer highlights (rough surfaces).
     * 
     * ## Common Value Ranges
     * - **1-8**: Very rough surfaces (chalk, unfinished wood, rubber)
     * - **8-32**: Rough to medium surfaces (matte plastic, fabric, leather)
     * - **32-64**: Medium to smooth surfaces (satin paint, polished wood)
     * - **64-128**: Smooth to very smooth (gloss paint, polished metal)
     * - **128+**: Mirror-like surfaces (chrome, glass, water)
     * 
     * ## Technical Details
     * - Mathematical relationship: highlight_size ∝ 1/shininess
     * - Exponential falloff in Phong/Blinn-Phong models
     * - Values above 128 provide diminishing visual returns
     * 
     * @note This property corresponds to the shininess exponent in Phong shading
     * @note In PBR workflows, this relates to (1 - roughness)
     */
    float shininess{32.0f};
    
    /**
     * @brief Material transparency and alpha blending
     * 
     * Controls the transparency of the material surface. Values less than 1.0
     * enable alpha blending, allowing background objects to show through the
     * material. This is essential for glass, water, and other transparent materials.
     * 
     * ## Opacity Guidelines
     * - **1.0**: Completely opaque (solid objects, most materials)
     * - **0.7-0.9**: Translucent materials (tinted glass, plastics)
     * - **0.3-0.7**: Semi-transparent materials (frosted glass, gels)
     * - **0.1-0.3**: Highly transparent materials (clear glass, water)
     * - **0.0**: Completely transparent (invisible)
     * 
     * ## Rendering Implications
     * - Opacity < 1.0 requires alpha blending (performance impact)
     * - Transparent objects need depth sorting for correct rendering
     * - Refraction effects may require additional shader complexity
     * 
     * @note This property is used for alpha channel in RGBA rendering
     * @note Transparent materials often have reduced diffuse values
     */
    float opacity{1.0f};

    // ========================================================================
    // Texture Mapping Support
    // ========================================================================
    
    /**
     * @brief Path to diffuse/albedo texture image
     * 
     * Specifies the file path to a texture image that provides spatial
     * variation in the material's base color. The texture is sampled using
     * UV coordinates to map the image onto the 3D surface.
     * 
     * ## Supported Formats
     * - **Common**: PNG, JPG/JPEG, TGA, BMP
     * - **HDR**: EXR, HDR (for high dynamic range textures)
     * - **Compressed**: DDS, KTX (for optimized loading)
     * 
     * ## Usage Guidelines
     * - Use high-resolution textures for close-up surfaces
     * - Consider texture compression for memory efficiency
     * - Ensure textures tile seamlessly for repeating patterns
     * - Use gamma-corrected images for diffuse textures
     * 
     * ## Path Conventions
     * - Relative paths are resolved from project asset directory
     * - Use forward slashes (/) for cross-platform compatibility
     * - Example: "assets/textures/wood_diffuse.jpg"
     * 
     * @note Empty string indicates no texture (use material color)
     * @note Texture loading is handled by the rendering system
     * @note UV coordinates must be provided by the mesh geometry
     */
    std::string diffuseTexture;
    
    /**
     * @brief Path to normal map texture for surface detail
     * 
     * Specifies the file path to a normal map texture that provides fine
     * surface detail without additional geometry complexity. Normal maps
     * encode surface normal perturbations in RGB channels to simulate
     * bumps, scratches, and other surface features.
     * 
     * ## Normal Map Types
     * - **Tangent Space**: Most common, encoded relative to surface tangent
     * - **Object Space**: Encoded in object coordinates (less flexible)
     * - **World Space**: Encoded in world coordinates (rarely used)
     * 
     * ## Encoding Formats
     * - **DirectX Style**: Y-up, green channel represents "up" direction
     * - **OpenGL Style**: Y-down, green channel represents "down" direction
     * - **Height Maps**: Grayscale height data converted to normals
     * 
     * ## Quality Guidelines
     * - Use same resolution as diffuse texture for consistency
     * - Ensure normal map matches the surface's detail level
     * - Higher contrast creates more pronounced surface features
     * - Blue channel should contain positive Z normals (outward-facing)
     * 
     * @note Normal maps require tangent-space vectors in vertex data
     * @note Empty string disables normal mapping
     * @note Normal intensity can be adjusted in shaders
     */
    std::string normalTexture;
    
    /**
     * @brief Path to specular/metallic map texture
     * 
     * Specifies the file path to a texture that controls spatial variation
     * in specular reflection properties. This allows different areas of the
     * surface to have varying reflectivity, enabling complex material effects.
     * 
     * ## Traditional Workflow (Specular Map)
     * - **Grayscale**: White areas are highly reflective, black areas are matte
     * - **RGB**: Colored specular reflection for non-metallic surfaces
     * - **Usage**: Controls specular intensity and color variation
     * 
     * ## PBR Workflow (Metallic Map)
     * - **Grayscale**: White areas are metallic, black areas are dielectric
     * - **Binary Values**: Typically pure white (1.0) or pure black (0.0)
     * - **Usage**: Classifies surface as metallic or non-metallic
     * 
     * ## Combined Maps
     * - **R Channel**: Metallic values (0=dielectric, 1=metallic)
     * - **G Channel**: Roughness values (0=smooth, 1=rough)
     * - **B Channel**: Additional properties (occlusion, height, etc.)
     * 
     * @note Map type interpretation depends on shader implementation
     * @note Empty string uses uniform specular properties
     * @note Consider workflow consistency (traditional vs PBR)
     */
    std::string specularTexture;
};

/**
 * @}
 */ 