
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <tool/shader.h>
#include <tool/camera.h>
#include <tool/MyObj.hpp>
#include <tool/MyBezier.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);

std::string Shader::dirName;
int SCREEN_WIDTH = 1600;
int SCREEN_HEIGHT = 1200;

// delta time
float deltaTime = 0.0f;
float lastTime = 0.0f;

float lastX = SCREEN_WIDTH / 2.0f; // 鼠标上一帧的位置
float lastY = SCREEN_HEIGHT / 2.0f;

Camera camera(glm::vec3(5.0, 5.0, 5.0));

using namespace std;

int main(int argc, char *argv[])
{
	Shader::dirName = argv[1];
	glfwInit();
	// 设置主要和次要版本
	const char *glsl_version = "#version 330";

	// 片段着色器将作用域每一个采样点（采用4倍抗锯齿，则每个像素有4个片段（四个采样点））
	// glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 窗口
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dream_Car", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置视口
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	// 鼠标键盘
	// 1.注册窗口变化监听
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// 2.鼠标事件
	glfwSetCursorPosCallback(window, mouse_callback);

	Shader shader("./shader/vertex.glsl", "./shader/fragment.glsl");
	MyObj car("./src/dream_car.obj");
	MyBezier bezier(28);
	MyBezier bezier2(28);
	bezier.Init();
	bezier2.Init();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastTime;
		lastTime = currentFrame;

		// 渲染指令
		glClearColor(1, 1, 1, 1); // 纯白色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);
		float fov = 45.0f; // 视锥体的角度
		projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		car.Draw(shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.8f, 2.3f, -2.7f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		bezier.Draw(shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 2.3f, -2.7f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		bezier2.Draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// 释放资源
	glfwTerminate();
	return 0;
}

// 窗口变动监听
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 键盘输入监听
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// 相机按键控制
	// 相机移动
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// 鼠标移动监听
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
