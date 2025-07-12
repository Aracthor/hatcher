#pragma once

#include "gl_types.hpp"

#include "hatcher/basic_types.hpp"

namespace hatcher
{

class VertexBufferObject
{
public:
    VertexBufferObject(bool isElement);
    ~VertexBufferObject();

    void SetData(const float* data, int length, bool dynamic);
    void SetData(const ushort* data, int length, bool dynamic);

    void Bind() const;
    int ElementCount() const { return m_elementCount; }

private:
    GLuint m_id;
    GLenum m_target = 0;
    int m_elementCount = 0;
};

} // namespace hatcher
