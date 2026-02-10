#include <glad.h>
#include <glfw3.h>
#include <iostream>

const char* WINDOW_NAME = "Fariha Chowdhury";
const int CLOSE_KEY = GLFW_KEY_F;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, CLOSE_KEY) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, WINDOW_NAME, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Orange background
        glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //cyan obtuse triangles
        glColor3f(0.0f, 1.0f, 1.0f);

        glBegin(GL_TRIANGLES);

        // Bottom-left obtuse triangle
        glVertex2f(-0.9f, -0.9f);
        glVertex2f(-0.2f, -0.9f);
        glVertex2f(-0.9f, -0.3f);

        // Top-right obtuse triangle
        glVertex2f(0.9f, 0.9f);
        glVertex2f(0.2f, 0.9f);
        glVertex2f(0.9f, 0.3f);

        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
