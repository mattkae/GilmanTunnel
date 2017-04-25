#include "stdafx.h"
#include "Gallery.h"
#include "Texture.h"
#include "SwipeGesture.h"
#include <dirent.h>
#include <iostream>

const int DefaultNodes_ = 16;

Gallery::Gallery() {
	this->m_isActive = false;
	this->m_isEnding = false;
	this->m_currentIndex = 0;
	this->m_before = CrossedState_False;
	this->m_current = CrossedState_False;
	this->LoadImages();
	this->m_swipeGesture = new SwipeGesture();
	this->m_swipeGesture->SetNodes(DefaultNodes_);
}


void Gallery::LoadImages() {
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("assets/images/test/")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			std::string name = ent->d_name;
			if (!(name.compare(".") == 0 || name.compare("..") == 0)) {
				Texture* texture = new Texture();
				if (texture->LoadTexture((char*)("assets/images/test/" + name).c_str())) {
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

Gallery::~Gallery() {
	for (auto it = this->m_textures.begin(); it !=  this->m_textures.end(); ++it) {
		delete *it;
	}
}

void Gallery::Update(CrossedState state) {
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
		if (this->m_elapsed >= ApplicationConstants::ProjectionTime_ || this->m_swipeGesture->Update()) {
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


void Gallery::Render(unsigned int elapsed) {
	if (this->m_isActive || this->m_isEnding) {
		this->m_textures.at(this->m_currentIndex)->Render(elapsed);
		this->m_elapsed += elapsed;
	}
}


USHORT* Gallery::GetData() {
	return this->m_swipeGesture->DepthData;
}