/*
 * BuzzerServer.h
 *
 *  Created on: Oct. 17, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file BuzzerServer.h
 * \brief The header file for the BuzzerServer class.
**/
#ifndef INCLUDE_BUZZERSERVER_H_
#define INCLUDE_BUZZERSERVER_H_

#include <string.h>
#include <unistd.h>

/**
\const MAXPENDING
\brief The length of the queue for pending connections.
**/
#define MAXPENDING 5

/**
\class BuzzerServer
\brief BuzzerServer is a class that manages server functions and attributes.
**/
class BuzzerServer {
public:
	/**
	\fn TCPClient()
	\brief  BuzzerServer constructor. Initializes attributes.
	**/
	BuzzerServer();
	virtual ~BuzzerServer();

	/**
	\fn initSocket(int port)
	\brief Initializes a TCP socket and waits for client to connect.
	\param port The port number to connect to.
	**/
	void initSocket(int port);

	/**
	\fn receiveQRData();
	\brief Receives the string decoded from the QR code.
	\returns The string that was transmitted.
	**/
	std::string receiveQRData();

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
	\fn receiveNumBytes();
	\brief Receives the number of bytes in the following transmission.
	**/
	void receiveNumBytes();

	/**
	\var _numByyes
	\brief Stores the length of the QR data string sent by client.
	**/
	uint32_t _numBytes;

	/**
	\var _decodedString
	\brief The received message from client.
	**/
	std::string _decodedString;

	/**
	\var _clntSock
	\brief The TCP socket used to communicate with client.
	**/
	int _clntSock;
};

#endif /* INCLUDE_BUZZERSERVER_H_ */
