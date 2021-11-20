#include "VertexArrayObject.hpp"

#include "VertexBufferObject.hpp"
#include "assert.hpp"
#include "gl.hpp"

namespace hatcher
{

VertexArrayObject::VertexArrayObject()
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

void VertexArrayObject::Draw() const
{
    GL_CHECK(glBindVertexArray(m_id));
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, m_elementCount));
}

} // namespace hatcher