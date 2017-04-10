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
#include "SOIL.h"
#include <cstdlib>
#include <iostream>>

// TO-DO: Variable data.
GLfloat vertices[] = {
	// Positions          // Colors           // Texture Coords
	1.f,  1.f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
	1.f, -1.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
	-1.f, -1.f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
	-1.f,  1.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
};
GLuint indices[] = {  // Note that we start from 0!
	0, 1, 3, // First Triangle
	1, 2, 3  // Second Triangle
};

/*
	Default constructor generates a new 2D texture.
*/

// Parameterized Constructor loads an image from the given path with SOIL
Texture::Texture(char* path) {
	// Bind image data
	/*
	glGenFramebuffers(1, &this->m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_fbo);

	glGenTextures(1, &this->m_textureId);
	glBindTexture(GL_TEXTURE_2D, this->m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_textureId, 0);
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "TEXTURE:: Failed to load" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/

	// Generate buffers
	glGenBuffers(1, &this->m_vao);
	glGenBuffers(1, &this->m_vbo);
	glGenBuffers(1, &this->m_ebo);
	
	// Load vertex data
	glBindVertexArray(this->m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Create texture
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLint w, h;
	unsigned char* image = SOIL_load_image(path, &w, &h, 0, SOIL_LOAD_RGB);
	if (!image)
		std::cerr << "TEXTURE:: Unable to load image from path: " << path << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	// Set variables
	this->m_textureId = textureId;
	this->m_width = w;
	this->m_height = h;

	// Load shader
	this->m_shader = new Shader("assets/shader.vs", "assets/shader.fs");
	std::cout << "Loaded texture " << path << " with dimensions " << w << ", " << h << std::endl;
}

/*
	Default destructor.
*/
Texture::~Texture() {
	glDeleteVertexArrays(1, &this->m_vao);
	glDeleteBuffers(1, &this->m_vbo);
	glDeleteBuffers(1, &this->m_ebo);
	//glDeleteFramebuffers(1, &this->m_fbo);
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
	/* Bind our framebuffer and draw 3D scene (spinning cube)
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_fbo);
	glBindVertexArray(this->m_vao);
	glEnable(GL_DEPTH_TEST);
	m_shader->Use();*/

	// Bind texture
	m_shader->Use();
	glBindTexture(GL_TEXTURE_2D, this->m_textureId);
	m_shader->SetUniform1i("ourTexture1", 0);
	// Draw container
	glBindVertexArray(this->m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Texture::RenderData() {
	/*
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
	*/
}