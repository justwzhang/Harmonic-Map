#pragma once

#include <string>

namespace MeshLib
{
class Texture
{
public:
    /*!
     *  Construtor. 
     *    Load an image into the memory, and generate a buffer on GPU, bind it
     *    on the type `GL_TEXTURE_2D`, transfer the data from the host to GPU,
     *    finally, release the image on the memory.
     * 
     *  \param[in] input: the input image filename.
     *  \param[in] index: 0, 1, ..., 31 represent GL_TEXTURE0, GL_TEXTURE1, ...,
     *				      GL_TEXTURE31 respectively.
     */
    Texture(const std::string& input, unsigned int index = 0);

    /*!
     *  Destructor.
     *    Delete the buffer on GPU.
     */
    ~Texture();

    /*!
     *  Bind the texture object on the texture unit.
     */
    void bind() const;

    /*!
     *  Unbind the texture object on the given texture unit.
     */
    void unbind() const;

protected:
    /*!
     *  Buffer id.
     */
    unsigned int m_id;

    /*!
     *  The `index` represents GL_TEXTURE0, GL_TEXTURE1, ..., or GL_TEXTURE31,
     *    so the range of `index` should be [0, 31].
     */
    unsigned int m_index;

    int m_width;
    int m_height;
};
}
