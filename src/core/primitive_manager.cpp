#include "core/primitive_manager.hpp"
#include <QDebug>
#include <QStringList>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>
#include <unordered_map>

PrimitiveManager::PrimitiveManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[PrimitiveManager] Created";
}

PrimitiveManager::~PrimitiveManager()
{
    qDebug() << "[PrimitiveManager] Destroyed";
}

// Utility function for parameter validation
bool PrimitiveManager::validateParameters(const QString& primitiveName, const std::vector<std::pair<QString, float>>& params)
{
    for (const auto& param : params) {
        if (param.second <= 0.0f) {
            QString error = QString("%1 parameter '%2' must be positive (got %3)")
                .arg(primitiveName, param.first).arg(param.second);
            emit primitiveCreationFailed(primitiveName, error);
            return false;
        }
    }
    return true;
}

bool PrimitiveManager::validateSegments(const QString& primitiveName, const QString& paramName, int value, int minimum)
{
    if (value < minimum) {
        QString error = QString("%1 parameter '%2' must be at least %3 (got %4)")
            .arg(primitiveName, paramName).arg(minimum).arg(value);
        emit primitiveCreationFailed(primitiveName, error);
        return false;
    }
    return true;
}

std::shared_ptr<Mesh> PrimitiveManager::createCube(float size)
{
    qDebug() << "[PrimitiveManager] Creating cube with size:" << size;
    
    if (!validateParameters("Cube", {{"size", size}})) {
        return nullptr;
    }
    
    try {
        auto mesh = std::make_shared<Mesh>();
        
        const float half = size * 0.5f;
        
        // 8 vertices of a cube (using DirectXTK approach)
        std::vector<glm::vec3> vertices = {
            // Front face (z = +half)
            glm::vec3(-half, -half,  half),  // 0: bottom-left-front
            glm::vec3( half, -half,  half),  // 1: bottom-right-front
            glm::vec3( half,  half,  half),  // 2: top-right-front
            glm::vec3(-half,  half,  half),  // 3: top-left-front
            
            // Back face (z = -half)
            glm::vec3(-half, -half, -half),  // 4: bottom-left-back
            glm::vec3( half, -half, -half),  // 5: bottom-right-back
            glm::vec3( half,  half, -half),  // 6: top-right-back
            glm::vec3(-half,  half, -half)   // 7: top-left-back
        };
        
        // 36 indices for 12 triangles (6 faces * 2 triangles each)
        // Using proper winding order (counter-clockwise for outward-facing normals)
        std::vector<unsigned int> indices = {
            // Front face
            0, 1, 2,  2, 3, 0,
            // Back face  
            5, 4, 7,  7, 6, 5,
            // Left face
            4, 0, 3,  3, 7, 4,
            // Right face
            1, 5, 6,  6, 2, 1,
            // Top face
            3, 2, 6,  6, 7, 3,
            // Bottom face
            4, 5, 1,  1, 0, 4
        };
        
        // Calculate proper vertex normals (average of adjacent face normals)
        std::vector<glm::vec3> normals = {
            glm::normalize(glm::vec3(-1, -1,  1)),  // 0
            glm::normalize(glm::vec3( 1, -1,  1)),  // 1
            glm::normalize(glm::vec3( 1,  1,  1)),  // 2
            glm::normalize(glm::vec3(-1,  1,  1)),  // 3
            glm::normalize(glm::vec3(-1, -1, -1)),  // 4
            glm::normalize(glm::vec3( 1, -1, -1)),  // 5
            glm::normalize(glm::vec3( 1,  1, -1)),  // 6
            glm::normalize(glm::vec3(-1,  1, -1))   // 7
        };
        
        // UV coordinates for texture mapping
        std::vector<glm::vec2> uvs = {
            glm::vec2(0.0f, 0.0f),  // 0
            glm::vec2(1.0f, 0.0f),  // 1
            glm::vec2(1.0f, 1.0f),  // 2
            glm::vec2(0.0f, 1.0f),  // 3
            glm::vec2(0.0f, 0.0f),  // 4
            glm::vec2(1.0f, 0.0f),  // 5
            glm::vec2(1.0f, 1.0f),  // 6
            glm::vec2(0.0f, 1.0f)   // 7
        };
        
        mesh->setData(vertices, indices, normals, uvs);
        
        emit primitiveCreated("Cube", mesh);
        return mesh;
        
    } catch (const std::exception& e) {
        QString error = QString("Exception creating cube: %1").arg(e.what());
        emit primitiveCreationFailed("Cube", error);
        return nullptr;
    }
}

std::shared_ptr<Mesh> PrimitiveManager::createPlane(float width, float height, int widthSegments, int heightSegments)
{
    qDebug() << "[PrimitiveManager] Creating plane:" << width << "x" << height << "segments:" << widthSegments << "x" << heightSegments;
    
    try {
        auto mesh = std::make_shared<Mesh>();
        
        // Calculate vertex positions
        std::vector<glm::vec3> vertices;
        std::vector<uint32_t> indices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        
        float halfWidth = width * 0.5f;
        float halfHeight = height * 0.5f;
        
        // Generate vertices
        for (int y = 0; y <= heightSegments; ++y) {
            for (int x = 0; x <= widthSegments; ++x) {
                float xPos = (float(x) / widthSegments - 0.5f) * width;
                float zPos = (float(y) / heightSegments - 0.5f) * height;
                
                vertices.push_back(glm::vec3(xPos, 0.0f, zPos));
                normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // All normals point up
                uvs.push_back(glm::vec2(float(x) / widthSegments, float(y) / heightSegments));
            }
        }
        
        // Generate indices
        for (int y = 0; y < heightSegments; ++y) {
            for (int x = 0; x < widthSegments; ++x) {
                uint32_t topLeft = y * (widthSegments + 1) + x;
                uint32_t topRight = topLeft + 1;
                uint32_t bottomLeft = (y + 1) * (widthSegments + 1) + x;
                uint32_t bottomRight = bottomLeft + 1;
                
                // First triangle
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
                
                // Second triangle
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
        
        mesh->setData(vertices, indices, normals, uvs);
        
        if (mesh) {
            emit primitiveCreated("Plane", mesh);
        } else {
            emit primitiveCreationFailed("Plane", "Failed to create plane mesh");
        }
        
        return mesh;
    } catch (const std::exception& e) {
        QString error = QString("Exception creating plane: %1").arg(e.what());
        emit primitiveCreationFailed("Plane", error);
        return nullptr;
    }
}

std::shared_ptr<Mesh> PrimitiveManager::createSphere(float radius, int segments, int rings)
{
    qDebug() << "[PrimitiveManager] Creating sphere: radius=" << radius << "segments=" << segments << "rings=" << rings;
    
    if (!validateParameters("Sphere", {{"radius", radius}})) {
        return nullptr;
    }
    if (!validateSegments("Sphere", "segments", segments, 3)) {
        return nullptr;
    }
    if (!validateSegments("Sphere", "rings", rings, 2)) {
        return nullptr;
    }
    
    try {
        auto mesh = createUVSphere(radius, segments, rings);
        
        if (mesh) {
            emit primitiveCreated("Sphere", mesh);
        } else {
            emit primitiveCreationFailed("Sphere", "Failed to create sphere mesh");
        }
        
        return mesh;
    } catch (const std::exception& e) {
        QString error = QString("Exception creating sphere: %1").arg(e.what());
        emit primitiveCreationFailed("Sphere", error);
        return nullptr;
    }
}

std::shared_ptr<Mesh> PrimitiveManager::createCylinder(float radius, float height, int segments)
{
    qDebug() << "[PrimitiveManager] Creating cylinder: radius=" << radius << "height=" << height << "segments=" << segments;
    
    if (!validateParameters("Cylinder", {{"radius", radius}, {"height", height}})) {
        return nullptr;
    }
    if (!validateSegments("Cylinder", "segments", segments, 3)) {
        return nullptr;
    }
    
    try {
        auto mesh = std::make_shared<Mesh>();
        
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        
        const float halfHeight = height * 0.5f;
        const float angleStep = 2.0f * glm::pi<float>() / segments;
        
        // Center vertices for caps
        vertices.push_back(glm::vec3(0.0f, halfHeight, 0.0f));   // Top center (0)
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        uvs.push_back(glm::vec2(0.5f, 0.5f));
        
        vertices.push_back(glm::vec3(0.0f, -halfHeight, 0.0f));  // Bottom center (1)
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        uvs.push_back(glm::vec2(0.5f, 0.5f));
        
        // Side vertices (top and bottom rings)
        for (int i = 0; i < segments; ++i) {
            float angle = i * angleStep;
            float x = radius * std::cos(angle);
            float z = radius * std::sin(angle);
            
            // Top ring
            vertices.push_back(glm::vec3(x, halfHeight, z));
            normals.push_back(glm::normalize(glm::vec3(x, 0.0f, z))); // Side normal
            uvs.push_back(glm::vec2(float(i) / segments, 1.0f));
            
            // Bottom ring  
            vertices.push_back(glm::vec3(x, -halfHeight, z));
            normals.push_back(glm::normalize(glm::vec3(x, 0.0f, z))); // Side normal
            uvs.push_back(glm::vec2(float(i) / segments, 0.0f));
        }
        
        // Side faces (connecting top and bottom rings)
        for (int i = 0; i < segments; ++i) {
            int nextI = (i + 1) % segments;
            
            int topCurrent = 2 + i * 2;      // Top ring vertex
            int bottomCurrent = 2 + i * 2 + 1; // Bottom ring vertex
            int topNext = 2 + nextI * 2;
            int bottomNext = 2 + nextI * 2 + 1;
            
            // Two triangles per side quad
            indices.push_back(topCurrent);
            indices.push_back(bottomCurrent);
            indices.push_back(topNext);
            
            indices.push_back(topNext);
            indices.push_back(bottomCurrent);
            indices.push_back(bottomNext);
        }
        
        // Top cap (fan around top center)
        for (int i = 0; i < segments; ++i) {
            int nextI = (i + 1) % segments;
            int current = 2 + i * 2;      // Top ring vertex
            int next = 2 + nextI * 2;     // Next top ring vertex
            
            indices.push_back(0);      // Top center
            indices.push_back(current);
            indices.push_back(next);
        }
        
        // Bottom cap (fan around bottom center)
        for (int i = 0; i < segments; ++i) {
            int nextI = (i + 1) % segments;
            int current = 2 + i * 2 + 1;      // Bottom ring vertex
            int next = 2 + nextI * 2 + 1;     // Next bottom ring vertex
            
            indices.push_back(1);      // Bottom center
            indices.push_back(next);   // Reverse winding for bottom
            indices.push_back(current);
        }
        
        mesh->setData(vertices, indices, normals, uvs);
        
        emit primitiveCreated("Cylinder", mesh);
        return mesh;
        
    } catch (const std::exception& e) {
        QString error = QString("Exception creating cylinder: %1").arg(e.what());
        emit primitiveCreationFailed("Cylinder", error);
        return nullptr;
    }
}

std::shared_ptr<Mesh> PrimitiveManager::createCone(float radius, float height, int segments)
{
    qDebug() << "[PrimitiveManager] Creating cone: radius=" << radius << "height=" << height << "segments=" << segments;
    
    if (!validateParameters("Cone", {{"radius", radius}, {"height", height}})) {
        return nullptr;
    }
    if (!validateSegments("Cone", "segments", segments, 3)) {
        return nullptr;
    }
    
    try {
        auto mesh = std::make_shared<Mesh>();
        
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        
        const float halfHeight = height * 0.5f;
        const float angleStep = 2.0f * glm::pi<float>() / segments;
        
        // Apex of cone
        vertices.push_back(glm::vec3(0.0f, halfHeight, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        uvs.push_back(glm::vec2(0.5f, 1.0f));
        
        // Base center
        vertices.push_back(glm::vec3(0.0f, -halfHeight, 0.0f));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        uvs.push_back(glm::vec2(0.5f, 0.5f));
        
        // Base circle vertices
        for (int i = 0; i < segments; ++i) {
            float angle = i * angleStep;
            float x = radius * std::cos(angle);
            float z = radius * std::sin(angle);
            
            vertices.push_back(glm::vec3(x, -halfHeight, z));
            
            // Calculate side normal for cone (pointing outward and upward)
            glm::vec3 toApex = glm::normalize(glm::vec3(0.0f, halfHeight, 0.0f) - glm::vec3(x, -halfHeight, z));
            glm::vec3 radial = glm::normalize(glm::vec3(x, 0.0f, z));
            glm::vec3 tangent = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), radial));
            glm::vec3 sideNormal = glm::normalize(glm::cross(tangent, toApex));
            normals.push_back(sideNormal);
            
            uvs.push_back(glm::vec2(float(i) / segments, 0.0f));
        }
        
        // Side faces (triangular faces from apex to base)
        for (int i = 0; i < segments; ++i) {
            int nextI = (i + 1) % segments;
            int current = 2 + i;      // Base vertex
            int next = 2 + nextI;     // Next base vertex
            
            indices.push_back(0);      // Apex
            indices.push_back(current);
            indices.push_back(next);
        }
        
        // Base cap (fan around base center)
        for (int i = 0; i < segments; ++i) {
            int nextI = (i + 1) % segments;
            int current = 2 + i;      // Base vertex
            int next = 2 + nextI;     // Next base vertex
            
            indices.push_back(1);      // Base center
            indices.push_back(next);   // Reverse winding for bottom face
            indices.push_back(current);
        }
        
        mesh->setData(vertices, indices, normals, uvs);
        
        emit primitiveCreated("Cone", mesh);
        return mesh;
        
    } catch (const std::exception& e) {
        QString error = QString("Exception creating cone: %1").arg(e.what());
        emit primitiveCreationFailed("Cone", error);
        return nullptr;
    }
}

std::shared_ptr<Mesh> PrimitiveManager::createTorus(float majorRadius, float minorRadius, int majorSegments, int minorSegments)
{
    qDebug() << "[PrimitiveManager] Creating torus: major=" << majorRadius << "minor=" << minorRadius;
    
    if (!validateParameters("Torus", {{"majorRadius", majorRadius}, {"minorRadius", minorRadius}})) {
        return nullptr;
    }
    if (!validateSegments("Torus", "majorSegments", majorSegments, 3)) {
        return nullptr;
    }
    if (!validateSegments("Torus", "minorSegments", minorSegments, 3)) {
        return nullptr;
    }
    
    try {
        auto mesh = std::make_shared<Mesh>();
        
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        
        const float majorAngleStep = 2.0f * glm::pi<float>() / majorSegments;
        const float minorAngleStep = 2.0f * glm::pi<float>() / minorSegments;
        
        // Generate vertices
        for (int i = 0; i < majorSegments; ++i) {
            float majorAngle = i * majorAngleStep;
            float cosMajor = std::cos(majorAngle);
            float sinMajor = std::sin(majorAngle);
            
            for (int j = 0; j < minorSegments; ++j) {
                float minorAngle = j * minorAngleStep;
                float cosMinor = std::cos(minorAngle);
                float sinMinor = std::sin(minorAngle);
                
                // Calculate vertex position
                float x = (majorRadius + minorRadius * cosMinor) * cosMajor;
                float y = minorRadius * sinMinor;
                float z = (majorRadius + minorRadius * cosMinor) * sinMajor;
                
                vertices.push_back(glm::vec3(x, y, z));
                
                // Calculate normal (pointing outward from torus center)
                glm::vec3 center = glm::vec3(majorRadius * cosMajor, 0.0f, majorRadius * sinMajor);
                glm::vec3 normal = glm::normalize(glm::vec3(x, y, z) - center);
                normals.push_back(normal);
                
                // Calculate UV coordinates
                float u = float(i) / float(majorSegments);
                float v = float(j) / float(minorSegments);
                uvs.push_back(glm::vec2(u, v));
            }
        }
        
        // Generate indices
        for (int i = 0; i < majorSegments; ++i) {
            int nextI = (i + 1) % majorSegments;
            
            for (int j = 0; j < minorSegments; ++j) {
                int nextJ = (j + 1) % minorSegments;
                
                int current = i * minorSegments + j;
                int nextMajor = nextI * minorSegments + j;
                int nextMinor = i * minorSegments + nextJ;
                int nextBoth = nextI * minorSegments + nextJ;
                
                // Two triangles per quad
                indices.push_back(current);
                indices.push_back(nextMajor);
                indices.push_back(nextMinor);
                
                indices.push_back(nextMinor);
                indices.push_back(nextMajor);
                indices.push_back(nextBoth);
            }
        }
        
        mesh->setData(vertices, indices, normals, uvs);
        
        emit primitiveCreated("Torus", mesh);
        return mesh;
        
    } catch (const std::exception& e) {
        QString error = QString("Exception creating torus: %1").arg(e.what());
        emit primitiveCreationFailed("Torus", error);
        return nullptr;
    }
}

std::shared_ptr<Mesh> PrimitiveManager::createIcosphere(float radius, int subdivisions)
{
    qDebug() << "[PrimitiveManager] Creating icosphere: radius=" << radius << "subdivisions=" << subdivisions;
    
    try {
        auto mesh = createIcosahedron(radius);
        if (mesh && subdivisions > 0) {
            subdivideMesh(mesh, subdivisions);
        }
        
        if (mesh) {
            emit primitiveCreated("Icosphere", mesh);
        } else {
            emit primitiveCreationFailed("Icosphere", "Failed to create icosphere mesh");
        }
        
        return mesh;
    } catch (const std::exception& e) {
        QString error = QString("Exception creating icosphere: %1").arg(e.what());
        emit primitiveCreationFailed("Icosphere", error);
        return nullptr;
    }
}

std::shared_ptr<Mesh> PrimitiveManager::createGrid(float size, int divisions)
{
    qDebug() << "[PrimitiveManager] Creating grid: size=" << size << "divisions=" << divisions;
    
    // TODO: Implement grid creation
    emit primitiveCreationFailed("Grid", "Grid creation not yet implemented");
    return nullptr;
}

QStringList PrimitiveManager::getSupportedPrimitives()
{
    return QStringList{
        "Cube",
        "Plane", 
        "Sphere",
        "Cylinder",
        "Cone",
        "Torus",
        "Icosphere",
        "Grid"
    };
}

QString PrimitiveManager::getPrimitiveDescription(const QString& primitiveType)
{
    static QMap<QString, QString> descriptions = {
        {"Cube", "A six-faced polyhedron with square faces"},
        {"Plane", "A flat two-dimensional surface"},
        {"Sphere", "A perfectly round geometrical object"},
        {"Cylinder", "A surface of revolution around a straight line"},
        {"Cone", "A surface formed by rotating a line around an axis"},
        {"Torus", "A doughnut-shaped surface of revolution"},
        {"Icosphere", "A sphere created from subdivided icosahedron"},
        {"Grid", "A reference grid for modeling"}
    };
    
    return descriptions.value(primitiveType, "Unknown primitive type");
}

bool PrimitiveManager::isValidPrimitiveType(const QString& primitiveType) const
{
    return getSupportedPrimitives().contains(primitiveType, Qt::CaseInsensitive);
}

std::shared_ptr<Mesh> PrimitiveManager::createUVSphere(float radius, int segments, int rings)
{
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    
    // Generate vertices using DirectXTK approach for better UV mapping
    for (int ring = 0; ring <= rings; ++ring) {
        float phi = glm::pi<float>() * float(ring) / rings;
        float sinPhi = std::sin(phi);
        float cosPhi = std::cos(phi);
        
        for (int segment = 0; segment <= segments; ++segment) {
            float theta = 2.0f * glm::pi<float>() * float(segment) / segments;
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);
            
            // Calculate vertex position
            float x = cosTheta * sinPhi;
            float y = cosPhi;
            float z = sinTheta * sinPhi;
            
            vertices.push_back(glm::vec3(x, y, z) * radius);
            normals.push_back(glm::normalize(glm::vec3(x, y, z)));
            
            // Calculate UV coordinates (avoiding pole singularities)
            float u = float(segment) / float(segments);
            float v = float(ring) / float(rings);
            uvs.push_back(glm::vec2(u, v));
        }
    }
    
    // Generate indices with proper winding order
    for (int ring = 0; ring < rings; ++ring) {
        for (int segment = 0; segment < segments; ++segment) {
            uint32_t current = ring * (segments + 1) + segment;
            uint32_t next = current + segments + 1;
            
            // Avoid degenerate triangles at poles
            if (ring > 0) {
                // First triangle
                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);
            }
            
            if (ring < rings - 1) {
                // Second triangle
                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }
    }
    
    mesh->setData(vertices, indices, normals, uvs);
    return mesh;
}

std::shared_ptr<Mesh> PrimitiveManager::createIcosahedron(float radius)
{
    qDebug() << "[PrimitiveManager] Creating icosahedron with radius:" << radius;
    
    if (!validateParameters("Icosahedron", {{"radius", radius}})) {
        return nullptr;
    }
    
    try {
        auto mesh = std::make_shared<Mesh>();
        
        // Golden ratio
        const float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;
        
        // 12 vertices of icosahedron (based on libigl implementation)
        std::vector<glm::vec3> vertices = {
            // Top point
            glm::vec3(0.0f, 0.0f, 1.0f),
            
            // Upper ring (5 vertices)
            glm::vec3(std::cos(0.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 
                      std::sin(0.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 0.5f / phi),
            glm::vec3(std::cos(1.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 
                      std::sin(1.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 0.5f / phi),
            glm::vec3(std::cos(2.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 
                      std::sin(2.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 0.5f / phi),
            glm::vec3(std::cos(3.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 
                      std::sin(3.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 0.5f / phi),
            glm::vec3(std::cos(4.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 
                      std::sin(4.0f * 2.0f * glm::pi<float>() / 5.0f) / phi, 0.5f / phi),
                      
            // Lower ring (5 vertices) - offset by pi/5
            glm::vec3(std::cos((0.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, 
                      std::sin((0.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, -0.5f / phi),
            glm::vec3(std::cos((1.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, 
                      std::sin((1.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, -0.5f / phi),
            glm::vec3(std::cos((2.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, 
                      std::sin((2.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, -0.5f / phi),
            glm::vec3(std::cos((3.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, 
                      std::sin((3.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, -0.5f / phi),
            glm::vec3(std::cos((4.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, 
                      std::sin((4.0f * 2.0f + 1.0f) * glm::pi<float>() / 5.0f) / phi, -0.5f / phi),
                      
            // Bottom point
            glm::vec3(0.0f, 0.0f, -1.0f)
        };
        
        // Scale and normalize vertices
        for (auto& vertex : vertices) {
            vertex = glm::normalize(vertex) * radius;
        }
        
        // 20 triangular faces of icosahedron
        std::vector<unsigned int> indices = {
            // Top cap (5 triangles)
            0, 1, 2,   0, 2, 3,   0, 3, 4,   0, 4, 5,   0, 5, 1,
            
            // Upper belt (5 triangles)
            1, 6, 2,   2, 7, 3,   3, 8, 4,   4, 9, 5,   5, 10, 1,
            
            // Lower belt (5 triangles) 
            6, 7, 2,   7, 8, 3,   8, 9, 4,   9, 10, 5,   10, 6, 1,
            
            // Bottom cap (5 triangles)
            11, 6, 7,   11, 7, 8,   11, 8, 9,   11, 9, 10,   11, 10, 6
        };
        
        // Calculate normals (for icosahedron, normals equal normalized vertex positions)
        std::vector<glm::vec3> normals;
        for (const auto& vertex : vertices) {
            normals.push_back(glm::normalize(vertex));
        }
        
        // Calculate UV coordinates (simple spherical mapping)
        std::vector<glm::vec2> uvs;
        for (const auto& vertex : vertices) {
            glm::vec3 n = glm::normalize(vertex);
            float u = 0.5f + std::atan2(n.z, n.x) / (2.0f * glm::pi<float>());
            float v = 0.5f - std::asin(n.y) / glm::pi<float>();
            uvs.push_back(glm::vec2(u, v));
        }
        
        mesh->setData(vertices, indices, normals, uvs);
        
        return mesh;
        
    } catch (const std::exception& e) {
        QString error = QString("Exception creating icosahedron: %1").arg(e.what());
        qDebug() << error;
        return nullptr;
    }
}

void PrimitiveManager::subdivideMesh(std::shared_ptr<Mesh> mesh, int levels)
{
    // TODO: Implement mesh subdivision
    // This would use the half-edge mesh structure for subdivision
    qDebug() << "[PrimitiveManager] Mesh subdivision not yet implemented";
}
