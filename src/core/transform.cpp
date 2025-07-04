#include "core/transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Transform::toMatrix() const {
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), position);
    mat *= glm::mat4_cast(rotation);
    mat = glm::scale(mat, scale);
    return mat;
} 