#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"

#include <string>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	// Mesh Data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector <Texture> textures;
	// Functions
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indecies, std::vector<Texture> textures);
	void Draw(Shader shader);
private:
	// Render data
	unsigned int _VAO, _VBO, _EBO;
	// Functions
	void _setupMesh();
};
#endif
