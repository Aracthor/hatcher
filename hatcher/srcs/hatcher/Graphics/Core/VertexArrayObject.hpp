#pragma once

#include "gl_types.hpp"

#include "hatcher/Graphics/Primitive.hpp"
#include "hatcher/basic_types.hpp"

namespace hatcher
{
class VertexBufferObject;

class VertexArrayObject
{
public:
    VertexArrayObject(Primitive primitive);
    ~VertexArrayObject();

    void Bind();
    void Unbind();

    void AttribVBO(const VertexBufferObject& parVBO, GLint componentCount, GLuint layoutLocation);
    void AttribInstancedVBO(const VertexBufferObject& parVBO, GLint componentCount, GLuint layoutLocation);

    void DrawArrays(int count) const;
    void DrawElements(int count) const;
    void DrawArraysInstanced(int count, int instanceCount) const;
    void DrawElementsInstanced(int count, int instanceCount) const;

private:
    bool IsBinded() const;

    GLuint m_id;
    GLenum m_mode;
};

} // namespace hatcher
