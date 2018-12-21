#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


enum ShaderErrorTypes {
	PROGRAM,
	VERTEX,
	FRAGMENT,
	GEOMETRY
};


class Shader {
public:
	// Constructor
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// Function to activate the shader
	void use();

	// ID getter and setter
	unsigned int getProgramId();

	// GLUniform getters and setters
	void setBool	(const std::string &name, bool value) const;
	void setInt		(const std::string &name, int value) const;
	void setFloat	(const std::string &name, float value) const;
	void setVec2	(const std::string &name, const glm::vec2 &value) const;
	void setvec2	(const std::string &name, float x, float y) const;
	void setVec3	(const std::string &name, const glm::vec3 &value) const;
	void setVec3	(const std::string &name, float x, float y, float z) const;
	void setVec4	(const std::string &name, const glm::vec4 &value) const;
	void setVec4	(const std::string &name, float x, float y, float z, float w) const;
	void setMat2	(const std::string &name, const glm::mat2 &mat) const;
	void setMat3	(const std::string &name, const glm::mat3 &mat) const;
	void setMat4	(const std::string &name, const glm::mat4 &mat) const;

private:
	// Shader Program ID
	unsigned int _programID;

	// Uitility function for checking compilation/linking errors
	void _checkCompile(GLuint shader, ShaderErrorTypes type);
};

#endif