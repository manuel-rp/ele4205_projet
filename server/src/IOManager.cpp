/*
 * IOManager.cpp
 *
 *  Created on: Nov. 11, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file IOManager.cpp
 * \brief The source file for the IOManager class.
**/

#include "IOManager.h"

using namespace std;

IOManager::IOManager() {
	 _adcPath = "/sys/class/saradc/ch0";
	 _buttonPath = "/sys/class/gpio/gpio228/value";
	 _buttonWasPressed = 0;
}

IOManager::~IOManager() {
}

uint32_t IOManager::getADCValue(){
	uint32_t value = 0;
	ifstream file(_adcPath.c_str(), std::ifstream::in);

	if (file.fail()) {
        cout << "Failed to open "+_adcPath << std::endl;
		exit(-1);
	}

	file >> value;
	file.close();
	return value;
}

bool IOManager::isButtonPressed(){
	bool value = 0;
	ifstream file(_buttonPath.c_str(), std::ifstream::in);

	if (file.fail()) {
        cout << "Failed to open "+_buttonPath << std::endl;
		exit(-1);
	}

	file >> value;
	file.close();
	return !value;
}

void IOManager::updatePreviousButtonState(){
	_buttonWasPressed = isButtonPressed();
}

uint32_t IOManager::getSystemState(){
	uint32_t ADCValue = getADCValue();
	if (ADCValue < LIGHT_THRESHOLD){
		if (isButtonPressed() && !_buttonWasPressed){
			updatePreviousButtonState();
			return PUSHB;
		}
		else{
			updatePreviousButtonState();
			return READY;
		}
	}
	else // No light
		return IDOWN;
}
