#pragma once
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "../HarmonicMapMesh.h"
#include <gmtl/gmtl.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279
#endif

namespace MeshLib
{
class RenderObject
{
public:
    enum Type
    {
        POINTS, LINES, TRIANGLES, QUADS, CYLINDER,
    };

    RenderObject() : vbo(nullptr), vao(nullptr), ebo(nullptr), type(POINTS) {};

    ~RenderObject() 
    {
        if (ebo)
        {
            ebo->unbind();
            delete ebo;
            ebo = nullptr;
        }
        if (vao)
        {
            vao->unbind();
            delete vao;
            vao = nullptr;
        }
        if (vbo)
        {
            vbo->unbind();
            delete vbo;
            vbo = nullptr;
        }
    };

    VertexBuffer*  vbo;
    VertexArray*   vao;
    ElementBuffer* ebo;

    Type type;
};

class RenderPoints : public RenderObject
{
public:
    RenderPoints(const std::vector<CPoint>& points)
    {
        type = POINTS;

        size_t nv = points.size();
        float* pts = (float*)malloc(nv * 3 * sizeof(float));

        for (size_t i = 0; i < points.size(); ++i)
        {
            pts[i * 3 + 0] = points[i][0];
            pts[i * 3 + 1] = points[i][1];
            pts[i * 3 + 2] = points[i][2];
        }

        vbo = new VertexBuffer(pts, nv, 3 * sizeof(float));
        vao = new VertexArray();
        VertexAttribLayout layout;
        layout.push(3, VertexAttribType::FLOAT);
        vao->add_buffer(*vbo, layout);  //"3F"

        free(pts); pts = NULL;
    }

    void draw()
    {
        vao->bind();

        int count = (int)vbo->vertex_count();
        glPointSize(5.0);
        glDrawArrays(GL_POINTS, 0, count);
    }


};

class RenderLines : public RenderObject
{
public:
    RenderLines(const std::vector<CPoint>& points)
    {
        type = LINES;

        size_t nv = points.size();
        size_t ne = nv / 2;

        float* verts = (float*)malloc(nv * 3 * sizeof(float));
        int* edges = (int*)malloc(ne * 2 * sizeof(int));

        for (int i = 0; i < nv; ++i)
        {
            const CPoint& p = points[i];
            verts[i * 3 + 0] = (float)p[0];
            verts[i * 3 + 1] = (float)p[1];
            verts[i * 3 + 2] = (float)p[2];
        }

        for (int i = 0; i < ne; ++i)
        {
            edges[i * 2 + 0] = i * 2;
            edges[i * 2 + 1] = i * 2 + 1;
        }

        {
            vbo = new VertexBuffer(verts, nv, 3 * sizeof(float));
            ebo = new ElementBuffer(edges, ne * 2);

            vao = new VertexArray();
            VertexAttribLayout layout;
            layout.push(3, VertexAttribType::FLOAT);
            vao->add_buffer(*vbo, layout);  //"3F"
        }

        free(verts); verts = NULL;
        free(edges); edges = NULL;
    }

    // 9/17/2023 xiang

   

    void draw()
    {
        //m_shader->bind();
        //if(m_texture) 
        //    m_texture->bind();
        vao->bind();

        if (ebo != nullptr)
        {
            ebo->bind();
            glLineWidth(1.5);
            glDrawElements(GL_LINES, ebo->count(), GL_UNSIGNED_INT, (const void*)(0));
        }
        else
        {
            int vertexCount = (int)vbo->vertex_count();
            glPointSize(2.0);
            glDrawArrays(GL_POINTS, 0, vertexCount);
        }

        //if (m_texture)
        //    m_texture->unbind();
    }
};

class RenderCylinder : public RenderObject
{
public:
    RenderCylinder(const CPoint& source, const CPoint& target, const float radius, const glm::vec3& color)
    {
        type = CYLINDER;

        const size_t nSlice = 6;
        const float angleStep = 2.0 * M_PI / nSlice;

        const size_t nv = 2 * nSlice + 2;
        const size_t nf = 4 * nSlice;


        float* verts = (float*)malloc(nv * 3 * sizeof(float));
        int* faces = (int*)malloc(nf * 3 * sizeof(int));

        for (int i = 0; i < nSlice; ++i)
        {
            float theta = angleStep * i;
            CPoint p(std::cos(theta) * radius, std::sin(theta) * radius, 0.0);
            verts[i * 3 + 0] = (float)p[0];
            verts[i * 3 + 1] = (float)p[1];
            verts[i * 3 + 2] = (float)p[2];

            p[2] = 1.0;
            verts[(nSlice + i) * 3 + 0] = (float)p[0];
            verts[(nSlice + i) * 3 + 1] = (float)p[1];
            verts[(nSlice + i) * 3 + 2] = (float)p[2];
        }

        verts[(2 * nSlice + 0) * 3 + 0] = 0.0f;
        verts[(2 * nSlice + 0) * 3 + 1] = 0.0f;
        verts[(2 * nSlice + 0) * 3 + 2] = 0.0f;
        verts[(2 * nSlice + 1) * 3 + 0] = 0.0f;
        verts[(2 * nSlice + 1) * 3 + 1] = 0.0f;
        verts[(2 * nSlice + 1) * 3 + 2] = 1.0f;

        // cylinder
        for (int i = 0; i < nSlice; ++i)
        {
            faces[i * 6 + 0] = i;
            faces[i * 6 + 1] = (i + 1) % nSlice;
            faces[i * 6 + 2] = i + nSlice;

            faces[i * 6 + 3] = i + nSlice;
            faces[i * 6 + 4] = (i + 1) % nSlice;
            faces[i * 6 + 5] = i + nSlice + 1 >= 2 * nSlice ? i + 1 : i + nSlice + 1;
        }
        
        // two caps
        for (int i = 0; i < nSlice; ++i)
        {
            int j = (i + 1) % nSlice;
            int k = 2 * nSlice;
            faces[(2 * nSlice + i) * 3 + 0] = j;
            faces[(2 * nSlice + i) * 3 + 1] = i;
            faces[(2 * nSlice + i) * 3 + 2] = k;

            faces[(3 * nSlice + i) * 3 + 0] = i + nSlice;
            faces[(3 * nSlice + i) * 3 + 1] = j + nSlice;
            faces[(3 * nSlice + i) * 3 + 2] = k + 1;
        }
        
        // rotate, scale and translate
        {
            CPoint v = target - source;
            float scale = v.norm();
            v /= scale;

            gmtl::Vec3f from(0, 0, 1);
            gmtl::Vec3f to(v[0], v[1], v[2]);

            gmtl::Matrix33f m;
            gmtl::setRot(m, from, to);

            for (int i = 0; i < nv; ++i)
            {
                float x = verts[i * 3 + 0];
                float y = verts[i * 3 + 1];
                float z = verts[i * 3 + 2];
            
                gmtl::Vec3f d(x, y, z * scale);
                d = m * d;

                d += gmtl::Vec3f(source[0], source[1], source[2]);

                verts[i * 3 + 0] = d[0];
                verts[i * 3 + 1] = d[1];
                verts[i * 3 + 2] = d[2];
            }
        }

        if(0)
        {
            CPoint n = (target - source) ^ CPoint(0, 0, 1);
            n /= n.norm();

            float scale = (target - source).norm();
            
            float cosTheta = (target - source) * CPoint(0, 0, 1) / (target - source).norm();
            float theta = std::acos(cosTheta);
            float sinTheta = std::sin(theta);
            float oneMinusCosTheta = 1.0 - cosTheta;

            for (int i = 0; i < nv; ++i)
            {
                float x = verts[i * 3 + 0];
                float y = verts[i * 3 + 1];
                float z = verts[i * 3 + 2];

                float X, Y, Z;
                X = (cosTheta + n[0] * n[0] * oneMinusCosTheta) * x +
                    (n[0] * n[1] * oneMinusCosTheta - n[2] * sinTheta) * y +
                    (n[0] * n[2] * oneMinusCosTheta + n[1] * sinTheta) * z;

                Y = (n[1] * n[0] * oneMinusCosTheta + n[2] * sinTheta) * x +
                    (cosTheta + n[1] * n[1] * oneMinusCosTheta) * y +
                    (n[1] * n[2] * oneMinusCosTheta - n[0] * sinTheta) * z;

                Z = (n[2] * n[0] * oneMinusCosTheta - n[1] * sinTheta) * x +
                    (n[2] * n[1] * oneMinusCosTheta + n[0] * sinTheta) * y +
                    (cosTheta + n[2] * n[2] * oneMinusCosTheta) * z;

                verts[i * 3 + 0] = X * scale + source[0];
                verts[i * 3 + 1] = Y * scale + source[1];
                verts[i * 3 + 2] = Z * scale + source[2];
            }
        }

        {
            vbo = new VertexBuffer(verts, nv, 3 * sizeof(float));
            ebo = new ElementBuffer(faces, nf * 3);

            vao = new VertexArray();
            VertexAttribLayout layout;
            layout.push(3, VertexAttribType::FLOAT);
            vao->add_buffer(*vbo, layout);  //"3F"
        }

        free(verts); verts = NULL;
        free(faces); faces = NULL;
    }

    void draw()
    {
        //m_shader->bind();
        //if(m_texture) 
        //    m_texture->bind();
        vao->bind();

        if (ebo != nullptr)
        {
            ebo->bind();
            glDrawElements(GL_TRIANGLES, ebo->count(), GL_UNSIGNED_INT, (const void*)(0));
        }
        else
        {
            int vertexCount = (int)vbo->vertex_count();
            glPointSize(2.0);
            glDrawArrays(GL_POINTS, 0, vertexCount);
        }

        //if (m_texture)
        //    m_texture->unbind();
    }
};

class RenderTriMesh : public RenderObject
{
public:
    RenderTriMesh(CHarmonicMapMesh& mesh)
    {
        type = TRIANGLES;

        size_t nv = mesh.num_vertices();
        size_t nf = mesh.num_faces();

        float* verts = (float*)malloc(nv * 8 * sizeof(float));
        int* faces = (int*)malloc(nf * 3 * sizeof(int));

        size_t idx = 0;
        for (CHarmonicMapMesh::MeshVertexIterator vit(&mesh); !vit.end(); ++vit)
        {
            auto* pV = *vit;
            verts[idx * 8 + 0] = (float)pV->point()[0];
            verts[idx * 8 + 1] = (float)pV->point()[1];
            verts[idx * 8 + 2] = (float)pV->point()[2];
            verts[idx * 8 + 3] = (float)pV->uv()[0];
            verts[idx * 8 + 4] = (float)pV->uv()[1];
            verts[idx * 8 + 5] = (float)pV->normal()[0];
            verts[idx * 8 + 6] = (float)pV->normal()[1];
            verts[idx * 8 + 7] = (float)pV->normal()[2];
            ++idx;
        }
 
        idx = 0;
        for (CHarmonicMapMesh::MeshFaceIterator fit(&mesh); !fit.end(); ++fit)
        {
            auto* pF = *fit;
            size_t j = 0;
            for (CHarmonicMapMesh::FaceVertexIterator fvit(pF); !fvit.end(); ++fvit)
            {
                auto* pV = *fvit;
                faces[idx * 3 + j] = pV->idx();
                ++j;
            }
            ++idx;
        }

        {
            vbo = new VertexBuffer(verts, nv, 8 * sizeof(float));
            ebo = new ElementBuffer(faces, nf * 3);

            vao = new VertexArray();
            VertexAttribLayout layout;
            layout.push(3, VertexAttribType::FLOAT);
            layout.push(2, VertexAttribType::FLOAT);
            layout.push(3, VertexAttribType::FLOAT);
            vao->add_buffer(*vbo, layout);  //"3F|2F|3F"
        }

        free(verts); verts = NULL;
        free(faces); faces = NULL;
    }

    void draw()
    {
        //m_shader->bind();
        //if(m_texture) 
        //    m_texture->bind();
        vao->bind();

        if (ebo != nullptr)
        {
            ebo->bind();
            glDrawElements(GL_TRIANGLES, ebo->count(), GL_UNSIGNED_INT, (const void*)(0));
        }
        else
        {
            int vertexCount = (int)vbo->vertex_count();
            glPointSize(2.0);
            glDrawArrays(GL_POINTS, 0, vertexCount);
        }

        //if (m_texture)
        //    m_texture->unbind();
    }
};


class RendererConfig
{
public:
    enum Mode
    {
        TEXTURE_DISABLE,
        TEXTURE_REPLACE,
        TEXTURE_MODULATE,
        TEXTURE_NORMAL,
        TEXTURE_BLENDED,

        SURFACE_HIDDEN,
        SURFACE_FLAT,
        SURFACE_SMOOTH
    };

    bool  ShowWireframe = true;

    bool  ShowPoints    = false;

    bool  ShowCullFace  = false;

    Mode  TextureMode   = TEXTURE_DISABLE; // For homework purpose
    

    float TextureModulateWeight = 0.5f;

    bool ShowUV         = false;
    bool ShowPos        = true;

    float PointSize = 1.5f;
    glm::vec3 PointColor = { 1.0f, 1.0f, 1.0f };
    float LineWidth = 2.0f;
    glm::vec3 LineColor  = { 1.0f, 1.0f, 0.0f };
    glm::vec3 FaceColor  = { 229.0f / 255.0f, 162.0f / 255.0f, 141.0f / 255.0f };

    void setFlatOrSmooth(const Mode m)
    {   
        m_surface_render_mode = m;
    }

    bool isSmooth() const { return m_surface_render_mode == SURFACE_SMOOTH; }
    bool isFlat() const { return m_surface_render_mode == SURFACE_FLAT; }
    bool isHidden() const { return m_surface_render_mode == SURFACE_HIDDEN; }
protected:
    //bool  m_showFlat = false;
    Mode  m_surface_render_mode = SURFACE_SMOOTH;
};

class Renderer
{
public:
    Renderer();

    /*
    Renderer(VertexArray& vao, ElementBuffer& ebo, Shader& shader);

    Renderer(VertexArray& vao, ElementBuffer& ebo, Shader& shader, Texture& texture);

    Renderer(VertexArray* vao, ElementBuffer* ebo, Shader* shader, Texture* texture = NULL);
    */
    // renderer copy constructor, only copy pointer

    void add_trimesh(CHarmonicMapMesh& mesh);

    void add_lines(const std::vector<CPoint>& points);

    void add_edge(const CPoint& source, const CPoint& target);

    void add_points(const std::vector<CPoint>& points);
    //void add_line(const CPoint& a, const CPoint& b);

    void draw() const;

    void clear() const;

    void set_shader(Shader* shader);

    void set_shader(const std::string& vertex_shade_src,
                    const std::string& fragment_shader_src,
                    const std::string& geometry_shader_src = std::string());

    Shader* shader() const { return m_shader.get(); };

    RendererConfig& config() { return m_config; }


    void unbind();

protected:
    std::vector<RenderObject*> m_render_objects;

protected:
    std::shared_ptr<Shader> m_shader;
    //Texture*       m_texture;
public:
    RendererConfig m_config;
};
}