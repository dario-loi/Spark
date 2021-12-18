#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vec3.hpp>
#include <math.h>
#include <random>
#include <vector>
#include <time.h>

#include "GLData/Model.h"
#include "GLData/Buffers/VAO.h"
#include "GLData/Instance.h"

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

constexpr float width = 1280, height = 960;
float lastX = width / 2, lastY = height / 2;

Camera cam(1000.0f, 4 / 3, 2.5f);

void processInput(GLFWwindow* window, Camera* camera)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->Move(camera->getViewDirection());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->Move(-camera->getViewDirection());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->Move(camera->getSideVector());
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->Move(-camera->getSideVector());
    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->Sprint(true);
    else
        camera->Sprint(false);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{

    float xoffset = lastX - xpos;
    float yoffset = ypos  - lastY;

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam.Rotate(xoffset, yoffset);
    
}

int main(void)
{

    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }




    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //vsync
    glfwSwapInterval(1);
    //Enable Mouse in
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

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

        //Randomly gen 100 instances far away
        std::vector<Instance> instances;

        instances.reserve(100);

        srand(time(NULL));

        for (int c = 0; c < 100; ++c)
        {
            instances.push_back(Instance(&triangle, glm::vec3((rand()%2000 - 1000)/100, (rand() % 2000 - 1000) / 100, -10.0f + (rand() % 2000 - 1000) / 1000 )));
            instances.at(c).Scale(glm::vec3(0.5f + (rand() % 2000 - 1000) / 1000, 0.5f + (rand() % 2000 - 1000) / 1000, 0.5f + (rand() % 2000 - 1000) / 1000));
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

        shader.setUniform4mat("projection", cam.getProj());

        while (!glfwWindowShouldClose(window))
        {

            thisFrame = glfwGetTime();
            float deltaTime = thisFrame - lastFrame;
            lastFrame = thisFrame;

            processInput(window, &cam);

            

            shader.setUniform4mat("view", cam.getView(deltaTime));
            
            glClear(GL_COLOR_BUFFER_BIT);
            /* Render here */
            for (auto&& inst : instances)
            {
                shader.setUniform4mat("u_mMatrix", inst.getModelMatrix());

                glDrawElements(GL_TRIANGLES, inst.getModel()->getIndexSize(), GL_UNSIGNED_INT, inst.getModel()->getIndexReference());

                inst.Rotate(glm::vec3(0.0f, 1.0f, 0.0f));

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