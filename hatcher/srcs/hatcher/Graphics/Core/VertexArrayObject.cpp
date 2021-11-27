#include "VertexArrayObject.hpp"

#include "VertexBufferObject.hpp"
#include "gl.hpp"

#include "hatcher/assert.hpp"

namespace hatcher
{

static_assert(Primitive::Count == 2);
VertexArrayObject::VertexArrayObject(Primitive::Type primitive)
    : m_mode((primitive == Primitive::Lines) ? GL_LINES : GL_TRIANGLES)
{
    GL_CHECK(glGenVertexArrays(1, &m_id));
}

VertexArrayObject::~VertexArrayObject()
{
    GL_CHECK(glDeleteVertexArrays(1, &m_id));
}

void VertexArrayObject::AttribVBO(const VertexBufferObject& parVBO, GLuint parLayoutLocation)
{
    GL_CHECK(glBindVertexArray(m_id));
    GL_CHECK(glEnableVertexAttribArray(parLayoutLocation));
    parVBO.Bind();
    GL_CHECK(glVertexAttribPointer(parLayoutLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL));
    if (m_elementCount == 0)
        m_elementCount = parVBO.ElementCount();
    HATCHER_ASSERT(m_elementCount == parVBO.ElementCount());
}

void VertexArrayObject::DrawArrays() const
{
    GL_CHECK(glBindVertexArray(m_id));
    GL_CHECK(glDrawArrays(m_mode, 0, m_elementCount));
}

void VertexArrayObject::DrawElements(int count) const
{
    GL_CHECK(glBindVertexArray(m_id));
    GL_CHECK(glDrawElements(m_mode, count, GL_UNSIGNED_SHORT, nullptr));
}

} // namespace hatcher