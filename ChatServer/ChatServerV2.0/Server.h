#pragma once
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <vector>
#include "ChatRoom.h"
#include "sstream"

#pragma comment (lib, "ws2_32.lib")

using namespace std;


class UserOnline {
public:
	SOCKET socket;
	string username;
	UserOnline(SOCKET socket, string username) {
		this->socket = socket;
		this->username = username;
	}
};


class Server
{
public:
	Server();
	void active();
	void ShowUserOnline();
	void ShowRoom();
	bool SendRoom(int idRoom, string buf);
	~Server();
	stringstream log;
	SOCKET UserSendFile;
private:
	SOCKET listening;
	fd_set master;
	bool running;
	ChatRoom room;
	vector<UserOnline> uOnl;
	bool SendRoom(int idRoom, SOCKET s, char buf[], int len);
	int decryptIdRoom(char buf[], int len);
	char* encryptIdRoom(char buf[4], int idRoom);
	void RemoveClient(SOCKET sock);
	void LeaveRoom(SOCKET sock, int idRoom);
};

