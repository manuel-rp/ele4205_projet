/*
 * TCPClient.h
 *
 *  Created on: Oct. 17, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file TCPClient.h
 * \brief The header file for the TCPClient class.
**/

#ifndef INCLUDE_TCPCLIENT_H_
#define INCLUDE_TCPCLIENT_H_

#include <opencv2/opencv.hpp>

/**
\const ELE4205_OK
\brief The bit used to indicate if the transmission continues.
**/
#define ELE4205_OK	0b0001

/**
\const RES1
\brief The identifier for RES01.
**/
#define RES1		0b0000

/**
\const RES2
\brief The identifier for RES02.
**/
#define RES2		0b0010

/**
\const RES3
\brief The identifier for RES03.
**/
#define RES3		0b0100

/**
\const RES4
\brief The identifier for RES04.
**/
#define RES4		0b0110

/**
\const KEY_TIME
\brief The time during which the application waits for input.
**/
#define KEY_TIME 	30

/**
\const KEY_1
\brief ASCII equivalent for 1 key.
**/
#define KEY_1 		49

/**
\const KEY_2
\brief ASCII equivalent for 2 key.
**/
#define KEY_2		50

/**
\const KEY_3
\brief ASCII equivalent for 3 key.
**/
#define KEY_3 		51

/**
\const KEY_4
\brief ASCII equivalent for 4 key.
**/
#define KEY_4 		52

/**
\const KEY_ESC
\brief ASCII equivalent for ESC key.
**/
#define KEY_ESC		27

/**
\const MENU_SIZE
\brief The number of resolutions in the menu.
**/
#define MENU_SIZE 4

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

/**
\class TCPClient
\brief TCPClient is a class that manages client functions and attributes.
**/
class TCPClient {
public:
	/**
	\fn TCPClient()
	\brief  TCPClient constructor. Initializes attributes.
	**/
	TCPClient();
	virtual ~TCPClient();

	/**
	\fn connectToServer(char *servIP, int port);
	\brief Receives an image from server through TCP socket.
	**/
	void connectToServer(char *servIP, int port);

	/**
	\fn receiveMenuInfoFromServer()
	\brief Receives the menu information from the server.
	**/
	void receiveMenuInfoFromServer();

	/**
	\fn printMenu(CameraSpecs menu[])
	\brief Displays the resolution menu.
	**/
	void printMenu(CameraSpecs menu[]);

	/**
	\fn readServerMessage();
	\brief Receives the information from server about server state.
	\returns The state of the server (READY, IDOWN, PUSHB).
	**/
	uint32_t readServerMessage();

	/**
	\fn receiveFromServer()
	\brief Receives an image from server through TCP socket.
	\returns The image received by the server.
	**/
	cv::Mat receiveFromServer();

	/**
	\fn sendMessages()
	\brief Sends the messages register to the server to indicate if client wishes to continue transmission.
	\returns true if transmission is pursued, false otherwise.
	**/
	bool sendMessages();

	/**
	\fn sendQRData(string data);
	\brief Sends the decoded string to the buzzer server .
	\param data The decoded QR string to send.
	**/
	void sendQRData(std::string data);

	/**
	\fn closeSocket()
	\brief Closes server socket.
	**/
	void closeSocket();

	/**
	\fn DieWithUserMessage(const char *msg, const char *detail);
	\brief Prints the user error and quits the application. Adapted from Donahoo's DieWithMessage.c
	\param msg The message to display.
	\param detail The details of the message.
	**/
	void DieWithUserMessage(const char *msg, const char *detail);

	/**
	\fn DieWithSystemMessage(const char *msg);
	\brief Prints the system error and quits the application. From Donahoo's DieWithMessage.c
	\param msg The message to display.
	**/
	void DieWithSystemMessage(const char *msg);

private:
	/**
	\var _servSock
	\brief The TCP socket used to communicate with server.
	**/
	int _servSock;

	/**
	\var _desiredRes
	\brief The number the user pressed to choose a resolution.
	**/
	int _desiredRes;
};

#endif /* INCLUDE_TCPCLIENT_H_ */
