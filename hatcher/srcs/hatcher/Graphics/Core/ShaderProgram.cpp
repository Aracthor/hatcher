#include "ShaderProgram.hpp"

#include <fstream>

#include "gl.hpp"
#include "hatcher/assert.hpp"
#include "hatcher/basic_types.hpp"

namespace hatcher
{
namespace
{

GLuint CompileShader(const char* parShaderFileName, GLenum parShaderType)
{
    char* fileContent;
    try
    {
        std::ifstream ifs;
        ifs.exceptions(std::ios::failbit | std::ios::badbit);

        ifs.open(parShaderFileName, std::ifstream::in | std::ifstream::binary);

        ifs.seekg(0, ifs.end);
        uint fileSize = ifs.tellg();
        ifs.seekg(0, ifs.beg);
        fileContent = new char[fileSize + 1];
        ifs.read(fileContent, fileSize);
        fileContent[fileSize] = '\0';
    }
    catch (const std::exception& exception)
    {
        throw std::runtime_error(std::string("Error reading shader file '") + parShaderFileName + "':\n" +
                                 exception.what());
    }

    GLint compiled;
    GLuint shaderID = glCreateShader(parShaderType);
    GL_CHECK(glShaderSource(shaderID, 1, &fileContent, NULL));
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
            throw std::runtime_error(std::string("Unkown error compiling shader '") + parShaderFileName + "':\n" +
                                     infoLog + "\n" + fileContent);
            delete[] infoLog;
        }
        else
        {
            throw std::runtime_error(std::string("Unkown error compiling shader '") + parShaderFileName + "'");
        }
    }
    delete[] fileContent;

    return shaderID;
}

} // namespace

ShaderProgram::ShaderProgram(const char* parVertexShaderFileName, const char* parFragmentShaderFileName)
{
    m_vertexShaderID = CompileShader(parVertexShaderFileName, GL_VERTEX_SHADER);
    m_fragmentShaderID = CompileShader(parFragmentShaderFileName, GL_FRAGMENT_SHADER);

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
            throw std::runtime_error(std::string("Error linking program:\n") + infoLog);
        }
        else
        {
            throw std::runtime_error("Unkown error linking program.");
        }
    }
}

ShaderProgram::~ShaderProgram()
{
    GL_CHECK(glDeleteProgram(m_programID));
    GL_CHECK(glDeleteShader(m_vertexShaderID));
    GL_CHECK(glDeleteShader(m_fragmentShaderID));
}

void ShaderProgram::Use() const
{
    GL_CHECK(glUseProgram(m_programID));
}

bool ShaderProgram::IsCurrentlyUsed() const
{
    GLint currentProgramID;
    GL_CHECK(glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgramID));
    return (currentProgramID == static_cast<GLint>(m_programID));
}

void ShaderProgram::SetFloatUniform(const char* name, float value) const
{
    HATCHER_ASSERT(IsCurrentlyUsed());
    // TODO cache uniform locations ?
    GLint uniformLocation;
    GL_CHECK(uniformLocation = glGetUniformLocation(m_programID, name));
    HATCHER_ASSERT_MESSAGE(uniformLocation >= 0, "uniform name: " << name);
    GL_CHECK(glUniform1f(uniformLocation, value));
}

void ShaderProgram::SetVector4Uniform(const char* name, const float* vectorPtr) const
{
    HATCHER_ASSERT(IsCurrentlyUsed());
    // TODO cache uniform locations ?
    GLint uniformLocation;
    GL_CHECK(uniformLocation = glGetUniformLocation(m_programID, name));
    HATCHER_ASSERT_MESSAGE(uniformLocation >= 0, "uniform name: " << name);
    GL_CHECK(glUniform4fv(uniformLocation, 1, vectorPtr));
}

void ShaderProgram::SetMatrix4Uniform(const char* name, const float* matrixPtr) const
{
    HATCHER_ASSERT(IsCurrentlyUsed());
    // TODO cache uniform locations ?
    GLint uniformLocation;
    GL_CHECK(uniformLocation = glGetUniformLocation(m_programID, name));
    HATCHER_ASSERT_MESSAGE(uniformLocation >= 0, "uniform name: " << name);
    GL_CHECK(glUniformMatrix4fv(uniformLocation, 1, false, matrixPtr));
}

GLint ShaderProgram::GetAttribLocation(const char* name) const
{
    GLint attribLocation;
    GL_CHECK(attribLocation = glGetAttribLocation(m_programID, name));
    HATCHER_ASSERT_MESSAGE(attribLocation >= 0, "attrib name: " << name);
    return attribLocation;
}

} // namespace hatcher
