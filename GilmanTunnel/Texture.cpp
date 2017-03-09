/**
	Texture.cpp
	Purpose: Render the buffer to the screen.
**/

#include "stdafx.h"
#include "Texture.h"
#include "Shader.h"
#include <Windows.h>
#include <Ole2.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <cstdlib>

// TO-DO: Variable data.
GLfloat vertices[] = {
	// Positions         // Texture Coords
	0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
	0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Bottom Left
	-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // Top Left 
};
GLuint indices[] = {  // Note that we start from 0!
	0, 1, 3, // First Triangle
	1, 2, 3  // Second Triangle
};

/*
	Default constructor generates a new 2D texture.
*/
Texture::Texture() {
	glGenVertexArrays(1, &this->m_vao);
	glGenBuffers(1, &this->m_vbo);
	glGenBuffers(1, &this->m_ebo);

	glBindVertexArray(this->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Load textures
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &this->m_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ApplicationConstants::DefaultWidth_, 
				ApplicationConstants::DefaultHeight_, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)this->data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Load shader files
	this->m_shader = new Shader("path/to/shaders/textures.vs", "path/to/shaders/textures.frag");
}

/*
	Default destructor.
*/
Texture::~Texture() {
	glDeleteVertexArrays(1, &this->m_vao);
	glDeleteBuffers(1, &this->m_vbo);
	glDeleteBuffers(1, &this->m_ebo);
	free(this->data);
}

/*
	Clear data in buffer.
*/
void Texture::ClearData() {
	memset(this->data, 0, sizeof(GLubyte) * ApplicationConstants::DefaultWidth_ * ApplicationConstants::DefaultHeight_ * 4);
}

/*
	Render data in buffer to the screen.
	OpenGL is opaque af, look on learnopengl.com if you're interested.
*/
void Texture::Render() {
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &this->m_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ApplicationConstants::DefaultWidth_, 
				ApplicationConstants::DefaultHeight_, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)this->data);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(this->m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}