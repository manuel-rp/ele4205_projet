/*
 * TCPServer.cpp
 *
 *  Created on: Oct. 17, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file TCPServer.cpp
 * \brief The source file for the TCPServer class.
**/

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include "TCPServer.h"

using namespace std;
using cv::Mat;

TCPServer::TCPServer() {
	_clntSock = -1;
	_messages = 0;
}

TCPServer::~TCPServer() {
}

void TCPServer::initSocket(int port){

	in_port_t servPort = port;

	// Create socket for incoming connections
	int servSock; // Socket descriptor for server
	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithSystemMessage("socket() failed");

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
	cout << "Awaiting client connection..." << endl;

	_clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
	if (_clntSock < 0)
	  DieWithSystemMessage("accept() failed");

	char clntName[INET_ADDRSTRLEN]; // String to contain client address
	if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
		sizeof(clntName)) != NULL)
	  printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
	else
	  printf("Unable to get client address");
}

void TCPServer::sendSystemState(uint32_t systemState) {
	ssize_t numBytesSent = send(_clntSock, &systemState, sizeof(uint32_t), 0);

	if (numBytesSent < 0)
		DieWithSystemMessage("send() failed");
}

void TCPServer::sendMatHeader(const Mat &image){
	int header[3]={image.rows, image.cols, image.type()};

	ssize_t numBytesSent = send(_clntSock, header, sizeof(header), 0);
	if (numBytesSent < 0)
		DieWithSystemMessage("send() failed");
}

void TCPServer::sendToClient(const Mat &image){
	sendMatHeader(image);

	// Send image data
	int size = image.total()*image.elemSize();
	ssize_t numBytesSent = send(_clntSock, image.data, size, 0);
    if (numBytesSent < 0)
    	DieWithSystemMessage("send() failed");

	// Read message register
	receiveMessages();
}

void TCPServer::receiveMessages(){
	ssize_t numBytesRead = recv(_clntSock, &_messages, sizeof(uint32_t), 0);

    if (numBytesRead < 0)
    	DieWithSystemMessage("recv() failed");
}

bool TCPServer::continueTransmission(){
	return (_messages & ELE4205_OK == ELE4205_OK);
}

int TCPServer::desiredRes(){
	return (_messages & RESBITS) >> 1;
}

void TCPServer::sendMenuInfoToClient(CameraSpecs* menu){
	ssize_t numBytesSent = send(_clntSock, menu, 4*sizeof(CameraSpecs), 0);
	if (numBytesSent < 0)
		DieWithSystemMessage("send() failed");
}

void TCPServer::closeSocket(){
	close(_clntSock);
	cout << "Server: socket closed." << endl;
}

void TCPServer::DieWithUserMessage(const char *msg, const char *detail) {
  printf("%s : %s \n", msg, detail);
  exit(1);
}

void TCPServer::DieWithSystemMessage(const char *msg) {
  perror(msg);
  exit(1);
}

