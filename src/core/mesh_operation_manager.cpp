#include "core/entity.hpp"
#include "core/mesh.hpp"
#include "core/mesh_operation_manager.hpp"
#include "core/selection_manager.hpp"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

namespace rude {

::std::vector<SelectionData> MeshOperationManager::getValidSelection(ComponentType requiredType) const {
    ::std::vector<SelectionData> validSelections;
    if (!m_selectionManager) {
        // Cannot modify m_lastResult in const method - would need to be mutable or method non-const
        // m_lastResult = "No selection manager available.";
        return validSelections;
    }
    
    // TODO: Implement proper selection filtering when SelectionManager interface is complete
    // For now, use the requiredType parameter to avoid unreferenced parameter warning
    if (requiredType == ComponentType{}) {
        // Placeholder logic to use the parameter
    }
    
    return validSelections;
}

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
    auto mesh = getSelectedMesh();
    if (!mesh) {
        m_lastResult = "No mesh selected for extrusion.";
        return false;
    }
    
    if (distance <= 0.0f) {
        m_lastResult = "Extrusion distance must be positive.";
        return false;
    }
    
    // TODO: Implement actual face extrusion algorithm
    // For now, record the parameters to show they're being used
    glm::vec3 normalizedDir = glm::normalize(direction);
    m_lastResult = "Extrusion with distance " + std::to_string(distance) + 
                   " in direction (" + std::to_string(normalizedDir.x) + ", " + 
                   std::to_string(normalizedDir.y) + ", " + std::to_string(normalizedDir.z) + ") - Implementation pending.";
    return false;
}

bool MeshOperationManager::bevelEdges(float distance) {
    auto mesh = getSelectedMesh();
    if (!mesh) {
        m_lastResult = "No mesh selected for beveling.";
        return false;
    }
    
    if (distance <= 0.0f) {
        m_lastResult = "Bevel distance must be positive.";
        return false;
    }
    
    // TODO: Implement actual edge beveling algorithm
    // For now, record the parameter to show it's being used
    m_lastResult = "Bevel edges with distance " + std::to_string(distance) + " - Implementation pending.";
    return false;
}

bool MeshOperationManager::subdivideFaces(int levels) {
    auto mesh = getSelectedMesh();
    if (!mesh) {
        m_lastResult = "No mesh selected for subdivision.";
        return false;
    }
    
    if (levels <= 0) {
        m_lastResult = "Subdivision levels must be positive.";
        return false;
    }
    
    // TODO: Implement actual face subdivision algorithm (Catmull-Clark or Loop)
    // For now, record the parameter to show it's being used
    m_lastResult = "Subdivide faces with " + std::to_string(levels) + " levels - Implementation pending.";
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

::std::shared_ptr<Mesh> MeshOperationManager::getSelectedMesh() const {
    auto entity = getSelectedEntity();
    if (!entity)
        return nullptr;
    return entity->getMesh();
}

} // namespace rude