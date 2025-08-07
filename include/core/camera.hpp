//==============================================================================
// RudeBase3D Engine - camera.hpp
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
 * @file camera.hpp
 * @brief 3D camera with view and projection management
 * 
 * This file contains the Camera class which handles view and projection
 * transformations for 3D rendering. The camera supports both perspective
 * and orthographic projections, interactive navigation controls, and
 * screen-to-world coordinate transformations for user interaction.
 * 
 * @author RudeBase3D Development Team
 * @date 2025
 * 
 * @ingroup Core
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "core/transform.hpp"

/**
 * @addtogroup Core
 * @{
 */

/**
 * @brief 3D camera with perspective/orthographic projection and navigation
 * 
 * The Camera class provides complete camera functionality for 3D rendering,
 * including view and projection matrix management, interactive navigation
 * controls, and coordinate space transformations. It supports both perspective
 * and orthographic projections with smooth transitions between them.
 * 
 * ## Core Features
 * - **Dual Projection Modes**: Perspective for realistic views, orthographic for technical/CAD work
 * - **Interactive Navigation**: Orbit, pan, zoom controls for intuitive scene exploration
 * - **Matrix Management**: Automatic view/projection matrix computation and caching
 * - **Coordinate Transformation**: Screen-to-world and world-to-screen conversions
 * - **Viewport Integration**: Automatic aspect ratio and viewport size handling
 * 
 * ## Projection Types
 * 
 * ### Perspective Projection
 * - Simulates human vision with realistic depth perception
 * - Objects appear smaller with distance
 * - Defined by field-of-view, aspect ratio, and near/far planes
 * - Best for artistic and realistic rendering
 * 
 * ### Orthographic Projection
 * - Parallel projection without perspective distortion
 * - Objects maintain size regardless of distance
 * - Defined by left/right/top/bottom bounds and near/far planes
 * - Best for technical drawings and CAD applications
 * 
 * ## Navigation Controls
 * The camera provides standard 3D navigation patterns:
 * - **Orbit**: Rotate around a pivot point (common for object inspection)
 * - **Pan**: Move parallel to the view plane (2D translation in 3D space)
 * - **Zoom**: Move closer/farther from target (distance-based scaling)
 * 
 * ## Usage Examples
 * ```cpp
 * // Create and configure camera
 * Camera camera;
 * camera.setPerspective(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
 * camera.setViewportSize(1920, 1080);
 * 
 * // Position camera to look at scene
 * camera.lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
 * 
 * // Interactive navigation
 * camera.orbit(mouseX * 0.01f, mouseY * 0.01f, glm::vec3(0, 0, 0));
 * camera.zoom(scrollDelta * 0.1f);
 * 
 * // Use for rendering
 * shader.setMatrix("view", camera.getViewMatrix());
 * shader.setMatrix("projection", camera.getProjectionMatrix());
 * 
 * // Mouse picking
 * glm::vec3 worldPos = camera.screenToWorldPoint(mouseX, mouseY, 0.5f);
 * ```
 * 
 * ## Performance Considerations
 * - View and projection matrices are cached and only recomputed when needed
 * - Transform updates automatically invalidate cached matrices
 * - Matrix operations are optimized using GLM's SIMD instructions where available
 * - Viewport updates trigger aspect ratio recalculation
 * 
 * @see Transform for spatial transformation details
 * @see Scene for camera integration with scene rendering
 * @see InputManager for camera control integration
 * 
 * @ingroup Core
 */
class Camera {
public:
    // ========================================================================
    // Construction and Destruction
    // ========================================================================
    
    /**
     * @brief Default constructor
     * 
     * Creates a camera with default perspective projection settings.
     * The camera is positioned at the origin looking down the negative Z axis.
     * 
     * @post Camera has perspective projection (45° FOV, 16:9 aspect, 0.1-1000 range)
     * @post Camera transform is at identity (origin, no rotation, unit scale)
     * @post View and projection matrices are computed and cached
     */
    Camera();
    
    /**
     * @brief Default destructor
     * 
     * Cleans up camera resources. The destructor is defaulted as the camera
     * uses only stack-allocated data and GLM types.
     */
    ~Camera() = default;

    // ========================================================================
    // Transform Management
    // ========================================================================
    
    /**
     * @brief Get mutable reference to the camera's transform
     * 
     * Provides direct access to the camera's spatial transform for advanced
     * manipulation. Changes to the transform automatically invalidate the
     * cached view matrix.
     * 
     * @return Mutable reference to the Transform object
     * 
     * @note Modifying the transform triggers view matrix recomputation
     * @see Transform class for detailed transformation operations
     */
    Transform& getTransform() { return m_transform; }
    
    /**
     * @brief Get read-only reference to the camera's transform
     * 
     * @return Const reference to the Transform object
     * 
     * @note Use this for reading transform data without triggering updates
     */
    const Transform& getTransform() const { return m_transform; }
    
    // ========================================================================
    // Projection Configuration
    // ========================================================================
    
    /**
     * @brief Configure perspective projection parameters
     * 
     * Sets up the camera for perspective projection with realistic depth
     * perception. Objects appear smaller with distance, suitable for
     * most 3D rendering scenarios.
     * 
     * @param fov Field of view angle in degrees (vertical FOV)
     * @param aspectRatio Width/height ratio of the viewport
     * @param nearPlane Distance to near clipping plane (> 0)
     * @param farPlane Distance to far clipping plane (> nearPlane)
     * 
     * @pre fov must be in range (0, 180) degrees
     * @pre aspectRatio must be > 0
     * @pre nearPlane must be > 0
     * @pre farPlane must be > nearPlane
     * 
     * @post Camera uses perspective projection
     * @post Projection matrix is recomputed and cached
     * 
     * @note Common FOV values: 45° (normal), 60° (wide), 30° (telephoto)
     * @note Larger FOV values create more distortion at viewport edges
     */
    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
    
    /**
     * @brief Configure orthographic projection parameters
     * 
     * Sets up the camera for orthographic projection without perspective
     * distortion. Objects maintain size regardless of distance, suitable
     * for technical drawings and CAD applications.
     * 
     * @param left Left edge of the projection volume
     * @param right Right edge of the projection volume (> left)
     * @param bottom Bottom edge of the projection volume
     * @param top Top edge of the projection volume (> bottom)
     * @param nearPlane Distance to near clipping plane
     * @param farPlane Distance to far clipping plane (> nearPlane)
     * 
     * @pre right > left
     * @pre top > bottom
     * @pre farPlane > nearPlane
     * 
     * @post Camera uses orthographic projection
     * @post Projection matrix is recomputed and cached
     * 
     * @note Use symmetric bounds around zero for centered projection
     * @note Bounds define the visible area in world units at z=0 plane
     */
    void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    
    // ========================================================================
    // Projection Parameter Queries
    // ========================================================================
    
    /**
     * @brief Get the field of view angle for perspective projection
     * 
     * @return Vertical field of view in degrees
     * 
     * @note Only meaningful when using perspective projection
     * @see isOrthographic() to check projection type
     */
    float getFOV() const { return m_fov; }
    
    /**
     * @brief Get the current aspect ratio
     * 
     * @return Width/height ratio of the viewport
     * 
     * @note Automatically updated when viewport size changes
     */
    float getAspectRatio() const { return m_aspectRatio; }
    
    /**
     * @brief Get the near clipping plane distance
     * 
     * @return Distance to near clipping plane
     * 
     * @note Objects closer than this distance are clipped (not rendered)
     */
    float getNearPlane() const { return m_nearPlane; }
    
    /**
     * @brief Get the far clipping plane distance
     * 
     * @return Distance to far clipping plane
     * 
     * @note Objects farther than this distance are clipped (not rendered)
     */
    float getFarPlane() const { return m_farPlane; }
    
    /**
     * @brief Check if camera is using orthographic projection
     * 
     * @return true if orthographic, false if perspective
     * 
     * @note Use this to determine which projection parameters are relevant
     */
    bool isOrthographic() const { return m_isOrthographic; }
    
    // ========================================================================
    // Matrix Access
    // ========================================================================
    
    /**
     * @brief Get the current view matrix
     * 
     * Returns the transformation matrix that converts from world space
     * to camera/view space. This matrix represents the camera's position
     * and orientation in the world.
     * 
     * @return Const reference to the 4x4 view matrix
     * 
     * @note Matrix is cached and automatically updated when transform changes
     * @note View matrix is the inverse of the camera's world transform
     */
    const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
    
    /**
     * @brief Get the current projection matrix
     * 
     * Returns the transformation matrix that converts from camera/view space
     * to normalized device coordinates (NDC). This matrix handles the
     * perspective or orthographic projection.
     * 
     * @return Const reference to the 4x4 projection matrix
     * 
     * @note Matrix is cached and automatically updated when projection parameters change
     */
    const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
    
    /**
     * @brief Get combined view-projection matrix
     * 
     * Returns the result of multiplying projection * view matrices,
     * providing a single transformation from world space to NDC.
     * 
     * @return Combined view-projection matrix (projection * view)
     * 
     * @note More efficient than manually multiplying matrices in shaders
     * @note Commonly used for vertex transformation in rendering pipelines
     */
    glm::mat4 getViewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }
    
    // ========================================================================
    // Camera Navigation
    // ========================================================================
    
    /**
     * @brief Position camera to look at a specific target
     * 
     * Sets the camera position and orientation to look from a specific eye
     * position toward a target point, with a specified up direction.
     * 
     * @param eye Camera position in world space
     * @param center Target point to look at in world space
     * @param up Up vector defining camera roll (typically {0,1,0})
     * 
     * @post Camera transform is updated to look from eye toward center
     * @post View matrix is automatically recomputed
     * 
     * @note This is equivalent to OpenGL's gluLookAt function
     * @note Up vector should not be parallel to the view direction
     */
    void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
    
    /**
     * @brief Orbit camera around a pivot point
     * 
     * Rotates the camera around a specified pivot point, maintaining the
     * distance while changing the viewing angle. Commonly used for object
     * inspection and 3D model viewers.
     * 
     * @param deltaX Horizontal rotation amount in radians
     * @param deltaY Vertical rotation amount in radians
     * @param pivot World space point to orbit around
     * 
     * @post Camera position is updated to maintain distance from pivot
     * @post Camera orientation is updated to continue looking at pivot
     * @post View matrix is automatically recomputed
     * 
     * @note Positive deltaX typically rotates left, positive deltaY rotates up
     * @note Distance to pivot is preserved during orbiting
     */
    void orbit(float deltaX, float deltaY, const glm::vec3& pivot);
    
    /**
     * @brief Pan camera parallel to the view plane
     * 
     * Moves the camera sideways and up/down relative to its current
     * orientation, without changing the viewing direction. Useful for
     * exploring large scenes or adjusting framing.
     * 
     * @param deltaX Horizontal movement amount (camera-relative)
     * @param deltaY Vertical movement amount (camera-relative)
     * 
     * @post Camera position is updated by movement in camera-relative space
     * @post Camera orientation remains unchanged
     * @post View matrix is automatically recomputed
     * 
     * @note Movement is in camera space: positive X moves right, positive Y moves up
     * @note Distance and direction to viewed objects changes
     */
    void pan(float deltaX, float deltaY);
    
    /**
     * @brief Zoom camera closer to or farther from target
     * 
     * Moves the camera forward or backward along its viewing direction.
     * For perspective cameras, this changes the apparent size of objects.
     * For orthographic cameras, consider adjusting projection bounds instead.
     * 
     * @param delta Distance to move (positive = forward, negative = backward)
     * 
     * @post Camera position is updated along the forward direction
     * @post Camera orientation remains unchanged
     * @post View matrix is automatically recomputed
     * 
     * @note For orthographic cameras, consider using projection scaling instead
     * @note Be careful not to zoom past the near clipping plane
     */
    void zoom(float delta);
    
    // ========================================================================
    // Camera Vectors and Position
    // ========================================================================
    
    /**
     * @brief Get camera position in world space
     * 
     * @return 3D position vector of the camera
     * 
     * @note Equivalent to getTransform().getPosition()
     */
    glm::vec3 getPosition() const { return m_transform.getPosition(); }
    
    /**
     * @brief Get camera forward direction vector
     * 
     * Returns the normalized vector pointing in the direction the camera
     * is looking. This is typically the negative Z axis in camera space.
     * 
     * @return Normalized forward direction vector in world space
     * 
     * @note Useful for ray casting and collision detection
     */
    glm::vec3 getForward() const { return m_transform.getForward(); }
    
    /**
     * @brief Get camera up direction vector
     * 
     * Returns the normalized vector pointing "up" relative to the camera.
     * This is typically the Y axis in camera space.
     * 
     * @return Normalized up direction vector in world space
     * 
     * @note Defines camera roll orientation
     */
    glm::vec3 getUp() const { return m_transform.getUp(); }
    
    /**
     * @brief Get camera right direction vector
     * 
     * Returns the normalized vector pointing "right" relative to the camera.
     * This is typically the X axis in camera space.
     * 
     * @return Normalized right direction vector in world space
     * 
     * @note Useful for camera movement and strafing operations
     */
    glm::vec3 getRight() const { return m_transform.getRight(); }
    
    // ========================================================================
    // Coordinate Transformations
    // ========================================================================
    
    /**
     * @brief Convert screen coordinates to world space point
     * 
     * Transforms 2D screen coordinates and a depth value to a 3D point
     * in world space. Useful for mouse picking and 3D interaction.
     * 
     * @param screenX Screen X coordinate (typically 0 to viewport width)
     * @param screenY Screen Y coordinate (typically 0 to viewport height)
     * @param depth Normalized depth value (0.0 = near plane, 1.0 = far plane)
     * 
     * @return 3D point in world space coordinates
     * 
     * @note Screen coordinates should be in viewport pixel coordinates
     * @note Depth 0.0 projects to near plane, 1.0 to far plane
     * @note For mouse picking, use depth 0.0 to get ray origin
     */
    glm::vec3 screenToWorldPoint(float screenX, float screenY, float depth) const;
    
    /**
     * @brief Convert screen coordinates to world space direction
     * 
     * Transforms 2D screen coordinates to a normalized direction vector
     * in world space. Useful for creating picking rays and mouse-based
     * 3D interactions.
     * 
     * @param screenX Screen X coordinate (typically 0 to viewport width)
     * @param screenY Screen Y coordinate (typically 0 to viewport height)
     * 
     * @return Normalized direction vector in world space
     * 
     * @note Combined with camera position, creates a picking ray
     * @note Direction is normalized and points away from camera
     */
    glm::vec3 screenToWorldDirection(float screenX, float screenY) const;
    
    /**
     * @brief Get current viewport dimensions as 3D vector
     * 
     * @return Vector containing (width, height, 0) of viewport
     * 
     * @note Z component is always 0, included for convenience
     * @note Useful for normalized coordinate calculations
     */
    glm::vec3 getViewportSize() const { return glm::vec3(m_viewportWidth, m_viewportHeight, 0); }
    
    /**
     * @brief Get viewport width in pixels
     * 
     * @return Width of the viewport in pixels
     */
    int getViewportWidth() const { return m_viewportWidth; }
    
    /**
     * @brief Get viewport height in pixels
     * 
     * @return Height of the viewport in pixels
     */
    int getViewportHeight() const { return m_viewportHeight; }
    
    // ========================================================================
    // Legacy Compatibility
    // ========================================================================
    
    /**
     * @brief Legacy alias for getProjectionMatrix()
     * 
     * @return Const reference to the projection matrix
     * 
     * @deprecated Use getProjectionMatrix() instead
     * @note Provided for compatibility with older code
     */
    const glm::mat4& getProjMatrix() const { return getProjectionMatrix(); }
    
    /**
     * @brief Set viewport dimensions and update aspect ratio
     * 
     * Updates the viewport size and automatically recalculates the aspect
     * ratio for perspective projection. Triggers projection matrix update.
     * 
     * @param width New viewport width in pixels
     * @param height New viewport height in pixels
     * 
     * @pre width and height must be > 0
     * 
     * @post Viewport dimensions are updated
     * @post Aspect ratio is recalculated
     * @post Projection matrix is recomputed for perspective cameras
     */
    void setViewportSize(int width, int height) { m_viewportWidth = width; m_viewportHeight = height; updateAspectRatio(); }
    
    // ========================================================================
    // Matrix Update Operations
    // ========================================================================
    
    /**
     * @brief Force update of the view matrix
     * 
     * Manually triggers recomputation of the view matrix from the current
     * transform. Normally called automatically when transform changes.
     * 
     * @post View matrix is recomputed from current transform state
     * 
     * @note Usually not needed as updates are automatic
     * @note Useful for custom transform modifications
     */
    void updateViewMatrix();
    
    /**
     * @brief Force update of the projection matrix
     * 
     * Manually triggers recomputation of the projection matrix from current
     * projection parameters. Normally called automatically when parameters change.
     * 
     * @post Projection matrix is recomputed from current parameters
     * 
     * @note Usually not needed as updates are automatic
     * @note Useful after direct parameter modifications
     */
    void updateProjectionMatrix();

private:
    // ========================================================================
    // Internal Data Members
    // ========================================================================
    
    /**
     * @brief Camera spatial transformation
     * 
     * Contains position, rotation, and scale of the camera in world space.
     * The view matrix is computed as the inverse of this transform.
     */
    Transform m_transform;
    
    /**
     * @brief Current projection mode flag
     * 
     * true for orthographic projection, false for perspective projection.
     * Determines which set of projection parameters are used.
     */
    bool m_isOrthographic;
    
    /**
     * @brief Perspective field of view angle
     * 
     * Vertical field of view in degrees for perspective projection.
     * Only used when m_isOrthographic is false.
     */
    float m_fov;
    
    /**
     * @brief Viewport aspect ratio
     * 
     * Width/height ratio of the viewport, automatically calculated
     * from viewport dimensions. Used for perspective projection.
     */
    float m_aspectRatio;
    
    /**
     * @brief Near clipping plane distance
     * 
     * Distance from camera to near clipping plane. Objects closer
     * than this distance are not rendered.
     */
    float m_nearPlane;
    
    /**
     * @brief Far clipping plane distance
     * 
     * Distance from camera to far clipping plane. Objects farther
     * than this distance are not rendered.
     */
    float m_farPlane;
    
    /**
     * @brief Orthographic projection bounds
     * 
     * Left, right, bottom, and top boundaries for orthographic projection.
     * Only used when m_isOrthographic is true.
     */
    float m_left, m_right, m_bottom, m_top;
    
    /**
     * @brief Cached view transformation matrix
     * 
     * Pre-computed matrix that transforms from world space to camera space.
     * Updated automatically when camera transform changes.
     */
    glm::mat4 m_viewMatrix;
    
    /**
     * @brief Cached projection transformation matrix
     * 
     * Pre-computed matrix that transforms from camera space to NDC.
     * Updated automatically when projection parameters change.
     */
    glm::mat4 m_projectionMatrix;
    
    /**
     * @brief Viewport width in pixels
     * 
     * Current width of the rendering viewport. Used for aspect ratio
     * calculation and coordinate transformations.
     */
    int m_viewportWidth;
    
    /**
     * @brief Viewport height in pixels
     * 
     * Current height of the rendering viewport. Used for aspect ratio
     * calculation and coordinate transformations.
     */
    int m_viewportHeight;
    
    // ========================================================================
    // Internal Helper Methods
    // ========================================================================
    
    /**
     * @brief Update aspect ratio from viewport dimensions
     * 
     * Recalculates the aspect ratio from current viewport width and height.
     * Automatically called when viewport size changes.
     * 
     * @post Aspect ratio is updated to width/height
     * @post Projection matrix is updated for perspective cameras
     */
    void updateAspectRatio();
};

/**
 * @}
 */
