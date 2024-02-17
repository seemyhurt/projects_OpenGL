#include <iostream>

#include <utils/shader.h>	//������������ ���� ��������� ���������
#include <utils/camera.h>	//������������ ���� ������


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);	//���������� �����������
void rotation_sketch();																//�������� ��������
void do_movement();																	//����������� ������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);			//���������� �������� ����
void mouse_callback(GLFWwindow* window, double xpos, double ypos);					//���������� �����

const GLuint WIDTH = 1200, HEIGHT = 800;    //������ ����
GLdouble lastX, lastY;                      //��������� ��������� ������
GLfloat deltaTime = 0.0f;                   // �����, ��������� ����� ��������� � ������� ������
GLfloat lastFrame = 0.0f;                   // ����� ������ ���������� �����
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];                            //������ ���������� ����������� ����, �������������� ������� ��� ���������� ��������� ������
bool firstMouse = true;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);       //��������� ������ ���������
float angle_x = 0.0f;                       //���� �������� ������� �� ��� x
float angle_y = 0.0f;						//���� �������� ������� �� ��� y

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	//������������� GLFW
	glfwInit();
	//��������� GLFW
	//�������� ����������� ��������� ������ OpenGL. 
	//�������� 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//��������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//��������� �������� ��� �������� ��������� ��������
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//���������� ����������� ��������� ������� ����
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//�������� ������� ���� � �������� �� ������
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LAB_2_LIGHT", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//������� ��������� ������
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //������ �������
	glfwGetCursorPos(window, &lastX, &lastY); //������������� ���������� ��������� �������
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//��������� �������� �������
	glEnable(GL_DEPTH_TEST);

	//������������� GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	//������ ��������������� ����
	glViewport(0, 0, WIDTH, HEIGHT);
	//�������� ��������� �������� ��������� �������� � �������� ���������� �����
    std::string vertexPath = std::string(PROJECT_NAME) + std::string("/vertex.cpp");
    std::string fragmentPath = std::string(PROJECT_NAME) + std::string("/fragment.cpp");
    std::string lightVertexPath = std::string(PROJECT_NAME) + std::string("/lampVertex.cpp");
    std::string lightFragmentPath = std::string(PROJECT_NAME) + std::string("/lampFragment.cpp");

    Shader lightShader(vertexPath, fragmentPath);
    Shader lampShader(lightVertexPath, lightFragmentPath);
	//������� � ���������� ������������
	float vertices[] = {
		// ����������       // �������          
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
	//�������� ����������
	float materials[][10] = {
		//����  ��� ������� ����������	//���� ��� ���������� ����������	//���� �����											//������ �����
		{0.24725f,	0.1995f,	0.0745f,			0.75164f,	0.60648f,	0.22648f,					0.628281f,	0.555802f,	0.366065f,		0.4f}, //������
		{0.0215f,	 0.1745f,	0.0215f,				0.07568f,	0.61424f,	0.07568f,					0.633f,	0.727811f,	0.633f,						0.6f}, //�������
		{0.05375f,	0.05f,	0.06625f,				0.18275f,	0.17f,	0.22525f,						0.332741f,	0.328634f,	0.346435f,		0.3f}, //��������
		{0.19225f,	0.19225f,	0.19225f,			0.50754f, 0.50754f,	0.50754f,					0.508273f,	0.508273f,	0.508273f,		0.4f}, //�������
		{0.25f,	0.20725f,	0.20725f,					1.0f,	0.829f,	0.829f,									0.296648f, 0.296648f,	0.296648f,			0.09f} //������
	};
	//������� �������� � ������������
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//������� �������� ���������� ����� � ������������
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(4.6f, 0.6f, -4.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	//���� �������� ���������� �����
	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
	};

	GLuint VBO, cubeVAO;
	//������ ���������� �������
	glGenVertexArrays(1, &cubeVAO);
	//������ ���������� ������
	glGenBuffers(1, &VBO);
	//��������� GL_ARRAY_BUFFER � ������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����������� ��������� ������ �  �����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//����������� VAO
	glBindVertexArray(cubeVAO);
	//������� � ������������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//������� ��������
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
		// ��������� ������� � �������� ������� ��������� ������.
		glfwPollEvents();
		do_movement();
		rotation_sketch();
		//���������� ���� ����
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//�������� ����� ������� ����� �� ������� ��� � ������ �����
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//������ �������
        lightShader.use();
		lightShader.setVec3("viewPos", camera.Position);

		//�������� ����
		lightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightShader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
		// �������� �������� ����� 1
		lightShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
		lightShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightShader.setFloat("pointLights[0].constant", 1.0f);
		lightShader.setFloat("pointLights[0].linear", 0.07f);
		lightShader.setFloat("pointLights[0].quadratic", 0.017);
		// �������� �������� ����� 2
		lightShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
		lightShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightShader.setFloat("pointLights[1].constant", 1.0f);
		lightShader.setFloat("pointLights[1].linear", 0.07f);
		lightShader.setFloat("pointLights[1].quadratic", 0.017);
		// �������� �������� ����� 3
		lightShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
		lightShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		lightShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		lightShader.setFloat("pointLights[2].constant", 1.0f);
		lightShader.setFloat("pointLights[2].linear", 0.07f);
		lightShader.setFloat("pointLights[2].quadratic", 0.017);
		//������� ������
		glm::mat4 view{ 1.0f };
		view = camera.GetViewMatrix();
		lightShader.setMat4("view", view);
		glm::mat4 model{ 1.0 };
		lightShader.setMat4("model", model);
		//������������ ������������� ��������
		glm::mat4 projection{ 1.0f };
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		lightShader.setMat4("projection", projection);
		glBindVertexArray(cubeVAO);
		//��������� ��������� ������� ���������� �� �������
		for (unsigned int i = 0; i < 5; i++)
		{
			lightShader.setVec3("material.ambient", materials[i][0], materials[i][1], materials[i][2]);
			lightShader.setVec3("material.diffuse", materials[i][3], materials[i][4], materials[i][5]);
			lightShader.setVec3("material.specular", materials[i][6], materials[i][7], materials[i][8]);
			lightShader.setFloat("material.shininess", materials[i][9] * 128);
			glm::mat4 model{ 1.0 };
			//������ ��������
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f*i), glm::vec3(5.0f, 0.2f, 0.3f));
			model = glm::rotate(model, glm::radians(angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
			lightShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// ������ ������� ���������
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
	//�������� ���������� �������
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// ����� ������������ �������� ESC, �� ������������� �������� WindowShouldClose � true, 
	// � ���������� ����� ����� ���������
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
	//������ ���, ����� �� �������� ���� �� ������ WASD, ��������� ������ ����������� � ������������ � � ������ ������������. 
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
	//�������� �������� ��� ������� �������� �� ���������
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
	//���������� ��������� ������ � ������� ����
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
	//�������� yoffset ������������ ����� ����������, ������������ �� ���������
	camera.ProcessMouseScroll(yoffset);
}
