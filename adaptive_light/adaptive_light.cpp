#include <iostream>

#include <utils/shader.h>	//Заголовочный файл шейдерной программы
#include <utils/camera.h>	//Заголовочный файл камеры


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);	//Управление клавиатурой
void rotation_sketch();																//Вращение объектов
void do_movement();																	//Перемещение камеры
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);			//Управление колёсиком мыши
void mouse_callback(GLFWwindow* window, double xpos, double ypos);					//Управление мышью

const GLuint WIDTH = 1200, HEIGHT = 800;    //Размер окна
GLdouble lastX, lastY;                      //Начальное положение камеры
GLfloat deltaTime = 0.0f;                   // Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;                   // Время вывода последнего кадра
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];                            //Массив переменных логического типа, представляющий нажатые или отпущенные состояния клавиш
bool firstMouse = true;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);       //Положение общего освещения
float angle_x = 0.0f;                       //Угол поворота объекта по оси x
float angle_y = 0.0f;						//Угол поворота объекта по оси y

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LAB_2_LIGHT", nullptr, nullptr);
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
	//Создание шейдерных программ отрисовки объектов и точечных источников света
    std::string vertexPath = std::string(PROJECT_NAME) + std::string("/vertex.cpp");
    std::string fragmentPath = std::string(PROJECT_NAME) + std::string("/fragment.cpp");
    std::string lightVertexPath = std::string(PROJECT_NAME) + std::string("/lampVertex.cpp");
    std::string lightFragmentPath = std::string(PROJECT_NAME) + std::string("/lampFragment.cpp");

    Shader lightShader(vertexPath, fragmentPath);
    Shader lampShader(lightVertexPath, lightFragmentPath);
	//вершины в трехмерном пространстве
	float vertices[] = {
		// координаты       // нормали          
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
	};
	//свойства материалов
	float materials[][10] = {
		//цвет  под фоновым освещением	//цвет под рассеянным освещением	//цвет блика											//радиус блика
		{0.24725f,	0.1995f,	0.0745f,			0.75164f,	0.60648f,	0.22648f,					0.628281f,	0.555802f,	0.366065f,		0.4f}, //золото
		{0.0215f,	 0.1745f,	0.0215f,				0.07568f,	0.61424f,	0.07568f,					0.633f,	0.727811f,	0.633f,						0.6f}, //изумруд
		{0.05375f,	0.05f,	0.06625f,				0.18275f,	0.17f,	0.22525f,						0.332741f,	0.328634f,	0.346435f,		0.3f}, //обсидиан
		{0.19225f,	0.19225f,	0.19225f,			0.50754f, 0.50754f,	0.50754f,					0.508273f,	0.508273f,	0.508273f,		0.4f}, //серебро
		{0.25f,	0.20725f,	0.20725f,					1.0f,	0.829f,	0.829f,									0.296648f, 0.296648f,	0.296648f,			0.09f} //жемчуг
	};
	//позиции объектов в пространстве
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//позиции точечных источников света в пространстве
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(4.6f, 0.6f, -4.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	//цвет точечных источников света
	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
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
	//Атрибут с координатами
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Атрибут нормалей
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Проверяем события и вызываем функции обратного вызова.
		glfwPollEvents();
		do_movement();
		rotation_sketch();
		//Установить цвет фона
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//Очистить буфер глубины таким же образом как и буфера цвета
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//запуск шейдера
        lightShader.use();
		lightShader.setVec3("viewPos", camera.Position);

		//основной свет
		lightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightShader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
		// точечный источник света 1
		lightShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
		lightShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightShader.setFloat("pointLights[0].constant", 1.0f);
		lightShader.setFloat("pointLights[0].linear", 0.07f);
		lightShader.setFloat("pointLights[0].quadratic", 0.017);
		// точечный источник света 2
		lightShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
		lightShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightShader.setFloat("pointLights[1].constant", 1.0f);
		lightShader.setFloat("pointLights[1].linear", 0.07f);
		lightShader.setFloat("pointLights[1].quadratic", 0.017);
		// точечный источник света 3
		lightShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
		lightShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		lightShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		lightShader.setFloat("pointLights[2].constant", 1.0f);
		lightShader.setFloat("pointLights[2].linear", 0.07f);
		lightShader.setFloat("pointLights[2].quadratic", 0.017);
		//матрицы камеры
		glm::mat4 view{ 1.0f };
		view = camera.GetViewMatrix();
		lightShader.setMat4("view", view);
		glm::mat4 model{ 1.0 };
		lightShader.setMat4("model", model);
		//использовать перспективную проекцию
		glm::mat4 projection{ 1.0f };
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		lightShader.setMat4("projection", projection);
		glBindVertexArray(cubeVAO);
		//установка различных свойств материалов на объекты
		for (unsigned int i = 0; i < 5; i++)
		{
			lightShader.setVec3("material.ambient", materials[i][0], materials[i][1], materials[i][2]);
			lightShader.setVec3("material.diffuse", materials[i][3], materials[i][4], materials[i][5]);
			lightShader.setVec3("material.specular", materials[i][6], materials[i][7], materials[i][8]);
			lightShader.setFloat("material.shininess", materials[i][9] * 128);
			glm::mat4 model{ 1.0 };
			//задать вращение
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f*i), glm::vec3(5.0f, 0.2f, 0.3f));
			model = glm::rotate(model, glm::radians(angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
			lightShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Рисуем объекты освещения
        lampShader.use();
		for (unsigned int i = 0; i < 3; i++)
		{
			lampShader.setVec3("lightColor", pointLightColors[i]);
			lampShader.setMat4("view", view);
			lampShader.setMat4("projection", projection);
			model = glm::mat4{ 1.0f };
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.setMat4("model", model);
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	//Очистить выделенные ресурсы
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
void rotation_sketch()
{
	//Вращение объектов при нажатии клавишей со стрелками
	if (keys[GLFW_KEY_UP])
		angle_x -= 0.1f;
	if (keys[GLFW_KEY_DOWN])
		angle_x += 0.1f;
	if (keys[GLFW_KEY_LEFT])
		angle_y -= 0.1f;
	if (keys[GLFW_KEY_RIGHT])
		angle_y+= 0.1f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Управление вращением камеры с помощью мыши
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
	//Параметр yoffset представляет собой расстояние, прокрученное по вертикали
	camera.ProcessMouseScroll(yoffset);
}
