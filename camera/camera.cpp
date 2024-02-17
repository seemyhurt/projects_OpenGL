// GLEW нужно подключать до GLFW
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utils/shader.h>
#include "utils/Camera.h"

#include <SOIL.h>
#include "3rd_party/glm/gtc/type_ptr.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const GLuint WIDTH = 1200, HEIGHT = 800;
GLfloat mixValue = 0.2f;
GLdouble lastX, lastY;		//Начальное положение камеры
GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];					//Массив переменных логического типа, представляющий нажатые или отпущенные состояния клавиш
bool firstMouse = true;

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
	
	//Функции обратного вызова
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //захват курсора
	glfwSetKeyCallback(window, key_callback);
	glfwGetCursorPos(window, &lastX, &lastY);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	//разрешить проверку глубины
	glEnable(GL_DEPTH_TEST);

	glewExperimental = GL_TRUE;
	glewInit();
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);

    std::string vertexPath = std::string(PROJECT_NAME) + std::string("/vertex.txt");
    std::string fragmentPath = std::string(PROJECT_NAME) + std::string("/fragment.txt");
    Shader ourShader(vertexPath, fragmentPath);

	//вершины в трехмерном пространстве
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	//позиции для 10 кубов
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
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

	// Атрибут с координатами
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//создание идентификатора текстуры
	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	//привязка текстуры
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Устанавливаем настройки фильтрации и преобразований
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	//сгенерировать данные текстуры используя предварительно загруженное изображение
    std::string containerPath = std::string("images/container.jpg");
    unsigned char* image1 = SOIL_load_image(containerPath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	//освобождение участка памяти, выделенного под загруженное изображение, и отвязка объекта текстуры
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &texture2);
	//привязка текстуры
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Устанавливаем настройки фильтрации и преобразований
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//сгенерировать данные текстуры используя предварительно загруженное изображение
    std::string facePath = std::string("images/awesomeface.png");
    unsigned char* image2 = SOIL_load_image(facePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	//освобождение участка памяти, выделенного под загруженное изображение, и отвязка объекта текстуры
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//очистить буфер глубины таким же образом как и буфера цвета
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//активация текстурного блока
		glActiveTexture(GL_TEXTURE0);
		ourShader.use();
		// Проверка поступивших событий и вызов их обработчиков
		glfwPollEvents();
		do_movement();
		//привязать текстуру  и она автоматически будет передана сэмплеру фрагментного шейдера
		glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		//привязка обеих текстур к соответствующим текстурным блокам и указанием к какому сэмплеру относится какой текстурный блок :
		glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

        glUniform1f(glGetUniformLocation(ourShader.Program, "mixValue"), mixValue);

		//матрица камеры
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//использовать перспективную проекцию
		glm::mat4 projection{ 1.0f };
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		//отправить матрицы в шейдер
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");

        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model{ 1.0 };
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = glm::radians(20.0f) * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDrawArrays(GL_LINE, 0, 12);
		}
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		mixValue += 0.05f;
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		mixValue -= 0.05f;
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}
void do_movement()
{
	// Camera controls
	//Всякий раз, когда мы нажимаем одну из клавиш WASD, положение камеры обновляется в соответствии с её новыми координатами. 
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
