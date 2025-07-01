#pragma once

#include "Common.h"
#include "core/math/Transform.h"
#include <QString>

// Forward declarations
class Mesh;
class Material;

class SceneObject {
public:
    SceneObject(const QString& name = "Object");
    virtual ~SceneObject() = default;

    // Name and identification
    void setName(const QString& name) { m_name = name; }
    const QString& getName() const { return m_name; }
    
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
    void setMesh(MeshPtr mesh) { m_mesh = mesh; }
    MeshPtr getMesh() const { return m_mesh; }
    
    void setMaterial(MaterialPtr material) { m_material = material; }
    MaterialPtr getMaterial() const { return m_material; }
    
    // Rendering
    virtual void render(const QMatrix4x4& viewMatrix, const QMatrix4x4& projectionMatrix, RenderMode mode);
    
    // Bounding box
    QVector3D getBoundingBoxMin() const;
    QVector3D getBoundingBoxMax() const;
    QVector3D getBoundingBoxCenter() const;
    
    // Static factory methods for primitives
    static SceneObjectPtr createCube(const QString& name = "Cube");
    static SceneObjectPtr createSphere(const QString& name = "Sphere");
    static SceneObjectPtr createCylinder(const QString& name = "Cylinder");
    static SceneObjectPtr createPlane(const QString& name = "Plane");

private:
    static unsigned int s_nextId;
    
    unsigned int m_id;
    QString m_name;
    bool m_visible;
    bool m_selected;
    
    Transform m_transform;
    MeshPtr m_mesh;
    MaterialPtr m_material;
};
