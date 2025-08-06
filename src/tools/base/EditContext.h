#pragma once

#include "Common.h"

#include <QObject>
#include <memory>
#include <vector>

// Forward declarations
class Scene;
class SceneObject;
class Viewport3D;

/**
 * @brief Edit Context
 * Manages the current editing state and operations
 */
class EditContext : public QObject {
    Q_OBJECT

public:
    explicit EditContext(QObject* parent = nullptr);
    ~EditContext() = default;

    // Mode management
    void setEditMode(EditMode mode);
    EditMode getEditMode() const { return m_currentEditMode; }
    
    void setSelectionType(SelectionType type);
    SelectionType getSelectionType() const { return m_currentSelectionType; }

    // Object editing
    void setActiveObject(SceneObjectPtr object);
    SceneObjectPtr getActiveObject() const { return m_activeObject.lock(); }
    
    // Half-edge mesh access
    HalfEdgeMeshPtr getActiveHalfEdgeMesh() const;
    void commitChangesToMesh(); // Apply half-edge changes back to face-vertex mesh

    // Selection management
    const std::vector<rude::VertexPtr>& getSelectedVertices() const { return m_selectedVertices; }
    const std::vector<rude::EdgePtr>& getSelectedEdges() const { return m_selectedEdges; }
    const std::vector<rude::FacePtr>& getSelectedFaces() const { return m_selectedFaces; }
    
    void clearSelection();
    void selectAll();
    void invertSelection();
    
    // Element selection
    void selectVertex(rude::VertexPtr vertex, bool addToSelection = false);
    void selectEdge(rude::EdgePtr edge, bool addToSelection = false);
    void selectFace(rude::FacePtr face, bool addToSelection = false);
    
    void deselectVertex(rude::VertexPtr vertex);
    void deselectEdge(rude::EdgePtr edge);
    void deselectFace(rude::FacePtr face);
    
    // Box selection
    void boxSelect(const glm::vec3& min, const glm::vec3& max, bool addToSelection = false);
    
    // Loop/ring selection
    void selectEdgeLoop(rude::EdgePtr startEdge, bool addToSelection = false);
    void selectEdgeRing(rude::EdgePtr startEdge, bool addToSelection = false);

    // Validation
    bool canEdit() const;
    bool hasSelection() const;

signals:
    void editModeChanged(EditMode mode);
    void selectionTypeChanged(SelectionType type);
    void activeObjectChanged(SceneObjectPtr object);
    void selectionChanged();
    void meshModified();

private slots:
    void onActiveObjectDestroyed();

private:
    EditMode m_currentEditMode = EditMode::Object;
    SelectionType m_currentSelectionType = SelectionType::Vertex;
    
    std::weak_ptr<SceneObject> m_activeObject;
    HalfEdgeMeshPtr m_workingHalfEdgeMesh;
    
    // Current selection
    std::vector<rude::VertexPtr> m_selectedVertices;
    std::vector<rude::EdgePtr> m_selectedEdges;
    std::vector<rude::FacePtr> m_selectedFaces;
    
    // Helper methods
    void updateWorkingMesh();
    void clearSelectionInternal();
    void addVertexToSelection(rude::VertexPtr vertex);
    void addEdgeToSelection(rude::EdgePtr edge);
    void addFaceToSelection(rude::FacePtr face);
    void removeVertexFromSelection(rude::VertexPtr vertex);
    void removeEdgeFromSelection(rude::EdgePtr edge);
    void removeFaceFromSelection(rude::FacePtr face);
};

/**
 * @brief Modeling Tool Base Class
 * Base class for all modeling tools (extrude, bevel, etc.)
 */
class ModelingTool : public QObject {
    Q_OBJECT

public:
    explicit ModelingTool(EditContext* context, QObject* parent = nullptr);
    virtual ~ModelingTool() = default;

    // Tool interface
    virtual bool canExecute() const = 0;
    virtual bool execute() = 0;
    virtual void preview() {}
    virtual void cancel() {}
    
    // Tool properties
    virtual QString getName() const = 0;
    virtual QString getDescription() const = 0;

protected:
    EditContext* m_context;
    
    // Helper methods
    bool hasValidSelection() const;
    void emitMeshModified();

signals:
    void toolExecuted();
    void toolCancelled();
    void previewUpdated();
};

// Forward declarations for modeling tools
class ExtrudeTool;
class InsetTool;
class BevelTool;
class LoopCutTool;
class SubdivisionTool;
class ModelingToolManager;
