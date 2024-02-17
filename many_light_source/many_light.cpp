//// GLEW нужно подключать до GLFW
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL.h>

#include <utils/shader.h>
#include "stb_image_aug.h"
#include "utils/camera.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const GLuint WIDTH = 1200, HEIGHT = 800;
GLdouble lastX, lastY;		//Начальное положение камеры
GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];					//Массив переменных логического типа, представляющий нажатые или отпущенные состояния клавиш
bool firstMouse = true;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
	glfwGetCursorPos(window, &lastX, &lastY); //инициализация начального положения курсора
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//разрешить проверку глубины
	glEnable(GL_DEPTH_TEST);

	//glewExperimental = GL_TRUE;
	glewInit();
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);

    std::string vertexPath = std::string(PROJECT_NAME) + std::string("/vertex.txt");
    std::string fragmentPath = std::string(PROJECT_NAME) + std::string("/fragment.txt");
    std::string lightVertexPath = std::string(PROJECT_NAME) + std::string("/lightVertex.txt");
    std::string lightFragmentPath = std::string(PROJECT_NAME) + std::string("/lightFragment.txt");

    Shader lightShader(vertexPath, fragmentPath);
    Shader lampShader(lightVertexPath, lightFragmentPath);
	//вершины в трехмерном пространстве
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
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
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	glm::vec3 pointLightColors[] = {
		glm::vec3(0.4f, 0.7f, 0.1f),
		glm::vec3(0.4f, 0.7f, 0.1f),
		glm::vec3(0.4f, 0.7f, 0.1f),
		glm::vec3(0.4f, 0.7f, 0.1f)
	};

	GLuint VBO, cubeVAO;
	//объект вершинного массива
	glGenVertexArrays(1, &cubeVAO);
	//объект вершинного буфера
	glGenBuffers(1, &VBO);
	//привязать GL_ARRAY_BUFFER к буферу
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//копирование вершинных данных в  буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//привязываем VAO
	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//создание идентификатора текстуры
	GLuint texture1;
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
    std::string containerPath = std::string("images/container.png");
    unsigned char* image1 = stbi_load(containerPath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	//освобождение участка памяти, выделенного под загруженное изображение, и отвязка объекта текстуры
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texture2;
	glGenTextures(1, &texture2);
	//привязка текстуры
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Устанавливаем настройки фильтрации и преобразований
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//сгенерировать данные текстуры используя предварительно загруженное изображение
    std::string containerSpecPath = std::string("images/container_specular.png");
    unsigned char* image2 = stbi_load(containerSpecPath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
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
		do_movement();

		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		//очистить буфер глубины таким же образом как и буфера цвета
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lightShader.use();
		lightShader.setVec3("viewPos", camera.Position);
		lightShader.setFloat("material.shininess", 32.0f);

		lightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightShader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
		// point light 1
		lightShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
		lightShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightShader.setFloat("pointLights[0].constant", 1.0f);
		lightShader.setFloat("pointLights[0].linear", 0.07f);
		lightShader.setFloat("pointLights[0].quadratic", 0.017);
		// point light 2
		lightShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
		lightShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightShader.setFloat("pointLights[1].constant", 1.0f);
		lightShader.setFloat("pointLights[1].linear", 0.07f);
		lightShader.setFloat("pointLights[1].quadratic", 0.017);
		// point light 3
		lightShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
		lightShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		lightShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		lightShader.setFloat("pointLights[2].constant", 1.0f);
		lightShader.setFloat("pointLights[2].linear", 0.07f);
		lightShader.setFloat("pointLights[2].quadratic", 0.017);
		// point light 4
		lightShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
		lightShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		lightShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		lightShader.setFloat("pointLights[3].constant", 1.0f);
		lightShader.setFloat("pointLights[3].linear", 0.07f);
		lightShader.setFloat("pointLights[3].quadratic", 0.017);
		// flashLight
		lightShader.setVec3("flashLight.position", camera.Position);
		lightShader.setVec3("flashLight.direction", camera.Front);
		lightShader.setVec3("flashLight.ambient", 0.0f, 0.0f, 0.0f);
		lightShader.setVec3("flashLight.diffuse", 0.0f, 1.0f, 0.0f);
		lightShader.setVec3("flashLight.specular", 0.0f, 1.0f, 0.0f);
		lightShader.setFloat("flashLight.constant", 1.0f);
		lightShader.setFloat("flashLight.linear", 0.07f);
		lightShader.setFloat("flashLight.quadratic", 0.017);
		lightShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(7.0f)));
		lightShader.setFloat("flashLight.outerCutOff", glm::cos(glm::radians(10.0f)));
		//матрица камеры
		glm::mat4 view{ 1.0f };
		view = camera.GetViewMatrix();
		lightShader.setMat4("view", view);
		glm::mat4 model{ 1.0 };
		lightShader.setMat4("model", model);
		//использовать перспективную проекцию
		glm::mat4 projection{ 1.0f };
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		lightShader.setMat4("projection", projection);
		//отправить матрицы в шейдер
		//отрисовка куба
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model{ 1.0 };
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Рисуем объект освещения
		lampShader.use();
		// Set matrices
		for (unsigned int i = 0; i < 4; i++)
		{
			lampShader.setVec3("lightColor", pointLightColors[i]);
			lampShader.setMat4("view", view);
			lampShader.setMat4("projection", projection);
			model = glm::mat4{ 1.0f };
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setMat4("model", model);
			// Draw the light object (using light's vertex attributes)
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
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
