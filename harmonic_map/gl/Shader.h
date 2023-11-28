#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <string>
#include <map>

namespace MeshLib
{
class Shader
{
public:
    Shader(const std::string& vertex_shade_src,
           const std::string& fragment_shader_src,
           const std::string& geometry_shader_src = std::string());

    ~Shader();

    unsigned int create_shader(const std::string& vertex_shade_src, 
                               const std::string& fragment_shader_src,
                               const std::string& geometry_shader_src = std::string());

    unsigned int compile_shader(unsigned int type, const std::string& src);

    void bind() const;

    void unbind() const;

    void set_uniform(const std::string& name, bool value);
    
    void set_uniform(const std::string& name, int value);
    
    void set_uniform(const std::string& name, float value);
    
    void set_uniform(const std::string& name, const glm::vec2& value);

    void set_uniform(const std::string& name, float x, float y);
    
    void set_uniform(const std::string& name, const glm::vec3& value);

    void set_uniform(const std::string& name, float x, float y, float z);
    
    void set_uniform(const std::string& name, const glm::vec4& value);

    void set_uniform(const std::string& name, float x, float y, float z, float w);
    
    void set_uniform(const std::string& name, const glm::mat2& mat);
    
    void set_uniform(const std::string& name, const glm::mat3& mat);
    
    void set_uniform(const std::string& name, const glm::mat4& mat);

protected:
    /*!
     *  Load the source code from the input glsl file.
     *  \param[in] input: the given glsl file.
     *  \return the string of source code.
     */
    std::string load_source(const std::string& input);

    /*!
     *  Get the location of a uniform variable.
     *  \param[in] name: the name of the uniform variable.
     *  \return an integer that represents the location of the uniform variable
     *          within a program object.
     */
    int uniform_location(const std::string& name);

    /*!
     *  Program object.
     */
    unsigned int m_id;

    /*! 
     *  The cache of the uniforms.
     */ 
    std::map<std::string, int> m_uniform_locations;
};
}