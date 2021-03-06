// ChatServerV2.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ChatServerV2.0.h"
#include "Server.h"
#include <thread>
#include "Menu.h"
#include <algorithm>
#include <codecvt>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

void Enter(Server& server) {
	string enter = "";
	while (enter != "exit") {
		cout << "Enter your command: ";
		getline(cin, enter);
		transform(enter.begin(), enter.end(), enter.begin(), ::toupper);
		if (enter == "SHOW USER ONLINE")
			server.ShowUserOnline();
		else if (enter == "SHOW LOG")
			cout << server.log.str();
		else if (enter == "SEND TO CLIENT") {
			string buf;
			int idRoom;
			cout << "Enter id room: ";
			cin >> idRoom;
			cin.ignore();
			cout << "Enter message: ";
			getline(cin, buf);
			server.SendRoom(idRoom, buf);
		}
		else if (enter == "SHOW ROOM")
			server.ShowRoom();
		else if (enter == "CLEAR") {
			system("cls");
			showMenu();
		}
	}
}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: change error code to suit your needs
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
			showMenu();
			Server server;
			thread t(Enter, ref(server));
			server.active();
			t.join();

        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
