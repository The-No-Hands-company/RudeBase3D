#include "GLFWViewport.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Platform-specific includes for window embedding
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#endif

#include <QApplication>
#include <QVBoxLayout>
#include <QDebug>
#include <stdexcept>

// Static instance for GLFW callbacks
GLFWViewport* GLFWViewport::s_instance = nullptr;

GLFWViewport::GLFWViewport(QWidget* parent)
    : QWidget(parent)
    , m_renderTimer(new QTimer(this))
{
    // Set static instance for callbacks
    s_instance = this;
    
    // Setup render timer
    connect(m_renderTimer, &QTimer::timeout, this, &GLFWViewport::renderFrame);
    
    // Initialize GLFW and create window
    if (!initializeGLFW()) {
        emit renderingError("Failed to initialize GLFW");
        return;
    }
    
    qDebug() << "GLFWViewport created successfully";
}

GLFWViewport::~GLFWViewport()
{
    cleanupGLFW();
    s_instance = nullptr;
}

bool GLFWViewport::initializeGLFW()
{
    // Set error callback
    glfwSetErrorCallback(glfwErrorCallback);
    
    // Initialize GLFW
    if (!glfwInit()) {
        qDebug() << "Failed to initialize GLFW";
        return false;
    }
    
    // Configure GLFW for OpenGL 4.6 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Multisampling for anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    // Make window non-visible initially (we'll embed it)
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    
    return createGLFWWindow();
}

bool GLFWViewport::createGLFWWindow()
{
    // Create GLFW window
    m_glfwWindow = glfwCreateWindow(m_width, m_height, "RudeBase3D Viewport", nullptr, nullptr);
    if (!m_glfwWindow) {
        qDebug() << "Failed to create GLFW window";
        glfwTerminate();
        return false;
    }

    // Set window user pointer for callbacks
    glfwSetWindowUserPointer(m_glfwWindow, this);

    // Set callbacks
    glfwSetFramebufferSizeCallback(m_glfwWindow, glfwFramebufferSizeCallback);
    glfwSetMouseButtonCallback(m_glfwWindow, glfwMouseButtonCallback);
    glfwSetCursorPosCallback(m_glfwWindow, glfwCursorPosCallback);
    glfwSetScrollCallback(m_glfwWindow, glfwScrollCallback);
    glfwSetKeyCallback(m_glfwWindow, glfwKeyCallback);

    // Make context current
    glfwMakeContextCurrent(m_glfwWindow);

    // Initialize OpenGL loader (GLEW)
    if (!initializeOpenGL()) {
        destroyGLFWWindow();
        return false;
    }
    
    // Create Qt window from GLFW native handle
#ifdef _WIN32
    HWND hwnd = glfwGetWin32Window(m_glfwWindow);
    m_qtWindow = QWindow::fromWinId((WId)hwnd);
#elif defined(__linux__)
    Window xWindow = glfwGetX11Window(m_glfwWindow);
    m_qtWindow = QWindow::fromWinId(xWindow);
#elif defined(__APPLE__)
    // macOS implementation would go here
    qDebug() << "macOS not yet implemented for GLFW window embedding";
    return false;
#endif
    
    if (!m_qtWindow) {
        qDebug() << "Failed to create Qt window from GLFW window";
        destroyGLFWWindow();
        return false;
    }
    
    // Create container widget
    m_container = QWidget::createWindowContainer(m_qtWindow, this);
    if (!m_container) {
        qDebug() << "Failed to create window container";
        destroyGLFWWindow();
        return false;
    }
    
    // Setup layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_container);
    setLayout(layout);
    
    // Enable V-Sync
    glfwSwapInterval(1);
    
    m_initialized = true;
    
    // Start render loop
    m_renderTimer->start(16); // ~60 FPS
    
    qDebug() << "GLFW window created and embedded successfully";
    qDebug() << "OpenGL Version:" << (char*)glGetString(GL_VERSION);
    qDebug() << "OpenGL Vendor:" << (char*)glGetString(GL_VENDOR);
    qDebug() << "OpenGL Renderer:" << (char*)glGetString(GL_RENDERER);
    
    emit viewportReady();
    return true;
}

bool GLFWViewport::initializeOpenGL()
{
    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        qDebug() << "Failed to initialize GLEW:" << (char*)glewGetErrorString(err);
        return false;
    }
    
    // Check OpenGL version
    if (!GLEW_VERSION_4_6) {
        qDebug() << "OpenGL 4.6 not available";
        // Try lower version
        if (!GLEW_VERSION_3_3) {
            qDebug() << "OpenGL 3.3 not available - viewport may not work correctly";
        }
    }
    
    setupOpenGLState();
    return true;
}

void GLFWViewport::setupOpenGLState()
{
    // Set viewport
    glViewport(0, 0, m_width, m_height);
    
    // Professional viewport background (dark gray like Blender/Maya)
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Enable multisampling (if available)
    glEnable(GL_MULTISAMPLE);
    
    // Enable line smoothing for better grid appearance
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    qDebug() << "OpenGL state configured";
}

void GLFWViewport::renderFrame()
{
    if (!m_initialized || !m_glfwWindow) {
        return;
    }
    
    // Make context current
    glfwMakeContextCurrent(m_glfwWindow);
    
    // Render
    render();
    
    // Swap buffers
    glfwSwapBuffers(m_glfwWindow);
    
    // Poll events
    glfwPollEvents();
}

void GLFWViewport::render()
{
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Simple test rendering - draw a colorful triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(0.0f, 0.6f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(-0.6f, -0.6f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(0.6f, -0.6f, 0.0f);
    glEnd();
    
    // Draw grid if enabled
    if (m_gridSystem && m_showGrid) {
        // TODO: Integrate with existing GridSystem
        // For now, draw a simple grid
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_LINES);
        for (int i = -10; i <= 10; ++i) {
            // Vertical lines
            glVertex3f(i * 0.1f, -1.0f, 0.0f);
            glVertex3f(i * 0.1f, 1.0f, 0.0f);
            // Horizontal lines
            glVertex3f(-1.0f, i * 0.1f, 0.0f);
            glVertex3f(1.0f, i * 0.1f, 0.0f);
        }
        glEnd();
    }
    
    // TODO: Integrate with RenderSystem, Camera, etc.
    if (m_renderSystem) {
        // m_renderSystem->render();
    }
}

void GLFWViewport::cleanupGLFW()
{
    m_renderTimer->stop();
    
    if (m_container) {
        m_container->deleteLater();
        m_container = nullptr;
    }
    
    destroyGLFWWindow();
    glfwTerminate();
}

void GLFWViewport::destroyGLFWWindow()
{
    if (m_glfwWindow) {
        glfwDestroyWindow(m_glfwWindow);
        m_glfwWindow = nullptr;
    }
    
    if (m_qtWindow) {
        m_qtWindow->deleteLater();
        m_qtWindow = nullptr;
    }
}

// System setters
void GLFWViewport::setRenderSystem(std::shared_ptr<RenderSystem> renderSystem)
{
    m_renderSystem = renderSystem;
}

void GLFWViewport::setGridSystem(std::shared_ptr<GridSystem> gridSystem)
{
    m_gridSystem = gridSystem;
}

void GLFWViewport::setLightingSystem(std::shared_ptr<LightingSystem> lightingSystem)
{
    m_lightingSystem = lightingSystem;
}

void GLFWViewport::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

void GLFWViewport::setShowGrid(bool show)
{
    m_showGrid = show;
}

bool GLFWViewport::isGridVisible() const
{
    return m_showGrid;
}

void GLFWViewport::resetCamera()
{
    // TODO: Implement camera reset
}

void GLFWViewport::frameScene()
{
    // TODO: Implement scene framing
}

// Qt event handlers
void GLFWViewport::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    
    if (m_glfwWindow && m_initialized) {
        m_width = event->size().width();
        m_height = event->size().height();
        glfwSetWindowSize(m_glfwWindow, m_width, m_height);
    }
}

void GLFWViewport::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    
    if (m_glfwWindow) {
        glfwShowWindow(m_glfwWindow);
    }
}

// GLFW Callbacks
void GLFWViewport::glfwErrorCallback(int error, const char* description)
{
    qDebug() << "GLFW Error" << error << ":" << description;
}

void GLFWViewport::glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    
    GLFWViewport* viewport = static_cast<GLFWViewport*>(glfwGetWindowUserPointer(window));
    if (viewport) {
        viewport->m_width = width;
        viewport->m_height = height;
    }
}

void GLFWViewport::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    GLFWViewport* viewport = static_cast<GLFWViewport*>(glfwGetWindowUserPointer(window));
    if (!viewport) return;
    
    // TODO: Implement mouse button handling
    qDebug() << "Mouse button:" << button << "action:" << action << "mods:" << mods;
}

void GLFWViewport::glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    GLFWViewport* viewport = static_cast<GLFWViewport*>(glfwGetWindowUserPointer(window));
    if (!viewport) return;
    
    // TODO: Implement mouse movement handling
}

void GLFWViewport::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    GLFWViewport* viewport = static_cast<GLFWViewport*>(glfwGetWindowUserPointer(window));
    if (!viewport) return;
    
    // TODO: Implement scroll wheel handling (zoom)
    qDebug() << "Scroll:" << xoffset << yoffset;
}

void GLFWViewport::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLFWViewport* viewport = static_cast<GLFWViewport*>(glfwGetWindowUserPointer(window));
    if (!viewport) return;
    
    // TODO: Implement keyboard handling
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        qDebug() << "Escape pressed";
    }
}
