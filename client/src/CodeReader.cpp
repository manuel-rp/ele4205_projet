/*
 * CodeReader.cpp
 *
 *  Created on: Nov. 18, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file CodeReader.cpp
 * \brief The source file for the CodeReader class.
**/

#include <opencv2/opencv.hpp>
#include "CodeReader.h"

using namespace std;
using namespace zbar;
using cv::Mat;

CodeReader::CodeReader() {}
CodeReader::~CodeReader() {}

//Adapted from
//https://www.learnopencv.com/barcode-and-qr-code-scanner-using-zbar-and-opencv/?fbclid=IwAR25vzVWTK0jD9rWJR_4ZSfAZBZB7kjOsDGANnawiHu08s7pfimFp7a-dR0
int CodeReader::decode(cv::Mat &image){

	ImageScanner scanner;

	// Initialize scanner
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE,1);

	// Convert image to greyscale
	Mat greyImage;
	cvtColor(image, greyImage, CV_BGR2GRAY);

	// Wrap image data in a zbar image
	Image zbarImage(image.cols, image.rows, "Y800", (uchar *)greyImage.data, image.cols*image.rows);

	int flag = scanner.scan(zbarImage);
	switch (flag){
		case -1: // Error
			cout << "An error occured with image scan." << endl;
			break;
		case 0: // No code
			cout << "No code was found in image." << endl;
			break;
		default: // Code found
			cout << "A code was detected!" << endl;

			Image::SymbolIterator symbol = zbarImage.symbol_begin();

			_codeData = symbol->get_data();
	}
	return flag;
}

string CodeReader::getCodeData(){
	return _codeData;
}


