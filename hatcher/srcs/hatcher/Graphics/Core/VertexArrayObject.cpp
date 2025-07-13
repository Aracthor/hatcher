#include "VertexArrayObject.hpp"

#include "VertexBufferObject.hpp"
#include "gl.hpp"

#include "hatcher/assert.hpp"

namespace hatcher
{
namespace
{

static_assert(static_cast<int>(Primitive::Count) == 4);
GLenum PrimitiveToGLPrimitive(Primitive type)
{
    switch (type)
    {
    case Primitive::Lines:
        return GL_LINES;
    case Primitive::LineStrip:
        return GL_LINE_STRIP;
    case Primitive::Triangles:
        return GL_TRIANGLES;
    case Primitive::TriangleFan:
        return GL_TRIANGLE_FAN;
    default:
        HATCHER_ASSERT_MESSAGE(false, "Unknown primitive type.");
        return -1;
    }
}

} // namespace

VertexArrayObject::VertexArrayObject(Primitive primitive)
    : m_mode(PrimitiveToGLPrimitive(primitive))
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

void VertexArrayObject::AttribVBO(const VertexBufferObject& parVBO, GLint componentCount, GLuint layoutLocation)
{
    HATCHER_ASSERT(IsBinded());

    GL_CHECK(glEnableVertexAttribArray(layoutLocation));
    parVBO.Bind();
    GL_CHECK(glVertexAttribPointer(layoutLocation, componentCount, GL_FLOAT, GL_FALSE, 0, NULL));
}

void VertexArrayObject::DrawArrays(int count) const
{
    GL_CHECK(glBindVertexArray(m_id));
    GL_CHECK(glDrawArrays(m_mode, 0, count));
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
