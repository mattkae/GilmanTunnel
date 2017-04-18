/**
	Texture.cpp
	Purpose: Render the buffer to the screen.
**/

#include "stdafx.h"
#include "Texture.h"
#include "Shader.h"
#include "SOIL.h"
#include <cstdlib>
#include <iostream>

const float TIME_TO_FADE = 5000;
const float TIME_ALIVE = 30000;


/*
	Default constructor generates an empty texture
*/
Texture::Texture() 
{
	Texture(nullptr);
}

/*
	Parameterized Constructor loads an image from the given path with SOIL.

	@param path path to image
*/
Texture::Texture(char* path) 
{
	// Do nothing on a nullptr
	if (path == nullptr)
		return;

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

	// Load image data into textyure
	unsigned char* image = SOIL_load_image(path, &w, &h, 0, SOIL_LOAD_RGB);
	if (!image)
		std::cerr << "TEXTURE:: Unable to load image from path: " << path << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set variables
	this->m_textureId = textureId;
	this->m_width = w;
	this->m_height = h;

	// Load shader
	this->m_shader = new Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
	std::cout << "Loaded texture " << path << " with dimensions " << w << ", " << h << std::endl;
	this->m_alpha = 0.5f;
}

/*
	Default destructor.
*/
Texture::~Texture()
{
	glDeleteVertexArrays(1, &this->m_vao);
	glDeleteBuffers(1, &this->m_vbo);
	glDeleteBuffers(1, &this->m_ebo);
}

/*
	Render data in buffer to the screen.
	OpenGL is opaque af, look on learnopengl.com if you're interested.
*/
void Texture::Render(unsigned int elapsed)
{
	/*this->m_xvalue += elapsed;
	this->m_alpha = 1.f - (this->m_xvalue / TIME_TO_FADE);
	this->m_alpha = this->m_alpha < 0 ? 0.f : this->m_alpha > 1.f ? 1.f : this->m_alpha;*/

	// Bind texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	m_shader->Use();
	glBindTexture(GL_TEXTURE_2D, this->m_textureId);
	m_shader->SetUniform1i("ourTexture1", 0);
	m_shader->SetUniform1f("alpha", this->m_alpha);
	// Draw container
	glBindVertexArray(this->m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
}