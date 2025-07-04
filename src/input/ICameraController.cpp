#include "ICameraController.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include <QDateTime>
#include <QDebug>

// CameraStateManager Implementation
CameraStateManager::CameraStateManager(QObject* parent)
    : QObject(parent)
    , m_historyIndex(-1)
    , m_orbitPivot(0.0f, 0.0f, 0.0f)
    , m_autoOrbitPivot(true)
    , m_framingMargin(0.1f)
{
}

void CameraStateManager::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

void CameraStateManager::saveBookmark(int slot, const QString& name)
{
    if (slot < 0 || slot > 9 || !m_camera) {
        return;
    }
    
    CameraBookmark bookmark = createBookmarkFromCamera();
    bookmark.name = name.isEmpty() ? QString("Bookmark %1").arg(slot) : name;
    
    m_bookmarks[slot] = bookmark;
    
    qDebug() << "Camera bookmark saved to slot" << slot << ":" << bookmark.name;
    emit bookmarkSaved(slot, bookmark.name);
}

void CameraStateManager::restoreBookmark(int slot, bool animate)
{
    if (slot < 0 || slot > 9 || !hasBookmark(slot)) {
        return;
    }
    
    const CameraBookmark& bookmark = m_bookmarks[slot];
    applyCameraBookmark(bookmark, animate);
    
    qDebug() << "Camera bookmark restored from slot" << slot << ":" << bookmark.name;
    emit bookmarkRestored(slot);
}

bool CameraStateManager::hasBookmark(int slot) const
{
    return slot >= 0 && slot <= 9 && m_bookmarks.contains(slot);
}

CameraBookmark CameraStateManager::getBookmark(int slot) const
{
    if (hasBookmark(slot)) {
        return m_bookmarks[slot];
    }
    return CameraBookmark();
}

void CameraStateManager::clearBookmark(int slot)
{
    if (hasBookmark(slot)) {
        m_bookmarks.remove(slot);
    }
}

void CameraStateManager::clearAllBookmarks()
{
    m_bookmarks.clear();
}

void CameraStateManager::pushCurrentView()
{
    if (!m_camera) return;
    
    CameraBookmark current = createBookmarkFromCamera();
    addToHistory(current);
}

void CameraStateManager::goBackInHistory()
{
    if (!canGoBack()) return;
    
    m_historyIndex--;
    const CameraBookmark& bookmark = m_viewHistory[m_historyIndex];
    applyCameraBookmark(bookmark, true);
    
    emit historyChanged();
}

void CameraStateManager::goForwardInHistory()
{
    if (!canGoForward()) return;
    
    m_historyIndex++;
    const CameraBookmark& bookmark = m_viewHistory[m_historyIndex];
    applyCameraBookmark(bookmark, true);
    
    emit historyChanged();
}

bool CameraStateManager::canGoBack() const
{
    return m_historyIndex > 0;
}

bool CameraStateManager::canGoForward() const
{
    return m_historyIndex >= 0 && m_historyIndex < m_viewHistory.size() - 1;
}

void CameraStateManager::setOrbitPivot(const QVector3D& pivot)
{
    if (m_orbitPivot != pivot) {
        m_orbitPivot = pivot;
        m_autoOrbitPivot = false; // Manual pivot overrides auto
        emit pivotChanged(pivot);
    }
}

QVector3D CameraStateManager::getOrbitPivot() const
{
    return m_orbitPivot;
}

void CameraStateManager::setAutoOrbitPivot(bool enabled)
{
    m_autoOrbitPivot = enabled;
}

bool CameraStateManager::isAutoOrbitPivotEnabled() const
{
    return m_autoOrbitPivot;
}

void CameraStateManager::updateFromCamera()
{
    // This can be called when external code modifies the camera
    // to keep our state in sync
}

void CameraStateManager::addToHistory(const CameraBookmark& bookmark)
{
    // Remove any forward history if we're not at the end
    if (m_historyIndex >= 0 && m_historyIndex < m_viewHistory.size() - 1) {
        m_viewHistory.erase(m_viewHistory.begin() + m_historyIndex + 1, m_viewHistory.end());
    }
    
    // Add new bookmark
    m_viewHistory.append(bookmark);
    m_historyIndex = m_viewHistory.size() - 1;
    
    // Limit history size
    while (m_viewHistory.size() > MAX_HISTORY_SIZE) {
        m_viewHistory.removeFirst();
        m_historyIndex--;
    }
    
    emit historyChanged();
}

CameraBookmark CameraStateManager::createBookmarkFromCamera() const
{
    if (!m_camera) {
        return CameraBookmark();
    }
    
    QVector3D position = m_camera->getWorldPosition();
    QVector3D forward = m_camera->getForward();
    QVector3D up = m_camera->getUp();
    QVector3D target = position + forward * 10.0f; // Arbitrary target distance
    float fov = m_camera->getFOV();
    
    return CameraBookmark(position, target, up, fov);
}

void CameraStateManager::applyCameraBookmark(const CameraBookmark& bookmark, bool animate)
{
    if (!m_camera) return;
    
    // For now, apply immediately (animate parameter ignored)
    // TODO: Implement smooth animation system
    
    m_camera->getTransform().setPosition(bookmark.position);
    m_camera->lookAt(bookmark.target, bookmark.up);
    m_camera->setFOV(bookmark.fov);
    
    // Update orbit pivot to the target
    if (m_autoOrbitPivot) {
        setOrbitPivot(bookmark.target);
    }
}

// CameraControllerFactory Implementation
std::unique_ptr<ICameraController> CameraControllerFactory::createController(ControllerType type, QObject* parent)
{
    // For now, return nullptr - we'll implement specific controllers next
    // This will be populated as we create each controller type
    Q_UNUSED(type)
    Q_UNUSED(parent)
    return nullptr;
}

QStringList CameraControllerFactory::getAvailableControllers()
{
    return QStringList() << "Maya" << "Blender" << "CAD" << "Game" << "Custom";
}

QString CameraControllerFactory::getControllerDescription(ControllerType type)
{
    switch (type) {
        case ControllerType::Maya:
            return "Maya-style: Alt+LMB=orbit, Alt+MMB=pan, Alt+RMB=zoom";
        case ControllerType::Blender:
            return "Blender-style: MMB=orbit, Shift+MMB=pan, Ctrl+MMB=zoom";
        case ControllerType::CAD:
            return "CAD-style: RMB=orbit, MMB=pan, Wheel=zoom";
        case ControllerType::Game:
            return "Game-style: WASD=move, Mouse=look, RMB=mouse capture";
        case ControllerType::Custom:
            return "Custom user-defined control scheme";
        default:
            return "Unknown controller type";
    }
}
