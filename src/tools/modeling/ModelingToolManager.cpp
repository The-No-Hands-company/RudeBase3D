#include "ModelingToolManager.h"
#include "GeometryConverter.h"
#include <QDebug>
#include "core/mesh_elements.hpp"
#include "core/half_edge_mesh.hpp"

ModelingToolManager::ModelingToolManager(QObject* parent)
    : QObject(parent) {
    initializeTools();
}

void ModelingToolManager::setEditContext(EditContext* context) {
    if (m_editContext) {
        // Disconnect from previous context
        disconnect(m_editContext, nullptr, this, nullptr);
    }
    
    m_editContext = context;
    
    if (m_editContext) {
        // Connect to new context signals
        connect(m_editContext, &EditContext::selectionChanged,
                this, &ModelingToolManager::onSelectionChanged);
        connect(m_editContext, &EditContext::editModeChanged,
                this, &ModelingToolManager::onEditModeChanged);
        
        updateToolsWithContext();
    }
}

bool ModelingToolManager::executeExtrude(float distance) {
    if (!canExecuteTool(ToolType::Extrude)) {
        reportError("Extrude", "Cannot execute extrude operation");
        return false;
    }
    
    auto mesh = getCurrentMesh();
    if (!mesh) {
        reportError("Extrude", "No valid mesh available");
        return false;
    }
    
    m_extrudeTool->setMesh(mesh);
    
    if (!m_extrudeTool->beginExtrude()) {
        reportError("Extrude", "Failed to begin extrude operation");
        return false;
    }
    
    m_extrudeTool->updateExtrude(distance);
    m_extrudeTool->confirmExtrude();
    
    commitMeshChanges(mesh);
    reportSuccess("Extrude", QString("Extruded with distance: %1").arg(distance));
    emit toolExecuted(ToolType::Extrude, true);
    return true;
}

bool ModelingToolManager::executeInset(float amount) {
    if (!canExecuteTool(ToolType::Inset)) {
        reportError("Inset", "Cannot execute inset operation");
        return false;
    }
    
    auto mesh = getCurrentMesh();
    if (!mesh) {
        reportError("Inset", "No valid mesh available");
        return false;
    }
    
    m_insetTool->setMesh(mesh);
    
    if (m_editContext->getSelectionType() == SelectionType::Face) {
        auto selectedFaces = m_editContext->getSelectedFaces();
        if (selectedFaces.empty()) {
            reportError("Inset", "No faces selected");
            return false;
        }
        
        bool success = m_insetTool->insetFaces(selectedFaces, amount);
        if (success) {
            commitMeshChanges(mesh);
            reportSuccess("Inset", QString("Inset %1 faces with amount: %2")
                         .arg(selectedFaces.size()).arg(amount));
            emit toolExecuted(ToolType::Inset, true);
            return true;
        }
    }
    
    reportError("Inset", "Failed to execute inset operation");
    return false;
}

bool ModelingToolManager::executeLoopCut(int numCuts, float position) {
    if (!canExecuteTool(ToolType::LoopCut)) {
        reportError("Loop Cut", "Cannot execute loop cut operation");
        return false;
    }
    
    auto mesh = getCurrentMesh();
    if (!mesh) {
        reportError("Loop Cut", "No valid mesh available");
        return false;
    }
    
    m_loopCutTool->setMesh(mesh);
    
    if (m_editContext->getSelectionType() == SelectionType::Edge) {
        auto selectedEdges = m_editContext->getSelectedEdges();
        if (selectedEdges.empty()) {
            reportError("Loop Cut", "No edges selected");
            return false;
        }
        
        bool success = m_loopCutTool->createLoopCutAtPosition(selectedEdges[0], position, numCuts);
        if (success) {
            commitMeshChanges(mesh);
            reportSuccess("Loop Cut", QString("Created %1 loop cuts at position %2")
                         .arg(numCuts).arg(position));
            emit toolExecuted(ToolType::LoopCut, true);
            return true;
        }
    }
    
    reportError("Loop Cut", "Failed to execute loop cut operation");
    return false;
}

bool ModelingToolManager::executeSubdivision(int levels) {
    if (!canExecuteTool(ToolType::Subdivision)) {
        reportError("Subdivision", "Cannot execute subdivision operation");
        return false;
    }
    
    auto mesh = getCurrentMesh();
    if (!mesh) {
        reportError("Subdivision", "No valid mesh available");
        return false;
    }
    
    m_subdivisionTool->setMesh(mesh);
    
    auto subdividedMesh = m_subdivisionTool->subdivide(levels);
    if (subdividedMesh) {
        commitMeshChanges(subdividedMesh);
        reportSuccess("Subdivision", QString("Applied %1 levels of subdivision")
                     .arg(levels));
        emit toolExecuted(ToolType::Subdivision, true);
        return true;
    }
    
    reportError("Subdivision", "Failed to execute subdivision operation");
    return false;
}

bool ModelingToolManager::executeBevel(float amount, int segments) {
    // This would integrate with the BevelTool from EditContext when it's fully implemented
    qDebug() << "ModelingToolManager: Bevel tool integration pending";
    return false;
}

bool ModelingToolManager::executeKnifeCut(const glm::vec3& startPoint, const glm::vec3& endPoint) {
    qDebug() << "ModelingToolManager: Knife tool not yet implemented";
    Q_UNUSED(startPoint);
    Q_UNUSED(endPoint);
    return false;
}

bool ModelingToolManager::executeBridge(const std::vector<HalfEdgeEdgePtr>& edges1, 
                                       const std::vector<HalfEdgeEdgePtr>& edges2) {
    qDebug() << "ModelingToolManager: Bridge tool not yet implemented";
    Q_UNUSED(edges1);
    Q_UNUSED(edges2);
    return false;
}

bool ModelingToolManager::extrudeSelection(float distance) {
    return executeExtrude(distance);
}

bool ModelingToolManager::insetSelection(float amount) {
    return executeInset(amount);
}

bool ModelingToolManager::subdivideSelection(int levels) {
    return executeSubdivision(levels);
}

bool ModelingToolManager::dissolveSelection() {
    qDebug() << "ModelingToolManager: Dissolve operation not yet implemented";
    return false;
}

bool ModelingToolManager::mergeSelection() {
    qDebug() << "ModelingToolManager: Merge operation not yet implemented";
    return false;
}

bool ModelingToolManager::canExecuteTool(ToolType tool) const {
    if (!m_editContext || !m_editContext->canEdit()) {
        return false;
    }
    
    return validateMeshForTool(tool);
}

QString ModelingToolManager::getToolStatusMessage(ToolType tool) const {
    if (!m_editContext) {
        return "No edit context available";
    }
    
    if (!m_editContext->canEdit()) {
        return "Not in edit mode";
    }
    
    auto mesh = getCurrentMesh();
    if (!mesh) {
        return "No valid mesh selected";
    }
    
    switch (tool) {
        case ToolType::Extrude:
            return validateExtrudeOperation() ? "Ready to extrude" : "Invalid selection for extrude";
            
        case ToolType::Inset:
            return validateInsetOperation() ? "Ready to inset" : "Invalid selection for inset";
            
        case ToolType::LoopCut:
            return validateLoopCutOperation() ? "Ready for loop cut" : "Invalid selection for loop cut";
            
        case ToolType::Subdivision:
            return validateSubdivisionOperation() ? "Ready to subdivide" : "Invalid mesh for subdivision";
            
        default:
            return "Tool not implemented";
    }
}

bool ModelingToolManager::triangulate() {
    auto mesh = getCurrentMesh();
    if (!mesh) return false;
    
    // Simple triangulation - convert quads to triangles
    auto faces = mesh->getFaces();
    bool modified = false;
    
    for (auto face : faces) {
        auto vertices = face->getVertices();
        if (vertices.size() == 4) {
            // Split quad into two triangles
            // This would need proper implementation
            qDebug() << "ModelingToolManager: Triangulation needs full implementation";
            modified = true;
        }
    }
    
    if (modified) {
        commitMeshChanges(mesh);
        emit meshModified();
    }
    
    return modified;
}

bool ModelingToolManager::quadrangulateMesh() {
    qDebug() << "ModelingToolManager: Quadrangulation not yet implemented";
    return false;
}

bool ModelingToolManager::cleanupMesh() {
    auto mesh = getCurrentMesh();
    if (!mesh) return false;
    
    // Basic mesh cleanup - remove degenerate elements
    bool modified = false;
    
    // Remove faces with less than 3 vertices
    auto faces = mesh->getFaces();
    for (auto face : faces) {
        if (face->getVertices().size() < 3) {
            mesh->removeFace(face);
            modified = true;
        }
    }
    
    if (modified) {
        commitMeshChanges(mesh);
        reportSuccess("Cleanup", "Removed degenerate faces");
        emit meshModified();
    }
    
    return modified;
}

bool ModelingToolManager::optimizeMesh() {
    qDebug() << "ModelingToolManager: Mesh optimization not yet implemented";
    return false;
}

void ModelingToolManager::onExtrudeRequested() {
    executeExtrude();
}

void ModelingToolManager::onInsetRequested() {
    executeInset();
}

void ModelingToolManager::onLoopCutRequested() {
    executeLoopCut();
}

void ModelingToolManager::onSubdivisionRequested() {
    executeSubdivision();
}

void ModelingToolManager::onBevelRequested() {
    executeBevel();
}

void ModelingToolManager::onUndoRequested() {
    // Integrate with undo system
    qDebug() << "ModelingToolManager: Undo integration pending";
}

void ModelingToolManager::onRedoRequested() {
    // Integrate with redo system
    qDebug() << "ModelingToolManager: Redo integration pending";
}

void ModelingToolManager::onSelectionChanged() {
    // Update tool availability based on selection
    updateToolsWithContext();
}

void ModelingToolManager::onEditModeChanged() {
    // Update tools when edit mode changes
    updateToolsWithContext();
}

void ModelingToolManager::initializeTools() {
    m_extrudeTool = std::make_unique<ExtrudeTool>();
    m_insetTool = std::make_unique<InsetTool>();
    m_loopCutTool = std::make_unique<LoopCutTool>();
    m_subdivisionTool = std::make_unique<SubdivisionTool>();
    
    m_toolsInitialized = true;
}

void ModelingToolManager::updateToolsWithContext() {
    if (!m_toolsInitialized || !m_editContext) {
        return;
    }
    
    auto mesh = getCurrentMesh();
    if (mesh) {
        m_extrudeTool->setMesh(mesh);
        m_insetTool->setMesh(mesh);
        m_loopCutTool->setMesh(mesh);
        m_subdivisionTool->setMesh(mesh);
    }
}

bool ModelingToolManager::validateMeshForTool(ToolType tool) const {
    auto mesh = getCurrentMesh();
    if (!mesh || mesh->isEmpty()) {
        return false;
    }
    
    switch (tool) {
        case ToolType::Extrude:
            return validateExtrudeOperation();
        case ToolType::Inset:
            return validateInsetOperation();
        case ToolType::LoopCut:
            return validateLoopCutOperation();
        case ToolType::Subdivision:
            return validateSubdivisionOperation();
        default:
            return false;
    }
}

rude::HalfEdgeMeshPtr ModelingToolManager::getCurrentMesh() const {
    if (!m_editContext) return nullptr;
    return m_editContext->getActiveHalfEdgeMesh();
}

void ModelingToolManager::commitMeshChanges(rude::HalfEdgeMeshPtr modifiedMesh) {
    if (!m_editContext || !modifiedMesh) return;
    
    // The EditContext handles committing changes back to the scene object
    m_editContext->commitChangesToMesh();
    emit meshModified();
}

void ModelingToolManager::reportError(const QString& operation, const QString& details) {
    QString message = QString("Error in %1: %2").arg(operation, details);
    qWarning() << message;
    emit errorOccurred(message);
    emit operationCompleted(operation, false);
}

void ModelingToolManager::reportSuccess(const QString& operation, const QString& details) {
    QString message = details.isEmpty() ? QString("%1 completed successfully").arg(operation) 
                                       : QString("%1: %2").arg(operation, details);
    qDebug() << message;
    emit operationCompleted(operation, true);
}

bool ModelingToolManager::validateExtrudeOperation() const {
    if (!m_editContext) return false;
    
    auto selectionType = m_editContext->getSelectionType();
    return (selectionType == SelectionType::Face && !m_editContext->getSelectedFaces().empty()) ||
           (selectionType == SelectionType::Edge && !m_editContext->getSelectedEdges().empty()) ||
           (selectionType == SelectionType::Vertex && !m_editContext->getSelectedVertices().empty());
}

bool ModelingToolManager::validateInsetOperation() const {
    if (!m_editContext) return false;
    
    auto selectionType = m_editContext->getSelectionType();
    return selectionType == SelectionType::Face && !m_editContext->getSelectedFaces().empty();
}

bool ModelingToolManager::validateLoopCutOperation() const {
    if (!m_editContext) return false;
    
    auto selectionType = m_editContext->getSelectionType();
    return selectionType == SelectionType::Edge && !m_editContext->getSelectedEdges().empty();
}

bool ModelingToolManager::validateSubdivisionOperation() const {
    auto mesh = getCurrentMesh();
    return mesh && !mesh->isEmpty() && m_subdivisionTool->canSubdivide();
}
