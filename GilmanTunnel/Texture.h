#pragma once
#include "ApplicationConstants.h"
#define GLEW_STATIC
#include <Windows.h>
#include <Ole2.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

const GLfloat DefaultVertices[] = {
	// Positions          // Colors           // Texture Coords
	1.f,  1.f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
	1.f, -1.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
	-1.f, -1.f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
	-1.f,  1.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
};
const GLuint DefaultIndices[] = {  // Note that we start from 0!
	0, 1, 3, // First Triangle
	1, 2, 3  // Second Triangle
};

class Shader;

class Texture {
public:
	Texture();
	Texture(char* path);
	~Texture();
	virtual void Render(unsigned int elapsed);

protected:
	GLuint m_textureId;			// ID of texture
	GLuint m_vao;				// VAO
	GLuint m_vbo;				// Vertex buffer onject
	GLuint m_ebo;				// Element buffer object
	GLuint m_width, m_height;	// Dimensions of Texture

	GLfloat* m_vertices;
	GLuint* m_indices;

	float m_alpha;				// Alpha value of image
	float m_xvalue;				// Time along alpha value
	Shader* m_shader;			// Shader program
};