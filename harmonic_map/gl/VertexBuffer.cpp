#include "VertexBuffer.h"

#include <glad/glad.h>

namespace MeshLib
{

VertexBuffer::VertexBuffer(const void* data, size_t vertex_count, size_t size_per_vertex)
{
    m_vertex_count = vertex_count;
    size_t size = vertex_count * size_per_vertex;
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

size_t VertexBuffer::vertex_count() const
{
    return m_vertex_count;
}
}
