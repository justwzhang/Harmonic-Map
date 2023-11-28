#include "Renderer.h"
#include <glad/glad.h>

namespace MeshLib
{

Renderer::Renderer()
   
{
    
}

/*
Renderer::Renderer(VertexArray& vao, ElementBuffer& ebo, Shader& shader)
{
    m_vao = &vao;
    m_ebo = &ebo;
    m_shader = &shader;
    m_texture = NULL;
}

Renderer::Renderer(VertexArray& vao, ElementBuffer& ebo, 
                   Shader& shader, Texture& texture)
{
    m_vao = &vao;
    m_ebo = &ebo;
    m_shader = &shader;
    m_texture = &texture;
}

Renderer::Renderer(VertexArray* vao, ElementBuffer* ebo, 
                   Shader* shader, Texture* texture)
{
    m_vao = vao;
    m_ebo = ebo;
    m_shader = shader;
    m_texture = texture;
}
*/

void Renderer::add_trimesh(CHarmonicMapMesh& mesh)
{
    RenderTriMesh* renderTriMesh = new RenderTriMesh(mesh);
    m_render_objects.push_back(renderTriMesh);
}

void Renderer::add_lines(const std::vector<CPoint>& points)
{
    RenderLines* renderLines = new RenderLines(points);
    m_render_objects.push_back(renderLines);
}

void Renderer::add_edge(const CPoint& source, const CPoint& target)
{
    m_config.LineColor = glm::vec3(1, 0, 0);
    RenderCylinder* renderCylinder = new RenderCylinder(source, target, 0.01, {1, 1, 0});
    m_render_objects.push_back(renderCylinder);
}

void Renderer::add_points(const std::vector<CPoint>& points)
{
    RenderPoints* renderPoints = new RenderPoints(points);
    m_render_objects.push_back(renderPoints);
}

void Renderer::draw() const
{
    int i = 0;
    m_shader->bind();
    for (auto obj : m_render_objects)
    {
        if (obj->type == RenderObject::Type::TRIANGLES)
        {
            RenderTriMesh* object = (RenderTriMesh*)obj;
            for (int i = 0; i < 2; ++i)
            {
                if (i == 0 && m_config.ShowPos)
                    m_shader->set_uniform("u_showUV", 0);
                else if (i == 1 && m_config.ShowUV)
                    m_shader->set_uniform("u_showUV", 1);
                else
                    continue;

                if (m_config.ShowCullFace)
                    glDisable(GL_CULL_FACE);
                else
                    glEnable(GL_CULL_FACE);

                if (m_config.TextureMode == RendererConfig::TEXTURE_DISABLE)
                    m_shader->set_uniform("u_texture.mode", 0);
                else if (m_config.TextureMode == RendererConfig::TEXTURE_REPLACE)
                    m_shader->set_uniform("u_texture.mode", 1);
                else if (m_config.TextureMode == RendererConfig::TEXTURE_MODULATE)
                    m_shader->set_uniform("u_texture.mode", 2);
                else if (m_config.TextureMode == RendererConfig::TEXTURE_NORMAL)
                    m_shader->set_uniform("u_texture.mode", 3);
                else if (m_config.TextureMode == RendererConfig::TEXTURE_BLENDED)
                    m_shader->set_uniform("u_texture.mode", 4);

            
                if (m_config.isSmooth())
                {
                    m_shader->set_uniform("u_vertexColor", m_config.FaceColor);
                    m_shader->set_uniform("u_DrawMode", 4);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    object->draw();
                }
                else if (m_config.isFlat())
                {
                    m_shader->set_uniform("u_vertexColor", m_config.FaceColor);
                    m_shader->set_uniform("u_DrawMode", 3);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    object->draw();
                }
                else
                {
                    // hidden
                }


                m_shader->set_uniform("u_texture.mode", 0);

                if (m_config.ShowPoints)
                {
                    m_shader->set_uniform("u_vertexColor", m_config.PointColor);
                    glPointSize(m_config.PointSize);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                    object->draw();
                }

                if (m_config.ShowWireframe)
                {
                    m_shader->set_uniform("u_vertexColor", m_config.LineColor);
                    glEnable(GL_LINE_SMOOTH);
                    glLineWidth(m_config.LineWidth);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    object->draw();
                }
            }
        }
        else if (obj->type == RenderObject::Type::LINES)
        {
            RenderLines* object = (RenderLines*)obj;
            object->draw();
        }
        else if (obj->type == RenderObject::Type::CYLINDER)
        {
            RenderCylinder* object = (RenderCylinder*)obj;
            m_shader->set_uniform("u_vertexColor", m_config.LineColor);
            m_shader->set_uniform("u_DrawMode", 3);
            m_shader->set_uniform("u_texture.mode", 0);
            glEnable(GL_LINE_SMOOTH);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            object->draw();
        }
        else if (obj->type == RenderObject::Type::POINTS)
        {
            RenderPoints* object = (RenderPoints*)obj;
            object->draw();
        }
        ++i;
    }
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::set_shader(Shader* shader)
{
    m_shader = std::shared_ptr<Shader>(shader);
}

void Renderer::set_shader(const std::string& vertex_shade_src, 
                          const std::string& fragment_shader_src, 
                          const std::string& geometry_shader_src)
{
    Shader* shader = new Shader(vertex_shade_src, fragment_shader_src, geometry_shader_src);
    this->set_shader(shader);
}

void Renderer::unbind() {
    m_render_objects.clear();
}

}



