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

void VertexArrayObject::Bind()
{
    GL_CHECK(glBindVertexArray(m_id));
}

void VertexArrayObject::Unbind()
{
    GL_CHECK(glBindVertexArray(0));
}

void VertexArrayObject::AttribVBO(const VertexBufferObject& parVBO, GLint componentCount,
                                  GLuint layoutLocation)
{
    HATCHER_ASSERT(IsBinded());

    GL_CHECK(glEnableVertexAttribArray(layoutLocation));
    parVBO.Bind();
    GL_CHECK(glVertexAttribPointer(layoutLocation, componentCount, GL_FLOAT, GL_FALSE, 0, NULL));

    if (m_elementCount == 0)
        m_elementCount = parVBO.ElementCount();
    HATCHER_ASSERT(m_elementCount == parVBO.ElementCount());
}

void VertexArrayObject::DrawArrays() const
{
    GL_CHECK(glBindVertexArray(m_id));
    GL_CHECK(glDrawArrays(m_mode, 0, m_elementCount));
    GL_CHECK(glBindVertexArray(0));
}

void VertexArrayObject::DrawElements(int count) const
{
    GL_CHECK(glBindVertexArray(m_id));
    GL_CHECK(glDrawElements(m_mode, count, GL_UNSIGNED_SHORT, nullptr));
    GL_CHECK(glBindVertexArray(0));
}

bool VertexArrayObject::IsBinded() const
{
    GLint binded;
    GL_CHECK(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &binded));
    return binded == static_cast<GLint>(m_id);
}

} // namespace hatcher
