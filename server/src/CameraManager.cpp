/*
 * CameraManager.cpp
 *
 *  Created on: Oct. 10, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file CameraManager.cpp
 * \brief The source file for the CameraManager class.
**/

#include <time.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "CameraManager.h"

using namespace std;
using namespace cv;

const int CameraManager::_width[N_RES] = {176, 160, 320, 320, 352, 432, 800, 864, 960, 960, 1184, 1280, 1280};
const int CameraManager::_height[N_RES] = {144, 120, 176, 240, 288, 240, 600, 480, 544, 720, 656, 720, 960};
const int CameraManager::_validIndices[MENU_SIZE] = {0, 1, 2, 3};

CameraManager::CameraManager(){
	cout << "Initializing camera..." << endl;

	_capture.open(0);

	for (int i = 0; i < N_RES; i++){
		_specs[i].resX = _width[i];
		_specs[i].resY = _height[i];
	}
	for (int i = 0; i < MENU_SIZE; i++)
		_menuInfo[i]=_specs[_validIndices[i]];

	_specIndex = _validIndices[0];
	setResolution(_specIndex);

	cout << "Done!" << endl;
}

// Adapted from boneCVtimings.cpp by Derek Molloy
int CameraManager::calculateFps(){
    for (int n = 0; n < N_RES; n++){
		if (setResolution(_width[n], _height[n]) == -1)
			return -1;

		Mat frame;
		struct timespec start, end;
		clock_gettime( CLOCK_REALTIME, &start );

		int frames=10;
		for(int i=0; i<frames; i++){
			_capture >> frame;
			if(frame.empty()){
				cout << "Failed to _capture an image" << endl;
				return -1;
			}
		}

		clock_gettime( CLOCK_REALTIME, &end );
		double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0d;
		cout << "Calculating FPS " << n+1 << "/" << N_RES << endl;

		// Save fps for each resolution
		_specs[n].fps = frames/difference;
    }

    return 0;
}

CameraSpecs* CameraManager::getMenuInfo(){
	return _menuInfo;
}

void CameraManager::selectResolution(){

	// Print available resolutions
	cout << "Available resolutions: " << endl;

	for (int i = 0; i < N_RES; i++){
		printf("%2i: %4i x %i \n",i, _specs[i].resX, _specs[i].resY);
	}

	cout << endl << "Select resolution index (0-12) and press <Enter>: ";
	cin >> _specIndex;

	// Set resolution of capture device
	setResolution(_specs[_specIndex].resX,_specs[_specIndex].resY);
}

int CameraManager::setResolution(int width, int height){
	_capture.set(CV_CAP_PROP_FRAME_WIDTH,width);
	_capture.set(CV_CAP_PROP_FRAME_HEIGHT,height);
	if(!_capture.isOpened()){
		cout << "Failed to connect to the camera." << endl;
		return -1;
	}

	Mat frame;

	// Capture 2 frames to exclude init time from fps measurement
	for(int i=0; i<2; i++){
		_capture >> frame;
		if(frame.empty()){
			cout << "Failed to _capture an image" << endl;
			return -1;
		}
	}

	return 0;
}

int CameraManager::setResolution(int resIndex){
	_resIndex = resIndex;
	_specIndex = _validIndices[_resIndex];
	return setResolution(_specs[_specIndex].resX, _specs[_specIndex].resY);
}

int CameraManager::currentRes(){
	return _resIndex;
}

Mat CameraManager::captureImage(){
	Mat frame;
	_capture >> frame;

	return frame;
}

void CameraManager::captureVideo(double totalDuration){
	//int format = VideoWriter::fourcc('M', 'J', 'P', 'G');

	VideoWriter video("capture-liv1.avi",
			CV_FOURCC('M', 'J', 'P', 'G'),
			_specs[_specIndex].fps,
			Size(_specs[_specIndex].resX,_specs[_specIndex].resY));

	Mat frame;

	struct timespec start, end;
	double captureDuration = 0.0;
	int nFrames = 0;

	clock_gettime( CLOCK_REALTIME, &start );

	do{
		_capture >> frame;

		if(frame.empty()){
			cout << "Failed to _capture an image" << endl;
			break;
		}

		video.write(frame);
		nFrames++;

		clock_gettime( CLOCK_REALTIME, &end );
		captureDuration = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0d;
	} while (captureDuration < totalDuration);

	cout << nFrames << " frames captured in " << captureDuration << " seconds." << endl;
	video.release();
}
