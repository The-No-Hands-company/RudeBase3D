#include "core/mesh_operation_manager.hpp"
#include "core/entity.hpp"
#include <QDebug>

namespace rude {

MeshOperationManager::MeshOperationManager() {
}

bool MeshOperationManager::executeOperation(const QString& operationName) {
    m_lastResult = "";
    m_lastOperationCount = 0;
    
    if (!m_selectionManager) {
        m_lastResult = "No selection manager available";
        return false;
    }
    
    if (operationName == "extrude_face") {
        return extrudeFaces();
    } else if (operationName == "bevel_edge") {
        return bevelEdges();
    } else if (operationName == "subdivide_face") {
        return subdivideFaces();
    } else {
        m_lastResult = "Unknown operation: " + operationName;
        return false;
    }
}

bool MeshOperationManager::extrudeFaces(float distance, const glm::vec3& direction) {
    auto faceSelections = getValidSelection(ComponentType::Face);
    if (faceSelections.empty()) {
        m_lastResult = "No faces selected for extrusion";
        return false;
    }
    
    auto mesh = getSelectedMesh();
    if (!mesh) {
        m_lastResult = "No valid mesh found";
        return false;
    }
    
    int successCount = 0;
    
    for (const auto& selection : faceSelections) {
        if (selection.face) {
            try {
                // Use face normal if direction is zero vector
                glm::vec3 extrudeDir = direction;
                if (glm::length(extrudeDir) < 0.001f) {
                    // Calculate face normal (basic implementation)
                    // For now, use default up direction
                    extrudeDir = glm::vec3(0.0f, 1.0f, 0.0f);
                }
                
                mesh->extrudeFace(selection.face, distance);
                successCount++;
                
                qDebug() << "Extruded face successfully";
            } catch (const std::exception& e) {
                qDebug() << "Error extruding face:" << e.what();
            }
        }
    }
    
    if (successCount > 0) {
        // Update mesh normals and GPU data
        mesh->updateNormals();
        m_lastOperationCount = successCount;
        m_lastResult = QString("Extruded %1 face(s) successfully").arg(successCount);
        return true;
    } else {
        m_lastResult = "Failed to extrude any faces";
        return false;
    }
}

bool MeshOperationManager::bevelEdges(float distance) {
    auto edgeSelections = getValidSelection(ComponentType::Edge);
    if (edgeSelections.empty()) {
        m_lastResult = "No edges selected for beveling";
        return false;
    }
    
    auto mesh = getSelectedMesh();
    if (!mesh) {
        m_lastResult = "No valid mesh found";
        return false;
    }
    
    int successCount = 0;
    
    for (const auto& selection : edgeSelections) {
        if (selection.edge) {
            try {
                mesh->bevelEdge(selection.edge, distance);
                successCount++;
                
                qDebug() << "Beveled edge successfully";
            } catch (const std::exception& e) {
                qDebug() << "Error beveling edge:" << e.what();
            }
        }
    }
    
    if (successCount > 0) {
        // Update mesh normals and GPU data
        mesh->updateNormals();
        m_lastOperationCount = successCount;
        m_lastResult = QString("Beveled %1 edge(s) successfully").arg(successCount);
        return true;
    } else {
        m_lastResult = "Failed to bevel any edges";
        return false;
    }
}

bool MeshOperationManager::subdivideFaces(int levels) {
    auto faceSelections = getValidSelection(ComponentType::Face);
    if (faceSelections.empty()) {
        m_lastResult = "No faces selected for subdivision";
        return false;
    }
    
    auto mesh = getSelectedMesh();
    if (!mesh) {
        m_lastResult = "No valid mesh found";
        return false;
    }
    
    int successCount = 0;
    
    for (const auto& selection : faceSelections) {
        if (selection.face) {
            try {
                // Apply subdivision multiple times based on levels
                for (int i = 0; i < levels; ++i) {
                    mesh->subdivideFace(selection.face);
                }
                successCount++;
                
                qDebug() << "Subdivided face successfully with" << levels << "levels";
            } catch (const std::exception& e) {
                qDebug() << "Error subdividing face:" << e.what();
            }
        }
    }
    
    if (successCount > 0) {
        // Update mesh normals and GPU data
        mesh->updateNormals();
        m_lastOperationCount = successCount;
        m_lastResult = QString("Subdivided %1 face(s) successfully").arg(successCount);
        return true;
    } else {
        m_lastResult = "Failed to subdivide any faces";
        return false;
    }
}

bool MeshOperationManager::canExtrudeFaces() const {
    return !getValidSelection(ComponentType::Face).empty();
}

bool MeshOperationManager::canBevelEdges() const {
    return !getValidSelection(ComponentType::Edge).empty();
}

bool MeshOperationManager::canSubdivideFaces() const {
    return !getValidSelection(ComponentType::Face).empty();
}

Entity* MeshOperationManager::getSelectedEntity() const {
    if (!m_selectionManager) return nullptr;
    
    const auto& selection = m_selectionManager->getSelection();
    if (selection.empty()) return nullptr;
    
    // Get entity from first selection item
    auto firstSelection = *selection.begin();
    return firstSelection.entity;
}

std::shared_ptr<Mesh> MeshOperationManager::getSelectedMesh() const {
    auto entity = getSelectedEntity();
    if (!entity) return nullptr;
    
    return entity->getMesh();
}

std::vector<SelectionData> MeshOperationManager::getValidSelection(ComponentType requiredType) const {
    std::vector<SelectionData> validSelections;
    
    if (!m_selectionManager) return validSelections;
    
    const auto& allSelections = m_selectionManager->getSelection();
    
    for (const auto& selection : allSelections) {
        if (selection.type == requiredType && selection.isValid()) {
            validSelections.push_back(selection);
        }
    }
    
    return validSelections;
}

} // namespace rude
