// GLEW нужно подключать до GLFW
// GLEW
#include <filesystem>
#include <iostream>
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <utils/shader.h>

#ifndef PROJECT_NAME
#define PROJECT_NAME "Unknown"
#endif

const GLuint WIDTH = 800, HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

    std::string vertexPath = std::string(PROJECT_NAME) + std::string("/vertex.txt");
    std::string fragmentPath = std::string(PROJECT_NAME) + std::string("/fragment.txt");

    Shader ourShader(vertexPath, fragmentPath);

	//3 вершины в трехмерном пространстве
	GLfloat vertices[] = {
		// Позиции        
		 0.5f, -0.5f, 0.0f,   // Нижний правый угол
		-0.5f, -0.5f, 0.0f,    // Нижний левый угол
		 0.0f,  0.5f, 0.0f,    // Верхний угол
	};

	GLuint VBO, VAO;
	//объект вершинного массива
	glGenVertexArrays(1, &VAO);
	//объект вершинного буфера
	glGenBuffers(1, &VBO);
	//привязываем VAO
	glBindVertexArray(VAO);
	//привязать GL_ARRAY_BUFFER к буферу
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//копирование вершинных данных в  буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Интерпретация вершинных данных
	// Атрибут с координатами
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // Обрабатываем события
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Очищаем буфер цвета
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();

		GLfloat timeValue = glfwGetTime(); //получаем время работы в секундах
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5; //изменяем значение от 0.0 до 1.0
        GLint vertexColorLocation = glGetUniformLocation(ourShader.Program, "ourColor"); //запрашиваем индекс формы ourColor
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); //установка значения формы ourColor

		glBindVertexArray(VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
