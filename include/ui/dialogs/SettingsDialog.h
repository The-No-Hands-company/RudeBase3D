#pragma once

#include <QDialog>

/**
 * @brief Simple settings dialog for RudeBase3D
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog();

private:
    void setupUI();
};
