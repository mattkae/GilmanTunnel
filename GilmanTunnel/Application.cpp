/**
	Application.cpp
	Purpose: Initializing, running, and closing the Kinect sensor and its tools.
	Processes the main loop.
**/

#include "stdafx.h"
#include "Application.h"
#include "Texture.h"
#include "Window.h"
#include <SDL.h>
#include <iostream>
#ifdef KINECT18
#include <NuiImageCamera.h>
#else
#include <Kinect.h>
#endif

using namespace std; // For error reporting.


/**
	Default constructor to initialize the application instance.
*/
Application::Application(unsigned int width, unsigned int height)
{
	// Setup SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "APPLICATION::SDL_INIT:: " << SDL_GetError() << endl;
		exit(EXIT_FAILURE);
	}

	// Initialize window & Kinect
	this->m_window = new Window(width, height);
	this->m_context = this->m_window->SetContext();
	this->m_running = initializeKinect();

	if (!this->m_running) {
		exit(EXIT_FAILURE);
	}
	this->m_paused = false;

	// Setup GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "APPLICATION::GLEW_INIT:: " << "Glew failed to initialize." << endl;
		exit(EXIT_FAILURE);
	}

	// Setup OpenGL
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Initialize texture
	this->m_texture = new Texture("assets/panda.jpg");

	// Store dimensions
	this->m_width = width;
	this->m_height = height;

	// Set depth boolean
	this->m_depth = true;

	// Allocate space for data
	if (m_depth) {
		this->m_depthData = new GLushort[(width * height)];
		if (!this->m_depthData) {
			std::cerr << "APPLICATION:: " << "Depth data allocation." << endl;
			exit(EXIT_FAILURE);
		}
	} else {
		this->m_rgbData = new GLbyte[(width * height) * 4];
		if (!this->m_rgbData) {
			std::cerr << "APPLICATION:: " << "RGB data allocation." << endl;
			exit(EXIT_FAILURE);
		}
	}
}

/*
	Default destructor.
*/
Application::~Application() 
{
	SDL_GL_DeleteContext(this->m_context);

	if (m_depth) delete[] this->m_depthData;
	//else delete[] this->m_rgbData;

#ifdef KINECT18
	if (this->m_sensor) {
		this->m_sensor->NuiShutdown();
		this->m_sensor->Release();
	}
#else
	// Close the sensor
	if (this->m_sensor) {
		this->m_sensor->Close();
		this->m_sensor = NULL;
	}
#endif

	// Free window memory
	//if (this->m_window) {
	//	this->m_window->Free();
	////	delete this->m_window;
	//}
	
	SDL_Quit();
}

/*
	Copy coonstructor.
*/
Application::Application(Application& other) 
{
	// TO-DO: Overload.
}

/*
	Initialize the Kinect sensor and prepare to read data from it.
	This works even without a Kinect connected.

	@return succesful initialization.
*/
bool Application::initializeKinect()
{
#ifdef KINECT18
	// Retrieve the number of sensors
	int numSensors = 0;
	HRESULT hr = NuiGetSensorCount(&numSensors);
	if (hr < 0) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: Unable to retrieve sensor count" << endl;
		return false;
	}

	if (numSensors == 0) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: No sensors available" << endl;
		return false;
	}

	// Create a new sensor
	if (NuiCreateSensorByIndex(0, &this->m_sensor) != S_OK) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: Unable to create sensor" << endl;
		return false;
	}
	else {
		std::cout << "Successfully loaded Kinect sensor" << endl;
	}

	// Initialize new sensor
	if (this->m_sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR) != S_OK) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: Unable to initialize NUI" << endl;
		return false;
	}
	// Choose data to collect
	if (this->m_depth) {
		this->m_sensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_DEPTH,
			NUI_IMAGE_RESOLUTION_1280x960,
			NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE,
			2,
			NULL,
			&this->m_depthStream
		);
	} else {
		this->m_sensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_COLOR,
			NUI_IMAGE_RESOLUTION_1280x960,
			0,
			2,
			NULL,
			&this->m_rgbStream
		);
	}

	if (this->m_sensor->NuiStatus() != S_OK) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: Sensor status not ok" << endl;
		return false;
	}

	return true;
#else
	if (FAILED(GetDefaultKinectSensor(&this->m_sensor))) {
		cerr << "ERROR::INITIALIZE_KINECT:: Unable to initialize sensor" << endl;
		return false;
	}

	if (this->m_sensor) {
		this->m_sensor->Open();

		// Color source
		IColorFrameSource* colorFrameSource = nullptr;
		this->m_sensor->get_ColorFrameSource(&colorFrameSource);
		colorFrameSource->OpenReader(&this->m_colorReader);
		if (colorFrameSource) {
			colorFrameSource->Release();
			colorFrameSource = NULL;
		}

		// Depth source
		IDepthFrameSource* depthFrameSource = nullptr;
		this->m_sensor->get_DepthFrameSource(&depthFrameSource);
		depthFrameSource->OpenReader(&this->m_depthReader);
		// Initialize depth dimensions
		IFrameDescription* frameDescription = nullptr;
		depthFrameSource->get_FrameDescription(&frameDescription);
		frameDescription->get_Width(&this->m_depthWidth);
		frameDescription->get_Height(&this->m_depthHeight);
		this->m_depthData = new uint16_t[this->m_depthWidth * this->m_depthHeight];
		if (depthFrameSource) {
			depthFrameSource->Release();
			depthFrameSource = NULL;
		}

		return true;
	}
	std::cerr << "ERROR::INITIAILIZE_KINECT:: m_sensor was null" << endl;
	return true;
#endif
}

/*
	Runs the application using SDL2 as a context.
*/
void Application::Run() 
{
	Uint32 currentTime, lastTime = 0, elapsed;
	float lag = 0;
	SDL_Event e;

	while (this->m_running) {
		currentTime = SDL_GetTicks();
		elapsed = currentTime - lastTime;
		lastTime = currentTime;
		lag += elapsed;

		// Poll input
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
				this->m_running = false;
			} else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_p) {
				this->m_paused = !this->m_paused;
			}
		}

		if (this->m_paused) {
			lag = 0;
			continue;
		}

		while (lag >= ApplicationConstants::OptimalTime_) {
			// Run Updates
			this->getKinectData();
			lag -= (Uint32) ApplicationConstants::OptimalTime_;
		}

		// Render...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->m_texture->Render(elapsed);
		SDL_GL_SwapWindow(this->m_window->GetWindow());
	}
}

/*
	Return whether the game is running or not.

	@return running
*/
bool Application::GetRunning() {
	return this->m_running;
}

/*
	Copy the kinect data into either the rgbStream
	or the depth stream.
*/
void Application::getKinectData() {
#ifdef KINECT18
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT lockedRect;

	// Acquire a frame
	if (this->m_depth) {
		if (this->m_sensor->NuiImageStreamGetNextFrame(this->m_depthStream, 0, &imageFrame) < 0) {
			std::cout << "Miss on depth data" << std::endl;
			return;
		}
	} else {
		if (this->m_sensor->NuiImageStreamGetNextFrame(this->m_rgbStream, 0, &imageFrame) < 0) {
			std::cout << "Miss on image data" << std::endl;
			return;
		}
	}
	
	INuiFrameTexture* iTexture = imageFrame.pFrameTexture;
	iTexture->LockRect(0, &lockedRect, NULL, 0);

	// Collect frame data
	if (lockedRect.Pitch != 0) {
		if (this->m_depth) {
			memset(this->m_depthData, 0, this->m_width * this->m_height);
			const USHORT* curr = (const USHORT*)lockedRect.pBits;
			const USHORT* dataEnd = curr + (this->m_width * this->m_height);
			std::cout << "Collecting depth data..." << std::endl;
			while (curr < dataEnd) {
				USHORT depth = NuiDepthPixelToDepth(*curr++);
				*this->m_depthData++ = depth;
			}
			this->m_sensor->NuiImageStreamReleaseFrame(this->m_depthStream, &imageFrame);
		} else {
			std::cout << "Collecting image data..." << std::endl;
			memset(this->m_rgbData, 0, this->m_width * this->m_height * 4);
			const BYTE* curr = (const BYTE*)lockedRect.pBits;
			const BYTE* dataEnd = curr + 1;

			while (curr < dataEnd) {
				*this->m_rgbData++ = *curr++;
			}
			this->m_sensor->NuiImageStreamReleaseFrame(this->m_rgbStream, &imageFrame);
		}
	}
	iTexture->UnlockRect(0);
#else
	IDepthFrame* depthFrame = nullptr;
	if (SUCCEEDED(this->m_depthReader->AcquireLatestFrame(&depthFrame))) {
		depthFrame->CopyFrameDataToArray(this->m_depthWidth * this->m_depthHeight, this->m_depthData);
	}
	// Acquire the color data (you can do this similarly with the whatever reader that you want)
	IColorFrame* frame = NULL;

	// The program is running way faster than the Kinect can output frames, so you must
	// not read the data if it isn't provided.
	if (SUCCEEDED(this->m_colorReader->AcquireLatestFrame(&frame))) {
		frame->CopyConvertedFrameDataToArray(this->m_window->GetWidth() * this->m_window->GetHeight() * 4, ((GLubyte*)texture->data), ColorImageFormat_Bgra);
		}
	if (frame) {
		frame->Release();
	}
#endif
}