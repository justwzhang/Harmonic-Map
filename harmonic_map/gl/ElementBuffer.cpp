#include "ElementBuffer.h"

#include <glad/glad.h>

namespace MeshLib
{
ElementBuffer::ElementBuffer(const void* data, unsigned int count) : m_count(count)
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void ElementBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}
