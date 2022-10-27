
//OpenGL Libs
#include <winsdkver.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_MESSAGES
#define GLM_PRECISION_MEDIUMP_FLOAT
#define GLM_PRECISION_MEDIUMP_INT
#define GLM_PRECISION_MEDIUMP_DOUBLE
#define GLM_PRECISION_MEDIUMP_UINT

#include <glm.hpp>
#include <vec3.hpp>

//ImGUI

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
constexpr const char* glsl_version = "#version 130";

//STDLIB
#include <cmath>
#include <random>
#include <algorithm>
#include <memory>
#include <vector>
#include <array>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <format>
#include <ranges>

//Custom OpenGL Data Structures
#include "GLData/Model.h"
#include "GLData/Buffers/VAO.h"
#include "GLData/Buffers/UBO.h"
#include "GLData/Instance.h"
#include "GLData/Texture.h"
#include "Utility/Importer.h"
#include "Utility/RandomGenerator.h"
#include "GLData/Shader.h"
#include "Camera.h"

//ResourceManager and Renderer
#include "Spark/ResourceManager.h"
#include "Spark/Renderer.h"

#ifdef _DEBUG
#define LOG(x) std::clog<<(x)<<std::endl;
#else  
#define LOG(x)  
#endif

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

/*
SHOULD NOT be global!
*/
Camera cam(1000.0f, 4 / 3, 5.0f);

/*
MUST be refactored, really ugly!
*/
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

/*
    MUST be refactored! really ugly!
*/
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{

    double xoffset = lastX - xpos;
    double yoffset = ypos  - lastY;

    lastX = xpos;
    lastY = ypos;

    const double sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam.Rotate(xoffset, yoffset);
    
}

int main(void)
{

    using namespace spark;
    using namespace sparkutils;

    ResourceManager manager;
    Renderer renderer{ manager };

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

    //IMGUI init

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));



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

        manager.importModel("res/model/cube/cube.obj", "cube", SPARK_STANDARD_LAYOUT_INSTANCED);
        

        /*
            Create Instances
        */

        //Randomly gen INSTANCES instances
        std::vector<Instance> instances;

        constexpr int INSTANCES = 1000;

        instances.reserve(INSTANCES);

        {
            auto pos_dist = RandomGenerator::getRealDistribution(-50, 50);
            auto engine = sparkutils::randGen.getEngine();

            for (auto i : std::views::iota(0, INSTANCES))
            {
                glm::vec4 pos{ pos_dist(engine), pos_dist(engine), pos_dist(engine), 1.0F };

                manager.spawnObject("cube", pos, std::format("cube{i}", i));
            }
        }

        /*
            Create the lights
        */

        /*
            Creating Texture (Temporary workaround for testing, will move this into either Instance or Model depending on design choices)
        */
        manager.addTexture("res/textures/grass.bmp", "Grass", DIFFUSE_MAP);
        manager.addTexture("res/textures/grass_spec.bmp", "Grass Speculars", SPECULAR_MAP);
        manager.addTexture("res/textures/grass_normal.bmp", "Grass Normals", NORMAL_MAP);
        manager.addTexture("res/textures/emissive.png", "Emissive without corners", EMISSIVE_MAP);



        manager.loadShader("res/shaders/Phong.shader", "Main Lightning Model");
        //tentative initialization outside of the manager
        auto phong{ manager.getShader("Main Lightning Model") };
        phong->SetUniform1i("nLights", SPARK_MAXIMUM_UBO_SIZE);


        manager.loadShader("res/shaders/Light.shader", "Simple Color Shader");

        /*
            Init Shader
        */

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


        // Our state
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (!glfwWindowShouldClose(window))
        {
            thisFrame = static_cast<float>(glfwGetTime());
            float deltaTime = thisFrame - lastFrame;
            lastFrame = thisFrame;

            processInput(window, &cam);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            camera_data.getElementAt(0).view = cam.getView(deltaTime);
            camera_data.getElementAt(0).pos = cam.getPos();
            camera_data.Update();

            renderer.render();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (!glfwWindowShouldClose(window))
            {
                /* Swap front and back buffers */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                glfwPollEvents();
            }
        }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::clog << "Checking for leaks.. " << std::endl;

    return 0;
}
