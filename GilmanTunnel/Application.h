#pragma once

#define GLEW_STATIC

#include "ApplicationConstants.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <Windows.h>
#include <NuiApi.h>
#include <vector>
#include <string>

class KinectTexture;
class Window;
class Gallery;

// Determines what data we're trying to display
enum ApplicationState {
	ApplicationState_Depth,
	ApplicationState_RGB,
	ApplicationState_Gallery
};

class Application {
public:
	Application(unsigned int width = ApplicationConstants::DefaultWidth_, unsigned int height = ApplicationConstants::DefaultHeight_);
	~Application();
	Application(Application & other);
	void Run();
	bool GetRunning();
private:
	bool initializeKinect();
	void getKinectDepthData(GLubyte* dest);
	void getKinectRgbData(GLubyte* dest);
	CrossedState checkIfCrossed();

	int m_width, m_height;				// Dimensions of application
	ApplicationState m_state;			// Specifies what data we're interested in

	INuiSensor* m_sensor;				// Kinect sensor
	HANDLE m_rgbStream;					// Stream for Color Data
	HANDLE m_depthStream;				// Stream for Depth data

	bool m_running;						// Denotes prepared instance of Application
	bool m_paused;						// Denotes paused instance of Application
	Window* m_window;					// Window context
	void* m_context;					// GL's context
	
	KinectTexture* m_kTexture;			// Texture from kinect being rendered
	Gallery* m_gallery;
};