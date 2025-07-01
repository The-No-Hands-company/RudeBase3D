#pragma once

#include "Common.h"
#include "HalfEdgeMesh.h"
#include "EditContext.h"

// Modeling tools
#include "ExtrudeTool.h"
#include "InsetTool.h"
#include "LoopCutTool.h"
#include "SubdivisionTool.h"

#include <QObject>
#include <QString>
#include <memory>
#include <unordered_map>

/**
 * @brief Manager for all modeling tools in RudeBase3D
 * 
 * The ModelingToolManager provides a centralized interface for accessing
 * and executing modeling operations on half-edge meshes. It integrates
 * with the EditContext to provide proper selection handling and undo/redo.
 */
class ModelingToolManager : public QObject {
    Q_OBJECT

public:
    enum class ToolType {
        Extrude,
        Inset,
        LoopCut,
        Subdivision,
        Bevel,
        Knife,
        Bridge,
        Merge,
        Dissolve
    };

    explicit ModelingToolManager(QObject* parent = nullptr);
    ~ModelingToolManager() = default;

    // Tool management
    void setEditContext(EditContext* context);
    EditContext* getEditContext() const { return m_editContext; }
    
    // Core tool access
    ExtrudeTool* getExtrudeTool() const { return m_extrudeTool.get(); }
    InsetTool* getInsetTool() const { return m_insetTool.get(); }
    LoopCutTool* getLoopCutTool() const { return m_loopCutTool.get(); }
    SubdivisionTool* getSubdivisionTool() const { return m_subdivisionTool.get(); }
    
    // Tool execution
    bool executeExtrude(float distance = 1.0f);
    bool executeInset(float amount = 0.2f);
    bool executeLoopCut(int numCuts = 1, float position = 0.5f);
    bool executeSubdivision(int levels = 1);
    
    // Advanced operations
    bool executeBevel(float amount = 0.1f, int segments = 1);
    bool executeKnifeCut(const QVector3D& startPoint, const QVector3D& endPoint);
    bool executeBridge(const std::vector<HalfEdgeEdgePtr>& edges1, 
                      const std::vector<HalfEdgeEdgePtr>& edges2);
    
    // Selection-based operations
    bool extrudeSelection(float distance = 1.0f);
    bool insetSelection(float amount = 0.2f);
    bool subdivideSelection(int levels = 1);
    bool dissolveSelection();
    bool mergeSelection();
    
    // Tool validation
    bool canExecuteTool(ToolType tool) const;
    QString getToolStatusMessage(ToolType tool) const;
    
    // Mesh operations
    bool triangulate();
    bool quadrangulateMesh();
    bool cleanupMesh();
    bool optimizeMesh();

public slots:
    // Context-aware tool execution
    void onExtrudeRequested();
    void onInsetRequested();
    void onLoopCutRequested();
    void onSubdivisionRequested();
    void onBevelRequested();
    
    // Undo/Redo integration
    void onUndoRequested();
    void onRedoRequested();

signals:
    void toolExecuted(ToolType tool, bool success);
    void meshModified();
    void operationCompleted(const QString& operationName, bool success);
    void errorOccurred(const QString& errorMessage);

private slots:
    void onSelectionChanged();
    void onEditModeChanged();

private:
    // Core components
    EditContext* m_editContext = nullptr;
    
    // Modeling tools
    std::unique_ptr<ExtrudeTool> m_extrudeTool;
    std::unique_ptr<InsetTool> m_insetTool;
    std::unique_ptr<LoopCutTool> m_loopCutTool;
    std::unique_ptr<SubdivisionTool> m_subdivisionTool;
    
    // Tool state
    ToolType m_activeTool = ToolType::Extrude;
    bool m_toolsInitialized = false;
    
    // Internal methods
    void initializeTools();
    void updateToolsWithContext();
    bool validateMeshForTool(ToolType tool) const;
    
    // Mesh helpers
    HalfEdgeMeshPtr getCurrentMesh() const;
    void commitMeshChanges(HalfEdgeMeshPtr modifiedMesh);
    
    // Error handling
    void reportError(const QString& operation, const QString& details);
    void reportSuccess(const QString& operation, const QString& details = QString());
    
    // Tool-specific validation
    bool validateExtrudeOperation() const;
    bool validateInsetOperation() const;
    bool validateLoopCutOperation() const;
    bool validateSubdivisionOperation() const;
};
