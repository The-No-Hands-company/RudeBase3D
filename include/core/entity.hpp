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

#include "core/mesh_forward.hpp"

namespace rude {
    enum class PrimitiveType { Unknown, None, Cube, Sphere, Plane, Cylinder, Cone, Torus, Icosphere };
    class Mesh;
}

class Entity {
public:
    Entity(int id, rude::PrimitiveType type, const std::string& name = "Entity");
    int getId() const;
    const std::string& getName() const;
    Transform& getTransform();
    rude::PrimitiveType getPrimitiveType() const;
    rude::MeshPtr getMesh() const;
    std::pair<glm::vec3, glm::vec3> getAABB() const; // Returns (min, max)

    // Modern API: getType (string representation of primitive type or custom type)
    std::string getType() const;

    // Transform convenience methods
    glm::vec3 getPosition() const { 
        return transform.getPosition();
    }
    void setPosition(const glm::vec3& pos) { 
        transform.setPosition(pos);
    }
    glm::quat getRotation() const { 
        // TODO: Refactor rotation to glm::quat in Transform
        return glm::quat(1, 0, 0, 0); // Placeholder
    }
    void setRotation(const glm::quat& rot) { 
        // TODO: Refactor rotation to glm::quat in Transform
    }
    glm::vec3 getWorldPosition() const {
        return getPosition(); // For now, just return local position; update if world space is different
    }
    glm::vec3 getScale() const { 
        return transform.getScale();
    }
    void setScale(const glm::vec3& scale) { 
        transform.setScale(scale);
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
    void setMesh(const rude::MeshPtr& newMesh) { mesh = newMesh; }

    rude::MeshPtr mesh; // Make mesh public for now

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