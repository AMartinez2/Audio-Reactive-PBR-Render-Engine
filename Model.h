#ifndef MODEL_H
#define MODEL_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);


class Model {
public:
	// Model Data
	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	// Functions
	// Constructor, expects a filepath to a 3D model
	Model(std::string const &path, bool gamma = false);
	// Draws the model, and this all its meshes
	void Draw(Shader shader);
private:
	// Functions
	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
	void _loadModel(std::string const &path);

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its child nodes 
	void _processNode(aiNode *node, const aiScene *scene);
	
	Mesh _processMesh(aiMesh *mesh, const aiScene *scene);

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> _loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeNames);
};
#endif