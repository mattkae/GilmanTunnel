#pragma once

#if !__APPLE__
#define GLEW_STATIC
#include <Windows.h>
#include <Ole2.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#endif

namespace ApplicationConstants {
	const float Second_ = 1000.f;
	const float TargetFps_		= 60.f;
	const float OptimalTime_	= Second_ / TargetFps_;

	const unsigned int DefaultWidth_ = 1280;
	const unsigned int DefaultHeight_ = 960;

	const unsigned int DepthWidth_ = 640;
	const unsigned int DepthHeight_ = 480;

	const USHORT MaxDepth_ = 1700.f;
}