#include "ShaderProgram.hpp"

#include "gl.hpp"
#include <iostream>

namespace hatcher
{
namespace
{

GLuint CompileShader(const char* parShaderCode, GLenum parShaderType)
{
    GLint compiled;
    GLuint shaderID = glCreateShader(parShaderType);
    GL_CHECK(glShaderSource(shaderID, 1, &parShaderCode, NULL));
    GL_CHECK(glCompileShader(shaderID));
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = new char[infoLen];
            glGetShaderInfoLog(shaderID, infoLen, NULL, infoLog);
            std::cerr << "Error compiling vertex shader:" << std::endl << infoLog << std::endl;
            delete[] infoLog;
        }
        else
        {
            std::cerr << "Unknown compiling vertex shader." << std::endl;
        }
        std::terminate();
    }

    return shaderID;
}

} // namespace

ShaderProgram::ShaderProgram(const char* parVertexShader, const char* parFragmentShader)
{
    m_vertexShaderID = CompileShader(parVertexShader, GL_VERTEX_SHADER);
    m_fragmentShaderID = CompileShader(parFragmentShader, GL_FRAGMENT_SHADER);

    m_programID = glCreateProgram();
    GL_CHECK(glAttachShader(m_programID, m_vertexShaderID));
    GL_CHECK(glAttachShader(m_programID, m_fragmentShaderID));
    GL_CHECK(glLinkProgram(m_programID));

    GLint linked;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = new char[infoLen];
            glGetProgramInfoLog(m_programID, infoLen, NULL, infoLog);
            std::cerr << "Error linking program:" << std::endl << infoLog << std::endl;
            delete[] infoLog;
        }
        else
        {
            std::cerr << "Unkown error linking program." << std::endl;
        }
        std::terminate();
    }
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_programID);
    glDeleteShader(m_vertexShaderID);
    glDeleteShader(m_fragmentShaderID);
}

void ShaderProgram::Use() const
{
    GL_CHECK(glUseProgram(m_programID));
}

} // namespace hatcher
