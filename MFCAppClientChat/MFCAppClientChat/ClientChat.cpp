#include "stdafx.h"
#include "ClientChat.h"
#include <sstream>

ClientChat::ClientChat() {
	size = 4096;
	sendf = NULL;
}

void ClientChat::SendMess(string buf) {
	send(client, buf.c_str(), buf.length(), 0);
}

void ClientChat::SendMess(char buf[], int len) {
	unsigned long sz = strlen(buf);
	send(client, buf, len, 0);
}

void RecvMess(ClientChat clientChat) {
	char buf[4096] = "";
	while (strcmp(buf, "exit")) {
		ZeroMemory(buf, 4096);
		int len = recv(clientChat.client, buf, 4096, 0);
		buf[len] = 0;
	}
}

bool ClientChat::StartConnect(string ipAddress, int port) {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		stringstream ss;
		ss << "Can't start Winsock, Err #" << wsResult << endl;
		string s(ss.str());
		AfxMessageBox(CString(s.c_str(), s.length()));
		return false;
	}

	// Create socket
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client == INVALID_SOCKET)
	{
		stringstream ss;
		ss << "Can't create socket, Err #" << WSAGetLastError() << endl;
		string s(ss.str());
		AfxMessageBox(CString(s.c_str(), s.length()));
		WSACleanup();
		return false;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str() , &hint.sin_addr);

	// Connect to server
	int connResult = connect(client, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		stringstream ss;
		ss << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		string s(ss.str());
		AfxMessageBox(CString(s.c_str(), s.length()));
		closesocket(client);
		WSACleanup();
		return false;
	}

	return true;
}

string encryptMsgLogin(string username, string password) {
	string format = "dn";
	int pos = username.length();
	format += (char)pos;
	return username + password + format;
}

string encryptMsgSignUp(string username, string password) {
	string format = "dk";
	int pos = username.length();
	format += (char)pos;
	return username + password + format;
}

string encryptMsgChangeP(string username, string password) {
	string format = "dc";
	int pos = username.length();
	format += (char)pos;
	return username + password + format;
}

int decryptIdRoom(char buf[], int len) {
	return ((int)buf[len - 2] << 8) | (int)buf[len - 1];
}

char* encryptIdRoom(char buf[4], int idRoom) {
	buf[1] = (char)(idRoom >> 8);
	buf[2] = (char)(idRoom & 0xff);
	return buf;
}

bool ClientChat::sendFile(string username, string filename) {
	sendf = new SendFile();

	//Check if user is online or not
	sendf->StateSendFile = false;
	username += "suo";
	send(client, username.c_str(), username.length() + 1, 0);
	while (!sendf->StateSendFile);

	if (!sendf->StateUserOnline) {
		AfxMessageBox(_T("User's not online"));
		return false;
	}

	sendf->filename = filename;
	if (!sendf->readfile()) {
		AfxMessageBox(_T("File Not Found"));
		return false;
	}

	//Send File name
	filename = sendf->splitFileName(filename);
	filename += "nof";
	sendf->StateSendFile = false;
	send(client, filename.c_str(), filename.length() + 1, 0);
	//wait Server
	while (!sendf->StateSendFile);

	//Send Size name
	char Size[14];
	itoa(sendf->size, Size, 10);
	strcat(Size, "sof");
	sendf->StateSendFile = false;
	send(client, Size, strlen(Size) + 1, 0);
	//wait Server
	while (!sendf->StateSendFile);

	//Send file
	cout << "Send Content" << endl;
	for (int i = 0; i < sendf->noPackage - 1; i++) {
		sendf->StateSendFile = false;
		send(client, sendf->content[i], SIZE + 6, 0);
		while (!sendf->StateSendFile);
	}
	send(client, sendf->content[sendf->noPackage - 1], ((sendf->surplus == 0) ? SIZE : sendf->surplus) + 6, 0);
	delete sendf;
	sendf = NULL;
	return true;
}