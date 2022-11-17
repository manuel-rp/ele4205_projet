/*
 * CameraSpecs.h
 *
 *  Created on: Oct. 28, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file CameraSpecs.h
 * \brief The header file for the CameraSpecs structure.
**/

#ifndef INCLUDE_CAMERASPECS_H_
#define INCLUDE_CAMERASPECS_H_

/**
\struct CameraSpecs
\brief CameraSpecs is a structure that contains the parameters for a camera configuration.
**/
struct CameraSpecs{
	/**
	\var resX
	\brief The horizontal resolution of a configuration
	**/
	int resX;

	/**
	\var resY
	\brief The vertical resolution of a configuration
	**/
	int resY;

	/**
	\var fps
	\brief The framerate corresponding to the resolution
	**/
	double fps;
};

#endif /* INCLUDE_CAMERASPECS_H_ */
