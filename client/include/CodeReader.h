/*
 * CodeReader.h
 *
 *  Created on: Nov. 18, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file CodeReader.h
 * \brief The header file for the CodeReader class.
**/
#ifndef SRC_CODEREADER_H_
#define SRC_CODEREADER_H_

#include <opencv2/opencv.hpp>
#include "zbar.h"

/**
 * \class CodeReader
 * \brief CodeReader is a class that manages reading a QR code from a picture.
 */
class CodeReader {
public:
	CodeReader();
	virtual ~CodeReader();

	/**
	\fn decode(cv::Mat &image)
	\brief Scans an image and detects whether or not there is a code. Saves the code information. See https://www.learnopencv.com/barcode-and-qr-code-scanner-using-zbar-and-opencv/?fbclid=IwAR25vzVWTK0jD9rWJR_4ZSfAZBZB7kjOsDGANnawiHu08s7pfimFp7a-dR0 for more information.
	\param image The image in which to check for a code.
	\returns -1 if an error occurs, 0 if no code is found, and >0 if decoding is successful
	**/
	int decode(cv::Mat &image);

	/**
	\fn getCodeData()
	\brief Returns _codeData attribute
	\returns _codeData attribute
	**/
	std::string getCodeData();

private:
	/**
	\var _codeData
	\brief Stores the code data.
	**/
	std::string _codeData;
};

#endif /* SRC_CODEREADER_H_ */
