#include "stdafx.h"
#include "SwipeGesture.h"
#include "ApplicationConstants.h"
#include <iostream>

const int Closeness_ = 1200;
const int WaitTime_ = 3 * ApplicationConstants::Second_;

SwipeGesture::SwipeGesture() {
	this->m_head = nullptr;
	this->m_current = this->m_head;
	this->DepthData = new USHORT[ApplicationConstants::DepthWidth_ * ApplicationConstants::DepthHeight_];
}

void SwipeGesture::SetNodes(int numNodes, int yOffset) {
	SwipeNode* curr = this->m_head;
	for (int nodeIndex = numNodes - 1; nodeIndex >= 0; nodeIndex--) {
		SwipeNode* newNode = new SwipeNode;
		newNode->w = ApplicationConstants::DepthWidth_ / numNodes;
		newNode->h = ApplicationConstants::DepthHeight_ * PercentOfScreen_;
		newNode->x = newNode->w * nodeIndex;
		newNode->y = yOffset;
		newNode->elapsed = 0;
		newNode->next = this->m_head;
		this->m_head = newNode;
	}
}

bool SwipeGesture::Update() {
	if (this->m_current) {
		this->m_current->elapsed += ApplicationConstants::OptimalTime_;
		// Return if we didn't update in time
		if (this->m_current->elapsed > WaitTime_) {
			this->m_current->elapsed = 0;
			this->m_current = this->m_head;
			//std::cout << "False over time" << std::endl;
			return false;
		}

		// Check if there's a hit
		for (int y = this->m_current->y; y < this->m_current->h; y++) {
			for (int x = this->m_current->x; x < this->m_current->x + this->m_current->w; x++) {
				int index = y * this->m_current->w + x;

				if (this->DepthData[index] < Closeness_ && this->DepthData[index] > 0) {
					this->m_current = this->m_current->next;
					if (this->m_current)
						this->m_current->elapsed = 0;
					goto end;
				}
			}
		}

	}
	
end:
	// Return true if there are no other nodes to hit
	if (!this->m_current) {
		//std::cout << "True" << std::endl;
		this->m_current = this->m_head;
		return true;
	}

	//std::cout << "False not end " << this->m_current->x << std::endl;
	return false;
}