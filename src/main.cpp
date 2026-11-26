// STARTER CODE FROM learnopengl.com
// Additional code for rendering a tri also from learnopengl.com

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "camera.h"
#include "environment.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// for reading in shader files
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string> // string makes it easier to read in

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Camera &camera, float deltaTime, glm::vec3 &cursorPosition);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// for reading Vertex and Fragment Shaders
std::string readShaderFile(const char *path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		throw std::runtime_error(
			std::string("Failure to open shader file: ") + path);
	}

	std::ostringstream contents;
	contents << file.rdbuf();

	return contents.str();
}

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
	// read shader files
	std::string vertexShaderCode;
	std::string fragmentShaderCode;
	try
	{
		vertexShaderCode = readShaderFile("shaders/shader.vert");
		fragmentShaderCode = readShaderFile("shaders/shader.frag");
	}
	catch (const std::exception &error)
	{
		std::cerr << error.what() << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	const char *vertexShaderSource = vertexShaderCode.c_str();
	const char *fragmentShaderSource = fragmentShaderCode.c_str();

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

	// Cursor related code
	glm::vec3 cursorPosition(0.0f, 0.0f, 1.0f);
	const float cursorVertex[] = {0.0f, 0.0f, 0.0f};
	unsigned int cursorVAO, cursorVBO;
	glGenVertexArrays(1, &cursorVAO);
	glGenBuffers(1, &cursorVBO);
	glBindVertexArray(cursorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cursorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cursorVertex), cursorVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const glm::mat4 model{1.0f};

	const GLint modelLocation =
		glGetUniformLocation(shaderProgram, "model");

	const GLint viewLocation =
		glGetUniformLocation(shaderProgram, "view");

	const GLint projectionLocation =
		glGetUniformLocation(shaderProgram, "projection");

	const GLint colorLocation =
		glGetUniformLocation(shaderProgram, "objectColor");

	glEnable(GL_DEPTH_TEST);

	double lastFrame = glfwGetTime();

	// temporary
	Cube *floor = spawnCube(glm::vec3(0.0f, -2.0f, 0.0f));

	if (floor)
	{
		floor->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
		floor->setMass(0.0f);
		floor->setColor(glm::vec3(1.0f));
	}
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		const double currentFrame = glfwGetTime();
		const float deltaTime = static_cast<float>(currentFrame - lastFrame);
		lastFrame = currentFrame;

		processInput(window, camera, deltaTime, cursorPosition);
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

		// draw triangle
		glUniform3f(colorLocation, 1.0f, 0.5f, 0.2f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw cubes
		drawObjects(shaderProgram);
		updateObjects(deltaTime);

		// draw cursor
		const glm::mat4 cursorModel = glm::translate(glm::mat4(1.0f), cursorPosition);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cursorModel));
		glUniform3f(colorLocation, 1.0f, 0.0f, 0.0f);
		glPointSize(10.0f);
		glBindVertexArray(cursorVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		glfwSwapBuffers(window);
	}

	// more housekeeping
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &cursorVAO);
	glDeleteBuffers(1, &cursorVBO);
	cleanupObjects();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, Camera &camera, float deltaTime, glm::vec3 &cursorPosition)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// spawn cube on p
	static bool fWasPressed = false; // static allows the bool is change and not reset on each processInput call
	const bool fIsPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
	if (fIsPressed && !fWasPressed)
	{
		Cube *cube = spawnCube(camera.getPosition() + camera.getForward() * 1.0f);
		applyForceToObject(*cube, camera.getForward(), 100.0f, deltaTime);
	}

	fWasPressed = fIsPressed;
	// camera movement
	// walking around
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.move(glm::vec3(0.0f, 0.0f, 1.0f), deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.move(glm::vec3(0.0f, 0.0f, -1.0f), deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.move(glm::vec3(-1.0f, 0.0f, 0.0f), deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.move(glm::vec3(1.0f, 0.0f, 0.0f), deltaTime);

	// cursor movement
	glm::vec3 cursorMovement(0.0f);
	const float cursorSpeed = 1.0f;

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		cursorMovement.x -= 1.0f;

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		cursorMovement.x += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		cursorMovement.y -= 1.0f;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		cursorMovement.y += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		cursorMovement.z -= 1.0f;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		cursorMovement.z += 1.0f;

	if (glm::length(cursorMovement) > 0.0f)
	{
		cursorPosition += glm::normalize(cursorMovement) * cursorSpeed * deltaTime;
	}

	// spawn cube on p
	static bool pWasPressed = false; // static allows the bool is change and not reset on each processInput call
	const bool pIsPressed = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
	if (pIsPressed && !pWasPressed)
	{
		spawnCube(cursorPosition);
	}
	pWasPressed = pIsPressed;

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}