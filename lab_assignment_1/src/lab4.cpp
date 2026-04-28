#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <cmath>

const char* WINDOW_TITLE = "0432220005101042";
const int CLOSE_KEY = GLFW_KEY_F; 

// window size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


bool isWhite = false;
bool isRedPermanent = false;

// process input
void processInput(GLFWwindow* window)
{
    // Close window
    if (glfwGetKey(window, CLOSE_KEY) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Hold W converts to white
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        isWhite = true;
    else
        isWhite = false;

    // Press R converts to permanent red
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        isRedPermanent = true;
}

// vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
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
        std::cout << "Failed to create GLFW window\n";
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

    // triangle vertices
    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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

        float r, g, b;

        if (isRedPermanent)
        {
            r = 1.0f; g = 0.0f; b = 0.0f;
        }
        else if (isWhite)
        {
           
            r = g = b = 1.0f;
        }
        else
        {
            // animation cyan to magenta
            float timeValue = glfwGetTime();
            float t = (sin(timeValue) + 1.0f) / 2.0f;
            r = t;
            g = 1.0f - t;
            b = 1.0f;
        }

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform3f(vertexColorLocation, r, g, b);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}