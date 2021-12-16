#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "Shader.h"
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

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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
        float vert[] = {
            0.5f, 0.0f,
            0.0f, 0.5f,
           -0.5f, 0.0f
        };

        GLuint indx[] = {

            0,1,2

        };

        /* Buffer Creation */
           
        //Initializing VBO and VAO
        
        //VBO Generation
        GLuint VBO, VAO, EBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //EBO Generation
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), vert, GL_STATIC_DRAW);

        //VAO Generation
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), vert, GL_STATIC_DRAW);

        //VBO Layout Definition
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        

        //Unbinding VAOs
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        std::cerr << "Got here! " << std::endl;

        /* Get window metadata*/

        //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //const GLFWvidmode* vidAttr = glfwGetVideoMode(monitor);

        //std::cout << " width is " << vidAttr->width << " height is " << vidAttr->height << std::endl;

        //locate uniform addresses


        /* Loop until the user closes the window */

        while (!glfwWindowShouldClose(window))
        {

            glClear(GL_COLOR_BUFFER_BIT);
            /* Render here */

            glBindVertexArray(VAO);

            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indx);
            glBindVertexArray(0);
            /* Swap front and back buffers */

            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
            
        }
    }
    glfwTerminate();
    return 0;
}