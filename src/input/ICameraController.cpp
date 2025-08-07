#include "ICameraController.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// CameraStateManager Implementation
CameraStateManager::CameraStateManager()
    : m_historyIndex(-1)
    , m_orbitPivot(0.0f, 0.0f, 0.0f)
    , m_autoOrbitPivot(true)
    , m_framingMargin(0.1f)
{
}

void CameraStateManager::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

void CameraStateManager::saveBookmark(int slot, const std::string& name)
{
    if (slot < 0 || slot > 9 || !m_camera) {
        return;
    }

    CameraBookmark bookmark = createBookmarkFromCamera();
    bookmark.name = name.empty() ? std::string("Bookmark ") + std::to_string(slot) : name;

    m_bookmarks[slot] = bookmark;
}

void CameraStateManager::restoreBookmark(int slot, bool animate)
{
    if (slot < 0 || slot > 9 || !hasBookmark(slot)) {
        return;
    }

    const CameraBookmark& bookmark = m_bookmarks.at(slot);
    applyCameraBookmark(bookmark, animate);
}

bool CameraStateManager::hasBookmark(int slot) const
{
    return slot >= 0 && slot <= 9 && m_bookmarks.find(slot) != m_bookmarks.end();
}

CameraBookmark CameraStateManager::getBookmark(int slot) const
{
    if (hasBookmark(slot)) {
        return m_bookmarks.at(slot);
    }
    return CameraBookmark();
}

void CameraStateManager::clearBookmark(int slot)
{
    if (hasBookmark(slot)) {
        m_bookmarks.erase(slot);
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
}

void CameraStateManager::goForwardInHistory()
{
    if (!canGoForward()) return;

    m_historyIndex++;
    const CameraBookmark& bookmark = m_viewHistory[m_historyIndex];
    applyCameraBookmark(bookmark, true);
}

bool CameraStateManager::canGoBack() const
{
    return m_historyIndex > 0;
}

bool CameraStateManager::canGoForward() const
{
    return m_historyIndex >= 0 && m_historyIndex < static_cast<int>(m_viewHistory.size()) - 1;
}

void CameraStateManager::setOrbitPivot(const glm::vec3& pivot)
{
    if (m_orbitPivot != pivot) {
        m_orbitPivot = pivot;
        m_autoOrbitPivot = false;
    }
}

glm::vec3 CameraStateManager::getOrbitPivot() const
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
    if (m_historyIndex >= 0 && m_historyIndex < static_cast<int>(m_viewHistory.size()) - 1) {
        m_viewHistory.erase(m_viewHistory.begin() + m_historyIndex + 1, m_viewHistory.end());
    }

    // Add new bookmark
    m_viewHistory.push_back(bookmark);
    m_historyIndex = static_cast<int>(m_viewHistory.size()) - 1;

    // Limit history size
    while (m_viewHistory.size() > MAX_HISTORY_SIZE) {
        m_viewHistory.erase(m_viewHistory.begin());
        m_historyIndex--;
    }
}

CameraBookmark CameraStateManager::createBookmarkFromCamera() const
{
    if (!m_camera) {
        return CameraBookmark();
    }

    glm::vec3 position = m_camera->getWorldPosition();
    glm::vec3 forward = m_camera->getForward();
    glm::vec3 up = m_camera->getUp();
    glm::vec3 target = position + forward * 10.0f;
    float fov = m_camera->getFOV();

    return CameraBookmark(position, target, up, fov);
}

void CameraStateManager::applyCameraBookmark(const CameraBookmark& bookmark, bool animate)
{
    if (!m_camera) return;

    // animate parameter could be used for smooth transitions
    (void)animate; // Future: enable smooth animated bookmark transitions
    
    m_camera->getTransform().setPosition(bookmark.position);
    m_camera->lookAt(bookmark.target, bookmark.up);
    m_camera->setFOV(bookmark.fov);

    if (m_autoOrbitPivot) {
        setOrbitPivot(bookmark.target);
    }
}

// CameraControllerFactory Implementation
std::unique_ptr<ICameraController> CameraControllerFactory::createController(ControllerType type)
{
    // For now, return nullptr - we'll implement specific controllers next
    (void)type;
    return nullptr;
}

std::vector<std::string> CameraControllerFactory::getAvailableControllers()
{
    return { "Maya", "Blender", "CAD", "Game", "Custom" };
}

std::string CameraControllerFactory::getControllerDescription(ControllerType type)
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