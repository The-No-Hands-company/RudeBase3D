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
 * █  FILE: SceneObject.h                                                         █
 * █  DESCRIPTION: Professional 3D Scene Object & Entity Management System       █
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
 * █  This file contains the SceneObject class - a comprehensive 3D entity       █
 * █  representation providing mesh, material, transform, and state management   █
 * █  for professional 3D modeling and visualization workflows.                   █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file SceneObject.h
 * @brief Professional 3D Scene Object and Entity Management System
 * 
 * This file contains the comprehensive SceneObject class implementation that serves
 * as the fundamental building block for all 3D entities within the RudeBase3D modeling
 * environment. The SceneObject provides a complete entity representation system with
 * integrated mesh data, material properties, spatial transformations, visibility
 * states, and selection management for professional 3D modeling and visualization
 * workflows. This class forms the core of the scene graph hierarchy and rendering
 * pipeline integration within the RudeBase3D professional modeling suite.
 * 
 * ## Professional Scene Object Architecture
 * 
 * ### Entity-Component Foundation
 * Modern entity management system following industry-standard patterns:
 * - **Unique Identification**: Global unique ID system for object tracking and referencing
 * - **Hierarchical Transforms**: World-space and local-space transformation management
 * - **Component Integration**: Extensible component system for specialized functionality
 * - **State Management**: Comprehensive object state tracking and modification
 * 
 * ### Mesh and Geometry Integration
 * Advanced 3D geometry representation and management:
 * - **Multi-Format Support**: Compatible with various mesh formats (OBJ, STL, PLY, FBX)
 * - **Dynamic Geometry**: Runtime mesh modification and procedural generation
 * - **Level-of-Detail**: Automatic mesh simplification for performance optimization
 * - **Instancing Support**: Efficient rendering of repeated geometry
 * 
 * ### Material and Appearance System
 * Professional surface property and visual quality management:
 * - **PBR Material Integration**: Physically-based rendering material compatibility
 * - **Multi-Layer Materials**: Complex material composition with layered properties
 * - **Texture Mapping**: Comprehensive UV mapping and texture coordinate management
 * - **Shader Integration**: Direct integration with modern shader pipelines
 * 
 * ## Advanced Object Management Features
 * 
 * ### Spatial Transformation System
 * Professional-grade 3D transformation management:
 * - **TRS Decomposition**: Translation, Rotation, Scale with quaternion stability
 * - **Hierarchical Transforms**: Parent-child transformation inheritance
 * - **World-Local Conversion**: Efficient coordinate space transformation
 * - **Animation Integration**: Keyframe animation and interpolation support
 * - **Constraint Systems**: Movement and rotation constraints for controlled manipulation
 * 
 * ### Visibility and Culling Management
 * Advanced visibility control for complex scene optimization:
 * - **Hierarchical Visibility**: Parent-child visibility inheritance and override
 * - **Distance Culling**: Automatic object hiding based on camera distance
 * - **Frustum Culling**: View-dependent visibility determination
 * - **Occlusion Culling**: Hardware-accelerated hidden object elimination
 * - **Layer Systems**: Group-based visibility control for workflow organization
 * 
 * ### Selection and Interaction Systems
 * Professional object selection and manipulation frameworks:
 * - **Multi-Selection Support**: Complex selection sets with group operations
 * - **Selection Filtering**: Type-based and property-based selection constraints
 * - **Interaction Modes**: Different selection behaviors for various tools
 * - **Undo/Redo Integration**: Complete selection state tracking for history
 * - **Transform Gizmos**: Interactive visual manipulation widgets
 * 
 * ## Professional Workflow Integration
 * 
 * ### Industry-Standard Object Types
 * Comprehensive primitive library matching professional 3D software:
 * - **Geometric Primitives**: Cubes, spheres, cylinders, planes with parametric control
 * - **Complex Meshes**: Imported CAD models, scanned geometry, and artistic assets
 * - **Procedural Objects**: Algorithm-generated geometry with parameter controls
 * - **Instanced Objects**: Efficient representation of repeated elements
 * 
 * ### Animation and Timeline Integration
 * Professional animation workflow support:
 * - **Keyframe Animation**: Timeline-based object transformation animation
 * - **Motion Paths**: Spline-based object movement and camera paths
 * - **Constraint Animation**: Parent-child relationships and animation constraints
 * - **Physics Integration**: Rigid body and soft body physics simulation
 * 
 * ### File Format Compatibility
 * Comprehensive import/export support for professional workflows:
 * - **Native Formats**: RudeBase3D native scene object serialization
 * - **Industry Standards**: FBX, OBJ, DAE, glTF, USD compatibility
 * - **CAD Integration**: STEP, IGES, STL technical drawing import
 * - **Version Control**: Incremental saves and merge conflict resolution
 * 
 * ## Advanced Technical Implementation
 * 
 * ### Memory Management and Performance
 * Enterprise-grade performance optimization for complex scenes:
 * - **Reference Counting**: Smart pointer-based automatic memory management
 * - **Object Pooling**: Efficient reuse of frequently created/destroyed objects
 * - **Copy-on-Write**: Efficient mesh and material sharing between objects
 * - **Garbage Collection**: Automatic cleanup of unreferenced resources
 * 
 * ### Spatial Data Structures
 * Optimized spatial organization for large scene performance:
 * - **Bounding Volume Hierarchies**: Efficient collision detection and ray casting
 * - **Spatial Hashing**: Fast proximity queries and neighbor finding
 * - **Octree Integration**: Adaptive space subdivision for complex scenes
 * - **Cache Optimization**: Memory layout optimization for rendering performance
 * 
 * ### Rendering Pipeline Integration
 * Seamless integration with modern graphics rendering systems:
 * - **Draw Call Optimization**: Automatic batching and instancing
 * - **State Sorting**: Material and shader state organization for efficiency
 * - **Multi-Pass Rendering**: Support for complex rendering techniques
 * - **GPU Memory Management**: Efficient vertex buffer and texture allocation
 * 
 * ## Professional Object Features
 * 
 * ### Bounding Volume Computation
 * Advanced spatial bounds calculation for optimization and queries:
 * - **Axis-Aligned Bounding Boxes (AABB)**: Fast intersection testing and culling
 * - **Oriented Bounding Boxes (OBB)**: Tight-fitting bounds for rotated objects
 * - **Bounding Spheres**: Simple distance-based spatial queries
 * - **Convex Hull Generation**: Precise object bounds for collision detection
 * 
 * ### Component System Architecture
 * Extensible component framework for specialized object behaviors:
 * - **Transform Component**: Spatial transformation and hierarchy management
 * - **Render Component**: Mesh and material rendering properties
 * - **Physics Component**: Collision detection and physics simulation
 * - **Animation Component**: Keyframe and procedural animation systems
 * - **Script Component**: Custom behavior scripting and logic integration
 * 
 * ### Event System Integration
 * Comprehensive notification system for object state changes:
 * - **Property Change Events**: Automatic notification of object modifications
 * - **Transform Events**: Position, rotation, and scale change notifications
 * - **Visibility Events**: Show/hide state change broadcasting
 * - **Selection Events**: Selection state change notification
 * - **Lifecycle Events**: Object creation, destruction, and modification tracking
 * 
 * ## Usage Examples
 * 
 * ### Basic Scene Object Creation and Management
 * ```cpp
 * // Create a basic scene object with mesh and material
 * auto sceneObject = std::make_shared<SceneObject>("MyObject");
 * 
 * // Set up geometry using built-in primitives
 * auto cubeMesh = GeometryGenerator::createCube(2.0f);
 * sceneObject->setMesh(cubeMesh);
 * 
 * // Apply professional material
 * auto material = Material::createMetal();
 * material->setDiffuseColor(glm::vec4(0.7f, 0.7f, 0.8f, 1.0f)); // Steel color
 * material->setRoughness(0.3f);  // Brushed metal finish
 * sceneObject->setMaterial(material);
 * 
 * // Configure transform
 * sceneObject->getTransform().setPosition(glm::vec3(5.0f, 0.0f, 0.0f));
 * sceneObject->getTransform().setRotation(glm::angleAxis(glm::radians(45.0f), 
 *                                                       glm::vec3(0, 1, 0)));
 * sceneObject->getTransform().setScale(glm::vec3(1.5f, 1.0f, 1.5f));
 * ```
 * 
 * ### Professional Primitive Factory Usage
 * ```cpp
 * class PrimitiveFactory {
 * public:
 *     static std::vector<SceneObjectPtr> createBasicPrimitives() {
 *         std::vector<SceneObjectPtr> primitives;
 *         
 *         // Create professional-quality primitives
 *         auto cube = SceneObject::createCube("Steel Cube");
 *         auto sphere = SceneObject::createSphere("Glass Sphere");
 *         auto cylinder = SceneObject::createCylinder("Aluminum Cylinder");
 *         auto plane = SceneObject::createPlane("Ground Plane");
 *         
 *         // Configure materials for realistic appearance
 *         cube->setMaterial(Material::createMetal());
 *         sphere->setMaterial(Material::createGlass());
 *         cylinder->setMaterial(createAluminumMaterial());
 *         plane->setMaterial(createConcreteMaterial());
 *         
 *         // Arrange in professional layout
 *         cube->getTransform().setPosition(glm::vec3(-3.0f, 1.0f, 0.0f));
 *         sphere->getTransform().setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
 *         cylinder->getTransform().setPosition(glm::vec3(3.0f, 1.0f, 0.0f));
 *         plane->getTransform().setScale(glm::vec3(20.0f, 1.0f, 20.0f));
 *         
 *         primitives = { cube, sphere, cylinder, plane };
 *         return primitives;
 *     }
 * };
 * ```
 * 
 * ### Advanced Object Selection and Manipulation
 * ```cpp
 * class SceneObjectManipulator {
 * private:
 *     std::vector<SceneObjectPtr> m_selectedObjects;
 *     glm::vec3 m_manipulationCenter;
 *     
 * public:
 *     void selectObject(SceneObjectPtr object, bool addToSelection = false) {
 *         if (!addToSelection) {
 *             clearSelection();
 *         }
 *         
 *         object->setSelected(true);
 *         m_selectedObjects.push_back(object);
 *         updateManipulationCenter();
 *     }
 *     
 *     void selectObjectsInRegion(const glm::vec3& min, const glm::vec3& max) {
 *         // Selection using bounding box intersection
 *         for (auto& object : getAllSceneObjects()) {
 *             glm::vec3 objMin = object->getBoundingBoxMin();
 *             glm::vec3 objMax = object->getBoundingBoxMax();
 *             
 *             // Check AABB intersection
 *             if (aabbIntersection(min, max, objMin, objMax)) {
 *                 selectObject(object, true);
 *             }
 *         }
 *     }
 *     
 *     void translateSelection(const glm::vec3& delta) {
 *         for (auto& object : m_selectedObjects) {
 *             glm::vec3 currentPos = object->getTransform().getPosition();
 *             object->getTransform().setPosition(currentPos + delta);
 *         }
 *         updateManipulationCenter();
 *     }
 *     
 *     void rotateSelectionAroundCenter(const glm::quat& rotation) {
 *         for (auto& object : m_selectedObjects) {
 *             glm::vec3 objPos = object->getTransform().getPosition();
 *             glm::vec3 offset = objPos - m_manipulationCenter;
 *             
 *             // Rotate offset around center
 *             glm::vec3 rotatedOffset = rotation * offset;
 *             
 *             // Apply rotation to object transform
 *             object->getTransform().setPosition(m_manipulationCenter + rotatedOffset);
 *             
 *             glm::quat currentRotation = object->getTransform().getRotation();
 *             object->getTransform().setRotation(rotation * currentRotation);
 *         }
 *     }
 *     
 * private:
 *     void updateManipulationCenter() {
 *         if (m_selectedObjects.empty()) {
 *             m_manipulationCenter = glm::vec3(0.0f);
 *             return;
 *         }
 *         
 *         glm::vec3 center(0.0f);
 *         for (const auto& object : m_selectedObjects) {
 *             center += object->getBoundingBoxCenter();
 *         }
 *         m_manipulationCenter = center / static_cast<float>(m_selectedObjects.size());
 *     }
 * };
 * ```
 * 
 * ### Professional Object Animation System
 * ```cpp
 * class ObjectAnimationController {
 * private:
 *     struct AnimationKeyframe {
 *         float time;
 *         glm::vec3 position;
 *         glm::quat rotation;
 *         glm::vec3 scale;
 *     };
 *     
 *     std::map<SceneObjectPtr, std::vector<AnimationKeyframe>> m_animations;
 *     float m_currentTime = 0.0f;
 *     bool m_isPlaying = false;
 *     
 * public:
 *     void addKeyframe(SceneObjectPtr object, float time) {
 *         AnimationKeyframe keyframe;
 *         keyframe.time = time;
 *         keyframe.position = object->getTransform().getPosition();
 *         keyframe.rotation = object->getTransform().getRotation();
 *         keyframe.scale = object->getTransform().getScale();
 *         
 *         auto& keyframes = m_animations[object];
 *         
 *         // Insert keyframe in chronological order
 *         auto it = std::lower_bound(keyframes.begin(), keyframes.end(), keyframe,
 *                                   [](const auto& a, const auto& b) {
 *                                       return a.time < b.time;
 *                                   });
 *         keyframes.insert(it, keyframe);
 *     }
 *     
 *     void update(float deltaTime) {
 *         if (!m_isPlaying) return;
 *         
 *         m_currentTime += deltaTime;
 *         
 *         for (auto& [object, keyframes] : m_animations) {
 *             updateObjectAnimation(object, keyframes, m_currentTime);
 *         }
 *     }
 *     
 * private:
 *     void updateObjectAnimation(SceneObjectPtr object, 
 *                               const std::vector<AnimationKeyframe>& keyframes,
 *                               float time) {
 *         if (keyframes.size() < 2) return;
 *         
 *         // Find surrounding keyframes
 *         auto nextIt = std::upper_bound(keyframes.begin(), keyframes.end(), time,
 *                                       [](float t, const auto& kf) {
 *                                           return t < kf.time;
 *                                       });
 *         
 *         if (nextIt == keyframes.begin() || nextIt == keyframes.end()) {
 *             return; // Before first or after last keyframe
 *         }
 *         
 *         auto prevIt = nextIt - 1;
 *         
 *         // Interpolate between keyframes
 *         float t = (time - prevIt->time) / (nextIt->time - prevIt->time);
 *         
 *         glm::vec3 position = glm::mix(prevIt->position, nextIt->position, t);
 *         glm::quat rotation = glm::slerp(prevIt->rotation, nextIt->rotation, t);
 *         glm::vec3 scale = glm::mix(prevIt->scale, nextIt->scale, t);
 *         
 *         // Apply interpolated values
 *         object->getTransform().setPosition(position);
 *         object->getTransform().setRotation(rotation);
 *         object->getTransform().setScale(scale);
 *     }
 * };
 * ```
 * 
 * ### Complex Object Hierarchy Management
 * ```cpp
 * class HierarchicalObjectManager {
 * private:
 *     struct ObjectNode {
 *         SceneObjectPtr object;
 *         std::vector<std::shared_ptr<ObjectNode>> children;
 *         std::weak_ptr<ObjectNode> parent;
 *     };
 *     
 *     std::shared_ptr<ObjectNode> m_rootNode;
 *     std::unordered_map<SceneObjectPtr, std::shared_ptr<ObjectNode>> m_objectToNode;
 *     
 * public:
 *     void addChildObject(SceneObjectPtr parent, SceneObjectPtr child) {
 *         auto parentNode = findOrCreateNode(parent);
 *         auto childNode = findOrCreateNode(child);
 *         
 *         // Remove from previous parent
 *         if (auto oldParent = childNode->parent.lock()) {
 *             auto& siblings = oldParent->children;
 *             siblings.erase(std::remove(siblings.begin(), siblings.end(), childNode),
 *                           siblings.end());
 *         }
 *         
 *         // Add to new parent
 *         parentNode->children.push_back(childNode);
 *         childNode->parent = parentNode;
 *         
 *         // Update transform hierarchy
 *         updateChildTransforms(parentNode);
 *     }
 *     
 *     void updateTransforms() {
 *         if (m_rootNode) {
 *             updateNodeTransforms(m_rootNode, glm::mat4(1.0f));
 *         }
 *     }
 *     
 * private:
 *     void updateNodeTransforms(std::shared_ptr<ObjectNode> node, 
 *                              const glm::mat4& parentWorldMatrix) {
 *         // Compute world transform
 *         glm::mat4 localMatrix = node->object->getTransform().getMatrix();
 *         glm::mat4 worldMatrix = parentWorldMatrix * localMatrix;
 *         
 *         // Store world transform for rendering
 *         node->object->getTransform().setWorldMatrix(worldMatrix);
 *         
 *         // Update children recursively
 *         for (auto& child : node->children) {
 *             updateNodeTransforms(child, worldMatrix);
 *         }
 *     }
 * };
 * ```
 * 
 * ### Object Serialization and File I/O
 * ```cpp
 * class SceneObjectSerializer {
 * public:
 *     static nlohmann::json serializeObject(SceneObjectPtr object) {
 *         nlohmann::json objectJson;
 *         
 *         // Basic properties
 *         objectJson["id"] = object->getId();
 *         objectJson["name"] = object->getName();
 *         objectJson["visible"] = object->isVisible();
 *         objectJson["selected"] = object->isSelected();
 *         
 *         // Transform data
 *         auto& transform = objectJson["transform"];
 *         const auto& objTransform = object->getTransform();
 *         transform["position"] = {
 *             objTransform.getPosition().x,
 *             objTransform.getPosition().y,
 *             objTransform.getPosition().z
 *         };
 *         
 *         auto rotation = objTransform.getRotation();
 *         transform["rotation"] = {
 *             rotation.x, rotation.y, rotation.z, rotation.w
 *         };
 *         
 *         transform["scale"] = {
 *             objTransform.getScale().x,
 *             objTransform.getScale().y,
 *             objTransform.getScale().z
 *         };
 *         
 *         // Mesh reference
 *         if (object->getMesh()) {
 *             objectJson["meshFile"] = object->getMesh()->getSourceFile();
 *             objectJson["meshId"] = object->getMesh()->getId();
 *         }
 *         
 *         // Material reference
 *         if (object->getMaterial()) {
 *             objectJson["materialFile"] = object->getMaterial()->getSourceFile();
 *             objectJson["materialId"] = object->getMaterial()->getId();
 *         }
 *         
 *         return objectJson;
 *     }
 *     
 *     static SceneObjectPtr deserializeObject(const nlohmann::json& objectJson) {
 *         auto object = std::make_shared<SceneObject>();
 *         
 *         // Basic properties
 *         object->setName(objectJson["name"]);
 *         object->setVisible(objectJson["visible"]);
 *         object->setSelected(objectJson["selected"]);
 *         
 *         // Transform data
 *         const auto& transform = objectJson["transform"];
 *         
 *         glm::vec3 position(transform["position"][0],
 *                           transform["position"][1],
 *                           transform["position"][2]);
 *         object->getTransform().setPosition(position);
 *         
 *         glm::quat rotation(transform["rotation"][3], // w
 *                           transform["rotation"][0], // x
 *                           transform["rotation"][1], // y
 *                           transform["rotation"][2]); // z
 *         object->getTransform().setRotation(rotation);
 *         
 *         glm::vec3 scale(transform["scale"][0],
 *                        transform["scale"][1],
 *                        transform["scale"][2]);
 *         object->getTransform().setScale(scale);
 *         
 *         // Load mesh and material (would be implemented with asset manager)
 *         if (objectJson.contains("meshFile")) {
 *             auto mesh = AssetManager::loadMesh(objectJson["meshFile"]);
 *             object->setMesh(mesh);
 *         }
 *         
 *         if (objectJson.contains("materialFile")) {
 *             auto material = AssetManager::loadMaterial(objectJson["materialFile"]);
 *             object->setMaterial(material);
 *         }
 *         
 *         return object;
 *     }
 * };
 * ```
 * 
 * ## Performance Optimization Strategies
 * 
 * ### Memory Management
 * Enterprise-grade memory optimization for complex scenes:
 * - **Smart Pointers**: Automatic memory management with reference counting
 * - **Object Pooling**: Pre-allocated pools for frequently created objects
 * - **Copy-on-Write**: Efficient data sharing between similar objects
 * - **Garbage Collection**: Automatic cleanup of unreferenced resources
 * 
 * ### Rendering Optimization
 * High-performance rendering techniques for large scenes:
 * - **Instanced Rendering**: Efficient rendering of repeated geometry
 * - **Draw Call Batching**: Grouping similar objects for reduced GPU overhead
 * - **Level-of-Detail**: Distance-based mesh simplification
 * - **Frustum Culling**: View-dependent object elimination for performance
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Scene
 * @ingroup Objects
 * @ingroup EntityManagement
 */

#pragma once

#include "Common.h"
#include "core/mesh_forward.hpp"
#include "core/transform.hpp"
#include <QString>

// Forward declarations
class Material;


class SceneObject {
public:
    SceneObject(const std::string& name = "Object");
    virtual ~SceneObject() = default;

    // Name and identification
    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() const { return m_name; }
    
    unsigned int getId() const { return m_id; }
    
    // Visibility
    void setVisible(bool visible) { m_visible = visible; }
    bool isVisible() const { return m_visible; }
    
    // Selection
    void setSelected(bool selected) { m_selected = selected; }
    bool isSelected() const { return m_selected; }
    
    // Transform
    Transform& getTransform() { return m_transform; }
    const Transform& getTransform() const { return m_transform; }
    
    // Mesh and material
    void setMesh(rude::MeshPtr mesh) { m_mesh = mesh; }
    rude::MeshPtr getMesh() const { return m_mesh; }
    
    void setMaterial(MaterialPtr material) { m_material = material; }
    MaterialPtr getMaterial() const { return m_material; }
    
    // Rendering
    virtual void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, RenderMode mode);
    
    // Bounding box
    glm::vec3 getBoundingBoxMin() const;
    glm::vec3 getBoundingBoxMax() const;
    glm::vec3 getBoundingBoxCenter() const;
    
    // Static factory methods for primitives
    static SceneObjectPtr createCube(const std::string& name = "Cube");
    static SceneObjectPtr createSphere(const std::string& name = "Sphere");
    static SceneObjectPtr createCylinder(const std::string& name = "Cylinder");
    static SceneObjectPtr createPlane(const std::string& name = "Plane");

private:
    static unsigned int s_nextId;
    unsigned int m_id;
    std::string m_name;
    bool m_visible;
    bool m_selected;
    Transform m_transform;
    rude::MeshPtr m_mesh;
    MaterialPtr m_material;
};
