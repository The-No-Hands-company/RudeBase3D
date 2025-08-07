#include "FileFormatHandlers.h"
#include <glm/glm.hpp>
#include "GeometryConverter.h"
#include "core/mesh_elements.hpp"
#include "core/mesh.hpp"
#include "core/half_edge_mesh.hpp"
// Qt includes
#include <QFileInfo>
#include <QString>
#include <QTextStream>
#include <algorithm>
#include <unordered_map>

#include <fstream>

// OBJFileHandler Implementation
OBJFileHandler::ImportResult OBJFileHandler::importFromFile(const std::string& filePath, const ImportOptions& options) {
    ImportResult result;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        result.errorMessage = "Cannot open file: " + filePath;
        return result;
    }
    return importFromStream(file, options);
}

bool OBJFileHandler::exportToFile(const std::string& filePath, rude::MeshPtr mesh, const ExportOptions& options) {
    if (!mesh) {
        return false;
    }
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    return exportToStream(file, mesh, options);
}

bool OBJFileHandler::exportToFile(const std::string& filePath, rude::HalfEdgeMeshPtr mesh, const ExportOptions& options) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportToFile(filePath, faceVertexMesh, options);
}

bool OBJFileHandler::exportToFile(const std::string& filePath, const std::vector<rude::MeshPtr>& meshes, const ExportOptions& options) {
    if (meshes.empty()) {
        return false;
    }
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    // Write header
    file << "# Exported from RudeBase3D\n";
    file << "# Meshes: " << meshes.size() << "\n\n";
    int vertexOffset = 0;
    int normalOffset = 0;
    int texCoordOffset = 0;
    for (size_t i = 0; i < meshes.size(); ++i) {
        const auto& mesh = meshes[i];
        if (!mesh) continue;
        // Write group/object name
        file << "g mesh_" << i << "\n";
        file << "o mesh_" << i << "\n";
        const auto& vertices = mesh->getVertices();
        // Write vertices
        for (const auto& vertex : vertices) {
            file << "v " << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << "\n";
        }
        // Write normals
        if (options.exportNormals) {
            for (const auto& vertex : vertices) {
                file << "vn " << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << "\n";
            }
        }
        // Write texture coordinates
        if (options.exportTexCoords) {
            for (const auto& vertex : vertices) {
                file << "vt " << vertex.texCoord.x << " " << vertex.texCoord.y << "\n";
            }
        }
        // Write faces
        const auto& indices = mesh->getIndices();
        for (size_t j = 0; j < indices.size(); j += 3) {
            file << "f";
            for (int k = 0; k < 3; ++k) {
                int idx = indices[j + k] + vertexOffset + 1; // OBJ indices are 1-based
                file << " " << idx;
                if (options.exportTexCoords) {
                    file << "/" << (idx + texCoordOffset);
                }
                if (options.exportNormals) {
                    if (!options.exportTexCoords) {
                        file << "/";
                    }
                    file << "/" << (idx + normalOffset);
                }
            }
            file << "\n";
        }
        vertexOffset += static_cast<int>(vertices.size());
        if (options.exportNormals) normalOffset += static_cast<int>(vertices.size());
        if (options.exportTexCoords) texCoordOffset += static_cast<int>(vertices.size());
        file << "\n";
    }
    return true;
}

OBJFileHandler::ImportResult OBJFileHandler::importFromStream(std::istream& stream, const ImportOptions& options) {
    // Convert std::istream to QString for processing
    std::stringstream buffer;
    buffer << stream.rdbuf();
    QString content = QString::fromStdString(buffer.str());
    QTextStream qstream(&content, QIODevice::ReadOnly);
    return importFromStream(qstream, options);
}

bool OBJFileHandler::exportToStream(std::ostream& stream, rude::MeshPtr mesh, const ExportOptions& options) {
    QString output;
    QTextStream qstream(&output, QIODevice::WriteOnly);
    bool success = exportToStream(qstream, mesh, options);
    if (success) {
        stream << output.toStdString();
    }
    return success;
}

bool OBJFileHandler::exportToStream(std::ostream& stream, rude::HalfEdgeMeshPtr mesh, const ExportOptions& options) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportToStream(stream, faceVertexMesh, options);
}

OBJFileHandler::ImportResult OBJFileHandler::importFromStream(QTextStream& stream, const ImportOptions& options) {
    ImportResult result;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
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
            glm::vec3 vertex;
            // Convert QStringList to std::vector<std::string>
            std::vector<std::string> stdTokens;
            for (const QString& token : tokens) {
                stdTokens.push_back(token.toStdString());
            }
            if (parseVertex(stdTokens, vertex)) {
                vertices.push_back(vertex);
            } else {
                qWarning() << "Invalid vertex at line" << lineNumber;
            }
        }
        else if (command == "vn") {
            glm::vec3 normal;
            // Convert QStringList to std::vector<std::string>
            std::vector<std::string> stdTokens;
            for (const QString& token : tokens) {
                stdTokens.push_back(token.toStdString());
            }
            if (parseNormal(stdTokens, normal)) {
                normals.push_back(normal);
            } else {
                qWarning() << "Invalid normal at line" << lineNumber;
            }
        }
        else if (command == "vt") {
            glm::vec2 texCoord;
            // Convert QStringList to std::vector<std::string>
            std::vector<std::string> stdTokens;
            for (const QString& token : tokens) {
                stdTokens.push_back(token.toStdString());
            }
            if (parseTexCoord(stdTokens, texCoord)) {
                texCoords.push_back(texCoord);
            } else {
                qWarning() << "Invalid texture coordinate at line" << lineNumber;
            }
        }
        else if (command == "f") {
            std::vector<int> vertexIndices;
            std::vector<int> texCoordIndices;
            std::vector<int> normalIndices;
            
            // Convert QStringList to std::vector<std::string>
            std::vector<std::string> stdTokens;
            for (const QString& token : tokens) {
                stdTokens.push_back(token.toStdString());
            }
            if (parseFace(stdTokens, vertexIndices, texCoordIndices, normalIndices)) {
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

bool OBJFileHandler::exportToStream(QTextStream& stream, rude::MeshPtr mesh, const ExportOptions& options) {
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
        stream << "v " << formatFloat(vertex.position.x, options.precision)
               << " " << formatFloat(vertex.position.y, options.precision)
               << " " << formatFloat(vertex.position.z, options.precision) << Qt::endl;
    }
    
    // Write normals
    if (options.exportNormals) {
        for (const auto& vertex : vertices) {
            stream << "vn " << formatFloat(vertex.normal.x, options.precision)
                   << " " << formatFloat(vertex.normal.y, options.precision)
                   << " " << formatFloat(vertex.normal.z, options.precision) << Qt::endl;
        }
    }
    
    // Write texture coordinates
    if (options.exportTexCoords) {
        for (const auto& vertex : vertices) {
            stream << "vt " << formatFloat(vertex.texCoord.x, options.precision)
                   << " " << formatFloat(vertex.texCoord.y, options.precision) << Qt::endl;
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

bool OBJFileHandler::exportToStream(QTextStream& stream, rude::HalfEdgeMeshPtr mesh, const ExportOptions& options) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportToStream(stream, faceVertexMesh, options);
}

bool OBJFileHandler::parseVertex(const std::vector<std::string>& tokens, glm::vec3& vertex) {
    if (tokens.size() < 4) {
        return false;
    }
    
    try {
        vertex.x = std::stof(tokens[1]);
        vertex.y = std::stof(tokens[2]);
        vertex.z = std::stof(tokens[3]);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}
// (removed Qt version, see new version at end of file)

bool OBJFileHandler::parseNormal(const std::vector<std::string>& tokens, glm::vec3& normal) {
    return parseVertex(tokens, normal); // Same format as vertex
}
// (removed Qt version, see new version at end of file)

bool OBJFileHandler::parseTexCoord(const std::vector<std::string>& tokens, glm::vec2& texCoord) {
    if (tokens.size() < 3) {
        return false;
    }
    
    try {
        texCoord.x = std::stof(tokens[1]);
        texCoord.y = std::stof(tokens[2]);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}
// (removed Qt version, see new version at end of file)

bool OBJFileHandler::parseFace(const std::vector<std::string>& tokens, std::vector<int>& vertexIndices,
                              std::vector<int>& texCoordIndices, std::vector<int>& normalIndices) {
    if (tokens.size() < 4) {
        return false;
    }
    
    for (int i = 1; i < tokens.size(); ++i) {
        const std::string& token = tokens[i];
        QString qtoken = QString::fromStdString(token);
        QStringList parts = qtoken.split('/');
        
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
// (removed Qt version, see new version at end of file)

rude::MeshPtr OBJFileHandler::buildMesh(const std::vector<glm::vec3>& vertices,
                                 const std::vector<glm::vec3>& normals,
                                 const std::vector<glm::vec2>& texCoords,
                                 const std::vector<std::vector<int>>& faces,
                                 const std::vector<std::vector<int>>& texCoordFaces,
                                 const std::vector<std::vector<int>>& normalFaces,
                                 const ImportOptions& options) {
    
    auto mesh = std::make_shared<rude::Mesh>();
    
    std::vector<rude::Vertex> meshVertices;
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
                rude::Vertex vertex;


                // Position
                int vertexIndex = face[triIdx];
                if (vertexIndex >= 0 && vertexIndex < static_cast<int>(vertices.size())) {
                    vertex.position = vertices[vertexIndex];
                } else {
                    qWarning() << "Invalid vertex index:" << vertexIndex;
                    continue;
                }

                // Normal
                if (!normalFace.empty() && triIdx < static_cast<int>(normalFace.size())) {
                    int normalIndex = normalFace[triIdx];
                    if (normalIndex >= 0 && normalIndex < static_cast<int>(normals.size())) {
                        vertex.normal = normals[normalIndex];
                    } else {
                        vertex.normal = glm::vec3(0, 1, 0); // Default normal
                    }
                } else {
                    vertex.normal = glm::vec3(0, 1, 0); // Default normal
                }

                // Texture coordinate
                if (!texCoordFace.empty() && triIdx < static_cast<int>(texCoordFace.size())) {
                    int texCoordIndex = texCoordFace[triIdx];
                    if (texCoordIndex >= 0 && texCoordIndex < static_cast<int>(texCoords.size())) {
                        vertex.texCoord = texCoords[texCoordIndex];
                    } else {
                        vertex.texCoord = glm::vec2(0, 0); // Default texcoord
                    }
                } else {
                    vertex.texCoord = glm::vec2(0, 0); // Default texcoord
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
    
    mesh->setData(meshVertices, meshIndices);
    
    return mesh;
}

void OBJFileHandler::mergeVertices(std::vector<rude::Vertex>& vertices, std::vector<unsigned int>& indices, float tolerance) {
    std::vector<rude::Vertex> mergedVertices;
    std::vector<unsigned int> vertexMap(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i) {
        bool found = false;
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

    for (auto& index : indices) {
        if (index < vertexMap.size()) {
            index = vertexMap[index];
        }
    }

    vertices = std::move(mergedVertices);
}

void OBJFileHandler::generateNormals(std::vector<rude::Vertex>& vertices, const std::vector<unsigned int>& indices) {
    // Reset all normals
    for (auto& vertex : vertices) {
        vertex.normal = glm::vec3(0, 0, 0);
    }

    // Accumulate face normals
    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        const glm::vec3& v0 = vertices[indices[i]].position;
        const glm::vec3& v1 = vertices[indices[i + 1]].position;
        const glm::vec3& v2 = vertices[indices[i + 2]].position;

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

        vertices[indices[i]].normal += faceNormal;
        vertices[indices[i + 1]].normal += faceNormal;
        vertices[indices[i + 2]].normal += faceNormal;
    }

    // Normalize vertex normals
    for (auto& vertex : vertices) {
        vertex.normal = glm::normalize(vertex.normal);
    }
}

QString OBJFileHandler::formatFloat(float value, int precision) {
    return QString::number(value, 'f', precision);
}
// (removed Qt version, see new version at end of file)

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
// (removed Qt version, see new version at end of file)

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
            auto objResult = OBJFileHandler::importFromFile(filePath.toStdString(), s_objImportOptions);
            result.success = objResult.success;
            result.errorMessage = QString::fromStdString(objResult.errorMessage);
            result.meshes = objResult.meshes;
            // Convert std::vector<std::string> to std::vector<QString>
            result.meshNames.clear();
            for (const auto& name : objResult.meshNames) {
                result.meshNames.push_back(QString::fromStdString(name));
            }
            break;
        }
        
        case Format::STL: {
            auto stlResult = STLFileHandler::importFromFile(filePath.toStdString(), s_stlImportOptions);
            result.success = stlResult.success;
            result.errorMessage = QString::fromStdString(stlResult.errorMessage);
            if (stlResult.mesh) {
                result.meshes.push_back(stlResult.mesh);
                result.meshNames.push_back("STL_Mesh");
            }
            break;
        }
        
        case Format::PLY: {
            auto plyResult = PLYFileHandler::importFromFile(filePath.toStdString(), s_plyImportOptions);
            result.success = plyResult.success;
            result.errorMessage = QString::fromStdString(plyResult.errorMessage);
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

bool FileFormatManager::exportFile(const QString& filePath, rude::MeshPtr mesh) {
    Format format = detectFormat(filePath);
    
    switch (format) {
        case Format::OBJ:
            return OBJFileHandler::exportToFile(filePath.toStdString(), mesh, s_objExportOptions);
        case Format::STL:
            return STLFileHandler::exportToFile(filePath.toStdString(), mesh, s_stlExportOptions);
        case Format::PLY:
            return PLYFileHandler::exportToFile(filePath.toStdString(), mesh, s_plyExportOptions);
        default:
            return false;
    }
}

bool FileFormatManager::exportFile(const QString& filePath, rude::HalfEdgeMeshPtr mesh) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportFile(filePath, faceVertexMesh);
}

bool FileFormatManager::exportFile(const QString& filePath, const std::vector<rude::MeshPtr>& meshes) {
    Format format = detectFormat(filePath);
    
    switch (format) {
        case Format::OBJ:
            return OBJFileHandler::exportToFile(filePath.toStdString(), meshes, s_objExportOptions);
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
STLFileHandler::ImportResult STLFileHandler::importFromFile(const ::std::string& filePath, const ImportOptions& options) {
    ImportResult result;
    result.errorMessage = "STL import not yet implemented";
    // Parameters will be used when STL parsing is implemented
    (void)filePath; // Future: file path for STL reading
    (void)options;  // Future: import configuration options
    return result;
}

bool STLFileHandler::exportToFile(const ::std::string& filePath, rude::MeshPtr mesh, const ExportOptions& options) {
    // TODO: Implement STL export logic using std::ofstream
    // Placeholder implementation - parameters acknowledged for future use
    (void)filePath; // Future: output file path for STL writing
    (void)mesh;     // Future: mesh data to export as STL
    (void)options;  // Future: export configuration options
    return false;
}

bool STLFileHandler::exportToFile(const std::string& filePath, rude::HalfEdgeMeshPtr mesh, const ExportOptions& options) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportToFile(filePath, faceVertexMesh, options);
}

PLYFileHandler::ImportResult PLYFileHandler::importFromFile(const ::std::string& filePath, const ImportOptions& options) {
    ImportResult result;
    result.errorMessage = "PLY import not yet implemented";
    // Parameters acknowledged for future PLY parsing implementation
    (void)filePath; // Future: file path for PLY reading
    (void)options;  // Future: import configuration options
    return result;
}

bool PLYFileHandler::exportToFile(const ::std::string& filePath, rude::MeshPtr mesh, const ExportOptions& options) {
    // TODO: Implement PLY export logic using std::ofstream
    // Placeholder implementation - parameters acknowledged for future use
    (void)filePath; // Future: output file path for PLY writing
    (void)mesh;     // Future: mesh data to export as PLY
    (void)options;  // Future: export configuration options
    return false;
}

bool PLYFileHandler::exportToFile(const std::string& filePath, rude::HalfEdgeMeshPtr mesh, const ExportOptions& options) {
    auto faceVertexMesh = GeometryConverter::toFaceVertex(mesh);
    return exportToFile(filePath, faceVertexMesh, options);
}
