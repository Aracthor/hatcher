#pragma once

#include <GL/glew.h>

namespace hatcher
{

class ShaderProgram
{
public:
    ShaderProgram(const char* parVertexShader, const char* parFragmentShader);
    ~ShaderProgram();

    void Use() const;

private:
    GLuint m_vertexShaderID;
    GLuint m_fragmentShaderID;
    GLuint m_programID;
};

} // namespace hatcher