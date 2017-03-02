#pragma once
#include "ApplicationConstants.h"
#include <SDL_opengl.h>

class Texture {
public:
	Texture();
	~Texture();
	void ClearData();

	GLubyte* data;
private:
	GLuint m_textureId;		// ID of texture
	GLuint m_fbo;			// Frame buffer object
};