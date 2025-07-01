#include "EditContext.h"
#include "GeometryConverter.h"
#include "SceneObject.h"
#include "Scene.h"
#include <QDebug>
#include <algorithm>

// EditContext Implementation
EditContext::EditContext(QObject* parent)
    : QObject(parent)
{
}

void EditContext::setEditMode(EditMode mode) {
    if (m_currentEditMode != mode) {
        m_currentEditMode = mode;
        
        if (mode == EditMode::Object) {
            // Clear mesh selection when switching to object mode
            clearSelection();
            m_workingHalfEdgeMesh.reset();
        } else if (mode == EditMode::Edit) {
            // Switch to edit mode - prepare half-edge mesh
            updateWorkingMesh();
        }
        
        emit editModeChanged(mode);
    }
}

void EditContext::setSelectionType(SelectionType type) {
    if (m_currentSelectionType != type) {
        // Clear current selection when changing selection type
        clearSelection();
        m_currentSelectionType = type;
        emit selectionTypeChanged(type);
    }
}

void EditContext::setActiveObject(SceneObjectPtr object) {
    auto currentObject = m_activeObject.lock();
    if (currentObject != object) {
        // Clear selection when changing objects
        clearSelection();
        
        m_activeObject = object;
        
        // Update working mesh if in edit mode
        if (m_currentEditMode == EditMode::Edit) {
            updateWorkingMesh();
        }
        
        emit activeObjectChanged(object);
    }
}

HalfEdgeMeshPtr EditContext::getActiveHalfEdgeMesh() const {
    return m_workingHalfEdgeMesh;
}

void EditContext::commitChangesToMesh() {
    auto activeObject = m_activeObject.lock();
    if (!activeObject || !m_workingHalfEdgeMesh) {
        return;
    }
    
    // Convert half-edge mesh back to face-vertex mesh
    auto newMesh = GeometryConverter::toFaceVertex(m_workingHalfEdgeMesh);
    if (newMesh) {
        activeObject->setMesh(newMesh);
        emit meshModified();
    }
}

void EditContext::clearSelection() {
    clearSelectionInternal();
    emit selectionChanged();
}

void EditContext::selectAll() {
    if (!m_workingHalfEdgeMesh) return;
    
    clearSelectionInternal();
    
    switch (m_currentSelectionType) {
        case SelectionType::Vertex:
            m_selectedVertices = m_workingHalfEdgeMesh->getVertices();
            for (auto& vertex : m_selectedVertices) {
                vertex->setSelected(true);
            }
            break;
            
        case SelectionType::Edge:
            m_selectedEdges = m_workingHalfEdgeMesh->getEdges();
            for (auto& edge : m_selectedEdges) {
                edge->setSelected(true);
            }
            break;
            
        case SelectionType::Face:
            m_selectedFaces = m_workingHalfEdgeMesh->getFaces();
            for (auto& face : m_selectedFaces) {
                face->setSelected(true);
            }
            break;
            
        default:
            break;
    }
    
    emit selectionChanged();
}

void EditContext::invertSelection() {
    if (!m_workingHalfEdgeMesh) return;
    
    switch (m_currentSelectionType) {
        case SelectionType::Vertex: {
            auto allVertices = m_workingHalfEdgeMesh->getVertices();
            clearSelectionInternal();
            
            for (auto& vertex : allVertices) {
                if (!vertex->isSelected()) {
                    addVertexToSelection(vertex);
                }
                vertex->setSelected(!vertex->isSelected());
            }
            break;
        }
        
        case SelectionType::Edge: {
            auto allEdges = m_workingHalfEdgeMesh->getEdges();
            clearSelectionInternal();
            
            for (auto& edge : allEdges) {
                if (!edge->isSelected()) {
                    addEdgeToSelection(edge);
                }
                edge->setSelected(!edge->isSelected());
            }
            break;
        }
        
        case SelectionType::Face: {
            auto allFaces = m_workingHalfEdgeMesh->getFaces();
            clearSelectionInternal();
            
            for (auto& face : allFaces) {
                if (!face->isSelected()) {
                    addFaceToSelection(face);
                }
                face->setSelected(!face->isSelected());
            }
            break;
        }
        
        default:
            break;
    }
    
    emit selectionChanged();
}

void EditContext::selectVertex(HalfEdgeVertexPtr vertex, bool addToSelection) {
    if (!vertex || m_currentSelectionType != SelectionType::Vertex) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    addVertexToSelection(vertex);
    vertex->setSelected(true);
    
    emit selectionChanged();
}

void EditContext::selectEdge(HalfEdgeEdgePtr edge, bool addToSelection) {
    if (!edge || m_currentSelectionType != SelectionType::Edge) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    addEdgeToSelection(edge);
    edge->setSelected(true);
    
    emit selectionChanged();
}

void EditContext::selectFace(HalfEdgeFacePtr face, bool addToSelection) {
    if (!face || m_currentSelectionType != SelectionType::Face) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    addFaceToSelection(face);
    face->setSelected(true);
    
    emit selectionChanged();
}

void EditContext::deselectVertex(HalfEdgeVertexPtr vertex) {
    if (!vertex) return;
    
    removeVertexFromSelection(vertex);
    vertex->setSelected(false);
    
    emit selectionChanged();
}

void EditContext::deselectEdge(HalfEdgeEdgePtr edge) {
    if (!edge) return;
    
    removeEdgeFromSelection(edge);
    edge->setSelected(false);
    
    emit selectionChanged();
}

void EditContext::deselectFace(HalfEdgeFacePtr face) {
    if (!face) return;
    
    removeFaceFromSelection(face);
    face->setSelected(false);
    
    emit selectionChanged();
}

void EditContext::boxSelect(const QVector3D& min, const QVector3D& max, bool addToSelection) {
    if (!m_workingHalfEdgeMesh) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    switch (m_currentSelectionType) {
        case SelectionType::Vertex: {
            for (auto& vertex : m_workingHalfEdgeMesh->getVertices()) {
                const auto& pos = vertex->getPosition();
                if (pos.x() >= min.x() && pos.x() <= max.x() &&
                    pos.y() >= min.y() && pos.y() <= max.y() &&
                    pos.z() >= min.z() && pos.z() <= max.z()) {
                    addVertexToSelection(vertex);
                    vertex->setSelected(true);
                }
            }
            break;
        }
        
        case SelectionType::Edge: {
            for (auto& edge : m_workingHalfEdgeMesh->getEdges()) {
                auto midpoint = edge->getMidpoint();
                if (midpoint.x() >= min.x() && midpoint.x() <= max.x() &&
                    midpoint.y() >= min.y() && midpoint.y() <= max.y() &&
                    midpoint.z() >= min.z() && midpoint.z() <= max.z()) {
                    addEdgeToSelection(edge);
                    edge->setSelected(true);
                }
            }
            break;
        }
        
        case SelectionType::Face: {
            for (auto& face : m_workingHalfEdgeMesh->getFaces()) {
                auto centroid = face->getCentroid();
                if (centroid.x() >= min.x() && centroid.x() <= max.x() &&
                    centroid.y() >= min.y() && centroid.y() <= max.y() &&
                    centroid.z() >= min.z() && centroid.z() <= max.z()) {
                    addFaceToSelection(face);
                    face->setSelected(true);
                }
            }
            break;
        }
        
        default:
            break;
    }
    
    emit selectionChanged();
}

void EditContext::selectEdgeLoop(HalfEdgeEdgePtr startEdge, bool addToSelection) {
    if (!startEdge || m_currentSelectionType != SelectionType::Edge) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    // Simple edge loop implementation
    // In a real implementation, this would traverse the mesh topology
    auto edgeLoop = HalfEdgeUtils::getEdgeLoop(startEdge);
    
    for (auto& edge : edgeLoop) {
        addEdgeToSelection(edge);
        edge->setSelected(true);
    }
    
    emit selectionChanged();
}

void EditContext::selectEdgeRing(HalfEdgeEdgePtr startEdge, bool addToSelection) {
    if (!startEdge || m_currentSelectionType != SelectionType::Edge) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    // Simple edge ring implementation
    auto edgeRing = HalfEdgeUtils::getEdgeRing(startEdge);
    
    for (auto& edge : edgeRing) {
        addEdgeToSelection(edge);
        edge->setSelected(true);
    }
    
    emit selectionChanged();
}

bool EditContext::canEdit() const {
    return m_currentEditMode == EditMode::Edit && 
           m_activeObject.lock() != nullptr && 
           m_workingHalfEdgeMesh != nullptr;
}

bool EditContext::hasSelection() const {
    return !m_selectedVertices.empty() || 
           !m_selectedEdges.empty() || 
           !m_selectedFaces.empty();
}

void EditContext::onActiveObjectDestroyed() {
    m_activeObject.reset();
    clearSelection();
    m_workingHalfEdgeMesh.reset();
    emit activeObjectChanged(nullptr);
}

void EditContext::updateWorkingMesh() {
    auto activeObject = m_activeObject.lock();
    if (!activeObject) {
        m_workingHalfEdgeMesh.reset();
        return;
    }
    
    auto mesh = activeObject->getMesh();
    if (mesh) {
        // Convert to half-edge mesh for editing
        m_workingHalfEdgeMesh = GeometryConverter::toHalfEdge(mesh);
    } else {
        m_workingHalfEdgeMesh.reset();
    }
}

void EditContext::clearSelectionInternal() {
    // Clear selection flags on elements
    for (auto& vertex : m_selectedVertices) {
        vertex->setSelected(false);
    }
    for (auto& edge : m_selectedEdges) {
        edge->setSelected(false);
    }
    for (auto& face : m_selectedFaces) {
        face->setSelected(false);
    }
    
    // Clear selection lists
    m_selectedVertices.clear();
    m_selectedEdges.clear();
    m_selectedFaces.clear();
}

void EditContext::addVertexToSelection(HalfEdgeVertexPtr vertex) {
    if (std::find(m_selectedVertices.begin(), m_selectedVertices.end(), vertex) == m_selectedVertices.end()) {
        m_selectedVertices.push_back(vertex);
    }
}

void EditContext::addEdgeToSelection(HalfEdgeEdgePtr edge) {
    if (std::find(m_selectedEdges.begin(), m_selectedEdges.end(), edge) == m_selectedEdges.end()) {
        m_selectedEdges.push_back(edge);
    }
}

void EditContext::addFaceToSelection(HalfEdgeFacePtr face) {
    if (std::find(m_selectedFaces.begin(), m_selectedFaces.end(), face) == m_selectedFaces.end()) {
        m_selectedFaces.push_back(face);
    }
}

void EditContext::removeVertexFromSelection(HalfEdgeVertexPtr vertex) {
    m_selectedVertices.erase(
        std::remove(m_selectedVertices.begin(), m_selectedVertices.end(), vertex),
        m_selectedVertices.end());
}

void EditContext::removeEdgeFromSelection(HalfEdgeEdgePtr edge) {
    m_selectedEdges.erase(
        std::remove(m_selectedEdges.begin(), m_selectedEdges.end(), edge),
        m_selectedEdges.end());
}

void EditContext::removeFaceFromSelection(HalfEdgeFacePtr face) {
    m_selectedFaces.erase(
        std::remove(m_selectedFaces.begin(), m_selectedFaces.end(), face),
        m_selectedFaces.end());
}

// ModelingTool Implementation
ModelingTool::ModelingTool(EditContext* context, QObject* parent)
    : QObject(parent)
    , m_context(context)
{
}

bool ModelingTool::hasValidSelection() const {
    return m_context && m_context->hasSelection();
}

void ModelingTool::emitMeshModified() {
    if (m_context) {
        m_context->commitChangesToMesh();
    }
}

// ExtrudeTool Implementation
ExtrudeTool::ExtrudeTool(EditContext* context, QObject* parent)
    : ModelingTool(context, parent)
{
}

bool ExtrudeTool::canExecute() const {
    return hasValidSelection() && m_context->canEdit();
}

bool ExtrudeTool::execute() {
    if (!canExecute()) {
        return false;
    }
    
    bool success = false;
    
    switch (m_context->getSelectionType()) {
        case SelectionType::Face:
            success = extrudeFaces();
            break;
        case SelectionType::Edge:
            success = extrudeEdges();
            break;
        case SelectionType::Vertex:
            success = extrudeVertices();
            break;
        default:
            break;
    }
    
    if (success) {
        emitMeshModified();
        emit toolExecuted();
    }
    
    return success;
}

bool ExtrudeTool::extrudeFaces() {
    auto selectedFaces = m_context->getSelectedFaces();
    if (selectedFaces.empty()) {
        return false;
    }
    
    auto mesh = m_context->getActiveHalfEdgeMesh();
    if (!mesh) {
        qWarning() << "No active mesh for extrusion";
        return false;
    }
    
    // Use HalfEdgeUtils for face extrusion
    auto extrudedFaces = HalfEdgeUtils::extrudeFaces(mesh, selectedFaces, m_distance);
    
    return !extrudedFaces.empty();
}

bool ExtrudeTool::extrudeEdges() {
    auto selectedEdges = m_context->getSelectedEdges();
    if (selectedEdges.empty()) {
        return false;
    }
    
    // Edge extrusion creates faces from edges
    // This would be implemented based on the specific requirements
    qDebug() << "Edge extrusion not yet implemented";
    return false;
}

bool ExtrudeTool::extrudeVertices() {
    auto selectedVertices = m_context->getSelectedVertices();
    if (selectedVertices.empty()) {
        return false;
    }
    
    // Vertex extrusion creates edges from vertices
    // This would be implemented based on the specific requirements
    qDebug() << "Vertex extrusion not yet implemented";
    return false;
}

// InsetTool Implementation
InsetTool::InsetTool(EditContext* context, QObject* parent)
    : ModelingTool(context, parent)
{
}

bool InsetTool::canExecute() const {
    return hasValidSelection() && 
           m_context->canEdit() && 
           m_context->getSelectionType() == SelectionType::Face;
}

bool InsetTool::execute() {
    if (!canExecute()) {
        return false;
    }
    
    auto selectedFaces = m_context->getSelectedFaces();
    auto mesh = m_context->getActiveHalfEdgeMesh();
    if (!mesh) {
        qWarning() << "No active mesh for inset operation";
        return false;
    }
    
    auto insetFaces = HalfEdgeUtils::insetFaces(mesh, selectedFaces, m_insetAmount);
    
    if (!insetFaces.empty()) {
        emitMeshModified();
        emit toolExecuted();
        return true;
    }
    
    return false;
}

// BevelTool Implementation
BevelTool::BevelTool(EditContext* context, QObject* parent)
    : ModelingTool(context, parent)
{
}

bool BevelTool::canExecute() const {
    return hasValidSelection() && 
           m_context->canEdit() && 
           (m_context->getSelectionType() == SelectionType::Edge ||
            m_context->getSelectionType() == SelectionType::Vertex);
}

bool BevelTool::execute() {
    if (!canExecute()) {
        return false;
    }
    
    auto mesh = m_context->getActiveHalfEdgeMesh();
    if (!mesh) {
        qWarning() << "No active mesh for bevel operation";
        return false;
    }
    
    if (m_context->getSelectionType() == SelectionType::Edge) {
        auto selectedEdges = m_context->getSelectedEdges();
        auto beveledEdges = HalfEdgeUtils::bevelEdges(mesh, selectedEdges, m_bevelAmount);
        
        if (!beveledEdges.empty()) {
            emitMeshModified();
            emit toolExecuted();
            return true;
        }
    }
    
    return false;
}

// LoopCutTool Implementation
LoopCutTool::LoopCutTool(EditContext* context, QObject* parent)
    : ModelingTool(context, parent)
{
}

bool LoopCutTool::canExecute() const {
    return m_context->canEdit() && 
           m_context->getSelectionType() == SelectionType::Edge &&
           !m_context->getSelectedEdges().empty();
}

bool LoopCutTool::execute() {
    if (!canExecute()) {
        return false;
    }
    
    // Loop cut implementation would go here
    qDebug() << "Loop cut tool not yet implemented";
    return false;
}

// ModelingToolManager Implementation
ModelingToolManager::ModelingToolManager(EditContext* context, QObject* parent)
    : QObject(parent)
    , m_context(context)
{
    setupDefaultTools();
}

void ModelingToolManager::registerTool(std::unique_ptr<ModelingTool> tool) {
    if (!tool) return;
    
    QString name = tool->getName();
    m_tools[name] = std::move(tool);
}

ModelingTool* ModelingToolManager::getTool(const QString& name) const {
    auto it = m_tools.find(name);
    return (it != m_tools.end()) ? it->second.get() : nullptr;
}

std::vector<ModelingTool*> ModelingToolManager::getAvailableTools() const {
    std::vector<ModelingTool*> tools;
    tools.reserve(m_tools.size());
    
    for (const auto& pair : m_tools) {
        tools.push_back(pair.second.get());
    }
    
    return tools;
}

bool ModelingToolManager::executeTool(const QString& name) {
    auto tool = getTool(name);
    if (!tool || !tool->canExecute()) {
        return false;
    }
    
    bool success = tool->execute();
    if (success) {
        emit toolExecuted(name);
    }
    
    return success;
}

void ModelingToolManager::cancelCurrentTool() {
    if (m_activeTool) {
        m_activeTool->cancel();
    }
}

void ModelingToolManager::setActiveTool(const QString& name) {
    auto tool = getTool(name);
    if (m_activeTool != tool) {
        m_activeTool = tool;
        emit activeToolChanged(tool);
    }
}

void ModelingToolManager::setupDefaultTools() {
    registerTool(std::make_unique<ExtrudeTool>(m_context, this));
    registerTool(std::make_unique<InsetTool>(m_context, this));
    registerTool(std::make_unique<BevelTool>(m_context, this));
    registerTool(std::make_unique<LoopCutTool>(m_context, this));
}
