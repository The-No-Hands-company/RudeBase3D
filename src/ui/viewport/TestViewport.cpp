#include "TestViewport.h"
#include <QDebug>
#include <QtMath>
#include <QOpenGLFunctions_3_3_Core>

TestViewport::TestViewport(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_cameraPos(5.0f, 5.0f, 5.0f)
    , m_cameraTarget(0.0f, 0.0f, 0.0f)
    , m_cameraUp(0.0f, 1.0f, 0.0f)
    , m_cameraDistance(8.0f)
    , m_cameraYaw(45.0f)
    , m_cameraPitch(30.0f)
    , m_isDragging(false)
    , m_dragButton(Qt::NoButton)
    , m_time(0.0f)
    , m_initialized(false)
{
    setFocusPolicy(Qt::StrongFocus);
    
    // Create update timer
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &TestViewport::updateViewport);
    m_updateTimer->start(16); // ~60 FPS
    
    qDebug() << "TestViewport created";
}

void TestViewport::initializeGL()
{
    qDebug() << "TestViewport::initializeGL() called";
    
    initializeOpenGLFunctions();
    
    // Set OpenGL state
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f); // Slightly different color to distinguish from main viewport
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Setup camera
    setupCamera();
    
    m_initialized = true;
    qDebug() << "TestViewport OpenGL initialized successfully";
}

void TestViewport::resizeGL(int w, int h)
{
    qDebug() << "TestViewport::resizeGL() called - Size:" << w << "x" << h;
    
    glViewport(0, 0, w, h);
    
    // Update projection matrix
    float aspect = float(w) / float(h);
    m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void TestViewport::paintGL()
{
    if (!m_initialized) return;
    
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update camera
    updateCamera();
    
    // Render simple content
    renderGrid();
    renderAxes();
    renderTestCube();
    
    // Simple animation
    m_time += 0.016f; // Rough 60 FPS
}

void TestViewport::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePos = event->pos();
    m_isDragging = true;
    m_dragButton = event->button();
    
    qDebug() << "TestViewport: Mouse press at" << event->pos() << "button:" << event->button();
}

void TestViewport::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_isDragging) return;
    
    QPoint delta = event->pos() - m_lastMousePos;
    m_lastMousePos = event->pos();
    
    if (m_dragButton == Qt::RightButton) {
        // Orbit camera
        float sensitivity = 0.5f;
        m_cameraYaw += delta.x() * sensitivity;
        m_cameraPitch -= delta.y() * sensitivity;
        
        // Clamp pitch
        m_cameraPitch = qBound(-89.0f, m_cameraPitch, 89.0f);
        
        update();
    } else if (m_dragButton == Qt::MiddleButton) {
        // Pan camera
        float sensitivity = 0.01f;
        glm::vec3 right = glm::normalize(glm::cross(m_cameraPos - m_cameraTarget, m_cameraUp));
        glm::vec3 up = glm::normalize(glm::cross(right, m_cameraPos - m_cameraTarget));
        
        glm::vec3 panDelta = -right * float(delta.x()) * sensitivity + up * float(delta.y()) * sensitivity;
        m_cameraTarget += panDelta;
        
        update();
    }
}

void TestViewport::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
    m_isDragging = false;
    m_dragButton = Qt::NoButton;
    
    qDebug() << "TestViewport: Mouse release";
}

void TestViewport::wheelEvent(QWheelEvent* event)
{
    // Zoom
    float sensitivity = 0.1f;
    float delta = event->angleDelta().y() / 120.0f; // Standard wheel step
    m_cameraDistance -= delta * sensitivity;
    m_cameraDistance = qBound(1.0f, m_cameraDistance, 50.0f);
    
    update();
    
    qDebug() << "TestViewport: Wheel zoom, distance:" << m_cameraDistance;
}

void TestViewport::updateViewport()
{
    update(); // Trigger paintGL
}

void TestViewport::setupCamera()
{
    updateCamera();
}

void TestViewport::updateCamera()
{
    // Convert spherical to cartesian coordinates
    float yawRad = glm::radians(m_cameraYaw);
    float pitchRad = glm::radians(m_cameraPitch);
    
    m_cameraPos.x = m_cameraTarget.x + m_cameraDistance * cos(pitchRad) * cos(yawRad);
    m_cameraPos.y = m_cameraTarget.y + m_cameraDistance * sin(pitchRad);
    m_cameraPos.z = m_cameraTarget.z + m_cameraDistance * cos(pitchRad) * sin(yawRad);
    
    m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraTarget, m_cameraUp);
}

void TestViewport::renderGrid()
{
    // Simple grid rendering using immediate mode (for testing)
    // In production, this would use VBOs and shaders
    
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&m_projMatrix[0][0]);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&m_viewMatrix[0][0]);
    
    // Draw grid lines
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    
    float gridSize = 10.0f;
    int gridLines = 20;
    float step = gridSize / gridLines;
    
    for (int i = -gridLines; i <= gridLines; ++i) {
        float pos = i * step;
        
        // Horizontal lines
        glVertex3f(-gridSize, 0.0f, pos);
        glVertex3f(gridSize, 0.0f, pos);
        
        // Vertical lines
        glVertex3f(pos, 0.0f, -gridSize);
        glVertex3f(pos, 0.0f, gridSize);
    }
    
    glEnd();
}

void TestViewport::renderAxes()
{
    // Draw coordinate axes
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    
    // X axis - Red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 0.0f, 0.0f);
    
    // Y axis - Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.0f, 0.0f);
    
    // Z axis - Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 2.0f);
    
    glEnd();
    glLineWidth(1.0f);
}

void TestViewport::renderTestCube()
{
    // Draw a simple animated cube
    glPushMatrix();
    
    // Animate rotation
    glRotatef(m_time * 20.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(m_time * 15.0f, 1.0f, 0.0f, 0.0f);
    
    // Draw cube wireframe
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glLineWidth(2.0f);
    
    // Simple cube using immediate mode
    glBegin(GL_LINE_LOOP);
    // Front face
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    // Back face
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();
    
    // Connect front and back
    glBegin(GL_LINES);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
    
    glLineWidth(1.0f);
    glPopMatrix();
}

#include "TestViewport.moc"
