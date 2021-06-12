#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    /****************************************
                    Step 1
    *****************************************/
    glfwInit();
    //options for glfw windows can be found here
    //https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




    /****************************************
                    Step 2 Create Context
    *****************************************/
    GLFWwindow* window = glfwCreateWindow(800, 600, "MyOpenGLWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);



    /****************************************
                Step 3 Manage
    *****************************************/
    // glad manage function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /****************************************
            Step 4 Data Providing
    *****************************************/

    //vertex buffer objects [VBO] stores a large number of vertices in the GPU's memory
    //Just like any object in OpenGL, this buffer has a unique ID corresponding
    //to that buffer, so we can generate one with a buffer ID using the glGenBuffers function: 
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    //any buffer calls we make will be used to configure the currently bound buffer, which is VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //copies the previously defined vertex data into the Buffers Memory which is VBO cause we binded it
    //4 parameter how graphics card manage the given data
    // we use GL_STATIC_DRAW becasue triangle is not moving
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //see also
    //GL_STREAM_DRAW: the data is set only onceand used by the GPU at most a few times.
    //GL_STATIC_DRAW : the data is set only onceand used many times.
    //GL_DYNAMIC_DRAW : the data is changed a lotand used many times.

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";



    /****************************************
                Step 5 Vertex shader
    *****************************************/

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    /****************************************
            Step 6 Fragment shader
    *****************************************/

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";


    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    /****************************************
             Step 7 shader programm
    *****************************************/
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        glUseProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    //first two parameters set the location of the lower left corner of the window
    glViewport(0, 0, 800, 600);

    /*
        Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D 
        coordinates it processed to coordinates on your screen. For example, a processed 
        point of location (-0.5,0.5) would (as its final transformation) be mapped to (200,450) 
        in screen coordinates. Note that processed coordinates in OpenGL are between -1 and 1 
        so we effectively map from the range (-1 to 1) to (0, 800) and (0, 600). 
    */



    //for resize purposes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // input
        processInput(window);

        // rendering commands here
        //...

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    // clean all glfw resources
    glfwTerminate();
    return 0;


}

