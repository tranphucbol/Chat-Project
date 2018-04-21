#include "Account.h"

// ACCOUNT LIST AREA

string encryptMsgLogin(string username, string password) {
	string format = "dn";
	int pos = username.length();
	format += (char)pos;
	return username + password + format;
}

void decryptMsgLogin(string msg, string &username, string &password) {
	int pos = (int)msg[msg.length() - 1];
	username = msg.substr(0, pos);
	password = msg.substr(pos, msg.length() - pos - 3);
	msg = msg.substr(msg.length() - 3, msg.length() - 1);
}

string hash_function(char* pass) {
	int number;
	char _passw[21];
	string p = "";
	for (int i = 0; i < 20; i++) {
		number = (((int)pass[i] % 13) + 35 + i*i);
		p += (char)number;
	}
	return p;
}


AccountList::AccountList() {
	arr.resize(0);
	readAllFile();
	return;
}

int AccountList::getID(char* username) {
	for (int i = 0; i < arr.size(); i++) {
		if (strcmp(arr[i].getUsername(),username) == 0)
			return i;
	}
	return -1;
}

void AccountList::display() {
	cout << "\t\t --- ACCOUNT LIST: --- " << endl;
	for (int i = 0; i < arr.size(); i++) {
		cout << "Account " << i + 1 << ": " << endl;
		arr[i].display();
	}
}


bool AccountList::signIn(char* username, char* passw) {
	for (int i = 0; i < arr.size(); i++) {
		if (strcmp(arr[i].getUsername(), username) == 0) {
			// hash password
			char password[21];
			string p = "";
			p = hash_function(passw);
			strcpy_s(password, p.c_str());
			//

			if (strcmp(arr[i].getPassw(), password) == 0)
				return true;
			return false;
		}
	}
	return false;
}

bool AccountList::signUp(char* username, char* passw) {
	for (int i = 0; i < arr.size(); i++) {
		if (strcmp(arr[i].getUsername(), username) == 0) // The username has existed
			return false;
	}
	
	//Hash password
	char password[21];
	string p = "";
	p = hash_function(passw);
	strcpy_s(password, p.c_str()); //pw[20+1] = '\0'

	//

	user newUser;
	newUser.input(username, password);
	arr.push_back(newUser);
	//newUser.display();
	writeFile(newUser);
	return true;
}

bool AccountList::updatePassw(char* username, char* passw) {
	for (int i = 0; i < arr.size(); i++) {
		if (strcmp(arr[i].getUsername(),username) == 0) {
			// hash password
			char password[21];
			string p = "";
			p = hash_function(passw);
			strcpy_s(password, p.c_str());
			//

			arr[i].updatePassw(password);
			updatePassInFile(arr[i], i);
			return true;
		}
	}

	return false;
}

void AccountList::readAllFile() {
	ifstream fi("user.dat", ios::binary);
	user User;
	while(fi.read(reinterpret_cast<char*>(&User), sizeof(user))) {
		arr.push_back(User);
	}

	fi.close();
	return;
}


void AccountList::writeFile(user User) {
	ofstream fo("user.dat", ios::binary | ios::app);
	fo.write(reinterpret_cast<char*>(&User), sizeof(user));
	fo.close();
	return;
}


void AccountList::updatePassInFile(user User, int id) {
	if (id == -1)
		return;

	fstream fi("user.dat", ios::in | ios::out | ios::binary);
	fi.seekp(sizeof(user)*id);
	fi.write(reinterpret_cast<char*>(&User), sizeof(user));
	fi.close();
	return;
}

