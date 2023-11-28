#pragma once

#include "VertexBuffer.h"
#include "VertexAttribLayout.h"

namespace MeshLib
{
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();
    
    /*!
     *  
     */
    void add_buffer(const VertexBuffer& vb, const VertexAttribLayout& layout);

    /*!
     *  Bind the vertex array object.
     */
    void bind() const;

    /*!
     *  Unbind the vertex array object.
     */
    void unbind() const;
protected:
    /*!
     *  Buffer id.
     */
    unsigned int m_id;
};
}
