#pragma once

#include "Common.h"
#include "HalfEdgeMesh.h"
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
    const std::vector<HalfEdgeVertexPtr>& getSelectedVertices() const { return m_selectedVertices; }
    const std::vector<HalfEdgeEdgePtr>& getSelectedEdges() const { return m_selectedEdges; }
    const std::vector<HalfEdgeFacePtr>& getSelectedFaces() const { return m_selectedFaces; }
    
    void clearSelection();
    void selectAll();
    void invertSelection();
    
    // Element selection
    void selectVertex(HalfEdgeVertexPtr vertex, bool addToSelection = false);
    void selectEdge(HalfEdgeEdgePtr edge, bool addToSelection = false);
    void selectFace(HalfEdgeFacePtr face, bool addToSelection = false);
    
    void deselectVertex(HalfEdgeVertexPtr vertex);
    void deselectEdge(HalfEdgeEdgePtr edge);
    void deselectFace(HalfEdgeFacePtr face);
    
    // Box selection
    void boxSelect(const QVector3D& min, const QVector3D& max, bool addToSelection = false);
    
    // Loop/ring selection
    void selectEdgeLoop(HalfEdgeEdgePtr startEdge, bool addToSelection = false);
    void selectEdgeRing(HalfEdgeEdgePtr startEdge, bool addToSelection = false);

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
    std::vector<HalfEdgeVertexPtr> m_selectedVertices;
    std::vector<HalfEdgeEdgePtr> m_selectedEdges;
    std::vector<HalfEdgeFacePtr> m_selectedFaces;
    
    // Helper methods
    void updateWorkingMesh();
    void clearSelectionInternal();
    void addVertexToSelection(HalfEdgeVertexPtr vertex);
    void addEdgeToSelection(HalfEdgeEdgePtr edge);
    void addFaceToSelection(HalfEdgeFacePtr face);
    void removeVertexFromSelection(HalfEdgeVertexPtr vertex);
    void removeEdgeFromSelection(HalfEdgeEdgePtr edge);
    void removeFaceFromSelection(HalfEdgeFacePtr face);
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

/**
 * @brief Extrude Tool
 * Extrudes selected faces, edges, or vertices
 */
class ExtrudeTool : public ModelingTool {
    Q_OBJECT

public:
    explicit ExtrudeTool(EditContext* context, QObject* parent = nullptr);

    // Tool interface
    bool canExecute() const override;
    bool execute() override;
    
    QString getName() const override { return "Extrude"; }
    QString getDescription() const override { return "Extrude selected elements"; }

    // Tool parameters
    void setDistance(float distance) { m_distance = distance; }
    float getDistance() const { return m_distance; }

private:
    float m_distance = 1.0f;
    
    // Implementation methods
    bool extrudeFaces();
    bool extrudeEdges();
    bool extrudeVertices();
};

/**
 * @brief Inset Tool
 * Insets selected faces
 */
class InsetTool : public ModelingTool {
    Q_OBJECT

public:
    explicit InsetTool(EditContext* context, QObject* parent = nullptr);

    // Tool interface
    bool canExecute() const override;
    bool execute() override;
    
    QString getName() const override { return "Inset"; }
    QString getDescription() const override { return "Inset selected faces"; }

    // Tool parameters
    void setInsetAmount(float amount) { m_insetAmount = amount; }
    float getInsetAmount() const { return m_insetAmount; }

private:
    float m_insetAmount = 0.1f;
};

/**
 * @brief Bevel Tool
 * Bevels selected edges or vertices
 */
class BevelTool : public ModelingTool {
    Q_OBJECT

public:
    explicit BevelTool(EditContext* context, QObject* parent = nullptr);

    // Tool interface
    bool canExecute() const override;
    bool execute() override;
    
    QString getName() const override { return "Bevel"; }
    QString getDescription() const override { return "Bevel selected edges or vertices"; }

    // Tool parameters
    void setBevelAmount(float amount) { m_bevelAmount = amount; }
    float getBevelAmount() const { return m_bevelAmount; }
    
    void setSegments(int segments) { m_segments = segments; }
    int getSegments() const { return m_segments; }

private:
    float m_bevelAmount = 0.1f;
    int m_segments = 1;
};

/**
 * @brief Loop Cut Tool
 * Creates edge loops across faces
 */
class LoopCutTool : public ModelingTool {
    Q_OBJECT

public:
    explicit LoopCutTool(EditContext* context, QObject* parent = nullptr);

    // Tool interface
    bool canExecute() const override;
    bool execute() override;
    
    QString getName() const override { return "Loop Cut"; }
    QString getDescription() const override { return "Create edge loops"; }

    // Tool parameters
    void setNumberOfCuts(int cuts) { m_numberOfCuts = cuts; }
    int getNumberOfCuts() const { return m_numberOfCuts; }

private:
    int m_numberOfCuts = 1;
};

/**
 * @brief Modeling Tool Manager
 * Manages all modeling tools and provides a unified interface
 */
class ModelingToolManager : public QObject {
    Q_OBJECT

public:
    explicit ModelingToolManager(EditContext* context, QObject* parent = nullptr);
    ~ModelingToolManager() = default;

    // Tool management
    void registerTool(std::unique_ptr<ModelingTool> tool);
    ModelingTool* getTool(const QString& name) const;
    std::vector<ModelingTool*> getAvailableTools() const;
    
    // Tool execution
    bool executeTool(const QString& name);
    void cancelCurrentTool();
    
    // Current tool
    void setActiveTool(const QString& name);
    ModelingTool* getActiveTool() const { return m_activeTool; }

signals:
    void activeToolChanged(ModelingTool* tool);
    void toolExecuted(const QString& toolName);

private:
    EditContext* m_context;
    std::unordered_map<QString, std::unique_ptr<ModelingTool>> m_tools;
    ModelingTool* m_activeTool = nullptr;
    
    void setupDefaultTools();
};
