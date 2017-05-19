#include "stdafx.h"
#include "Gallery.h"
#include "Texture.h"
#include "SwipeGesture.h"
#include <dirent.h>
#include <iostream>

// Number of nodes to swipe.
const int DefaultNodes_ = 16;

/*
	Default constructor for the gallery.
*/
Gallery::Gallery()
{
	this->m_isActive = false;
	this->m_isEnding = false;
	this->m_currentIndex = 0;
	this->m_before = CrossedState_False;
	this->m_current = CrossedState_False;
	this->LoadImages();
	this->m_swipeGesture = new SwipeGesture();
	this->m_swipeGesture->SetNodes(DefaultNodes_);
}

/*
	Loads every image from the "assets/images" directory into the
	gallery as a new texture.
*/
void Gallery::LoadImages()
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("assets/images/")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			std::string name = ent->d_name;
			if (!(name.compare(".") == 0 || name.compare("..") == 0)) {
				Texture* texture = new Texture();
				if (texture->LoadTexture((char*)("assets/images/" + name).c_str())) {
					this->m_textures.push_back(texture);
				} else {
					delete texture;
				}
			}
		}
		closedir(dir);
	} else {
		return;
	}
}

/*
	Deallocate memory for the gallery.
*/
Gallery::~Gallery()
{
	for (auto it = this->m_textures.begin(); it !=  this->m_textures.end(); ++it) {
		delete *it;
	}
}

/*
	Update the currently shown image in the gallery if someone is in front of the Kinect.

	@param state the current state of the depth data.
*/
void Gallery::Update(CrossedState state) 
{
	this->m_current = state;

	bool before = this->m_isActive; // Store before state
	this->m_isActive = (this->m_current != CrossedState_False) && (this->m_before == CrossedState_True); // Store current state

	// End projection
	if (before == true && this->m_isActive == false) {
		int e = this->m_textures.at(this->m_currentIndex)->GetElapsed();
		if (e < ApplicationConstants::FadeInTime_)
			this->m_textures.at(this->m_currentIndex)->SetElapsed(ApplicationConstants::ProjectionTime_ - e);
		else if (e > ApplicationConstants::FadeOutTime_)
			this->m_textures.at(this->m_currentIndex)->SetElapsed(e);
		else
			this->m_textures.at(this->m_currentIndex)->SetElapsed(ApplicationConstants::FadeOutTime_);

		this->m_isEnding = true;
	}

	this->m_elapsed = (this->m_current == CrossedState_True && this->m_before != CrossedState_True) ? 0 : this->m_elapsed;

	if (m_isActive) {
		// Update image if it exceeds projection time
		if (this->m_elapsed >= ApplicationConstants::ProjectionTime_) {
			this->m_textures.at(this->m_currentIndex)->Reset();
			this->m_currentIndex = (this->m_currentIndex + 1) % this->m_textures.size();
			this->m_elapsed = 0;
		}
	} else if (m_isEnding) {
		// Check if we've elapsed the proper time, and the end
		if (this->m_textures.at(this->m_currentIndex)->GetElapsed() >= ApplicationConstants::ProjectionTime_) {
			this->m_textures.at(this->m_currentIndex)->Reset();
			this->m_currentIndex = (this->m_currentIndex + 1) % this->m_textures.size();
			this->m_elapsed = 0;
			this->m_isEnding = false;
		}
	}
	
	// Set the previous state to the current state if valid
	if (this->m_current != CrossedState_Invalid)
		this->m_before = this->m_current;
}

/*
	Render the currently active texture.

	@param elapsed time elapsed since last render
*/
void Gallery::Render(unsigned int elapsed)
{
	if (this->m_isActive || this->m_isEnding) {
		this->m_textures.at(this->m_currentIndex)->Render(elapsed);
		this->m_elapsed += elapsed;
	}
}

/*
	Returns the swipe gesture's data.

	@return swipe gesture data.
*/
USHORT* Gallery::GetData() 
{
	return this->m_swipeGesture->DepthData;
}