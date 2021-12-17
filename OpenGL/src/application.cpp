#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vec3.hpp>
#include <math.h>

#include "GLData/Model.h"
#include "GLData/Buffers/VAO.h"
#include "GLData/Instance.h"

#include <vector>

#include "Shader.h"
#include "Camera.h"

#define LOG(x) std::cerr<<x<<std::endl;
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
        unsigned int n_verts = 24;
        float* vert = new float[n_verts]
        {
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f
        };

        unsigned int n_indxs = 36;
        GLuint* indx = new GLuint[n_indxs]
        {
                0, 1, 2, // -x
                1, 3, 2,

                4, 6, 5, // +x
                6, 7, 5,

                0, 5, 1, // -y
                5, 6, 1,

                2, 6, 3, // +y
                6, 7, 3,

                0, 4, 2, // +z
                4, 6, 2,

                1, 7, 3, // -z
                5, 7, 1
        };

        /*
            Create Model
        */
        


        Model triangle(vert, n_verts, indx, n_indxs);
        triangle.getVAO().add_attr<float>(3);
        triangle.ModelInit();

        triangle.Bind();

        /*
            Create Instances
        */


        std::vector<Instance> instances;
        
        float offset = 0.25f;
        for (int y = -2; y < 2; y += 2)
        {
            for (int x = -2; x < 2; x += 2)
            {
                glm::vec3 translation;
                translation.x = (float)x / 4.0f + offset;
                translation.y = (float)y / 4.0f + offset;
                translation.z = 1.0f;
                instances.push_back(Instance(&triangle, translation));

                instances.at(instances.size() - 1).Rotate(glm::vec3(0.0f, 0.0f, 90.0f * abs(x) ) );

            }
        }
        


        /*
            Init Shader
        */

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        /* Get window metadata*/

        //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //const GLFWvidmode* vidAttr = glfwGetVideoMode(monitor);

        //std::cout << " width is " << vidAttr->width << " height is " << vidAttr->height << std::endl;

        //locate uniform addresses


        /* Loop until the user closes the window */

        unsigned int* indices = triangle.getIndexReference();
        
        float lastFrame = 0.0f;
        float thisFrame;

        Camera cam(90.0f,  4 / 3);

        shader.setUniform4mat("projection", cam.getProj());

        while (!glfwWindowShouldClose(window))
        {
            thisFrame = glfwGetTime();
            float deltaTime = thisFrame - lastFrame;
            lastFrame = thisFrame;

            shader.setUniform4mat("view", cam.getView());
            

            glClear(GL_COLOR_BUFFER_BIT);
            /* Render here */
            for (auto&& inst : instances)
            {
                shader.setUniform4mat("u_mMatrix", inst.getModelMatrix());

                glDrawElements(GL_TRIANGLES, inst.getModel()->getIndexSize(), GL_UNSIGNED_INT, inst.getModel()->getIndexReference());

                inst.Rotate(glm::vec3(0.0f, 0.0f, 1.0f));

            }



            /* Swap front and back buffers */

            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
            
            std::cerr << "Frame time: " << deltaTime << std::endl;
        }
    }
    glfwTerminate();

    std::cerr << "checking for leaks.. " << std::endl;

    return 0;
}