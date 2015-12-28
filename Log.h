#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include "Date.h"
#include "Reader.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

bool isLogged = false;
bool admLogged = false;
int id = 0;
char password[15];
Date date;

int calIdLen(int id) {
	int k = 0;
	int temp = id;
	while (temp) {
		temp = temp / 10;
		k++;
	}

	return k;
}

bool checkUserInfo(int stuID, char *pw) {
	Reader reader;
	bool isFind = false;
	FILE *readerFile = fopen("reader", "rb");
	fread(&reader, READER_SIZE, 1, readerFile);
	while (!feof(readerFile)) {
		if (reader.id == stuID && strcmp(reader.password, pw) == 0)  {
			if (reader.isExist == 1) {
				isFind = true;
			}
		}
		fread(&reader, READER_SIZE, 1, readerFile);
	}

	return isFind;
}

bool login() {
	
	int i, m = 0;
	char ch;
	while (m < 3) {
		cout << "ѧ���ţ�";
		cin >> id;
		if (cin.fail()) {
			cout << "��������" << endl;
			cin.clear();
			cin.sync();
			m++;
			continue;
		}
		cin.clear();
		cin.sync();

		int k = calIdLen(id);
		if (k < 6 || k > 8) {
			cout << "��������" << endl;
			m++;
			continue;
		}
		cin.clear();
		cin.sync();

		cout << "���룺";
		i = 0;
		while ((ch = _getch()) != '\r') {
			if (ch == '\b' && i > 0) {
				printf("\b \b");
				--i;
			}
			else if (ch != '\b') {
				if (i > 14) {
					cout << endl << "������������" << endl;
					m++;
					return false;
				}
				password[i++] = ch;
				printf("*");
			}
		}
		password[i] = '\0';
		printf("\n");
		if (!checkUserInfo(id, password)) {
			printf("��Ϣ������������������!\n");
			m++;
		}
		else {
			printf("��½�ɹ���\n");
			return true;
		}
	}
	printf("����3����������˳�!\n");
	return false;
}

bool admLogin() {

	int i, m = 0;
	char ch;
	while (m < 3) {
		cout << "����Ա�˺ţ�";
		cin >> id;
		if (cin.fail()) {
			cout << "��������" << endl;
			cin.clear();
			cin.sync();
			m++;
			continue;
		}
		cin.clear();
		cin.sync();

		int k = calIdLen(id);
		if (k != 5) {
			cout << "��������" << endl;
			m++;
			continue;
		}
		cin.clear();
		cin.sync();

		cout << "���룺";
		i = 0;
		while ((ch = _getch()) != '\r') {
			if (ch == '\b' && i > 0) {
				printf("\b \b");
				--i;
			}
			else if (ch != '\b') {
				if (i > 14) {
					cout << endl << "������������" << endl;
					m++;
					return false;
				}
				password[i++] = ch;
				printf("*");
			}
		}
		password[i] = '\0';
		printf("\n");
		if (!checkUserInfo(id, password)) {
			printf("��Ϣ������������������!\n");
			m++;
		}
		else {
			printf("��½�ɹ���\n");
			return true;
		}
	}
	printf("����3����������˳�!\n");
	return false;
}

bool checkLog() {
	if (!isLogged) {
		cout << "���¼" << endl;
		if (login()) {
			isLogged = true;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
}
bool checkLog2() {
	if (!admLogged) {
		cout << "���¼" << endl;
		if (admLogin()) {
			admLogged = true;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
}
#endif
