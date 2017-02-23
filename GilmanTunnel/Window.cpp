#include "stdafx.h"
#include "Window.h"
#include <SDL.h>
#include <iostream>

using namespace std;

/*
	Creates a new instance of an SDL window.

	@param width - width of window
	@param height - height of window
	@param name - title of window
*/
Window::Window(GLuint width, GLuint height, const char* name) 
{
	this->m_width = width;
	this->m_height = height;

	this->m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
}

/*
	Destroys the instance of the SDL window.
*/
Window::~Window() 
{
	this->Free();
}

/*
	Destroys the instance of the SDL window, explicitly.
*/
void Window::Free()
{
	SDL_DestroyWindow(this->m_window);
}

/*
	@return width of context
*/
GLuint Window::GetWidth() 
{
	return this->m_width;
}

/*
	@return height of context
*/
GLuint Window::GetHeight()
{
	return this->m_height;
}