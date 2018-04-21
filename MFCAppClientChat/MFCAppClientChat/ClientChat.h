#pragma once
#include "stdafx.h"
#include <string>
#include <iostream>
#include <afxsock.h>
#include <thread>
#include <fstream>
#include <vector>
#include "SendFile.h"
using namespace std;

class ClientChat {
public:
	ClientChat();
	bool StartConnect(string ip, int port);
	void SendMess(string buf);
	void SendMess(char buf[], int len);
	bool sendFile(string username, string filename);
	string name;
	string pass;
	int size;
	char fn[20];
	bool valid = false;
	SendFile* sendf;
	SOCKET client;
	vector<int> room;
	~ClientChat() {
		delete sendf;
		closesocket(client);
		WSACleanup();
	};
};
void RecvMess(ClientChat clientChat);
string encryptMsgLogin(string username, string password);
string encryptMsgSignUp(string username, string password);
string encryptMsgChangeP(string username, string password);
int decryptIdRoom(char buf[], int len);
char* encryptIdRoom(char buf[4], int idRoom);
