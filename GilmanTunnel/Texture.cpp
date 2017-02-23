/**
	Texture.cpp
	Purpose: OpenGL for 
**/

#include "stdafx.h"
#include "Texture.h"

Texture::Texture() {
	this->m_textureId = 0;
	this->m_fbo = 0;
}

Texture::Texture(GLuint textureId) {
	this->m_textureId = textureId;
}