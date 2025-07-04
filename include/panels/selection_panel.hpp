#pragma once

#include <QDockWidget>
#include <QToolButton>
#include <QButtonGroup>
#include <QLabel>
#include "core/selection_manager.hpp"

class SelectionPanel : public QDockWidget {
    Q_OBJECT

public:
    explicit SelectionPanel(QWidget* parent = nullptr);
    ~SelectionPanel() = default;

    void setSelectionManager(rude::SelectionManager* manager);
    void updateSelectionInfo();
    void setCurrentMode(rude::ComponentType mode);
    void setSelectionMode(rude::ComponentType mode) { setCurrentMode(mode); } // Alias for compatibility

signals:
    void selectionModeChanged(rude::ComponentType mode);
    void operationRequested(const QString& operation);
    void operationRequestedWithParams(const QString& operation, double parameter);

private slots:
    void onSelectionModeChanged(int buttonId);
    void onExtrudeFaceClicked();
    void onBevelEdgeClicked();
    void onSubdivideFaceClicked();
    void onInsertEdgeLoopClicked();

private:
    void setupUI();
    void updateSelectionDisplay();

    rude::SelectionManager* m_selectionManager = nullptr;
    
    // UI elements
    QButtonGroup* m_selectionModeGroup;
    QToolButton* m_entityModeButton;
    QToolButton* m_vertexModeButton;
    QToolButton* m_edgeModeButton;
    QToolButton* m_faceModeButton;
    
    QLabel* m_selectionInfoLabel;
    
    // Operation buttons
    QToolButton* m_extrudeFaceButton;
    QToolButton* m_bevelEdgeButton;
    QToolButton* m_subdivideFaceButton;
};
