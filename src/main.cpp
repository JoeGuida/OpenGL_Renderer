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
const float SCREEN_WIDTH = 1920;
const float SCREEN_HEIGHT = 1080;

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
	Texture diffuseMap(getLocalFilepath("/textures/container2.png").c_str(), TEXTURE_2D, RGBA, REPEAT, LINEAR_MIPMAP_LINEAR, LINEAR, true);
	Texture specularMap(getLocalFilepath("/textures/container2_specular.png").c_str(), TEXTURE_2D, RGBA, REPEAT, LINEAR_MIPMAP_LINEAR, LINEAR, true);

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

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap.ID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap.ID);
	
	// Set light positions and colors
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
	};

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
		glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), ASPECT_RATIO, NEAR_DISTANCE, FAR_DISTANCE);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		// shaded cube material
		phongShader.setInt("material.diffuse", 0);
		phongShader.setInt("material.specular", 1);
		phongShader.setFloat("material.shininess", 32.0f);

		// directional light
		phongShader.setVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
		phongShader.setVec3("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
		phongShader.setVec3("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
		phongShader.setVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);

		// point light 1
		phongShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		phongShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		phongShader.setVec3("pointLights[0].diffuse", 1.0f, 0.0f, 0.0f);
		phongShader.setVec3("pointLights[0].specular", 1.0f, 0.0f, 0.0f);
		phongShader.setFloat("pointLights[0].constant", 1.0f);
		phongShader.setFloat("pointLights[0].linear", 0.09f);
		phongShader.setFloat("pointLights[0].quadratic", 0.032f);

		// point light 2
		phongShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		phongShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		phongShader.setVec3("pointLights[1].diffuse", 0.0f, 1.0f, 0.0f);
		phongShader.setVec3("pointLights[1].specular", 0.0f, 1.0f, 0.0f);
		phongShader.setFloat("pointLights[1].constant", 1.0f);
		phongShader.setFloat("pointLights[1].linear", 0.09f);
		phongShader.setFloat("pointLights[1].quadratic", 0.032f);

		// point light 3
		phongShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		phongShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		phongShader.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 1.0f);
		phongShader.setVec3("pointLights[2].specular", 0.0f, 0.0f, 1.0f);
		phongShader.setFloat("pointLights[2].constant", 1.0f);
		phongShader.setFloat("pointLights[2].linear", 0.09f);
		phongShader.setFloat("pointLights[2].quadratic", 0.032f);

		// point light 4
		phongShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		phongShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		phongShader.setVec3("pointLights[3].diffuse", 1.0f, 1.0f, 0.0f);
		phongShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 0.0f);
		phongShader.setFloat("pointLights[3].constant", 1.0f);
		phongShader.setFloat("pointLights[3].linear", 0.09f);
		phongShader.setFloat("pointLights[3].quadratic", 0.032f);

		// spotLight
		phongShader.setVec3("spotLight.position", camera.Position);
		phongShader.setVec3("spotLight.direction", camera.Front);
		phongShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		phongShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		phongShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		phongShader.setFloat("spotLight.constant", 1.0f);
		phongShader.setFloat("spotLight.linear", 0.09f);
		phongShader.setFloat("spotLight.quadratic", 0.032f);
		phongShader.setFloat("spotLight.innerCutoff", glm::cos(glm::radians(12.5f)));
		phongShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));

		// Draw shaded cubes
		// -----------------------------------------------------------------------------------------------
		phongShader.use();
		phongShader.setVec3("viewPos", camera.Position);
		phongShader.setMatrix4f("projection", projection);
		phongShader.setMatrix4f("view", view);

		glBindVertexArray(shadedCubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle),
				glm::vec3(1.0f, 0.3f, 0.5f));
			phongShader.setMatrix4f("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw light cubes
		// -----------------------------------------------------------------------------------------------
		lightCubeShader.use();
		lightCubeShader.setMatrix4f("projection", projection);
		lightCubeShader.setMatrix4f("view", view);

		glBindVertexArray(lightCubeVAO);
		for (int i = 0; i < (sizeof(pointLightPositions) / sizeof(pointLightPositions[0])); i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lightCubeShader.setMatrix4f("model", model);
			lightCubeShader.setVec3("diffuse", pointLightColors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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