#include "panels/outliner_panel.hpp"
#include "core/scene.hpp"
#include "core/entity.hpp"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

OutlinerPanel::OutlinerPanel(QWidget* parent)
    : QDockWidget("Outliner", parent), treeWidget(new QTreeWidget(this))
{
    setWidget(treeWidget);
    treeWidget->setHeaderHidden(true);
    connect(treeWidget, &QTreeWidget::itemSelectionChanged, this, &OutlinerPanel::onItemSelectionChanged);
    connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &OutlinerPanel::onItemDoubleClicked);
}

void OutlinerPanel::setScene(rude::Scene* s) {
    scene = s;
    updateEntityList();
}

void OutlinerPanel::updateEntityList() {
    treeWidget->clear();
    if (!scene) return;
    for (const auto& entityPtr : scene->getEntities()) {
        QTreeWidgetItem* item = new QTreeWidgetItem(treeWidget);
        item->setText(0, QString::fromStdString(entityPtr->getName()));
        item->setData(0, Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(entityPtr.get())));
        treeWidget->addTopLevelItem(item);
    }
}

void OutlinerPanel::setSelectedEntity(Entity* entity) {
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem* item = treeWidget->topLevelItem(i);
        if (reinterpret_cast<Entity*>(item->data(0, Qt::UserRole).value<quintptr>()) == entity) {
            treeWidget->setCurrentItem(item);
            return;
        }
    }
    treeWidget->setCurrentItem(nullptr);
}

void OutlinerPanel::onItemSelectionChanged() {
    QTreeWidgetItem* item = treeWidget->currentItem();
    if (!item) {
        emit selectionChanged(nullptr);
        return;
    }
    Entity* entity = reinterpret_cast<Entity*>(item->data(0, Qt::UserRole).value<quintptr>());
    emit selectionChanged(entity);
}

void OutlinerPanel::onItemDoubleClicked(QTreeWidgetItem* item, int column) {
    if (!item) return;
    
    // Column parameter could be used for different actions based on which column was clicked
    // For now, acknowledge parameter until column-specific actions are implemented
    (void)column;
    
    Entity* entity = reinterpret_cast<Entity*>(item->data(0, Qt::UserRole).value<quintptr>());
    emit zoomToEntity(entity);
}