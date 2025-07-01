#include "SceneHierarchyPanel.h"
#include "Scene.h"
#include "SceneObject.h"
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>

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

void SceneHierarchyPanel::setScene(std::shared_ptr<Scene> scene)
{
    // Disconnect from previous scene
    if (m_scene) {
        disconnect(m_scene.get(), nullptr, this, nullptr);
    }
    
    m_scene = scene;
    
    // Connect to new scene
    if (m_scene) {
        connect(m_scene.get(), &Scene::objectAdded, this, &SceneHierarchyPanel::refreshHierarchy);
        connect(m_scene.get(), &Scene::objectRemoved, this, &SceneHierarchyPanel::refreshHierarchy);
        connect(m_scene.get(), &Scene::selectionChanged, this, &SceneHierarchyPanel::selectObject);
    }
    
    refreshHierarchy();
}

void SceneHierarchyPanel::refreshHierarchy()
{
    m_treeWidget->clear();
    
    if (!m_scene) {
        return;
    }
    
    for (const auto& object : m_scene->getObjects()) {
        auto item = new QTreeWidgetItem(m_treeWidget);
        updateItemFromObject(item, object);
    }
    
    m_treeWidget->expandAll();
}

void SceneHierarchyPanel::selectObject(SceneObjectPtr object)
{
    if (!object) {
        m_treeWidget->clearSelection();
        return;
    }
    
    auto item = findItemByObjectId(object->getId());
    if (item) {
        m_treeWidget->setCurrentItem(item);
    }
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
        auto object = getObjectFromItem(selectedItems.first());
        m_scene->setSelectedObject(object);
        emit objectSelected(object);
    } else if (m_scene) {
        m_scene->clearSelection();
        emit objectSelected(nullptr);
    }
}

void SceneHierarchyPanel::onItemChanged(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column)
    
    if (!item || !m_scene) {
        return;
    }
    
    auto object = getObjectFromItem(item);
    if (object) {
        QString newName = item->text(0);
        if (newName != object->getName()) {
            object->setName(newName);
            emit objectRenamed(object, newName);
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
    
    auto object = getObjectFromItem(selectedItems.first());
    if (object) {
        int ret = QMessageBox::question(this, "Delete Object", 
                                       QString("Are you sure you want to delete '%1'?").arg(object->getName()),
                                       QMessageBox::Yes | QMessageBox::No, 
                                       QMessageBox::No);
        
        if (ret == QMessageBox::Yes) {
            m_scene->removeObject(object);
            emit objectDeleted(object);
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
    
    auto object = getObjectFromItem(selectedItems.first());
    if (object) {
        // Create a copy of the object
        auto newObject = std::make_shared<SceneObject>(object->getName() + "_Copy");
        newObject->setMesh(object->getMesh());
        newObject->setMaterial(object->getMaterial());
        newObject->getTransform() = object->getTransform();
        newObject->getTransform().translate(QVector3D(1.0f, 0.0f, 0.0f)); // Offset slightly
        
        m_scene->addObject(newObject);
        m_scene->setSelectedObject(newObject);
    }
}

QTreeWidgetItem* SceneHierarchyPanel::findItemByObjectId(unsigned int objectId)
{
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        auto item = m_treeWidget->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toUInt() == objectId) {
            return item;
        }
    }
    return nullptr;
}

SceneObjectPtr SceneHierarchyPanel::getObjectFromItem(QTreeWidgetItem* item)
{
    if (!item || !m_scene) {
        return nullptr;
    }
    
    unsigned int objectId = item->data(0, Qt::UserRole).toUInt();
    return m_scene->getObject(objectId);
}

void SceneHierarchyPanel::updateItemFromObject(QTreeWidgetItem* item, SceneObjectPtr object)
{
    if (!item || !object) {
        return;
    }
    
    item->setText(0, object->getName());
    item->setData(0, Qt::UserRole, object->getId());
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    
    // Set icon or color based on object visibility
    if (!object->isVisible()) {
        item->setForeground(0, QBrush(QColor(128, 128, 128)));
    } else {
        item->setForeground(0, QBrush(QColor(255, 255, 255)));
    }
}
