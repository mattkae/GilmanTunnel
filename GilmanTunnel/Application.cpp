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
#include <Kinect.h>
#include <iostream>

using namespace std; // For error reporting.

/**
	Default constructor to initialize the application instance.
*/
Application::Application(unsigned int width, unsigned int height) 
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cerr << "APPLICATION::SDL_INIT:: " << SDL_GetError() << endl;
		exit(EXIT_FAILURE);
	}

	// Setup OpenGL
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glEnable(GL_TEXTURE_2D);
	// Camera setup
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	this->m_window = new Window(width, height);
	this->m_context = this->m_window->SetContext();
	this->m_running = initializeKinect();
}

/*
	Default destructor.
*/
Application::~Application() 
{
	SDL_GL_DeleteContext(this->m_context);

	if (this->m_sensor) {
		this->m_sensor->Close();
		this->m_sensor = NULL;
	}

	// Free window memory
	if (this->m_window) {
		this->m_window->Free();
		delete this->m_window;
	}
	
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

	@return succesful initialization.
*/
bool Application::initializeKinect()
{
	if (FAILED(GetDefaultKinectSensor(&this->m_sensor))) {
		cerr << "ERROR::INITIALIZE_KINECT:: Unable to initialize sensor" << endl;
		return false;
	}

	if (this->m_sensor) {
		this->m_sensor->Open();

		IColorFrameSource* frameSource = NULL;
		this->m_sensor->get_ColorFrameSource(&frameSource);
		frameSource->OpenReader(&this->m_reader);
		if (frameSource) {
			frameSource->Release();
			frameSource = NULL;
		}
		return true;
	}

	cerr << "ERROR::INITIAILIZE_KINECT:: m_sensor was null" << endl;
	return false;
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
			}
		}

		while (lag >= ApplicationConstants::OptimalTime_) {
			// Run Updates

			lag -= (Uint32) ApplicationConstants::OptimalTime_;
		}

		// Render...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void Application::getKinectData(Texture* texture) {
	IColorFrame* frame = NULL;
	if (SUCCEEDED(this->m_reader->AcquireLatestFrame(&frame))) {
		frame->CopyConvertedFrameDataToArray(this->m_window->GetWidth() * this->m_window->GetHeight() * 4, texture->data, ColorImageFormat_Bgra);
	}
	if (frame) frame->Release();
}