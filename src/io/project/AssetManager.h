#pragma once

#include "Common.h"
#include <QObject>
#include <QString>
#include <QHash>
#include <memory>

class Mesh;
class Material;

class AssetManager : public QObject
{
    Q_OBJECT
    
public:
    explicit AssetManager(QObject* parent = nullptr);
    ~AssetManager();
    
    // Initialization
    bool initialize();
    void cleanup();
    
    // Mesh management
    std::shared_ptr<Mesh> createMesh(const QString& name);
    std::shared_ptr<Mesh> getMesh(const QString& name) const;
    bool loadMesh(const QString& name, const QString& filePath);
    void removeMesh(const QString& name);
    
    // Material management
    std::shared_ptr<Material> createMaterial(const QString& name);
    std::shared_ptr<Material> getMaterial(const QString& name) const;
    bool loadMaterial(const QString& name, const QString& filePath);
    void removeMaterial(const QString& name);
    
    // Primitive mesh creation
    std::shared_ptr<Mesh> createCubeMesh(float size = 1.0f);
    std::shared_ptr<Mesh> createSphereMesh(float radius = 0.5f, int segments = 32, int rings = 16);
    std::shared_ptr<Mesh> createPlaneMesh(float width = 2.0f, float height = 2.0f);
    std::shared_ptr<Mesh> createCylinderMesh(float radius = 0.5f, float height = 1.0f, int segments = 32);
    std::shared_ptr<Mesh> createConeMesh(float radius = 0.5f, float height = 1.0f, int segments = 32);
    std::shared_ptr<Mesh> createGridMesh(float size = 20.0f, int divisions = 20);
    
    // Default material creation
    std::shared_ptr<Material> createDefaultMaterial();
    std::shared_ptr<Material> createColoredMaterial(const QVector4D& color);
    std::shared_ptr<Material> createMetallicMaterial(const QVector4D& color, float metallic = 0.8f, float roughness = 0.2f);
    
    // Asset queries
    QStringList getMeshNames() const;
    QStringList getMaterialNames() const;
    size_t getMeshCount() const;
    size_t getMaterialCount() const;
    
    // Cache management
    void clearCache();
    void clearMeshCache();
    void clearMaterialCache();
    
signals:
    void assetLoaded(const QString& name, const QString& type);
    void assetLoadFailed(const QString& name, const QString& error);
    
private:
    QString generateUniqueName(const QString& baseName, const QStringList& existingNames) const;
    
    QHash<QString, std::shared_ptr<Mesh>> m_meshCache;
    QHash<QString, std::shared_ptr<Material>> m_materialCache;
    
    // Asset counters for unique naming
    int m_meshCounter;
    int m_materialCounter;
};
