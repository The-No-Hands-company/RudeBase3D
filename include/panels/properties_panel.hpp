#pragma once
#include <QDockWidget>
#include <QWidget>

class Entity;

namespace rude {

class PropertiesPanel : public QDockWidget {
    Q_OBJECT
public:
    explicit PropertiesPanel(QWidget* parent = nullptr);
    void setEntity(Entity* entity);
private:
    QWidget* contentWidget;
    Entity* currentEntity = nullptr;
    void updateProperties();
};

} // namespace rude 