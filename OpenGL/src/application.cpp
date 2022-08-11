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
#include <algorithm>
#include <memory>
#include <vector>
#include <array>

#include <time.h>

#include "GLData/Model.h"
#include "GLData/Buffers/VAO.h"
#include "GLData/Buffers/UBO.h"
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
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, source = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, source, message);
}

constexpr const int width = 1280;
constexpr const int height = 720;

float lastX = width / 2;
float lastY = height / 2;

Camera cam(1000.0f, 4 / 3, 5.0f);


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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Spark", nullptr, nullptr);
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
        /*
        Model robot = importObj("res/model/robot/source/robot.obj");
            
        robot.getVAO().add_attr<float>(3); //Position
        robot.getVAO().add_attr<float>(3); //Normal
        robot.getVAO().add_attr<float>(2); //Texture
        robot.getVAO().add_attr<float>(3); //Tangent
        robot.ModelInit();
        */

        Model cube = importObj("res/model/cube/cube.obj");
        cube.getVAO().add_attr<float>(3); //Position
        cube.getVAO().add_attr<float>(3); //Normal
        cube.getVAO().add_attr<float>(2); //Texture
        cube.getVAO().add_attr<float>(3); //Tangent
        cube.ModelInit();


        /*
            Create Instances
        */

        //Randomly gen INSTANCES instances
        std::vector<Instance> instances;

        constexpr const unsigned int INSTANCES = 1000;

        instances.reserve(INSTANCES);

        {
            auto model_ptr = std::make_shared<Model>(cube);
            auto gen = RandomGenerator::getGenerator();
            auto dist = RandomGenerator::getRealDistribution(-50.f, 50.f);
            auto dist_scale = RandomGenerator::getRealDistribution(0.75f, 1.25f);
            auto dist_rot = RandomGenerator::getRealDistribution(0.f, 359.9f);

            for (int c = 0; c < INSTANCES; ++c)
            {
                auto position = glm::vec3(
                    dist(gen), dist(gen), dist(gen)
                );

                instances.emplace_back(model_ptr, position);
                instances.at(c).Rotate(glm::vec3(dist_rot(gen), dist_rot(gen), dist_rot(gen)));
                instances.at(c).Scale(glm::vec3(dist_scale(gen), dist_scale(gen), dist_scale(gen)));
            }
        }

        std::vector<Instance> lights;

        const constexpr unsigned int NUM_LIGHTS = 2;

        for (size_t i = 0; i < NUM_LIGHTS; ++i)
        {
            lights.push_back({ std::make_shared<Model>(cube), { 5.0f , -25.0f, 0.0f - (static_cast<float>(i)) * 4.0f } });
        }

        /*
            Creating Texture (Temporary workaround for testing, will move this into either Instance or Model depending on design choices)
        */
        auto robot_tex  = Texture("res/textures/grass.bmp", GL_TEXTURE_2D, GL_SRGB8_ALPHA8);
        auto robot_spec = Texture("res/textures/grass_spec.bmp", GL_TEXTURE_2D, GL_RGBA8);
        auto robot_norm = Texture("res/textures/grass_normal.bmp", GL_TEXTURE_2D, GL_RGBA8);

        /*
            Init Shader
        */

        auto shader = Shader("res/shaders/Phong.shader");
        auto light_shader = Shader("res/shaders/Light.shader");
        
        shader.Bind();

        /*
            Initialize UBOs
        */
        
        std::vector<glm::vec4> positions(lights.size());

        std::transform(lights.begin(), lights.end(), positions.begin(), [](Instance const& inst) -> glm::vec4 {
            return glm::vec4(inst.getTransform().vDisplacement.vector, 1.0f);
            });

        UBO light_positions{std::move(positions), 1};
        light_positions.Bind();

        using cam_struct = struct {
            glm::mat4 view;
            glm::mat4 proj;
            glm::vec3 pos;
        };

        std::vector<cam_struct> cam_mat{ {cam.getView(1.0f), cam.getProj(), cam.getPos()} };
        UBO camera_data{ std::move(cam_mat), 2 };
        camera_data.Bind();
        
        std::cout << sizeof(glm::mat3);

        /* Loop until the user closes the window */
        
        float lastFrame = 0.0f;
        float thisFrame;

        shader.SetUniform1ui("nLights", NUM_LIGHTS);
        shader.Unbind();

        while (!glfwWindowShouldClose(window))
        {
            
            thisFrame = static_cast<float>(glfwGetTime());
            float deltaTime = thisFrame - lastFrame;
            lastFrame = thisFrame;

            processInput(window, &cam);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            size_t inst_indx = 0;

            /* Render here */

            light_shader.Bind();

            camera_data.getElementAt(0).view = cam.getView(deltaTime);
            camera_data.getElementAt(0).pos = cam.getPos();
            camera_data.Update();

            for (size_t indx = 0; auto& light : lights)
            {

                light.getModel()->Bind();

                light_shader.setUniform4mat("u_mMatrix", light.getModelMatrix());

               /* light.Move(
                    glm::vec3(
                        0.01f + (0.02f * ((int) indx - 1)), 0.0f, 0.0f
                    )
                );*/

                light_positions.Bind();
                light_positions.setSubData(glm::vec4(light.getTransform().vDisplacement.vector, 1.0f), indx++);
                
                light.Draw();
                light.getModel()->Unbind();
            }

            light_positions.Update();
            light_shader.Unbind();

            shader.Bind();

            instances[0].getModel()->Bind();
            robot_tex.Bind(0);
            robot_spec.Bind(1);
            robot_norm.Bind(2);

            shader.SetUniform1i("u_Texture", 0);
            shader.SetUniform1i("u_Specular", 1);
            shader.SetUniform1i("u_NormalMap", 2);

            for (auto& inst : instances)
            {

                shader.setUniform4mat("u_normalMatrix", inst.getNormalMatrix());
                shader.setUniform4mat("u_mMatrix", inst.getModelMatrix());
                
                inst.Rotate({ 0.0f, 0.1f, 0.0f });
                inst.Draw();

                ++inst_indx;
            }
            instances[0].getModel()->Unbind();
            shader.Unbind();

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