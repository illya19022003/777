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

// Функция MAIN, отсюда мы запускаем приложение и запускаем игровой цикл
int main()
{
    // Инициируем GLFW
    glfwInit();
    // Устанавливаем все необходимые параметры для GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Создаем объект GLFWwindow, который мы можем использовать для функций GLFW.
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Установливаем необходимые функции обратного вызова
    glfwSetKeyCallback(window, key_callback);

    // Установливаем значение true, чтобы GLEW знал, как использовать современный подход к получению указателей на функции и расширений.
    glewExperimental = GL_TRUE;
    // Инициализация GLEW для настройки указателей функций OpenGL
    glewInit();

    // Инициализация GLEW для настройки указателей функций OpenGL
    glViewport(0, 0, WIDTH, HEIGHT);


    // Создаем и скомпилируем нашу шейдерную программу
    // Вершинный шейдер
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Проверяем наличие ошибок времени компиляции
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Фрагментный шейдер
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Проверьте наличие ошибок времени компиляции
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Связивание шейдеров
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Проверяем наличие ошибок при связывании
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Настроиваем данные вершин (и буфер (ы)) и указатели атрибутов
    GLfloat vertices[] = {
        // Позиции        
         0.5f, -0.5f, 0.0f,  // Внизу справа
        -0.5f, -0.5f, 0.0f,  // Нижняя левая
         0.0f,  0.5f, 0.0f   // Верх 
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Сначала связиваем объект массива вершин, затем связиваем и установливаем буфер (ы) вершин и указатели атрибутов.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Атрибут позиции
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Отвязать VAO


    // Игровой цикл
    while (!glfwWindowShouldClose(window))
    {
        // Проверяем, не были ли активированы какие-либо события (нажатие клавиши, перемещение мыши и т. Д.), И вызиваем соответствующие функции ответа.
        glfwPollEvents();

        // Рендер
        // Очистка цветного буфера
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Активируем шейдер
        glUseProgram(shaderProgram);

        // Обновляем равномерно цвет
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        // Рисуем треугольник
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Меняем местами экранные буферы
        glfwSwapBuffers(window);
    }
    // Правильно перераспределяеме все ресурсы, когда они достигли своей цели.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Завершение GLFW, очистив все ресурсы, выделенные GLFW.
    glfwTerminate();
    return 0;
}

// Вызывается при нажатии / отпускании клавиши через GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}