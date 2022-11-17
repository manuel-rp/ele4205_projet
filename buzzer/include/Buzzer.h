/*
 * Buzzer.h
 *
 *  Created on: Nov. 24, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file Buzzer.h
 * \brief The header file for the Buzzer class.
**/

#ifndef SRC_BUZZER_H_
#define SRC_BUZZER_H_

#include <stdint.h>
#include <string>

/**
\const DOT_LENGTH
\brief The length of a morse code dot in milliseconds.
**/
#define DOT_LENGTH 60*1000

/**
\const DASH_LENGTH
\brief The length of a morse code dash in milliseconds.
**/
#define DASH_LENGTH 3*DOT_LENGTH

/**
\const FREQUENCY
\brief The frequency of the tones.
**/
#define FREQUENCY 698

/**
\const CHAR_PAUSE
\brief The additionnal time to wait after a character. Is added to the customary DOT_LENGTH pause after each symbol.
**/
#define CHAR_PAUSE 2*DOT_LENGTH

/**
\const WORD_PAUSE
\brief The additionnal time to wait after a word. Is added to the customary DOT_LENGTH pause after each symbol, plus the CHAR_PAUSe.
**/
#define WORD_PAUSE 4*DOT_LENGTH

/**
\const CHAR_SEPARATOR
\brief Indicates that another character is starting.
**/
#define CHAR_SEPARATOR 'C'

/**
\const WORD_SEPARATOR
\brief Indicates that another word is starting.
**/
#define WORD_SEPARATOR 'W'

/**
 * \class Buzzer
 * \brief Buzzer is a class that contains the various playback and translation to Morse functions.
 */
class Buzzer {
public:
	Buzzer();
	virtual ~Buzzer();

	/**
	\fn setDutyCycle(uint32_t dutyCycle);
	\brief Writes the specified duty cycle at the pilot path.
	\param dutyCycle The desired duty cycle based on a 100% of 1024 (e.g. 512 for a 50% cycle).
	**/
	void setDutyCycle(uint32_t dutyCycle);

	/**
	\fn translateToMorse(std::string message);
	\brief Translates the string of characters into a string of dots, dashes and separators.
	\param message The character string to translate.
	\returns The Morse equivalent of the string.
	**/
	std::string translateToMorse(std::string message);

	/**
	\fn playSequence(std::string morseSequence);
	\brief Plays the Morse sounds that correspond to the specified Morse string.
	\param morseSequence The previously formatted Morse string returned by translateToMorse().
	**/
	void playSequence(std::string morseSequence);

private:
	/**
	\fn char_to_morse(char c);
	\brief Translates a single character into its Morse equivalent
	\param c The character to translate.
	\returns A string containing the Morse symbols equivalent to the char.
	**/
	const std::string char_to_morse(char c);

	/**
	\fn tone(uint32_t frequency, uint32_t length) const;
	\brief Plays a single tone with the buzzer, of the specified frequency and duration.
	\param frequency The frequency of the tone.
	\param length The duration of the tone in milliseconds.
	**/
	void tone(uint32_t frequency, uint32_t length) const;

	/**
	\var CHAR_TO_MORSE[128];
	\brief An array containing the Morse sequence of some characters, at the index corresponding to that character's ASCII code.
	**/
	static const std::string CHAR_TO_MORSE[128];

	/**
	\var _frequencyPath
	\brief The path to the file where the buzzer frequency is set.
	**/
	std::string _frequencyPath;

	/**
	\var _enablePath
	\brief The path to the file where the buzzer is enabled or disabled.
	**/
	std::string _enablePath;

	/**
	\var _dutyPath
	\brief The path to the file where the buzzer duty cycle is set.
	**/
	std::string _dutyPath;
};

#endif /* SRC_BUZZER_H_ */
