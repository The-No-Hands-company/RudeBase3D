#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <glm/glm.hpp>

namespace rude {

// Utility function to convert QMatrix4x4 to glm::mat4
inline glm::mat4 qMatrixToGlm(const QMatrix4x4& qMatrix) {
    glm::mat4 result;
    const float* data = qMatrix.constData();
    for (int i = 0; i < 16; ++i) {
        result[i / 4][i % 4] = data[i];
    }
    return result;
}

// Utility function to convert glm::mat4 to QMatrix4x4
inline QMatrix4x4 glmToQMatrix(const glm::mat4& glmMatrix) {
    QMatrix4x4 result;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            result(row, col) = glmMatrix[col][row];
        }
    }
    return result;
}

// Utility function to convert QVector3D to glm::vec3
inline glm::vec3 qVectorToGlm(const QVector3D& qVector) {
    return glm::vec3(qVector.x(), qVector.y(), qVector.z());
}

// Utility function to convert glm::vec3 to QVector3D
inline QVector3D glmToQVector(const glm::vec3& glmVector) {
    return QVector3D(glmVector.x, glmVector.y, glmVector.z);
}

} // namespace rude
