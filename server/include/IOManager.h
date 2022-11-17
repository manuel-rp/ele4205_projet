/*
 * IOManager.h
 *
 *  Created on: Nov. 11, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file IOManager.h
 * \brief The header file for the IOManager class.
**/

#ifndef SRC_IOMANAGER_H_
#define SRC_IOMANAGER_H_

#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

/**
\const LIGHT_THRESHOLD
\brief The ADC value above which the light is not sufficient for capture.
**/
#define LIGHT_THRESHOLD 985

/**
\const READY
\brief The identifier for the READY state.
**/
#define READY 0b00

/**
\const IDOWN
\brief The identifier for the IDOWN state.
**/
#define IDOWN 0b11

/**
\const PUSHB
\brief The identifier for the PUSHB state.
**/
#define PUSHB 0b01

/**
\class IOManager
\brief IOManager is a class that manages inputs to the server (button and ADC).
**/
class IOManager {
public:
	/**
	\fn IOManager()
	\brief  IOManager constructor. Initializes attributes.
	**/
	IOManager();
	virtual ~IOManager();

	/**
	\fn getADCValue()
	\brief Reads the value from ADC0.
	\returns the value of the ADC
	**/
	uint32_t getADCValue();

	/**
	\fn isButtonPressed()
	\brief Reads the value of GPIO pin 228 and inverts it.
	\returns true if the button is pressed and false if it isn't.
	**/
	bool isButtonPressed();

	/**
	\fn updatePreviousButtonState()
	\brief Sets the _buttonWasPressed variable to its new value.
	**/
	void updatePreviousButtonState();

	/**
	\fn getSystemState()
	\brief Checks the ADC and button to determine system state.
	\returns the current state of the system.
	**/
	uint32_t getSystemState();

private:
	/**
	\var _adcPath
	\brief The path to the file where ADC value is stored.
	**/
	std::string _adcPath;

	/**
	\var _buttonPath
	\brief The path to the file where GPIO228 (button) value is stored.
	**/
	std::string _buttonPath;

	/**
	\var _buttonWasPressed
	\brief The state of the button at the previous loop.
	**/
	bool _buttonWasPressed;
};

#endif /* SRC_IOMANAGER_H_ */
