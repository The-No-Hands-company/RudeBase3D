#include "AssetManager.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshGenerator.h"
#include <QDebug>
#include <QFileInfo>

AssetManager::AssetManager(QObject* parent)
    : QObject(parent)
    , m_meshCounter(1)
    , m_materialCounter(1)
{
}

AssetManager::~AssetManager()
{
    cleanup();
}

bool AssetManager::initialize()
{
    // Create some default assets
    createDefaultMaterial();
    
    qDebug() << "AssetManager initialized successfully";
    return true;
}

void AssetManager::cleanup()
{
    clearCache();
}

std::shared_ptr<Mesh> AssetManager::createMesh(const QString& name)
{
    QString uniqueName = name;
    if (m_meshCache.contains(name)) {
        uniqueName = generateUniqueName(name, getMeshNames());
    }
    
    auto mesh = std::make_shared<Mesh>();
    m_meshCache[uniqueName] = mesh;
    
    emit assetLoaded(uniqueName, "Mesh");
    return mesh;
}

std::shared_ptr<Mesh> AssetManager::getMesh(const QString& name) const
{
    return m_meshCache.value(name);
}

bool AssetManager::loadMesh(const QString& name, const QString& filePath)
{
    Q_UNUSED(name)
    Q_UNUSED(filePath)
    
    // TODO: Implement mesh loading from file
    qDebug() << "Mesh loading from file not yet implemented";
    emit assetLoadFailed(name, "File loading not implemented");
    return false;
}

void AssetManager::removeMesh(const QString& name)
{
    m_meshCache.remove(name);
}

std::shared_ptr<Material> AssetManager::createMaterial(const QString& name)
{
    QString uniqueName = name;
    if (m_materialCache.contains(name)) {
        uniqueName = generateUniqueName(name, getMaterialNames());
    }
    
    auto material = std::make_shared<Material>();
    m_materialCache[uniqueName] = material;
    
    emit assetLoaded(uniqueName, "Material");
    return material;
}

std::shared_ptr<Material> AssetManager::getMaterial(const QString& name) const
{
    return m_materialCache.value(name);
}

bool AssetManager::loadMaterial(const QString& name, const QString& filePath)
{
    Q_UNUSED(name)
    Q_UNUSED(filePath)
    
    // TODO: Implement material loading from file
    qDebug() << "Material loading from file not yet implemented";
    emit assetLoadFailed(name, "File loading not implemented");
    return false;
}

void AssetManager::removeMaterial(const QString& name)
{
    m_materialCache.remove(name);
}

std::shared_ptr<Mesh> AssetManager::createCubeMesh(float size)
{
    QString name = QString("Cube_%1").arg(m_meshCounter++);
    auto mesh = MeshGenerator::generateCube(size);
    if (mesh) {
        m_meshCache[name] = mesh;
        emit assetLoaded(name, "Mesh");
    }
    return mesh;
}

std::shared_ptr<Mesh> AssetManager::createSphereMesh(float radius, int segments, int rings)
{
    QString name = QString("Sphere_%1").arg(m_meshCounter++);
    auto mesh = MeshGenerator::generateSphere(radius, segments, rings);
    if (mesh) {
        m_meshCache[name] = mesh;
        emit assetLoaded(name, "Mesh");
    }
    return mesh;
}

std::shared_ptr<Mesh> AssetManager::createPlaneMesh(float width, float height)
{
    QString name = QString("Plane_%1").arg(m_meshCounter++);
    auto mesh = MeshGenerator::generatePlane(width, height);
    if (mesh) {
        m_meshCache[name] = mesh;
        emit assetLoaded(name, "Mesh");
    }
    return mesh;
}

std::shared_ptr<Mesh> AssetManager::createCylinderMesh(float radius, float height, int segments)
{
    QString name = QString("Cylinder_%1").arg(m_meshCounter++);
    auto mesh = MeshGenerator::generateCylinder(radius, height, segments);
    if (mesh) {
        m_meshCache[name] = mesh;
        emit assetLoaded(name, "Mesh");
    }
    return mesh;
}

std::shared_ptr<Mesh> AssetManager::createConeMesh(float radius, float height, int segments)
{
    QString name = QString("Cone_%1").arg(m_meshCounter++);
    // TODO: Implement generateCone, using generateCylinder as temporary workaround
    auto mesh = MeshGenerator::generateCylinder(radius, height, segments);
    if (mesh) {
        m_meshCache[name] = mesh;
        emit assetLoaded(name, "Mesh");
    }
    return mesh;
}

std::shared_ptr<Mesh> AssetManager::createGridMesh(float size, int divisions)
{
    QString name = QString("Grid_%1").arg(m_meshCounter++);
    auto mesh = MeshGenerator::generateGrid(size, divisions);
    if (mesh) {
        m_meshCache[name] = mesh;
        emit assetLoaded(name, "Mesh");
    }
    return mesh;
}

std::shared_ptr<Material> AssetManager::createDefaultMaterial()
{
    QString name = "Default";
    
    if (m_materialCache.contains(name)) {
        return m_materialCache[name];
    }
    
    auto material = std::make_shared<Material>();
    material->setDiffuseColor(QVector4D(0.7f, 0.7f, 0.7f, 1.0f));
    material->setSpecularColor(QVector4D(0.3f, 0.3f, 0.3f, 1.0f));
    material->setShininess(32.0f);
    
    m_materialCache[name] = material;
    emit assetLoaded(name, "Material");
    return material;
}

std::shared_ptr<Material> AssetManager::createColoredMaterial(const QVector4D& color)
{
    QString name = QString("ColoredMaterial_%1").arg(m_materialCounter++);
    auto material = std::make_shared<Material>();
    material->setDiffuseColor(color);
    material->setSpecularColor(QVector4D(0.2f, 0.2f, 0.2f, 1.0f));
    material->setShininess(16.0f);
    
    m_materialCache[name] = material;
    emit assetLoaded(name, "Material");
    return material;
}

std::shared_ptr<Material> AssetManager::createMetallicMaterial(const QVector4D& color, float metallic, float roughness)
{
    QString name = QString("MetallicMaterial_%1").arg(m_materialCounter++);
    auto material = std::make_shared<Material>();
    material->setDiffuseColor(color);
    
    // Calculate specular based on metallic value
    QVector4D specular = color * metallic + QVector4D(0.04f, 0.04f, 0.04f, 1.0f) * (1.0f - metallic);
    material->setSpecularColor(specular);
    
    // Convert roughness to shininess (inverse relationship)
    float shininess = (1.0f - roughness) * 256.0f;
    material->setShininess(shininess);
    
    m_materialCache[name] = material;
    emit assetLoaded(name, "Material");
    return material;
}

QStringList AssetManager::getMeshNames() const
{
    return m_meshCache.keys();
}

QStringList AssetManager::getMaterialNames() const
{
    return m_materialCache.keys();
}

size_t AssetManager::getMeshCount() const
{
    return m_meshCache.size();
}

size_t AssetManager::getMaterialCount() const
{
    return m_materialCache.size();
}

void AssetManager::clearCache()
{
    clearMeshCache();
    clearMaterialCache();
}

void AssetManager::clearMeshCache()
{
    m_meshCache.clear();
    m_meshCounter = 1;
}

void AssetManager::clearMaterialCache()
{
    m_materialCache.clear();
    m_materialCounter = 1;
}

QString AssetManager::generateUniqueName(const QString& baseName, const QStringList& existingNames) const
{
    QString uniqueName = baseName;
    int counter = 1;
    
    while (existingNames.contains(uniqueName)) {
        uniqueName = QString("%1_%2").arg(baseName).arg(counter++);
    }
    
    return uniqueName;
}
