#pragma once
#define GLEW_STATIC

#include "ApplicationConstants.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <Windows.h>
#include <NuiApi.h>
#include <vector>
#include <string>

const float PercentOfScreen_ = 0.3f; // Height of swipe area compared to screen
const int DefaultOffset_ = 0;

struct SwipeNode {
	SwipeNode* next;		// Follow up node
	int x, y;				// Top left coordinates of swipe
	int w, h;				// Swipe area
	unsigned int elapsed;	// Internal timer
};

class SwipeGesture {
public:
	SwipeGesture();
	~SwipeGesture();
	void SetNodes(int numNodes, int yOffset = DefaultOffset_);
	bool Update();
	USHORT* DepthData;		// DEpth data of the Kinect sensor
private:
	SwipeNode* m_head;		// Starting node
	SwipeNode* m_current;	// Node being updated
};