#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{


    glfwInit();
    //options for glfw windows can be found here
    //https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "MyOpenGLWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad manage function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
