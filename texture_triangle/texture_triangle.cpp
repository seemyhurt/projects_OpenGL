// GLEW нужно подключать до GLFW
#include <iostream>
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <utils/shader.h>
#include <SOIL.h>

const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat mixValue = 0.2f;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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
	glViewport(0, 0, WIDTH, HEIGHT);

    std::string vertexPath = std::string(PROJECT_NAME) + std::string("/vertex.txt");
    std::string fragmentPath = std::string(PROJECT_NAME) + std::string("/fragment.txt");
    Shader ourShader(vertexPath, fragmentPath);

	//3 вершины в трехмерном пространстве
	GLfloat vertices[] = {
		// Позиции          // Цвета             // Текстурные координаты
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // Верхний правый
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // Нижний правый
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // Верхний левый
	};
	GLuint indices[] = {
		0, 1, 3, // Первый треугольник
		1, 2, 3  //Второй треугольник
	};

	GLuint VBO, VAO, EBO;
	//объект вершинного массива
	glGenVertexArrays(1, &VAO);
	//объект вершинного буфера
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//привязываем VAO
	glBindVertexArray(VAO);
	//привязать GL_ARRAY_BUFFER к буферу
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//копирование вершинных данных в  буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//Интерпретация вершинных данных
	
	// Атрибут с координатами
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
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
    unsigned char* image1= SOIL_load_image(containerPath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
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
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//активация текстурного блока
		glActiveTexture(GL_TEXTURE0);
        ourShader.use();

		//привязать текстуру  и она автоматически будет передана сэмплеру фрагментного шейдера
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		//привязка обеих текстур к соответствующим текстурным блокам и указанием к какому сэмплеру относится какой текстурный блок :
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
		
		glUniform1f(glGetUniformLocation(ourShader.Program, "mixValue"), mixValue);

		glBindVertexArray(VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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
}
