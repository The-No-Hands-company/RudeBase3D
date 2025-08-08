#pragma once

#include <QWidget>
#include <QWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QShowEvent>
#include <QString>
#include <memory>

// Forward declarations
struct GLFWwindow;
class Camera;
class RenderSystem;
class GridSystem;
class LightingSystem;

/**
 * @brief Native GLFW viewport widget embedded in Qt
 * 
 * This widget creates a native GLFW window for OpenGL rendering and embeds it
 * into the Qt UI using QWidget::createWindowContainer(). This provides direct
 * control over the OpenGL context while maintaining Qt UI integration.
 */
class GLFWViewport : public QWidget
{
    Q_OBJECT

public:
    explicit GLFWViewport(QWidget* parent = nullptr);
    ~GLFWViewport();

    // Scene and rendering systems
    void setRenderSystem(std::shared_ptr<RenderSystem> renderSystem);
    void setGridSystem(std::shared_ptr<GridSystem> gridSystem);
    void setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem);
    void setCamera(std::shared_ptr<Camera> camera);

    // Viewport properties
    void setShowGrid(bool show);
    bool isGridVisible() const;

    // Camera controls
    void resetCamera();
    void frameScene();

signals:
    void viewportReady();
    void renderingError(const QString& error);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void renderFrame();

private:
    // GLFW window management
    bool initializeGLFW();
    void cleanupGLFW();
    bool createGLFWWindow();
    void destroyGLFWWindow();

    // OpenGL initialization
    bool initializeOpenGL();
    void setupOpenGLState();

    // Rendering
    void render();

    // Event handling
    static void glfwErrorCallback(int error, const char* description);
    static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Member variables
    GLFWwindow* m_glfwWindow = nullptr;
    QWindow* m_qtWindow = nullptr;
    QWidget* m_container = nullptr;
    QTimer* m_renderTimer = nullptr;

    // Rendering systems
    std::shared_ptr<RenderSystem> m_renderSystem;
    std::shared_ptr<GridSystem> m_gridSystem;
    std::shared_ptr<LightingSystem> m_lightingSystem;
    std::shared_ptr<Camera> m_camera;

    // State
    bool m_initialized = false;
    bool m_showGrid = true;
    int m_width = 800;
    int m_height = 600;

    // Static instance for callbacks
    static GLFWViewport* s_instance;
};
