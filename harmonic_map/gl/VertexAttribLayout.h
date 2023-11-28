#pragma once

#include <glad/glad.h>
#include <vector>

namespace MeshLib
{

enum VertexAttribType
{ 
	FLOAT         = GL_FLOAT,  
	UNSIGNED_INT  = GL_UNSIGNED_INT,
	UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
};

struct VertexAttrib
{
	VertexAttribType type;
	unsigned int     count;
	unsigned char    normalized;
};

/*!
 *  Class VertexAttribLayout is used to explain how to layout the data 
 *    transfered into GPU, this class will be used in VAO(class VertexArray).
 */
class VertexAttribLayout
{
public:
	/*!
	 *  Constructor. 
	 */
	VertexAttribLayout();

	/*!
	 *  Destructor.
	 */
	~VertexAttribLayout();

	/*!
	 *  Push the data type and the number of the data in the order of the layout
	 *    in the vertex shader language.
	 *  \param[in] count: the number of the data.
	 *  \param[in]  type: the data type.
	 */
	void push(const unsigned int count, const VertexAttribType& type);

	const std::vector<VertexAttrib>& vertex_attribs() const { return m_vertex_attribs; };

	unsigned int stride() const { return m_stride; };

	/*!
	 *  Get the size of bytes of the given type.
	 *  \param[in] type: the given data type of vertex attribution.
	 */
	unsigned int size_of(const VertexAttribType& type) const;
protected:
	std::vector<VertexAttrib> m_vertex_attribs;
	unsigned int m_stride;
};
}