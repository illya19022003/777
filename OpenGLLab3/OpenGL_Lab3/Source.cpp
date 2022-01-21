#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = color;\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"color = ourColor;\n"
"}\n\0";

// ������� MAIN, ������ �� ��������� ���������� � ��������� ������� ����
int main()
{
    // ���������� GLFW
    glfwInit();
    // ������������� ��� ����������� ��������� ��� GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // ������� ������ GLFWwindow, ������� �� ����� ������������ ��� ������� GLFW.
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // ������������� ����������� ������� ��������� ������
    glfwSetKeyCallback(window, key_callback);

    // ������������� �������� true, ����� GLEW ����, ��� ������������ ����������� ������ � ��������� ���������� �� ������� � ����������.
    glewExperimental = GL_TRUE;
    // ������������� GLEW ��� ��������� ���������� ������� OpenGL
    glewInit();

    // ������������� GLEW ��� ��������� ���������� ������� OpenGL
    glViewport(0, 0, WIDTH, HEIGHT);


    // ������� � ������������ ���� ��������� ���������
    // ��������� ������
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // ��������� ������� ������ ������� ����������
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // ����������� ������
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // ��������� ������� ������ ������� ����������
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // ���������� ��������
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // ��������� ������� ������ ��� ����������
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // ����������� ������ ������ (� ����� (�)) � ��������� ���������
    GLfloat vertices[] = {
        // �������        
         0.5f, -0.5f, 0.0f,  // ����� ������
        -0.5f, -0.5f, 0.0f,  // ������ �����
         0.0f,  0.5f, 0.0f   // ���� 
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // ������� ��������� ������ ������� ������, ����� ��������� � ������������� ����� (�) ������ � ��������� ���������.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ������� �������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // �������� VAO


    // ������� ����
    while (!glfwWindowShouldClose(window))
    {
        // ���������, �� ���� �� ������������ �����-���� ������� (������� �������, ����������� ���� � �. �.), � �������� ��������������� ������� ������.
        glfwPollEvents();

        // ������
        // ������� �������� ������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ���������� ������
        glUseProgram(shaderProgram);

        // ��������� ���������� ����
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        // ������ �����������
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // ������ ������� �������� ������
        glfwSwapBuffers(window);
    }
    // ��������� ����������������� ��� �������, ����� ��� �������� ����� ����.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // ���������� GLFW, ������� ��� �������, ���������� GLFW.
    glfwTerminate();
    return 0;
}

// ���������� ��� ������� / ���������� ������� ����� GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}