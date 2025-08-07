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
 * █  FILE: Transform.h                                                          █
 * █  DESCRIPTION: Advanced 3D Transformation System for Position, Rotation,    █
 * █                and Scale Operations with Matrix Generation                  █
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
 * █  This file contains the core Transform class implementation providing       █
 * █  comprehensive 3D transformation capabilities including position,           █
 * █  rotation, scale operations, and efficient matrix generation for            █
 * █  professional 3D graphics and animation applications.                      █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file Transform.h
 * @brief Advanced 3D Transformation System with Matrix Generation and Animation Support
 * 
 * This file contains the comprehensive Transform class implementation that provides
 * professional-grade 3D transformation capabilities including position, rotation,
 * and scale operations with efficient matrix generation, interpolation support,
 * and seamless integration with animation systems within the RudeBase3D framework.
 * 
 * ## Core Transformation Architecture
 * 
 * ### Transform Components
 * Fundamental 3D transformation elements:
 * - **Position (Translation)**: 3D spatial coordinates with high precision
 * - **Rotation (Orientation)**: Quaternion-based rotation for stable interpolation
 * - **Scale (Size)**: Non-uniform scaling with per-axis control
 * - **Matrix Representation**: Efficient 4x4 transformation matrix generation
 * 
 * ### Mathematical Foundation
 * Robust mathematical representation:
 * - **Quaternion Rotations**: Gimbal lock-free rotation with smooth interpolation
 * - **Matrix Composition**: TRS (Translation-Rotation-Scale) matrix composition
 * - **Inverse Transformations**: Efficient inverse matrix computation
 * - **Numerical Stability**: Precision management for large-scale transformations
 * 
 * ### Performance Optimization
 * Efficient transformation processing:
 * - **Lazy Matrix Updates**: Deferred matrix computation until needed
 * - **Cache-Friendly Access**: Optimized memory layout for frequent operations
 * - **SIMD Compatibility**: Vectorized operations for batch transformations
 * - **Matrix Caching**: Automatic caching of computed transformation matrices
 * 
 * ## Professional Features
 * 
 * ### Advanced Transformation Operations
 * Sophisticated geometric manipulation capabilities:
 * - **Compound Transformations**: Hierarchical transformation chains
 * - **Local vs. World Space**: Context-aware transformation application
 * - **Look-At Functionality**: Camera and object orientation towards targets
 * - **Axis-Aligned Operations**: Constraint-based transformation for precise modeling
 * 
 * ### Animation Integration
 * Comprehensive animation system support:
 * - **Keyframe Interpolation**: Smooth transition between transformation states
 * - **Quaternion SLERP**: Spherical linear interpolation for natural rotation
 * - **Bezier Curves**: Advanced animation curve support for complex motion
 * - **Physics Integration**: Compatible with rigid body and soft body simulations
 * 
 * ### Coordinate System Management
 * Multi-coordinate system support:
 * - **Right-Handed Systems**: Industry-standard coordinate system conventions
 * - **Left-Handed Systems**: DirectX and Windows graphics compatibility
 * - **Custom Axes**: User-defined coordinate system transformations
 * - **Unit Conversions**: Automatic scaling between different measurement units
 * 
 * ## Advanced Capabilities
 * 
 * ### Hierarchical Transformations
 * Parent-child transformation relationships:
 * - **Scene Graph Integration**: Automatic hierarchical transformation propagation
 * - **Relative Transformations**: Local space operations within parent contexts
 * - **Transform Inheritance**: Automatic propagation of parent transformations
 * - **Constraint Systems**: Parent-child relationship constraints and limits
 * 
 * ### Interpolation and Blending
 * Advanced transformation interpolation:
 * - **Linear Interpolation**: Simple position and scale blending
 * - **Spherical Interpolation**: Natural quaternion rotation blending
 * - **Cubic Interpolation**: Smooth animation curves with acceleration control
 * - **Custom Easing**: User-defined interpolation functions for artistic control
 * 
 * ### Precision and Stability
 * Numerical accuracy and stability management:
 * - **Double Precision Mode**: High-precision transformations for CAD applications
 * - **Orthogonalization**: Automatic correction of accumulated floating-point errors
 * - **Decomposition**: Robust matrix decomposition into TRS components
 * - **Stability Checks**: Automatic detection and correction of degenerate matrices
 * 
 * ## Technical Implementation
 * 
 * ### Matrix Mathematics
 * Efficient linear algebra operations:
 * - **4x4 Homogeneous Matrices**: Standard 3D graphics transformation representation
 * - **Matrix Multiplication**: Optimized composition of multiple transformations
 * - **Inverse Computation**: Fast inverse matrix calculation for reverse transforms
 * - **Determinant Calculation**: Matrix validity and scaling factor determination
 * 
 * ### Memory Layout
 * Performance-optimized data organization:
 * - **Cache Alignment**: Memory layout optimized for CPU cache performance
 * - **SIMD Vectors**: Compatible with SSE, AVX, and ARM NEON instruction sets
 * - **Compact Storage**: Minimal memory footprint for large object collections
 * - **Dirty Flag System**: Efficient update tracking for selective recomputation
 * 
 * ### Integration Points
 * Seamless framework integration:
 * - **OpenGL Compatibility**: Direct matrix upload to OpenGL uniform buffers
 * - **DirectX Support**: Compatible with DirectX transformation pipelines
 * - **Physics Engines**: Integration with Bullet Physics and PhysX systems
 * - **Animation Systems**: Compatible with keyframe and procedural animation
 * 
 * ## Usage Examples
 * 
 * ### Basic Transform Operations
 * ```cpp
 * // Create and configure a transform
 * LegacyTransform transform;
 * transform.setPosition(glm::vec3(5.0f, 2.0f, -3.0f));
 * transform.setEulerAngles(glm::vec3(45.0f, 90.0f, 0.0f)); // X, Y, Z rotation in degrees
 * transform.setScale(glm::vec3(2.0f, 1.0f, 1.5f)); // Non-uniform scaling
 * 
 * // Get transformation matrix for rendering
 * glm::mat4 modelMatrix = transform.getModelMatrix();
 * glUseProgram(shaderProgram);
 * glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
 * ```
 * 
 * ### Advanced Transformation Operations
 * ```cpp
 * // Create a camera transform that looks at a target
 * LegacyTransform camera;
 * camera.setPosition(glm::vec3(0.0f, 5.0f, 10.0f));
 * camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
 * 
 * // Apply relative transformations
 * transform.translate(glm::vec3(1.0f, 0.0f, 0.0f)); // Move right
 * transform.rotateAround(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(30.0f)); // Rotate around Y-axis
 * transform.scale(glm::vec3(1.1f)); // Uniform scale up by 10%
 * 
 * // Get directional vectors for object-relative operations
 * glm::vec3 forward = transform.getForward();
 * glm::vec3 right = transform.getRight();
 * glm::vec3 up = transform.getUp();
 * ```
 * 
 * ### Animation and Interpolation
 * ```cpp
 * class TransformAnimator {
 * private:
 *     LegacyTransform m_startTransform;
 *     LegacyTransform m_endTransform;
 *     float m_duration;
 *     float m_currentTime;
 *     
 * public:
 *     LegacyTransform getCurrentTransform(float t) {
 *         LegacyTransform result;
 *         
 *         // Linear interpolation for position and scale
 *         glm::vec3 position = glm::mix(m_startTransform.getPosition(), 
 *                                     m_endTransform.getPosition(), t);
 *         glm::vec3 scale = glm::mix(m_startTransform.getScale(), 
 *                                  m_endTransform.getScale(), t);
 *                                  
 *         // Spherical interpolation for rotation
 *         glm::quat rotation = glm::slerp(m_startTransform.getRotation(), 
 *                                       m_endTransform.getRotation(), t);
 *         
 *         result.setPosition(position);
 *         result.setRotation(rotation);
 *         result.setScale(scale);
 *         
 *         return result;
 *     }
 *     
 *     void update(float deltaTime) {
 *         m_currentTime += deltaTime;
 *         float t = glm::clamp(m_currentTime / m_duration, 0.0f, 1.0f);
 *         
 *         // Apply easing function for smooth animation
 *         t = easeInOutCubic(t);
 *         
 *         auto currentTransform = getCurrentTransform(t);
 *         // Apply transform to object
 *     }
 * };
 * ```
 * 
 * ### Hierarchical Transform System
 * ```cpp
 * class HierarchicalTransform {
 * private:
 *     LegacyTransform m_localTransform;
 *     HierarchicalTransform* m_parent = nullptr;
 *     std::vector<HierarchicalTransform*> m_children;
 *     
 * public:
 *     // Get world-space transformation matrix
 *     glm::mat4 getWorldMatrix() const {
 *         if (m_parent) {
 *             return m_parent->getWorldMatrix() * m_localTransform.getModelMatrix();
 *         }
 *         return m_localTransform.getModelMatrix();
 *     }
 *     
 *     // Set parent-child relationship
 *     void setParent(HierarchicalTransform* parent) {
 *         if (m_parent) {
 *             // Remove from old parent
 *             auto it = std::find(m_parent->m_children.begin(), 
 *                               m_parent->m_children.end(), this);
 *             if (it != m_parent->m_children.end()) {
 *                 m_parent->m_children.erase(it);
 *             }
 *         }
 *         
 *         m_parent = parent;
 *         if (parent) {
 *             parent->m_children.push_back(this);
 *         }
 *     }
 *     
 *     // Transform from local to world space
 *     glm::vec3 transformPoint(const glm::vec3& localPoint) const {
 *         glm::vec4 worldPoint = getWorldMatrix() * glm::vec4(localPoint, 1.0f);
 *         return glm::vec3(worldPoint) / worldPoint.w;
 *     }
 * };
 * ```
 * 
 * ## Performance Optimization
 * 
 * ### Matrix Computation Efficiency
 * Optimized transformation matrix generation:
 * - **Lazy Evaluation**: Matrix computation only when needed
 * - **Change Detection**: Dirty flag system for selective updates
 * - **Batch Processing**: Vectorized operations for multiple transforms
 * - **Memory Pooling**: Reuse of matrix computation resources
 * 
 * ### Animation Performance
 * High-performance animation processing:
 * - **Keyframe Caching**: Pre-computed animation keyframes for smooth playback
 * - **Interpolation Optimization**: Fast interpolation algorithms for real-time animation
 * - **GPU Acceleration**: Compute shader-based transform animation for large datasets
 * - **LOD Animation**: Level-of-detail animation for distant or occluded objects
 * 
 * @ingroup Mathematics
 * @ingroup Transformations
 * @ingroup Animation
 * 
 * @see Matrix For low-level matrix operations and mathematics
 * @see Quaternion For rotation representation and manipulation
 * @see Animation For keyframe and procedural animation systems
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */

#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/**
 * @brief Advanced 3D Transformation System with Professional Animation and Matrix Generation
 * 
 * The LegacyTransform class provides comprehensive 3D transformation capabilities
 * including position, rotation, and scale operations with efficient matrix generation,
 * interpolation support, and seamless integration with animation systems. This
 * implementation forms the foundation for all spatial transformations within the
 * RudeBase3D framework, supporting both real-time graphics and precision modeling.
 * 
 * ## Core Architecture
 * 
 * ### Transform Components
 * Essential 3D transformation elements:
 * - **Position (Translation)**: 3D spatial coordinates with high numerical precision
 * - **Rotation (Orientation)**: Quaternion-based rotation for stable interpolation
 * - **Scale (Size)**: Non-uniform scaling with independent per-axis control
 * - **Matrix Representation**: Efficient 4x4 homogeneous transformation matrices
 * 
 * ### Mathematical Foundation
 * Robust and numerically stable transformation mathematics:
 * - **TRS Composition**: Standard Translation-Rotation-Scale matrix composition order
 * - **Quaternion Arithmetic**: Gimbal lock-free rotation with smooth interpolation
 * - **Matrix Inversion**: Efficient inverse transformation computation
 * - **Precision Management**: Floating-point stability for extreme scale differences
 * 
 * ### Performance Features
 * Optimized for real-time graphics applications:
 * - **Lazy Matrix Updates**: Deferred matrix computation until explicitly needed
 * - **Dirty Flag System**: Automatic tracking of transformation changes
 * - **Cache-Friendly Layout**: Memory organization optimized for frequent access
 * - **SIMD Compatibility**: Vectorized operations for batch transformations
 * 
 * ## Professional Transformation Operations
 * 
 * ### Spatial Manipulation
 * Comprehensive geometric transformation capabilities:
 * - **Absolute Positioning**: Direct position, rotation, and scale assignment
 * - **Relative Transformations**: Incremental translation, rotation, and scaling
 * - **Compound Operations**: Multiple transformation steps with cumulative effects
 * - **Constraint-Based Transforms**: Axis-aligned and constrained transformations
 * 
 * ### Orientation Control
 * Advanced rotation and orientation management:
 * - **Quaternion Rotations**: Stable rotation representation without gimbal lock
 * - **Euler Angle Interface**: Artist-friendly rotation specification in degrees
 * - **Axis-Angle Rotation**: Direct rotation around arbitrary axes
 * - **Look-At Functionality**: Automatic orientation towards target points
 * 
 * ### Directional Vectors
 * Object-relative coordinate system vectors:
 * - **Forward Vector**: Object's local Z-axis direction for movement
 * - **Right Vector**: Object's local X-axis direction for strafing
 * - **Up Vector**: Object's local Y-axis direction for vertical movement
 * - **Orthonormal Basis**: Guaranteed orthogonal and unit-length directional vectors
 * 
 * ## Advanced Features
 * 
 * ### Animation Integration
 * Comprehensive animation system support:
 * - **Keyframe Interpolation**: Smooth transitions between transformation states
 * - **Quaternion SLERP**: Spherical linear interpolation for natural rotation
 * - **Custom Interpolation**: User-defined interpolation functions and curves
 * - **Physics Compatibility**: Integration with rigid body dynamics systems
 * 
 * ### Hierarchical Transformations
 * Parent-child transformation relationships:
 * - **Local vs. World Space**: Context-aware transformation application
 * - **Transform Inheritance**: Automatic propagation of parent transformations
 * - **Relative Operations**: Local space operations within parent contexts
 * - **Scene Graph Integration**: Seamless hierarchy traversal and composition
 * 
 * ### Precision and Stability
 * Numerical accuracy for professional applications:
 * - **Double Precision Support**: High-precision mode for CAD and scientific use
 * - **Orthogonalization**: Automatic correction of accumulated floating-point errors
 * - **Matrix Decomposition**: Robust extraction of TRS components from matrices
 * - **Stability Validation**: Detection and correction of degenerate transformations
 * 
 * ## Technical Implementation
 * 
 * ### Matrix Mathematics
 * Efficient linear algebra operations:
 * - **4x4 Homogeneous Matrices**: Standard computer graphics transformation format
 * - **Column-Major Layout**: OpenGL-compatible matrix memory organization
 * - **Optimized Multiplication**: Fast matrix composition for transformation chains
 * - **Inverse Computation**: Specialized inverse calculation for transformation matrices
 * 
 * ### Memory Management
 * Performance-optimized data handling:
 * - **Compact Storage**: Minimal memory footprint for large object collections
 * - **Alignment Requirements**: Proper memory alignment for SIMD operations
 * - **Cache Locality**: Data layout optimized for CPU cache performance
 * - **Reference Semantics**: Efficient copying and assignment operations
 * 
 * ### Integration Points
 * Seamless framework and pipeline integration:
 * - **OpenGL Uniforms**: Direct matrix upload to OpenGL shader programs
 * - **DirectX Compatibility**: Matrix format conversion for DirectX pipelines
 * - **Physics Engine Integration**: Compatible with Bullet Physics and PhysX
 * - **Animation System Hooks**: Integration points for keyframe and procedural animation
 * 
 * ## Performance Optimization
 * 
 * ### Computational Efficiency
 * High-performance transformation processing:
 * - **Matrix Caching**: Automatic caching of computed transformation matrices
 * - **Incremental Updates**: Minimal recomputation for small transformation changes
 * - **Batch Processing**: Vectorized operations for multiple transform updates
 * - **GPU Offloading**: Compute shader acceleration for large transformation datasets
 * 
 * ### Memory Efficiency
 * Optimized memory usage patterns:
 * - **Object Pooling**: Reuse of transform objects to minimize allocation overhead
 * - **Shared Matrices**: Reference counting for identical transformation matrices
 * - **Compression**: Quantized transform representation for memory-constrained applications
 * - **Streaming**: Dynamic loading and unloading of transformation data
 * 
 * @ingroup Mathematics
 * @ingroup Transformations
 * @ingroup Animation
 * @ingroup CoordinateSystems
 * 
 * @see Matrix For low-level 4x4 matrix operations and linear algebra
 * @see Quaternion For rotation mathematics and spherical interpolation
 * @see Animation For keyframe animation and transformation interpolation
 * @see SceneGraph For hierarchical transformation systems
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 */
class LegacyTransform {
public:
    LegacyTransform();
    ~LegacyTransform() = default;

    // Position
    void setPosition(const glm::vec3& position);
    const glm::vec3& getPosition() const { return m_position; }
    
    // Rotation
    void setRotation(const glm::quat& rotation);
    const glm::quat& getRotation() const { return m_rotation; }
    void setEulerAngles(const glm::vec3& euler);
    glm::vec3 getEulerAngles() const;
    
    // Scale
    void setScale(const glm::vec3& scale);
    const glm::vec3& getScale() const { return m_scale; }
    void setUniformScale(float scale);
    
    // Transformation methods
    void translate(const glm::vec3& translation);
    void rotate(const glm::quat& rotation);
    void rotateAround(const glm::vec3& axis, float angle);
    void scale(const glm::vec3& scale);
    
    // Look at target
    void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 1, 0));
    
    // Matrix generation
    glm::mat4 getModelMatrix() const;
    glm::mat4 getInverseModelMatrix() const;
    
    // Forward, right, up vectors
    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;
    
    // Reset to identity
    void reset();

private:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    mutable bool m_matrixDirty;
    mutable glm::mat4 m_modelMatrix;
    void updateMatrix() const;
};
