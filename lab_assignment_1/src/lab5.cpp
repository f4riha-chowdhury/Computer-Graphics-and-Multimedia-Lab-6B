#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <cmath>

const char* WINDOW_TITLE = "Animated Rectangle";
const int CLOSE_KEY = GLFW_KEY_F;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// process input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, CLOSE_KEY) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// vertex shader 
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";

// fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Rectangle (2 triangles)
    float vertices[] = {
        // first triangle
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,

        // second triangle
        -0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // yellow background
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float time = glfwGetTime();

        // -color 
        float t = (sin(time) + 1.0f) / 2.0f;
        float r = 1.0f;
        float g = t;
        float b = t;

        int colorLoc = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform3f(colorLoc, r, g, b);

        // transform
        float scale = 0.5f + 0.5f * sin(time); // magnify
        float angle = time; // rotation

        float cosA = cos(angle);
        float sinA = sin(angle);

        // manual transform matrix (scale + rotate)
        float transform[] = {
            scale * cosA,  scale * sinA, 0.0f, 0.0f,
           -scale * sinA,  scale * cosA, 0.0f, 0.0f,
            0.0f,          0.0f,        1.0f, 0.0f,
            0.0f,          0.0f,        0.0f, 1.0f
        };

        int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}