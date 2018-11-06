#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <BASS/bass.h>


HSTREAM bassInit(const char* audioFile);


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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Audio Reactive PBR Engine", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// TODO::Window functionality 
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl State
	glEnable(GL_DEPTH_TEST);


	BASS_Start();
	BASS_ChannelPlay(stream, false);
	const int NUM_BINS = 512;
	float prevbins[NUM_BINS] = { };
	float bins[NUM_BINS] = { };
	float FFT_SAMPLE_RANGEf = 1024.0f / (float)NUM_BINS;
	// render loop
	while (!glfwWindowShouldClose(window)) {
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
		// render
		std::cout << blue/50000 << std::endl;
		glClearColor(0.05f, 0.05f, blue / 50000, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
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