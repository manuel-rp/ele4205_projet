/*
 * Buzzer.cpp
 *
 *  Created on: Nov. 24, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file Buzzer.cpp
 * \brief The source file for the Buzzer class.
**/

#include <unistd.h>
#include <fstream>
#include "Buzzer.h"

using std::string;
using std::ofstream;

const string Buzzer::CHAR_TO_MORSE[128] = {
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "", "-.-.--", ".-..-.", "", "", "", "", ".----.",
        "-.--.", "-.--.-", "", "", "--..--", "-....-", ".-.-.-", "-..-.",
        "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...",
        "---..", "----.", "---...", "", "", "-...-", "", "..--..",
        ".--.-.", ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
        "....", "..", ".---", "-.-", ".-..", "--", "-.", "---",
        ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--",
        "-..-", "-.--", "--..", "", "", "", "", "..--.-",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
};

Buzzer::Buzzer() {
	_frequencyPath = "/sys/devices/pwm-ctrl.42/freq0";
	_enablePath = "/sys/devices/pwm-ctrl.42/enable0";
	_dutyPath = "/sys/devices/pwm-ctrl.42/duty0";
}

Buzzer::~Buzzer() {}

// From https://gist.github.com/jacquerie/8493041a5c5848ddccc8 and adapted from C to C++
const string Buzzer::char_to_morse (char c)
{
        if (islower(c))
                c += ('A' - 'a');

        return CHAR_TO_MORSE[(int) c];
}

void Buzzer::setDutyCycle(uint32_t dutyCycle){
	ofstream file;

	// Writes duty cycle in duty0 file
	file.open(_dutyPath.c_str());
	file << dutyCycle;
	file.close();
}

string Buzzer::translateToMorse(string message){

	string result = "";

	for (int i = 0; i < message.length(); i++){
		if (message[i] == ' '){
			result += WORD_SEPARATOR;
		}
		else{
			result += char_to_morse(message[i]);
			result += CHAR_SEPARATOR;
		}
	}

	return result;
}

void Buzzer::playSequence(string morseSequence){
	for (int i = 0; i < morseSequence.length(); i++){
		char symbol = morseSequence[i];
		switch (symbol){
			case CHAR_SEPARATOR:
				usleep(CHAR_PAUSE);
				break;

			case WORD_SEPARATOR:
				usleep(WORD_PAUSE);
				break;

			case '.':
				tone(FREQUENCY, DOT_LENGTH);
				usleep(DOT_LENGTH);
				break;

			case '-':
				tone(FREQUENCY, DASH_LENGTH);
				usleep(DOT_LENGTH);
				break;

			default: // Shouldn't happen normally. If untreated character, ignore.
				break;
		}
	}
}

void Buzzer::tone(uint32_t frequency, uint32_t length) const
{
	ofstream file;

	// Writes frequency in freq0 file
	file.open(_frequencyPath.c_str());
	file << frequency;
	file.close();

	// Enables sound
	file.open(_enablePath.c_str());
	file << 1;
	file.close();
	// Waits specified duration
	usleep(length);

	//Disables sound after duration
	file.open(_enablePath.c_str());
	file << 0;
	file.close();
}
