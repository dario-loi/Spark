#include <iostream>
#include <filesystem>

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
#include "Utility/Importer.h"

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
constexpr int height = 720;

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

    //Make the window's context current
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

    //Gamma Correction
    glEnable(GL_FRAMEBUFFER_SRGB);



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
        Model guitar = importObj("res/model/robot/source/robot.obj");
            
        guitar.getVAO().add_attr<float>(3); //Position
        guitar.getVAO().add_attr<float>(3); //Normal
        guitar.getVAO().add_attr<float>(2); //Texture
        guitar.ModelInit();
        
        /*
            Create Instances
        */

        //Randomly gen INSTANCES instances
        std::vector<Instance> instances;

        constexpr const unsigned int INSTANCES = 1;

        instances.reserve(INSTANCES);

        auto posDistribution = RandomGenerator::getRealDistribution(-10, 10);
        auto rotDistribution = RandomGenerator::getRealDistribution(-180, 180);
        auto gen = RandomGenerator::getGenerator();

        {
            auto model_ptr = std::make_shared<Model>(guitar);

            for (int c = 0; c < INSTANCES; ++c)
            {
                auto position = glm::vec3(
                    posDistribution(gen),
                    posDistribution(gen),
                    posDistribution(gen)
                );

                instances.emplace_back(model_ptr, position);
                instances.at(c).Rotate(glm::vec3(180.0f, 0.0f, 0.0f));
                instances.at(c).Scale(glm::vec3(0.5f));
            }
        }

        /*
            Creating Texture (Temporary workaround for testing, will move this into either Instance or Model depending on design choices
        */

        auto guitar_tex = Texture("res/model/robot/textures/texture.jpg", GL_TEXTURE_2D);
        auto guitar_spec = Texture("res/model/robot/textures/specular.jpeg", GL_TEXTURE_2D);

        /*
            Init Shader
        */

        auto shader = Shader("res/shaders/Guitar.shader");
        
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
            shader.SetUniform3f("u_CameraPos", cam.getPos());
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            unsigned int inst_indx = 0;

            /* Render here */
            instances[0].getModel()->Bind();
            guitar_tex.Bind(0);
            guitar_spec.Bind(1);
            for (auto& inst : instances)
            {
                inst.Rotate(glm::vec3(0.0f, 0.1f, 0.0f));

                shader.SetUniform1i("u_Texture", 0);
                shader.SetUniform1i("u_Specular", 1);
                shader.setUniform4mat("u_mMatrix", inst.getModelMatrix());
                shader.setUniform3mat("u_normalMatrix", inst.getNormalMatrix());

                inst.Draw();

                inst_indx += 1;
            }
            instances[0].getModel()->Unbind();

            if (!glfwWindowShouldClose(window))
            {
                /* Swap front and back buffers */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                glfwPollEvents();
            }
        }

    }
    glfwTerminate();

    std::clog << "Checking for leaks.. " << std::endl;

    return 0;
}