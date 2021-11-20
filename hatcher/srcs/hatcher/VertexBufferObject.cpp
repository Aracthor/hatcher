#include "VertexBufferObject.hpp"

#include "assert.hpp"
#include "gl.hpp"

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

void VertexBufferObject::SetData(float* data, unsigned int length)
{
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), data, GL_STATIC_DRAW));
    HATCHER_ASSERT(length / 3 * 3 == length);
    m_elementCount = length / 3;
}

void VertexBufferObject::Bind() const
{
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

} // namespace hatcher