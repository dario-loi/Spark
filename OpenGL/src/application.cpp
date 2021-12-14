#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "GLData/VertexBuffer.h"
#include "GLData/IndexBuffer.h"
#include "GLData/VertexArray.h"

/**
*  \brief Callback function that defines debug behaviour. $\sum_0^\infty x$
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

static GLuint 
CompileShader(GLuint type, 
    const std::string& source) 
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader!" << std::endl;

        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
        
    return id;
}

static GLuint 
CreateShader(const std::string& vertexShader, 
    const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
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
        float position[21] = {
            0.0f,  0.0f,  0.0f,
            0.5f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.0f,
            0.0f,  0.5f,  0.0f,
           -0.15f,  0.75f, -0.25f,
           -0.15f,  0.25f, -0.25f,
            0.35f,  0.75f, -0.25f,
        };

        GLuint indices[18] = {

            2,0,1,
            2,3,0,
            0,3,5,
            3,4,5,
            3,4,6,
            3,2,6

        };

        /* Buffer Creation */
           
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        VertexArray va;

        VertexBuffer vb(position, 7 * 3 * sizeof(GLfloat));

        VertexBufferLayout layout;
        layout.Push<float>(3);

        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 18);

        ib.Bind();

        /*Creating Shaders*/

        std::string vertexShader = R"(
        #version 330 core
        
        layout(location = 0) in vec4 position;
        
        void main()
        {
            
            gl_Position = position;
            
        }
    )";

        std::string fragmentShader = R"(
        #version 330 core
        
        layout(location = 0) out vec4 color;

        uniform vec4 u_Color;
        
        void main()
        {

            color = u_Color;
            
        }
    )";

        GLuint shader = CreateShader(vertexShader, fragmentShader);

        glUseProgram(shader);

        /* Get window metadata*/

        //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //const GLFWvidmode* vidAttr = glfwGetVideoMode(monitor);

        //std::cout << " width is " << vidAttr->width << " height is " << vidAttr->height << std::endl;

        //locate uniform addresses

        GLint c_Addr = glGetUniformLocation(shader, "u_Color");
        glUniform4f(c_Addr, 1.0f, 0.0f, 0.0f, 1.0f);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {


            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            glUniform4f(c_Addr, 1.0f, 0.0f, 0.0f, 1.0f);

            glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, nullptr);


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

        }
    }
    glfwTerminate();
    return 0;
}