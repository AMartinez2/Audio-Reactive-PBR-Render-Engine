#include "Shader.h"


unsigned int programID;


// Constructor
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// Get source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// Enable exception throwing in our ifstreams
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Fill our streams shader's file buffers
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// Close our files
		vShaderFile.close();
		fShaderFile.close();
		// Convert ours streams to strings
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_FAIL_AT_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// Compile our shaders
	unsigned int vertex, fragment;
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	_checkCompile(vertex, VERTEX);
	// Fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	_checkCompile(fragment, FRAGMENT);
	// Create the associated shader program
	_programID = glCreateProgram();
	glAttachShader(_programID, vertex);
	glAttachShader(_programID, fragment);
	glLinkProgram(_programID);
	_checkCompile(_programID, PROGRAM);
	// Shaders are no longer needed
	glDeleteShader(vertex);
	glDeleteShader(vertex);
}


// Function to activate the shader
void Shader::use() {
	glUseProgram(_programID);
}


// Uitility function for checking compilation/linking errors
void Shader::_checkCompile(GLuint shader, ShaderErrorTypes type) {
	GLint success;
	GLchar infoLog[1024];
	if (type == PROGRAM) {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR::" << type << "\n" << infoLog << std::endl;
		}
	}
	else {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR::" << type << "\n" << infoLog << std::endl;
		}
	}
}


// ProgramId getter
unsigned int Shader::getProgramId() {
	return _programID;
}


// GLUniform getters and setters -----------------------------------------------
void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(_programID, name.c_str()), (int)value);
}


void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(_programID, name.c_str()), value);
}


void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(_programID, name.c_str()), value);
}


void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
	glUniform2fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]);
}


void Shader::setvec2(const std::string &name, float x, float y) const {
	glUniform2f(glGetUniformLocation(_programID, name.c_str()), x, y);
}


void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
	glUniform3fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]);
}


void Shader::setVec3(const std::string &name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(_programID, name.c_str()), x, y, z);
}


void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
	glUniform4fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]);
}


void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(_programID, name.c_str()), x, y, z, w);
}


void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
	glUniformMatrix2fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
	glUniformMatrix3fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


