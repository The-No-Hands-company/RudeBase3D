#pragma once
#include <glm/glm.hpp>
#include <string>

class Material {
public:
    Material();
    Material(const glm::vec3& diffuse, const glm::vec3& specular, float shininess);

    // Material properties
    glm::vec3 diffuse{0.8f, 0.8f, 0.8f};  // Default gray
    glm::vec3 specular{1.0f, 1.0f, 1.0f}; // Default white
    float shininess{32.0f};               // Default shininess
    float opacity{1.0f};                  // Default fully opaque

    // Texture support (to be implemented)
    std::string diffuseTexture;
    std::string normalTexture;
    std::string specularTexture;
}; 