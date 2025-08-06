#include "core/entity.hpp"
#include "core/mesh.hpp"
#include "core/mesh_operation_manager.hpp"
#include <string>
#include <iostream>

namespace rude {

MeshOperationManager::MeshOperationManager() {}

bool MeshOperationManager::executeOperation(const std::string& operationName) {
    m_lastResult = "";
    m_lastOperationCount = 0;
    // Example: handle "extrude" operation
    if (operationName == "extrude") {
        return extrudeFaces();
    } else if (operationName == "bevel") {
        return bevelEdges();
    } else if (operationName == "subdivide") {
        return subdivideFaces();
    }
    m_lastResult = "Unknown operation: " + operationName;
    return false;
}

bool MeshOperationManager::extrudeFaces(float distance, const glm::vec3& direction) {
    // Stub implementation
    m_lastResult = "ExtrudeFaces not implemented.";
    return false;
}

bool MeshOperationManager::bevelEdges(float distance) {
    // Stub implementation
    m_lastResult = "BevelEdges not implemented.";
    return false;
}

bool MeshOperationManager::subdivideFaces(int levels) {
    // Stub implementation
    m_lastResult = "SubdivideFaces not implemented.";
    return false;
}

bool MeshOperationManager::canExtrudeFaces() const {
    // Stub implementation
    return false;
}

bool MeshOperationManager::canBevelEdges() const {
    // Stub implementation
    return false;
}

bool MeshOperationManager::canSubdivideFaces() const {
    // Stub implementation
    return false;
}

Entity* MeshOperationManager::getSelectedEntity() const {
    // Stub implementation
    return nullptr;
}

std::shared_ptr<Mesh> MeshOperationManager::getSelectedMesh() const {
    auto entity = getSelectedEntity();
    if (!entity)
        return nullptr;
    return entity->getMesh();
}

std::vector<SelectionData> MeshOperationManager::getValidSelection(ComponentType requiredType) const {
    std::vector<SelectionData> validSelections;
    if (!m_selectionManager)
        return validSelections;
    const auto& allSelections = m_selectionManager->getSelection();
    for (const auto& selection : allSelections) {
        if (selection.type == requiredType && selection.isValid()) {
            validSelections.push_back(selection);
        }
    }
    return validSelections;
}

} // namespace rude