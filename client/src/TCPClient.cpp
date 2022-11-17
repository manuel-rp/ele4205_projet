/*
 * TCPClient.cpp
 *
 *  Created on: Oct. 17, 2020
 *      Author: ele4205
 *	Manuel Roger-Proulx	1899033
 *	Valérie Servranckx	1893677
 */

/*! \file TCPClient.cpp
 * \brief The source file for the TCPClient class.
**/

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include "TCPClient.h"

using namespace std;
using cv::Mat;

TCPClient::TCPClient() {
	_servSock = -1;
	_desiredRes = 1;
}

TCPClient::~TCPClient() {
}

void TCPClient::connectToServer(char* servIP, int port){
	in_port_t servPort = port;

	// Create a reliable, stream socket using TCP
	_servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_servSock < 0)
		DieWithSystemMessage("socket() failed");

	// Set socket to be able to reuse after closing
	int optValue = 1;
	setsockopt(_servSock, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(int));

	// Construct the server address structure
	struct sockaddr_in servAddr;            // Server address
	memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
	servAddr.sin_family = AF_INET;          // IPv4 address family

	// Convert address
	int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);

	if (rtnVal == 0)
		DieWithUserMessage("inet_pton() failed", "invalid address string");
	else if (rtnVal < 0)
		DieWithSystemMessage("inet_pton() failed");

	servAddr.sin_port = htons(servPort);    // Server port

	// Establish the connection to the echo server
	if (connect(_servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("connect() failed");

	//cout << "Connected to server :^)" << endl;
}

void TCPClient::receiveMenuInfoFromServer(){
	CameraSpecs menu[MENU_SIZE];

	// For every element in the menu (4), receive it
	for (int i = 0; i < MENU_SIZE; i++){
		ssize_t bytesRcvd = recv(_servSock, menu + i, sizeof(CameraSpecs), 0);
		if (bytesRcvd < 0)
			DieWithSystemMessage("recv() failed");
		else if (bytesRcvd == 0)
			DieWithUserMessage("recv()", "connection closed prematurely");
	}

	printMenu(menu);
}

void TCPClient::printMenu(CameraSpecs menu[]){

	// Print available resolutions
	cout << "Available resolutions: " << endl;

	for (int i = 0; i < MENU_SIZE; i++){
		printf("%2i: %4i x %i \n",i+1, menu[i].resX, menu[i].resY);
	}

	cout << endl << "Change resolution on the fly with (1-4)." << endl;
}

uint32_t TCPClient::readServerMessage(){
	uint32_t serverMessage = 0;

	ssize_t numBytesRead = recv(_servSock, &serverMessage, sizeof(uint32_t), 0);
	if (numBytesRead < 0)
		DieWithSystemMessage("recv() failed");

	return serverMessage;
}

Mat TCPClient::receiveFromServer(){
	// Receive header to initialise Mat object
	int header[3];
	ssize_t bytesRcvd = recv(_servSock, header, sizeof(header), 0);

	Mat image = Mat::zeros(header[0], header[1], header[2]);

	int imageSize = image.total()*image.elemSize();
	ssize_t totalBytesRcvd = 0;
	uchar data[imageSize];

	// Receive data and reconstruct image. Adapted from https://stackoverflow.com/a/20321262
	while (totalBytesRcvd < imageSize) {
		ssize_t bytesRcvd = recv(_servSock, data + totalBytesRcvd, imageSize - totalBytesRcvd, 0);
		if (bytesRcvd < 0)
			DieWithSystemMessage("recv() failed");
		else if (bytesRcvd == 0)
			DieWithUserMessage("recv()", "connection closed prematurely");

		totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
	}

	// Assign pixel value to image
	int ptr=0;
	for (int i = 0; i < image.rows; i++){
		for (int j = 0; j < image.cols; j++){
			image.at<cv::Vec3b>(i,j) = cv::Vec3b(data[ptr+0], data[ptr+1], data[ptr+2]);
			ptr += 3;
		}
	}

	return image;
}

bool TCPClient::sendMessages(){
	// Register
	uint32_t messages = 0;
	bool continueTransmission = true;

	// Read keyboard inputs
	int key = cv::waitKey(KEY_TIME);
	switch (key)
	{
	case KEY_1:
		_desiredRes = 1;
		break;

	case KEY_2:
		_desiredRes = 2;
		break;

	case KEY_3:
		_desiredRes = 3;
		break;

	case KEY_4:
		_desiredRes = 4;
		break;

	case KEY_ESC:
		continueTransmission = false;
		break;

	default:
		// Do nothing if no key is pressed
		break;
	}

	// Set register bits
	messages |= ELE4205_OK && continueTransmission;
	
	switch (_desiredRes)
	{
	case 1:
		messages |= RES1;
		break;

	case 2:
		messages |= RES2;
		break;

	case 3:
		messages |= RES3;
		break;

	case 4:
		messages |= RES4;
		break;

	default:
			// Never supposed to happen. exit(1)?
		break;
	}

	// Send message register to server
	ssize_t numBytesSent = send(_servSock, &messages, sizeof(messages), 0);

    if (numBytesSent < 0)
    	DieWithSystemMessage("send() failed");

	return continueTransmission;
}

void TCPClient::sendQRData(string data)
{
	// Send string length to server
	uint32_t numChar = data.length();
	ssize_t numBytesSent = send(_servSock, &numChar, sizeof(numChar), 0);
	if (numBytesSent < 0)
		DieWithSystemMessage("send() failed");

	// Send data to server
	char* dataPtr = &data[0];

	numBytesSent = send(_servSock, dataPtr, numChar, 0);
	if (numBytesSent < 0)
		DieWithSystemMessage("send() failed");
}

void TCPClient::closeSocket(){
	close(_servSock);
	cout << "Client: socket closed." << endl;
}
void TCPClient::DieWithUserMessage(const char *msg, const char *detail) {
  printf("%s : %s \n", msg, detail);
  exit(1);
}

void TCPClient::DieWithSystemMessage(const char *msg) {
  perror(msg);
  exit(1);
}
