#pragma once
#include <QDockWidget>
#include <QTreeWidget>

namespace rude {
    class Scene;
    class Entity;
}

class OutlinerPanel : public QDockWidget {
    Q_OBJECT
public:
    explicit OutlinerPanel(QWidget* parent = nullptr);
    void setScene(rude::Scene* scene);
    void setSelectedEntity(rude::Entity* entity);
    void updateEntityList();
signals:
    void selectionChanged(rude::Entity* entity);
    void zoomToEntity(rude::Entity* entity);
private slots:
    void onItemSelectionChanged();
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);
private:
    QTreeWidget* treeWidget;
    rude::Scene* scene = nullptr;
}; 