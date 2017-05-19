#include "stdafx.h"
#include "KinectTexture.h"
#include "Shader.h"
#include <iostream>

/*
	Load the Kienct texture given the size of the data, the width
	of the screen, and the height of the screen.

	@param size array size for RGBA data
	@param width width of screen
	@param height height of screen
*/
KinectTexture::KinectTexture(int size, int width, int height) 
{
	// Allocate data
	this->data = new GLubyte[size];

	// Generate buffers
	glGenBuffers(1, &this->m_vao);
	glGenBuffers(1, &this->m_vbo);
	glGenBuffers(1, &this->m_ebo);

	// Load vertex data
	glBindVertexArray(this->m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultVertices), DefaultVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DefaultIndices), DefaultIndices, GL_STATIC_DRAW);
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
	w = width;
	h = height;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)this->data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set variables
	this->m_textureId = textureId;
	this->m_width = w;
	this->m_height = h;

	// Load shader
	this->m_shader = new Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");

	// Set alpha value
	this->m_alpha = 0.5f;

	std::cout << "SUCCESS:: Loaded kinect texture with size " << size << std::endl;
}


/*
	Deallocator.
*/
KinectTexture::~KinectTexture()
{
	delete this->data;
}

/*
	Render the Kinect's data across the whole screen.

	@param elapsed - void, unused in this context
*/
void KinectTexture::Render(unsigned int elapsed) 
{
	(void)elapsed;
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	m_shader->Use();
	glBindTexture(GL_TEXTURE_2D, this->m_textureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->m_width, this->m_height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)this->data);
	m_shader->SetUniform1i("ourTexture1", 0);
	m_shader->SetUniform1f("alpha", this->m_alpha);
	// Draw container
	glBindVertexArray(this->m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
}