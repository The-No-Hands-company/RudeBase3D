#pragma once

#include "base_toolbar.hpp"

/**
 * @brief Toolbar for transform operations and constraints
 * 
 * Contains tools for:
 * - Transform modes (Move, Rotate, Scale)
 * - Coordinate systems (Global, Local, View)
 * - Constraints (X, Y, Z axis locking)
 * - Transform options (Snap, Proportional editing)
 */
class TransformToolbar : public BaseToolbar
{
    Q_OBJECT

public:
    explicit TransformToolbar(QMainWindow* parent = nullptr);
    
    // BaseToolbar interface
    QString getToolbarId() const override { return "transform_toolbar"; }
    QString getToolbarDescription() const override { return "Transform tools and constraints"; }

    // Transform state management
    void setTransformMode(const QString& mode);
    void setCoordinateSystem(const QString& system);
    void setAxisConstraint(const QString& axis);
    
    QString getTransformMode() const { return m_currentMode; }
    QString getCoordinateSystem() const { return m_coordinateSystem; }
    QString getAxisConstraint() const { return m_axisConstraint; }

signals:
    void transformModeChanged(const QString& mode);
    void coordinateSystemChanged(const QString& system);
    void axisConstraintChanged(const QString& axis);
    void snapToggled(bool enabled);
    void proportionalEditingToggled(bool enabled);

protected:
    void createActions() override;
    void setupLayout() override;
    void connectSignals() override;

private slots:
    void onTransformModeChanged();
    void onCoordinateSystemChanged();
    void onAxisConstraintChanged();
    void onSnapToggled();
    void onProportionalEditingToggled();

private:
    void createModeActions();
    void createCoordinateActions();
    void createConstraintActions();
    void createOptionActions();
    void updateModeButtons();
    void updateConstraintButtons();
    
    QString m_currentMode;
    QString m_coordinateSystem;
    QString m_axisConstraint;
    bool m_snapEnabled;
    bool m_proportionalEditingEnabled;
};
