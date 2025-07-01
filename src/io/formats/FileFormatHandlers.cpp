#include "FileFormatHandlers.h"
#include "GeometryConverter.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QRegularExpression>
#include <algorithm>
#include <unordered_map>

// OBJFileHandler Implementation
OBJFileHandler::ImportResult OBJFileHandler::importFromFile(const QString& filePath, const ImportOptions& options) {
    ImportResult result;
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        result.errorMessage = QString("Cannot open file: %1").arg(filePath);
        return result;
    }
    
    QTextStream stream(&file);
    return importFromStream(stream, options);
}

bool OBJFileHandler::exportToFile(const QString& filePath, MeshPtr mesh, const ExportOptions& options) {
    if (!mesh) {
        return false;
    }
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream stream(&file);
    return exportToStream(stream, mesh, options);
}

bool OBJFileHandler::exportToFile(const QString& filePath, HalfEdgeMeshPtr mesh, const ExportOptions& options) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportToFile(filePath, faceVertexMesh, options);
}

bool OBJFileHandler::exportToFile(const QString& filePath, const std::vector<MeshPtr>& meshes, const ExportOptions& options) {
    if (meshes.empty()) {
        return false;
    }
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream stream(&file);
    
    // Write header
    stream << "# Exported from RudeBase3D" << Qt::endl;
    stream << "# Meshes: " << meshes.size() << Qt::endl;
    stream << Qt::endl;
    
    int vertexOffset = 0;
    int normalOffset = 0;
    int texCoordOffset = 0;
    
    for (size_t i = 0; i < meshes.size(); ++i) {
        const auto& mesh = meshes[i];
        if (!mesh) continue;
        
        // Write group/object name
        stream << "g mesh_" << i << Qt::endl;
        stream << "o mesh_" << i << Qt::endl;
        
        const auto& vertices = mesh->getVertices();
        
        // Write vertices
        for (const auto& vertex : vertices) {
            stream << "v " << formatFloat(vertex.position.x(), options.precision)
                   << " " << formatFloat(vertex.position.y(), options.precision)
                   << " " << formatFloat(vertex.position.z(), options.precision) << Qt::endl;
        }
        
        // Write normals
        if (options.exportNormals) {
            for (const auto& vertex : vertices) {
                stream << "vn " << formatFloat(vertex.normal.x(), options.precision)
                       << " " << formatFloat(vertex.normal.y(), options.precision)
                       << " " << formatFloat(vertex.normal.z(), options.precision) << Qt::endl;
            }
        }
        
        // Write texture coordinates
        if (options.exportTexCoords) {
            for (const auto& vertex : vertices) {
                stream << "vt " << formatFloat(vertex.texCoord.x(), options.precision)
                       << " " << formatFloat(vertex.texCoord.y(), options.precision) << Qt::endl;
            }
        }
        
        // Write faces
        const auto& indices = mesh->getIndices();
        for (size_t j = 0; j < indices.size(); j += 3) {
            stream << "f";
            for (int k = 0; k < 3; ++k) {
                int idx = indices[j + k] + vertexOffset + 1; // OBJ indices are 1-based
                stream << " " << idx;
                
                if (options.exportTexCoords) {
                    stream << "/" << (idx + texCoordOffset);
                }
                
                if (options.exportNormals) {
                    if (!options.exportTexCoords) {
                        stream << "/";
                    }
                    stream << "/" << (idx + normalOffset);
                }
            }
            stream << Qt::endl;
        }
        
        vertexOffset += static_cast<int>(vertices.size());
        if (options.exportNormals) normalOffset += static_cast<int>(vertices.size());
        if (options.exportTexCoords) texCoordOffset += static_cast<int>(vertices.size());
        
        stream << Qt::endl;
    }
    
    return true;
}

OBJFileHandler::ImportResult OBJFileHandler::importFromStream(QTextStream& stream, const ImportOptions& options) {
    ImportResult result;
    
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> texCoords;
    std::vector<std::vector<int>> faces;
    std::vector<std::vector<int>> texCoordFaces;
    std::vector<std::vector<int>> normalFaces;
    
    QString line;
    int lineNumber = 0;
    
    while (stream.readLineInto(&line)) {
        lineNumber++;
        line = line.trimmed();
        
        if (line.isEmpty() || line.startsWith('#')) {
            continue;
        }
        
        QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.isEmpty()) {
            continue;
        }
        
        const QString& command = tokens[0];
        
        if (command == "v") {
            QVector3D vertex;
            if (parseVertex(tokens, vertex)) {
                vertices.push_back(vertex);
            } else {
                qWarning() << "Invalid vertex at line" << lineNumber;
            }
        }
        else if (command == "vn") {
            QVector3D normal;
            if (parseNormal(tokens, normal)) {
                normals.push_back(normal);
            } else {
                qWarning() << "Invalid normal at line" << lineNumber;
            }
        }
        else if (command == "vt") {
            QVector2D texCoord;
            if (parseTexCoord(tokens, texCoord)) {
                texCoords.push_back(texCoord);
            } else {
                qWarning() << "Invalid texture coordinate at line" << lineNumber;
            }
        }
        else if (command == "f") {
            std::vector<int> vertexIndices;
            std::vector<int> texCoordIndices;
            std::vector<int> normalIndices;
            
            if (parseFace(tokens, vertexIndices, texCoordIndices, normalIndices)) {
                faces.push_back(vertexIndices);
                texCoordFaces.push_back(texCoordIndices);
                normalFaces.push_back(normalIndices);
            } else {
                qWarning() << "Invalid face at line" << lineNumber;
            }
        }
        // Ignore other commands for now (g, o, mtllib, usemtl, etc.)
    }
    
    // Build mesh
    auto mesh = buildMesh(vertices, normals, texCoords, faces, texCoordFaces, normalFaces, options);
    if (mesh) {
        result.success = true;
        result.meshes.push_back(mesh);
        result.meshNames.push_back("ImportedMesh");
        result.vertexCount = static_cast<int>(vertices.size());
        result.faceCount = static_cast<int>(faces.size());
    } else {
        result.errorMessage = "Failed to build mesh from OBJ data";
    }
    
    return result;
}

bool OBJFileHandler::exportToStream(QTextStream& stream, MeshPtr mesh, const ExportOptions& options) {
    if (!mesh) {
        return false;
    }
    
    const auto& vertices = mesh->getVertices();
    const auto& indices = mesh->getIndices();
    
    // Write header
    stream << "# Exported from RudeBase3D" << Qt::endl;
    stream << "# Vertices: " << vertices.size() << Qt::endl;
    stream << "# Faces: " << (indices.size() / 3) << Qt::endl;
    stream << Qt::endl;
    
    // Write vertices
    for (const auto& vertex : vertices) {
        stream << "v " << formatFloat(vertex.position.x(), options.precision)
               << " " << formatFloat(vertex.position.y(), options.precision)
               << " " << formatFloat(vertex.position.z(), options.precision) << Qt::endl;
    }
    
    // Write normals
    if (options.exportNormals) {
        for (const auto& vertex : vertices) {
            stream << "vn " << formatFloat(vertex.normal.x(), options.precision)
                   << " " << formatFloat(vertex.normal.y(), options.precision)
                   << " " << formatFloat(vertex.normal.z(), options.precision) << Qt::endl;
        }
    }
    
    // Write texture coordinates
    if (options.exportTexCoords) {
        for (const auto& vertex : vertices) {
            stream << "vt " << formatFloat(vertex.texCoord.x(), options.precision)
                   << " " << formatFloat(vertex.texCoord.y(), options.precision) << Qt::endl;
        }
    }
    
    stream << Qt::endl;
    
    // Write faces
    for (size_t i = 0; i < indices.size(); i += 3) {
        stream << "f";
        for (int j = 0; j < 3; ++j) {
            int idx = indices[i + j] + 1; // OBJ indices are 1-based
            stream << " " << idx;
            
            if (options.exportTexCoords) {
                stream << "/" << idx;
            }
            
            if (options.exportNormals) {
                if (!options.exportTexCoords) {
                    stream << "/";
                }
                stream << "/" << idx;
            }
        }
        stream << Qt::endl;
    }
    
    return true;
}

bool OBJFileHandler::exportToStream(QTextStream& stream, HalfEdgeMeshPtr mesh, const ExportOptions& options) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportToStream(stream, faceVertexMesh, options);
}

bool OBJFileHandler::parseVertex(const QStringList& tokens, QVector3D& vertex) {
    if (tokens.size() < 4) {
        return false;
    }
    
    bool ok;
    vertex.setX(tokens[1].toFloat(&ok));
    if (!ok) return false;
    
    vertex.setY(tokens[2].toFloat(&ok));
    if (!ok) return false;
    
    vertex.setZ(tokens[3].toFloat(&ok));
    if (!ok) return false;
    
    return true;
}

bool OBJFileHandler::parseNormal(const QStringList& tokens, QVector3D& normal) {
    return parseVertex(tokens, normal); // Same format as vertex
}

bool OBJFileHandler::parseTexCoord(const QStringList& tokens, QVector2D& texCoord) {
    if (tokens.size() < 3) {
        return false;
    }
    
    bool ok;
    texCoord.setX(tokens[1].toFloat(&ok));
    if (!ok) return false;
    
    texCoord.setY(tokens[2].toFloat(&ok));
    if (!ok) return false;
    
    return true;
}

bool OBJFileHandler::parseFace(const QStringList& tokens, std::vector<int>& vertexIndices,
                              std::vector<int>& texCoordIndices, std::vector<int>& normalIndices) {
    if (tokens.size() < 4) {
        return false;
    }
    
    for (int i = 1; i < tokens.size(); ++i) {
        const QString& token = tokens[i];
        QStringList parts = token.split('/');
        
        if (parts.isEmpty()) {
            return false;
        }
        
        // Vertex index (required)
        bool ok;
        int vertexIndex = parts[0].toInt(&ok) - 1; // Convert to 0-based
        if (!ok || vertexIndex < 0) {
            return false;
        }
        vertexIndices.push_back(vertexIndex);
        
        // Texture coordinate index (optional)
        int texCoordIndex = -1;
        if (parts.size() > 1 && !parts[1].isEmpty()) {
            texCoordIndex = parts[1].toInt(&ok) - 1; // Convert to 0-based
            if (!ok || texCoordIndex < 0) {
                texCoordIndex = -1;
            }
        }
        texCoordIndices.push_back(texCoordIndex);
        
        // Normal index (optional)
        int normalIndex = -1;
        if (parts.size() > 2 && !parts[2].isEmpty()) {
            normalIndex = parts[2].toInt(&ok) - 1; // Convert to 0-based
            if (!ok || normalIndex < 0) {
                normalIndex = -1;
            }
        }
        normalIndices.push_back(normalIndex);
    }
    
    return true;
}

MeshPtr OBJFileHandler::buildMesh(const std::vector<QVector3D>& vertices,
                                 const std::vector<QVector3D>& normals,
                                 const std::vector<QVector2D>& texCoords,
                                 const std::vector<std::vector<int>>& faces,
                                 const std::vector<std::vector<int>>& texCoordFaces,
                                 const std::vector<std::vector<int>>& normalFaces,
                                 const ImportOptions& options) {
    
    auto mesh = std::make_shared<Mesh>();
    
    std::vector<Vertex> meshVertices;
    std::vector<unsigned int> meshIndices;
    
    // Convert faces to triangles and build vertex list
    for (size_t faceIdx = 0; faceIdx < faces.size(); ++faceIdx) {
        const auto& face = faces[faceIdx];
        const auto& texCoordFace = (faceIdx < texCoordFaces.size()) ? texCoordFaces[faceIdx] : std::vector<int>();
        const auto& normalFace = (faceIdx < normalFaces.size()) ? normalFaces[faceIdx] : std::vector<int>();
        
        if (face.size() < 3) {
            continue; // Skip invalid faces
        }
        
        // Triangulate face (fan triangulation)
        for (size_t i = 1; i < face.size() - 1; ++i) {
            // Create triangle: vertex[0], vertex[i], vertex[i+1]
            std::vector<int> triangleIndices = {0, static_cast<int>(i), static_cast<int>(i + 1)};
            
            for (int triIdx : triangleIndices) {
                Vertex vertex;
                
                // Position
                int vertexIndex = face[triIdx];
                if (vertexIndex >= 0 && vertexIndex < vertices.size()) {
                    vertex.position = vertices[vertexIndex];
                } else {
                    qWarning() << "Invalid vertex index:" << vertexIndex;
                    continue;
                }
                
                // Normal
                if (!normalFace.empty() && triIdx < normalFace.size()) {
                    int normalIndex = normalFace[triIdx];
                    if (normalIndex >= 0 && normalIndex < normals.size()) {
                        vertex.normal = normals[normalIndex];
                    }
                } else {
                    vertex.normal = QVector3D(0, 1, 0); // Default normal
                }
                
                // Texture coordinate
                if (!texCoordFace.empty() && triIdx < texCoordFace.size()) {
                    int texCoordIndex = texCoordFace[triIdx];
                    if (texCoordIndex >= 0 && texCoordIndex < texCoords.size()) {
                        vertex.texCoord = texCoords[texCoordIndex];
                    }
                } else {
                    vertex.texCoord = QVector2D(0, 0); // Default texture coordinate
                }
                
                meshVertices.push_back(vertex);
                meshIndices.push_back(static_cast<unsigned int>(meshVertices.size() - 1));
            }
        }
    }
    
    // Apply options
    if (options.mergeVertices) {
        mergeVertices(meshVertices, meshIndices, options.vertexMergeTolerance);
    }
    
    if (options.generateNormals) {
        generateNormals(meshVertices, meshIndices);
    }
    
    mesh->setVertices(meshVertices);
    mesh->setIndices(meshIndices);
    
    return mesh;
}

void OBJFileHandler::mergeVertices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, float tolerance) {
    std::vector<Vertex> mergedVertices;
    std::vector<unsigned int> vertexMap(vertices.size());
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        bool found = false;
        
        // Look for existing vertex within tolerance
        for (size_t j = 0; j < mergedVertices.size(); ++j) {
            const auto& v1 = vertices[i].position;
            const auto& v2 = mergedVertices[j].position;
            
            if ((v1 - v2).length() < tolerance) {
                vertexMap[i] = static_cast<unsigned int>(j);
                found = true;
                break;
            }
        }
        
        if (!found) {
            vertexMap[i] = static_cast<unsigned int>(mergedVertices.size());
            mergedVertices.push_back(vertices[i]);
        }
    }
    
    // Update indices
    for (auto& index : indices) {
        if (index < vertexMap.size()) {
            index = vertexMap[index];
        }
    }
    
    vertices = std::move(mergedVertices);
}

void OBJFileHandler::generateNormals(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    // Reset all normals
    for (auto& vertex : vertices) {
        vertex.normal = QVector3D(0, 0, 0);
    }
    
    // Accumulate face normals
    for (size_t i = 0; i < indices.size(); i += 3) {
        if (i + 2 >= indices.size()) break;
        
        const auto& v0 = vertices[indices[i]].position;
        const auto& v1 = vertices[indices[i + 1]].position;
        const auto& v2 = vertices[indices[i + 2]].position;
        
        QVector3D edge1 = v1 - v0;
        QVector3D edge2 = v2 - v0;
        QVector3D faceNormal = QVector3D::crossProduct(edge1, edge2).normalized();
        
        vertices[indices[i]].normal += faceNormal;
        vertices[indices[i + 1]].normal += faceNormal;
        vertices[indices[i + 2]].normal += faceNormal;
    }
    
    // Normalize vertex normals
    for (auto& vertex : vertices) {
        vertex.normal = vertex.normal.normalized();
    }
}

QString OBJFileHandler::formatFloat(float value, int precision) {
    return QString::number(value, 'f', precision);
}

int OBJFileHandler::parseIndex(const QString& indexStr, int maxIndex) {
    bool ok;
    int index = indexStr.toInt(&ok);
    if (!ok) {
        return -1;
    }
    
    // Convert to 0-based and handle negative indices
    if (index > 0) {
        return index - 1;
    } else if (index < 0) {
        return maxIndex + index;
    } else {
        return -1; // Invalid index
    }
}

// FileFormatManager Implementation
FileFormatManager::Format FileFormatManager::detectFormat(const QString& filePath) {
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix().toLower();
    
    if (extension == "obj") {
        return Format::OBJ;
    } else if (extension == "stl") {
        return Format::STL;
    } else if (extension == "ply") {
        return Format::PLY;
    }
    
    return Format::Unknown;
}

QStringList FileFormatManager::getSupportedImportExtensions() {
    return {"obj", "stl", "ply"};
}

QStringList FileFormatManager::getSupportedExportExtensions() {
    return {"obj", "stl", "ply"};
}

FileFormatManager::ImportResult FileFormatManager::importFile(const QString& filePath) {
    ImportResult result;
    result.filePath = filePath;
    result.detectedFormat = detectFormat(filePath);
    
    switch (result.detectedFormat) {
        case Format::OBJ: {
            auto objResult = OBJFileHandler::importFromFile(filePath, s_objImportOptions);
            result.success = objResult.success;
            result.errorMessage = objResult.errorMessage;
            result.meshes = objResult.meshes;
            result.meshNames = objResult.meshNames;
            break;
        }
        
        case Format::STL: {
            auto stlResult = STLFileHandler::importFromFile(filePath, s_stlImportOptions);
            result.success = stlResult.success;
            result.errorMessage = stlResult.errorMessage;
            if (stlResult.mesh) {
                result.meshes.push_back(stlResult.mesh);
                result.meshNames.push_back("STL_Mesh");
            }
            break;
        }
        
        case Format::PLY: {
            auto plyResult = PLYFileHandler::importFromFile(filePath, s_plyImportOptions);
            result.success = plyResult.success;
            result.errorMessage = plyResult.errorMessage;
            if (plyResult.mesh) {
                result.meshes.push_back(plyResult.mesh);
                result.meshNames.push_back("PLY_Mesh");
            }
            break;
        }
        
        default:
            result.errorMessage = "Unsupported file format";
            break;
    }
    
    return result;
}

bool FileFormatManager::exportFile(const QString& filePath, MeshPtr mesh) {
    Format format = detectFormat(filePath);
    
    switch (format) {
        case Format::OBJ:
            return OBJFileHandler::exportToFile(filePath, mesh, s_objExportOptions);
        case Format::STL:
            return STLFileHandler::exportToFile(filePath, mesh, s_stlExportOptions);
        case Format::PLY:
            return PLYFileHandler::exportToFile(filePath, mesh, s_plyExportOptions);
        default:
            return false;
    }
}

bool FileFormatManager::exportFile(const QString& filePath, HalfEdgeMeshPtr mesh) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportFile(filePath, faceVertexMesh);
}

bool FileFormatManager::exportFile(const QString& filePath, const std::vector<MeshPtr>& meshes) {
    Format format = detectFormat(filePath);
    
    switch (format) {
        case Format::OBJ:
            return OBJFileHandler::exportToFile(filePath, meshes, s_objExportOptions);
        default:
            // For other formats, combine meshes or export the first one
            if (!meshes.empty()) {
                return exportFile(filePath, meshes[0]);
            }
            return false;
    }
}

// Static member definitions
OBJFileHandler::ImportOptions FileFormatManager::s_objImportOptions;
OBJFileHandler::ExportOptions FileFormatManager::s_objExportOptions;
STLFileHandler::ImportOptions FileFormatManager::s_stlImportOptions;
STLFileHandler::ExportOptions FileFormatManager::s_stlExportOptions;
PLYFileHandler::ImportOptions FileFormatManager::s_plyImportOptions;
PLYFileHandler::ExportOptions FileFormatManager::s_plyExportOptions;

// STL and PLY handlers - simplified implementations
STLFileHandler::ImportResult STLFileHandler::importFromFile(const QString& filePath, const ImportOptions& options) {
    ImportResult result;
    result.errorMessage = "STL import not yet implemented";
    return result;
}

bool STLFileHandler::exportToFile(const QString& filePath, MeshPtr mesh, const ExportOptions& options) {
    Q_UNUSED(filePath)
    Q_UNUSED(mesh)
    Q_UNUSED(options)
    qDebug() << "STL export not yet implemented";
    return false;
}

bool STLFileHandler::exportToFile(const QString& filePath, HalfEdgeMeshPtr mesh, const ExportOptions& options) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportToFile(filePath, faceVertexMesh, options);
}

PLYFileHandler::ImportResult PLYFileHandler::importFromFile(const QString& filePath, const ImportOptions& options) {
    ImportResult result;
    Q_UNUSED(filePath)
    Q_UNUSED(options)
    result.errorMessage = "PLY import not yet implemented";
    return result;
}

bool PLYFileHandler::exportToFile(const QString& filePath, MeshPtr mesh, const ExportOptions& options) {
    Q_UNUSED(filePath)
    Q_UNUSED(mesh)
    Q_UNUSED(options)
    qDebug() << "PLY export not yet implemented";
    return false;
}

bool PLYFileHandler::exportToFile(const QString& filePath, HalfEdgeMeshPtr mesh, const ExportOptions& options) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportToFile(filePath, faceVertexMesh, options);
}
