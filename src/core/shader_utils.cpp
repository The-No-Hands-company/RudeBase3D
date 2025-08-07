#include "core/shader_utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint loadShaderProgram(const std::string& vertPath, const std::string& fragPath, QOpenGLFunctions_4_5_Core* f) {
    auto compileShader = [&](const std::string& src, GLenum type) -> GLuint {
        GLuint shader = f->glCreateShader(type);
        const char* srcCStr = src.c_str();
        f->glShaderSource(shader, 1, &srcCStr, nullptr);
        f->glCompileShader(shader);
        GLint success;
        f->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            f->glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compile error: " << infoLog << std::endl;
        }
        return shader;
    };

    std::string vertSrc = readFile(vertPath);
    std::string fragSrc = readFile(fragPath);

    GLuint vertShader = compileShader(vertSrc, GL_VERTEX_SHADER);
    GLuint fragShader = compileShader(fragSrc, GL_FRAGMENT_SHADER);

    GLuint program = f->glCreateProgram();
    f->glAttachShader(program, vertShader);
    f->glAttachShader(program, fragShader);
    f->glLinkProgram(program);

    GLint success;
    f->glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        f->glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader link error: " << infoLog << std::endl;
    }

    f->glDeleteShader(vertShader);
    f->glDeleteShader(fragShader);

    return program;
} 
