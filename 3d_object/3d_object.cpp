
#include <iostream>
#include <SOIL.h>					//Simple OpenGL Image Library
#include <utils/shader.h>			//Заголовочный файл шейдерной программы
#include <utils/camera.h>			//Заголовочный файл камеры
#include "gtc/type_ptr.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);		//Управление клавиатурой
void do_movement();																		//Перемещение камеры
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);				//Управление колёсиком мыши
void mouse_callback(GLFWwindow* window, double xpos, double ypos);						//Управление мышью

const GLuint WIDTH = 1200, HEIGHT = 800;    //Размер окна
GLdouble lastX, lastY;                      //Начальное положение камеры
GLfloat deltaTime = 0.0f;                   // Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;                   // Время вывода последнего кадра
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];                            //Массив переменных логического типа, представляющий нажатые или отпущенные состояния клавиш
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
	//Создание объекта окна и проверка на ошибки
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LAB_1_3D_OBJ", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Функции обратного вызова
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Захват курсора
	glfwSetKeyCallback(window, key_callback);
	glfwGetCursorPos(window, &lastX, &lastY);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//Инициализация GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	//Размер отрисовываемого окна
	glViewport(0, 0, WIDTH, HEIGHT);
	//Создание шейдеров
    std::string vertexPath = std::string(PROJECT_NAME) + std::string("/vertex.cpp");
    std::string fragmentPath = std::string(PROJECT_NAME) + std::string("/fragment.cpp");
    Shader ourShader(vertexPath, fragmentPath);
	//Разрешить проверку глубины
	glEnable(GL_DEPTH_TEST);
	//Вершины в трехмерном пространстве и текстурные координаты
	float vertices[] = {
		//вершиные			  //текстурные
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

	GLuint VBO, VAO;
	//Объекты вершинного массива
	glGenVertexArrays(1, &VAO);
	//Объекты вершинного буфера
	glGenBuffers(1, &VBO);
	//Привязываем VAO
	glBindVertexArray(VAO);
	//Привязать GL_ARRAY_BUFFER к буферу
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//копирование вершинных данных в  буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Атрибут с координатами
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Атрибут текстуры
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//Создание идентификатора текстуры
	GLuint texture1;
	glGenTextures(1, &texture1);
	//Привязка текстуры
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Устанавливаем настройки фильтрации и преобразований
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	//Сгенерировать данные текстуры используя предварительно загруженное изображение
    std::string containerPath = std::string("images/texture.jpg");
    unsigned char* image = SOIL_load_image(containerPath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//Освобождение участка памяти, выделенного под загруженное изображение, и отвязка объекта текстуры
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Активация текстуры и запуск шейдера
		glActiveTexture(GL_TEXTURE0);
        ourShader.use();

		// Проверяем события и вызываем функции обратного вызова.
		glfwPollEvents();
		do_movement();

		//Цвет фона
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//Очистить буфер глубины таким же образом как и буфера цвета
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Привязать текстуру  и она автоматически будет передана сэмплеру фрагментного шейдера
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);
		glActiveTexture(GL_TEXTURE1);

		//Матрица модели
		glm::mat4 model{ 1.0f };
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		
		//Матрица вида
		glm::mat4 view{ 1.0f };
		view = camera.GetViewMatrix();

		//Использовать перспективную проекцию
		glm::mat4 projection{ 1.0f };
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		
		//отправить матрицы в шейдер
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		GLint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate(); //Очистить выделенные ресурсы
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	//Управление перемещением камеры
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
	//Управление вращением камеры с помощью мыши
	glfwGetCursorPos(window, &xpos, &ypos);

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//Параметр yoffset представляет собой расстояние, прокрученное по вертикали
	camera.ProcessMouseScroll(yoffset);
}
