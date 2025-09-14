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

    void SetFloatUniform(const char* name, float value) const;
    void SetVector4Uniform(const char* name, const float* vectorPtr) const;
    void SetMatrix4Uniform(const char* name, const float* matrixPtr) const;

    GLint GetAttribLocation(const char* name) const;
    bool HasAttribute(const char* name) const;

private:
    GLuint m_vertexShaderID;
    GLuint m_fragmentShaderID;
    GLuint m_programID;
};

} // namespace hatcher