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

void VertexBufferObject::SetData(float* data, uint length)
{
    m_target = GL_ARRAY_BUFFER;
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), data, GL_STATIC_DRAW));
    m_elementCount = length;
}

void VertexBufferObject::SetData(ushort* data, uint length)
{
    m_target = GL_ELEMENT_ARRAY_BUFFER;
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(ushort), data, GL_STATIC_DRAW));
    m_elementCount = length;
}

void VertexBufferObject::Bind() const
{
    GL_CHECK(glBindBuffer(m_target, m_id));
}

} // namespace hatcher