#pragma once

#include "gl_types.hpp"

namespace hatcher
{

class VertexBufferObject
{
public:
    VertexBufferObject();
    ~VertexBufferObject();

    void SetData(float* data, unsigned int length);

    void Bind() const;
    unsigned int ElementCount() const { return m_elementCount; }

private:
    GLuint m_id;
    unsigned int m_elementCount = 0;
};

} // namespace hatcher
