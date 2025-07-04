#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QMatrix4x4>
#include "core/math/Transform.h"
#include "core/qt_glm_utils.hpp"
// Forward declarations

namespace rude {
    enum class PrimitiveType { None, Cube, Sphere, Plane, Cylinder, Cone, Torus, Icosphere };
}

class Mesh; // Forward declaration

class Entity {
public:
    Entity(int id, rude::PrimitiveType type, const std::string& name = "Entity");
    int getId() const;
    const std::string& getName() const;
    Transform& getTransform();
    rude::PrimitiveType getPrimitiveType() const;
    std::shared_ptr<Mesh> getMesh() const;
    std::pair<glm::vec3, glm::vec3> getAABB() const; // Returns (min, max)

    // Transform convenience methods
    glm::vec3 getPosition() const { 
        QVector3D pos = transform.getPosition();
        return glm::vec3(pos.x(), pos.y(), pos.z());
    }
    void setPosition(const glm::vec3& pos) { 
        transform.setPosition(QVector3D(pos.x, pos.y, pos.z)); 
    }
    glm::quat getRotation() const { 
        QQuaternion rot = transform.getRotation();
        return glm::quat(rot.scalar(), rot.x(), rot.y(), rot.z());
    }
    void setRotation(const glm::quat& rot) { 
        transform.setRotation(QQuaternion(rot.w, rot.x, rot.y, rot.z)); 
    }
    glm::vec3 getWorldPosition() const {
        return getPosition(); // For now, just return local position; update if world space is different
    }
    glm::vec3 getScale() const { 
        QVector3D scale = transform.getScale();
        return glm::vec3(scale.x(), scale.y(), scale.z());
    }
    void setScale(const glm::vec3& scale) { 
        transform.setScale(QVector3D(scale.x, scale.y, scale.z)); 
    }

    // Drawing method
    void draw(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform = glm::mat4(1.0f)) const;
    glm::mat4 getWorldTransform() const;

    // Hierarchy
    Entity* getParent() const { return parent; }
    const std::vector<Entity*>& getChildren() const { return children; }
    void addChild(Entity* child);
    void removeChild(Entity* child);
    void setParent(Entity* newParent);

    // Mesh assignment utility
    void setMesh(const std::shared_ptr<Mesh>& newMesh) { mesh = newMesh; }

    std::shared_ptr<Mesh> mesh; // Make mesh public for now

private:
    int id;
    Transform transform;
    rude::PrimitiveType primitiveType;
    glm::vec3 aabb_min{0.0f}, aabb_max{0.0f}; // World-space AABB
    Entity* parent = nullptr;
    std::vector<Entity*> children;
    
public:
    std::string name;
};