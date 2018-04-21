#include "stdafx.h"
#include "SendFile.h"
#include <fstream>


SendFile::SendFile()
{
	content = NULL;
	StateSendFile = false;
	StateUserOnline = false;
	size = 0;
	count = 0;
}

SendFile::SendFile(string filename, int size) {
	this->filename = filename;
	this->size = size;
	content = NULL;
	StateSendFile = false;
}


SendFile::~SendFile()
{
	for(int i=0; i<noPackage; i++)
		delete[]content[i];
	delete[]content;
}

bool SendFile::readfile() {
	ifstream fi(filename, ios::ate | ios::binary);
	if (!fi.is_open()) {
		fi.close();
		return false;
	}
	size = fi.tellg();

	CreateContent();

	fi.seekg(0);

	//read
	for (int i = 0; i < noPackage - 1; i++) {
		content[i][0] = 'b';
		content[i][1] = 'o';
		content[i][2] = 'f';
		fi.read(content[i] + 3, SIZE);
		content[i][SIZE + 3] = 'b';
		content[i][SIZE + 4] = 'o';
		content[i][SIZE + 5] = 'f';
	}
	content[noPackage - 1][0] = 'b';
	content[noPackage - 1][1] = 'o';
	content[noPackage - 1][2] = 'f';
	fi.read(content[noPackage - 1] + 3, surplus == 0 ? SIZE : surplus);
	content[noPackage - 1][SIZE + 3] = 'z';
	content[noPackage - 1][SIZE + 4] = 'z';
	content[noPackage - 1][SIZE + 5] = 'z';

	fi.close();
	return true;
}

bool SendFile::writefile(char* content) {
	int n = SIZE + 6;
	if (count == noPackage - 1)
		n = ((surplus == 0) ? SIZE : surplus) + 6;
	for (int i = 0; i < n; i++)
		this->content[count][i] = content[i];
	count++;
	if (count < noPackage) return false;
	string fname = LINK_FILE + this->filename;
	ofstream fo(fname, ios::binary);
	for (int i = 0; i < noPackage - 1; i++) {
		fo.write(this->content[i] + 3, SIZE);
	}
	fo.write(this->content[noPackage - 1] + 3, surplus == 0 ? SIZE : surplus);
	fo.close();
	return true;
}

string SendFile::splitFileName(string pathfile) {
	int index = -1;
	for (int i = pathfile.length() - 1; i >= 0; i--) {
		if (pathfile[i] == (char)47) {
			index = i;
			break;
		}
	}
	return pathfile.substr(index + 1, pathfile.length() - (index + 1));
}

void SendFile::CreateContent() {
	if (size == 0)
		return;
	surplus = size % SIZE;
	noPackage = size / SIZE + ((surplus == 0) ? 0 : 1);

	content = new char*[noPackage];
	for (int i = 0; i < noPackage - 1; i++)
		content[i] = new char[SIZE + 6];
	content[noPackage - 1] = new char[((surplus == 0) ? SIZE : surplus) + 6];
}