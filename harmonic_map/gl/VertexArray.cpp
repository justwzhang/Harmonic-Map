#include "VertexArray.h"
#include <glad/glad.h>

namespace MeshLib
{

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

void VertexArray::add_buffer(const VertexBuffer& vb, const VertexAttribLayout& layout)
{
    // after bind the vertex array object, the following vertex buffer will act on it.
    this->bind();
    vb.bind();
    auto& vertex_attribs = layout.vertex_attribs();
    unsigned long long offset = 0;
    for (unsigned int i = 0; i < vertex_attribs.size(); ++i)
    {
        const auto& attrib = vertex_attribs[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, attrib.count, attrib.type, attrib.normalized, 
                              layout.stride(), (const void*)(offset));
        offset += attrib.count * layout.size_of(attrib.type);
    }
}

void VertexArray::bind() const
{
	glBindVertexArray(m_id);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
}
