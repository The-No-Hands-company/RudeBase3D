#pragma once

#include "base_toolbar.hpp"

/**
 * @brief Toolbar for primitive creation and basic geometry
 * 
 * Contains tools for creating basic 3D primitives:
 * - Basic shapes (Cube, Sphere, Cylinder, Plane)
 * - Advanced primitives (Torus, Cone, etc.)
 * - Curve tools
 * - Import/creation tools
 */
class PrimitivesToolbar : public BaseToolbar
{
    Q_OBJECT

public:
    explicit PrimitivesToolbar(QMainWindow* parent = nullptr);
    
    // BaseToolbar interface
    QString getToolbarId() const override { return "primitives_toolbar"; }
    QString getToolbarDescription() const override { return "Primitive creation tools"; }

signals:
    void primitiveRequested(const QString& primitiveType);
    void curveToolRequested(const QString& curveType);

protected:
    void createActions() override;
    void setupLayout() override;
    void connectSignals() override;

private slots:
    void onPrimitiveRequested();
    void onCurveToolRequested();
    void onActionTriggeredFromBase(const QString& actionId);

private:
    void createBasicPrimitives();
    void createAdvancedPrimitives();
    void createCurveTools();
};
