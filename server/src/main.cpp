/*
 * main.cpp
 *
 *  Created on: Oct 04 2019
 *      Authors: 4205_43
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/**
\mainpage
This program opens a TCP socket and transmits a video stream from the remote server to the local client.
Use [1]-[4] to change the resolution. A picture from the stream can be saved locally by pressing the button. If the room is too dark,
the server stops the stream until lighting conditions are better. The image captured by pressing
the button is analyzed with OpenCV to detect and read a QR code. The string contained in the code
is played in Morse Code through a buzzer, after which the buzzer program is terminated. It can be
executed any number of times by capturing a code with the camera. [ESC] terminates all programs.
**/

/*! \file server/src/main.cpp
 * \brief The main program for the server application. Connects to the client and sends a video stream
 * from the camera. Evaluates light levels and user inputs. Launches the buzzer app when the button is pressed.
**/

#include <ctime>
#include <cstdlib>
#include <sys/wait.h>
#include <iostream>
#include "TCPServer.h"
#include "CameraManager.h"
#include "IOManager.h"

/**
\const MAIN_SOCKET
\brief The socket number of the main server application
**/
#define MAIN_SOCKET 4099

using namespace std;

int main(int argc, char** argv)
{
	TCPServer serveur;
	serveur.initSocket(MAIN_SOCKET);

	CameraManager camera;
	IOManager ioManager;
	uint32_t systemState = IDOWN;

	serveur.sendMenuInfoToClient(camera.getMenuInfo());

	do{
		// Wait for lighting to be appropriate before capturing images
		do {
			// Send lighting & button info to client. 
			systemState = ioManager.getSystemState();
			serveur.sendSystemState(systemState);
			usleep(5000); // wait 5 ms to avoid server shutdown
		} 
		while (systemState == IDOWN);

		if (systemState == PUSHB){
			pid_t pid = fork();

			if (pid == 0){
				execv("Buzzer", NULL);
				exit(0);
			}
			// Fork failed
			else if (pid < 0){
				cout << "Buzzer fork failed." << endl;
				return 1;
			}
		}

		// Capture and send an image to client
		serveur.sendToClient(camera.captureImage());

		// Adjust resolution on the fly
		if (serveur.desiredRes() != camera.currentRes()) 
			camera.setResolution(serveur.desiredRes());

	}
	while (serveur.continueTransmission());

	cout << "QUIT message received." << endl;
	serveur.closeSocket();

	return 0;
}
