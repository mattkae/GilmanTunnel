#include "stdafx.h"
#include "SwipeGesture.h"
#include "ApplicationConstants.h"
#include <iostream>

const int Closeness_ = 1200;
const int WaitTime_ = 3 * ApplicationConstants::Second_;

/*
	Default constructor
*/
SwipeGesture::SwipeGesture() {
	this->m_head = nullptr;
	this->m_current = this->m_head;
	this->DepthData = new USHORT[ApplicationConstants::DepthWidth_ * ApplicationConstants::DepthHeight_];
}

/*
	Constructor specifying number of nodes and yOffset in screen.

	@param numNodes specifies how many squares to use horizontally across the screen.
	@param yoffset specifies the upper y coordinate of each of those nodes
*/
void SwipeGesture::SetNodes(int numNodes, int yOffset) {
	SwipeNode* curr = this->m_head;
	for (int nodeIndex = numNodes - 1; nodeIndex >= 0; nodeIndex--) {
		SwipeNode* newNode = new SwipeNode;
		newNode->w = ApplicationConstants::DepthWidth_ / numNodes;
		newNode->h = (int) ApplicationConstants::DepthHeight_ * PercentOfScreen_;
		newNode->x = newNode->w * nodeIndex;
		newNode->y = yOffset;
		newNode->elapsed = 0;
		newNode->next = this->m_head;
		this->m_head = newNode;
	}
}

/*
	Default destructor.
*/
SwipeGesture::~SwipeGesture() {
	SwipeNode* curr = this->m_head;
	while (this->m_head) {
		curr = this->m_head->next;
		delete this->m_head;
		this->m_head = nullptr;
		this->m_head = curr;
		curr = this->m_head;
	}
}

/*
	Updates the current state of the swipe feature. A "swipe" as it is
	defined here moves from the leftmost node to the rightmost node, and
	must be in order, within a certain time interval for the swipe to
	be accepted.

	@return whether or not all nodes have been swiped.
*/
bool SwipeGesture::Update() {
	if (this->m_current) {
		this->m_current->elapsed += (unsigned int)ApplicationConstants::OptimalTime_;
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