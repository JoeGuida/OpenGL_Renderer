#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <camera.hpp>
#include <shader.hpp>
#include <texture.hpp>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);
void processInput(GLFWwindow* window);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

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
const float NEAR_DISTANCE = 0.1f; 
const float FAR_DISTANCE = 100.0f;
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

// lighting
glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

int main() {

	// initialize GLFW, OpenGL, and GLAD
	// -----------------------------------------------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// create GLFW window and set callbacks
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Renderer", NULL, NULL);
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
	glEnable(GL_DEPTH_TEST);

	// Load shaders
	// -----------------------------------------------------------------------------------------------
	Shader phongShader(getLocalFilepath("/shaders/phong.vert").c_str(), getLocalFilepath("/shaders/phong.frag").c_str());
	Shader lightCubeShader(getLocalFilepath("/shaders/lightCube.vert").c_str(), getLocalFilepath("/shaders/lightCube.frag").c_str());

	// Load textures
	// -----------------------------------------------------------------------------------------------
	Texture texture0(getLocalFilepath("/textures/container2.png").c_str(), TEXTURE_2D, RGBA, REPEAT, LINEAR_MIPMAP_LINEAR, LINEAR, true);
	Texture texture1(getLocalFilepath("/textures/container2_specular.png").c_str(), TEXTURE_2D, RGBA, REPEAT, LINEAR_MIPMAP_LINEAR, LINEAR, true);

	// Setup vertex data and buffers and configure vertex attributes
	// -----------------------------------------------------------------------------------------------
	float cubeVertices[] = {
		// position           // normal           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};

	// Create shaded cube object
	// -----------------------------------------------------------------------------------------------
	unsigned int shadedCubeVAO, shadedCubeVBO;
	glGenVertexArrays(1, &shadedCubeVAO);
	glGenBuffers(1, &shadedCubeVBO);
	glBindVertexArray(shadedCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, shadedCubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// position attribute (vec3)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0);

	// normal attribute (vec3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinates attribute (vec2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Create light cube object
	// -----------------------------------------------------------------------------------------------
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, shadedCubeVBO);

	// position attribute (vec3)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set texture units
	glBindTexture(GL_TEXTURE0, texture0.ID);
	glBindTexture(GL_TEXTURE1, texture1.ID);
	
	// Render Loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// Update time
	    // -----------------------------------------------------------------------------------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		// -----------------------------------------------------------------------------------------------
		processInput(window);

		// Clear color and depth buffer
		// -----------------------------------------------------------------------------------------------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set values for phong shader
		// -----------------------------------------------------------------------------------------------
		phongShader.use();
		phongShader.setVec3("viewPos", camera.Position);

		glm::vec3 lightColor(1.0f, 0.0f, 0.0f);
		glm::vec3 ambientIntensity = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::vec3 diffuseIntensity = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 specularIntensity = glm::vec3(1.0f, 1.0f, 1.0f);

		lightPosition = glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), -1.0);
		phongShader.setVec3("light.color", lightColor);
		phongShader.setVec3("light.position", lightPosition);
		phongShader.setVec3("light.ambient", ambientIntensity);
		phongShader.setVec3("light.diffuse", diffuseIntensity);
		phongShader.setVec3("light.specular", specularIntensity);

		phongShader.setVec3("material.ambient", 0.0f, 0.5f, 0.0f);
		phongShader.setVec3("material.diffuse", 0.0f, 0.5f, 0.0f);
		phongShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		phongShader.setFloat("material.shininess", 32.0f);

		// Draw shaded cube 
		// -----------------------------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), ASPECT_RATIO, NEAR_DISTANCE, FAR_DISTANCE);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		phongShader.setMatrix4f("projection", projection);
		phongShader.setMatrix4f("view", view);
		phongShader.setMatrix4f("model", model);

		glBindVertexArray(shadedCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw light cube
		// -----------------------------------------------------------------------------------------------
		lightCubeShader.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f));

		lightCubeShader.setMatrix4f("projection", projection);
		lightCubeShader.setMatrix4f("view", view);
		lightCubeShader.setMatrix4f("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Swap buffers and poll input events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Delete VAOs, VBOs, and shaders
	// -----------------------------------------------------------------------------------------------
	glDeleteVertexArrays(1, &shadedCubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &shadedCubeVBO);
	glDeleteProgram(phongShader.ID);
	glDeleteProgram(lightCubeShader.ID);
	
	glfwTerminate();
	return EXIT_SUCCESS;
}

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

// whenever mouse scroll wheel is moved this function is called
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessMouseScroll(static_cast<float>(yOffset));
}