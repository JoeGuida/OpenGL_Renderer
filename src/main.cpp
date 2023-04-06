#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <Windows.h>

#include <camera.hpp>
#include <shader.hpp>

// Helper methods to pass local filepaths for shaders/textures
std::string getCurrentDirectoryOnWindows()
{
	const unsigned long maxDir = 260;
	char currentDir[maxDir];
	GetCurrentDirectory(maxDir, currentDir);
	return std::string(currentDir);
}

std::string getLocalFilepath(std::string value) {
	return (getCurrentDirectoryOnWindows() + value);
}

// screen
const float SCREEN_WIDTH = 800;
const float SCREEN_HEIGHT = 600;

// time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// mouse
bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

// camera
const float ASPECT_RATIO = SCREEN_WIDTH / SCREEN_HEIGHT; 
const float NEAR_DISTANCE = 1.0f; 
const float FAR_DISTANCE = 100.0f;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

// whenever the window is resized by OS or the user this function is called
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// whenever mouse moves this function is called
void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn) {
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// whenever mouse scroll wheel is moved this function is called
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

// queries GLFW for input events
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main() {

	// initialize GLFW with OpenGL version and profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// create GLFW window and set callbacks
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// hide mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// create shaders
	// ----------------
	Shader colorShader(getLocalFilepath("/shaders/color.vert").c_str(), getLocalFilepath("/shaders/color.frag").c_str());
	Shader lightCubeShader(getLocalFilepath("/shaders/lightCube.vert").c_str(), getLocalFilepath("/shaders/lightCube.frag").c_str());

	// Setup vertex data and buffers and configure vertex attributes
	// -----------------------------------------------------------------------------------------------

	// cube
	float vertices[] = {
		// position (x,y,z)    
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f, 

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	// Create container object
	// -----------------------------------------------------------------------------------------------
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0);

	// Create light object
	// -----------------------------------------------------------------------------------------------
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Load and create textures
	// -----------------------------------------------------------------------------------------------
	unsigned int textures[2];
	glGenTextures(2, textures);

	// texture 1
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	// Set texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image, create texture, and generate mipmaps
	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(getLocalFilepath("/textures/box.png").c_str(), &width, &height, &numChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 2
	// ---------
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	// Set texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image, create texture, and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(getLocalFilepath("/textures/gem.png").c_str(), &width, &height, &numChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Set texture units
	glUseProgram(colorShader.ID);
	glUniform1i(glGetUniformLocation(colorShader.ID, "texture0"), 0);
	glUniform1i(glGetUniformLocation(colorShader.ID, "texture1"), 1);

	// Render Loop
	while (!glfwWindowShouldClose(window)) {
		// Update time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// clear with color, clear depth_buffer_bit for depth testing
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set uniforms for color shader
		glUseProgram(colorShader.ID);
		glUniform3f(glGetUniformLocation(colorShader.ID, "objectColor"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(colorShader.ID, "lightColor"), 1.0f, 1.0f, 1.0f);

		glm::mat4 projection = glm::perspective(camera.Zoom, ASPECT_RATIO, NEAR_DISTANCE, FAR_DISTANCE);
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(colorShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(colorShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw cube
		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(colorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw light cube
		glUseProgram(lightCubeShader.ID);
		glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// swap buffers and poll input events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// delete VAOs, VBOs, and shaders
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(colorShader.ID);
	glDeleteProgram(lightCubeShader.ID);
	
	// terminate GLFWB
	glfwTerminate();

	return EXIT_SUCCESS;
}