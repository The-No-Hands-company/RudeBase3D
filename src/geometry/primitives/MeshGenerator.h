#pragma once

#include "Common.h"
#include <vector>
#include <memory>

class MeshGenerator {
public:
    // Primitive generation
    static MeshPtr generateCube(float size = 1.0f);
    static MeshPtr generateSphere(float radius = 1.0f, int segments = 32, int rings = 16);
    static MeshPtr generateCylinder(float radius = 1.0f, float height = 2.0f, int segments = 32);
    static MeshPtr generatePlane(float width = 2.0f, float height = 2.0f, int widthSegments = 1, int heightSegments = 1);
    
    // Advanced primitives
    static MeshPtr generateCone(float radius = 1.0f, float height = 2.0f, int segments = 32);
    static MeshPtr generateTorus(float majorRadius = 1.0f, float minorRadius = 0.3f, int majorSegments = 32, int minorSegments = 16);
    static MeshPtr generateIcosphere(float radius = 1.0f, int subdivisions = 2);
    
    // Grid and debug meshes
    static MeshPtr generateGrid(float size = 10.0f, int divisions = 10);
    static MeshPtr generateWireCube(float size = 1.0f);
    static MeshPtr generateWireSphere(float radius = 1.0f, int segments = 16);

private:
    // Helper functions
    static void addQuad(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                       const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, const QVector3D& v3,
                       const QVector3D& normal);
    
    static void addTriangle(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
                           const QVector3D& v0, const QVector3D& v1, const QVector3D& v2,
                           const QVector3D& normal);
    
    static QVector3D calculateNormal(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2);
    static QVector2D sphericalToUV(const QVector3D& position);
};
