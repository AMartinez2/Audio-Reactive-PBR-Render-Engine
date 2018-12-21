#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "SoundControl.h"
#include "Model.h"


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void renderCubes(Shader shader, glm::mat4 projection, glm::mat4 view, unsigned int cubeVAO, float angle, float* bins);

// camera
//Camera camera(glm::vec3(7.386900f, 4.178008f, -2.777811f), glm::vec3(0.0f, 1.0f, 0.0f), -15.0f, 185.5f);
Camera camera(glm::vec3(7.386900f, 4.178008f, -2.777811f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// Use primative vertex object just while we resolve frequency interpretation
float vertices[] = {
	// positions          
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
	-0.5f,  0.5f, -0.5f,
};


glm::vec3 positions[] = {
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(-3.0f, 0.0f, 0.0f),
	glm::vec3(3.0f, 0.0f, 0.0f),
	glm::vec3(-5.0f, 0.0f, 0.0f),
	glm::vec3(5.0f, 0.0f, 0.0f),
	glm::vec3(-7.0f, 0.0f, 0.0f),
	glm::vec3(7.0f, 0.0f, 0.0f),
	glm::vec3(-9.0f, 0.0f, 0.0f),
	glm::vec3(9.0f, 0.0f, 0.0f),
	glm::vec3(13.0f, 0.0f, 0.0f),
};


glm::vec3 pointLightPositions[] = {
		glm::vec3(2.0f,  2.0f,  3.0f),
		glm::vec3(-2.0f,  2.0f,  -3.0f),
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
};


// Program entry point
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif 
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Audio Reactive PBR Engine", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	camera.GetViewMatrix();

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader modelShader("./shaders/model_shader.vert", "./shaders/model_shader.frag");
	Shader lightShader("./shaders/vShader.vert", "./shaders/fShader.frag");
	// configure global opengl State
	glEnable(GL_DEPTH_TEST);

	//Model ourModel("./models/nanosuit/nanosuit.obj");
	//Model ourModel("./models/planet/planet.obj");
	//Model ourModel("./spider/spider.obj");

	Model ourModel("./models/stag/untitled.obj");

	// Initialize our sound stream buffer
	SoundControl soundControl("HOT DRUM.mp3");
	//SoundControl soundControl("jasei.mp3");

	// Rotation angle
	float angle = 0;

	// Play the audio	
	//soundControl.setVolume(.1);
	soundControl.playAudio();

	camera.Front = glm::vec3(-0.879961, -0.284015, 0.380793);
	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// User input
		processInput(window);

		// Extract lower granularity frequency bins
		float bins[10];
		float* temp = soundControl.processBins();
		for (int i = 0; i < 10; i++) {
			bins[i] = temp[i];
		}
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// render
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// function call to view frequency bins
		//renderCubes(lightShader, projection, view, cubeVAO, angle, bins);

		modelShader.use();

		modelShader.setVec3("viewPos", camera.Position);
		modelShader.setFloat("material.shininess", 32.0f);

		// directional light
		modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		modelShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		modelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// point light 
		/*for (int i = 0; i < pointLightPositions->length(); i++) {
			std::string uniform = "pointLights[" + std::to_string(i) + "]";
			modelShader.setVec3(uniform + ".position", pointLightPositions[i]);
			modelShader.setVec3(uniform + ".ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3(uniform + ".diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3(uniform + ".specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat(uniform + ".constant", 1-((bins[0] + bins[1] + bins[2])/3));
			modelShader.setFloat(uniform + ".linear", 0.09);
			modelShader.setFloat(uniform + ".quadratic", 0.032);
		}*/
		std::string uniform = "pointLights[0]";
		modelShader.setVec3(uniform + ".position", pointLightPositions[0]);
		modelShader.setVec3(uniform + ".ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3(uniform + ".diffuse", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3(uniform + ".specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat(uniform + ".constant", 1 - ((bins[0] + bins[1] + bins[2] ) / 3));
		modelShader.setFloat(uniform + ".linear", 0.09);
		modelShader.setFloat(uniform + ".quadratic", 0.032);

		std::string uniform2 = "pointLights[1]";
		modelShader.setVec3(uniform2 + ".position", pointLightPositions[1]);
		modelShader.setVec3(uniform2 + ".ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3(uniform2 + ".diffuse", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3(uniform2 + ".specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat(uniform2 + ".constant", 1 - (( bins[6] + bins[7] + bins[8]) / 3));
		modelShader.setFloat(uniform2 + ".linear", 0.09);
		modelShader.setFloat(uniform2 + ".quadratic", 0.032);

		/*
		modelShader.setVec3("pointLight.position", 0.0f, -3.0f, 2.0f);
		modelShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("pointLight.constant", 1.0f);
		modelShader.setFloat("pointLight.linear", 0.09);
		modelShader.setFloat("pointLight.quadratic", 0.032);*/

		// spotLight
		modelShader.setVec3("spotLight.position", camera.Position);
		modelShader.setVec3("spotLight.direction", camera.Front);
		modelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		modelShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		modelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("spotLight.constant", 1.0f);
		modelShader.setFloat("spotLight.linear", 0.09);
		modelShader.setFloat("spotLight.quadratic", 0.032);
		modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(18.0f)));

		modelShader.setBool("useDirLight", false);
		modelShader.setBool("usePointLight", true);
		// flashlight
		modelShader.setBool("useSpotLight", false);

		// view/projection transformations
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.5f));
		modelShader.setMat4("model", model);
		ourModel.Draw(modelShader);

		// also draw the lamp object(s)
		lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		lightShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 2; i++) {
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		std::cout << "Position: " << glm::to_string(camera.Position) << " | Front: " << glm::to_string(camera.Front) << " | Pitch: " << camera.Pitch << " | Yaw: " << camera.Yaw << std::endl;
		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	//soundControl.~SoundControl();
	return 0;
}


// glfw: This callback function is called whenever the window changes size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Height will be significantly larger than specified on retina displays
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


// glfw: This callback function is called whenever the mouse moves
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}


// Process Keyboard inputs
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
}


// Readlly messy, as we only use this function for looking at frequencies
void renderCubes(Shader shader, glm::mat4 projection, glm::mat4 view, unsigned int cubeVAO, float angle, float* bins) {
	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	glBindVertexArray(cubeVAO);
	for (int i = 0; i < 10; i++) {
		// World transformation
		glm::mat4 model;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, positions[i]);
		// Set the cube color
		glm::vec3 color(0.05, bins[i], 0.05f);
		// Assign the values to our shader
		shader.setVec3("color", color);
		shader.setMat4("model", model);
		// Draw
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	if (angle >= 360.0f) {
		angle = 0;
	}
	else {
		angle += 0.5f;
	}

}