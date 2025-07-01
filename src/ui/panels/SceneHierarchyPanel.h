#pragma once

#include "Common.h"
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QBrush>
#include <memory>

// Forward declarations
class Scene;

class SceneHierarchyPanel : public QWidget {
    Q_OBJECT

public:
    explicit SceneHierarchyPanel(QWidget* parent = nullptr);
    ~SceneHierarchyPanel() = default;

    void setScene(std::shared_ptr<Scene> scene);
    std::shared_ptr<Scene> getScene() const { return m_scene; }
    
    void refreshHierarchy();
    void selectObject(SceneObjectPtr object);

signals:
    void objectSelected(SceneObjectPtr object);
    void objectRenamed(SceneObjectPtr object, const QString& newName);
    void objectDeleted(SceneObjectPtr object);

private slots:
    void onItemSelectionChanged();
    void onItemChanged(QTreeWidgetItem* item, int column);
    void onCustomContextMenu(const QPoint& pos);
    void onDeleteSelectedObject();
    void onRenameSelectedObject();
    void onDuplicateSelectedObject();

private:
    std::shared_ptr<Scene> m_scene;
    
    QVBoxLayout* m_layout;
    QTreeWidget* m_treeWidget;
    QHBoxLayout* m_buttonLayout;
    QPushButton* m_deleteButton;
    
    QMenu* m_contextMenu;
    QAction* m_deleteAction;
    QAction* m_renameAction;
    QAction* m_duplicateAction;
    
    void setupUI();
    void setupContextMenu();
    void connectSignals();
    
    QTreeWidgetItem* findItemByObjectId(unsigned int objectId);
    SceneObjectPtr getObjectFromItem(QTreeWidgetItem* item);
    void updateItemFromObject(QTreeWidgetItem* item, SceneObjectPtr object);
};
