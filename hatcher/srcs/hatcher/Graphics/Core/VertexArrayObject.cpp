#include "VertexArrayObject.hpp"

#include "VertexBufferObject.hpp"
#include "gl.hpp"

#include "hatcher/assert.hpp"

namespace hatcher
{
namespace
{

static_assert(static_cast<int>(Primitive::Count) == 5);
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
    case Primitive::TriangleStrip:
        return GL_TRIANGLE_STRIP;
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

void VertexArrayObject::AttribInstancedVBO(const VertexBufferObject& parVBO, GLint componentCount,
                                           GLuint layoutLocation)
{
    HATCHER_ASSERT(IsBinded());

    if (componentCount <= 4)
    {
        AttribVBO(parVBO, componentCount, layoutLocation);
        GL_CHECK(glVertexAttribDivisor(layoutLocation, 1));
    }
    else
    {
        parVBO.Bind();
        HATCHER_ASSERT(componentCount % 4 == 0);
        const int layoutSize = 4;
        const int stride = componentCount * 4;
        const int subLayoutCount = componentCount / 4;
        int offset = 0;
        for (int subLayoutIndex = 0; subLayoutIndex < subLayoutCount; subLayoutIndex++)
        {
            const int subLayoutLocation = layoutLocation + subLayoutIndex;
            const void* pointer = reinterpret_cast<const void*>(offset);
            GL_CHECK(glEnableVertexAttribArray(subLayoutLocation));
            GL_CHECK(glVertexAttribPointer(subLayoutLocation, layoutSize, GL_FLOAT, GL_FALSE, stride, pointer));
            GL_CHECK(glVertexAttribDivisor(subLayoutLocation, 1));
            offset += 16;
        }
    }
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

void VertexArrayObject::DrawArraysInstanced(int count, int instanceCount) const
{
    GL_CHECK(glBindVertexArray(m_id));
    GL_CHECK(glDrawArraysInstanced(m_mode, 0, count, instanceCount));
    GL_CHECK(glBindVertexArray(0));
}

void VertexArrayObject::DrawElementsInstanced(int count, int instanceCount) const
{
    GL_CHECK(glBindVertexArray(m_id));
    GL_CHECK(glDrawElementsInstanced(m_mode, count, GL_UNSIGNED_SHORT, nullptr, instanceCount));
    GL_CHECK(glBindVertexArray(0));
}

bool VertexArrayObject::IsBinded() const
{
    GLint binded;
    GL_CHECK(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &binded));
    return binded == static_cast<GLint>(m_id);
}

} // namespace hatcher
