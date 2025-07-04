#pragma once

#include "core/selection_manager.hpp"
#include "geometry/core/Mesh.h"
#include <QString>
#include <memory>

class Entity;

namespace rude {

class MeshOperationManager {
public:
    MeshOperationManager();
    ~MeshOperationManager() = default;
    
    void setSelectionManager(SelectionManager* manager) { m_selectionManager = manager; }
    
    // Main operation dispatcher
    bool executeOperation(const QString& operationName);
    
    // Specific operations
    bool extrudeFaces(float distance = 1.0f, const glm::vec3& direction = glm::vec3(0.0f));
    bool bevelEdges(float distance = 0.1f);
    bool subdivideFaces(int levels = 1);
    
    // Operation availability checks
    bool canExtrudeFaces() const;
    bool canBevelEdges() const;
    bool canSubdivideFaces() const;
    
    // Get operation results/info
    QString getLastOperationResult() const { return m_lastResult; }
    int getLastOperationCount() const { return m_lastOperationCount; }
    
private:
    SelectionManager* m_selectionManager = nullptr;
    QString m_lastResult;
    int m_lastOperationCount = 0;
    
    // Helper methods
    Entity* getSelectedEntity() const;
    std::shared_ptr<Mesh> getSelectedMesh() const;
    std::vector<SelectionData> getValidSelection(ComponentType requiredType) const;
};

} // namespace rude
