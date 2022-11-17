/*
 * TCPServer.h
 *
 *  Created on: Oct. 17, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file TCPServer.h
 * \brief The header file for the TCPServer class.
**/
#ifndef INCLUDE_TCPSERVER_H_
#define INCLUDE_TCPSERVER_H_

#include <opencv2/opencv.hpp>
#include "CameraSpecs.h"

/**
\const MAXPENDING
\brief The length of the queue for pending connections.
**/
#define MAXPENDING 5

/**
\const ELE4205_OK
\brief The bit used to indicate if the transmission continues.
**/
#define ELE4205_OK	0b0001

/**
\const RESBITS
\brief The bits that contain the resolution.
**/
#define RESBITS		0b0110

/**
\class TCPServer
\brief TCPServer is a class that manages server functions and attributes.
**/
class TCPServer {
public:
	/**
	\fn TCPClient()
	\brief  TCPServer constructor. Initializes attributes.
	**/
	TCPServer();
	virtual ~TCPServer();

	/**
	\fn initSocket(int port)
	\brief Initializes a TCP socket and waits for client to connect.
	\param port The port number to connect to.
	**/
	void initSocket(int port);

	/**
	\fn sendSystemState(uint32_t systemState)
	\brief Sends the state information to the client.
	\param systemState The current state of the system.
	**/
	void sendSystemState(uint32_t systemState);

	/**
	\fn sendToClient(const Mat &image)
	\brief Sends image data to client throught the TCP socket.
	\param image The image being sent.
	**/
	void sendToClient(const cv::Mat &image);

	/**
	\fn continueTransmission();
	\brief Verifies the messages register to determine if client wants to continue transmission.
	\returns true to continue and false to stop.
	**/
	bool continueTransmission();

	/**
	\fn desiredRes();
	\brief Verifies the messages register to determine the desired resolution.
	\returns the index of the desired resolution.
	**/
	int desiredRes();

	/**
	\fn sendMenuInfoToClient(CameraSpecs* menu)
	\brief Sends the available resolutions menu to client.
	\pram menu The pointer to the menu.
	**/
	void sendMenuInfoToClient(CameraSpecs* menu);

	/**
	\fn closeSocket()
	\brief Closes client socket.
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
	\fn sendMatHeader(const Mat &image)
	\brief Sends the header information of an image.
	\param image The image whose header is transmitted.
	**/
	void sendMatHeader(const cv::Mat &image);

	/**
	\fn receiveMessages();
	\brief Receives the messages register from the client.
	**/
	void receiveMessages();

	/**
	\var _messages
	\brief An integer that contains various messages.
	**/
	uint32_t _messages;

	/**
	\var _clntSock
	\brief The TCP socket used to communicate with client.
	**/
	int _clntSock;
};

#endif /* INCLUDE_TCPSERVER_H_ */
