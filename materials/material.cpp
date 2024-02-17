// GLEW нужно подключать до GLFW
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utils/shader.h>
#include "utils/camera.h"

#include <SOIL.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const GLuint WIDTH = 1200, HEIGHT = 800;
GLdouble lastX, lastY;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
bool firstMouse = true;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(window, &lastX, &lastY);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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
    std::string lightVertexPath = std::string(PROJECT_NAME) + std::string("/lightVertex.txt");
    std::string lightFragmentPath = std::string(PROJECT_NAME) + std::string("/lightFragment.txt");

    Shader lightShader(vertexPath, fragmentPath);
    Shader lampShader(lightVertexPath, lightFragmentPath);

	float vertices[] = {
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
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};


	GLuint VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
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
		glfwPollEvents();
		do_movement();

		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightShader.use();
		glm::vec3 lightColor {0.1f, 1.3f, 1.0f};
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		lightShader.setVec3("light.ambient", ambientColor);
		lightShader.setVec3("light.diffuse", diffuseColor);
		lightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		lightShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		lightShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		lightShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightShader.setFloat("material.shininess", 32.0f);
        lightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);


		lightShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
		lightShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

		glm::mat4 view{ 1.0f };
		view = camera.GetViewMatrix();
		lightShader.setMat4("view", view);
		glm::mat4 model{ 1.0 };
		lightShader.setMat4("model", model);
        glm::mat4 projection{ 1.0f };

        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		lightShader.setMat4("projection", projection);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

        lampShader.use();

        // Set matrices
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		model = glm::mat4{ 1.0f };
		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
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
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}
void do_movement()
{ 
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
    GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
