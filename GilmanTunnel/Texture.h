#pragma once
#include <SDL_opengl.h>

class Texture {
	Texture();
	Texture(GLuint textureId);
	~Texture();

private:
	GLuint m_textureId;		// ID of texture
	GLuint m_fbo;			// Frame buffer object
	GLubyte* data;
};