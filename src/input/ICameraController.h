#pragma once

#include "Common.h"
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <memory>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
#include "InputEvents.h"
class Camera;
// Forward declaration for rude::Scene
namespace rude { class Scene; }

/**
 * @brief Abstract interface for camera control strategies
 * 
 * This interface allows different camera control schemes to be plugged in,
 * following the Strategy pattern. Each implementation provides a different
 * user interaction model (Maya, Blender, CAD, etc.).
 */
class ICameraController {
public:
    virtual ~ICameraController() = default;
    virtual void setCamera(std::shared_ptr<Camera> camera) = 0;
    virtual void setScene(std::shared_ptr<rude::Scene> scene) = 0;
    // Input handling - return true if event was handled
    virtual bool handleMousePress(const MouseEvent& event) = 0;
    virtual bool handleMouseMove(const MouseEvent& event) = 0;
    virtual bool handleMouseRelease(const MouseEvent& event) = 0;
    virtual bool handleWheel(const WheelEvent& event) = 0;
    virtual bool handleKeyPress(const KeyEvent& event) = 0;
    virtual bool handleKeyRelease(const KeyEvent& event) = 0;
    // Camera operations
    virtual void resetCamera() = 0;
    virtual void frameScene(bool animate = true) = 0;
    virtual void frameSelection(bool animate = true) = 0;
    virtual void updateAspectRatio(float aspectRatio) = 0;
    // Camera state queries
    virtual glm::vec3 getWorldPosition() const = 0;
    virtual glm::mat4 getViewMatrix() const = 0;
    virtual glm::mat4 getProjectionMatrix() const = 0;
    virtual glm::vec3 screenToWorldRay(const glm::vec2& screenPos, const glm::ivec2& viewportSize) const = 0;
    // Settings
    virtual void setMovementSpeed(float speed) = 0;
    virtual void setRotationSpeed(float speed) = 0;
    virtual void setPanSpeed(float speed) = 0;
    virtual void setZoomSpeed(float speed) = 0;
    virtual void setInvertY(bool invert) = 0;
    virtual float getMovementSpeed() const = 0;
    virtual float getRotationSpeed() const = 0;
    virtual float getPanSpeed() const = 0;
    virtual float getZoomSpeed() const = 0;
    virtual bool isYInverted() const = 0;
    // Controller identification
    virtual std::string getControllerName() const = 0;
    virtual std::string getControllerDescription() const = 0;
protected:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<rude::Scene> m_scene;
};

/**
 * @brief Camera bookmark system for saving/restoring view states
 */
class CameraBookmark {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov;
    std::string name;
    uint64_t timestamp; // Use UNIX timestamp or std::chrono for cross-platform

    CameraBookmark() = default;
    CameraBookmark(const glm::vec3& pos, const glm::vec3& tgt, const glm::vec3& upVec,
                  float fieldOfView, const std::string& bookmarkName = "")
        : position(pos), target(tgt), up(upVec), fov(fieldOfView), name(bookmarkName)
        , timestamp(static_cast<uint64_t>(time(nullptr))) {}
};

/**
 * @brief Advanced camera state management system
 * 
 * Handles camera bookmarks, view history, focus management, and other
 * professional camera features that should be consistent across all
 * camera controller implementations.
 */
class CameraStateManager {
public:
    CameraStateManager();

    // Bookmark management
    void saveBookmark(int slot, const std::string& name = "");
    void restoreBookmark(int slot, bool animate = true);
    bool hasBookmark(int slot) const;
    CameraBookmark getBookmark(int slot) const;
    void clearBookmark(int slot);
    void clearAllBookmarks();

    // View history
    void pushCurrentView();
    void goBackInHistory();
    void goForwardInHistory();
    bool canGoBack() const;
    bool canGoForward() const;

    // Focus management
    void setOrbitPivot(const glm::vec3& pivot);
    glm::vec3 getOrbitPivot() const;
    void setAutoOrbitPivot(bool enabled);
    bool isAutoOrbitPivotEnabled() const;

    // Smart framing
    void setFramingMargin(float margin) { m_framingMargin = margin; }
    float getFramingMargin() const { return m_framingMargin; }

    // Camera state
    void setCamera(std::shared_ptr<Camera> camera);
    void updateFromCamera();

private:
    std::shared_ptr<Camera> m_camera;

    // Bookmarks (slots 0-9)
    std::map<int, CameraBookmark> m_bookmarks;

    // View history
    std::vector<CameraBookmark> m_viewHistory;
    int m_historyIndex;
    static const int MAX_HISTORY_SIZE = 50;

    // Orbit pivot
    glm::vec3 m_orbitPivot;
    bool m_autoOrbitPivot;

    // Settings
    float m_framingMargin;

    void addToHistory(const CameraBookmark& bookmark);
    CameraBookmark createBookmarkFromCamera() const;
    void applyCameraBookmark(const CameraBookmark& bookmark, bool animate);
};

/**
 * @brief Factory for creating camera controllers
 */
class CameraControllerFactory {
public:
    enum class ControllerType {
        Maya,
        Blender,
        CAD,
        Game,
        Custom
    };

    static std::unique_ptr<ICameraController> createController(ControllerType type);
    static std::vector<std::string> getAvailableControllers();
    static std::string getControllerDescription(ControllerType type);
};
