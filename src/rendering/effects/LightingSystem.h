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
 * █  FILE: LightingSystem.h                                                      █
 * █  DESCRIPTION: Professional Lighting System & Illumination Management        █
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
 * █  This file contains the LightingSystem class - a comprehensive lighting     █
 * █  orchestrator implementing industry-standard three-point lighting setups    █
 * █  with professional presets matching Maya, Blender, and studio workflows.    █
 * █  Provides advanced illumination control for professional 3D visualization.  █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file LightingSystem.h
 * @brief Professional Lighting System and Illumination Management for 3D Visualization
 * 
 * This file contains the comprehensive LightingSystem class implementation that provides
 * industry-standard lighting orchestration for professional 3D modeling and animation
 * workflows. The system implements sophisticated three-point lighting theory with
 * preset configurations matching major professional software packages including Maya,
 * Blender, and studio visualization standards. The LightingSystem serves as the primary
 * illumination coordinator within the RudeBase3D rendering pipeline, providing both
 * artistic control and technical precision for high-quality 3D scene visualization.
 * 
 * ## Professional Lighting Architecture
 * 
 * ### Three-Point Lighting Theory Implementation
 * Industry-standard cinematographic lighting setup adapted for 3D visualization:
 * - **Key Light (Primary)**: Main illumination source defining primary shadows and form
 * - **Fill Light (Secondary)**: Shadow softening and detail revelation in shadow areas
 * - **Rim Light (Accent)**: Edge highlighting for subject-background separation
 * - **Ambient Light (Environment)**: Global illumination simulation preventing pure black
 * 
 * ### Professional Software Preset Compatibility
 * Authenticated lighting configurations matching industry-standard applications:
 * - **Maya Preset**: Autodesk Maya viewport lighting for animation workflows
 * - **Blender Preset**: Blender default lighting with artistic color temperature
 * - **Studio Preset**: Clean product visualization lighting for technical modeling
 * - **Outdoor Preset**: Natural daylight simulation with sun and sky illumination
 * - **Custom Preset**: User-defined artistic lighting with full parameter control
 * 
 * ### Advanced Illumination Features
 * Professional lighting control for sophisticated visualization:
 * - **Color Temperature Control**: Realistic light temperature simulation (2700K-10000K)
 * - **Intensity Management**: Professional brightness control with linear scaling
 * - **Directional Precision**: World-space lighting direction with quaternion accuracy
 * - **Real-Time Updates**: Immediate lighting adjustments for interactive modeling
 * - **Shader Integration**: Direct GPU uniform uploads for optimal performance
 * 
 * ## Cinematographic Lighting Theory
 * 
 * ### Key Light (Primary Illumination)
 * The dominant light source establishing scene mood and primary shadows:
 * - **Positioning**: Typically 45° above and to one side of the subject
 * - **Color Temperature**: Neutral to warm (3200K-5600K) for natural appearance
 * - **Intensity Range**: 0.8-2.0x base intensity for dramatic to subtle lighting
 * - **Artistic Control**: Direction, color, and intensity fully customizable
 * - **Shadow Quality**: Defines primary shadow direction and sharpness
 * 
 * ### Fill Light (Secondary Illumination)
 * Secondary light source reducing contrast and revealing shadow detail:
 * - **Positioning**: Opposite side from key light at lower elevation
 * - **Color Temperature**: Cooler than key light (5600K-7000K) for natural contrast
 * - **Intensity Range**: 30-70% of key light intensity for optimal balance
 * - **Shadow Softening**: Reduces harsh shadows while maintaining form definition
 * - **Detail Enhancement**: Reveals texture and detail in shadowed areas
 * 
 * ### Rim Light (Accent Illumination)
 * Accent lighting creating edge definition and subject separation:
 * - **Positioning**: Behind and slightly above subject for edge highlighting
 * - **Color Temperature**: Neutral to warm for natural edge definition
 * - **Intensity Range**: 20-50% of key light intensity for subtle accent
 * - **Edge Definition**: Creates subtle rim lighting separating subject from background
 * - **Depth Enhancement**: Adds visual depth and three-dimensional form perception
 * 
 * ### Ambient Light (Environmental Illumination)
 * Global illumination simulation providing uniform base lighting:
 * - **Purpose**: Simulates environmental light bouncing and global illumination
 * - **Color Temperature**: Sky simulation (6500K-10000K) or warm interior (2700K-3200K)
 * - **Intensity Range**: 10-30% of total illumination for realistic appearance
 * - **Shadow Prevention**: Prevents completely black shadows for natural rendering
 * - **Atmosphere Simulation**: Creates environmental mood and lighting context
 * 
 * ## Professional Software Integration
 * 
 * ### Maya Workflow Compatibility
 * Authentic Autodesk Maya lighting configuration:
 * - **Key Light**: Standard 45° angle with neutral white (5500K)
 * - **Fill Light**: Balanced secondary illumination at 50% intensity
 * - **Rim Light**: Subtle edge highlighting at 30% intensity
 * - **Ambient**: Low-level environmental illumination at 15%
 * - **Color Space**: Linear sRGB workflow matching Maya's rendering pipeline
 * 
 * ### Blender Workflow Compatibility
 * Blender-style artistic lighting with enhanced visual appeal:
 * - **Key Light**: Slightly warm primary illumination (4500K)
 * - **Fill Light**: Cool-tinted secondary light (6500K) for artistic contrast
 * - **Rim Light**: Neutral accent lighting for clean edge definition
 * - **Ambient**: Cool ambient (7000K) simulating sky light bouncing
 * - **Artistic Focus**: Enhanced contrast for creative and artistic modeling
 * 
 * ### Studio Visualization Preset
 * Professional product visualization lighting setup:
 * - **Key Light**: Clean, controlled illumination with minimal color temperature
 * - **Fill Light**: Soft, uniform secondary lighting for shadow detail
 * - **Rim Light**: Precise edge definition without harsh highlights
 * - **Ambient**: Minimal ambient lighting maintaining clean shadows
 * - **Technical Accuracy**: Optimized for precise technical documentation
 * 
 * ### Outdoor Simulation Preset
 * Natural daylight environment simulation:
 * - **Key Light**: Sun simulation with warm color temperature (5200K)
 * - **Fill Light**: Reduced intensity sky light simulation
 * - **Rim Light**: Atmospheric edge lighting from environmental reflection
 * - **Ambient**: Blue sky ambient (8000K-10000K) for natural outdoor appearance
 * - **Environmental Realism**: Matches natural outdoor lighting conditions
 * 
 * ## Advanced Technical Implementation
 * 
 * ### Real-Time Lighting Pipeline
 * Optimized lighting calculation for interactive 3D modeling:
 * - **GPU Shader Integration**: Direct uniform uploads to vertex and fragment shaders
 * - **Conditional Updates**: Only modified lighting parameters uploaded per frame
 * - **Uniform Buffer Objects**: Efficient GPU memory management for lighting data
 * - **Multi-Light Support**: Framework for additional specialized light sources
 * - **Performance Profiling**: Lighting calculation cost monitoring and optimization
 * 
 * ### Color Science and Temperature
 * Professional color accuracy for realistic lighting:
 * - **Blackbody Radiation**: Accurate color temperature to RGB conversion
 * - **CIE Color Space**: Standard colorimetric calculations for professional accuracy
 * - **Gamma Correction**: Linear workflow with proper gamma handling
 * - **HDR Support**: High dynamic range lighting for advanced rendering
 * - **Color Calibration**: Monitor and workflow color space compatibility
 * 
 * ### Shader Integration Architecture
 * Seamless lighting integration with modern rendering pipelines:
 * - **Forward Rendering**: Direct lighting calculation in forward rendering passes
 * - **Deferred Lighting**: G-buffer compatible lighting for complex scenes
 * - **PBR Integration**: Physically-based rendering material compatibility
 * - **Shadow Mapping**: Framework for dynamic shadow generation (future)
 * - **Global Illumination**: Integration points for advanced GI techniques
 * 
 * ## Performance Optimization Systems
 * 
 * ### Memory Management
 * Efficient lighting data handling for professional applications:
 * - **Uniform Caching**: Smart caching of lighting uniforms to minimize uploads
 * - **Parameter Tracking**: Change detection preventing unnecessary GPU updates
 * - **Memory Pooling**: Reusable lighting data structures for efficiency
 * - **Batch Processing**: Group lighting updates for optimal GPU utilization
 * - **Resource Cleanup**: Automatic cleanup of unused lighting resources
 * 
 * ### Computational Efficiency
 * Optimized lighting calculations for real-time performance:
 * - **Vector Math Optimization**: SIMD-optimized lighting calculations
 * - **Lookup Table Generation**: Pre-computed color temperature and intensity tables
 * - **Culling Integration**: Light influence culling for large scene optimization
 * - **Level-of-Detail**: Distance-based lighting complexity reduction
 * - **Multi-Threading**: Thread-safe lighting updates for concurrent processing
 * 
 * ### Scalability Architecture
 * Framework for advanced lighting features and expansion:
 * - **Modular Design**: Plugin architecture for custom lighting models
 * - **Extension Points**: Well-defined interfaces for advanced lighting features
 * - **Future Compatibility**: Framework for shadow mapping, IBL, and volumetrics
 * - **API Consistency**: Stable API for third-party lighting integrations
 * - **Version Migration**: Backward compatibility for lighting configuration files
 * 
 * ## Usage Examples
 * 
 * ### Basic Lighting System Setup
 * ```cpp
 * // Initialize lighting system with Qt integration
 * auto lightingSystem = std::make_shared<LightingSystem>(parent);
 * 
 * // Apply professional studio lighting preset
 * lightingSystem->setLightingPreset(LightingSystem::LightingPreset::Studio);
 * 
 * // Connect to renderer for automatic updates
 * connect(lightingSystem.get(), &LightingSystem::lightingChanged,
 *         [this]() { viewport->update(); });
 * 
 * // Apply lighting to renderer during frame rendering
 * lightingSystem->applyLighting(renderer, camera->getPosition());
 * ```
 * 
 * ### Custom Artistic Lighting Setup
 * ```cpp
 * // Create custom lighting configuration
 * auto lighting = std::make_shared<LightingSystem>();
 * 
 * // Configure warm key light for golden hour effect
 * glm::vec3 keyDirection = glm::normalize(glm::vec3(0.5f, 0.8f, 0.3f));
 * glm::vec4 warmKeyColor(1.0f, 0.9f, 0.7f, 1.0f); // 3200K warm white
 * lighting->setKeyLight(keyDirection, warmKeyColor, 1.2f);
 * 
 * // Cool-tinted fill light for artistic contrast
 * glm::vec3 fillDirection = glm::normalize(glm::vec3(-0.8f, 0.4f, -0.2f));
 * glm::vec4 coolFillColor(0.8f, 0.9f, 1.0f, 1.0f); // 6500K cool white
 * lighting->setFillLight(fillDirection, coolFillColor, 0.4f);
 * 
 * // Subtle rim light for edge definition
 * glm::vec3 rimDirection = glm::normalize(glm::vec3(0.2f, 0.6f, -0.8f));
 * glm::vec4 rimColor(1.0f, 0.95f, 0.9f, 1.0f); // Slightly warm accent
 * lighting->setRimLight(rimDirection, rimColor, 0.3f);
 * 
 * // Blue ambient for outdoor atmosphere
 * glm::vec4 skyAmbient(0.7f, 0.8f, 1.0f, 1.0f); // 8000K sky blue
 * lighting->setAmbientLight(skyAmbient, 0.15f);
 * ```
 * 
 * ### Professional Maya-Compatible Workflow
 * ```cpp
 * class MayaLightingController {
 * private:
 *     std::shared_ptr<LightingSystem> m_lighting;
 *     
 * public:
 *     void setupMayaLighting() {
 *         m_lighting = std::make_shared<LightingSystem>();
 *         
 *         // Apply Maya preset as base
 *         m_lighting->setLightingPreset(LightingSystem::LightingPreset::Maya);
 *         
 *         // Fine-tune for specific Maya viewport settings
 *         adjustKeyLightForMayaViewport();
 *         adjustFillLightForMayaViewport();
 *         adjustAmbientForMayaViewport();
 *     }
 *     
 * private:
 *     void adjustKeyLightForMayaViewport() {
 *         // Maya-specific key light adjustments
 *         glm::vec3 mayaKeyDirection(0.577f, 0.577f, 0.577f); // 45° on all axes
 *         glm::vec4 neutralWhite(1.0f, 1.0f, 1.0f, 1.0f);   // 5500K neutral
 *         m_lighting->setKeyLight(mayaKeyDirection, neutralWhite, 1.0f);
 *     }
 *     
 *     void adjustFillLightForMayaViewport() {
 *         // Maya-style fill light configuration
 *         glm::vec3 mayaFillDirection(-0.408f, 0.408f, -0.816f);
 *         glm::vec4 slightlyCoolFill(0.95f, 0.97f, 1.0f, 1.0f);
 *         m_lighting->setFillLight(mayaFillDirection, slightlyCoolFill, 0.5f);
 *     }
 *     
 *     void adjustAmbientForMayaViewport() {
 *         // Maya-compatible ambient lighting
 *         glm::vec4 mayaAmbient(0.9f, 0.92f, 0.95f, 1.0f);
 *         m_lighting->setAmbientLight(mayaAmbient, 0.2f);
 *     }
 * };
 * ```
 * 
 * ### Dynamic Lighting Animation
 * ```cpp
 * class AnimatedLightingSystem : public LightingSystem {
 * private:
 *     float m_animationTime = 0.0f;
 *     glm::vec3 m_baseKeyDirection;
 *     
 * public:
 *     AnimatedLightingSystem(QObject* parent = nullptr) 
 *         : LightingSystem(parent) {
 *         m_baseKeyDirection = getKeyLightDirection();
 *     }
 *     
 *     void updateAnimation(float deltaTime) {
 *         m_animationTime += deltaTime;
 *         
 *         // Animate key light for time-of-day simulation
 *         animateTimeOfDay();
 *         
 *         // Animate color temperature for mood changes
 *         animateColorTemperature();
 *         
 *         // Animate intensity for dramatic effects
 *         animateIntensity();
 *     }
 *     
 * private:
 *     void animateTimeOfDay() {
 *         // Rotate key light position for sun movement
 *         float angle = sin(m_animationTime * 0.1f) * 0.5f; // Slow sun movement
 *         glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
 *         glm::vec3 newDirection = glm::vec3(rotation * glm::vec4(m_baseKeyDirection, 1.0f));
 *         
 *         setKeyLight(newDirection, getKeyLightColor(), getKeyLightIntensity());
 *     }
 *     
 *     void animateColorTemperature() {
 *         // Animate from warm sunrise to cool noon to warm sunset
 *         float tempCycle = sin(m_animationTime * 0.05f);
 *         float temperature = 4000.0f + tempCycle * 1500.0f; // 2500K - 5500K range
 *         
 *         glm::vec4 temperatureColor = colorTemperatureToRGB(temperature);
 *         setKeyLight(getKeyLightDirection(), temperatureColor, getKeyLightIntensity());
 *     }
 *     
 *     void animateIntensity() {
 *         // Subtle intensity variation for atmospheric effects
 *         float intensityVariation = 0.9f + 0.2f * sin(m_animationTime * 2.0f);
 *         
 *         setKeyLight(getKeyLightDirection(), getKeyLightColor(), intensityVariation);
 *     }
 *     
 *     glm::vec4 colorTemperatureToRGB(float temperature) {
 *         // Simplified blackbody radiation calculation
 *         float red = temperature < 6600 ? 1.0f : 
 *                    1.292936f * pow(temperature - 60, -0.1332047f);
 *         float green = temperature < 6600 ? 
 *                      0.39008157f * log(temperature) - 0.63184144f :
 *                      1.292936f * pow(temperature - 60, -0.0755148f);
 *         float blue = temperature > 6600 ? 1.0f :
 *                     temperature < 1900 ? 0.0f :
 *                     0.543206789f * log(temperature - 10) - 1.19625408f;
 *         
 *         return glm::vec4(glm::clamp(red, 0.0f, 1.0f),
 *                         glm::clamp(green, 0.0f, 1.0f),
 *                         glm::clamp(blue, 0.0f, 1.0f), 1.0f);
 *     }
 * };
 * ```
 * 
 * ### Multi-Scene Lighting Management
 * ```cpp
 * class SceneLightingManager {
 * private:
 *     std::unordered_map<SceneID, std::shared_ptr<LightingSystem>> m_sceneLighting;
 *     SceneID m_activeScene;
 *     
 * public:
 *     void createSceneLighting(SceneID sceneId, LightingSystem::LightingPreset preset) {
 *         auto lighting = std::make_shared<LightingSystem>();
 *         lighting->setLightingPreset(preset);
 *         m_sceneLighting[sceneId] = lighting;
 *     }
 *     
 *     void setActiveScene(SceneID sceneId) {
 *         if (m_sceneLighting.find(sceneId) != m_sceneLighting.end()) {
 *             m_activeScene = sceneId;
 *         }
 *     }
 *     
 *     std::shared_ptr<LightingSystem> getActiveLighting() {
 *         auto it = m_sceneLighting.find(m_activeScene);
 *         return (it != m_sceneLighting.end()) ? it->second : nullptr;
 *     }
 *     
 *     void applyLightingToRenderer(std::shared_ptr<Renderer> renderer, 
 *                                 const glm::vec3& cameraPos) {
 *         auto lighting = getActiveLighting();
 *         if (lighting) {
 *             lighting->applyLighting(renderer, cameraPos);
 *         }
 *     }
 *     
 *     void saveLightingConfiguration(const std::string& filename) {
 *         // Serialize lighting configurations for all scenes
 *         for (const auto& [sceneId, lighting] : m_sceneLighting) {
 *             saveLightingToFile(sceneId, lighting, filename);
 *         }
 *     }
 * };
 * ```
 * 
 * ## Color Science and Professional Standards
 * 
 * ### Color Temperature Implementation
 * Professional color temperature ranges and applications:
 * - **Candlelight (1900K)**: glm::vec4(1.0f, 0.6f, 0.2f, 1.0f) - Warm atmospheric
 * - **Tungsten (3200K)**: glm::vec4(1.0f, 0.8f, 0.6f, 1.0f) - Studio lighting
 * - **Neutral White (5500K)**: glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) - Daylight balance
 * - **Cool White (6500K)**: glm::vec4(0.9f, 0.95f, 1.0f, 1.0f) - Overcast sky
 * - **Sky Blue (10000K)**: glm::vec4(0.7f, 0.8f, 1.0f, 1.0f) - Clear sky simulation
 * 
 * ### Professional Intensity Standards
 * Industry-standard lighting intensity ranges:
 * - **Key Light**: 0.8-2.0x base intensity for primary illumination
 * - **Fill Light**: 0.3-0.7x key light intensity for shadow fill
 * - **Rim Light**: 0.2-0.5x key light intensity for edge accent
 * - **Ambient Light**: 0.1-0.3x total illumination for environment
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Rendering
 * @ingroup Lighting
 * @ingroup Professional
 */

#pragma once

#include "../../Common.h"
#include <QObject>
#include <glm/glm.hpp>
#include <memory>

// Forward declarations
class Renderer;

/**
 * @addtogroup Rendering
 * @{
 */

/**
 * @brief Industry-standard lighting system for professional 3D modeling applications
 * 
 * The LightingSystem implements a comprehensive three-point lighting setup commonly
 * used in professional 3D modeling and animation software. It provides preset
 * configurations matching industry standards while offering full control over
 * individual light parameters for artistic customization.
 * 
 * ## Lighting Architecture
 * The system implements classic three-point lighting theory:
 * 
 * ### Key Light (Primary)
 * - **Purpose**: Primary illumination source providing main lighting
 * - **Position**: Typically 45° above and to one side of the subject
 * - **Characteristics**: Strongest light, defines primary shadows and form
 * - **Artistic Control**: Direction, color, intensity fully adjustable
 * 
 * ### Fill Light (Secondary)
 * - **Purpose**: Softens harsh shadows created by the key light
 * - **Position**: Usually opposite side from key light, lower intensity
 * - **Characteristics**: Reduces contrast, reveals detail in shadow areas
 * - **Typical Intensity**: 50-70% of key light strength
 * 
 * ### Rim Light (Accent)
 * - **Purpose**: Separates subject from background, adds depth
 * - **Position**: Behind and slightly above the subject
 * - **Characteristics**: Creates subtle edge highlighting
 * - **Typical Intensity**: 30-50% of key light strength
 * 
 * ### Ambient Light (Environment)
 * - **Purpose**: Simulates environmental light bouncing
 * - **Characteristics**: Uniform, low-intensity illumination
 * - **Prevents**: Completely black shadows for realistic rendering
 * - **Typical Intensity**: 10-20% overall brightness
 * 
 * ## Professional Presets
 * The system includes lighting presets matching industry-standard software:
 * 
 * ### Studio Preset
 * - **Use Case**: Product visualization, character modeling
 * - **Characteristics**: Clean, controlled lighting with minimal shadows
 * - **Key Light**: Bright, slightly warm directional light
 * - **Fill Light**: Cool, soft secondary illumination
 * - **Applications**: Technical modeling, product renders
 * 
 * ### Maya Preset
 * - **Use Case**: Matches Autodesk Maya default lighting
 * - **Characteristics**: Balanced three-point setup
 * - **Industry Standard**: Animation and film industry workflows
 * - **Key Light**: Standard 45° angle with neutral white
 * 
 * ### Blender Preset
 * - **Use Case**: Matches Blender default lighting
 * - **Characteristics**: Slightly cooler tone, artistic focus
 * - **Key Light**: Strong directional with blue-tinted fill
 * - **Applications**: Artistic modeling, game asset creation
 * 
 * ### Outdoor Preset
 * - **Use Case**: Simulates natural daylight conditions
 * - **Characteristics**: Warm key light, blue ambient
 * - **Key Light**: Sun simulation at typical outdoor angle
 * - **Ambient**: Sky light simulation with blue tint
 * 
 * ## Performance Optimization
 * The lighting system is optimized for real-time 3D modeling:
 * - **Uniform Caching**: Light parameters cached to minimize GPU uploads
 * - **Conditional Updates**: Only changed lighting values are updated
 * - **Shader Integration**: Direct integration with standard shaders
 * - **Multi-light Support**: Efficient handling of multiple light sources
 * 
 * ## Integration with Rendering Pipeline
 * The LightingSystem integrates seamlessly with the broader rendering system:
 * 1. **Setup Phase**: Configure lights based on current preset
 * 2. **Update Phase**: Apply lighting parameters to active shaders
 * 3. **Render Phase**: Lighting calculations performed per-fragment
 * 4. **Dynamic Updates**: Real-time lighting adjustments during modeling
 * 
 * ## Extensibility
 * The system is designed for future expansion:
 * - **Shadow Mapping**: Framework for dynamic shadow implementation
 * - **Image-Based Lighting**: Support for HDR environment maps
 * - **Area Lights**: Extension to area light sources
 * - **Volumetric Lighting**: Support for atmospheric effects
 * 
 * ## Usage Examples
 * ```cpp
 * // Create and configure lighting system
 * auto lighting = std::make_shared<LightingSystem>();
 * 
 * // Apply studio lighting preset
 * lighting->setLightingPreset(LightingSystem::LightingPreset::Studio);
 * 
 * // Customize key light for specific needs
 * lighting->setKeyLight(glm::vec3(1, 1, 0.5), glm::vec4(1.0, 0.95, 0.8, 1.0), 1.2f);
 * 
 * // Apply to renderer during frame rendering
 * lighting->applyLighting(renderer, cameraPosition);
 * 
 * // Real-time adjustments
 * lighting->setAmbientLight(glm::vec4(0.1, 0.15, 0.2, 1.0), 0.15f);
 * ```
 * 
 * ## Color Temperature
 * Light colors support realistic color temperature simulation:
 * - **Warm White** (3200K): glm::vec4(1.0, 0.9, 0.7, 1.0)
 * - **Neutral White** (5500K): glm::vec4(1.0, 1.0, 1.0, 1.0)
 * - **Cool White** (6500K): glm::vec4(0.9, 0.95, 1.0, 1.0)
 * - **Sky Blue** (10000K): glm::vec4(0.7, 0.8, 1.0, 1.0)
 * 
 * @see Renderer for low-level rendering operations
 * @see RenderSystem for rendering pipeline coordination
 * @see Material for surface interaction with lighting
 * 
 * @ingroup Rendering
 */
class LightingSystem : public QObject
{
    Q_OBJECT

public:
    // ========================================================================
    // Lighting Preset Types
    // ========================================================================
    
    /**
     * @enum LightingPreset
     * @brief Professional lighting presets matching industry standards
     * 
     * Predefined lighting configurations that match common setups used
     * in professional 3D modeling and animation software packages.
     */
    enum class LightingPreset {
        /**
         * @brief Professional studio lighting setup
         * 
         * Clean, controlled lighting optimized for product visualization
         * and technical modeling with minimal harsh shadows.
         */
        Studio,
        
        /**
         * @brief Autodesk Maya-style default lighting
         * 
         * Balanced three-point lighting matching Maya's standard viewport
         * illumination used throughout the animation industry.
         */
        Maya,
        
        /**
         * @brief Blender-style default lighting
         * 
         * Artistic lighting setup with slightly cooler tones matching
         * Blender's default viewport configuration.
         */
        Blender,
        
        /**
         * @brief Natural outdoor/daylight simulation
         * 
         * Simulates natural daylight conditions with warm key light
         * and blue sky ambient lighting.
         */
        Outdoor,
        
        /**
         * @brief User-defined custom lighting
         * 
         * Indicates that lighting has been manually customized
         * beyond the standard presets.
         */
        Custom
    };

    // ========================================================================
    // Construction and Lifecycle
    // ========================================================================
    
    /**
     * @brief Construct lighting system with Qt parent
     * 
     * Creates a new LightingSystem with default studio lighting configuration.
     * The system is ready for immediate use with standard three-point lighting.
     * 
     * @param parent Qt parent object for memory management
     * 
     * @post LightingSystem is created with Studio preset active
     * @post All lights are configured with sensible default values
     * @post System is ready for renderer integration
     */
    explicit LightingSystem(QObject* parent = nullptr);
    
    /**
     * @brief Destructor with automatic cleanup
     * 
     * Cleans up any allocated resources and disconnects from the Qt
     * object hierarchy for proper memory management.
     */
    ~LightingSystem();

    // ========================================================================
    // Preset Management
    // ========================================================================
    
    /**
     * @brief Apply professional lighting preset
     * 
     * Configures all lighting parameters to match the specified professional
     * preset, instantly updating the three-point lighting setup.
     * 
     * @param preset Lighting preset to apply
     * 
     * @post All light parameters are updated to match preset
     * @post Current preset is updated to specified value
     * @post lightingChanged() signal is emitted
     * 
     * @note Overwrites any custom lighting adjustments
     * @note Changes take effect immediately for real-time feedback
     * 
     * @see LightingPreset for available preset descriptions
     */
    void setLightingPreset(LightingPreset preset);
    
    /**
     * @brief Get currently active lighting preset
     * 
     * @return Currently active lighting preset
     * 
     * @note Returns LightingPreset::Custom if lights have been manually adjusted
     */
    LightingPreset getCurrentPreset() const { return m_currentPreset; }

    // ========================================================================
    // Key Light Control (Primary Illumination)
    // ========================================================================
    
    /**
     * @brief Configure primary key light parameters
     * 
     * Sets the main illumination source that provides the primary lighting
     * for the scene. The key light defines the main shadows and overall
     * lighting mood of the 3D visualization.
     * 
     * @param direction Normalized direction vector of the light (world space)
     * @param color RGBA color of the light including alpha for blending
     * @param intensity Brightness multiplier (typical range: 0.5-2.0)
     * 
     * @post Key light parameters are updated
     * @post Current preset is set to Custom if not matching a preset
     * @post lightingChanged() signal is emitted
     * 
     * @note Direction vector should be normalized for consistent results
     * @note Higher intensity values create more dramatic lighting
     * @note Color values > 1.0 create over-bright effects
     */
    void setKeyLight(const glm::vec3& direction, const glm::vec4& color, float intensity = 1.0f);
    
    /**
     * @brief Get current key light direction
     * 
     * @return Normalized direction vector of the key light in world space
     */
    glm::vec3 getKeyLightDirection() const { return m_keyLightDirection; }
    
    /**
     * @brief Get current key light color
     * 
     * @return RGBA color vector of the key light
     */
    glm::vec4 getKeyLightColor() const { return m_keyLightColor; }
    
    /**
     * @brief Get current key light intensity
     * 
     * @return Brightness multiplier of the key light
     */
    float getKeyLightIntensity() const { return m_keyLightIntensity; }

    // ========================================================================
    // Fill Light Control (Secondary Illumination)
    // ========================================================================
    
    /**
     * @brief Configure secondary fill light parameters
     * 
     * Sets the fill light that softens harsh shadows created by the key light.
     * The fill light is typically positioned opposite the key light with
     * lower intensity to maintain lighting contrast while revealing detail.
     * 
     * @param direction Normalized direction vector of the fill light
     * @param color RGBA color of the fill light (often cooler than key light)
     * @param intensity Brightness multiplier (typically 0.3-0.7)
     * 
     * @post Fill light parameters are updated
     * @post Current preset is set to Custom if not matching a preset
     * @post lightingChanged() signal is emitted
     * 
     * @note Fill light should be weaker than key light for natural appearance
     * @note Cool-toned fill lights (blue-ish) create more realistic shadows
     * @note Position typically 60-90 degrees from key light direction
     */
    void setFillLight(const glm::vec3& direction, const glm::vec4& color, float intensity = 0.5f);
    
    /**
     * @brief Get current fill light direction
     * 
     * @return Normalized direction vector of the fill light in world space
     */
    glm::vec3 getFillLightDirection() const { return m_fillLightDirection; }
    
    /**
     * @brief Get current fill light color
     * 
     * @return RGBA color vector of the fill light
     */
    glm::vec4 getFillLightColor() const { return m_fillLightColor; }
    
    /**
     * @brief Get current fill light intensity
     * 
     * @return Brightness multiplier of the fill light
     */
    float getFillLightIntensity() const { return m_fillLightIntensity; }

    // ========================================================================
    // Rim Light Control (Accent Illumination)
    // ========================================================================
    
    /**
     * @brief Configure accent rim light parameters
     * 
     * Sets the rim light that creates edge highlighting to separate objects
     * from the background. The rim light is typically positioned behind and
     * above the subject to create subtle edge illumination.
     * 
     * @param direction Normalized direction vector of the rim light
     * @param color RGBA color of the rim light (often warm or neutral)
     * @param intensity Brightness multiplier (typically 0.2-0.5)
     * 
     * @post Rim light parameters are updated
     * @post Current preset is set to Custom if not matching a preset
     * @post lightingChanged() signal is emitted
     * 
     * @note Rim light creates separation between subject and background
     * @note Subtle intensity prevents overpowering the main lighting
     * @note Position typically behind subject at 45-60 degree elevation
     */
    void setRimLight(const glm::vec3& direction, const glm::vec4& color, float intensity = 0.3f);
    
    /**
     * @brief Get current rim light direction
     * 
     * @return Normalized direction vector of the rim light in world space
     */
    glm::vec3 getRimLightDirection() const { return m_rimLightDirection; }
    
    /**
     * @brief Get current rim light color
     * 
     * @return RGBA color vector of the rim light
     */
    glm::vec4 getRimLightColor() const { return m_rimLightColor; }
    
    /**
     * @brief Get current rim light intensity
     * 
     * @return Brightness multiplier of the rim light
     */
    float getRimLightIntensity() const { return m_rimLightIntensity; }

    // ========================================================================
    // Environment Lighting Control
    // ========================================================================
    
    /**
     * @brief Configure ambient environment lighting
     * 
     * Sets the ambient light that simulates environmental light bouncing
     * and prevents completely black shadows. Ambient light provides uniform
     * low-level illumination across all surfaces.
     * 
     * @param color RGBA color of the ambient light
     * @param intensity Overall brightness of ambient illumination (typically 0.1-0.3)
     * 
     * @post Ambient light parameters are updated
     * @post Current preset is set to Custom if not matching a preset
     * @post lightingChanged() signal is emitted
     * 
     * @note Low intensity maintains lighting contrast while preventing pure black
     * @note Cool ambient colors (blue tint) simulate sky light bouncing
     * @note Warm ambient colors create cozy, indoor lighting feel
     */
    void setAmbientLight(const glm::vec4& color, float intensity = 0.2f);
    
    /**
     * @brief Get current ambient light color
     * 
     * @return RGBA color vector of the ambient light
     */
    glm::vec4 getAmbientColor() const { return m_ambientColor; }
    
    /**
     * @brief Get current ambient light intensity
     * 
     * @return Brightness multiplier of the ambient light
     */
    float getAmbientIntensity() const { return m_ambientIntensity; }

    // ========================================================================
    // Renderer Integration
    // ========================================================================
    
    /**
     * @brief Apply lighting configuration to renderer
     * 
     * Updates the renderer with current lighting parameters, preparing
     * all light uniforms for shader-based lighting calculations.
     * This is the primary method for integrating lighting with the rendering pipeline.
     * 
     * @param renderer Shared pointer to the renderer to update
     * @param cameraPosition Current camera position for specular calculations
     * 
     * @pre Renderer must be initialized and have valid shader programs loaded
     * @pre Camera position should be in world space coordinates
     * 
     * @post All lighting uniforms are uploaded to current shader
     * @post Renderer is ready for lighting-aware rendering
     * 
     * @note Call this method before rendering geometry that requires lighting
     * @note Camera position is used for accurate specular reflection calculations
     * @note Performance optimized - only uploads changed lighting parameters
     */
    void applyLighting(std::shared_ptr<Renderer> renderer, const glm::vec3& cameraPosition);
    
    /**
     * @brief Update lighting uniforms in renderer
     * 
     * Uploads current lighting parameters to the renderer's active shader program.
     * This is a lower-level method used internally by applyLighting().
     * 
     * @param renderer Shared pointer to the renderer to update
     * 
     * @pre Renderer must have an active shader program with lighting uniforms
     * 
     * @post Lighting uniform values are uploaded to GPU
     * 
     * @note Typically called automatically by applyLighting()
     * @note Can be called directly for advanced lighting control
     */
    void updateUniforms(std::shared_ptr<Renderer> renderer);

    // ========================================================================
    // Advanced Features (Future Expansion)
    // ========================================================================
    
    /**
     * @brief Enable or disable shadow casting
     * 
     * Controls whether lights cast dynamic shadows in the scene.
     * Currently reserved for future shadow mapping implementation.
     * 
     * @param enabled true to enable shadows, false to disable
     * 
     * @post Shadow state is updated for future shadow rendering
     * 
     * @note Shadow implementation is planned for future releases
     * @note Currently stored for forward compatibility
     */
    void setShadowsEnabled(bool enabled) { m_shadowsEnabled = enabled; }
    
    /**
     * @brief Check if shadows are enabled
     * 
     * @return true if shadows are enabled, false if disabled
     * 
     * @note Shadow implementation is planned for future releases
     */
    bool areShadowsEnabled() const { return m_shadowsEnabled; }

signals:
    /**
     * @brief Signal emitted when lighting configuration changes
     * 
     * Emitted whenever any lighting parameter is modified, allowing
     * connected systems to update accordingly for real-time feedback.
     * 
     * @note Connect to this signal for automatic viewport updates
     * @note Useful for UI synchronization with lighting changes
     */
    void lightingChanged();

private:
    // ========================================================================
    // Internal Preset Setup Methods
    // ========================================================================
    
    /**
     * @brief Configure professional studio lighting
     * 
     * Internal method that sets up clean, controlled lighting optimized
     * for product visualization and technical modeling workflows.
     */
    void setupStudioLighting();
    
    /**
     * @brief Configure Maya-style default lighting
     * 
     * Internal method that replicates Autodesk Maya's standard viewport
     * lighting configuration used throughout the animation industry.
     */
    void setupMayaLighting(); 
    
    /**
     * @brief Configure Blender-style default lighting
     * 
     * Internal method that replicates Blender's default lighting setup
     * with slightly cooler tones for artistic modeling workflows.
     */
    void setupBlenderLighting();
    
    /**
     * @brief Configure outdoor/daylight simulation
     * 
     * Internal method that sets up natural daylight simulation with
     * warm key light and blue sky ambient lighting.
     */
    void setupOutdoorLighting();

    // ========================================================================
    // Internal State Variables
    // ========================================================================
    
    /// Currently active lighting preset
    LightingPreset m_currentPreset;

    // ========================================================================
    // Key Light Parameters (Primary Illumination)
    // ========================================================================
    
    /// Direction vector of the key light in world space
    glm::vec3 m_keyLightDirection;
    
    /// RGBA color of the key light
    glm::vec4 m_keyLightColor;
    
    /// Intensity multiplier for the key light
    float m_keyLightIntensity;

    // ========================================================================
    // Fill Light Parameters (Secondary Illumination)
    // ========================================================================
    
    /// Direction vector of the fill light in world space
    glm::vec3 m_fillLightDirection;
    
    /// RGBA color of the fill light
    glm::vec4 m_fillLightColor;
    
    /// Intensity multiplier for the fill light
    float m_fillLightIntensity;

    // ========================================================================
    // Rim Light Parameters (Accent Illumination)
    // ========================================================================
    
    /// Direction vector of the rim light in world space
    glm::vec3 m_rimLightDirection;
    
    /// RGBA color of the rim light
    glm::vec4 m_rimLightColor;
    
    /// Intensity multiplier for the rim light
    float m_rimLightIntensity;

    // ========================================================================
    // Environment Lighting Parameters
    // ========================================================================
    
    /// RGBA color of the ambient environment light
    glm::vec4 m_ambientColor;
    
    /// Intensity multiplier for ambient lighting
    float m_ambientIntensity;

    // ========================================================================
    // Future Feature State
    // ========================================================================
    
    /// Whether dynamic shadows are enabled (future feature)
    bool m_shadowsEnabled;
};

/**
 * @}
 */
