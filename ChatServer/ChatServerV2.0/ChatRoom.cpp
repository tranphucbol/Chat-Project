#include "stdafx.h"
#include "ChatRoom.h"


ChatRoom::ChatRoom()
{
}

int ChatRoom::Create(SOCKET client1, SOCKET client2) {
	vector<SOCKET> r;
	r.push_back(client1);
	r.push_back(client2);
	bool isExistEmptyRoom = false;
	for (int i = 0; i < Room.size(); i++) {
		if (Room[i].size() == 0) {
			Room[i] = r;
			isExistEmptyRoom = true;
			break;
		}
	}
	if(!isExistEmptyRoom)
		Room.push_back(r);
	return (Room.size() - 1);
}

bool ChatRoom::Insert(int idRoom, SOCKET client) {
	if (idRoom > (Room.size() - 1))
		return false;
	else {
		// The user is in this room or not ?
		for (int i = 0; i < Room[idRoom].size(); i++) {
			if (Room[idRoom][i] == client) {
				return false;
			}
		}

		Room[idRoom].push_back(client);
		return true;
	}
}

bool ChatRoom::RemoveRoom(int idRoom) {
	if (idRoom > (Room.size() - 1)) {
		return false;
	}
	else {
		Room.erase(Room.begin() + idRoom);
		return true;
	}
}

int ChatRoom::RemoveClient(SOCKET sock, int idRoom) {
	if (idRoom > (Room.size() - 1) || idRoom < 0) {
		return -1;
	}
	else {
		int index = -1;
		for (int i = 0; i < Room[idRoom].size(); i++) {
			if (sock == Room[idRoom][i]) {
				index = i;
				break;
			}
		}
		if (index == -1) {
			return -1;
		}
		else {
			if (Room[idRoom].size() == 2) {
				SOCKET s = (sock == Room[idRoom][0]) ? Room[idRoom][1] : Room[idRoom][0];
				Room[idRoom].clear();
				return s;
			}
			else {
				Room[idRoom].erase(Room[idRoom].begin() + index);
				return -2;
			}
		}
	}
}

ChatRoom::~ChatRoom()
{
}
