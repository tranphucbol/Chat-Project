#include "stdafx.h"
#include "Server.h"
#include "Account.h"

AccountList arr;
char NOF[20];
bool check = false;
char* Data = NULL;
long SizeOfFile = 4096;
bool status = false;
int idroomSF = -1;

Server::Server()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
	}

	// Create a socket
	listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Create the master file descriptor set and zero it
	master;
	FD_ZERO(&master);

	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// connections 
	FD_SET(listening, &master);

	// this will be changed by the \quit command
	running = true;
}

Server::~Server()
{
	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(listening, &master);
	closesocket(listening);

	// Message to let users know what's happening.
	string msg = "Server is shutting down. Goodbyenot";

	while (master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = master.fd_array[0];

		// Send the goodbye message
		send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
}

void Server::active() {
	int size = 5000 + 6;
	while (running)
	{
		// SO MAKE A COPY OF THE MASTER LIST TO PASS INTO select() !!!

		fd_set copy = master;

		// See who's talking to us
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = copy.fd_array[i];

			// Is it an inbound communication?
			if (sock == listening)
			{
				// Accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);
				// Add the new connection to the list of connected clients
				FD_SET(client, &master);

				//log
				log << "Accept Socket #" << client << endl;

				// Send a welcome message to the connected client
				string welcomeMsg = "Welcome to the Awesome Chat Server!\r\n";
				string signinMsg = "Please sign in with your account or sign up!\r\n";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
				send(client, signinMsg.c_str(), signinMsg.size() + 1, 0);
			}
			else // It's an inbound message
			{
				char* buf = new char[size];
				ZeroMemory(buf, size);

				// Receive message
				int bytesIn = recv(sock, buf, size, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					log << "Socket #" << sock << " drop" << endl;
					closesocket(sock);
					FD_CLR(sock, &master);
				}

				else
				{
					// Check to see if it's a command. \quit kills the server
					string mess;
					// ACCOUNT
					if (buf[bytesIn - 3] == 'd') {
						string usernameMsg;
						string passwordMsg;

						// SIGN IN
						if (buf[strlen(buf) - 2] == 'n') {
							decryptMsgLogin((string)buf, usernameMsg, passwordMsg);


							bool beingOnl = false;
							for (int i = 0; i < uOnl.size(); i++)
								// Check the user gets online or not
								if (usernameMsg == uOnl[i].username) {
									beingOnl = true;
									break;
								}

							if (beingOnl) {
								// The user's being onl -> Can't sign in
								mess = "dn0";
								send(sock, mess.c_str(), mess.size() + 1, 0);
								break;
							}

							char username[20];
							char password[21];
							strcpy(username, usernameMsg.c_str());
							strcpy(password, passwordMsg.c_str());
							if (arr.signIn(username, password)) {
								mess = "dn1";
								send(sock, mess.c_str(), mess.size() + 1, 0);
								//add user is being online
								UserOnline user(sock, username);
								uOnl.push_back(user);
								//log
								log << "user: " << username << " has just logged in ----> Socket #" << sock << endl;
							}
							else {
								mess = "dn0";
								send(sock, mess.c_str(), mess.size() + 1, 0);
							}
						}
						// SIGN UP
						else if (buf[strlen(buf) - 2] == 'k') {
							decryptMsgLogin((string)buf, usernameMsg, passwordMsg);
							char username[20];
							char password[21];
							strcpy(username, usernameMsg.c_str());
							strcpy(password, passwordMsg.c_str());
							if (arr.signUp(username, password)) {
								mess = "dk1";
								send(sock, mess.c_str(), mess.size() + 1, 0);
								//add user is being online
								UserOnline user(sock, username);
								uOnl.push_back(user);

								//log
								log << "user: " << username << " has registered" << endl;
							}
							else {
								mess = "dk0";
								send(sock, mess.c_str(), mess.size() + 1, 0);
							}
						}
						// CHANGE PASSW
						else if (buf[strlen(buf) - 2] == 'c') {
							decryptMsgLogin((string)buf, usernameMsg, passwordMsg);
							char username[20];
							char password[21];
							strcpy(username, usernameMsg.c_str());
							strcpy(password, passwordMsg.c_str());
							if (arr.updatePassw(username, password)) {
								mess = "dm1";
								send(sock, mess.c_str(), mess.size() + 1, 0);
								//log
								log << "user: " << username << " has just changed password" << endl;
							}
							else {
								mess = "dm0";
								send(sock, mess.c_str(), mess.size() + 1, 0);
							}
						}
						// SIGN OUT
						else if (buf[strlen(buf) - 2] == 'x') {
							string username(buf, bytesIn - 3);

							int index = -1;

							for (int i = 0; i < uOnl.size(); i++)
								if (username == uOnl[i].username) {
									index = i; // Username always exists
									break;
								}
							if (index > -1)
								uOnl.erase(uOnl.begin() + index);
							RemoveClient(sock);
							//log
							log << "user: " << username << "has just logged out" << endl;

						}
					}

					//Send file
					else if (buf[bytesIn - 4] == 's' && buf[bytesIn - 3] == 'u' && buf[bytesIn - 2] == 'o') {
						string username(buf, strlen(buf) - 3);
						int index = -1;
						for (int i = 0; i < uOnl.size(); i++) {
							if (username == uOnl[i].username) {
								index = i;
								break;
							}
						}
						if (index == -1) {
							char notOnline[] = "0suo";
							send(sock, notOnline, strlen(notOnline) + 1, 0);
						}
						else {
							UserSendFile = uOnl[index].socket;
							char Online[] = "1suo";
							send(sock, Online, strlen(Online) + 1, 0);
						}
						char OK[] = "nok";
						send(sock, OK, strlen(OK) + 1, 0);
					}
					else if (buf[bytesIn - 4] == 'n' && buf[bytesIn - 3] == 'o' && buf[bytesIn - 2] == 'f') {
						send(UserSendFile, buf, bytesIn, 0);
						char OK[] = "nok";
						send(sock, OK, strlen(OK) + 1, 0);
					}
					else if (buf[bytesIn - 4] == 's' && buf[bytesIn - 3] == 'o' && buf[bytesIn - 2] == 'f') {
						send(UserSendFile, buf, bytesIn, 0);
						char OK[] = "nok";
						send(sock, OK, strlen(OK) + 1, 0);
					}
					else if (buf[0] == 'b' && buf[1] == 'o' && buf[2] == 'f') {
						//Check Send File
						//cout << "Size: " << size << endl;
						//cout << "Bytes In: " << bytesIn << endl;
						send(UserSendFile, buf, bytesIn, 0);
						char OK[] = "nok";
						send(sock, OK, strlen(OK) + 1, 0);
						//size = 4096;
					}
					//End send file

					//Chat Room
					else if (buf[bytesIn - 3] == 'u')
					{
						//check user online
						int index = -1;
						string username(buf, bytesIn - 3);
						for (int i = 0; i < uOnl.size(); i++) {
							if (username == uOnl[i].username) {
								if (sock == uOnl[i].socket)
									index = -2;

								else {
									index = i;
								}
								break;
							}
						}
						if (index == -1) {
							//user's not online
							//send notification to client
							char notification[] = "User's not onlinenot";
							send(sock, notification, strlen(notification) + 1, 0);
						}
						else if (index == -2) {
							// client add itself
							char notification[] = "You are adding yourselfnot";
							send(sock, notification, strlen(notification) + 1, 0);
						}
						else {
							//user's online
							//check if client have room or not?
							if (buf[bytesIn - 2] == (char)255 && buf[bytesIn - 1] == (char)255) {
								//client doesn't have room
								//Create room
								int idRoom = room.Create(sock, uOnl[index].socket);
								idroomSF = idRoom;
								char msgIdRoom[4];
								encryptIdRoom(msgIdRoom, idRoom);

								//send id room to client
								send(sock, msgIdRoom, 3, 0);
								send(uOnl[index].socket, msgIdRoom, 3, 0);

								char notification[] = "Add user successfulnot";
								send(sock, notification, strlen(notification) + 1, 0);
							}
							else {
								//client have room
								int idRoom = decryptIdRoom(buf, bytesIn);
								idroomSF = idRoom;
								if (!(room.Insert(idRoom, uOnl[index].socket))) {
									char notification[] = "The user already exists at the room!not";
									send(sock, notification, strlen(notification) + 1, 0);
									break;
								}

								char msgIdRoom[4];
								encryptIdRoom(msgIdRoom, idRoom);

								//send id to client
								send(uOnl[index].socket, msgIdRoom, 3, 0);

								char notification[] = "Add user successfulnot";
								send(sock, notification, strlen(notification) + 1, 0);
							}
						}
					}
					//End Chat Room

					//Close Chat Room
					else if (buf[bytesIn - 3] == 'c') {
						int idRoom = decryptIdRoom(buf, 3);
						LeaveRoom(sock, idRoom);
					}
					//End Close Chat Room

					// Send message to other clients, and definiately NOT the listening socket
					else {
						int idRoom = decryptIdRoom(buf, bytesIn);
						SendRoom(idRoom, sock, buf, bytesIn);
					}
					delete[]buf;
				}
			}
		}
	}
}

bool Server::SendRoom(int idRoom, SOCKET s, char buf[], int len) {
	if (idRoom > room.Room.size() - 1 || idRoom < 0)
		return false;
	for (int i = 0; i < room.Room[idRoom].size(); i++) {
		if (room.Room[idRoom][i] != s) {
			send(room.Room[idRoom][i], buf, len, 0);
		}
	}
	return true;
}

bool Server::SendRoom(int idRoom, string buf) {
	int size = room.Room.size();
	if (idRoom > (size - 1) || idRoom < 0)
		return false;
	else {
		char enIdRoom[4];
		encryptIdRoom(enIdRoom, idRoom);
		buf = "Server: " + buf;

		char msg[4096];
		strcpy(msg, buf.c_str());
		int len = strlen(msg);

		msg[len] = enIdRoom[0];
		msg[len + 1] = enIdRoom[1];
		msg[len + 2] = enIdRoom[2];

		for (int i = 0; i < room.Room[idRoom].size(); i++) {
			send(room.Room[idRoom][i], msg, len + 3, 0);
		}
		return true;
	}
}

int Server::decryptIdRoom(char buf[], int len) {
	return ((int)buf[len - 2] << 8) | (int)buf[len - 1];
}

char* Server::encryptIdRoom(char buf[4], int idRoom) {
	strcpy(buf, "r00");
	buf[1] = (char)(idRoom >> 8);
	buf[2] = (char)(idRoom & 0xff);
	return buf;
}

void Server::ShowUserOnline() {
	int size = 30;
	cout << "---UserOnline---" << endl;
	for (int i = 0; i < uOnl.size(); i++) {
		cout << uOnl[i].username;
		for (int j = 0; j < size - uOnl[i].username.length(); j++)
			cout << " ";
		cout << "Socket #" << uOnl[i].socket << endl;
	}
	if (uOnl.size() == 0)
		cout << "Nobody is online" << endl;
}

void Server::ShowRoom() {
	for (int i = 0; i < room.Room.size(); i++) {
		cout << "Room " << i << ":";
		for (int j = 0; j < room.Room[i].size(); j++)
			cout << " #" << room.Room[i][j];
		cout << endl;
	}
	if (room.Room.size() == 0)
		cout << "No room is exist" << endl;
}

void Server::LeaveRoom(SOCKET sock, int idRoom) {
	char buf[4];
	encryptIdRoom(buf, idRoom);
	buf[0] = 'c';
	int rmclient = room.RemoveClient(sock, idRoom);
	if (rmclient == -1) {
		char notification[] = "Room doesn't exist!not";
		send(sock, notification, strlen(notification) + 1, 0);
	}
	else if (rmclient == -2) {
		bool isExist = false;
		string username;
		for (int i = 0; i < uOnl.size(); i++)
			if (sock == uOnl[i].socket) {
				isExist = true;
				username = uOnl[i].username;
				break;
			}
		if (isExist) {
			char notification[50];
			strcpy(notification, username.c_str());
			strcat(notification, " has left the room");
			int len = strlen(notification);

			//assign id room
			notification[len] = 'r';
			notification[len + 1] = buf[1];
			notification[len + 2] = buf[2];

			SendRoom(idRoom, sock, notification, len + 3);
		}
	}
	else {
		send(rmclient, buf, 3, 0);

	}
}

void Server::RemoveClient(SOCKET sock) {
	for (int i = 0; i < room.Room.size(); i++) {
		LeaveRoom(sock, i);
	}
}