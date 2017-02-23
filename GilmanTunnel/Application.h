#pragma once

#include "ApplicationConstants.h"

struct IKinectSensor;
struct IColorFrameReader;
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
	void getKinectData(Texture* texture);

	IKinectSensor* m_sensor;			// Kinect sensor
	IColorFrameReader* m_reader;		// Kinect color data source
	bool m_running;						// Denotes prepared instance of Application
	Window* m_window;					// Window context
};