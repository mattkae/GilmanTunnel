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

	// Filter RGB values at depth less than LowestDepth_
	for (int i = 0; i < w * h * 4; i+=4) {
		intermediateSrc[i] = depth[depthIndices[i]] <= LowestDepth_ && depth[depthIndices[i]] > 0 ? 255 : 0;
		intermediateSrc[i + 1] = depth[depthIndices[i + 1]] <= LowestDepth_ && depth[depthIndices[i + 1]] > 0 ? 255 : 0;
		intermediateSrc[i + 2] = depth[depthIndices[i + 2]] <= LowestDepth_ && depth[depthIndices[i + 2]] > 0 ? 255 : 0;
		intermediateSrc[i + 3] = 255;
	}

	// Copy src data
	Mat matrixData(w, h, CV_8UC4);
	memcpy(matrixData.data, intermediateSrc, w * h * 4);
	Mat matrixDataBGR;
	cvtColor(matrixData, matrixDataBGR, CV_RGBA2BGR);

	int morph_size = 2;
	Mat closed;
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
	morphologyEx(matrixDataBGR, closed, MORPH_CLOSE, element);

	//  Blur and create one channel
	//Mat matrixBlur;
	//blur(matrixData, matrixBlur, Size(3, 3));
	Mat blackAndWhite;
	cvtColor(closed, blackAndWhite, CV_RGBA2GRAY);

	// Find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(blackAndWhite, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	if (contours.empty()) {
		return;
	}

	cout << "contours size:" << contours.size() << endl;
	cout << "hierarchy size:" << hierarchy.size() << endl;

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
	Mat markers = Mat::zeros(blackAndWhite.size(), CV_8UC3);
	for (int i = 0; i<contours.size(); i++) {
		if (hierarchy[i][0] != -1) {
			Scalar colour((rand() & 255), (rand() & 255), (rand() & 255));
			drawContours(markers, contours, i, colour);
			rectangle(markers, boundRect[i].tl(), boundRect[i].br(), colour, 2, 8, 0);
			circle(markers, center[i], (int)radius[i], colour, 2, 8, 0);
		}
	}


	Mat dst;
	cvtColor(markers, dst, CV_RGB2RGBA);

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



/*int idx = 0;
int compCount = 0;
for (; idx >= 0; idx = hierarchy[idx][0], compCount++) {
drawContours(markers, contours, idx, Scalar::all(compCount + 1), -1, 8, hierarchy);
}

std::cout << compCount << std::endl;

if (compCount == 0) {
return;
}

// Generate colors
vector<Vec4b> colorTab;
for (int i = 0; i < compCount; i++) {
int b = theRNG().uniform(0, 255);
int g = theRNG().uniform(0, 255);
int r = theRNG().uniform(0, 255);
int a = 255;
colorTab.push_back(Vec4b((uchar)b, (uchar)g, (uchar)r, (uchar) a));
}

// Apply watershed algorithm
watershed(srcImage, markers);

/*Create the final image
Mat dst = Mat::zeros(matrixImageRGBA.size(), CV_8UC4);
for (int i = 0; i < markers.rows; i++) {
for (int j = 0; j < markers.cols; j++) {
int index = markers.at<int>(i, j);
if (index == -1)
dst.at<Vec4b>(i, j) = Vec4b(0, 0, 0, 255);
else if (index > 0 && index <= compCount)
dst.at<Vec4b>(i, j) = colorTab[index - 1];
else
dst.at<Vec4b>(i, j) = Vec4b(0, 0, 0, 255);
}
}*/