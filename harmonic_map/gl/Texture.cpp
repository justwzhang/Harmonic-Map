#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

namespace MeshLib
{

Texture::Texture(const std::string& input, unsigned int index)
    : m_index(index)
{
    //stbi_set_flip_vertically_on_load(1);
    int comp = 0, req_comp = 4;
    unsigned char* buf = stbi_load(input.c_str(), &m_width, &m_height, &comp, req_comp);
    
    glGenTextures(1, &m_id);
    // bind the texture object before configuring
    // TODO do we need glActiveTexture if this is not the first texture?
    glBindTexture(GL_TEXTURE_2D, m_id);
    // configuring
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_REPEAT
    // generate texture image on GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, 
                 GL_UNSIGNED_BYTE, buf);
    // unbind the texture object after configuring
    glBindTexture(GL_TEXTURE_2D, 0);

    if (buf)
        stbi_image_free(buf);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind() const
{
    glActiveTexture(GL_TEXTURE0 + m_index);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
    glActiveTexture(GL_TEXTURE0 + m_index);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
