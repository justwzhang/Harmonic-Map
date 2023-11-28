#include "VertexAttribLayout.h"
#include <glad/glad.h>

namespace MeshLib
{

VertexAttribLayout::VertexAttribLayout() : m_stride(0)
{
}

VertexAttribLayout::~VertexAttribLayout()
{
}

void VertexAttribLayout::push(const unsigned int count, const VertexAttribType& type)
{
	m_vertex_attribs.push_back({ type, count, GL_FALSE });
	m_stride += count * size_of(type);
}

unsigned int VertexAttribLayout::size_of(const VertexAttribType& type) const
{
    switch (type)
    {
    case GL_FLOAT:
        return 4;
    case GL_UNSIGNED_INT:
        return 4;
    case GL_UNSIGNED_BYTE:
        return 1;
    default:
        return 0;
    }
}

}
