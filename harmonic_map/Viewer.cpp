#include "GLWindow.h"
#include "gl/Texture.h"

#include "HarmonicMapMesh.h"
#include "HarmonicMap.h"

#include <iostream>
#include <thread>
#include <mutex>

using namespace MeshLib;
using namespace std;

// Modified 9/17/2023 
std::mutex g_mutex;
glm::mat4  g_viewMat = glm::mat4(1.0);

/* Input your own shader path */
std::string shaderPath("C:/Users/justw/Desktop/CS hw/CS 528 Graphics/HW 3/HW 3/Code/harmonic_map/shaders/");



/* global g_mesh */
CHarmonicMapMesh g_mesh;
CHarmonicMap g_mapper;

class CMyWindow : public CGLWindow
{

public:
    glm::mat4 m_viewMat;
public:
    CMyWindow(std::string title, int width, int height)
        : CGLWindow(title, width, height)
    {
    };

protected:

    virtual void on_prepare_data()
    {
        Texture* texture = new Texture(shaderPath + "checker.bmp");
        Shader* meshShader = new Shader(

            shaderPath + "vertex.glsl",
            shaderPath + "frag.glsl",
            shaderPath + "geometry.glsl"
        
        );



        Shader* pointCloudShader = new Shader(shaderPath + "points.vs",
            shaderPath + "points.fs");

        Shader* pointCloudShader2 = new Shader(shaderPath + "points.vs",
            shaderPath + "points.fs");

        texture->bind();

        std::vector<CPoint> points;
        points.push_back(CPoint(+0.1, +0.1, +1.1));
        points.push_back(CPoint(-0.1, -0.1, -1.1));
        points.push_back(CPoint(+0.1, +0.2, +1.3));
        points.push_back(CPoint(-0.1, -0.2, -1.3));
        points.push_back(CPoint(0, 0, 0));

        if (0)
        {
            Renderer r;
            r.set_shader(shaderPath + "points.vs",
                shaderPath + "points.fs");
            r.shader()->bind();
            r.shader()->set_uniform("u_vertexColor", glm::vec3(1, 0, 1));
            r.add_lines(points);
            m_renderers.push_back(r);
        }
        if (1)
        {
            Renderer r;
            r.set_shader(meshShader);
            r.shader()->bind();
            r.shader()->set_uniform("u_vertexColor", glm::vec3(229.0 / 255.0, 162.0 / 255.0, 141.0 / 255.0));
            r.add_trimesh(g_mesh);
            m_renderers.push_back(r);
        }

        if (1)
        {
            Renderer r;
            r.set_shader(shaderPath + "vertex.glsl",
                shaderPath + "frag.glsl",
                shaderPath + "geometry.glsl");
            r.shader()->bind();
            r.shader()->set_uniform("u_vertexColor", glm::vec3(1, 1, 0));

            r.add_edge(CPoint(0, 0, 0), CPoint(1, 0, 0));
            r.add_edge(CPoint(0, 0, 0), CPoint(0, 1, 0));
            r.add_edge(CPoint(0, 0, 0), CPoint(0, 0, 1));

            CPoint pA(m_config.ray_origin[0], m_config.ray_origin[1], m_config.ray_origin[2]);
            CPoint pB(m_config.ray_dest[0], m_config.ray_dest[1], m_config.ray_dest[2]);

            pA = CPoint(-1.44975e-05, 6.2132e-05, -0.001);
            pB = CPoint(-123.689, -120.405, -1057.03);

            r.add_edge(pA, pB);
            m_renderers.push_back(r);
        }

        if (0)
        {
            Renderer r;
            r.set_shader(shaderPath + "vertex.glsl",
                shaderPath + "frag.glsl",
                shaderPath + "geometry.glsl");
            r.shader()->bind();
            r.shader()->set_uniform("u_vertexColor", glm::vec3(1, 0, 1));

            int i = 0;
            for (CHarmonicMapMesh::MeshEdgeIterator eit(&g_mesh); !eit.end(); ++eit)
            {
                //if (i > 10) break;
                CHarmonicMapMesh::CEdge* pE = *eit;
                const CPoint& pA = g_mesh.edgeVertex1(pE)->point();
                const CPoint& pB = g_mesh.edgeVertex2(pE)->point();
                r.add_edge(pA, pB);
                ++i;
            }

            m_renderers.push_back(r);
        }
        if (0)
        {
            Renderer r;
            r.set_shader(pointCloudShader);
            r.shader()->bind();
            r.shader()->set_uniform("u_vertexColor", glm::vec3(1, 1, 1));

            r.add_points(points);
            m_renderers.push_back(r);
        }
        if (0)
        {
            Renderer r;
            r.set_shader(pointCloudShader2);
            r.shader()->bind();
            r.shader()->set_uniform("u_vertexColor", glm::vec3(0, 1, 0));   // TODO should assign it before draw
            for (auto& p : points)
                p *= 2.0;
            glPointSize(5.0);
            r.add_points(points);
            m_renderers.push_back(r);
        }


        //texture->unbind();
        /*
        CViewerMesh_2 m;
        m.read("S:/proj/MEDICAL_IMAGE/virtual_colon/data/bw9/eight.m");
        Renderer r2;
        r2.add_trimesh(m);
        m_renderers.push_back(r2);
        */
        //m_actived_shader = meshShader;
    }

    /*
    virtual void paintGL()
    {
        glClearColor(0.35f, 0.53f, 0.70f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (g_mutex.try_lock())
        {
            if (m_config.arcball.updated())
            {
                m_viewMat = m_config.arcball.transform();
                g_viewMat = m_viewMat;
                m_config.arcball.updated() = false;
            }
            else
            {
                m_viewMat = g_viewMat;
            }

            m_actived_shader->set_uniform("u_viewMat", m_viewMat);
            g_mutex.unlock();
        }

        for each (auto & renderer in m_renderers)
        {
            renderer.draw();
        }
    }
    */

    void normalizeUV(CHarmonicMapMesh* pMesh) {
        double x_max = -10000, y_max = -10000;
        double x_min = 10000, y_min = 10000;

        for (CHarmonicMapMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
        {
            CHarmonicMapVertex* pV = *viter;

            x_max = std::max(x_max, pV->uv()[0]);
            y_max = std::max(y_max, pV->uv()[1]);

            x_min = std::min(x_min, pV->uv()[0]);
            y_min = std::min(y_min, pV->uv()[1]);

        }

        double beta1 = x_max, alpha1 = x_min;
        double beta2 = y_max, alpha2 = y_min;
        double a = 0.0, b = 1.0;

        for (CHarmonicMapMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
        {
            CHarmonicMapVertex* pV = *viter;

            pV->uv()[0] = ((b - a) * pV->uv()[0] + a * beta1 - b * alpha1) / (beta1 - alpha1);
            pV->uv()[1] = ((b - a) * pV->uv()[1] + a * beta2 - b * alpha2) / (beta2 - alpha2);
        }

    }
    void help()
    {
        printf("1  -  Show or hide mesh\n");
        printf("2  -  Show or hide uv in the back\n");
        printf("n  -  Take next one step of iterative method\n");
        printf("i  -  Iterative method of harmonic map\n");
        printf("h  -  Directly solve the equations\n");
        printf("t  -  Switching Texture Mode\n");
        printf("w  -  Wireframe Display\n");
        printf("s  -  Switching Shading Mode\n");
        printf("?  -  Help Information\n");
        printf("esc - Quit\n");
    }

    virtual void charEvent(int code)
    {
        switch (code)
        {
        case '1':
            for (auto& renderer : m_renderers)
            {
                renderer.config().ShowPos = !renderer.config().ShowPos;
                //renderer.shader()->bind();
                //renderer.shader()->set_uniform("u_showUV", false);
            }
            break;
        case '2':
            for (auto& renderer : m_renderers)
            {
                renderer.unbind();

                g_mapper.set_mesh(&g_mesh);
                g_mesh.reset_idx();

                renderer.shader()->bind();
                renderer.add_trimesh(g_mesh);
                renderer.config().ShowUV = !renderer.config().ShowUV;
                //renderer.shader()->bind();
                //renderer.shader()->set_uniform("u_showUV", true);
            }
            break;
        case 'n':
                // iterative method of harmonic map
                for (auto& renderer : m_renderers)
                {
                    renderer.unbind();

                    g_mapper.step_one();
                    g_mesh.reset_idx();

                    normalizeUV(&g_mesh);
                    renderer.shader()->bind();
                    renderer.add_trimesh(g_mesh);

                }
                break;
              
        case 'i':
            // iterative method of harmonic map
            for (auto& renderer : m_renderers)
            {
                renderer.unbind();

                g_mapper.set_mesh(&g_mesh);
                g_mapper.iterative_map();
                g_mesh.reset_idx();

                normalizeUV(&g_mesh);
                renderer.shader()->bind();
                renderer.add_trimesh(g_mesh);
               
            }
            break;
        case 'h':
            for (auto& renderer : m_renderers)
            {
                renderer.unbind();

                g_mapper.set_mesh(&g_mesh);
                g_mapper.map();
                g_mesh.reset_idx();

                normalizeUV(&g_mesh);
                renderer.shader()->bind();
                renderer.add_trimesh(g_mesh); 
             
            }
            break;
        case 'p':
            normalizeUV(&g_mesh);
            g_mesh.write_m("mesh_UV.m");
            
           /* for (CHarmonicMapMesh::MeshVertexIterator viter(&g_mesh); !viter.end(); ++viter) {
                CHarmonicMapVertex* v = *viter;
                std::cout << "VID" << " " << v->id() << " " 
                    << "pos" << v->point()[0] <<" "<< v->point()[1] << " " << v->point()[2] << 
                    " " << "UV" << " " << v->uv()[0] << " " << v->uv()[1] << std::endl;
            }*/
            break;
        case '?':
            help();
            break;
        default:
            break;
        }

        CGLWindow::charEvent(code);
    }
};


/*! Normalize g_mesh
 * \param pMesh the input g_mesh
 */
void normalizeMesh(CHarmonicMapMesh* pMesh)
{
    CPoint s(0, 0, 0);
    for (CHarmonicMapMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
    {
        CHarmonicMapVertex* v = *viter;
        s = s + v->point();
    }
    s = s / pMesh->numVertices();

    for (CHarmonicMapMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
    {
        CHarmonicMapVertex* v = *viter;
        CPoint p = v->point();
        p = p - s;
        v->point() = p;
    }

    double d = 0;
    for (CHarmonicMapMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
    {
        CHarmonicMapVertex* v = *viter;
        CPoint p = v->point();
        for (int k = 0; k < 3; k++)
        {
            d = (d > fabs(p[k])) ? d : fabs(p[k]);
        }
    }

    for (CHarmonicMapMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
    {
        CHarmonicMapVertex* v = *viter;
        CPoint p = v->point();
        p = p / d;
        v->point() = p;
    }
};

/*! Compute the face normal and vertex normal
 * \param pMesh the input g_mesh
 */
void computeNormal(CHarmonicMapMesh* pMesh)
{
    for (CHarmonicMapMesh::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
    {
        CHarmonicMapVertex* v = *viter;
        CPoint n(0, 0, 0);
        for (CHarmonicMapMesh::VertexFaceIterator vfiter(v); !vfiter.end(); ++vfiter)
        {
            CHarmonicMapFace* pF = *vfiter;

            CPoint p[3];
            CHalfEdge* he = pF->halfedge();
            for (int k = 0; k < 3; k++)
            {
                p[k] = he->target()->point();
                he = he->he_next();
            }

            CPoint fn = (p[1] - p[0]) ^ (p[2] - p[0]);
            pF->normal() = fn / fn.norm();
            n += fn;
        }

        n = n / n.norm();
        v->normal() = n;
    }
};



int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s input.m\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::string mesh_name(argv[1]);
    if (strutil::endsWith(mesh_name, ".m"))
    {
        g_mesh.read_m(mesh_name.c_str());
    }
    else
    {
        printf("Only file format .m supported.\n");
        return EXIT_FAILURE;
    }
    g_mesh.reset_idx();
    normalizeMesh(&g_mesh);
    computeNormal(&g_mesh);

    CMyWindow win1("Window 1", 800, 800);

    thread t1(&CMyWindow::run, &win1);
    t1.join();

    win1.terminate();

    return 0;

}

