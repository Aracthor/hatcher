#include "ShaderProgram.hpp"

#include "gl.hpp"
#include <fstream>
#include <iostream>

namespace hatcher
{
namespace
{

GLuint CompileShader(const char* parShaderFileName, GLenum parShaderType)
{
    std::ifstream ifs;

    ifs.open(parShaderFileName, std::ifstream::in | std::ifstream::binary);
    if (ifs.rdstate() & std::ios::failbit)
    {
        std::cerr << "Couldn't open file '" << parShaderFileName << "'." << std::endl;
        std::terminate();
    }

    ifs.seekg(0, ifs.end);
    unsigned int fileSize = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char* fileContent = new char[fileSize + 1];
    ifs.read(fileContent, fileSize);
    fileContent[fileSize] = '\0';

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
            std::cerr << "Error compiling shader '" << parShaderFileName << "':" << std::endl
                      << infoLog << std::endl
                      << fileContent << std::endl;
            delete[] infoLog;
        }
        else
        {
            std::cerr << "Unkown error compiling shader '" << parShaderFileName
                      << "':" << std::endl;
        }
        std::terminate();
    }
    delete[] fileContent;

    return shaderID;
}

} // namespace

ShaderProgram::ShaderProgram(const char* parVertexShaderFileName,
                             const char* parFragmentShaderFileName)
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
