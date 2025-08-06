// Modernized MeshGenerator implementation
#include "MeshGenerator.h"
#include "core/mesh.hpp"
#include "core/mesh_forward.hpp"
#include "geometry/core/Vertex.h"
#include <cmath>
#include <map>
#include <array>
#include <algorithm>



// Use rude::MeshPtr for all mesh pointers
rude::MeshPtr MeshGenerator::generateCube(float size)
{
    auto mesh = std::make_shared<rude::Mesh>();
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    
    float halfSize = size * 0.5f;
    
    // Define cube vertices (8 corners)
    glm::vec3 cubePositions[8] = {
        glm::vec3(-halfSize, -halfSize, -halfSize), // 0: left-bottom-back
        glm::vec3( halfSize, -halfSize, -halfSize), // 1: right-bottom-back
        glm::vec3( halfSize,  halfSize, -halfSize), // 2: right-top-back
        glm::vec3(-halfSize,  halfSize, -halfSize), // 3: left-top-back
        glm::vec3(-halfSize, -halfSize,  halfSize), // 4: left-bottom-front
        glm::vec3( halfSize, -halfSize,  halfSize), // 5: right-bottom-front
        glm::vec3( halfSize,  halfSize,  halfSize), // 6: right-top-front
        glm::vec3(-halfSize,  halfSize,  halfSize)  // 7: left-top-front
    };
    
    // Define face normals
    glm::vec3 faceNormals[6] = {
        glm::vec3( 0,  0, -1), // Back
        glm::vec3( 0,  0,  1), // Front
        glm::vec3(-1,  0,  0), // Left
        glm::vec3( 1,  0,  0), // Right
        glm::vec3( 0, -1,  0), // Bottom
        glm::vec3( 0,  1,  0)  // Top
    };
    
    // Define UV coordinates
    glm::vec2 faceUVs[4] = {
        glm::vec2(0, 0), // bottom-left
        glm::vec2(1, 0), // bottom-right
        glm::vec2(1, 1), // top-right
        glm::vec2(0, 1)  // top-left
    };
    
    // Define faces (vertex indices for each face)
    int faceIndices[6][4] = {
        {1, 0, 3, 2}, // Back face
        {4, 5, 6, 7}, // Front face
        {0, 4, 7, 3}, // Left face
        {5, 1, 2, 6}, // Right face
        {0, 1, 5, 4}, // Bottom face
        {3, 7, 6, 2}  // Top face
    };
    
    // Generate vertices for each face
    for (int face = 0; face < 6; ++face) {
        for (int vert = 0; vert < 4; ++vert) {
            positions.push_back(cubePositions[faceIndices[face][vert]]);
            normals.push_back(faceNormals[face]);
            texCoords.push_back(faceUVs[vert]);
        }
        
        // Generate indices for the face (two triangles)
        int baseIndex = face * 4;
        indices.push_back(baseIndex + 0);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);
        
        indices.push_back(baseIndex + 0);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 3);
    }
    
    // rude::Mesh::setData expects (const std::vector<rude::Vertex>&, const std::vector<unsigned int>&)
    std::vector<rude::Vertex> verts;
    for (size_t i = 0; i < positions.size(); ++i) {
        rude::Vertex v;
        // Convert glm::vec3 to glm::vec3 (no conversion needed if already glm)
        v.position = positions[i];
        v.normal = normals[i];
        v.texCoord = texCoords[i];
        verts.push_back(v);
    }
    mesh->setData(verts, indices);
    
    return mesh;
}

// ...existing code...
rude::MeshPtr MeshGenerator::generateSphere(float radius, int segments, int rings)
{
    auto mesh = std::make_shared<rude::Mesh>();
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    
    // Generate vertices
    for (int ring = 0; ring <= rings; ++ring) {
        float theta = static_cast<float>(ring) * PI / static_cast<float>(rings);
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        
        for (int segment = 0; segment <= segments; ++segment) {
            float phi = static_cast<float>(segment) * 2.0f * PI / static_cast<float>(segments);
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            glm::vec3 position = glm::vec3(
                radius * sinTheta * cosPhi,
                radius * cosTheta,
                radius * sinTheta * sinPhi
            );
            
            positions.push_back(position);
            normals.push_back(glm::normalize(position));
            texCoords.push_back(glm::vec2(
                static_cast<float>(segment) / static_cast<float>(segments),
                static_cast<float>(ring) / static_cast<float>(rings)
            ));
        }
    }
    
    // Generate indices
    for (int ring = 0; ring < rings; ++ring) {
        for (int segment = 0; segment < segments; ++segment) {
            int current = ring * (segments + 1) + segment;
            int next = current + segments + 1;
            
            // First triangle
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);
            
            // Second triangle
            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }
    
    std::vector<rude::Vertex> verts;
    for (size_t i = 0; i < positions.size(); ++i) {
        rude::Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texCoord = texCoords[i];
        verts.push_back(v);
    }
    mesh->setData(verts, indices);
    
    return mesh;
}

// ...existing code...
rude::MeshPtr MeshGenerator::generateCylinder(float radius, float height, int segments)
{
    auto mesh = std::make_shared<rude::Mesh>();
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    
    float halfHeight = height * 0.5f;
    
    // Generate side vertices
    for (int segment = 0; segment <= segments; ++segment) {
        float angle = static_cast<float>(segment) * 2.0f * PI / static_cast<float>(segments);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        
        glm::vec3 normal(cosAngle, 0.0f, sinAngle);
        
        // Bottom vertex
        positions.push_back(glm::vec3(radius * cosAngle, -halfHeight, radius * sinAngle));
        normals.push_back(normal);
        texCoords.push_back(glm::vec2(static_cast<float>(segment) / static_cast<float>(segments), 0.0f));
        
        // Top vertex
        positions.push_back(glm::vec3(radius * cosAngle, halfHeight, radius * sinAngle));
        normals.push_back(normal);
        texCoords.push_back(glm::vec2(static_cast<float>(segment) / static_cast<float>(segments), 1.0f));
    }
    
    // Generate side indices
    for (int segment = 0; segment < segments; ++segment) {
        int current = segment * 2;
        int next = (segment + 1) * 2;
        
        // First triangle
        indices.push_back(current);
        indices.push_back(next);
        indices.push_back(current + 1);
        
        // Second triangle
        indices.push_back(current + 1);
        indices.push_back(next);
        indices.push_back(next + 1);
    }
    
    // Generate bottom cap
    int bottomCenterIndex = static_cast<int>(positions.size());
    positions.push_back(glm::vec3(0.0f, -halfHeight, 0.0f));
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    texCoords.push_back(glm::vec2(0.5f, 0.5f));
    
    for (int segment = 0; segment < segments; ++segment) {
        float angle = static_cast<float>(segment) * 2.0f * PI / static_cast<float>(segments);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        
        positions.push_back(glm::vec3(radius * cosAngle, -halfHeight, radius * sinAngle));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        texCoords.push_back(glm::vec2(0.5f + 0.5f * cosAngle, 0.5f + 0.5f * sinAngle));
    }
    
    for (int segment = 0; segment < segments; ++segment) {
        int current = bottomCenterIndex + 1 + segment;
        int next = bottomCenterIndex + 1 + ((segment + 1) % segments);
        
        indices.push_back(bottomCenterIndex);
        indices.push_back(current);
        indices.push_back(next);
    }
    
    // Generate top cap
    int topCenterIndex = static_cast<int>(positions.size());
    positions.push_back(glm::vec3(0.0f, halfHeight, 0.0f));
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texCoords.push_back(glm::vec2(0.5f, 0.5f));
    
    for (int segment = 0; segment < segments; ++segment) {
        float angle = static_cast<float>(segment) * 2.0f * PI / static_cast<float>(segments);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        
        positions.push_back(glm::vec3(radius * cosAngle, halfHeight, radius * sinAngle));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        texCoords.push_back(glm::vec2(0.5f + 0.5f * cosAngle, 0.5f - 0.5f * sinAngle));
    }
    
    for (int segment = 0; segment < segments; ++segment) {
        int current = topCenterIndex + 1 + segment;
        int next = topCenterIndex + 1 + ((segment + 1) % segments);
        
        indices.push_back(topCenterIndex);
        indices.push_back(next);
        indices.push_back(current);
    }
    
    std::vector<rude::Vertex> verts;
    for (size_t i = 0; i < positions.size(); ++i) {
        rude::Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texCoord = texCoords[i];
        verts.push_back(v);
    }
    mesh->setData(verts, indices);
    
    return mesh;
}

// ...existing code...
rude::MeshPtr MeshGenerator::generatePlane(float width, float height, int widthSegments, int heightSegments)
{
    auto mesh = std::make_shared<rude::Mesh>();
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    
    // Generate vertices
    for (int y = 0; y <= heightSegments; ++y) {
        for (int x = 0; x <= widthSegments; ++x) {
            glm::vec3 position(
                -halfWidth + (static_cast<float>(x) / static_cast<float>(widthSegments)) * width,
                0.0f,
                -halfHeight + (static_cast<float>(y) / static_cast<float>(heightSegments)) * height
            );
            glm::vec3 normal(0.0f, 1.0f, 0.0f);
            glm::vec2 texCoord(
                static_cast<float>(x) / static_cast<float>(widthSegments),
                static_cast<float>(y) / static_cast<float>(heightSegments)
            );
            
            positions.push_back(position);
            normals.push_back(normal);
            texCoords.push_back(texCoord);
        }
    }
    
    // Generate indices
    for (int y = 0; y < heightSegments; ++y) {
        for (int x = 0; x < widthSegments; ++x) {
            int current = y * (widthSegments + 1) + x;
            int next = current + widthSegments + 1;
            
            // First triangle
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);
            
            // Second triangle
            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }
    
    std::vector<rude::Vertex> verts;
    for (size_t i = 0; i < positions.size(); ++i) {
        rude::Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texCoord = texCoords[i];
        verts.push_back(v);
    }
    mesh->setData(verts, indices);
    
    return mesh;
}

// ...existing code...
rude::MeshPtr MeshGenerator::generateGrid(float size, int divisions)
{
    auto mesh = std::make_shared<rude::Mesh>();
    
    std::vector<rude::VertexPtr> vertices;
    
    float halfSize = size * 0.5f;
    float step = size / static_cast<float>(divisions);
    
    // Generate grid lines parallel to X-axis
    for (int i = 0; i <= divisions; ++i) {
        float z = -halfSize + i * step;
        
        vertices.push_back(std::make_shared<rude::Vertex>(glm::vec3(-halfSize, 0.0f, z)));
        vertices.push_back(std::make_shared<rude::Vertex>(glm::vec3(halfSize, 0.0f, z)));
    }
    
    // Generate grid lines parallel to Z-axis
    for (int i = 0; i <= divisions; ++i) {
        float x = -halfSize + i * step;
        
        vertices.push_back(std::make_shared<rude::Vertex>(glm::vec3(x, 0.0f, -halfSize)));
        vertices.push_back(std::make_shared<rude::Vertex>(glm::vec3(x, 0.0f, halfSize)));
    }
    
    // No setVertices in rude::Mesh, convert to std::vector<rude::Vertex> and setData
    std::vector<rude::Vertex> verts;
    for (const auto& vptr : vertices) {
        if (vptr) verts.push_back(*vptr);
    }
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < verts.size(); ++i) indices.push_back(i);
    mesh->setData(verts, indices);
    
    return mesh;
}

// ...existing code...
rude::MeshPtr MeshGenerator::generateCone(float radius, float height, int segments)
{
    auto mesh = std::make_shared<rude::Mesh>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Apex vertex at top
    Vertex apex;
    apex.position = glm::vec3(0, height * 0.5f, 0);
    apex.normal = glm::vec3(0, 1, 0);
    apex.texCoord = glm::vec2(0.5f, 0.5f);
    vertices.push_back(apex);
    
    // Center vertex at bottom
    Vertex center;
    center.position = glm::vec3(0, -height * 0.5f, 0);
    center.normal = glm::vec3(0, -1, 0);
    center.texCoord = glm::vec2(0.5f, 0.5f);
    vertices.push_back(center);
    
    // Generate base circle vertices
    for (int i = 0; i <= segments; ++i) {
        float angle = static_cast<float>(i) * 2.0f * PI / static_cast<float>(segments);
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        
        // Vertex for side faces
        Vertex sideVertex;
        sideVertex.position = glm::vec3(x, -height * 0.5f, z);
        // Calculate side normal (pointing outward and upward)
        glm::vec3 toApex = glm::normalize(apex.position - sideVertex.position);
        glm::vec3 radial = glm::normalize(glm::vec3(x, 0, z));
        sideVertex.normal = glm::normalize(glm::cross(glm::cross(toApex, radial), toApex));
        sideVertex.texCoord = glm::vec2(static_cast<float>(i) / segments, 0);
        vertices.push_back(sideVertex);
        
        // Vertex for bottom face
        Vertex bottomVertex;
        bottomVertex.position = glm::vec3(x, -height * 0.5f, z);
        bottomVertex.normal = glm::vec3(0, -1, 0);
        bottomVertex.texCoord = glm::vec2(0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle));
        vertices.push_back(bottomVertex);
    }
    
    // Generate side triangles (apex to base)
    for (int i = 0; i < segments; ++i) {
        int current = 2 + i * 2;     // Current side vertex
        int next = 2 + ((i + 1) % segments) * 2; // Next side vertex
        
        indices.push_back(0);        // Apex
        indices.push_back(current);  // Current base vertex
        indices.push_back(next);     // Next base vertex
    }
    
    // Generate bottom face triangles
    for (int i = 0; i < segments; ++i) {
        int current = 3 + i * 2;     // Current bottom vertex
        int next = 3 + ((i + 1) % segments) * 2; // Next bottom vertex
        
        indices.push_back(1);        // Center
        indices.push_back(next);     // Next vertex (clockwise)
        indices.push_back(current);  // Current vertex
    }
    
    // Convert to GLM types and use setData
    std::vector<glm::vec3> positions, normals;
    std::vector<glm::vec2> texCoords;
    
    for (const auto& vertex : vertices) {
        positions.push_back(vertex.getPositionGLM());
        normals.push_back(vertex.getNormalGLM());
        texCoords.push_back(vertex.getTexCoordGLM());
    }
    
    std::vector<rude::Vertex> verts;
    for (size_t i = 0; i < positions.size(); ++i) {
        rude::Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texCoord = texCoords[i];
        verts.push_back(v);
    }
    mesh->setData(verts, indices);
    
    return mesh;
}

// ...existing code...
rude::MeshPtr MeshGenerator::generateTorus(float majorRadius, float minorRadius, int majorSegments, int minorSegments)
{
    auto mesh = std::make_shared<rude::Mesh>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Generate vertices
    for (int i = 0; i <= majorSegments; ++i) {
        float majorAngle = static_cast<float>(i) * 2.0f * PI / static_cast<float>(majorSegments);
        float cosMajor = cos(majorAngle);
        float sinMajor = sin(majorAngle);
        
        for (int j = 0; j <= minorSegments; ++j) {
            float minorAngle = static_cast<float>(j) * 2.0f * PI / static_cast<float>(minorSegments);
            float cosMinor = cos(minorAngle);
            float sinMinor = sin(minorAngle);
            
            Vertex vertex;
            
            // Position calculation
            float x = (majorRadius + minorRadius * cosMinor) * cosMajor;
            float y = minorRadius * sinMinor;
            float z = (majorRadius + minorRadius * cosMinor) * sinMajor;
            vertex.position = glm::vec3(x, y, z);
            
            // Normal calculation
            glm::vec3 center = glm::vec3(majorRadius * cosMajor, 0, majorRadius * sinMajor);
            vertex.normal = glm::normalize(vertex.position - center);
            
            // Texture coordinates
            vertex.texCoord = glm::vec2(
                static_cast<float>(i) / majorSegments,
                static_cast<float>(j) / minorSegments
            );
            
            vertices.push_back(vertex);
        }
    }
    
    // Generate indices
    for (int i = 0; i < majorSegments; ++i) {
        for (int j = 0; j < minorSegments; ++j) {
            int current = i * (minorSegments + 1) + j;
            int next = current + minorSegments + 1;
            
            // First triangle
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);
            
            // Second triangle
            indices.push_back(next);
            indices.push_back(next + 1);
            indices.push_back(current + 1);
        }
    }
    
    // Convert to GLM types and use setData
    std::vector<glm::vec3> positions, normals;
    std::vector<glm::vec2> texCoords;
    
    for (const auto& vertex : vertices) {
        positions.push_back(vertex.getPositionGLM());
        normals.push_back(vertex.getNormalGLM());
        texCoords.push_back(vertex.getTexCoordGLM());
    }
    
    std::vector<rude::Vertex> verts;
    for (size_t i = 0; i < positions.size(); ++i) {
        rude::Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texCoord = texCoords[i];
        verts.push_back(v);
    }
    mesh->setData(verts, indices);
    
    return mesh;
}

// ...existing code...
rude::MeshPtr MeshGenerator::generateIcosphere(float radius, int subdivisions)
{
    auto mesh = std::make_shared<rude::Mesh>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Golden ratio
    const float phi = (1.0f + sqrt(5.0f)) * 0.5f;
    
    // Create initial icosahedron vertices
    std::vector<glm::vec3> icosahedronVertices = {
        glm::normalize(glm::vec3(-1,  phi, 0)) * radius,
        glm::normalize(glm::vec3( 1,  phi, 0)) * radius,
        glm::normalize(glm::vec3(-1, -phi, 0)) * radius,
        glm::normalize(glm::vec3( 1, -phi, 0)) * radius,
        glm::normalize(glm::vec3(0, -1,  phi)) * radius,
        glm::normalize(glm::vec3(0,  1,  phi)) * radius,
        glm::normalize(glm::vec3(0, -1, -phi)) * radius,
        glm::normalize(glm::vec3(0,  1, -phi)) * radius,
        glm::normalize(glm::vec3( phi, 0, -1)) * radius,
        glm::normalize(glm::vec3( phi, 0,  1)) * radius,
        glm::normalize(glm::vec3(-phi, 0, -1)) * radius,
        glm::normalize(glm::vec3(-phi, 0,  1)) * radius
    };
    
    // Create initial icosahedron faces
    std::vector<std::array<int, 3>> icosahedronFaces = {
        {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
        {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
        {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
        {4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
    };
    
    // Subdivide faces
    for (int level = 0; level < subdivisions; ++level) {
        std::vector<std::array<int, 3>> newFaces;
        std::map<std::pair<int, int>, int> midpointCache;
        
        auto getMidpoint = [&](int i, int j) -> int {
            auto key = std::make_pair(std::min(i, j), std::max(i, j));
            auto it = midpointCache.find(key);
            if (it != midpointCache.end()) {
                return it->second;
            }
            
            glm::vec3 mid = glm::normalize((icosahedronVertices[i] + icosahedronVertices[j]) * 0.5f) * radius;
            int newIndex = static_cast<int>(icosahedronVertices.size());
            icosahedronVertices.push_back(mid);
            midpointCache[key] = newIndex;
            return newIndex;
        };
        
        for (const auto& face : icosahedronFaces) {
            int mid01 = getMidpoint(face[0], face[1]);
            int mid12 = getMidpoint(face[1], face[2]);
            int mid02 = getMidpoint(face[0], face[2]);
            
            newFaces.push_back({face[0], mid01, mid02});
            newFaces.push_back({face[1], mid12, mid01});
            newFaces.push_back({face[2], mid02, mid12});
            newFaces.push_back({mid01, mid12, mid02});
        }
        
        icosahedronFaces = newFaces;
    }
    
    // Convert to vertex/index format
    for (const auto& pos : icosahedronVertices) {
        Vertex vertex;
        vertex.position = pos;
        vertex.normal = glm::normalize(pos);
        vertex.texCoord = sphericalToUV(pos);
        vertices.push_back(vertex);
    }
    
    for (const auto& face : icosahedronFaces) {
        indices.push_back(face[0]);
        indices.push_back(face[1]);
        indices.push_back(face[2]);
    }
    
    // Convert to GLM types and use setData
    std::vector<glm::vec3> positions, normals;
    std::vector<glm::vec2> texCoords;
    
    for (const auto& vertex : vertices) {
        positions.push_back(vertex.getPositionGLM());
        normals.push_back(vertex.getNormalGLM());
        texCoords.push_back(vertex.getTexCoordGLM());
    }
    
    std::vector<rude::Vertex> verts;
    for (size_t i = 0; i < positions.size(); ++i) {
        rude::Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texCoord = texCoords[i];
        verts.push_back(v);
    }
    mesh->setData(verts, indices);
    
    return mesh;
}

// Helper functions implementation
void MeshGenerator::addQuad(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                           const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
                           const glm::vec3& normal)
{
    int baseIndex = static_cast<int>(vertices.size());
    
    vertices.push_back(Vertex(v0, normal, glm::vec2(0, 0)));
    vertices.push_back(Vertex(v1, normal, glm::vec2(1, 0)));
    vertices.push_back(Vertex(v2, normal, glm::vec2(1, 1)));
    vertices.push_back(Vertex(v3, normal, glm::vec2(0, 1)));
    
    // First triangle
    indices.push_back(baseIndex + 0);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);
    
    // Second triangle
    indices.push_back(baseIndex + 0);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
}

void MeshGenerator::addTriangle(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                               const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                               const glm::vec3& normal)
{
    int baseIndex = static_cast<int>(vertices.size());
    
    vertices.push_back(Vertex(v0, normal, glm::vec2(0, 0)));
    vertices.push_back(Vertex(v1, normal, glm::vec2(1, 0)));
    vertices.push_back(Vertex(v2, normal, glm::vec2(0.5f, 1)));
    
    indices.push_back(baseIndex + 0);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);
}

glm::vec3 MeshGenerator::calculateNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
    return glm::normalize(glm::cross(v1 - v0, v2 - v0));
}

glm::vec2 MeshGenerator::sphericalToUV(const glm::vec3& position)
{
    glm::vec3 normalized = glm::normalize(position);
    float u = 0.5f + atan2(normalized.z, normalized.x) / (2.0f * PI);
    float v = 0.5f - asin(normalized.y) / PI;
    return glm::vec2(u, v);
}
