#pragma once

namespace MeshLib
{
class VertexBuffer
{
public:
	/*!
	 *  Constructor.
	 *    Generate a buffer on GPU, and bind it on the type `GL_ARRAY_BUFFER`,
	 *    transfer the data from CPU to GPU.
	 *  \param[in] data: the data on the memory of PC.
	 *  \param[in] size: the size in bytes of the data.
	 */
	VertexBuffer(const void* data, size_t vertex_count, size_t size_per_vertex);

	/*!
	 *  Destructor.
	 *    Delete the buffer on GPU.
	 */
	~VertexBuffer();

	/*!
	 *  Bind the buffer with `GL_ARRAY_BUFFER`.
	 */
	void bind() const;

	/*!
	 *  Unbind the buffer.
	 */
	void unbind() const;

	size_t vertex_count() const;
protected:
	/*!
	 *  Buffer id.
	 */
	unsigned int m_id;

	size_t m_vertex_count;
};
}
