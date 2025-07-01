#include "Transform.h"
#include <QMatrix3x3>
#include <cmath>

Transform::Transform()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_rotation(QQuaternion())
    , m_scale(1.0f, 1.0f, 1.0f)
    , m_matrixDirty(true)
{
}

void Transform::setPosition(const QVector3D& position)
{
    m_position = position;
    m_matrixDirty = true;
}

void Transform::setRotation(const QQuaternion& rotation)
{
    m_rotation = rotation.normalized();
    m_matrixDirty = true;
}

void Transform::setEulerAngles(const QVector3D& euler)
{
    QQuaternion qx = QQuaternion::fromAxisAndAngle(1, 0, 0, euler.x());
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, euler.y());
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, euler.z());
    m_rotation = qy * qx * qz;
    m_matrixDirty = true;
}

QVector3D Transform::getEulerAngles() const
{
    // Convert quaternion to Euler angles (YXZ order)
    const float w = m_rotation.scalar();
    const float x = m_rotation.x();
    const float y = m_rotation.y();
    const float z = m_rotation.z();
    
    const float test = x * y + z * w;
    
    if (test > 0.499f) { // singularity at north pole
        return QVector3D(
            90.0f,
            2.0f * atan2(x, w) * 180.0f / PI,
            0.0f
        );
    }
    if (test < -0.499f) { // singularity at south pole
        return QVector3D(
            -90.0f,
            -2.0f * atan2(x, w) * 180.0f / PI,
            0.0f
        );
    }
    
    const float sqx = x * x;
    const float sqy = y * y;
    const float sqz = z * z;
    
    return QVector3D(
        asin(2.0f * test) * 180.0f / PI,
        atan2(2.0f * y * w - 2.0f * x * z, 1.0f - 2.0f * sqy - 2.0f * sqz) * 180.0f / PI,
        atan2(2.0f * x * w - 2.0f * y * z, 1.0f - 2.0f * sqx - 2.0f * sqz) * 180.0f / PI
    );
}

void Transform::setScale(const QVector3D& scale)
{
    m_scale = scale;
    m_matrixDirty = true;
}

void Transform::setUniformScale(float scale)
{
    m_scale = QVector3D(scale, scale, scale);
    m_matrixDirty = true;
}

void Transform::translate(const QVector3D& translation)
{
    m_position += translation;
    m_matrixDirty = true;
}

void Transform::rotate(const QQuaternion& rotation)
{
    m_rotation = (rotation * m_rotation).normalized();
    m_matrixDirty = true;
}

void Transform::rotateAround(const QVector3D& axis, float angle)
{
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(axis, angle);
    rotate(rotation);
}

void Transform::scale(const QVector3D& scale)
{
    m_scale *= scale;
    m_matrixDirty = true;
}

void Transform::lookAt(const QVector3D& target, const QVector3D& up)
{
    QVector3D forward = (target - m_position).normalized();
    QVector3D right = QVector3D::crossProduct(forward, up).normalized();
    QVector3D actualUp = QVector3D::crossProduct(right, forward).normalized();
    
    // Create rotation matrix from basis vectors
    QMatrix3x3 rotMatrix;
    rotMatrix(0, 0) = right.x();   rotMatrix(0, 1) = right.y();   rotMatrix(0, 2) = right.z();
    rotMatrix(1, 0) = actualUp.x(); rotMatrix(1, 1) = actualUp.y(); rotMatrix(1, 2) = actualUp.z();
    rotMatrix(2, 0) = -forward.x(); rotMatrix(2, 1) = -forward.y(); rotMatrix(2, 2) = -forward.z();
    
    // Convert to quaternion
    m_rotation = QQuaternion::fromRotationMatrix(rotMatrix);
    m_matrixDirty = true;
}

QMatrix4x4 Transform::getModelMatrix() const
{
    if (m_matrixDirty) {
        updateMatrix();
    }
    return m_modelMatrix;
}

QMatrix4x4 Transform::getInverseModelMatrix() const
{
    return getModelMatrix().inverted();
}

QVector3D Transform::getForward() const
{
    return m_rotation.rotatedVector(WORLD_FORWARD);
}

QVector3D Transform::getRight() const
{
    return m_rotation.rotatedVector(WORLD_RIGHT);
}

QVector3D Transform::getUp() const
{
    return m_rotation.rotatedVector(WORLD_UP);
}

void Transform::reset()
{
    m_position = QVector3D(0.0f, 0.0f, 0.0f);
    m_rotation = QQuaternion();
    m_scale = QVector3D(1.0f, 1.0f, 1.0f);
    m_matrixDirty = true;
}

void Transform::updateMatrix() const
{
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_position);
    m_modelMatrix.rotate(m_rotation);
    m_modelMatrix.scale(m_scale);
    m_matrixDirty = false;
}
