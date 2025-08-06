#pragma once

#include <memory>
#include <QString>
#include <QObject>
#include "geometry/core/Mesh.h"
#include "core/entity.hpp"

/**
 * @brief Manager for primitive creation and mesh generation
 * 
 * This class is responsible for:
 * - Creating primitive meshes (cube, sphere, plane, etc.)
 * - Managing primitive parameters and variations
 * - Providing a clean interface for primitive creation
 * - Handling primitive-specific logic and validation
 * 
 * Separates primitive creation logic from UI and scene management
 */
class PrimitiveManager : public QObject
{
    Q_OBJECT

public:
    explicit PrimitiveManager(QObject* parent = nullptr);
    ~PrimitiveManager();

    // Primitive creation methods
    std::shared_ptr<Mesh> createCube(float size = 1.0f);
    std::shared_ptr<Mesh> createPlane(float width = 1.0f, float height = 1.0f, int widthSegments = 1, int heightSegments = 1);
    std::shared_ptr<Mesh> createSphere(float radius = 1.0f, int segments = 32, int rings = 16);
    std::shared_ptr<Mesh> createCylinder(float radius = 1.0f, float height = 2.0f, int segments = 32);
    std::shared_ptr<Mesh> createCone(float radius = 1.0f, float height = 2.0f, int segments = 32);
    std::shared_ptr<Mesh> createTorus(float majorRadius = 1.0f, float minorRadius = 0.3f, int majorSegments = 32, int minorSegments = 16);
    std::shared_ptr<Mesh> createIcosphere(float radius = 1.0f, int subdivisions = 2);
    std::shared_ptr<Mesh> createGrid(float size = 10.0f, int divisions = 10);
    
    // Primitive information
    static QStringList getSupportedPrimitives();
    static QString getPrimitiveDescription(const QString& primitiveType);
    
    // Validation
    bool isValidPrimitiveType(const QString& primitiveType) const;

signals:
    void primitiveCreated(const QString& primitiveType, std::shared_ptr<Mesh> mesh);
    void primitiveCreationFailed(const QString& primitiveType, const QString& error);

private:
    // Helper methods for complex primitives
    std::shared_ptr<Mesh> createUVSphere(float radius, int segments, int rings);
    std::shared_ptr<Mesh> createIcosahedron(float radius);
    void subdivideMesh(std::shared_ptr<Mesh> mesh, int levels);
    
    // Validation helpers
    bool validateParameters(const QString& primitiveName, const std::vector<std::pair<QString, float>>& params);
    bool validateSegments(const QString& primitiveName, const QString& paramName, int value, int minimum);
};
