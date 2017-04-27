#pragma once

#include "ApplicationConstants.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

const ushort LowestDepth_ = 2000;

void ProcessRGBData(byte* src, ushort* depth, byte* dest, int w, int h) {
	if (!src || !depth || !dest)
		return;

	// Copy original image
	Mat matrixImageRGBA(w, h, CV_8UC4);
	memcpy(matrixImageRGBA.data, src, w * h * 4);
	Mat matrixImage;
	cvtColor(matrixImageRGBA, matrixImage, CV_RGBA2RGB);

	// Filter RGB values at depth less than LowestDepth_
	for (int i = 0; i < ApplicationConstants::DepthWidth_ * ApplicationConstants::DepthHeight_ * 4; i++) {
		src[i] = depth[i / 4] <= LowestDepth_ && depth[i / 4] > 0 ? src[i] : 0;
	}

	// Copy src data
	Mat matrixData(w, h, CV_8UC4);
	memcpy(matrixData.data, src, w * h * 4);

	// Gray-scale src data
	Mat matrixDataGray(w, h, CV_8UC4);
	cvtColor(matrixData, matrixDataGray, CV_RGBA2GRAY);
	Mat matrixThreshold;
	threshold(matrixDataGray, matrixThreshold, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);

	// Remove noise
	Mat kernel;
	kernel = Mat::ones(Size(3, 3), CV_8U);
	Mat matrixMorph;
	morphologyEx(matrixThreshold, matrixMorph, MORPH_OPEN, kernel, Point(0, 0), 2);

	// Background area
	Mat matrixBackground;
	dilate(matrixMorph, matrixBackground, kernel, Point(0, 0), 3);

	// Foreground area
	Mat matrixDistanceTransform, matrixForeground;
	distanceTransform(matrixMorph, matrixDistanceTransform, DIST_L2, 5);
	double min, max;
	minMaxLoc(matrixDistanceTransform, &min, &max);
	threshold(matrixDistanceTransform, matrixForeground, 0.7*max, 255, 0);
	Mat matrixForeground8U;
	matrixForeground.convertTo(matrixForeground8U, CV_8U);

	// Markers and set final image
	Mat markers;
	connectedComponents(matrixForeground8U, markers, 8, CV_32S);
	markers = markers + 1;
	watershed(matrixImage, markers);
	for (int i = 0; i < ApplicationConstants::DepthWidth_ * ApplicationConstants::DepthHeight_ * 3; i+=3) {
		if (markers.data[i / 3] == -1) {
			matrixImage.data[i] = 255;
			matrixImage.data[i + 1] = 0;
			matrixImage.data[i + 2] = 0;
		}
	}

	Mat matrixDest;
	cvtColor(matrixImage, matrixDest, CV_RGB2RGBA);

	// Copy data to destination array
	if (matrixDest.isContinuous()) {
		memcpy(dest, matrixDest.data, w * h  * 4);
	}
}