
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cmath>

Transform::Transform()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_rotation(1.0f, 0.0f, 0.0f, 0.0f) // identity quaternion
    , m_scale(1.0f, 1.0f, 1.0f)
    , m_matrixDirty(true)
{
}

void Transform::setPosition(const glm::vec3& position)
{
    m_position = position;
    m_matrixDirty = true;
}

void Transform::setRotation(const glm::quat& rotation)
{
    m_rotation = glm::normalize(rotation);
    m_matrixDirty = true;
}

void Transform::setEulerAngles(const glm::vec3& euler)
{
    // euler is in degrees, convert to radians
    glm::vec3 radians = glm::radians(euler);
    m_rotation = glm::quat(radians);
    m_matrixDirty = true;
}

glm::vec3 Transform::getEulerAngles() const
{
    glm::vec3 euler = glm::eulerAngles(m_rotation);
    return glm::degrees(euler);
}

void Transform::setScale(const glm::vec3& scale)
{
    m_scale = scale;
    m_matrixDirty = true;
}

void Transform::setUniformScale(float scale)
{
    m_scale = glm::vec3(scale);
    m_matrixDirty = true;
}

void Transform::translate(const glm::vec3& translation)
{
    m_position += translation;
    m_matrixDirty = true;
}

void Transform::rotate(const glm::quat& rotation)
{
    m_rotation = glm::normalize(rotation * m_rotation);
    m_matrixDirty = true;
}

void Transform::rotateAround(const glm::vec3& axis, float angle)
{
    // angle in degrees, convert to radians
    glm::quat q = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
    rotate(q);
}

void Transform::scale(const glm::vec3& scale)
{
    m_scale *= scale;
    m_matrixDirty = true;
}

void Transform::lookAt(const glm::vec3& target, const glm::vec3& up)
{
    glm::mat4 lookAtMat = glm::lookAt(m_position, target, up);
    m_rotation = glm::quat_cast(glm::inverse(lookAtMat));
    m_matrixDirty = true;
}

glm::mat4 Transform::getModelMatrix() const
{
    if (m_matrixDirty) {
        updateMatrix();
    }
    return m_modelMatrix;
}

glm::mat4 Transform::getInverseModelMatrix() const
{
    return glm::inverse(getModelMatrix());
}

glm::vec3 Transform::getForward() const
{
    return glm::normalize(m_rotation * glm::vec3(0, 0, -1));
}

glm::vec3 Transform::getRight() const
{
    return glm::normalize(m_rotation * glm::vec3(1, 0, 0));
}

glm::vec3 Transform::getUp() const
{
    return glm::normalize(m_rotation * glm::vec3(0, 1, 0));
}

void Transform::reset()
{
    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    m_matrixDirty = true;
}

void Transform::updateMatrix() const
{
    m_modelMatrix = glm::mat4(1.0f);
    m_modelMatrix = glm::translate(m_modelMatrix, m_position);
    m_modelMatrix *= glm::toMat4(m_rotation);
    m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
    m_matrixDirty = false;
}
