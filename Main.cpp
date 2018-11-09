#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <BASS/bass.h>

#include "Shader.h"
#include "Camera.h"


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


// Prototypes
HSTREAM bassInit(const char* audioFile);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


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


// Program entry point
int main() {
	HSTREAM stream = bassInit("air.mp3");
	if (!stream) {
		std::cout << "Bass initialization failed" << std::endl;
		return -1;
	}
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
	BASS_Start();
	BASS_ChannelPlay(stream, false);
	const int NUM_BINS = 512;
	float prevbins[NUM_BINS] = { };
	float bins[NUM_BINS] = { };
	float FFT_SAMPLE_RANGEf = 1024.0f / (float)NUM_BINS;
	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float fft[1024];
		BASS_ChannelGetData(stream, fft, BASS_DATA_FFT2048);
		//TODO NORMALISE AFTER SQRT
		// Scale FFT values
		for (int i = 0; i < 1024; i++)
			fft[i] = sqrt(fft[i]) * (5.0f * 800.0f); 

		// Update the old bins
		std::copy(bins, bins + NUM_BINS, prevbins);

		// Update the new bins
		//for (int i = 0; i < NUM_BINS; i++) {
		float blue = 0;
		for (int i = 0; i < 10; i++) {
			bins[i] = 0.0f;

			// Get upper and lower values for FFT sampling
			int lower = roundf(FFT_SAMPLE_RANGEf * (float)(i));
			int upper = roundf(FFT_SAMPLE_RANGEf * (float)(i + 1));

			// Average of FFT values 
			for (int j = lower; j < upper; j++)
				bins[i] += fft[j];
			bins[i] /= FFT_SAMPLE_RANGEf;

			// Smooth by averaging old and new bins
			bins[i] = (bins[i] + prevbins[i]) * 0.5f;
			blue += bins[i];
		}

		// User input
		processInput(window);

		// render
		std::cout << blue/50000 << std::endl;
		//glClearColor(0.05f, 0.05f, blue / 50000, 1.0f);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// world transformation
		glm::mat4 model;
		//model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
		glm::vec3 color (0.05, blue / 50000, 0.05f);
		shader.setVec3("color", color);
		shader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}


HSTREAM bassInit(const char* audioFile) {
	if (!BASS_Init(-1, 44100, 0, 0, 0)) {
		std::cout << "Bass failed to initialise" << std::endl;
		return -1;
	}

	HSTREAM stream;
	stream = BASS_StreamCreateFile(false, audioFile, 0, 0, BASS_STREAM_PRESCAN);

	if (!stream) {
		std::cout << "Bass failed to open audio file" << std::endl;
		return -1;
	}
	return stream;
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
	if (firstMouse)
	{
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