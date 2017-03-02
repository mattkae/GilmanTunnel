/**
	Texture.cpp
	Purpose: OpenGL for 
**/

#include "stdafx.h"
#include "Texture.h"
#include <Windows.h>
#include <Ole2.h>
#include <SDL_opengl.h>
#include <cstdlib>

Texture::Texture() {
	// Initialize textures
	glGenTextures(1, &this->m_textureId);
	glBindTexture(GL_TEXTURE_2D, this->m_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ApplicationConstants::DefaultWidth_, 
				ApplicationConstants::DefaultHeight_, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)this->data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
	free(this->data);
}

void Texture::ClearData() {
	memset(this->data, 0, sizeof(GLubyte) * ApplicationConstants::DefaultWidth_ * ApplicationConstants::DefaultHeight_ * 4);
}