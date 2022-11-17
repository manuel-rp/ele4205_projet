/*
 * main.cpp
 *
 *  Created on: Oct 04 2019
 *      Authors: 4205_43
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file client/src/main.cpp
 * \brief The main program for the client application. Connects to the server and displays
 * the image stream. Analyzes images for a QR code when prompted, and send the decoded string
 * to the buzzer application after connecting to it.
**/

#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <opencv2/opencv.hpp>
#include "TCPClient.h"
#include "CodeReader.h"

/**
\const MAIN_SOCKET
\brief The socket number of the main server application
**/
#define MAIN_SOCKET 4099

/**
\const BUZZER_SOCKET
\brief The socket number of the buzzer application
**/
#define BUZZER_SOCKET 4100

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	string ip = "192.168.0.120";
	int savedImageCount = 0;

	TCPClient client;
	client.connectToServer(&ip[0], MAIN_SOCKET);

	client.receiveMenuInfoFromServer();

	bool continueTransmission = true;
	uint32_t serverMessage = IDOWN;

	while (continueTransmission == true){
		// Wait for server to have proper lighting conditions before attempting to receive images
		do {
			serverMessage = client.readServerMessage();
		} while (serverMessage == IDOWN);

		// Receive image from server
		Mat image = client.receiveFromServer();

		if (image.empty()){
			cout << "Image sent by server is empty! :(" << endl;
			return 1;
		}

		// Display image
		if (serverMessage == READY){
			imshow("Display-o-matic 3000",image);
		}
		else if (serverMessage == PUSHB){
			savedImageCount++;

			stringstream ss;
			ss << "img" << std::setfill('0') << std::setw(3) << savedImageCount << ".png";
			string imagename = ss.str();

			pid_t pid = fork();
			// Child process
			if (pid==0){
				// Save image to local memory.
				imwrite(imagename, image);
				cout << "Image " <<imagename << " saved!" << endl;

				// Do code reading
				CodeReader codeReader;

				if (codeReader.decode(image) > 0){
					cout << "Data : \"" << codeReader.getCodeData() << "\""<< endl << endl;
				}

				// Send data to server port 4100
				client.connectToServer(&ip[0], BUZZER_SOCKET);
				client.sendQRData(codeReader.getCodeData());

				exit(0);
			}
			// Parent process
			else if (pid > 0){
				cv::imshow("Display-o-matic 3000",image);
				wait(0);
			}
			// Fork failed
			else{
				cout << "Fork failed." << endl;
				return 1;
			}

			//kill(pid,SIGTERM);
		}

		continueTransmission = client.sendMessages();
	}

	cout << "QUIT message sent." << endl;

	client.closeSocket();

	return 0;
}

