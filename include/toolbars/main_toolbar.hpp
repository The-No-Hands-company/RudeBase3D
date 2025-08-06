#pragma once

#include "base_toolbar.hpp"

/**
 * @brief Main toolbar with primary application tools
 * 
 * Contains the most commonly used tools:
 * - Selection tools
 * - Transform tools (Move, Rotate, Scale)
 * - View controls
 * - Basic operations
 */
class MainToolbar : public BaseToolbar
{
    Q_OBJECT

public:
    explicit MainToolbar(QMainWindow* parent = nullptr);
    
    // BaseToolbar interface
    QString getToolbarId() const override { return "main_toolbar"; }
    QString getToolbarDescription() const override { return "Main application tools"; }

    // Tool state management
    void setActiveSelectionTool(const QString& tool);
    void setActiveTransformTool(const QString& tool);
    QString getActiveSelectionTool() const { return m_activeSelectionTool; }
    QString getActiveTransformTool() const { return m_activeTransformTool; }

signals:
    void selectionToolChanged(const QString& tool);
    void transformToolChanged(const QString& tool);
    void viewModeChanged(const QString& mode);

protected:
    void createActions() override;
    void setupLayout() override;
    void connectSignals() override;

private slots:
    void onSelectionToolTriggered();
    void onTransformToolTriggered();
    void onViewModeTriggered();

private:
    void createSelectionActions();
    void createTransformActions();
    void createViewActions();
    void updateToolStates();
    
    QString m_activeSelectionTool;
    QString m_activeTransformTool;
    QString m_activeViewMode;
};
