/*
 * CameraManager.h
 *
 *  Created on: Oct. 10, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file CameraManager.h
 * \brief The header file for the CameraManager class.
**/

#ifndef CAMERAMANAGER_H_
#define CAMERAMANAGER_H_

#include <opencv2/opencv.hpp>
#include "CameraSpecs.h"

/**
\const N_RES
\brief The number of available resolutions.
**/
#define N_RES 13

/**
\const MENU_SIZE
\brief The number of available resolutions.
**/
#define MENU_SIZE 4

/**
\class CameraManager
\brief CameraManager is a class that manages video capture devices.
**/
class CameraManager{
public:
	/**
	\fn CameraManager()
	\brief CameraManager constructor. Assigns camera device to VideoCapture object and initializes attributes.
	**/
	CameraManager();

	/**
	\fn calculateFps()
	\brief Calculates framerate for every available resolution. Adapted from Derek Molloy's boneCVtimings.cpp
	\returns -1 if operation fails and 0 if it succeeds.
	**/
	int calculateFps();

	/**
	\fn getMenuInfo()
	\brief Gets the address of the _menuInfo attribute.
	\returns the pointer to the _menuInfo attribute.
	**/
	CameraSpecs* getMenuInfo();

	/**
	\fn selectResolution()
	\brief Displays available resolutions and prompts user to select one.
	**/
	void selectResolution();

	/**
	\fn setResolution(int width, int height)
	\brief Sets resolution width*height and captures two frames to account for initialisation time. Adapted from Derek Molloy's boneCVtimings.cpp
	\param width The desired width.
	\param height The desired height.
	\returns -1 if operation fails and 0 if it succeeds.
	**/
	int setResolution(int width, int height);

	/**
	\fn setResolution(int resIndex)
	\brief Sets the resolution and captures two frames to account for initialisation time.
	\param resIndex The index of the desired resolution (1-4)
	\returns -1 if operation fails and 0 if it succeeds.
	**/
	int setResolution(int resIndex);

	/**
	\fn currentRes()
	\brief Gets the current resolution index.
	\returns the index of the current resolution (1-4)
	**/
	int currentRes();

	/**
	\fn captureImage()
	\brief Captures a single frame.
	\returns the captured image.
	**/
	cv::Mat captureImage();

	/**
	\fn captureVideo(int duration)
	\brief Captures a video in avi format
	\param totalDuration The duration of the video in seconds.
	**/
	void captureVideo(double totalDuration);

private:
	/**
	\var _width[N_RES];
	\brief An array containing all possible predetermined widths.
	**/
	static const int _width[N_RES];

	/**
	\var _height[N_RES];
	\brief An array containing all possible pretedermined heights.
	**/
	static const int _height[N_RES];

	/**
	\var _validIndices[MENU_SIZE]
	\brief An array containing the available indices for _specs.
	**/
	static const int _validIndices[MENU_SIZE];

	/**
	\var _specs[N_RES]
	\brief An array containing all configurations of resolution and framerate.
	**/
	CameraSpecs _specs[N_RES];

	/**
	\var _specIndex
	\brief The index of the resolution selected by the user (0-12).
	**/
	int _specIndex;

	/**
	\var _specIndex
	\brief The index of the available resolution selected by the user (0-3).
	**/
	int _resIndex;

	/**
	\var _menuInfo[MENU_SIZE]
	\brief The menu containing the 4 available resolutions.
	**/
	CameraSpecs _menuInfo[MENU_SIZE];

	/**
	\var _capture
	\brief A VideoCapture object to grab frames.
	**/
	cv::VideoCapture _capture;

};



#endif /* CAMERAMANAGER_H_ */
