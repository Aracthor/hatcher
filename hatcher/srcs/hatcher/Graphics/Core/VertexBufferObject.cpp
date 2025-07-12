#include "VertexBufferObject.hpp"

#include "gl.hpp"

#include "hatcher/assert.hpp"

namespace hatcher
{

VertexBufferObject::VertexBufferObject(bool isElement)
    : m_target(isElement ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER)
{
    GL_CHECK(glGenBuffers(1, &m_id));
}

VertexBufferObject::~VertexBufferObject()
{
    GL_CHECK(glDeleteBuffers(1, &m_id));
}

void VertexBufferObject::SetData(const float* data, uint length, bool dynamic)
{
    const GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    GL_CHECK(glBindBuffer(m_target, m_id));
    GL_CHECK(glBufferData(m_target, length * sizeof(float), data, usage));
    m_elementCount = length;
}

void VertexBufferObject::SetData(const ushort* data, uint length, bool dynamic)
{
    const GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    GL_CHECK(glBindBuffer(m_target, m_id));
    GL_CHECK(glBufferData(m_target, length * sizeof(ushort), data, usage));
    m_elementCount = length;
}

void VertexBufferObject::Bind() const
{
    GL_CHECK(glBindBuffer(m_target, m_id));
}

} // namespace hatcher