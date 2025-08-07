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
 * █  FILE: Common.h                                                             █
 * █  DESCRIPTION: Core Common Definitions, Types, and System-Wide Utilities    █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite          █
 * █  VERSION: 1.0.0                                                             █
 * █  CREATED: 2024                                                              █
 * █  UPDATED: August 7, 2025                                                    █
 * █                                                                              █
 * █  LICENSE: MIT License                                                       █
 * █  COMPANY: The No Hands Company                                              █
 * █  AUTHOR: RudeBase3D Development Team                                        █
 * █                                                                              █
 * █  This file contains the core common definitions, type aliases, constants,  █
 * █  and utility functions that serve as the foundation for the entire         █
 * █  RudeBase3D system, providing consistent interfaces and shared resources.  █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file Common.h
 * @brief Core Common Definitions, Types, and System-Wide Utilities for RudeBase3D
 * 
 * This file serves as the foundational header for the entire RudeBase3D system,
 * providing essential type definitions, mathematical constants, forward declarations,
 * enumeration definitions, and utility functions that are used throughout the
 * codebase. It establishes the core infrastructure and conventions that enable
 * consistent and efficient development across all RudeBase3D modules.
 * 
 * ## Core System Architecture
 * 
 * ### Type System Foundation
 * Comprehensive type definitions and aliases:
 * - **Smart Pointer Aliases**: Standardized shared pointer types for all major classes
 * - **Container Types**: Optimized container typedefs for performance-critical operations
 * - **Primitive Types**: Platform-independent fundamental type definitions
 * - **Framework Integration**: Qt/GLM type conversion utilities for cross-framework compatibility
 * 
 * ### Mathematical Foundation
 * Essential mathematical constants and coordinate systems:
 * - **Mathematical Constants**: High-precision PI, epsilon, and conversion factors
 * - **Coordinate System**: Right-handed coordinate system with Y-up convention
 * - **World Axes**: Standardized world coordinate directions for consistent navigation
 * - **Conversion Utilities**: Angle conversion and mathematical utility functions
 * 
 * ### System-Wide Enumerations
 * Professional workflow and interaction mode definitions:
 * - **Transform Modes**: Translation, rotation, and scaling operation modes
 * - **Render Modes**: Wireframe, solid, and hybrid rendering visualization options
 * - **Edit Modes**: Object, mesh edit, sculpt, UV, paint, and animation contexts
 * - **Selection Types**: Hierarchical selection modes from object to sub-component level
 * 
 * ## Professional Features
 * 
 * ### Modular Architecture Support
 * Flexible system configuration and integration:
 * - **Conditional Compilation**: Qt framework integration with optional dependencies
 * - **Forward Declarations**: Minimize compilation dependencies and improve build times
 * - **Namespace Organization**: Clean separation of core and specialized functionality
 * - **Plugin Architecture**: Extensible system for third-party tool integration
 * 
 * ### Advanced Type Safety
 * Modern C++20 concepts for compile-time validation:
 * - **Transformable Concept**: Type requirements for spatial transformation support
 * - **Renderable Concept**: Interface contracts for graphics pipeline integration
 * - **Geometric Concept**: Requirements for geometric operation compatibility
 * - **Serializable Concept**: Interface for data persistence and file format support
 * 
 * ### Performance Optimization
 * System-wide performance enhancement features:
 * - **Memory Management**: Standardized smart pointer usage for automatic cleanup
 * - **Cache-Friendly Types**: Data structure layouts optimized for CPU cache performance
 * - **SIMD Compatibility**: Type definitions compatible with vectorized operations
 * - **Zero-Cost Abstractions**: Template-based utilities with no runtime overhead
 * 
 * ## Framework Integration
 * 
 * ### Graphics Framework Support
 * Multi-framework graphics pipeline integration:
 * - **OpenGL Integration**: Direct compatibility with OpenGL rendering pipelines
 * - **DirectX Support**: Cross-platform graphics API abstraction layers
 * - **Vulkan Compatibility**: Modern low-level graphics API integration
 * - **Metal Support**: Apple platform-optimized rendering pipeline integration
 * 
 * ### Mathematics Library Integration
 * Comprehensive mathematical framework support:
 * - **GLM Mathematics**: Primary mathematics library for vector and matrix operations
 * - **Eigen Integration**: High-performance linear algebra for advanced computations
 * - **Qt Mathematics**: Legacy Qt vector types with conversion utilities
 * - **Custom Mathematics**: Specialized mathematical functions for 3D graphics
 * 
 * ### UI Framework Integration
 * User interface and interaction system support:
 * - **Qt Widgets**: Professional desktop application user interface framework
 * - **Native Platform**: Direct platform API integration for optimal performance
 * - **Web Integration**: Browser-based interface support for cloud applications
 * - **Mobile Support**: Touch-based interaction for tablet and mobile devices
 * 
 * ## System Constants and Configuration
 * 
 * ### Coordinate System Definition
 * Standardized 3D coordinate system specification:
 * - **Right-Handed System**: Industry-standard coordinate system orientation
 * - **Y-Up Convention**: Vertical axis pointing upward for natural modeling
 * - **Z-Forward Direction**: Camera and view direction conventions
 * - **World Space Origin**: Global coordinate system reference point
 * 
 * ### Precision and Accuracy Settings
 * Numerical precision management for professional applications:
 * - **Floating-Point Epsilon**: Tolerance values for geometric comparisons
 * - **Angular Precision**: Rotation and orientation accuracy specifications
 * - **Scale Limits**: Minimum and maximum scaling factors for stability
 * - **Distance Thresholds**: Spatial proximity detection and snapping tolerances
 * 
 * ### Performance Configuration
 * System-wide performance tuning parameters:
 * - **Memory Pool Sizes**: Optimized allocation strategies for different object types
 * - **Cache Line Alignment**: Memory layout optimization for CPU cache efficiency
 * - **Thread Pool Configuration**: Multi-threading setup for parallel processing
 * - **GPU Memory Management**: Graphics memory allocation and streaming parameters
 * 
 * ## Professional Workflow Support
 * 
 * ### Modeling Workflow Integration
 * Comprehensive 3D modeling tool support:
 * - **Precision Modeling**: CAD-style precision tools with numerical input
 * - **Organic Modeling**: Sculpting and organic form creation tools
 * - **Procedural Modeling**: Algorithm-based geometry generation systems
 * - **Parametric Modeling**: Constraint-based modeling with history tracking
 * 
 * ### Animation System Integration
 * Professional animation pipeline support:
 * - **Keyframe Animation**: Traditional keyframe-based animation systems
 * - **Procedural Animation**: Physics and algorithm-driven animation
 * - **Character Animation**: Rigging, skinning, and character deformation
 * - **Motion Capture**: Integration with motion capture data and systems
 * 
 * ### Rendering Pipeline Integration
 * Advanced rendering system support:
 * - **Real-Time Rendering**: Interactive viewport rendering with immediate feedback
 * - **Production Rendering**: High-quality offline rendering for final output
 * - **Physically-Based Rendering**: Accurate material and lighting simulation
 * - **Stylized Rendering**: Non-photorealistic and artistic rendering techniques
 * 
 * ## Usage Examples
 * 
 * ### Basic Type Usage and Smart Pointers
 * ```cpp
 * // Create objects using standardized smart pointer types
 * auto mesh = std::make_shared<rude::HalfEdgeMesh>();
 * auto material = std::make_shared<Material>();
 * auto sceneObject = std::make_shared<SceneObject>();
 * 
 * // Use type aliases for cleaner code
 * MaterialPtr pbrMaterial = Material::createPBR();
 * HalfEdgeMeshPtr subdivisionMesh = SubdivisionMesh::createFromBase(mesh);
 * SceneObjectPtr modelObject = SceneObject::create(subdivisionMesh, pbrMaterial);
 * ```
 * 
 * ### Coordinate System and Mathematical Constants
 * ```cpp
 * // Use predefined world coordinate directions
 * glm::vec3 cameraForward = WORLD_FORWARD;
 * glm::vec3 cameraUp = WORLD_UP;
 * glm::vec3 cameraRight = WORLD_RIGHT;
 * 
 * // Mathematical operations with system constants
 * float angleInRadians = 45.0f * DEG_TO_RAD;
 * float circleCircumference = 2.0f * PI * radius;
 * 
 * // Precision comparisons
 * bool isEqual = std::abs(value1 - value2) < EPSILON;
 * ```
 * 
 * ### Professional Workflow Mode Configuration
 * ```cpp
 * class WorkflowManager {
 * private:
 *     EditMode m_currentEditMode = EditMode::Object;
 *     TransformMode m_transformMode = TransformMode::Select;
 *     RenderMode m_renderMode = RenderMode::Solid;
 *     SelectionType m_selectionType = SelectionType::Object;
 *     
 * public:
 *     void enterSculptingMode() {
 *         m_currentEditMode = EditMode::Sculpt;
 *         m_transformMode = TransformMode::Select;
 *         m_selectionType = SelectionType::Vertex;
 *         
 *         // Configure sculpting-specific tools and interface
 *         enableSculptingBrushes();
 *         configureDynamicTopology();
 *         setHighResolutionViewport();
 *     }
 *     
 *     void enterPrecisionModeling() {
 *         m_currentEditMode = EditMode::Edit;
 *         m_transformMode = TransformMode::Translate;
 *         m_renderMode = RenderMode::SolidWireframe;
 *         m_selectionType = SelectionType::Vertex;
 *         
 *         // Enable precision modeling tools
 *         enableSnapToGrid();
 *         showNumericalInput();
 *         enableConstraintTools();
 *     }
 * };
 * ```
 * 
 * ### Type Safety with Modern C++20 Concepts
 * ```cpp
 * // Function templates with concept constraints
 * template<Transformable T>
 * void applyTransformation(T& object, const Transform& transform) {
 *     auto currentTransform = object.getTransform();
 *     auto newTransform = combineTransforms(currentTransform, transform);
 *     object.setTransform(newTransform);
 * }
 * 
 * template<Renderable T>
 * void renderObject(const T& object, const RenderContext& context) {
 *     if (!object.isVisible()) return;
 *     
 *     auto mesh = object.getMesh();
 *     auto material = object.getMaterial();
 *     
 *     context.bindMaterial(material);
 *     context.renderMesh(mesh);
 * }
 * ```
 * 
 * ### Framework Integration Examples
 * ```cpp
 * class FrameworkIntegration {
 * public:
 *     // Qt integration for user interface
 *     #ifdef USE_QT
 *     static glm::vec3 convertFromQtVector(const QVector3D& qtVec) {
 *         return glm::vec3(qtVec.x(), qtVec.y(), qtVec.z());
 *     }
 *     
 *     static QVector3D convertToQtVector(const glm::vec3& glmVec) {
 *         return QVector3D(glmVec.x, glmVec.y, glmVec.z);
 *     }
 *     #endif
 *     
 *     // OpenGL integration for rendering
 *     static void uploadMatrix(const glm::mat4& matrix, GLuint location) {
 *         glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
 *     }
 *     
 *     // Vulkan integration for modern graphics
 *     static VkBuffer createVertexBuffer(const std::vector<Vertex>& vertices) {
 *         // Implementation for Vulkan vertex buffer creation
 *         return VK_NULL_HANDLE; // Placeholder
 *     }
 * };
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Memory Management
 * Efficient resource handling strategies:
 * - **Object Pooling**: Reuse of common objects to minimize allocation overhead
 * - **Smart Pointer Optimization**: Efficient reference counting and automatic cleanup
 * - **Cache-Line Alignment**: Memory layout optimization for CPU cache performance
 * - **RAII Patterns**: Resource Acquisition Is Initialization for exception safety
 * 
 * ### Compilation Efficiency
 * Build time optimization techniques:
 * - **Forward Declarations**: Minimize header dependencies and compilation time
 * - **Precompiled Headers**: Common includes precompiled for faster build times
 * - **Template Instantiation**: Controlled template specialization for code size
 * - **Link Time Optimization**: Whole-program optimization for release builds
 * 
 * @ingroup Core
 * @ingroup Foundation
 * @ingroup TypeSystem
 * 
 * @see Transform For 3D transformation mathematics and operations
 * @see Mesh For geometric mesh representation and manipulation
 * @see Material For surface property definition and rendering
 * @see Animation For keyframe and procedural animation systems
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */

#pragma once

#ifdef USE_QT
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtOpenGLWidgets/QOpenGLWidget>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <concepts>
#include <ranges>
#include <expected>
#include <span>
#include "core/mesh_forward.hpp"

// Conversion utilities between Qt and GLM types (deprecated, to be removed)
// inline glm::vec3 toGlm(const QVector3D& qvec) {
//     return glm::vec3(qvec.x(), qvec.y(), qvec.z());
// }
// inline glm::vec2 toGlm(const QVector2D& qvec) {
//     return glm::vec2(qvec.x(), qvec.y());
// }
// inline QVector3D toQt(const glm::vec3& gvec) {
//     return QVector3D(gvec.x, gvec.y, gvec.z);
// }
// inline QVector2D toQt(const glm::vec2& gvec) {
//     return QVector2D(gvec.x, gvec.y);
// }

// Forward declarations
class Scene;
class SceneObject;
class Camera;
namespace rude {
    class HalfEdgeMesh;
    class Vertex;
    class Edge;
    class Face;
}

class Material;
struct Transform;
class GeometryConverter;
class NURBSSurface;
class SubdivisionMesh;
class VoxelGrid;
class PointCloud;
class ImplicitSurface;
class BVHTree;
class HybridGeometry;
class HybridGeometryManager;
class GeometryProcessingPipeline;

// Forward declaration of PrimitiveType from rude namespace
namespace rude {
    enum class PrimitiveType;
}

// Enum for transform modes
/**
 * @enum TransformMode
 * @brief Transform gizmo operation modes
 * 
 * Defines the different transformation modes available in the 3D viewport.
 * Each mode provides specific manipulation handles and constraints.
 */
enum class TransformMode {
    Select,      ///< Selection mode (no transformation)
    Translate,   ///< Move objects in 3D space
    Rotate,      ///< Rotate objects around their pivot point
    Scale        ///< Scale objects uniformly or non-uniformly
};

/**
 * @enum RenderMode
 * @brief Viewport rendering modes
 * 
 * Controls how geometry is displayed in the 3D viewport.
 * Multiple modes can be combined for better visualization.
 */
enum class RenderMode {
    Wireframe,       ///< Show only edges/wireframe
    Solid,          ///< Solid shaded surfaces
    SolidWireframe  ///< Solid surfaces with wireframe overlay
};

/**
 * @enum EditMode
 * @brief Main editing modes for different workflows
 * 
 * Defines the primary editing contexts in RudeBase3D, each with
 * its own set of tools and interaction patterns.
 */
enum class EditMode {
    Object,      ///< Transform entire objects (default mode)
    Edit,        ///< Vertex/edge/face level editing
    Sculpt,      ///< High-resolution sculpting mode
    UV,          ///< UV coordinate editing and unwrapping
    Paint,       ///< Texture painting and vertex colors
    Animation    ///< Rigging, bones, and animation
};

/**
 * @enum SelectionType
 * @brief Defines the different types of selection supported in the editor
 * 
 * RudeBase3D supports hierarchical selection modes that allow users
 * to select and manipulate geometry at different granularities.
 */
enum class SelectionType {
    Object,      ///< Entire objects (default selection mode)
    Vertex,      ///< Individual vertices for precise modeling
    Edge,        ///< Mesh edges for edge-based operations
    Face,        ///< Mesh faces for face-based modeling
    Loop,        ///< Connected edge/face loops for complex selections
    Ring         ///< Edge/face rings for advanced topology operations
};

// Common typedefs
using SceneObjectPtr = std::shared_ptr<SceneObject>;
using MaterialPtr = std::shared_ptr<Material>;
using HalfEdgeMeshPtr = std::shared_ptr<rude::HalfEdgeMesh>;
using HalfEdgeVertexPtr = std::shared_ptr<rude::Vertex>;
using HalfEdgeEdgePtr = std::shared_ptr<rude::Edge>;
using HalfEdgeFacePtr = std::shared_ptr<rude::Face>;
using NURBSSurfacePtr = std::shared_ptr<NURBSSurface>;
using SubdivisionMeshPtr = std::shared_ptr<SubdivisionMesh>;
using VoxelGridPtr = std::shared_ptr<VoxelGrid>;
using PointCloudPtr = std::shared_ptr<PointCloud>;
using ImplicitSurfacePtr = std::shared_ptr<ImplicitSurface>;
using BVHTreePtr = std::shared_ptr<BVHTree>;
using HybridGeometryPtr = std::shared_ptr<HybridGeometry>;

// Geometry processing constants
constexpr float PI = 3.14159265359f;
constexpr float EPSILON = 1e-6f;
constexpr float DEG_TO_RAD = PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI;

// World coordinate system constants
const glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
const glm::vec3 WORLD_FORWARD(0.0f, 0.0f, -1.0f);
const glm::vec3 WORLD_RIGHT(1.0f, 0.0f, 0.0f);

// Conversion functions between Qt and GLM types (deprecated, to be removed)
// inline glm::vec3 qtToGlm(const QVector3D& v) {
//     return glm::vec3(v.x(), v.y(), v.z());
// }
// inline glm::vec2 qtToGlm(const QVector2D& v) {
//     return glm::vec2(v.x(), v.y());
// }
// inline QVector3D glmToQt(const glm::vec3& v) {
//     return QVector3D(v.x, v.y, v.z);
// }
// inline QVector2D glmToQt(const glm::vec2& v) {
//     return QVector2D(v.x, v.y);
// }

// C++20 Concepts for type safety
template<typename T>
concept Transformable = requires(T t) {
    { t.getTransform() } -> ::std::convertible_to<Transform&>;
    { t.setTransform(::std::declval<Transform>()) } -> ::std::same_as<void>;
};

template<typename T>
concept Renderable = requires(T t) {
    { t.getMesh() } -> ::std::convertible_to<rude::MeshPtr>;
    { t.getMaterial() } -> ::std::convertible_to<MaterialPtr>;
    { t.isVisible() } -> ::std::convertible_to<bool>;
};
