#pragma once
#include "ApplicationConstants.h"
#define GLEW_STATIC
#include <Windows.h>
#include <Ole2.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <string>

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
	~Texture();
	bool LoadTexture(char* path);
	virtual void Render(unsigned int elapsed);
	void Reset();
	void SetElapsed(float e);
	float GetElapsed() { return m_elapsed; }
	std::string GetPath();
protected:
	GLuint m_textureId;			// ID of texture
	GLuint m_vao;				// VAO
	GLuint m_vbo;				// Vertex buffer onject
	GLuint m_ebo;				// Element buffer object
	GLuint m_width, m_height;	// Dimensions of Texture
	std::string m_path;			// Path to image

	GLfloat m_vertices[32];

	float m_alpha;				// Alpha value of image
	float m_elapsed;			// Internal timer
	Shader* m_shader;			// Shader program
};