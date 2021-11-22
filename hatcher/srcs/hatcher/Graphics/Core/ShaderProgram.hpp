#pragma once

#include "gl_types.hpp"

namespace hatcher
{

class ShaderProgram
{
public:
    ShaderProgram(const char* parVertexShaderFileName, const char* parFragmentShaderFileName);
    ~ShaderProgram();

    void Use() const;
    bool IsCurrentlyUsed() const;

    void SetMatrix4Uniform(const char* name, float* matrixPtr) const;

private:
    GLuint m_vertexShaderID;
    GLuint m_fragmentShaderID;
    GLuint m_programID;
};

} // namespace hatcher