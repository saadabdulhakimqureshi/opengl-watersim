// STARTER CODE FROM learnopengl.com
// Additional code for rendering a tri also from learnopengl.com

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Camera &camera, float deltaTime);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex and Fragment Shader (inline)
// Ideally, these will be in their own glsl files.
const char *vertexShaderSource = R"(
	#version 330 core

	layout (location = 0) in vec3 aPos;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}
)";

const char *fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// -- Building Shaders --
	// build the vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Shared error-checking variables
	int success;
	char infoLog[512];

	// Checking for vertex shader compilation errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// build the fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Checking for fragment shader compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// Shader Program
	// Shaders get linked here
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Checking for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}

	// housekeeping
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// The triangle(s)
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f};

	// Where the magic happens
	// 1. Establish VBO and VAO Put vertices into buffer for OpenGL

	// VBO - Vertex Buffer Object(?)
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// VAO - Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Bind buffers and buffer data
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 2. set vertex attributes
	// Vertex attributes in the vertex buffer
	// glVertexAttribPointer( location of vertex attribute , size of vertex attribute (vec3), type of data, stride, weird typecasting stuff)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Not entirely understanding the importance of these from the learnopengl code
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Camera related code /
	Camera camera;

	const glm::mat4 model{1.0f};

	const GLint modelLocation =
		glGetUniformLocation(shaderProgram, "model");

	const GLint viewLocation =
		glGetUniformLocation(shaderProgram, "view");

	const GLint projectionLocation =
		glGetUniformLocation(shaderProgram, "projection");

	glEnable(GL_DEPTH_TEST);

	double lastFrame = glfwGetTime();
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		const double currentFrame = glfwGetTime();
		const float deltaTime = static_cast<float>(currentFrame - lastFrame);
		lastFrame = currentFrame;

		processInput(window, camera, deltaTime);
		if (glfwWindowShouldClose(window))
			break;

		int framebufferWidth;
		int framebufferHeight;

		glfwGetFramebufferSize(
			window,
			&framebufferWidth,
			&framebufferHeight);

		// for window minimizing // prevents division by zero
		if (framebufferWidth <= 0 || framebufferHeight <= 0)
		{
			glfwWaitEventsTimeout(0.05); // skip iterations when minimized
			continue;
		}

		glViewport(0, 0, framebufferWidth, framebufferHeight); // sets pixel drawing area

		const float aspectRatio =
			static_cast<float>(framebufferWidth) /
			static_cast<float>(framebufferHeight);

		const glm::mat4 view = camera.getViewMatrix();

		// creating perspective projection
		const glm::mat4 projection = glm::perspective(
			glm::radians(45.0f), // fov
			aspectRatio,
			0.1f,
			100.0f);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glUniformMatrix4fv(
			modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(
			viewLocation, 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(
			projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	// more housekeeping
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, Camera &camera, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// look around
	static bool firstMouse = true;
	static double lastX = 0.0;
	static double lastY = 0.0;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
		return;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	const float xoffset = static_cast<float>(xpos - lastX);
	const float yoffset = static_cast<float>(lastY - ypos);

	lastX = xpos;
	lastY = ypos;

	camera.rotate(xoffset, yoffset);

	// walking around
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.move(glm::vec3(0.0f, 0.0f, 1.0f), deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.move(glm::vec3(0.0f, 0.0f, -1.0f), deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.move(glm::vec3(-1.0f, 0.0f, 0.0f), deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.move(glm::vec3(1.0f, 0.0f, 0.0f), deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}