#pragma once
#ifndef ACC_H
#define ACC_H
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

string hash_function(char* pass);
void decryptMsgLogin(string msg, string &username, string &password);
string encryptMsgLogin(string username, string password);

class user {
private:
	char _username[20];
	char _passw[21];

public:
	void input(char* username, char* passw) {
		strcpy_s(_username, username);
		strcpy_s(_passw, passw);
	}

	void updatePassw(char* passw) {
		strcpy_s(_passw, passw);
	}

	char* getUsername() {
		return _username;
	}
	char* getPassw() {
		return _passw;
	}
	void display() {
		cout << "Username: " << _username << endl;
		cout << "Password: " << _passw << endl;
	}
};


class AccountList {
private:
	vector <user> arr;
public:
	AccountList();

	bool signIn(char* username, char* passw);
	bool signUp(char* username, char* passw); 
	bool updatePassw(char* username, char* passw);
	void display();
	void readAllFile();
	void writeFile(user User);
	void updatePassInFile(user User, int id);
	int getID(char* username);

};










#endif