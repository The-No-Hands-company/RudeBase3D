#include "MeshGenerator.h"
#include "Mesh.h"
#include <cmath>
#include <map>
#include <array>
#include <algorithm>

MeshPtr MeshGenerator::generateCube(float size)
{
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    float halfSize = size * 0.5f;
    
    // Define cube vertices (8 corners)
    QVector3D positions[8] = {
        QVector3D(-halfSize, -halfSize, -halfSize), // 0: left-bottom-back
        QVector3D( halfSize, -halfSize, -halfSize), // 1: right-bottom-back
        QVector3D( halfSize,  halfSize, -halfSize), // 2: right-top-back
        QVector3D(-halfSize,  halfSize, -halfSize), // 3: left-top-back
        QVector3D(-halfSize, -halfSize,  halfSize), // 4: left-bottom-front
        QVector3D( halfSize, -halfSize,  halfSize), // 5: right-bottom-front
        QVector3D( halfSize,  halfSize,  halfSize), // 6: right-top-front
        QVector3D(-halfSize,  halfSize,  halfSize)  // 7: left-top-front
    };
    
    // Define face normals
    QVector3D normals[6] = {
        QVector3D( 0,  0, -1), // Back
        QVector3D( 0,  0,  1), // Front
        QVector3D(-1,  0,  0), // Left
        QVector3D( 1,  0,  0), // Right
        QVector3D( 0, -1,  0), // Bottom
        QVector3D( 0,  1,  0)  // Top
    };
    
    // Define UV coordinates
    QVector2D uvs[4] = {
        QVector2D(0, 0), // bottom-left
        QVector2D(1, 0), // bottom-right
        QVector2D(1, 1), // top-right
        QVector2D(0, 1)  // top-left
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
            Vertex vertex;
            vertex.position = positions[faceIndices[face][vert]];
            vertex.normal = normals[face];
            vertex.texCoord = uvs[vert];
            vertices.push_back(vertex);
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
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

MeshPtr MeshGenerator::generateSphere(float radius, int segments, int rings)
{
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices;
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
            
            Vertex vertex;
            vertex.position = QVector3D(
                radius * sinTheta * cosPhi,
                radius * cosTheta,
                radius * sinTheta * sinPhi
            );
            vertex.normal = vertex.position.normalized();
            vertex.texCoord = QVector2D(
                static_cast<float>(segment) / static_cast<float>(segments),
                static_cast<float>(ring) / static_cast<float>(rings)
            );
            
            vertices.push_back(vertex);
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
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

MeshPtr MeshGenerator::generateCylinder(float radius, float height, int segments)
{
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    float halfHeight = height * 0.5f;
    
    // Generate side vertices
    for (int segment = 0; segment <= segments; ++segment) {
        float angle = static_cast<float>(segment) * 2.0f * PI / static_cast<float>(segments);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        
        QVector3D normal(cosAngle, 0.0f, sinAngle);
        
        // Bottom vertex
        Vertex bottomVertex;
        bottomVertex.position = QVector3D(radius * cosAngle, -halfHeight, radius * sinAngle);
        bottomVertex.normal = normal;
        bottomVertex.texCoord = QVector2D(static_cast<float>(segment) / static_cast<float>(segments), 0.0f);
        vertices.push_back(bottomVertex);
        
        // Top vertex
        Vertex topVertex;
        topVertex.position = QVector3D(radius * cosAngle, halfHeight, radius * sinAngle);
        topVertex.normal = normal;
        topVertex.texCoord = QVector2D(static_cast<float>(segment) / static_cast<float>(segments), 1.0f);
        vertices.push_back(topVertex);
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
    int bottomCenterIndex = static_cast<int>(vertices.size());
    Vertex bottomCenter;
    bottomCenter.position = QVector3D(0.0f, -halfHeight, 0.0f);
    bottomCenter.normal = QVector3D(0.0f, -1.0f, 0.0f);
    bottomCenter.texCoord = QVector2D(0.5f, 0.5f);
    vertices.push_back(bottomCenter);
    
    for (int segment = 0; segment < segments; ++segment) {
        float angle = static_cast<float>(segment) * 2.0f * PI / static_cast<float>(segments);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        
        Vertex vertex;
        vertex.position = QVector3D(radius * cosAngle, -halfHeight, radius * sinAngle);
        vertex.normal = QVector3D(0.0f, -1.0f, 0.0f);
        vertex.texCoord = QVector2D(0.5f + 0.5f * cosAngle, 0.5f + 0.5f * sinAngle);
        vertices.push_back(vertex);
    }
    
    for (int segment = 0; segment < segments; ++segment) {
        int current = bottomCenterIndex + 1 + segment;
        int next = bottomCenterIndex + 1 + ((segment + 1) % segments);
        
        indices.push_back(bottomCenterIndex);
        indices.push_back(current);
        indices.push_back(next);
    }
    
    // Generate top cap
    int topCenterIndex = static_cast<int>(vertices.size());
    Vertex topCenter;
    topCenter.position = QVector3D(0.0f, halfHeight, 0.0f);
    topCenter.normal = QVector3D(0.0f, 1.0f, 0.0f);
    topCenter.texCoord = QVector2D(0.5f, 0.5f);
    vertices.push_back(topCenter);
    
    for (int segment = 0; segment < segments; ++segment) {
        float angle = static_cast<float>(segment) * 2.0f * PI / static_cast<float>(segments);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        
        Vertex vertex;
        vertex.position = QVector3D(radius * cosAngle, halfHeight, radius * sinAngle);
        vertex.normal = QVector3D(0.0f, 1.0f, 0.0f);
        vertex.texCoord = QVector2D(0.5f + 0.5f * cosAngle, 0.5f - 0.5f * sinAngle);
        vertices.push_back(vertex);
    }
    
    for (int segment = 0; segment < segments; ++segment) {
        int current = topCenterIndex + 1 + segment;
        int next = topCenterIndex + 1 + ((segment + 1) % segments);
        
        indices.push_back(topCenterIndex);
        indices.push_back(next);
        indices.push_back(current);
    }
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

MeshPtr MeshGenerator::generatePlane(float width, float height, int widthSegments, int heightSegments)
{
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    
    // Generate vertices
    for (int y = 0; y <= heightSegments; ++y) {
        for (int x = 0; x <= widthSegments; ++x) {
            Vertex vertex;
            vertex.position = QVector3D(
                -halfWidth + (static_cast<float>(x) / static_cast<float>(widthSegments)) * width,
                0.0f,
                -halfHeight + (static_cast<float>(y) / static_cast<float>(heightSegments)) * height
            );
            vertex.normal = QVector3D(0.0f, 1.0f, 0.0f);
            vertex.texCoord = QVector2D(
                static_cast<float>(x) / static_cast<float>(widthSegments),
                static_cast<float>(y) / static_cast<float>(heightSegments)
            );
            
            vertices.push_back(vertex);
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
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

MeshPtr MeshGenerator::generateGrid(float size, int divisions)
{
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices;
    
    float halfSize = size * 0.5f;
    float step = size / static_cast<float>(divisions);
    
    // Generate grid lines parallel to X-axis
    for (int i = 0; i <= divisions; ++i) {
        float z = -halfSize + i * step;
        
        vertices.push_back(Vertex(QVector3D(-halfSize, 0.0f, z)));
        vertices.push_back(Vertex(QVector3D(halfSize, 0.0f, z)));
    }
    
    // Generate grid lines parallel to Z-axis
    for (int i = 0; i <= divisions; ++i) {
        float x = -halfSize + i * step;
        
        vertices.push_back(Vertex(QVector3D(x, 0.0f, -halfSize)));
        vertices.push_back(Vertex(QVector3D(x, 0.0f, halfSize)));
    }
    
    mesh->setVertices(vertices);
    
    return mesh;
}

MeshPtr MeshGenerator::generateCone(float radius, float height, int segments)
{
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Apex vertex at top
    Vertex apex;
    apex.position = QVector3D(0, height * 0.5f, 0);
    apex.normal = QVector3D(0, 1, 0);
    apex.texCoord = QVector2D(0.5f, 0.5f);
    vertices.push_back(apex);
    
    // Center vertex at bottom
    Vertex center;
    center.position = QVector3D(0, -height * 0.5f, 0);
    center.normal = QVector3D(0, -1, 0);
    center.texCoord = QVector2D(0.5f, 0.5f);
    vertices.push_back(center);
    
    // Generate base circle vertices
    for (int i = 0; i <= segments; ++i) {
        float angle = static_cast<float>(i) * 2.0f * PI / static_cast<float>(segments);
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        
        // Vertex for side faces
        Vertex sideVertex;
        sideVertex.position = QVector3D(x, -height * 0.5f, z);
        // Calculate side normal (pointing outward and upward)
        QVector3D toApex = (apex.position - sideVertex.position).normalized();
        QVector3D radial = QVector3D(x, 0, z).normalized();
        sideVertex.normal = QVector3D::crossProduct(QVector3D::crossProduct(toApex, radial), toApex).normalized();
        sideVertex.texCoord = QVector2D(static_cast<float>(i) / segments, 0);
        vertices.push_back(sideVertex);
        
        // Vertex for bottom face
        Vertex bottomVertex;
        bottomVertex.position = QVector3D(x, -height * 0.5f, z);
        bottomVertex.normal = QVector3D(0, -1, 0);
        bottomVertex.texCoord = QVector2D(0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle));
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
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

MeshPtr MeshGenerator::generateTorus(float majorRadius, float minorRadius, int majorSegments, int minorSegments)
{
    auto mesh = std::make_shared<Mesh>();
    
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
            vertex.position = QVector3D(x, y, z);
            
            // Normal calculation
            QVector3D center = QVector3D(majorRadius * cosMajor, 0, majorRadius * sinMajor);
            vertex.normal = (vertex.position - center).normalized();
            
            // Texture coordinates
            vertex.texCoord = QVector2D(
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
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

MeshPtr MeshGenerator::generateIcosphere(float radius, int subdivisions)
{
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Golden ratio
    const float phi = (1.0f + sqrt(5.0f)) * 0.5f;
    const float invNorm = 1.0f / sqrt(phi * phi + 1.0f);
    
    // Create initial icosahedron vertices
    std::vector<QVector3D> icosahedronVertices = {
        QVector3D(-1,  phi, 0).normalized() * radius,
        QVector3D( 1,  phi, 0).normalized() * radius,
        QVector3D(-1, -phi, 0).normalized() * radius,
        QVector3D( 1, -phi, 0).normalized() * radius,
        QVector3D(0, -1,  phi).normalized() * radius,
        QVector3D(0,  1,  phi).normalized() * radius,
        QVector3D(0, -1, -phi).normalized() * radius,
        QVector3D(0,  1, -phi).normalized() * radius,
        QVector3D( phi, 0, -1).normalized() * radius,
        QVector3D( phi, 0,  1).normalized() * radius,
        QVector3D(-phi, 0, -1).normalized() * radius,
        QVector3D(-phi, 0,  1).normalized() * radius
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
            
            QVector3D mid = ((icosahedronVertices[i] + icosahedronVertices[j]) * 0.5f).normalized() * radius;
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
        vertex.normal = pos.normalized();
        vertex.texCoord = sphericalToUV(pos);
        vertices.push_back(vertex);
    }
    
    for (const auto& face : icosahedronFaces) {
        indices.push_back(face[0]);
        indices.push_back(face[1]);
        indices.push_back(face[2]);
    }
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

// Helper functions implementation
void MeshGenerator::addQuad(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                           const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, const QVector3D& v3,
                           const QVector3D& normal)
{
    int baseIndex = static_cast<int>(vertices.size());
    
    vertices.push_back(Vertex(v0, normal, QVector2D(0, 0)));
    vertices.push_back(Vertex(v1, normal, QVector2D(1, 0)));
    vertices.push_back(Vertex(v2, normal, QVector2D(1, 1)));
    vertices.push_back(Vertex(v3, normal, QVector2D(0, 1)));
    
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
                               const QVector3D& v0, const QVector3D& v1, const QVector3D& v2,
                               const QVector3D& normal)
{
    int baseIndex = static_cast<int>(vertices.size());
    
    vertices.push_back(Vertex(v0, normal, QVector2D(0, 0)));
    vertices.push_back(Vertex(v1, normal, QVector2D(1, 0)));
    vertices.push_back(Vertex(v2, normal, QVector2D(0.5f, 1)));
    
    indices.push_back(baseIndex + 0);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);
}

QVector3D MeshGenerator::calculateNormal(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2)
{
    return QVector3D::crossProduct(v1 - v0, v2 - v0).normalized();
}

QVector2D MeshGenerator::sphericalToUV(const QVector3D& position)
{
    QVector3D normalized = position.normalized();
    float u = 0.5f + atan2(normalized.z(), normalized.x()) / (2.0f * PI);
    float v = 0.5f - asin(normalized.y()) / PI;
    return QVector2D(u, v);
}
