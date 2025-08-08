#include "ui/dialogs/SettingsDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("RudeBase3D Settings");
    setModal(true);
    resize(400, 300);
    
    setupUI();
}

SettingsDialog::~SettingsDialog()
{
    // Auto cleanup with Qt parent-child system
}

void SettingsDialog::setupUI()
{
    auto layout = new QVBoxLayout(this);
    
    auto label = new QLabel("Settings dialog - Coming soon!", this);
    layout->addWidget(label);
    
    auto closeButton = new QPushButton("Close", this);
    layout->addWidget(closeButton);
    
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}
