#pragma once

#include "Common.h"
#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QVector2D>
#include <QSize>
#include <QtCore/QDateTime>
#include <memory>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class Camera;
class Scene;

/**
 * @brief Abstract interface for camera control strategies
 * 
 * This interface allows different camera control schemes to be plugged in,
 * following the Strategy pattern. Each implementation provides a different
 * user interaction model (Maya, Blender, CAD, etc.).
 */
class ICameraController : public QObject
{
    Q_OBJECT
    
public:
    explicit ICameraController(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ICameraController() = default;
    
    // Core interface
    virtual void setCamera(std::shared_ptr<Camera> camera) = 0;
    virtual void setScene(std::shared_ptr<Scene> scene) = 0;
    
    // Input handling - return true if event was handled
    virtual bool handleMousePress(QMouseEvent* event) = 0;
    virtual bool handleMouseMove(QMouseEvent* event) = 0;
    virtual bool handleMouseRelease(QMouseEvent* event) = 0;
    virtual bool handleWheel(QWheelEvent* event) = 0;
    virtual bool handleKeyPress(QKeyEvent* event) = 0;
    virtual bool handleKeyRelease(QKeyEvent* event) = 0;
    
    // Camera operations
    virtual void resetCamera() = 0;
    virtual void frameScene(bool animate = true) = 0;
    virtual void frameSelection(bool animate = true) = 0;
    virtual void updateAspectRatio(float aspectRatio) = 0;
    
    // Camera state queries
    virtual QVector3D getWorldPosition() const = 0;
    virtual QMatrix4x4 getViewMatrix() const = 0;
    virtual QMatrix4x4 getProjectionMatrix() const = 0;
    virtual QVector3D screenToWorldRay(const QVector2D& screenPos, const QSize& viewportSize) const = 0;
    
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
    virtual QString getControllerName() const = 0;
    virtual QString getControllerDescription() const = 0;
    
signals:
    void cameraChanged();
    void pivotChanged(const QVector3D& pivot);
    void viewChanged();
    
protected:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Scene> m_scene;
};

/**
 * @brief Camera bookmark system for saving/restoring view states
 */
class CameraBookmark
{
public:
    QVector3D position;
    QVector3D target;
    QVector3D up;
    float fov;
    QString name;
    QDateTime timestamp;
    
    CameraBookmark() = default;
    CameraBookmark(const QVector3D& pos, const QVector3D& tgt, const QVector3D& upVec, 
                   float fieldOfView, const QString& bookmarkName = QString())
        : position(pos), target(tgt), up(upVec), fov(fieldOfView), name(bookmarkName)
        , timestamp(QDateTime::currentDateTime()) {}
};

/**
 * @brief Advanced camera state management system
 * 
 * Handles camera bookmarks, view history, focus management, and other
 * professional camera features that should be consistent across all
 * camera controller implementations.
 */
class CameraStateManager : public QObject
{
    Q_OBJECT
    
public:
    explicit CameraStateManager(QObject* parent = nullptr);
    
    // Bookmark management
    void saveBookmark(int slot, const QString& name = QString());
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
    void setOrbitPivot(const QVector3D& pivot);
    QVector3D getOrbitPivot() const;
    void setAutoOrbitPivot(bool enabled);
    bool isAutoOrbitPivotEnabled() const;
    
    // Smart framing
    void setFramingMargin(float margin) { m_framingMargin = margin; }
    float getFramingMargin() const { return m_framingMargin; }
    
    // Camera state
    void setCamera(std::shared_ptr<Camera> camera);
    void updateFromCamera();
    
signals:
    void bookmarkSaved(int slot, const QString& name);
    void bookmarkRestored(int slot);
    void pivotChanged(const QVector3D& pivot);
    void historyChanged();
    
private:
    std::shared_ptr<Camera> m_camera;
    
    // Bookmarks (slots 0-9)
    QMap<int, CameraBookmark> m_bookmarks;
    
    // View history
    QList<CameraBookmark> m_viewHistory;
    int m_historyIndex;
    static const int MAX_HISTORY_SIZE = 50;
    
    // Orbit pivot
    QVector3D m_orbitPivot;
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
class CameraControllerFactory
{
public:
    enum class ControllerType {
        Maya,
        Blender,
        CAD,
        Game,
        Custom
    };
    
    static std::unique_ptr<ICameraController> createController(ControllerType type, QObject* parent = nullptr);
    static QStringList getAvailableControllers();
    static QString getControllerDescription(ControllerType type);
};
