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


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);


// camera
Camera camera(glm::vec3(0.0f, 3.0f, 20.0f));
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

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader shader("./shaders/vShader.vert", "./shaders/fShader.frag");
	shader.use();
	// configure global opengl State
	glEnable(GL_DEPTH_TEST);

	// Initialize our sound stream buffer
	SoundControl soundControl("HOT DRUM.mp3");
	//SoundControl soundControl("jasei.mp3");

	// Play the audio	
	soundControl.playAudio();

	// Change direction camera faces
	camera.Front = camera.Front - glm::vec3(0.0f, 0.15f, 0.0f);

	// Rotation angle
	float angle = 0;

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// User input
		processInput(window);
		
		// Extract lower granularity frequency bins
		float* bins = soundControl.processBins();
		float cols[10];
		int count = 0;
		int index = 0;
		float temp = 0;
		// We need 10 freq data samples. So every 15 bins, we stash our current accumulation and start a new one.
		for (int i = 24; i < soundControl.getFFTRange(); i++) {
			temp += bins[i];
			if (count == 100) {
				cols[index] = temp/100;
				index += 1;
				count = 0;
				temp = 0;
			}
			count += 1;
		}
		// render
		glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		//std::cout << cols[3] << std::endl;
		glBindVertexArray(cubeVAO);
		for (int i = 0; i < 10; i++) {
			// World transformation
			glm::mat4 model;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
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