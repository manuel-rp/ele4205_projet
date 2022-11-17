/*
 * main.cpp
 *
 *  Created on: Oct 04 2019
 *      Authors: 4205_43
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file buzzer/src/main.cpp
 * \brief The main program for the buzzer application. Connects to the client when launched.
 * Receives a string from the client and plays it in Morse on the buzzer.
**/

#include <iostream>
#include "Buzzer.h"
#include "BuzzerServer.h"

/**
\const BUZZER_SOCKET
\brief The socket number of the buzzer application
**/
#define BUZZER_SOCKET 4100

using namespace std;

int main(int argc, char** argv)
{
	BuzzerServer serveur;
	serveur.initSocket(BUZZER_SOCKET);
	string message = serveur.receiveQRData();
	serveur.closeSocket();

	if (message.length() == 0){
		cout << "The message is empty." << endl;
	}
	else{
		cout << "Message received! Playing \"" << message << "\" in Morse code on buzzer." << endl;
	}

	Buzzer buzzer;
	buzzer.setDutyCycle(512);
	buzzer.playSequence(buzzer.translateToMorse(message));

	return 0;
}
