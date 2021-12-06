#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

/* Debug Function for callbacks*/
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

    float position[12] = {
       -0.5f, -0.5f,
        0.2f,  0.6f,
        0.4f, -0.5f,

       -0.5f,  0.3f,
    };

    GLuint indices[6] = {

        0,1,2,
        0,1,3

    };

    /* Buffer Creation */

    VertexBuffer vb(position, 4 * 2 * sizeof(GLfloat));
    IndexBuffer ib(indices, 6);

    ib.Bind();


    /* Initializing VAOs*/

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

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
        uniform int u_Width;
        uniform int u_Height;
        
        void main()
        {
            float y = u_Height - gl_FragCoord.y;
            
            float grad_x = gl_FragCoord.x / u_Width;
            float grad_y = y / u_Height;
            float grad_z = (gl_FragCoord.x * y) / (u_Height * u_Width);

            vec4 fin_Color = vec4( u_Color.x * grad_x, u_Color.y * grad_y, u_Color.z * grad_z , 1.0f);

            color = fin_Color;
            
        }
    )";

    GLuint shader = CreateShader(vertexShader, fragmentShader);

    glUseProgram(shader);

    /* Get window metadata*/
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidAttr = glfwGetVideoMode(monitor);

    std::cout << " width is " << vidAttr->width << " height is " << vidAttr->height << std::endl;

    //locate uniform addresses

    GLint c_Addr = glGetUniformLocation(shader, "u_Color");
    glUniform4f(c_Addr, 1.0f, 1.0f, 1.0f, 1.0f);

    GLint w_Addr = glGetUniformLocation(shader, "u_Width");
    glUniform1i(w_Addr, vidAttr->width);

    GLint h_Addr = glGetUniformLocation(shader, "u_Height");
    glUniform1i(h_Addr, vidAttr->height);

    GLfloat r = 0.0f;
    GLfloat increment = 0.01f;
    GLfloat scaling = 0.0f;
    GLfloat scalingInv = 0.0f;
 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform4f(c_Addr, scaling, scalingInv, scaling, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (scaling > 1.0f)
            increment = -0.05f;
        else if (scaling < 0.0f)
            increment = 0.05f;

        scaling += increment;

        scalingInv = 1.0f - scaling;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}