//==============================================================================
// RudeBase3D Engine - primitive_manager.hpp
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

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
