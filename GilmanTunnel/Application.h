#pragma once

#define GLEW_STATIC

#include "ApplicationConstants.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <Windows.h>
#include <NuiApi.h>
#include <vector>
#include <string>

class Texture;
class KinectTexture;
class Window;


// Determines what data we're trying to display
enum ApplicationState {
	ApplicationState_Depth,
	ApplicationState_RGB,
	ApplicationState_Gallery
};


// Determines whether someone is in the way of the Kinect
// Crossed state invalid is triggered when the Kinect does
// not collect data.
enum CrossedState {
	CrossedState_True,
	CrossedState_False,
	CrossedState_Invalid
};

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
	void loadImagePaths();
	void getKinectDepthData(GLubyte* dest);
	void getKinectRgbData(GLubyte* dest);
	CrossedState checkIfCrossed();

	int m_width, m_height;				// Dimensions of application
	ApplicationState m_state;			// Specifies what data we're interested in

	INuiSensor* m_sensor;
	HANDLE m_rgbStream;
	HANDLE m_depthStream;

	bool m_running;						// Denotes prepared instance of Application
	bool m_paused;						// Denotes paused instance of Application
	Window* m_window;					// Window context
	void* m_context;					// GL's context
	
	std::vector<std::string> m_paths;	// All of the image paths in the directory
	Texture* m_texture;					// Texture being rendered
	KinectTexture* m_kTexture;			// Texture from kinect being rendered
};