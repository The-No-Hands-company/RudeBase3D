#pragma once

#include "Common.h"
#include <QMatrix4x4>

class Transform {
public:
    Transform();
    ~Transform() = default;

    // Position
    void setPosition(const QVector3D& position);
    const QVector3D& getPosition() const { return m_position; }
    
    // Rotation
    void setRotation(const QQuaternion& rotation);
    const QQuaternion& getRotation() const { return m_rotation; }
    void setEulerAngles(const QVector3D& euler);
    QVector3D getEulerAngles() const;
    
    // Scale
    void setScale(const QVector3D& scale);
    const QVector3D& getScale() const { return m_scale; }
    void setUniformScale(float scale);
    
    // Transformation methods
    void translate(const QVector3D& translation);
    void rotate(const QQuaternion& rotation);
    void rotateAround(const QVector3D& axis, float angle);
    void scale(const QVector3D& scale);
    
    // Look at target
    void lookAt(const QVector3D& target, const QVector3D& up = QVector3D(0, 1, 0));
    
    // Matrix generation
    QMatrix4x4 getModelMatrix() const;
    QMatrix4x4 getInverseModelMatrix() const;
    
    // Forward, right, up vectors
    QVector3D getForward() const;
    QVector3D getRight() const;
    QVector3D getUp() const;
    
    // Reset to identity
    void reset();

private:
    QVector3D m_position;
    QQuaternion m_rotation;
    QVector3D m_scale;
    
    mutable bool m_matrixDirty;
    mutable QMatrix4x4 m_modelMatrix;
    
    void updateMatrix() const;
};
