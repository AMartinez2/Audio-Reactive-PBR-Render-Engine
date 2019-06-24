#ifndef POSITION_DATA_H
#define POSITION_DATA_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


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
	glm::vec3(5.0f,  5.0f,  6.0f),
	glm::vec3(-5.0f,  5.0f,  6.0f),
	glm::vec3(0.0f, 0.0f, -5.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};


glm::vec3 cubePositions[] = {
	glm::vec3(-8.0f, 0.0f, 0.0f),
	glm::vec3(8.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, -8.0f, 0.0f),
	glm::vec3(0.0f, 8.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, -8.0f),
	glm::vec3(0.0f, 0.0f, 8.0f)
};


#endif