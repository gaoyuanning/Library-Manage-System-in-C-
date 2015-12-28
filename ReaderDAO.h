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
	cout << "请输入id：" << endl;
	cin >> reader.id;
	if (cin.fail()) {
		cout << "输入有误，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	int k = calIdLen(reader.id);

	if (k != 5) {
		cout << "输入有误" << endl;
		return;
	}
	else {
		cin.clear();
		cin.sync();
	}
	cout << "请输入姓名：" << endl;
	char buffer[500];
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "名字太长，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(reader.name, buffer);

	char major[20] = "管理员";
	strcpy(reader.major, major);
	sprintf(reader.password, "%d", reader.id);
	reader.isExist = 1;

	if (findReader(reader.id, reader) != -1) {
		cout << "该用户存在" << endl;
		return;
	}

	FILE *readerFile = fopen("reader", "ab+");
	fwrite(&reader, READER_SIZE, 1, readerFile);
	fclose(readerFile);
	cout << "添加完成" << endl;
}

void addReader() {
	char buffer[500];
	Reader reader;
	cout << "请输入id：" << endl;
	cin >> reader.id;
	if (cin.fail()) {
		cout << "输入有误，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();
	int k = calIdLen(reader.id);

	if (k < 6 || k > 8) {
		cout << "输入有误" << endl;
		return;
	}
	else {
		cin.clear();
		cin.sync();
	}
	cout << "请输入姓名：" << endl;
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "名字太长，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(reader.name, buffer);

	cout << "请输入专业：" << endl;
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "名字太长，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(reader.major, buffer);
	sprintf(reader.password, "%d", reader.id);
	reader.isExist = 1;

	if (findReader(reader.id, reader) != -1) {
		cout << "该用户存在" << endl;
		return;
	}

	FILE *readerFile = fopen("reader", "ab+");
	fwrite(&reader, READER_SIZE, 1, readerFile);
	fclose(readerFile);
	cout << "添加用户完成" << endl;
}

void updateReaderInfo() {
	cout << "请输入学工号：";
	int id;
	cin >> id;
	if (cin.fail()) {
		cout << "输入有误，异常退出" << endl;
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
		cout << "请选择要更改的信息：" << endl;
		cout << "1:姓名" << endl;
		cout << "2:专业" << endl;
		cout << "3:密码" << endl;
		cout << "请选择：" << endl;
		int k = 0;
		cin >> k;
		if (k >= 1 && k <= 3) {
			cin.clear();
			cin.sync();
			switch (k)
			{
			case 1:
				cout << "请输入姓名：";
				cin >> buffer;
				if (strlen(buffer) > 20) {
					cout << "名字太长，异常退出" << endl;
					cin.clear();
					cin.sync();
					return;
				}
				strcpy(reader.name, buffer);
				break;
			case 2:
				cout << "请输入专业：";
				cin >> buffer;
				if (strlen(buffer) > 20) {
					cout << "名字太长，异常退出" << endl;
					return;
				}
				strcpy(reader.major, buffer);
				break;
			case 3:
				cout << "密码复位为学号" << endl;
				sprintf(reader.password, "%d", reader.id);
				break;
			default:
				cout << "输入错误" << endl;
				return;
				break;
			}
			cin.clear();
			cin.sync();
		}
		else if (cin.fail()) {
			cout << "输入有误，异常退出" << endl;
			cin.clear();
			cin.sync();
		}
		k = -1;
		FILE *readerFile = fopen("reader", "rb+");
		fseek(readerFile, position, SEEK_SET);
		fwrite(&reader, READER_SIZE, 1, readerFile);
		fclose(readerFile);
		cout << "更改成功" << endl;
	}
}

void deleteReader() {
	cout << "请输入学工号：";
	int uid;
	cin >> uid;
	if (cin.fail()) {
		cout << "输入有误，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	else {
		cin.clear();
		cin.sync();
	}
	if (uid == 12345) {
		cout << "不能删除默认管理员" << endl;
		return;
	}
	if (checkIsBorrowBook(uid)) {
		cout << "该读者有图书尚未归还，不允许删除" << endl;
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
			cout << "删除成功" << endl;
		}
		else {
			cout << "该用户不存在" << endl;
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
		cout << "请输入原始密码：";
		char pwd[15];
		if (!transferLetterToPwd(pwd)) {
			cout << endl << "密码输入有误" << endl;
		}
		if (strcmp(pwd, password) == 0) {
			cout << "请输入新密码(最大15位)：";
			char pwd1[15];
			if (!transferLetterToPwd(pwd1)) {
				cout << endl << "密码输入有误" << endl;
				return;
			}
			cout << "请再次输入：";
			char pwd2[15];
			if (!transferLetterToPwd(pwd2)) {
				cout << endl << "密码输入有误" << endl;
			}
			if (strcmp(pwd1, pwd2) == 0) {
				cout << "1、确认修改 0、放弃修改" << endl;
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
						cout << "更改成功" << endl;
					}
				}
				else if (cin.fail()){
					cout << "输入有误" << endl;
					cin.clear();
					cin.sync();
					return;
				}
			}
			else {
				cout << "两次输入不一致" << endl;
			}
		}
		else {
			cout << "密码输入错误" << endl;
		}
	}
}

#endif