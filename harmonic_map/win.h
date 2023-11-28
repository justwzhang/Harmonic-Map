#ifndef _DARTLIB_WIN_
#define _DARTLIB_WIN_

#include <string>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>  // glm::value_ptr

//#include "linmath.h"
#include "Arcball.h"
#include "Shader.h"
#include "SimpleObjectData.hpp"

namespace DartLib
{

struct CWinConf
{   
    // used to translation (on x-y plane) and scale (on z-axis)
    bool translate_on = false;
    glm::vec3 translate = glm::vec3(0.f, 0.f, 0.f);
    
    bool rotate_on = false;

    int last_curror[2];

    Arcball arcball = Arcball(600, 600, 0.1f, true, true);
};


static void window_close_callback(GLFWwindow* window) { }



class CWindow
{
  protected:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    std::string m_title;

    static CWinConf m_conf;
    glm::mat4 m_model, m_projection, m_view, m_mvp;

    GLuint m_vertex_buffer, m_vertex_shader, m_fragment_shader, m_program;
    GLint m_mvp_location, m_vpos_location, m_vcol_location;

    Shader m_shader;
    GLuint m_cube_buffers[2];
    GLuint m_floor_buffers[2];
  public:
    virtual void on_initialize(const GLFWwindow*);

    //virtual void on_resized(int w, int h);

    virtual bool on_render();
  protected:
    void render();

  public:

    virtual void on_exit(){};

    static void on_mouse_button_click(GLFWwindow* window, int button, int action, int mods);
    static void on_mouse_move(GLFWwindow* window, double xpos, double ypos);
    static void on_scroll(GLFWwindow* window, double xoffset, double yoffset);
    static void on_resized(GLFWwindow* window, int width, int height);
  public:
    CWindow(std::string title, int width, int height);
    ~CWindow();

    static void init();
    static void terminate();

    void run();
  protected:

};

void CWindow::on_initialize(const GLFWwindow*) 
{
    // register event
    // glfwSetWindowCloseCallback(m_window, window_close_callback);
    glfwSetMouseButtonCallback(m_window, &CWindow::on_mouse_button_click);
    glfwSetCursorPosCallback(m_window, &CWindow::on_mouse_move);
    glfwSetScrollCallback(m_window, &CWindow::on_scroll);
    glfwSetWindowSizeCallback(m_window, &CWindow::on_resized);

    
    //glGenBuffers(1, &m_vertex_buffer);
    //glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(2, m_cube_buffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_cube_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cube_buffer_data.size(), &cube_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_cube_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cube_normals_data.size(), &cube_normals_data[0], GL_STATIC_DRAW);

        
    glGenBuffers(2, m_floor_buffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_floor_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * floor_buffer_data.size(), &floor_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_floor_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * floor_normals_data.size(), &floor_normals_data[0], GL_STATIC_DRAW);


    m_shader.init("T:/dev/DartLib/examples/gui/shaders/simpleLighting.vsh", 
                  "T:/dev/DartLib/examples/gui/shaders/simpleLighting.fsh");
    /*
    m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(m_vertex_shader);

    m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(m_fragment_shader);
    
    m_program = glCreateProgram();
    glAttachShader(m_program, m_vertex_shader);
    glAttachShader(m_program, m_fragment_shader);
    glLinkProgram(m_program);
    */

    //m_mvp_location = glGetUniformLocation(m_program, "MVP");
    //m_vpos_location = glGetAttribLocation(m_program, "vPos");
    //m_vcol_location = glGetAttribLocation(m_program, "vCol");

    //glEnableVertexAttribArray(m_vpos_location);
    //glVertexAttribPointer(m_vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);
    //glEnableVertexAttribArray(m_vcol_location);
    //glVertexAttribPointer(m_vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 3));
}

void CWindow::on_resized(GLFWwindow* window, int width, int height)
{
    printf("resize...\n");

    glViewport(0, 0, width, height);
    
    // need glPerspective
    
}

bool CWindow::on_render()
{
    //printf("on render\n");
    return true;
}

void CWindow::render() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // refer: https://github.com/subokita/Arcball/blob/master/Arcball/main.cpp
    m_model = m_model * m_conf.arcball.createModelRotationMatrix(m_view);
    glm::mat4 m = glm::translate(glm::mat4(1.0f), m_conf.translate);
    m_model += m;

    //m_view = m_view * m_conf.arcball.createViewRotationMatrix();
    //glm::mat4 m = glm::translate(glm::mat4(1.0f), m_conf.translate);
    //m_view += m;

    m_mvp = m_projection * m_view * m_model;
    m_model = glm::mat4(1.0);
    //m_view = glm::lookAt(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

    on_render();

    m_shader.setUniform("v", m_view);
    m_shader.setUniform("m", m_model);
    m_shader.setUniform("p", m_projection);
    m_shader.setUniform("mvp", m_mvp);

    /* Draw cube */
    m_shader.setUniform("light_color", glm::vec3(1.0, 1.0, 0.0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_cube_buffers[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_cube_buffers[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glEnable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<int>(cube_buffer_data.size()));

    /* Draw the floor, with different color */
    m_shader.setUniform("light_color", glm::vec3(0.0, 1.0, 1.0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_floor_buffers[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_floor_buffers[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glDisable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<int>(floor_buffer_data.size()));
}

void CWindow::on_mouse_button_click(GLFWwindow* window, int button, int action, int mods) 
{
    printf("mouse button right pressed. button:%d, action:%d, mods:%d\n", 
           button, action, mods);
    m_conf.arcball.mouseButtonCallback(window, button, action, mods);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        m_conf.arcball.cursorCallback(window, x, y);
        m_conf.rotate_on = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        m_conf.rotate_on = false;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        m_conf.last_curror[0] = x, m_conf.last_curror[1] = y;
        m_conf.translate_on = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        m_conf.translate_on = false;
    }
}

void CWindow::on_mouse_move(GLFWwindow* window, double xpos, double ypos) 
{
    if (m_conf.rotate_on)
    {
        printf("on_mouse_move %g %g\n", xpos, ypos);
        if (xpos != m_conf.last_curror[0] || ypos != m_conf.last_curror[1])
        {
            m_conf.arcball.cursorCallback(window, xpos, ypos);
        }
    }
    if (m_conf.translate_on)
    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        double scale = 10. / h;
        glm::vec3 trans = glm::vec3(scale * (xpos - m_conf.last_curror[0]), 
                                    scale * (m_conf.last_curror[1] - ypos), 
                                    0);
        m_conf.last_curror[0] = xpos;
        m_conf.last_curror[1] = ypos;
        m_conf.translate += trans;
    }
}

void CWindow::on_scroll(GLFWwindow* window, double xoffset, double yoffset) 
{
    printf("scroll. xoffset:%g, yoffset:%g\n", xoffset, yoffset);
    if (yoffset != 0)
    {
        glm::vec3 trans = glm::vec3(0, 0, yoffset);
        m_conf.translate += trans;
    }
}

CWindow::CWindow(std::string title, int width, int height)
{
    m_title = title;
    m_width = width;
    m_height = height;
}

CWindow::~CWindow()
{
    glfwDestroyWindow(m_window);
}

void CWindow::init() 
{
    const char* err_msg;

    // initialize GLFW
    if (!glfwInit())
    {
        glfwGetError(&err_msg);
        printf("[ERROR] %s\n", err_msg);
        exit(EXIT_FAILURE);
    }

}

void CWindow::terminate() 
{
    glfwTerminate();
    ;
}

void CWindow::run() 
{
    const char* err_msg;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
    //glfwWindowHint(GLFW_STENCIL_BITS, 1);

    //glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    //glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    //glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    //glfwWindowHint(GLFW_FOCUSED, GL_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);

    if (m_window == NULL)
    {
        glfwGetError(&err_msg);
        printf("[ERROR] %s\n", err_msg);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make OpenGL context current
    glfwMakeContextCurrent(m_window);
    
    // Initialize OpenGL loader
    if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0)
    {
        printf("Failed to initialize GLAD OpenGL loader!");
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);
    // enable MSAA and depth testing in OpenGL
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_MULTISAMPLE);


    on_initialize(m_window);


    m_model = glm::mat4(1.0);
    m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
        glm::vec3(0., 0., 0.), glm::vec3(0., 1., 0.));
    m_projection = glm::perspective(glm::radians(40.0f), (float)m_width/m_height, 0.1f, 100.0f);
 
    glm::vec3 light_position = glm::vec3(4, 4, 4);
    m_shader.bind();
    m_shader.setUniform("light_position_w", light_position);

    while (!glfwWindowShouldClose(m_window))
    {
        //printf("running...\n");
        int width, height;
        
        glfwGetFramebufferSize(m_window, &width, &height);        
        if (width != m_width || height != m_height)
        {
            //on_resized(width, height);
            m_width = width;
            m_height = height;
        }    
        render();
        
        //glfwMakeContextCurrent(m_window);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    m_shader.unbind();
}

// configuration class
CWinConf CWindow::m_conf;

}
#endif //! _DARTLIB_WIN_