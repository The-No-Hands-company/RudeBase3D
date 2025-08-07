//==============================================================================
// RudeBase3D Engine - transform.hpp
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
 * @file transform.hpp
 * @brief 3D spatial transformation with position, rotation, and scale
 * 
 * This file contains the Transform struct which represents a complete 3D
 * spatial transformation including translation, rotation (as quaternion),
 * and non-uniform scaling. Transform provides efficient matrix computation
 * and convenient manipulation methods for 3D object positioning.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup Core
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/constants.hpp>

/**
 * @addtogroup Core
 * @{
 */

/**
 * @brief Complete 3D spatial transformation with TRS components
 * 
 * The Transform struct encapsulates a complete 3D spatial transformation
 * using the standard Translation-Rotation-Scale (TRS) decomposition.
 * It provides efficient quaternion-based rotation, convenient manipulation
 * methods, and optimized matrix generation for rendering pipelines.
 * 
 * ## Mathematical Foundation
 * The transform represents the mathematical operation: **T × R × S**
 * - **Translation (T)**: 3D position offset in world/parent space
 * - **Rotation (R)**: Orientation using unit quaternion representation
 * - **Scale (S)**: Non-uniform scaling along each axis
 * 
 * ## Quaternion Advantages
 * Using quaternions for rotation provides several benefits:
 * - **No Gimbal Lock**: Smooth rotation in all directions without singularities
 * - **Efficient Interpolation**: Natural spherical linear interpolation (SLERP)
 * - **Compact Storage**: 4 components vs 9 for rotation matrices
 * - **Composition**: Efficient rotation combination through multiplication
 * 
 * ## Coordinate System
 * RudeBase3D uses a **right-handed coordinate system**:
 * - **X-axis**: Points right (positive X = right)
 * - **Y-axis**: Points up (positive Y = up)  
 * - **Z-axis**: Points toward viewer (positive Z = forward, out of screen)
 * - **Rotation**: Counter-clockwise when looking down the axis
 * 
 * ## Usage Patterns
 * ```cpp
 * // Create transform at origin
 * Transform transform;
 * 
 * // Position object in space
 * transform.setPosition({10, 5, 0});
 * 
 * // Rotate 45 degrees around Y axis
 * glm::quat rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
 * transform.setRotation(rotation);
 * 
 * // Scale uniformly
 * transform.setScale({2.0f, 2.0f, 2.0f});
 * 
 * // Get transformation matrix for rendering
 * glm::mat4 modelMatrix = transform.toMatrix();
 * 
 * // Use direction vectors for movement
 * glm::vec3 forward = transform.getForward();
 * transform.translate(forward * speed * deltaTime);
 * ```
 * 
 * ## Animation and Interpolation
 * ```cpp
 * // Linear interpolation between transforms
 * Transform start, end, current;
 * float t = 0.5f; // 50% interpolation
 * 
 * current.position = glm::mix(start.position, end.position, t);
 * current.rotation = glm::slerp(start.rotation, end.rotation, t);
 * current.scale = glm::mix(start.scale, end.scale, t);
 * ```
 * 
 * ## Performance Considerations
 * - Matrix computation is deferred until needed (lazy evaluation)
 * - Quaternion operations are more efficient than Euler angles
 * - Direction vector computation involves quaternion-vector multiplication
 * - Avoid frequent Euler angle conversions due to trigonometric cost
 * 
 * @see Entity for transform hierarchy and world space computation
 * @see Camera for view transform and projection integration
 * @see glm::quat for quaternion mathematics
 * 
 * @ingroup Core
 */
struct Transform {
    // ========================================================================
    // Core Transform Components
    // ========================================================================
    
    /**
     * @brief 3D position in world or parent space
     * 
     * Represents the translation component of the transformation.
     * For root objects, this is world space position. For child objects,
     * this is position relative to the parent's coordinate system.
     * 
     * @note Default: origin (0, 0, 0)
     * @note Units are in world units (typically meters for realistic scales)
     */
    glm::vec3 position{0, 0, 0};
    
    /**
     * @brief 3D orientation as unit quaternion
     * 
     * Represents the rotation component using a unit quaternion for
     * efficient and robust 3D rotations. Quaternions avoid gimbal lock
     * and provide smooth interpolation between orientations.
     * 
     * ## Quaternion Format
     * - **w**: Real/scalar component
     * - **x, y, z**: Imaginary/vector components
     * - **Constraint**: ||q|| = 1 (unit quaternion)
     * 
     * @note Default: identity rotation (1, 0, 0, 0) - no rotation
     * @note Must remain normalized for correct behavior
     */
    glm::quat rotation{1, 0, 0, 0};
    
    /**
     * @brief 3D non-uniform scale factors
     * 
     * Represents the scaling component along each axis. Values greater
     * than 1.0 increase size, values between 0 and 1 decrease size.
     * Negative values create reflection (mirroring).
     * 
     * ## Scale Guidelines
     * - **Uniform Scaling**: Use same value for all axes (e.g., {2,2,2})
     * - **Non-uniform**: Different values per axis (e.g., {1,2,0.5})
     * - **Avoid Zero**: Zero scale creates degenerate transformations
     * - **Negative Values**: Create reflections but may affect normals
     * 
     * @note Default: uniform scale (1, 1, 1) - original size
     * @note Large scale differences may affect lighting calculations
     */
    glm::vec3 scale{1, 1, 1};

    // ========================================================================
    // Matrix Generation
    // ========================================================================
    
    /**
     * @brief Generate 4x4 transformation matrix
     * 
     * Computes the complete transformation matrix that represents this
     * transform's position, rotation, and scale. The matrix can be used
     * directly in rendering pipelines and shader uniforms.
     * 
     * ## Matrix Composition
     * The resulting matrix is: **Translation × Rotation × Scale**
     * 
     * @return 4x4 transformation matrix in column-major order
     * 
     * @note Matrix is computed fresh each call (no caching)
     * @note Compatible with OpenGL matrix conventions
     * @note Use for vertex transformation in shaders
     * 
     * @see getModelMatrix() for alias with clearer semantic meaning
     */
    glm::mat4 toMatrix() const;
    
    /**
     * @brief Alias for toMatrix() with semantic clarity
     * 
     * Provides the transformation matrix with a name that clearly indicates
     * its use as a model matrix in rendering pipelines.
     * 
     * @return 4x4 model transformation matrix
     * 
     * @note Identical to toMatrix(), provided for API clarity
     * @note Commonly used in MVP (Model-View-Projection) matrix computation
     */
    glm::mat4 getModelMatrix() const { return toMatrix(); }

    // ========================================================================
    // Component Access
    // ========================================================================
    
    /**
     * @brief Get current position
     * 
     * @return Copy of the position vector
     * 
     * @note Returns copy for value safety
     */
    glm::vec3 getPosition() const { return position; }
    
    /**
     * @brief Get current rotation quaternion
     * 
     * @return Copy of the rotation quaternion
     * 
     * @note Returns copy for value safety
     * @note Quaternion remains normalized
     */
    glm::quat getRotation() const { return rotation; }
    
    /**
     * @brief Get current scale factors
     * 
     * @return Copy of the scale vector
     * 
     * @note Returns copy for value safety
     */
    glm::vec3 getScale() const { return scale; }

    // ========================================================================
    // Component Modification
    // ========================================================================
    
    /**
     * @brief Set new position
     * 
     * @param pos New position in world or parent space
     * 
     * @post Transform position is updated to specified value
     */
    void setPosition(const glm::vec3& pos) { position = pos; }
    
    /**
     * @brief Set new rotation quaternion
     * 
     * @param rot New rotation as unit quaternion
     * 
     * @pre rot should be normalized (unit quaternion)
     * @post Transform rotation is updated to specified value
     * 
     * @note Assumes input quaternion is already normalized
     * @note For safety, consider normalizing input: glm::normalize(rot)
     */
    void setRotation(const glm::quat& rot) { rotation = rot; }
    
    /**
     * @brief Set new scale factors
     * 
     * @param scl New scale factors for each axis
     * 
     * @post Transform scale is updated to specified value
     * 
     * @warning Avoid zero or very small scale values
     * @note Negative values create reflections
     */
    void setScale(const glm::vec3& scl) { scale = scl; }
    
    // ========================================================================
    // Transform Operations
    // ========================================================================
    
    /**
     * @brief Apply translation offset to current position
     * 
     * Adds the specified translation vector to the current position,
     * effectively moving the transform by the given offset.
     * 
     * @param translation Offset to add to current position
     * 
     * @post position = position + translation
     * 
     * @note Translation is applied in current coordinate space
     * @note For relative movement, use direction vectors (getForward(), etc.)
     */
    void translate(const glm::vec3& translation) {
        position += translation;
    }
    
    // ========================================================================
    // Euler Angle Interface
    // ========================================================================
    
    /**
     * @brief Set rotation using Euler angles
     * 
     * Sets the rotation using Euler angles in degrees with standard
     * X-Y-Z rotation order. This provides an intuitive interface for
     * rotation specification, especially for user interfaces.
     * 
     * @param euler Euler angles in degrees (X=pitch, Y=yaw, Z=roll)
     * 
     * @post Rotation quaternion is updated to represent the Euler angles
     * 
     * ## Euler Angle Convention
     * - **X (Pitch)**: Rotation around X-axis (nose up/down)
     * - **Y (Yaw)**: Rotation around Y-axis (turn left/right)
     * - **Z (Roll)**: Rotation around Z-axis (lean left/right)
     * 
     * @note Angles are in degrees for user convenience
     * @note Uses intrinsic X-Y-Z rotation order
     * @warning Subject to gimbal lock at ±90° pitch
     */
    void setEulerAngles(const glm::vec3& euler) {
        // euler is in degrees, convert to radians
        glm::vec3 radians = glm::radians(euler);
        rotation = glm::quat(radians);
    }
    
    /**
     * @brief Get rotation as Euler angles
     * 
     * Extracts Euler angles from the current rotation quaternion using
     * X-Y-Z rotation order. Useful for user interface display and
     * debugging rotation values.
     * 
     * @return Euler angles in degrees (X=pitch, Y=yaw, Z=roll)
     * 
     * ## Return Convention
     * - **X (Pitch)**: -90° to +90° (nose up/down)
     * - **Y (Yaw)**: -180° to +180° (turn left/right)
     * - **Z (Roll)**: -180° to +180° (lean left/right)
     * 
     * @note Returns angles in degrees for user convenience
     * @note Conversion may not preserve exact input due to quaternion normalization
     * @warning Multiple Euler angle representations exist for some rotations
     */
    glm::vec3 getEulerAngles() const {
        glm::vec3 euler = glm::eulerAngles(rotation);
        return glm::degrees(euler);
    }
    
    // ========================================================================
    // Direction Vectors
    // ========================================================================
    
    /**
     * @brief Get forward direction vector
     * 
     * Computes the forward direction of the transform based on its current
     * rotation. In RudeBase3D's coordinate system, forward is the negative
     * Z direction rotated by the transform's orientation.
     * 
     * @return Normalized forward direction vector in world space
     * 
     * @note Forward direction is -Z axis rotated by transform rotation
     * @note Result is always normalized (unit length)
     * @note Useful for camera movement and object orientation
     */
    glm::vec3 getForward() const {
        return glm::normalize(rotation * glm::vec3(0, 0, -1));
    }
    
    /**
     * @brief Get right direction vector
     * 
     * Computes the right direction of the transform based on its current
     * rotation. This represents the local X-axis after rotation is applied.
     * 
     * @return Normalized right direction vector in world space
     * 
     * @note Right direction is +X axis rotated by transform rotation
     * @note Result is always normalized (unit length)
     * @note Useful for strafing movement and camera controls
     */
    glm::vec3 getRight() const {
        return glm::normalize(rotation * glm::vec3(1, 0, 0));
    }
    
    /**
     * @brief Get up direction vector
     * 
     * Computes the up direction of the transform based on its current
     * rotation. This represents the local Y-axis after rotation is applied.
     * 
     * @return Normalized up direction vector in world space
     * 
     * @note Up direction is +Y axis rotated by transform rotation
     * @note Result is always normalized (unit length)
     * @note Useful for vertical movement and camera orientation
     */
    glm::vec3 getUp() const {
        return glm::normalize(rotation * glm::vec3(0, 1, 0));
    }
};

/**
 * @}
 */ 