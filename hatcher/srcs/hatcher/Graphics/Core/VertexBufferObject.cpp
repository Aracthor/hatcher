#include "VertexBufferObject.hpp"

#include "gl.hpp"

#include "hatcher/assert.hpp"

namespace hatcher
{

VertexBufferObject::VertexBufferObject()
{
    GL_CHECK(glGenBuffers(1, &m_id));
}

VertexBufferObject::~VertexBufferObject()
{
    GL_CHECK(glDeleteBuffers(1, &m_id));
}

void VertexBufferObject::SetData(float* data, uint length, bool dynamic)
{
    const GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    m_target = GL_ARRAY_BUFFER;
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), data, usage));
    m_elementCount = length;
}

void VertexBufferObject::SetData(ushort* data, uint length, bool dynamic)
{
    const GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    m_target = GL_ELEMENT_ARRAY_BUFFER;
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(ushort), data, usage));
    m_elementCount = length;
}

void VertexBufferObject::Bind() const
{
    GL_CHECK(glBindBuffer(m_target, m_id));
}

} // namespace hatcher