#pragma once

#include "Common.h"
#include "core/mesh_forward.hpp"
#include "core/math/Transform.h"
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
