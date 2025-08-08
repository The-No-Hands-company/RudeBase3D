#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Simple test viewport to verify basic OpenGL rendering
 * 
 * This is a minimal viewport implementation to test if basic OpenGL
 * rendering, camera controls, and grid display work correctly.
 */
class TestViewport : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit TestViewport(QWidget* parent = nullptr);
    ~TestViewport() override = default;

protected:
    // OpenGL lifecycle
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Mouse and keyboard events
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private slots:
    void updateViewport();

private:
    // Simple camera system
    void setupCamera();
    void updateCamera();
    
    // Simple rendering
    void renderGrid();
    void renderAxes();
    void renderTestCube();
    
    // Camera state
    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraTarget;
    glm::vec3 m_cameraUp;
    float m_cameraDistance;
    float m_cameraYaw;
    float m_cameraPitch;
    
    // Matrices
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projMatrix;
    
    // Mouse interaction
    QPoint m_lastMousePos;
    bool m_isDragging;
    Qt::MouseButton m_dragButton;
    
    // Animation timer
    QTimer* m_updateTimer;
    float m_time;
    
    // OpenGL state
    bool m_initialized;
};
