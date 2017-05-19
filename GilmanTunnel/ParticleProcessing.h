#pragma once

#include "ApplicationConstants.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

const ushort LowestDepth_ = 2500;
const int MinimumArea_ = 1500;
byte* intermediateSrc;

/*
	Allocate the memory necessary for "Particle Highway."
*/
void InitializeParticleProcessing(int w = ApplicationConstants::DepthWidth_, int h = ApplicationConstants::DepthHeight_) {
	intermediateSrc = new byte[w * h];
}

/*
	Process the data for Particle Highway.

	@param src Kinect's RGB data stream
	@param depth Kinect's depth stream
	@param dest where the new image is written to
	@param w the width of the data array
	@param h the height of the data array
*/
void ProcessRGBData(byte* src, ushort* depth, byte* dest, int w, int h) {
	if (!src || !depth || !dest)
		return;

	// Copy original image
	Mat matrixImageRGBA(Size(w, h), CV_8UC4);
	matrixImageRGBA.data = src;

	// Filter RGB values at depth less than LowestDepth_
	for (int i = 0; i < w * h; i++) {
		intermediateSrc[i] = depth[i] < LowestDepth_ && depth[i] > 0 ? 255 : 0;
	}

	Mat matrixData(Size(w, h), CV_8UC1);
	matrixData.data = intermediateSrc;

	int morph_size = 2;
	Mat closed;
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
	morphologyEx(matrixData, closed, MORPH_CLOSE, element, Point(morph_size, morph_size));

	// Find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(closed, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	if (contours.empty())
		return;

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
	}

	// Draw contours
	Mat markers = Mat::zeros(matrixData.size(), CV_8UC3);
	int rectangleCount = 0;
	for (int i = 0; i < contours.size(); i++) {
		if (hierarchy[i][0] != -1 && contourArea(contours[i]) > MinimumArea_) {
			Scalar colour((rand() & 255), (rand() & 255), (rand() & 255));
			drawContours(markers, contours, i, colour);
			//rectangle(markers, boundRect[i].tl(), boundRect[i].br(), colour, 2, 8, 0);
			circle(markers, center[i], (int)radius[i], colour, -1, 8, 0);
			rectangleCount++;
		}
	}

	cout << rectangleCount << endl;

	Mat dst(w, h, CV_8UC4);
	cvtColor(markers, dst, CV_RGB2RGBA);

	dst = dst * 0.5 + matrixImageRGBA * 0.5;

	// Copy data to destination array
	if (dst.isContinuous()) {
		memcpy(dest, dst.data, w * h * 4);
	}

}

/*
	Deallocate memory.
*/
void CleanParticleProcessing() {
	delete intermediateSrc;
}