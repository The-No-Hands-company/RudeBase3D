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
 * █  FILE: CoreComponents.h                                                      █
 * █  DESCRIPTION: ECS Core Component Definitions & Types                        █
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
 * █  This file contains the core component definitions for common data          █
 * █  structures used throughout the Entity Component System architecture        █
 * █  in the RudeBase3D application.                                             █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

#pragma once

#include "../Component.h"
#include "../Entity.h"
#include "../../core/mesh_forward.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <unordered_map>

// Forward declaration for Material class
class Material;

namespace rude::ecs {

/**
 * @brief Component that defines the position, rotation, and scale of an entity
 */
struct TransformComponent : public Component {
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f}; // Identity quaternion
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    /**
     * @brief Get the transformation matrix
     * @return 4x4 transformation matrix
     */
    glm::mat4 getMatrix() const {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMat = glm::mat4_cast(rotation);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
        return translation * rotationMat * scaleMat;
    }

    /**
     * @brief Set rotation from Euler angles (in degrees)
     * @param eulerAngles Euler angles in degrees (x, y, z)
     */
    void setRotationFromEuler(const glm::vec3& eulerAngles) {
        rotation = glm::quat(glm::radians(eulerAngles));
    }

    /**
     * @brief Get rotation as Euler angles (in degrees)
     * @return Euler angles in degrees (x, y, z)
     */
    glm::vec3 getEulerAngles() const {
        return glm::degrees(glm::eulerAngles(rotation));
    }
};

/**
 * @brief Component that contains mesh geometry data
 */
struct MeshComponent : public Component {
    std::shared_ptr<rude::Mesh> mesh;
    bool isDirty = true; // Flag to indicate if mesh needs to be uploaded to GPU

    MeshComponent() = default;
    explicit MeshComponent(std::shared_ptr<rude::Mesh> meshPtr) : mesh(std::move(meshPtr)) {}
};

/**
 * @brief Component that defines material properties for rendering
 */
struct MaterialComponent : public Component {
    std::shared_ptr<::Material> material;
    
    // Basic material properties (can be overridden by material)
    glm::vec3 albedo{1.0f, 1.0f, 1.0f};
    float metallic = 0.0f;
    float roughness = 0.5f;
    float emissive = 0.0f;

    MaterialComponent() = default;
    explicit MaterialComponent(std::shared_ptr<::Material> mat) : material(std::move(mat)) {}
};

/**
 * @brief Component that makes an entity renderable
 */
struct RenderableComponent : public Component {
    bool visible = true;
    bool castShadows = true;
    bool receiveShadows = true;
    float lodDistance = 1000.0f; // Level of detail distance
    
    // Rendering layers/masks
    uint32_t renderMask = 0xFFFFFFFF; // Which layers this object renders to
    uint32_t cullMask = 0xFFFFFFFF;   // Which layers can cull this object
};

/**
 * @brief Component for entities that need a name/identifier
 */
struct NameComponent : public Component {
    std::string name;
    
    NameComponent() = default;
    explicit NameComponent(const std::string& entityName) : name(entityName) {}
    explicit NameComponent(std::string&& entityName) : name(std::move(entityName)) {}
};

/**
 * @brief Component for hierarchical relationships between entities
 */
struct HierarchyComponent : public Component {
    Entity parent = NULL_ENTITY;
    std::vector<Entity> children;
    
    /**
     * @brief Add a child entity
     * @param child The child entity to add
     */
    void addChild(Entity child) {
        if (std::find(children.begin(), children.end(), child) == children.end()) {
            children.push_back(child);
        }
    }
    
    /**
     * @brief Remove a child entity
     * @param child The child entity to remove
     */
    void removeChild(Entity child) {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    }
    
    /**
     * @brief Check if entity has a specific child
     * @param child The child entity to check for
     * @return True if the entity is a child
     */
    bool hasChild(Entity child) const {
        return std::find(children.begin(), children.end(), child) != children.end();
    }
    
    /**
     * @brief Get the number of children
     * @return Number of child entities
     */
    size_t getChildCount() const {
        return children.size();
    }
};

/**
 * @brief Component for entities that can be selected in the editor
 */
struct SelectableComponent : public Component {
    bool isSelected = false;
    bool isHovered = false;
    bool isSelectable = true;
    
    // Selection highlighting
    glm::vec3 selectionColor{1.0f, 0.6f, 0.0f}; // Orange
    glm::vec3 hoverColor{0.8f, 0.8f, 1.0f};     // Light blue
};

/**
 * @brief Component for camera entities
 */
struct CameraComponent : public Component {
    // Camera properties
    float fieldOfView = 45.0f;      // In degrees
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float aspectRatio = 16.0f / 9.0f;
    
    // Camera type
    enum class ProjectionType {
        Perspective,
        Orthographic
    };
    ProjectionType projectionType = ProjectionType::Perspective;
    
    // Orthographic properties
    float orthographicSize = 10.0f;
    
    // Camera state
    bool isActive = false;
    bool isMainCamera = false;
    
    /**
     * @brief Get the projection matrix
     * @return 4x4 projection matrix
     */
    glm::mat4 getProjectionMatrix() const {
        if (projectionType == ProjectionType::Perspective) {
            return glm::perspective(glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);
        } else {
            float halfWidth = orthographicSize * aspectRatio * 0.5f;
            float halfHeight = orthographicSize * 0.5f;
            return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
        }
    }
};

/**
 * @brief Component for light entities
 */
struct LightComponent : public Component {
    enum class LightType {
        Directional,    // Like sun light
        Point,          // Like light bulb
        Spot,           // Like flashlight
        Area            // Area light
    };
    
    LightType type = LightType::Point;
    glm::vec3 color{1.0f, 1.0f, 1.0f};
    float intensity = 1.0f;
    
    // Point and spot light properties
    float range = 10.0f;
    float innerConeAngle = 30.0f; // For spot lights (degrees)
    float outerConeAngle = 45.0f; // For spot lights (degrees)
    
    // Shadow properties
    bool castShadows = true;
    float shadowBias = 0.005f;
    uint32_t shadowMapSize = 1024;
};

} // namespace rude::ecs
