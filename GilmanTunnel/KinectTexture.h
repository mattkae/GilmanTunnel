#pragma once

#include "Texture.h"

class KinectTexture : public Texture {
public:
	KinectTexture(int size, int width, int height);
	~KinectTexture();
	void Render(unsigned int elapsed);
	GLubyte* data;		// The RGB Data in the texture
};