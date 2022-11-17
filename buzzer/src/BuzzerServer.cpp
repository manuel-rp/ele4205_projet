/*
 * BuzzerServer.cpp
 *
 *  Created on: Nov. 24, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file BuzzerServer.cpp
 * \brief The source file for the BuzzerServer class.
**/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "BuzzerServer.h"

using std::string;

BuzzerServer::BuzzerServer() {
	_clntSock = -1;
	_numBytes = 0;
	_decodedString = "";
}

BuzzerServer::~BuzzerServer() {
}

void BuzzerServer::initSocket(int port){

	in_port_t servPort = port;

	// Create socket for incoming connections
	int servSock; // Socket descriptor for server
	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithSystemMessage("socket() failed");

	// Set socket to be able to reuse after closing
	int optValue = 1;
	setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(int));

	// Construct local address structure
	struct sockaddr_in servAddr;                  // Local address
	memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
	servAddr.sin_family = AF_INET;                // IPv4 address family
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
	servAddr.sin_port = htons(servPort);          // Local port

	// Bind to the local address
	if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("bind() failed");

	// Mark the socket so it will listen for incoming connections
	if (listen(servSock, MAXPENDING) < 0)
		DieWithSystemMessage("listen() failed");

	struct sockaddr_in clntAddr; // Client address
	// Set length of client address structure (in-out parameter)
	socklen_t clntAddrLen = sizeof(clntAddr);

	// Wait for a client to connect
	_clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
	if (_clntSock < 0)
	  DieWithSystemMessage("accept() failed");

	char clntName[INET_ADDRSTRLEN]; // String to contain client address
	if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
		sizeof(clntName)) == NULL){
		printf("Unable to get client address");
	}

}

void BuzzerServer::receiveNumBytes(){
	ssize_t numBytesRead = recv(_clntSock, &_numBytes, sizeof(uint32_t), 0);

    if (numBytesRead < 0)
    	DieWithSystemMessage("recv() failed");
}

string BuzzerServer::receiveQRData(){
	receiveNumBytes();

	ssize_t totalBytesRcvd = 0;
	char data[_numBytes];

	// Receive data and reconstruct string.
	while (totalBytesRcvd < _numBytes) {
		ssize_t bytesRcvd = recv(_clntSock, data + totalBytesRcvd, _numBytes - totalBytesRcvd, 0);
		if (bytesRcvd < 0)
			DieWithSystemMessage("recv() failed");
		else if (bytesRcvd == 0)
			DieWithUserMessage("recv()", "connection closed prematurely");

		totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
	}

	for (int i = 0; i < _numBytes; i++){
		_decodedString += data[i];
	}
	return _decodedString;
}

void BuzzerServer::closeSocket(){
	close(_clntSock);
}

void BuzzerServer::DieWithUserMessage(const char *msg, const char *detail) {
  printf("%s : %s \n", msg, detail);
  exit(1);
}

void BuzzerServer::DieWithSystemMessage(const char *msg) {
  perror(msg);
  exit(1);
}

