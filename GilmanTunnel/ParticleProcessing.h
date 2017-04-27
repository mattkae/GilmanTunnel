#pragma once

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

int edgeThresh = 1;
int lowThreshold = 5;
int const max_lowThreshold = 10;
int ratio = 3;
int kernel_size = 3;

void DetectEdges(byte* src, byte* dest, int w, int h) {
	if (!src)
		return;

	// Copy src data
	Mat matrixData(w, h, CV_8UC4);
	memcpy(matrixData.data, src, w * h * 4);

	// Gray-scale src data
	Mat matrixDataGray(w, h, CV_8UC1);
	cvtColor(matrixData, matrixDataGray, CV_RGBA2GRAY);

	// Calculate sobekl
	Mat matrixSobelX;
	Sobel(matrixDataGray, matrixSobelX, CV_32F, 1, 0, 5);
	double minVal, maxVal;
	minMaxLoc(matrixSobelX, &minVal, &maxVal); //find minimum and maximum intensities

	Mat matrixAlmost;
	matrixSobelX.convertTo(matrixAlmost, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));


	// Blur gray-scale data
	//Mat detectedEdges(w, h, CV_8UC4);
	//blur(matrixDataGray, detectedEdges, Size(3, 3));

	// Apply canny filter
	//Canny(detectedEdges, detectedEdges, lowThreshold, lowThreshold*ratio, kernel_size);

	// Write data to destination
	Mat matrixDest(w, h, CV_8UC4);
	matrixData.copyTo(matrixDest, matrixAlmost);

	// Copy data to destination array
	if (matrixDest.isContinuous()) {
		memcpy(dest, matrixDest.data, w * h * 4);
	}
}