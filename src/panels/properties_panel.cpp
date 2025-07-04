#include "panels/properties_panel.hpp"
#include "core/entity.hpp"
#include <QVBoxLayout>
#include <QLabel>

PropertiesPanel::PropertiesPanel(QWidget* parent)
    : QDockWidget("Properties", parent), contentWidget(new QWidget(this))
{
    setWidget(contentWidget);
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    layout->addWidget(new QLabel("[Properties Placeholder]", contentWidget));
}

void PropertiesPanel::setEntity(Entity* entity) {
    currentEntity = entity;
    updateProperties();
}

void PropertiesPanel::updateProperties() {
    // For now, just update the placeholder text
    // In the future, display/edit entity properties
}