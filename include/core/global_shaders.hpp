#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QDebug>

class GlobalShaders : public QOpenGLFunctions_3_3_Core {
public:
    static GlobalShaders& getInstance();
    bool initialize();
    GLuint getMeshShader() const;

private:
    GlobalShaders();
    ~GlobalShaders();
    GlobalShaders(const GlobalShaders&) = delete;
    GlobalShaders& operator=(const GlobalShaders&) = delete;

    GLuint meshShaderProgram;
    bool initialized;
};