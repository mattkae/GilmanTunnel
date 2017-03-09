#include "stdafx.h"
#include "Shader.h"

#include <Ole2.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/*********************************************
Create a shader program from the given shader paths.

@param vertexPath - Path to vertex shader.
@param fragmentPath - Path to fragment shader.
*********************************************/
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// Read the shader from file path.
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Error: Shader file not successfully read" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile the shader.
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, 0);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, 0, infoLog);
		std::cerr << "Error: Vertex shader failed to compile - " << infoLog << std::endl;
	}

	// Fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, 0);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, 0, infoLog);
		std::cerr << "Error: Fragment shader failed to compile - " << infoLog << std::endl;
	}

	// Shader program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->Program, 512, 0, infoLog);
		std::cerr << "Error: Shader program failed to link - " << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

/*********************************************
Use this shader's program.

@return this - Shader object
*********************************************/
Shader* Shader::Use()
{
	glUseProgram(this->Program);
	return this;
}

/*********************************************
Returns the shader's program number.

@return shader program.
*********************************************/
unsigned int Shader::GetProgram() {
	return this->Program;
}

/*********************************************
Return the location of the given name.

@param name - Tag being looked for.

@return location of uniform.
*********************************************/
int Shader::GetUniform(const char* name)
{
	return glGetUniformLocation(this->Program, name);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
*********************************************/
void Shader::SetUniform1f(char* name, float v0)
{
	glUniform1f(this->GetUniform(name), v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
@param v1 - Second value.
*********************************************/
void Shader::SetUniform2f(char* name, float v0, float v1)
{
	glUniform2f(this->GetUniform(name), v0, v1);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
@param v1 - Second value.
@param v2 - Third value.
*********************************************/
void Shader::SetUniform3f(char* name, float v0, float v1, float v2)
{
	glUniform3f(this->GetUniform(name), v0, v1, v2);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
@param v1 - Second value.
@param v2 - Third value.
@param v3 - Fourth value.
*********************************************/
void Shader::SetUniform4f(char* name, float v0, float v1, float v2, float v3)
{
	glUniform4f(this->GetUniform(name), v0, v1, v2, v3);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
*********************************************/
void Shader::SetUniform1i(char* name, int v0)
{
	glUniform1i(this->GetUniform(name), v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
@param v1 - Second value.
*********************************************/
void Shader::SetUniform2i(char* name, int v0, int v1)
{
	glUniform2i(this->GetUniform(name), v0, v1);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
@param v1 - Second value.
@param v2 - Third value.
*********************************************/
void Shader::SetUniform3i(char* name, int v0, int v1, int v2)
{
	glUniform3i(this->GetUniform(name), v0, v1, v2);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
@param v1 - Second value.
@param v2 - Third value.
@param v3 - Fourth value.
*********************************************/
void Shader::SetUniform4i(char* name, int v0, int v1, int v2, int v3)
{
	glUniform4i(this->GetUniform(name), v0, v1, v2, v3);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
*********************************************/
void Shader::SetUniform1ui(char* name, unsigned int v0)
{
	glUniform1ui(this->GetUniform(name), v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
@param v1 - Second value.
*********************************************/
void Shader::SetUniform2ui(char* name, unsigned int v0, unsigned int v1)
{
	glUniform2ui(this->GetUniform(name), v0, v1);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
@param v1 - Second value.
@param v2 - Third value.
*********************************************/
void Shader::SetUniform3ui(char* name, unsigned int v0, unsigned int v1, unsigned int v2)
{
	glUniform3ui(this->GetUniform(name), v0, v1, v2);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param v0 - First value.
@param v1 - Second value.
@param v2 - Third value.
@param v3 - Fourth value.
*********************************************/
void Shader::SetUniform4ui(char* name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3)
{
	glUniform4ui(this->GetUniform(name), v0, v1, v2, v3);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform1fv(char* name, int count, const float *v0)
{
	glUniform1fv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform2fv(char* name, int count, const float *v0)
{
	glUniform2fv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform3fv(char* name, int count, const float *v0)
{
	glUniform3fv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform4fv(char* name, int count, const float *v0)
{
	glUniform4fv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform1iv(char* name, int count, const int *v0)
{
	glUniform1iv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform2iv(char* name, int count, const int *v0)
{
	glUniform2iv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform3iv(char* name, int count, const int *v0)
{
	glUniform3iv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform4iv(char* name, int count, const int *v0)
{
	glUniform4iv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform1uiv(char* name, int count, const unsigned int *v0)
{
	glUniform1uiv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform2uiv(char* name, int count, const unsigned int *v0)
{
	glUniform2uiv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform3uiv(char* name, int count, const unsigned int *v0)
{
	glUniform3uiv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param v0
*********************************************/
void Shader::SetUniform4uiv(char* name, int count, const unsigned int *v0)
{
	glUniform4uiv(this->GetUniform(name), count, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param transpose
@param v0
*********************************************/
void Shader::SetUniformMatrix2fv(char* name, int count, bool transpose, const float* v0)
{
	glUniformMatrix2fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param transpose
@param v0
*********************************************/
void Shader::SetUniformMatrix3fv(char* name, int count, bool transpose, const float* v0)
{
	glUniformMatrix3fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param transpose
@param v0
*********************************************/
void Shader::SetUniformMatrix4fv(char* name, int count, bool transpose, const float* v0)
{
	glUniformMatrix4fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param transpose
@param v0
*********************************************/
void Shader::SetUniformMatrix2x3fv(char* name, int count, bool transpose, const float* v0)
{
	glUniformMatrix2x3fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param transpose
@param v0
*********************************************/
void Shader::SetUniformMatrix3x2fv(char* name, int count, bool transpose, const float* v0)
{
	glUniformMatrix3x2fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param transpose
@param v0
*********************************************/
void Shader::SetUniformMatrix2x4fv(char* name, int count, bool transpose, const float* v0)
{
	glUniformMatrix2x4fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param transpose
@param v0
*********************************************/
void Shader::SetUniformMatrix4x2fv(char* name, int count, bool transpose, const float* v0)
{
	glUniformMatrix4x2fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param transpose
@param v0
*********************************************/
void Shader::SetUniformMatrix3x4fv(char* name, int count, bool transpose, const float* v0)
{
	glUniformMatrix3x4fv(this->GetUniform(name), count, transpose, v0);
}

/*********************************************
Set the value of the given tag in the shader.

@param name - Tag being looked for.
@param count
@param transpose
@param v0
*********************************************/
void Shader::SetUniformMatrix4x3fv(char* name, int count, bool transpose, const float* v0)
{
	glUniformMatrix4x3fv(this->GetUniform(name), count, transpose, v0);
}