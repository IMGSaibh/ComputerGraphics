#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /*
        Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D
        coordinates it processed to coordinates on your screen. For example, a processed
        point of location (-0.5,0.5) would (as its final transformation) be mapped to (200,450)
        in screen coordinates. Note that processed coordinates in OpenGL are between -1 and 1
        so we effectively map from the range (-1 to 1) to (0, 800) and (0, 600).
    */
    //first two parameters set the location of the lower left corner of the window
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";
int main()
{


    /****************************************
                    Step 1
    *****************************************/
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    //options for glfw windows can be found here
    //https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /****************************************
                    Step 2 Create Context
    *****************************************/
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "MyOpenGLWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //for resize purposes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


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
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
    };

    //vertex buffer objects [VBO] 
    //stores a large number of vertices in the GPU's memory
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

    //vertex array objects [VAO]
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    //element buffer objects [EBO]
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    /*
        1. parameter specifies which vertex attribute we want to configure see shader location = 0
        2. argument specifies the size of the vertex attribute. it is vec3 so it will be 3
        3. specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values)
        4.  specifies if we want the data to be normalized
            If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0
        5.  stride. it is the space between vertex. So 3 times cause eachh vertex has 3 values (x,y,z)
        6. This is the offset of where the position data begins in the buffer.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    /****************************************
                Step 5 Vertex shader
    *****************************************/
    int  success;
    char infoLog[512];

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // second parameter: how many string we pass as sourcecode
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /****************************************
            Step 6 Fragment shader
    *****************************************/

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // second parameter: how many string we pass as sourcecode
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /****************************************
        Step 7 shader programm link shader
    *****************************************/
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //combine both shader to one shaderprogramm
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //link both shader
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // input
        processInput(window);

        // rendering commands here
        //...
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //for one triangle
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();


    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    // clean all glfw resources
    glfwTerminate();
    return 0;


}

