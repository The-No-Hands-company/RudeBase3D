#include "SceneHierarchyPanel.h"
#include "core/scene.hpp"
#include "core/entity.hpp"
#include <QtWidgets>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QVariant>

SceneHierarchyPanel::SceneHierarchyPanel(QWidget* parent)
    : QWidget(parent)
    , m_layout(nullptr)
    , m_treeWidget(nullptr)
    , m_buttonLayout(nullptr)
    , m_deleteButton(nullptr)
    , m_contextMenu(nullptr)
{
    setupUI();
    setupContextMenu();
    connectSignals();
}

void SceneHierarchyPanel::setScene(std::shared_ptr<rude::Scene> scene)
{
    m_scene = scene;
    refreshHierarchy();
}

void SceneHierarchyPanel::refreshHierarchy()
{
    m_treeWidget->clear();
    
    if (!m_scene) {
        return;
    }
    
    // Use the new ECS API
    auto entities = m_scene->getAllEntities();
    for (auto* entity : entities) {
        auto item = new QTreeWidgetItem(m_treeWidget);
        updateItemFromEntity(item, entity);
    }
    
    m_treeWidget->expandAll();
}

void SceneHierarchyPanel::setupUI()
{
    m_layout = new QVBoxLayout(this);
    
    // Create tree widget
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabel("Scene Objects");
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeWidget->setDragDropMode(QAbstractItemView::InternalMove);
    m_treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Create button layout
    m_buttonLayout = new QHBoxLayout();
    m_deleteButton = new QPushButton("Delete", this);
    m_deleteButton->setEnabled(false);
    
    m_buttonLayout->addWidget(m_deleteButton);
    m_buttonLayout->addStretch();
    
    // Add widgets to layout
    m_layout->addWidget(m_treeWidget);
    m_layout->addLayout(m_buttonLayout);
}

void SceneHierarchyPanel::setupContextMenu()
{
    m_contextMenu = new QMenu(this);
    
    m_renameAction = m_contextMenu->addAction("Rename");
    m_duplicateAction = m_contextMenu->addAction("Duplicate");
    m_contextMenu->addSeparator();
    m_deleteAction = m_contextMenu->addAction("Delete");
    
    connect(m_renameAction, &QAction::triggered, this, &SceneHierarchyPanel::onRenameSelectedObject);
    connect(m_duplicateAction, &QAction::triggered, this, &SceneHierarchyPanel::onDuplicateSelectedObject);
    connect(m_deleteAction, &QAction::triggered, this, &SceneHierarchyPanel::onDeleteSelectedObject);
}

void SceneHierarchyPanel::connectSignals()
{
    connect(m_treeWidget, &QTreeWidget::itemSelectionChanged, 
            this, &SceneHierarchyPanel::onItemSelectionChanged);
    connect(m_treeWidget, &QTreeWidget::itemChanged, 
            this, &SceneHierarchyPanel::onItemChanged);
    connect(m_treeWidget, &QTreeWidget::customContextMenuRequested, 
            this, &SceneHierarchyPanel::onCustomContextMenu);
    connect(m_deleteButton, &QPushButton::clicked, 
            this, &SceneHierarchyPanel::onDeleteSelectedObject);
}

void SceneHierarchyPanel::onItemSelectionChanged()
{
    auto selectedItems = m_treeWidget->selectedItems();
    bool hasSelection = !selectedItems.isEmpty();
    
    m_deleteButton->setEnabled(hasSelection);
    
    if (hasSelection && m_scene) {
        auto entity = getEntityFromItem(selectedItems.first());
        // TODO: Implement entity selection system if needed
        emit entitySelected(entity);
    } else {
        emit entitySelected(nullptr);
    }
}

void SceneHierarchyPanel::onItemChanged(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column)
    
    if (!item || !m_scene) {
        return;
    }
    
    auto entity = getEntityFromItem(item);
    if (entity) {
        QString newName = item->text(0);
        std::string stdName = newName.toStdString();
        if (stdName != entity->getName()) {
            entity->name = stdName;  // Direct assignment since name is public
            emit entityRenamed(entity, newName);
        }
    }
}

void SceneHierarchyPanel::onCustomContextMenu(const QPoint& pos)
{
    auto item = m_treeWidget->itemAt(pos);
    if (item) {
        m_contextMenu->exec(m_treeWidget->mapToGlobal(pos));
    }
}

void SceneHierarchyPanel::onDeleteSelectedObject()
{
    auto selectedItems = m_treeWidget->selectedItems();
    if (selectedItems.isEmpty() || !m_scene) {
        return;
    }
    
    auto entity = getEntityFromItem(selectedItems.first());
    if (entity) {
        QString entityName = QString::fromStdString(entity->getName());
        auto ret = QMessageBox::question(this, "Delete Entity", 
                                        QString("Are you sure you want to delete '%1'?").arg(entityName),
                                        QMessageBox::Yes | QMessageBox::No, 
                                        QMessageBox::No);
        
        if (ret == QMessageBox::Yes) {
            m_scene->removeEntity(entity);
            refreshHierarchy();
            emit entityDeleted(entity);
        }
    }
}

void SceneHierarchyPanel::onRenameSelectedObject()
{
    auto selectedItems = m_treeWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }
    
    auto item = selectedItems.first();
    m_treeWidget->editItem(item, 0);
}

void SceneHierarchyPanel::onDuplicateSelectedObject()
{
    auto selectedItems = m_treeWidget->selectedItems();
    if (selectedItems.isEmpty() || !m_scene) {
        return;
    }
    
    auto entity = getEntityFromItem(selectedItems.first());
    if (entity) {
        // Use the scene's duplicate method
        auto duplicatedEntity = m_scene->duplicateEntity(entity);
        if (duplicatedEntity) {
            refreshHierarchy();
            emit entitySelected(duplicatedEntity);
        }
    }
}

QTreeWidgetItem* SceneHierarchyPanel::findItemByEntityId(unsigned int entityId)
{
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        auto item = m_treeWidget->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toUInt() == entityId) {
            return item;
        }
    }
    return nullptr;
}

Entity* SceneHierarchyPanel::getEntityFromItem(QTreeWidgetItem* item)
{
    if (!item || !m_scene) {
        return nullptr;
    }
    
    unsigned int entityId = item->data(0, Qt::UserRole).toUInt();
    return m_scene->findEntityById(entityId);
}

void SceneHierarchyPanel::updateItemFromEntity(QTreeWidgetItem* item, Entity* entity)
{
    if (!item || !entity) {
        return;
    }
    
    QString entityName = QString::fromStdString(entity->getName());
    item->setText(0, entityName);
    item->setData(0, Qt::UserRole, QVariant(entity->getId()));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    
    // Set color based on entity state (assume all entities are visible for now)
    item->setForeground(0, QBrush(QColor(255, 255, 255)));
}
