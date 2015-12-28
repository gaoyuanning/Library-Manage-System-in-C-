#ifndef READERDAO_H_INCLUDED
#define READERDAO_H_INCLUDED

#include "Reader.h"
#include "BorrowInfo.h"
#include "Log.h"
#include <iostream>

using namespace std;

const int READER_BLOCK_SIZE = 1;

void listAllReaders() {
	Reader reader;
	FILE *readerFile = fopen("reader", "rb");
	fread(&reader, READER_SIZE, 1, readerFile);
	while (!feof(readerFile)) {
		if (reader.isExist) {
			printReaderInfo(reader);
		}
		fread(&reader, READER_SIZE, 1, readerFile);
	}
}

void initReader(Reader &reader) {
	FILE *readerFile = fopen("reader", "rb");
	fread(&reader, READER_SIZE, 1, readerFile);
	while (!feof(readerFile)) {
		if (reader.id == id) {
			break;
		}
		fread(&reader, READER_SIZE, 1, readerFile);
	}
}

bool checkIsBorrowBook(int id) {
	FILE *borrowInfoFile = fopen("borrowinfo", "rb");
	BorrowInfo borrowInfo;
	fread(&borrowInfo, sizeof(BorrowInfo), 1, borrowInfoFile);
	bool isFind = false;
	long position = -1;
	while (!feof(borrowInfoFile) && !isFind) {
		if (borrowInfo.id == id) {
			isFind = true;
			position = ftell(borrowInfoFile) - sizeof(BorrowInfo);
			break;
		}
		fread(&borrowInfo, sizeof(BorrowInfo), 1, borrowInfoFile);
	}

	if (!isFind) {
		return false;
	}

	if (position != -1) {
		fseek(borrowInfoFile, position, SEEK_SET);
		fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
		while (borrowInfo.index != -1) {
			if (borrowInfo.isReturn == 0) {
				return true;
			}
			fseek(borrowInfoFile, borrowInfo.index, SEEK_SET);
			fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
		}
		if (borrowInfo.isReturn == 0) {
			return true;
		}
	}
	fclose(borrowInfoFile);

	return false;
}

long findReader(int id, Reader &reader) {

	FILE *readerFile = fopen("reader", "rb");
	Reader readers[READER_BLOCK_SIZE];
	fseek(readerFile, 0, SEEK_SET);
	fread(readers, READER_SIZE, READER_BLOCK_SIZE, readerFile);
	while (!feof(readerFile)) {
		for (int i = 0; i < READER_BLOCK_SIZE; i++) {
			if (readers[i].id == id && readers[i].isExist) {
				reader.id = readers[i].id;
				reader.isExist = readers[i].isExist;
				strcpy(reader.major, readers[i].major);
				strcpy(reader.name, readers[i].name);
				strcpy(reader.password, readers[i].password);
				return ftell(readerFile) - READER_SIZE*(READER_BLOCK_SIZE - i);
			}
		}
		fread(readers, READER_SIZE, READER_BLOCK_SIZE, readerFile);
	}
	for (int i = 0; i < READER_BLOCK_SIZE; i++) {
		if (readers[i].id == id && readers[i].isExist) {
			reader.id = readers[i].id;
			reader.isExist = readers[i].isExist;
			strcpy(reader.major, readers[i].major);
			strcpy(reader.name, readers[i].name);
			strcpy(reader.password, readers[i].password);
			return ftell(readerFile) - READER_SIZE*(READER_BLOCK_SIZE - i);
		}
	}
	fclose(readerFile);
	
	return -1;
}

void addAdm() {
	Reader reader;
	cout << "������id��" << endl;
	cin >> reader.id;
	if (cin.fail()) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	int k = calIdLen(reader.id);

	if (k != 5) {
		cout << "��������" << endl;
		return;
	}
	else {
		cin.clear();
		cin.sync();
	}
	cout << "������������" << endl;
	char buffer[500];
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "����̫�����쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(reader.name, buffer);

	char major[20] = "����Ա";
	strcpy(reader.major, major);
	sprintf(reader.password, "%d", reader.id);
	reader.isExist = 1;

	if (findReader(reader.id, reader) != -1) {
		cout << "���û�����" << endl;
		return;
	}

	FILE *readerFile = fopen("reader", "ab+");
	fwrite(&reader, READER_SIZE, 1, readerFile);
	fclose(readerFile);
	cout << "������" << endl;
}

void addReader() {
	char buffer[500];
	Reader reader;
	cout << "������id��" << endl;
	cin >> reader.id;
	if (cin.fail()) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();
	int k = calIdLen(reader.id);

	if (k < 6 || k > 8) {
		cout << "��������" << endl;
		return;
	}
	else {
		cin.clear();
		cin.sync();
	}
	cout << "������������" << endl;
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "����̫�����쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(reader.name, buffer);

	cout << "������רҵ��" << endl;
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "����̫�����쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(reader.major, buffer);
	sprintf(reader.password, "%d", reader.id);
	reader.isExist = 1;

	if (findReader(reader.id, reader) != -1) {
		cout << "���û�����" << endl;
		return;
	}

	FILE *readerFile = fopen("reader", "ab+");
	fwrite(&reader, READER_SIZE, 1, readerFile);
	fclose(readerFile);
	cout << "����û����" << endl;
}

void updateReaderInfo() {
	cout << "������ѧ���ţ�";
	int id;
	cin >> id;
	if (cin.fail()) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	char buffer[500];
	Reader reader;
	long position = findReader(id, reader);
	if (position != -1) {
		printReaderInfo(reader);
		cout << "��ѡ��Ҫ���ĵ���Ϣ��" << endl;
		cout << "1:����" << endl;
		cout << "2:רҵ" << endl;
		cout << "3:����" << endl;
		cout << "��ѡ��" << endl;
		int k = 0;
		cin >> k;
		if (k >= 1 && k <= 3) {
			cin.clear();
			cin.sync();
			switch (k)
			{
			case 1:
				cout << "������������";
				cin >> buffer;
				if (strlen(buffer) > 20) {
					cout << "����̫�����쳣�˳�" << endl;
					cin.clear();
					cin.sync();
					return;
				}
				strcpy(reader.name, buffer);
				break;
			case 2:
				cout << "������רҵ��";
				cin >> buffer;
				if (strlen(buffer) > 20) {
					cout << "����̫�����쳣�˳�" << endl;
					return;
				}
				strcpy(reader.major, buffer);
				break;
			case 3:
				cout << "���븴λΪѧ��" << endl;
				sprintf(reader.password, "%d", reader.id);
				break;
			default:
				cout << "�������" << endl;
				return;
				break;
			}
			cin.clear();
			cin.sync();
		}
		else if (cin.fail()) {
			cout << "���������쳣�˳�" << endl;
			cin.clear();
			cin.sync();
		}
		k = -1;
		FILE *readerFile = fopen("reader", "rb+");
		fseek(readerFile, position, SEEK_SET);
		fwrite(&reader, READER_SIZE, 1, readerFile);
		fclose(readerFile);
		cout << "���ĳɹ�" << endl;
	}
}

void deleteReader() {
	cout << "������ѧ���ţ�";
	int uid;
	cin >> uid;
	if (cin.fail()) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	else {
		cin.clear();
		cin.sync();
	}
	if (uid == 12345) {
		cout << "����ɾ��Ĭ�Ϲ���Ա" << endl;
		return;
	}
	if (checkIsBorrowBook(uid)) {
		cout << "�ö�����ͼ����δ�黹��������ɾ��" << endl;
		return;
	}
	else {
		Reader reader;
		long position = findReader(uid, reader);
		if (position != -1) {
			reader.isExist = 0;
			FILE *readerFile = fopen("reader", "rb+");
			fseek(readerFile, position, SEEK_SET);
			fwrite(&reader, READER_SIZE, 1, readerFile);
			fclose(readerFile);
			cout << "ɾ���ɹ�" << endl;
		}
		else {
			cout << "���û�������" << endl;
		}
	}
}

bool transferLetterToPwd(char *password) {
	int i = 0;
	char ch;
	while ((ch = _getch()) != '\r') {
		if (ch == '\b' && i > 0) {
			printf("\b \b");
			--i;
		}
		else if (ch != '\b') {
			if (i > 14) {
				return false;
			}
			password[i++] = ch;
			printf("*");
		}
	}
	password[i] = '\0';
	printf("\n");
	if (i == 0) {
		return false;
	}
	return true;
}

void changePasswd() {
	if (checkLog()) {
		cout << "������ԭʼ���룺";
		char pwd[15];
		if (!transferLetterToPwd(pwd)) {
			cout << endl << "������������" << endl;
		}
		if (strcmp(pwd, password) == 0) {
			cout << "������������(���15λ)��";
			char pwd1[15];
			if (!transferLetterToPwd(pwd1)) {
				cout << endl << "������������" << endl;
				return;
			}
			cout << "���ٴ����룺";
			char pwd2[15];
			if (!transferLetterToPwd(pwd2)) {
				cout << endl << "������������" << endl;
			}
			if (strcmp(pwd1, pwd2) == 0) {
				cout << "1��ȷ���޸� 0�������޸�" << endl;
				int k;
				cin >> k;
				if (k == 1) {
					Reader reader;
					long position = findReader(id, reader);
					if (position != -1) {
						strcpy(reader.password, pwd2);
						FILE *readerFile = fopen("reader", "rb+");
						fseek(readerFile, position, SEEK_SET);
						fwrite(&reader, READER_SIZE, 1, readerFile);
						fclose(readerFile);
						cout << "���ĳɹ�" << endl;
					}
				}
				else if (cin.fail()){
					cout << "��������" << endl;
					cin.clear();
					cin.sync();
					return;
				}
			}
			else {
				cout << "�������벻һ��" << endl;
			}
		}
		else {
			cout << "�����������" << endl;
		}
	}
}

#endif