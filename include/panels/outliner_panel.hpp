#pragma once
#include <QDockWidget>
#include <QTreeWidget>

namespace rude {
    class Scene;
}
class Entity;

class OutlinerPanel : public QDockWidget {
    Q_OBJECT
public:
    explicit OutlinerPanel(QWidget* parent = nullptr);
    void setScene(rude::Scene* scene);
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
    rude::Scene* scene = nullptr;
}; 