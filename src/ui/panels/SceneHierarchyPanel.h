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
namespace rude { 
    class Scene; 
}
class Entity;

class SceneHierarchyPanel : public QWidget {
    Q_OBJECT

public:
    explicit SceneHierarchyPanel(QWidget* parent = nullptr);
    ~SceneHierarchyPanel() = default;

    void setScene(std::shared_ptr<rude::Scene> scene);
    std::shared_ptr<rude::Scene> getScene() const { return m_scene; }
    
    void refreshHierarchy();

signals:
    void entitySelected(Entity* entity);
    void entityRenamed(Entity* entity, const QString& newName);
    void entityDeleted(Entity* entity);

private slots:
    void onItemSelectionChanged();
    void onItemChanged(QTreeWidgetItem* item, int column);
    void onCustomContextMenu(const QPoint& pos);
    void onDeleteSelectedObject();
    void onRenameSelectedObject();
    void onDuplicateSelectedObject();

private:
    std::shared_ptr<rude::Scene> m_scene;
    
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
    
    QTreeWidgetItem* findItemByEntityId(unsigned int entityId);
    Entity* getEntityFromItem(QTreeWidgetItem* item);
    void updateItemFromEntity(QTreeWidgetItem* item, Entity* entity);
};
