//==============================================================================
// RudeBase3D Engine - qt_glm_utils.hpp
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================

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
