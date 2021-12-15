#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "GLData/VertexBuffer.h"
#include "GLData/IndexBuffer.h"
#include "GLData/VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "GLData/Texture.h"

/**
*  \brief Callback function that defines debug behaviour. 
*
*  \return void
*/
void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}


int main(void)
{

    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 960, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //vsync
    glfwSwapInterval(1);

    /* Initialize GLEW*/

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    /* Init GL Debugging */

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    /* Vertex Data*/
    {
        float position[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, //0
             0.4f, -0.4f, 1.0f, 0.0f, //1
             0.5f,  0.5f, 1.0f, 1.0f, //2
            -0.4f,  0.4f, 0.0f, 1.0f  //3
        };

        GLuint indices[] = {

            0,1,2,
            2,3,0

        };

        /* Buffer Creation */
           
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        VertexArray va;
        VertexBuffer vb(position, 4 * 4 * sizeof(GLfloat));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind(); 
        shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        
        Texture texture("res/textures/Grass.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        /* Get window metadata*/

        //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //const GLFWvidmode* vidAttr = glfwGetVideoMode(monitor);

        //std::cout << " width is " << vidAttr->width << " height is " << vidAttr->height << std::endl;

        //locate uniform addresses

        Renderer renderer;

        /* Loop until the user closes the window */

        float period;

        while (!glfwWindowShouldClose(window))
        {

            period = (float) abs(cos(glfwGetTime()));
            /* Render here */
            renderer.Clear();

            shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
            shader.SetUniform1f("u_Blend", period);

            renderer.Draw(va, ib, shader);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
            
        }
    }
    glfwTerminate();
    return 0;
}