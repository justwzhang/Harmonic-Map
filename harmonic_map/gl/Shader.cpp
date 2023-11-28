#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

namespace MeshLib
{
Shader::Shader(const std::string& vertex_shade_src,
               const std::string& fragment_shader_src,
               const std::string& geometry_shader_src)
{
    m_id = create_shader(vertex_shade_src, fragment_shader_src, geometry_shader_src);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

unsigned int Shader::create_shader(const std::string& vertex_shade_src, 
                                   const std::string& fragment_shader_src,
                                   const std::string& geometry_shader_src)
{
    bool has_geometry_shader = !geometry_shader_src.empty();
    unsigned int program = glCreateProgram();
    
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, 
                                     load_source(vertex_shade_src));
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, 
                                     load_source(fragment_shader_src));

    unsigned int gs = 0;
    if(has_geometry_shader)
        gs = compile_shader(GL_GEOMETRY_SHADER, 
                            load_source(geometry_shader_src));

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    if (has_geometry_shader)
        glAttachShader(program, gs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(gs);

    return program;
}

unsigned int Shader::compile_shader(unsigned int type, const std::string& src)
{
    unsigned int id = glCreateShader(type);
    const char* pSrc = src.c_str();
    glShaderSource(id, 1, &pSrc, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " 
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }


    return id;
}

void Shader::bind() const
{
    glUseProgram(m_id);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::set_uniform(const std::string& name, bool value) 
{
    glUniform1i(uniform_location(name), value ? 1 : 0);
}

void Shader::set_uniform(const std::string& name, int value) 
{
    glUniform1i(uniform_location(name), value);
}

void Shader::set_uniform(const std::string& name, float value) 
{
    glUniform1f(uniform_location(name), value);
}

void Shader::set_uniform(const std::string& name, const glm::vec2& value) 
{
    glUniform2fv(uniform_location(name), 1, &value[0]);
}

void Shader::set_uniform(const std::string& name, float x, float y) 
{
    glUniform2f(uniform_location(name), x, y);
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value) 
{
    glUniform3fv(uniform_location(name), 1, &value[0]);
}

void Shader::set_uniform(const std::string& name, float x, float y, float z) 
{
    glUniform3f(uniform_location(name), x, y, z);
}

void Shader::set_uniform(const std::string& name, const glm::vec4& value) 
{
    glUniform4fv(uniform_location(name), 1, &value[0]);
}

void Shader::set_uniform(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(uniform_location(name), x, y, z, w);
}

void Shader::set_uniform(const std::string& name, const glm::mat2& mat) 
{
    glUniformMatrix2fv(uniform_location(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_uniform(const std::string& name, const glm::mat3& mat) 
{
    glUniformMatrix3fv(uniform_location(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_uniform(const std::string& name, const glm::mat4& mat) 
{
    glUniformMatrix4fv(uniform_location(name), 1, GL_FALSE, &mat[0][0]);
}

std::string Shader::load_source(const std::string& input)
{
    std::ifstream fs;
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    fs.open(input);
    std::stringstream ss;
    ss << fs.rdbuf();
    fs.close();
    
    return ss.str();
}

int Shader::uniform_location(const std::string& name)
{
    if (m_uniform_locations.find(name) != m_uniform_locations.end())
        return m_uniform_locations[name];

    int location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    m_uniform_locations[name] = location;
    return location;
}

}
