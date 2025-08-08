#pragma once

#include "base_toolbar.hpp"

/**
 * @brief Main toolbar with file operations and viewport controls
 * 
 * Contains essential application operations:
 * - File operations (New, Open, Save, Undo, Redo)
 * - View modes (Solid, Wireframe, Material, Rendered)
 * - Camera controls (Reset, Frame Selected/Scene, Grid)
 * - Primary viewport controls
 */
class MainToolbar : public BaseToolbar
{
    Q_OBJECT

public:
    explicit MainToolbar(QMainWindow* parent = nullptr);
    
    // BaseToolbar interface
    QString getToolbarId() const override { return "main_toolbar"; }
    QString getToolbarDescription() const override { return "File operations and viewport controls"; }

    // View mode management
    void setActiveViewMode(const QString& mode);
    QString getActiveViewMode() const { return m_activeViewMode; }

signals:
    void viewModeChanged(const QString& mode);
    void cameraResetRequested();
    void frameSelectedRequested();
    void frameSceneRequested();
    void gridToggleRequested();

protected:
    void createActions() override;
    void setupLayout() override;
    void connectSignals() override;

private slots:
    void onViewModeTriggered();
    void onCameraActionTriggered();

private:
    void createFileActions();
    void createViewActions();
    void createCameraActions();
    void updateViewModeStates();
    
    QString m_activeViewMode;
};
