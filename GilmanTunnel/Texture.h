#pragma once
#include "ApplicationConstants.h"
#if !__APPLE__
#define GLEW_STATIC
#endif

class Shader;

class Texture {
public:
	Texture(char* path = "");
	~Texture();
	void Render(unsigned int elapsed);

private:
	unsigned int m_textureId;		// ID of texture
	unsigned int m_vao;				// VAO
	//unsigned int m_fbo;
	unsigned int m_vbo;				// Vertex buffer onject
	unsigned int m_ebo;				// Element buffer object
	unsigned int m_width, m_height;	// Dimensions of Texture
	float m_alpha;
	float m_xvalue;
	Shader* m_shader;
};