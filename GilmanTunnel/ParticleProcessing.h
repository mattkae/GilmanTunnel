#pragma once

#include "ApplicationConstants.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

const ushort LowestDepth_ = 1250;
int* depthIndices;
byte* intermediateSrc;

void InitializeParticleProcessing(int w = ApplicationConstants::DepthWidth_, int h = ApplicationConstants::DepthHeight_) {
	intermediateSrc = new byte[w * h * 4];
	depthIndices = new int[w * h * 4];
	for (int i = 0; i < w * h * 4; i++) {
		depthIndices[i] = ceil(i / 4);
	}
}

void ProcessRGBData(byte* src, ushort* depth, byte* dest, int w, int h) {
	if (!src || !depth || !dest)
		return;

	// Copy original image
	Mat matrixImageRGBA(w, h, CV_8UC4);
	memcpy(matrixImageRGBA.data, src, w * h * 4);
	Mat matrixImage;
	cvtColor(matrixImageRGBA, matrixImage, CV_RGBA2RGB);

	// Filter RGB values at depth less than LowestDepth_
	for (int i = 0; i < w * h * 4; i++) {
		intermediateSrc[i] = depth[depthIndices[i]] <= LowestDepth_ && depth[depthIndices[i]] > 0 ? 255 : 0;
	}

	// Copy src data
	Mat matrixData(w, h, CV_8UC4);
	memcpy(matrixData.data, intermediateSrc, w * h * 4);

	// Convert to binary image
	Mat matrixDataGray(w, h, CV_8UC1);
	cvtColor(matrixData, matrixDataGray, CV_RGBA2GRAY);
	Mat matrixBinary;
	threshold(matrixDataGray, matrixBinary, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	// Find contours
	vector<Mat> contours;
	vector<Vec4i> hierarchy;
	std::cout << "Finding contours" << std::endl;
	findContours(matrixBinary, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);

	if (contours.empty()) {
		return;
	}

	if (hierarchy.empty()) {
		return;
	}

	// Create markers
	Mat markers = Mat::zeros(matrixBinary.size(), CV_32S);

	// Draw foreground markers
	int idx = 0;
	int compCount = 0;
	std::cout << "Drawing contours" << std::endl;
	for (; idx >= 0; idx = hierarchy[idx][0], compCount++) {
		drawContours(markers, contours, idx, Scalar::all(compCount + 1), -1, 8, hierarchy, INT_MAX);
	}

	if (compCount == 0) {
		return;
	}

	circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);

	std::cout << "Getting Colors" << std::endl;

	vector<Vec4b> colorTab;
	for (int i = 0; i < compCount; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);
		int a = 255;
		colorTab.push_back(Vec4b((uchar)b, (uchar)g, (uchar)r, (uchar) a));
	}


	std::cout << "Creating image" << std::endl;
	// Create the final image
	Mat dst = Mat::zeros(markers.size(), CV_8UC4);
	// Fill labeled objects with random colors
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
				dst.at<Vec4b>(i, j) = colorTab[index];
			else
				dst.at<Vec4b>(i, j) = Vec4b(0, 0, 0, 0);
		}
	}

	std::cout << "Copying" << std::endl;
	// Copy data to destination array
	if (dst.isContinuous()) {
		memcpy(dest, dst.data, w * h * 4);
	}

}

void CleanParticleProcessing() {
	delete intermediateSrc;
	if (depthIndices)
		delete depthIndices;
}