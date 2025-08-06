#pragma once
#include <string>
#include <QOpenGLFunctions_4_5_Core>
 
GLuint loadShaderProgram(const std::string& vertPath, const std::string& fragPath, QOpenGLFunctions_4_5_Core* f); 