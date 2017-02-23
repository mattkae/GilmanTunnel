#pragma once
#include <SDL_opengl.h>

struct SDL_Window;

// Window Constants

class Window {
public:
	Window(GLuint width, GLuint height, const GLchar* name = "Gilman Tunnel");
	~Window();
	void Free();
	// Getters
	GLuint GetWidth();
	GLuint GetHeight();
private:
	SDL_Window* m_window;		// SDL Window context
	GLuint m_width;				// Width of window
	GLuint m_height;			// Heigh of window
};