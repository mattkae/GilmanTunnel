#pragma once

#include "ApplicationConstants.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL_opengl.h>

#define KINECT18
#include <Windows.h>
#include <NuiApi.h>
#ifdef KINECT18
#else
struct IKinectSensor;
struct IColorFrameReader;
struct IDepthFrameReader;
#endif
class Texture;
class Window;

class Application {
public:
	Application(unsigned int width = ApplicationConstants::DefaultWidth_, unsigned int height = ApplicationConstants::DefaultHeight_);
	~Application();
	Application(Application & other);
	void Run();
	// Getters
	bool GetRunning();
private:
	bool initializeKinect();
	void getKinectData();

	int m_width, m_height;				// Dimensions of application
	bool m_depth;							// Set true when reading depth buffer

#ifdef KINECT18
	INuiSensor* m_sensor;
	HANDLE m_rgbStream;
	HANDLE m_depthStream;
#else
	IKinectSensor* m_sensor;			// Kinect sensor
	IColorFrameReader* m_colorReader;	// Kinect color data source
	IDepthFrameReader* m_depthReader;	// Kinect depth data
#endif
	bool m_running;						// Denotes prepared instance of Application
	bool m_paused;						// Denotes paused instance of Application
	Window* m_window;					// Window context
	int m_depthHeight, m_depthWidth;	// Depth Reader dimensions

	Texture* m_texture;					// Texture being rendered
	GLbyte* m_rgbData;					// Holds color data
	GLushort* m_depthData;				// Holds depth data
	void* m_context;					// GL's context
};