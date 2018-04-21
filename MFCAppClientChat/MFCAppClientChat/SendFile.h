#pragma once
#include <string>
using namespace std;

#define LINK_FILE ""
#define SIZE 5000

class SendFile
{
public:
	SendFile();
	SendFile(string filename, int size);
	~SendFile();
	bool readfile();
	bool writefile(char* content);
	void CreateContent();
	string splitFileName(string pathfile);
	string filename;
	int size;
	char** content;
	bool StateSendFile;
	bool StateUserOnline;
	int noPackage;
	int surplus;
	int count;
};

