#pragma once
#include "Texture.h"
#include <vector>

class SwipeGesture;

class Gallery {
public:
	Gallery();
	~Gallery();
	void LoadImages();
	void Update(CrossedState state);
	void Render(unsigned int elapsed);
	USHORT* GetData();
private:
	std::vector<Texture*> m_textures;	// List of textures
	bool m_isActive;					// Currently updating gallery
	bool m_isEnding;					// No one in front - fade out
	int m_currentIndex;					// Current texture being displayed
	int m_elapsed;						// Internal timer to trigger events
	CrossedState m_current;				// Is someone currently in front of the Kinect?
	CrossedState m_before;				// Was someone just in front of the Kinect?
	SwipeGesture* m_swipeGesture;		// Swipe gesture to change image
};