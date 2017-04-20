/**
	Application.cpp
	Purpose: Initializing, running, and closing the Kinect sensor and its tools.
	Processes the main loop.
**/

#include "stdafx.h"
#include "Application.h"
#include "KinectTexture.h"
#include "Window.h"
#include "Gallery.h"
#include <SDL.h>
#include <iostream>
#include <NuiImageCamera.h>

using namespace std; // For error reporting.

/*
	Default constructor to initialize the application instance.
*/
Application::Application(unsigned int width, unsigned int height)
{
	// Set depth boolean
	this->m_state = ApplicationState_Gallery;

	// Initialize Kinect sensor
	this->m_running = initializeKinect();
	if (!this->m_running) {
		system("pause");
		exit(EXIT_FAILURE);
	}

	// Setup SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "APPLICATION::SDL_INIT:: " << SDL_GetError() << endl;
		exit(EXIT_FAILURE);
	}

	// Initialize window & Kinect
	this->m_window = new Window(width, height);
	this->m_context = this->m_window->SetContext();

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


	// Initialize textures
	this->m_gallery = new Gallery();
	this->m_kTexture = new KinectTexture(width * height * 4);

	// Store application variables
	this->m_width = width;
	this->m_height = height;
	this->m_paused = false;
}

/*
	Default destructor.
*/
Application::~Application() 
{
	// Free GL data
	SDL_GL_DeleteContext(this->m_context);
	// Free texture data
	delete this->m_gallery;
	delete this->m_kTexture;
	// Free sensor data
	if (this->m_sensor) {
		this->m_sensor->NuiShutdown();
		this->m_sensor->Release();
	}
	// Free window memory
	if (this->m_window) {
		this->m_window->Free();
		delete this->m_window;
	}
	SDL_Quit();
}

/*
	Copy constructor.
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
	// Retrieve the number of sensors
	int numSensors = 0;
	HRESULT hr = NuiGetSensorCount(&numSensors);
	if (hr < 0) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: Unable to retrieve sensor count" << endl;
		return false;
	}
	// Check number of sensors
	if (numSensors == 0) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: No sensors available" << endl;
		return false;
	}
	// Create a new sensor
	hr = NuiCreateSensorByIndex(0, &this->m_sensor);
	if (hr != S_OK) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: Unable to create sensor" << endl;
		return false;
	} else {
		std::cout << "SUCCESS:: Successfully loaded Kinect sensor" << endl;
	}
	// Initialize new sensor
	hr = this->m_sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	if (hr != S_OK) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: Unable to initialize NUI" << endl;
		return false;
	}
	// Choose data to collect
	switch (this->m_state) {
	case ApplicationState_Depth:
		hr = this->m_sensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_DEPTH,
			NUI_IMAGE_RESOLUTION_640x480,
			0,
			2,
			NULL,
			&this->m_depthStream
		);
		if (hr!= S_OK) {
			std::cerr << "ERROR::INITIALIZE_KINECT:: Unable to open depth stream: Error code " << hr << std::endl;
			return false;
		}
		break;
	case ApplicationState_RGB:
		hr = this->m_sensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_COLOR,
			NUI_IMAGE_RESOLUTION_1280x960,
			0,
			2,
			NULL,
			&this->m_rgbStream
		);
		if (hr != S_OK) {
			std::cerr << "ERROR::INITIALIZE_KINECT:: Unable to open RGB stream: Error code " << hr << std::endl;
			return false;
		}
		break;
	case ApplicationState_Gallery:
		hr = this->m_sensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_DEPTH,
			NUI_IMAGE_RESOLUTION_640x480,
			0,
			2,
			NULL,
			&this->m_depthStream
		);
		if (hr != S_OK) {
			std::cerr << "ERROR::INITIALIZE_KINECT:: Unable to open depth stream: Error code " << hr << std::endl;
			return false;
		}
		break;
	default:
		break;
	}
	// Check status of sensor
	hr = this->m_sensor->NuiStatus();
	if (hr != S_OK) {
		std::cerr << "ERROR::INITIALIZE_KINECT:: Sensor status not ok" << endl;
		return false;
	}
	return true;
}

/*
	Runs the application using SDL2 as a context.
*/
void Application::Run() 
{
	Uint32 currentTime, lastTime = 0, elapsed;
	float lag = 0;
	SDL_Event e;

	// Main loop
	while (this->m_running) {
		// Update timing variables
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
		// Continue if paused
		if (this->m_paused) {
			lag = 0;
			continue;
		}
		// Run updates
		while (lag >= ApplicationConstants::OptimalTime_) {
			// Run Updates
			switch (this->m_state) {
			case ApplicationState_Depth:
				this->getKinectDepthData(this->m_kTexture->data);
				break;
			case ApplicationState_RGB:
				this->getKinectRgbData(this->m_kTexture->data);
				break;
			case ApplicationState_Gallery:
				this->m_gallery->Update(this->checkIfCrossed());
				break;
			default:
				break;
			}
			lag -= (Uint32) ApplicationConstants::OptimalTime_;
		}
		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		switch (this->m_state) {
		case ApplicationState_Depth:
			this->m_kTexture->Render(elapsed);
			break;
		case ApplicationState_RGB:
			this->m_kTexture->Render(elapsed);
			break;
		case ApplicationState_Gallery:
			this->m_gallery->Render(elapsed);
			break;
		default:
			break;
		}
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
	
*/
void Application::getKinectDepthData(GLubyte* dest) {
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (this->m_sensor->NuiImageStreamGetNextFrame(this->m_depthStream, 0, &imageFrame) < 0) {
		//std::cout << "Not collecting.." << std::endl;
		return;
	}
	//std::cout << " collecting.." << std::endl;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0)
	{
		const USHORT* curr = (const USHORT*)LockedRect.pBits;
		const USHORT* dataEnd = curr + (ApplicationConstants::DepthWidth_ * ApplicationConstants::DepthHeight_);

		while (curr < dataEnd) {
			// Get depth in millimeters
			USHORT depth = NuiDepthPixelToDepth(*curr++);

			// Draw a grayscale image of the depth:
			// B,G,R are all set to depth%256, alpha set to 1.
			for (int i = 0; i < 3; ++i)
				*dest++ = (BYTE)depth % 256;
			*dest++ = 0xff;
		}
	}
	texture->UnlockRect(0);
	this->m_sensor->NuiImageStreamReleaseFrame(this->m_depthStream, &imageFrame);
}



/*
	
*/
void Application::getKinectRgbData(GLubyte* dest) {
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (this->m_sensor->NuiImageStreamGetNextFrame(this->m_rgbStream, 0, &imageFrame) < 0) {
		std::cout << "Not collecting.." << std::endl;
		return;
	}
	std::cout << " collecting.." << std::endl;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0)
	{
		const BYTE* curr = (const BYTE*)LockedRect.pBits;
		const BYTE* dataEnd = curr + (this->m_width * this->m_height) * 4;

		while (curr < dataEnd) {
			*dest++ = *curr++;
		}
	}
	texture->UnlockRect(0);
	this->m_sensor->NuiImageStreamReleaseFrame(this->m_rgbStream, &imageFrame);
}

/*

*/
CrossedState Application::checkIfCrossed() {
	CrossedState result = CrossedState_False;
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (this->m_sensor->NuiImageStreamGetNextFrame(this->m_depthStream, 0, &imageFrame) < 0) {
		return CrossedState_Invalid;
	}
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0)
	{
		const USHORT* curr = (const USHORT*)LockedRect.pBits;
		const USHORT* dataEnd = curr + (ApplicationConstants::DepthWidth_ * ApplicationConstants::DepthHeight_);

		int index = 0;
		while (curr < dataEnd) {
			int x = index % ApplicationConstants::DepthWidth_;
			int y = floor(index / ApplicationConstants::DepthHeight_);
			if (x > ApplicationConstants::DepthWidth_ / 4.f && x < ApplicationConstants::DepthWidth_ * (3.f / 4.f)) {
				if (y > (ApplicationConstants::DepthHeight_ / 4.f) && y < ApplicationConstants::DepthHeight_ * (3.f / 4.f)) {
					// Get depth in millimeters
					USHORT depth = NuiDepthPixelToDepth(*curr);

					if (depth < ApplicationConstants::MaxDepth_ && depth > 0) {
						result = CrossedState_True;
						break;
					}
				}
			}
			*curr++;
			index++;
		}
	}
	texture->UnlockRect(0);
	this->m_sensor->NuiImageStreamReleaseFrame(this->m_depthStream, &imageFrame);
	return result;
}