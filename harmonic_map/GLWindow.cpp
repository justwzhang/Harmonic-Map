#include "GLWindow.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>  // glm::value_ptr
#include <glm/gtx/transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/matrix_transform.hpp>

namespace MeshLib
{
//CGLWindowConfig CGLWindow::m_config;
//std::map<GLFWwindow*, CGLWindowConfig> CGLWindow::m_map_win_config;

CGLWindow::CGLWindow()
{
    m_width = 0;
    m_height = 0;

    m_window = NULL;

}

CGLWindow::CGLWindow(std::string title, int width, int height)
{
    m_title = title;
    m_width = width;
    m_height = height;

    m_window = NULL;

    this->initGLFW();
}

CGLWindow::~CGLWindow()
{

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void CGLWindow::run() 
{
    // glfw: window creation
    // --------------------
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
    glfwSetWindowUserPointer(m_window, this);

    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(2); // will reduce the utilization of GPU

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    this->initializeGL();

    // glfw: register events
    this->on_register_events();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    this->on_prepare_data();

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    //m_actived_shader->bind(); // activate shader

    // create transformations
    //m_config.projection = glm::mat4(1.0f);
    //m_config.projection = glm::perspective(glm::radians(45.0f), (float) m_width / (float) m_height, 1e-3f, 1e+3f);
    this->resizeGL(m_width, m_height);

    for each (auto & renderer in m_renderers)
    {
        auto m_actived_shader = renderer.shader();
        m_actived_shader->bind();
        // pass transformation matrices to the shader
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely
        // changes it's often best practice to set it outside the main loop only once.
        m_actived_shader->set_uniform("u_projMat", m_config.projection);

        //conf.view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //conf.view = glm::translate(conf.view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::vec3 eye(0, 0, 3), center(0, 0, 0), up(0, 1, 0);
        m_config.view = glm::lookAt(eye, center, up);
        m_actived_shader->set_uniform("u_viewMat", m_config.view);
        m_actived_shader->set_uniform("u_viewPos", eye);
        m_config.arcball.set_eye(eye, center, up);

        m_config.model = glm::mat4(1.0f);
        m_actived_shader->set_uniform("u_modelMat", m_config.model);

        // directional light
        m_actived_shader->set_uniform("u_dirLight[0].position", { 0.0f, 0.0f, -10.0f });
        m_actived_shader->set_uniform("u_dirLight[0].color", { 1.0f, 1.0f, 1.0f });
        m_actived_shader->set_uniform("u_dirLight[0].ambient", { 0.6f, 0.6f, 0.6f }); //control the strength of the ambient
        m_actived_shader->set_uniform("u_dirLight[0].diffuse", { 1.0f, 1.0f, 1.0f });
        m_actived_shader->set_uniform("u_dirLight[0].specular", { 1.0f, 1.0f, 1.0f });

        m_actived_shader->set_uniform("u_dirLight[1].position", { 0.0f, 0.0f, +10.0f });
        m_actived_shader->set_uniform("u_dirLight[1].color", { 1.0f, 1.0f, 1.0f });
        m_actived_shader->set_uniform("u_dirLight[1].ambient", { 0.6f, 0.6f, 0.6f }); //control the strength of the ambient
        m_actived_shader->set_uniform("u_dirLight[1].diffuse", { 1.0f, 1.0f, 1.0f });
        m_actived_shader->set_uniform("u_dirLight[1].specular", { 1.0f, 1.0f, 1.0f });


        //m_actived_shader->set_uniform("u_vertexColor", glm::vec3(229.0 / 255.0, 162.0 / 255.0, 141.0 / 255.0));
        //m_actived_shader->set_uniform("u_objectColor", glm::vec3(0.8, 0.8, 0.8));

        m_actived_shader->set_uniform("u_material.ambient", glm::vec3(1.0, 1.0, 1.0));
        m_actived_shader->set_uniform("u_material.diffuse", glm::vec3(0.95, 0.95, 0.95));
        m_actived_shader->set_uniform("u_material.specular", glm::vec3(0.5, 0.5, 0.5));
        m_actived_shader->set_uniform("u_material.shininess", 32.0f);

        m_actived_shader->set_uniform("u_texture.rgb", 0);
        m_actived_shader->set_uniform("u_texture.mode", 2);
        /*
        if(m_textures.size() > 0)
            m_actived_shader->set_uniform("u_texture", 0);
        */
    }
    // render loop
    while (!glfwWindowShouldClose(m_window))
    {
        // input
        processInput(m_window); // TODO move it to key event?

        // render
        this->paintGL();
        
        // is this means update?
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    this->destroyGL();
}

void CGLWindow::initGLFW()
{
    const char* err_msg;
    // initialize GLFW
    if (!glfwInit())
    {
        glfwGetError(&err_msg);
        printf("[ERROR] %s\n", err_msg);
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void CGLWindow::terminate() 
{
    glfwTerminate();
}

void CGLWindow::initializeGL()
{
    // configure global opengl state
    // -----------------------------
    //glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void CGLWindow::paintGL()
{
    glClearColor(0.35f, 0.53f, 0.70f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    //m_actived_shader->set_uniform("u_viewMat", m_config.arcball.transform());
    for each (auto & renderer in m_renderers)
    {
        renderer.shader()->bind();
        renderer.shader()->set_uniform("u_viewMat", m_config.arcball.transform());

        renderer.draw();
    }
}

void CGLWindow::resizeGL(int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    m_width = width, m_height = height;

    m_config.projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1e-3f, 1e+3f);
    //m_actived_shader->set_uniform("u_projMat", m_config.projection);
    for each (auto & renderer in m_renderers)
    {
        renderer.shader()->bind();
        renderer.shader()->set_uniform("u_projMat", m_config.projection);
    }
}

void CGLWindow::destroyGL()
{
}

/*
void CGLWindow::on_prepare_data() 
{
    //m_vertex_data = new float[30];

    float vertices[] = {0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 
                        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 
                        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 
                        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 
                        0.5f,  0.5f,  0.5f,  1.0f, 1.0f};

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    m_VAOs.push_back(vao);
    m_VBOs.push_back(vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
*/

void CGLWindow::mousePressEvent(int button, int mods, int x, int y)
{
    if (m_config.select_on)
    {
        float mouseX = x / (m_width * 0.5) - 1.0f;
        float mouseY = y / (m_height * 0.5) - 1.0f;

        glm::vec4 viewport(0, 0, m_width, m_height);
        m_config.ray_origin = glm::unProject(glm::vec3(x, y, 0), m_config.model, m_config.projection, viewport);
        m_config.ray_dest = glm::unProject(glm::vec3(x, y, 1), m_config.model, m_config.projection, viewport);
        
        printf("click on screen (%g %g), ray (%g %g %g) -> (%g %g %g)\n", 
            mouseX, mouseY, m_config.ray_origin[0], m_config.ray_origin[1], m_config.ray_origin[2], 
                            m_config.ray_dest[0], m_config.ray_dest[1], m_config.ray_dest[2]);
    }
}

void CGLWindow::mouseReleaseEvent(int button, int mods, int x, int y)
{
}

void CGLWindow::keyPressEvent(int key, int scancode, int mods)
{
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
        m_config.select_on = true;
}

void CGLWindow::keyReleaseEvent(int key, int scancode, int mods)
{
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
        m_config.select_on = false;
}

void CGLWindow::charEvent(int code)
{
    static int s_texture_mode = 2;
    static int s_surface_mode = 0;

    switch (code)
    {

    case 'p':
        for (auto& renderer : m_renderers)
        {
            renderer.config().ShowPoints = !renderer.config().ShowPoints;
        }
        break;
    case 'w':
        for (auto& renderer : m_renderers)
        {
            renderer.config().ShowWireframe = !renderer.config().ShowWireframe;
        }
        break;
    case 's':
        ++s_surface_mode;
        for (auto& renderer : m_renderers)
        {
            RendererConfig::Mode m = RendererConfig::SURFACE_SMOOTH;
            switch (s_surface_mode % 3)
            {
            case 0:
                m = RendererConfig::SURFACE_SMOOTH;
                break;
            case 1:
                m = RendererConfig::SURFACE_FLAT;
                break;
            case 2:
                m = RendererConfig::SURFACE_HIDDEN;
                break;
            }
            renderer.config().setFlatOrSmooth(m);
        }
        break;
    case 't':
        ++s_texture_mode;
        for (auto& renderer : m_renderers)
        {
            RendererConfig::Mode m = RendererConfig::TEXTURE_DISABLE;
            switch (s_texture_mode % 4)
            {
            case 0:
                m = RendererConfig::TEXTURE_DISABLE;
                break;
            case 1:
                m = RendererConfig::TEXTURE_NORMAL;
                break;
            case 2:
                m = RendererConfig::TEXTURE_REPLACE;
                break;
            case 3:
                m = RendererConfig::TEXTURE_BLENDED;
                break;
            }

            renderer.config().TextureMode = m;
        }
        break;
    case 'b':
        for (auto& renderer : m_renderers)
        {
            renderer.config().ShowCullFace = !renderer.config().ShowCullFace;
        }
        break;
    default:
        break;
    }
}

void CGLWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    CGLWindow* self = (CGLWindow*)glfwGetWindowUserPointer(window);
    if (!self) return;
    auto& conf = self->m_config;

    printf("CGLWindow scroll. xoffset:%g, yoffset:%g\n", xoffset, yoffset);
    if (yoffset != 0)
    {
        conf.arcball.zoom((float) -yoffset * 0.05f);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void CGLWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    CGLWindow* self = (CGLWindow*)glfwGetWindowUserPointer(window);
    if (!self) return;
    auto& conf = self->m_config;

    self->resizeGL(width, height);
}

void CGLWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    CGLWindow* self = (CGLWindow*)glfwGetWindowUserPointer(window);
    if (!self) return;
    auto& conf = self->m_config;
    
    double x, y;
    int w, h;
    glfwGetCursorPos(window, &x, &y);
    glfwGetWindowSize(window, &w, &h);

    if (action == GLFW_PRESS)
    {
        self->mousePressEvent(button, mods, x, y);
    }
    else if (action == GLFW_RELEASE)
    {
        self->mouseReleaseEvent(button, mods, x, y);
    }

    const glm::vec2 cur_curror = glm::vec2(x * 2.f / w - 1.f, 1.f - 2.f * y / h);
    conf.prev_curror = cur_curror;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        conf.rotate_on = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        conf.rotate_on = false;
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        conf.translate_on = true;
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        conf.translate_on = false;
    }
}

void CGLWindow::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
{
    CGLWindow* self = (CGLWindow*)glfwGetWindowUserPointer(window);
    if (!self) return;
    auto& conf = self->m_config;
    
    self->mouseMoveEvent(xpos, ypos);

    double x, y;
    int w, h;
    glfwGetCursorPos(window, &x, &y);
    glfwGetWindowSize(window, &w, &h);

    const glm::vec2 cur_curror = glm::vec2(x * 2.f / w - 1.f, 1.f - 2.f * y / h);

    if (conf.rotate_on)
    {
        if (conf.prev_curror != glm::vec2(-2.f))    // not first time
        {
            conf.arcball.rotate(conf.prev_curror, cur_curror);
        }
        conf.prev_curror = cur_curror;
    }

    if (conf.translate_on)
    {
        if (conf.prev_curror != glm::vec2(-2.f))    // not first time
        {
            glm::vec2 motion = cur_curror - conf.prev_curror;
            motion *= 0.5;
            conf.arcball.pan(motion);
        }
        conf.prev_curror = cur_curror;
    }
}

void CGLWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    CGLWindow* self = (CGLWindow*)glfwGetWindowUserPointer(window);
    if (!self) return;
    
    if (action == GLFW_PRESS)
    {
        self->keyPressEvent(key, scancode, mods);
    }
    else if (action == GLFW_RELEASE)
    {
        self->keyReleaseEvent(key, scancode, mods);
    }
}

void CGLWindow::character_callback(GLFWwindow* window, unsigned int codepoint) 
{
    CGLWindow* self = (CGLWindow*)glfwGetWindowUserPointer(window);
    if (!self) return;

    self->charEvent(codepoint);
}

} // namespace DartLib
