#pragma once

namespace MeshLib
{
class ElementBuffer
{
public:
	/*!
	 *  Constructor.
	 *    Generate the buffer on GPU, and bind it on the type
	 *    `GL_ELEMENT_ARRAY_BUFFER`, transfer the data from CPU to GPU.
	 *  \param[in]  data: the data on the memory of PC.
	 *  \param[in] count: the number of the unsigned integers.
	 */
	ElementBuffer(const void* data, unsigned int count);

	/*!
	 *  Destructor.
	 *    Delete the buffer on GPU.
	 */
	~ElementBuffer();

	/*!
	 *  Bind the buffer with `GL_ELEMENT_ARRAY_BUFFER`.
	 */
	void bind() const;

	/*!
	 *  Unbind the buffer.
	 */
	void unbind() const;

	/*!
	 *  The number of the indices.
	 *  \return the number of the indices, e.x. num_triangle * 3.
	 */
	unsigned int count() const { return m_count; };

protected:
	/*!
	 *  Buffer id.
	 */
	unsigned int m_id;

	/*!
	 *  The number of the indices(unsigned integers).
	 *    E.x. if there are n triangles to be rendered, then the m_count should
	 *         be n * 3.
	 */
	unsigned int m_count;
};
}
