#pragma once
#include <QDockWidget>
#include <QTreeWidget>
class Scene;
class Entity;

class OutlinerPanel : public QDockWidget {
    Q_OBJECT
public:
    explicit OutlinerPanel(QWidget* parent = nullptr);
    void setScene(Scene* scene);
    void setSelectedEntity(Entity* entity);
    void updateEntityList();
signals:
    void selectionChanged(Entity* entity);
    void zoomToEntity(Entity* entity);
private slots:
    void onItemSelectionChanged();
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);
private:
    QTreeWidget* treeWidget;
    Scene* scene = nullptr;
}; 