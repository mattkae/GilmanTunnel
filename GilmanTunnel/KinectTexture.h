#pragma once

#include "Texture.h"

class KinectTexture : public Texture {
public:
	KinectTexture();
	KinectTexture(int size);
	~KinectTexture();
	void Render(unsigned int elapsed);
	GLubyte* data;
};