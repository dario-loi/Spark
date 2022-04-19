#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_MESSAGES
#define GLM_FORCE_ALIGNED_GENTYPES
#define GLM_FORCE_INTRINSICS 
#define GLM_PRECISION_MEDIUMP_FLOAT
#define GLM_PRECISION_MEDIUMP_INT
#define GLM_PRECISION_MEDIUMP_DOUBLE
#define GLM_PRECISION_MEDIUMP_UINT
#include <glm.hpp>

#include <vec3.hpp>
#include <math.h>
#include <random>
#include <memory>
#include <vector>
#include <array>
#include <time.h>

#include "GLData/Model.h"
#include "GLData/Buffers/VAO.h"
#include "GLData/Instance.h"
#include "GLData/Texture.h"

#include "Controllers/RandomGenerator.h"

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

constexpr int width = 1280;
constexpr int height = 960;

float lastX = width / 2;
float lastY = height / 2;

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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->Move(-camera->getUpDirection());
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->Move(camera->getUpDirection());
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

namespace deleter {

    template< typename T >
    struct array_deleter
    {
        void operator ()(T const* p)
        {
            delete[] p;
        }
    };

}

int main(void)
{

    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "D-Engine", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //vsync
    glfwSwapInterval(1);

    //Enable MSAA
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    //Enable Face Culling for additional performance 
    glEnable(GL_CULL_FACE);
    //Enable Depth Testing
    glEnable(GL_DEPTH_TEST);
    //Enable Gamma Correction (Turn on as soon as we have proper lightning processed)
    //glEnable(GL_FRAMEBUFFER_SRGB);

    //Enable Mouse in
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    /* Initialize GLEW*/

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error!" << std::endl;
    }

    /* Init GL Debugging */

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);

    
    {
        /* 
            Vertex Data Initialization
        */
        auto x = 1.0f;
        auto y = 1.0f;
        auto z = 1.0f;

        constexpr unsigned int n_verts = 8 * 3;

        auto temp_v = new float[n_verts] {         
            -x, -y, -z, // 0 
             x, -y, -z, // 1
             x,  y, -z, // 2
            -x,  y, -z, // 3
            -x, -y,  z, // 4
             x, -y,  z, // 5
             x,  y,  z, // 6
             -x, y,  z  // 7
        };

        auto vert = std::make_unique<float[]>(n_verts);

        memcpy(vert.get(), temp_v, n_verts * sizeof(float)); // "at this point just directly use a raw pointer" i know, i know...

        delete[] temp_v;

        constexpr unsigned int n_indxs = 6*2*3;

        auto temp_i = new unsigned int[n_indxs] {
                4, 5, 6, //z+ front
                4, 6, 7,

                7, 3, 0, //x+ right
                0, 4, 7,

                7, 6, 2, //y+ top
                2, 3, 7,

                6, 5, 1, //x- left
                2, 6, 1,

                1, 5, 4, //y- bottom
                0, 1, 4,

                2, 1, 0, //z- back
                3, 2, 0

        };

        auto indx = std::make_unique<unsigned int[]>(n_indxs);

        memcpy(indx.get(), temp_i, n_indxs * sizeof(unsigned int));

        delete[] temp_i;
        
        /*
            Create Model
        */
        
        Model triangle(std::move(vert), n_verts, std::move(indx), n_indxs);
        triangle.getVAO().add_attr<float>(3);
        triangle.ModelInit();

        triangle.Bind();

        /*
            Create Instances
        */

        //Randomly gen 100 instances far away
        std::vector<Instance> instances;

        unsigned int INSTANCES = 100;

        instances.reserve(INSTANCES);

        auto posDistribution = RandomGenerator::getRealDistribution(-10, 10);
        auto gen = RandomGenerator::getGenerator();
        for (int c = 0; c < INSTANCES; ++c)
        {
            auto position = glm::vec3(
                posDistribution(gen),
                posDistribution(gen),
                posDistribution(gen)
            );

            instances.emplace_back(Instance(&triangle, position));
            instances.at(c).Scale(glm::vec3(0.3f));
        }

        /*
            Creating Texture (Temporary workaround for testing, will move this into either Instance or Model depending on design choices
        */

        std::array<Texture, 2> textures{
            {
                Texture("res/textures/Grass.png", GL_TEXTURE_CUBE_MAP),
                Texture("res/textures/Bricks512x512.jpg", GL_TEXTURE_CUBE_MAP)
            }
        };

        /*
            Init Shader
        */

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        /* Loop until the user closes the window */
        
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
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            unsigned int inst_indx = 0;

            /* Render here */
            for (auto&& inst : instances)
            {
                textures.at((inst_indx % 2)).Bind((inst_indx % 2));
                shader.SetUniform1i("u_Texture", (inst_indx % 2));
                
                shader.setUniform4mat("u_mMatrix", inst.getModelMatrix());

                inst.Draw();
                inst.Rotate(glm::vec3(0.2f, 1.0f, 0.4f));

                inst_indx += 1;
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
            
        }

    }
    glfwTerminate();


    std::clog << "checking for leaks.. " << std::endl;

    return 0;
}