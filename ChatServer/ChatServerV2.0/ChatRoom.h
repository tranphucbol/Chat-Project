#pragma once
#include <vector>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

class ChatRoom
{
public:
	ChatRoom();
	int Create(SOCKET client1, SOCKET client2);
	bool Insert(int idRoom, SOCKET client);
	bool RemoveRoom(int idRoom);

	//-1 == not exist id room
	//-2 == remove sock
	//socket == remove room

	int RemoveClient(SOCKET sock, int idRoom);
	
	~ChatRoom();
	vector<vector<SOCKET>> Room;
};

