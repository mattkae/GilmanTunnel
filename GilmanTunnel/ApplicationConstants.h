#pragma once

#if !__APPLE__
#define GLEW_STATIC
#include <Windows.h>
#include <Ole2.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#endif

// Determines whether someone is in the way of the Kinect
// Crossed state invalid is triggered when the Kinect does
// not collect data.
enum CrossedState {
	CrossedState_True,
	CrossedState_False,
	CrossedState_Invalid
};

namespace ApplicationConstants {
	const float Second_						= 1000.f;
	const float TargetFps_					= 60.f;
	const float OptimalTime_				= Second_ / TargetFps_;

	const unsigned int DefaultWidth_		= 1280;
	const unsigned int DefaultHeight_		= 960;

	const unsigned int DepthWidth_			= 640;
	const unsigned int DepthHeight_			= 480;

	const USHORT MaxDepth_					= 1700.f;

	const unsigned int ProjectionTime_		= 10 * Second_;
	const unsigned int FadeInTime_			= ProjectionTime_ / 4;
	const unsigned int FadeOutTime_			= ProjectionTime_ - FadeInTime_;
}