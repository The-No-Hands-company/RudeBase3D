
#include "EditContext.h"
#include "GeometryConverter.h"
#include "SceneObject.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include <algorithm>

#include "core/mesh_elements.hpp"
#include "core/half_edge_mesh.hpp"
#include "core/half_edge_utils.hpp"

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

rude::HalfEdgeMeshPtr EditContext::getActiveHalfEdgeMesh() const {
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
                // TODO: Selection state is managed externally, not as vertex property
                // Future: Implement per-vertex selection state
                (void)vertex; // Acknowledge vertex until selection state implementation
            }
            break;
            
        case SelectionType::Edge:
            m_selectedEdges = m_workingHalfEdgeMesh->getEdges();
            for (auto& edge : m_selectedEdges) {
                // TODO: Selection state is managed externally, not as edge property
                // Future: Implement per-edge selection state
                (void)edge; // Acknowledge edge until selection state implementation
            }
            break;
            
        case SelectionType::Face:
            m_selectedFaces = m_workingHalfEdgeMesh->getFaces();
            for (auto& face : m_selectedFaces) {
                // TODO: Selection state is managed externally, not as face property
                // Future: Implement per-face selection state
                (void)face; // Acknowledge face until selection state implementation
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
                // Note: rude::Vertex doesn't have isSelected() method - using external selection tracking
                addVertexToSelection(vertex);
                // vertex->setSelected(!vertex->isSelected()); // Not available in rude:: API
            }
            break;
        }
        
        case SelectionType::Edge: {
            auto allEdges = m_workingHalfEdgeMesh->getEdges();
            clearSelectionInternal();
            
            for (auto& edge : allEdges) {
                // Note: rude::Edge doesn't have isSelected() method - using external selection tracking
                addEdgeToSelection(edge);
                // edge->setSelected(!edge->isSelected()); // Not available in rude:: API
            }
            break;
        }
        
        case SelectionType::Face: {
            auto allFaces = m_workingHalfEdgeMesh->getFaces();
            clearSelectionInternal();
            
            for (auto& face : allFaces) {
                // TODO: Fix selection logic to use external tracking (rude:: API has no isSelected/setSelected)
                // The rude:: API doesn't have built-in selection state, so we need to track it externally
                if (std::find(m_selectedFaces.begin(), m_selectedFaces.end(), face) == m_selectedFaces.end()) {
                    addFaceToSelection(face);
                } else {
                    removeFaceFromSelection(face);
                }
            }
            break;
        }
        
        default:
            break;
    }
    
    emit selectionChanged();
}

void EditContext::selectVertex(rude::VertexPtr vertex, bool addToSelection) {
    if (!vertex || m_currentSelectionType != SelectionType::Vertex) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    addVertexToSelection(vertex);
    // Note: Selection state is now managed externally, not as vertex property
    
    emit selectionChanged();
}

void EditContext::selectEdge(rude::EdgePtr edge, bool addToSelection) {
    if (!edge || m_currentSelectionType != SelectionType::Edge) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    addEdgeToSelection(edge);
    // Note: Selection state is now managed externally, not as edge property
    
    emit selectionChanged();
}

void EditContext::selectFace(rude::FacePtr face, bool addToSelection) {
    if (!face || m_currentSelectionType != SelectionType::Face) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    addFaceToSelection(face);
    // Note: Selection state is now managed externally, not as face property
    
    emit selectionChanged();
}

void EditContext::deselectVertex(rude::VertexPtr vertex) {
    if (!vertex) return;
    
    removeVertexFromSelection(vertex);
    // Note: Selection state is now managed externally, not as vertex property
    
    emit selectionChanged();
}

void EditContext::deselectEdge(rude::EdgePtr edge) {
    if (!edge) return;
    
    removeEdgeFromSelection(edge);
    // Note: Selection state is now managed externally, not as edge property
    
    emit selectionChanged();
}

void EditContext::deselectFace(rude::FacePtr face) {
    if (!face) return;
    
    removeFaceFromSelection(face);
    // Note: Selection state is now managed externally, not as face property
    
    emit selectionChanged();
}

void EditContext::boxSelect(const glm::vec3& min, const glm::vec3& max, bool addToSelection) {
    if (!m_workingHalfEdgeMesh) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    switch (m_currentSelectionType) {
        case SelectionType::Vertex: {
            for (auto& vertex : m_workingHalfEdgeMesh->getVertices()) {
                auto pos = vertex->position;
                if (pos.x >= min.x && pos.x <= max.x &&
                    pos.y >= min.y && pos.y <= max.y &&
                    pos.z >= min.z && pos.z <= max.z) {
                    addVertexToSelection(vertex);
                }
            }
            break;
        }
        
        case SelectionType::Edge: {
            for (auto& edge : m_workingHalfEdgeMesh->getEdges()) {
                // Calculate edge midpoint manually
                if (edge->halfEdge && edge->halfEdge->vertex && 
                    edge->halfEdge->twin && edge->halfEdge->twin->vertex) {
                    auto pos1 = edge->halfEdge->vertex->position;
                    auto pos2 = edge->halfEdge->twin->vertex->position;
                    glm::vec3 midpoint = (pos1 + pos2) * 0.5f;
                    
                    if (midpoint.x >= min.x && midpoint.x <= max.x &&
                        midpoint.y >= min.y && midpoint.y <= max.y &&
                        midpoint.z >= min.z && midpoint.z <= max.z) {
                        addEdgeToSelection(edge);
                        // Note: Selection state is now managed externally
                    }
                }
            }
            break;
        }
        
        case SelectionType::Face: {
            for (auto& face : m_workingHalfEdgeMesh->getFaces()) {
                // Calculate face centroid manually
                auto vertices = face->getVertices();
                if (!vertices.empty()) {
                    glm::vec3 centroid(0.0f);
                    for (auto vertex : vertices) {
                        if (vertex) {
                            centroid += vertex->position;
                        }
                    }
                    centroid /= static_cast<float>(vertices.size());
                    
                    if (centroid.x >= min.x && centroid.x <= max.x &&
                        centroid.y >= min.y && centroid.y <= max.y &&
                        centroid.z >= min.z && centroid.z <= max.z) {
                        addFaceToSelection(face);
                        // Note: Selection state is now managed externally
                    }
                }
            }
            break;
        }
        
        default:
            break;
    }
    
    emit selectionChanged();
}

void EditContext::selectEdgeLoop(rude::EdgePtr startEdge, bool addToSelection) {
    if (!startEdge || m_currentSelectionType != SelectionType::Edge) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    // Simple edge loop implementation
    // In a real implementation, this would traverse the mesh topology
    auto edgeLoop = rude::HalfEdgeUtils::getEdgeLoop(startEdge);
    
    for (auto& edge : edgeLoop) {
        addEdgeToSelection(edge);
        // Note: Selection state is now managed externally
    }
    
    emit selectionChanged();
}

void EditContext::selectEdgeRing(rude::EdgePtr startEdge, bool addToSelection) {
    if (!startEdge || m_currentSelectionType != SelectionType::Edge) return;
    
    if (!addToSelection) {
        clearSelectionInternal();
    }
    
    // Simple edge ring implementation
    auto edgeRing = rude::HalfEdgeUtils::getEdgeRing(startEdge);
    
    for (auto& edge : edgeRing) {
        addEdgeToSelection(edge);
        // Note: Selection state is now managed externally
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
    // Selection flags are no longer stored on elements, just clear our internal lists
    // Note: Selection state is now managed externally, not as element properties
    
    // Clear selection lists
    m_selectedVertices.clear();
    m_selectedEdges.clear();
    m_selectedFaces.clear();
}

void EditContext::addVertexToSelection(rude::VertexPtr vertex) {
    if (std::find(m_selectedVertices.begin(), m_selectedVertices.end(), vertex) == m_selectedVertices.end()) {
        m_selectedVertices.push_back(vertex);
    }
}

void EditContext::addEdgeToSelection(rude::EdgePtr edge) {
    if (std::find(m_selectedEdges.begin(), m_selectedEdges.end(), edge) == m_selectedEdges.end()) {
        m_selectedEdges.push_back(edge);
    }
}

void EditContext::addFaceToSelection(rude::FacePtr face) {
    if (std::find(m_selectedFaces.begin(), m_selectedFaces.end(), face) == m_selectedFaces.end()) {
        m_selectedFaces.push_back(face);
    }
}

void EditContext::removeVertexFromSelection(rude::VertexPtr vertex) {
    m_selectedVertices.erase(
        std::remove(m_selectedVertices.begin(), m_selectedVertices.end(), vertex),
        m_selectedVertices.end());
}

void EditContext::removeEdgeFromSelection(rude::EdgePtr edge) {
    m_selectedEdges.erase(
        std::remove(m_selectedEdges.begin(), m_selectedEdges.end(), edge),
        m_selectedEdges.end());
}

void EditContext::removeFaceFromSelection(rude::FacePtr face) {
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
