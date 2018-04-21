#include "Menu.h"

void BorderTop(int size) {
	cout << (char)201;
	for (int i = 0; i < size; i++)
		cout << (char)205;
	cout << (char)187 << endl;
}

void BorderBottom(int size) {
	cout << (char)200;
	for (int i = 0; i < size; i++)
		cout << (char)205;
	cout << (char)188 << endl;
}

void WriteInBoard(string buf, int size) {
	if (buf.length() > size - 2)
		buf = "Not Enough Space";
	cout << (char)186 << "  " << buf;
	for (int i = 0; i < (size - buf.length() - 2); i++)
		cout << " ";
	cout << (char)186 << endl;
}

void showMenu() {
	int size = 50;
	BorderTop(size);
	WriteInBoard("1.Show log", size);
	WriteInBoard("2.Show user online", size);
	WriteInBoard("3.Send to client", size);
	WriteInBoard("4.Show room", size);
	BorderBottom(size);
}