#pragma once

#include "base_toolbar.hpp"
#include "core/selection_manager.hpp"

/**
 * @brief Toolbar for selection mode and selection operations
 * 
 * Contains tools for:
 * - Selection modes (Entity, Vertex, Edge, Face)
 * - Selection modifiers (Add, Subtract, Intersect)
 * - Selection tools (Box select, Lasso select, etc.)
 * - Selection operations (Invert, Select All, etc.)
 */
class SelectionToolbar : public BaseToolbar
{
    Q_OBJECT

public:
    explicit SelectionToolbar(QMainWindow* parent = nullptr);
    
    // BaseToolbar interface
    QString getToolbarId() const override { return "selection_toolbar"; }
    QString getToolbarDescription() const override { return "Selection tools and modes"; }

    // Selection mode management
    void setSelectionMode(rude::ComponentType mode);
    rude::ComponentType getSelectionMode() const { return m_currentMode; }

signals:
    void selectionModeChanged(rude::ComponentType mode);
    void selectionModifierChanged(const QString& modifier);
    void selectionToolChanged(const QString& tool);
    void selectionOperationRequested(const QString& operation);

protected:
    void createActions() override;
    void setupLayout() override;
    void connectSignals() override;

private slots:
    void onSelectionModeChanged();
    void onSelectionModifierChanged();
    void onSelectionToolChanged();
    void onSelectionOperationRequested();

private:
    void createModeActions();
    void createModifierActions();
    void createToolActions();
    void createOperationActions();
    void updateModeButtons();
    
    rude::ComponentType m_currentMode;
    QString m_currentModifier;
    QString m_currentTool;
};
