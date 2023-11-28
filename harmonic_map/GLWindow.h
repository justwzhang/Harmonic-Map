#ifndef _CGLWINDOW_
#define _CGLWINDOW_

#include "gl/VertexAttribLayout.h"
#include "gl/Renderer.h"
#include "gl/Texture.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Arcball.h"

#include <string>
#include <vector>
#include <map>
#include <mutex>

namespace MeshLib
{

class CGLWindowConfig
{
  public:
    CGLWindowConfig() { printf("CGLWindowConfig init\n"); };
    // used to translation (on x-y plane) and scale (on z-axis)
    bool translate_on = false;
    bool rotate_on    = false;
    bool select_on    = false;
    
    Arcball arcball = Arcball(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    glm::vec2 prev_curror = glm::vec2(-1.0f);

    // for opengl
    glm::mat4 model, view, projection;
    //MyShader shader;

    glm::vec3 ray_origin, ray_dest;
};

class CGLWindow
{
  protected:
    // for glfw
    GLFWwindow* m_window;

    // attributes
    std::string m_title;
    int m_width;
    int m_height;

public:
    CGLWindowConfig m_config;    
//protected:
    // for opengl's shader
    /*
    std::vector<VertexArray*>   m_vertex_arrays;
    std::vector<VertexBuffer*>  m_vertex_buffers;
    std::vector<ElementBuffer*> m_element_buffers;
    std::vector<Shader*>        m_shaders;
    std::vector<Texture*>       m_textures;
    */

    /*!
     *  The pointer of current actived shader, because there maybe a number of
     *    shaders maintained by the subclass.
     */
    //Shader* m_actived_shader;

    std::vector<Renderer> m_renderers;
    
    // lock while create multi windows
    //std::mutex m_mutex;
public:
    CGLWindow();
    CGLWindow(std::string title, int width, int height);
    ~CGLWindow();

    void run();

    void initGLFW();

    void terminate();

protected:

    //--------- events for class CGLWindow
    // register envents
    virtual void on_register_events() 
    {
        glfwSetMouseButtonCallback(m_window, &CGLWindow::mouse_button_callback);
        glfwSetCursorPosCallback(m_window, &CGLWindow::cursor_position_callback);
        glfwSetScrollCallback(m_window, &CGLWindow::scroll_callback);
        glfwSetFramebufferSizeCallback(m_window, &CGLWindow::framebuffer_size_callback);
        glfwSetKeyCallback(m_window, &CGLWindow::key_callback);
        glfwSetCharCallback(m_window, &CGLWindow::character_callback);
    };
    
    virtual void on_prepare_data() {};

    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void destroyGL();

    virtual void mouseMoveEvent(double xpos, double ypos) {};
    virtual void mousePressEvent(int button, int mods, int x, int y);
    virtual void mouseReleaseEvent(int button, int mods, int x, int y);
    virtual void keyPressEvent(int key, int scancode, int mods);
    virtual void keyReleaseEvent(int key, int scancode, int mods);
    virtual void charEvent(int code);

    //--------- events for GLFW window
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void character_callback(GLFWwindow* window, unsigned int codepoint);
};
}
#endif //! _CGLWINDOW_
