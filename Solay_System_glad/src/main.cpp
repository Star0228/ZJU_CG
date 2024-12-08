#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <geometry/BoxGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/SphereGeometry.h>

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

#include <tool/gui.h>
#include <tool/mesh.h>
#include <tool/model.h>
#include <tool/Torus.hpp>
#include <tool/shader.h>
#include <tool/camera.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const *path);
unsigned int loadCubemap(vector<std::string> faces);

std::string Shader::dirName;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// delta time
float deltaTime = 0.0f;
float lastTime = 0.0f;

float lastX = SCREEN_WIDTH / 2.0f; // 鼠标上一帧的位置
float lastY = SCREEN_HEIGHT / 2.0f;

Camera camera(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 1.0, 0.0));

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

	// 窗口对象
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
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

	// -----------------------
	// 创建imgui上下文
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	// 设置样式
	ImGui::StyleColorsDark();
	// 设置平台和渲染器
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// -----------------------

	// 设置视口
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);

	// 鼠标键盘事件
	// 1.注册窗口变化监听
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// 2.鼠标事件
	glfwSetCursorPosCallback(window, mouse_callback);

	Shader Simple_Shader("./shader/vertex.glsl", "./shader/fragment.glsl");
	Shader Light_Shader("./shader/light_vert.glsl", "./shader/light_frag.glsl");
	Shader Skybox_Shader("./shader/skybox_vert.glsl", "./shader/skybox_frag.glsl");

	SphereGeometry Sun_1(0.2, 50, 50);
	SphereGeometry Sun_2(0.2, 50, 50);
	SphereGeometry Planet_1(0.1f, 50, 50);
	SphereGeometry Satellite_of_p1(0.03f, 50, 50);
	SphereGeometry Planet_2(0.12f, 50, 50);
	SphereGeometry Satellite_of_p2(0.03f, 50, 50);
	SphereGeometry Planet_3(0.28f, 50, 50);
	Torus Sun_torus(0.003, 0.7, 100, 50);
	Torus Planet1_torus(0.003, 1.5, 100, 50);
	Torus Satellite_of_p1_torus(0.003, 0.5, 100, 50);
	Torus Planet2_torus(0.003, 2.5, 100, 50);
	Torus Satellite_of_p2_torus(0.003, 0.5, 100, 50);
	Torus Planet3_torus(0.003, 3.5, 100, 50);

	float skyboxVertices[] = {
		// positions
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f};
	// 加载天空盒
	vector<std::string> faces{
		"./textures/skybox/right.jpg",
		"./textures/skybox/left.jpg",
		"./textures/skybox/top.jpg",
		"./textures/skybox/bottom.jpg",
		"./textures/skybox/front.jpg",
		"./textures/skybox/back.jpg"};

	unsigned int cubemapTexture = loadCubemap(faces);

	unsigned int diffuseMap_Sun = loadTexture("./textures/sun.jpg");
	unsigned int specularMap_Sun = loadTexture("./textures/sun.jpg");
	unsigned int diffuseMap_Earth = loadTexture("./textures/earth.jpg");
	unsigned int specularMap_Earth = loadTexture("./textures/earth_specular.jpg");
	unsigned int diffuseMap_Moon = loadTexture("./textures/moon.jpg");
	unsigned int specularMap_Moon = loadTexture("./textures/moon.jpg");
	unsigned int diffuseMap_Mars = loadTexture("./textures/mars.jpg");
	unsigned int specularMap_Mars = loadTexture("./textures/mars.jpg");

	Simple_Shader.use();
	Simple_Shader.setInt("material.diffuse", 0);
	Simple_Shader.setInt("material.specular", 1);
	// 传递材质属性
	Simple_Shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	Simple_Shader.setFloat("material.shininess", 32.0f);

	float fov = 45.0f; // 视锥体的角度
	glm::vec3 view_translate = glm::vec3(0.0, 0.0, -5.0);
	ImVec4 clear_color = ImVec4(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0); // 25, 25, 25

	// 行星位置
	glm::vec3 _planet_postions[] = {
		glm::vec3(0.7f, 0.0f, 0.0f),
		glm::vec3(-0.7f, 0.0f, 0.0f),
		glm::vec3(1.5f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.0f, 0.0f),
		glm::vec3(-2.5f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.0f, 0.0f),
		glm::vec3(3.5f, 0.0f, 0.0f)};
	// 行星颜色
	glm::vec3 _planet_colors[] = {
		glm::vec3(1.0f, 0.55f, 0.0f),
		glm::vec3(1.0f, 0.55f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.7f, 0.5f, 0.5f)};
	// 轨道颜色
	glm::vec3 _orbit_color = glm::vec3(0.7f, 0.7f, 0.7f);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastTime;
		lastTime = currentFrame;

		glfwSetWindowTitle(window, "LightUp_world");

		// 渲染指令
		// ...
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Simple_Shader.use();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		Simple_Shader.setMat4("view", view);
		Simple_Shader.setMat4("projection", projection);
		Simple_Shader.setVec3("viewPos", camera.Position);

		// 设置点光源属性
		for (unsigned int i = 0; i < 2; i++)
		{
			Simple_Shader.setVec3("pointLights[" + std::to_string(i) + "].position", _planet_postions[i]);
			Simple_Shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.01f, 0.01f, 0.01f);
			// Simple_Shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", _planet_colors[i]);
			Simple_Shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			Simple_Shader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);

			// // 设置衰减
			Simple_Shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
			Simple_Shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
			Simple_Shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
		}
		glm::mat4 model = glm::mat4(1.0f);
		// 绘制轨道
		{
			// Sun
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);

			model = glm::mat4(1.0f);
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _orbit_color);
			Sun_torus.draw();
			// Planet_1
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(135.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _orbit_color);
			Planet1_torus.draw();
			// Satellite_of_p1
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(1.0f * 150.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 1.0f));
			model = glm::translate(model, (glm::vec3(1.5f, 0.0f, 0.0f)));
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _orbit_color);
			Satellite_of_p1_torus.draw();
			// Planet_2
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _orbit_color);
			Planet2_torus.draw();
			// Satellite_of_p2
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(0.5f * 150.0f * (float)glfwGetTime()), glm::vec3(0.0f, -1.0f, 1.0f));
			model = glm::translate(model, (glm::vec3(-2.5f, 0.0f, 0.0f)));
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _orbit_color);
			Satellite_of_p2_torus.draw();
			// Planet_3
			model = glm::mat4(1.0f);
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _orbit_color);
			Planet3_torus.draw();
		}
		// 绘制行星
		{
			// Planet_1
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(1.0f * 150.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 1.0f));
			model = glm::translate(model, _planet_postions[2]);
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _planet_colors[2]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap_Earth);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap_Earth);

			glBindVertexArray(Planet_1.VAO);
			glDrawElements(GL_TRIANGLES, Planet_1.indices.size(), GL_UNSIGNED_INT, 0);
			// Satellite_of_p1
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(1.0f * 150.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 1.0f));
			model = glm::translate(model, (glm::vec3(1.5f, 0.0f, 0.0f)));
			model = glm::rotate(model, glm::radians(0.2f * 100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, _planet_postions[3]);
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _planet_colors[3]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap_Moon);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap_Moon);

			glBindVertexArray(Satellite_of_p1.VAO);
			glDrawElements(GL_TRIANGLES, Satellite_of_p1.indices.size(), GL_UNSIGNED_INT, 0);
			// Planet_2
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(0.5f * 150.0f * (float)glfwGetTime()), glm::vec3(0.0f, -1.0f, 1.0f));
			model = glm::translate(model, _planet_postions[4]);
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _planet_colors[4]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap_Mars);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap_Mars);

			glBindVertexArray(Planet_2.VAO);
			glDrawElements(GL_TRIANGLES, Planet_2.indices.size(), GL_UNSIGNED_INT, 0);
			// Satellite_of_p2
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(0.5f * 150.0f * (float)glfwGetTime()), glm::vec3(0.0f, -1.0f, 1.0f));
			model = glm::translate(model, (glm::vec3(-2.5f, 0.0f, 0.0f)));
			model = glm::rotate(model, glm::radians(0.1f * 100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, _planet_postions[5]);
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _planet_colors[5]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap_Moon);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap_Moon);

			glBindVertexArray(Satellite_of_p2.VAO);
			glDrawElements(GL_TRIANGLES, Satellite_of_p2.indices.size(), GL_UNSIGNED_INT, 0);
			// Planet_3
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(0.05f * 150.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, _planet_postions[6]);
			Simple_Shader.setMat4("model", model);
			Simple_Shader.setVec3("lightColor", _planet_colors[6]);
			// Simple_Shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap_Earth);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap_Earth);

			glBindVertexArray(Planet_3.VAO);
			glDrawElements(GL_TRIANGLES, Planet_3.indices.size(), GL_UNSIGNED_INT, 0);
		}

		// 绘制恒星
		{
			Light_Shader.use();
			Light_Shader.setMat4("view", view);
			Light_Shader.setMat4("projection", projection);
			// //Sun_1
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(0.1f * 150.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, _planet_postions[0]);
			Light_Shader.setMat4("model", model);
			// Light_Shader.setVec3("lightColor", _planet_colors[0]);
			Light_Shader.setInt("diffuse", 0);
			Light_Shader.setInt("specular", 1);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap_Sun);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap_Sun);

			glBindVertexArray(Sun_1.VAO);
			glDrawElements(GL_TRIANGLES, Sun_1.indices.size(), GL_UNSIGNED_INT, 0);
			// Sun_2
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(0.1f * 150.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, _planet_postions[1]);
			Light_Shader.setMat4("model", model);
			Light_Shader.setVec3("lightColor", _planet_colors[1]);
			glBindVertexArray(Sun_2.VAO);
			glDrawElements(GL_TRIANGLES, Sun_2.indices.size(), GL_UNSIGNED_INT, 0);
		}

		// 绘制天空盒
		{
			unsigned int skyboxVAO, skyboxVBO;
			glGenVertexArrays(1, &skyboxVAO);
			glGenBuffers(1, &skyboxVBO);
			glBindVertexArray(skyboxVAO);
			glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
			glBindVertexArray(0);
			glDepthFunc(GL_LEQUAL);
			Skybox_Shader.use();
			// view = camera.GetViewMatrix();
			//  重点代码：取4x4矩阵左上角的3x3矩阵来移除变换矩阵的位移部分，再变回4x4矩阵。///
			//  防止摄像机移动，天空盒会受到视图矩阵的影响而改变位置，即摄像机向z后退，天空盒和cube向z前进
			view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
			Skybox_Shader.setMat4("view", view);
			Skybox_Shader.setMat4("projection", projection);
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture); // 第一个参数从GL_TEXTURE_2D 变为GL_TEXTURE_CUBE_MAP
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Sun_1.dispose();
	Sun_2.dispose();
	Planet_1.dispose();
	Satellite_of_p1.dispose();
	Planet_2.dispose();
	Satellite_of_p2.dispose();
	Planet_3.dispose();
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

// 加载纹理贴图
unsigned int loadTexture(char const *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	// 图像y轴翻转
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path:" << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	;

	return textureID;
}
