#pragma once

#include "Common.h"
#include "geometry/core/Vertex.h"
#include "core/mesh.hpp"
#include "core/mesh_forward.hpp"
#include <vector>
#include <memory>


class MeshGenerator {
public:
    // Primitive generation
    static rude::MeshPtr generateCube(float size = 1.0f);
    static rude::MeshPtr generateSphere(float radius = 1.0f, int segments = 32, int rings = 16);
    static rude::MeshPtr generateCylinder(float radius = 1.0f, float height = 2.0f, int segments = 32);
    static rude::MeshPtr generatePlane(float width = 2.0f, float height = 2.0f, int widthSegments = 1, int heightSegments = 1);

    // Advanced primitives
    static rude::MeshPtr generateCone(float radius = 1.0f, float height = 2.0f, int segments = 32);
    static rude::MeshPtr generateTorus(float majorRadius = 1.0f, float minorRadius = 0.3f, int majorSegments = 32, int minorSegments = 16);
    static rude::MeshPtr generateIcosphere(float radius = 1.0f, int subdivisions = 2);

    // Grid and debug meshes
    static rude::MeshPtr generateGrid(float size = 10.0f, int divisions = 10);
    static rude::MeshPtr generateWireCube(float size = 1.0f);
    static rude::MeshPtr generateWireSphere(float radius = 1.0f, int segments = 16);

private:
    // Helper functions
    static void addQuad(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                       const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
                       const glm::vec3& normal);

    static void addTriangle(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                           const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                           const glm::vec3& normal);

    static glm::vec3 calculateNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
    static glm::vec2 sphericalToUV(const glm::vec3& position);
};
