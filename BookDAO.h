#ifndef BOOKDAO_H_INCLUDED
#define BOOKDAO_H_INCLUDED

#include "Book.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

using namespace std;

const int BOOK_SIZE = sizeof(Book);
const int BOOKINFO_SIZE = sizeof(BookInfo);
const int BLOCK_SIZE = 1;
const int N = 100;//��ISBN�õ�����ŵĳ˵Ļ�����С

long searchByISBN(long ISBN, BookInfo &bookInfo) {
	FILE *bookInfoFile = fopen("bookinfo", "rb");
	bool isFind = false;
	BookInfo bookInfos[BLOCK_SIZE];
	//bookInfo.ISBN = -1;
	long position;
	long location;
	fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
	while (!feof(bookInfoFile) && !isFind) {
		for (int i = 0; i < BLOCK_SIZE; i++) {
			if (bookInfos[i].exist == 1 && bookInfos[i].ISBN == ISBN) {
				isFind = true;
				position = bookInfos[i].index;
				bookInfo.ISBN = bookInfos[i].ISBN;
				bookInfo.price = bookInfos[i].price;
				bookInfo.index = position;
				bookInfo.exist = 1;
				strcpy(bookInfo.name, bookInfos[i].name);
				strcpy(bookInfo.press, bookInfos[i].press);
				strcpy(bookInfo.writer, bookInfos[i].writer);
				location = ftell(bookInfoFile) - BOOKINFO_SIZE*(BLOCK_SIZE - i);
				break;
			}
		}
		fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
	}

	fclose(bookInfoFile);
	if (isFind) {
		return location;
	}
	else {
		cout << "ͼ��ݲ����ڸ�����" << endl;
		return -1;
	}
}


void save(BookInfo bookInfo, int num) { //num��ʾ��ı���
	FILE *bookFile = fopen("book", "ab+");
	fseek(bookFile, 0, SEEK_END);
	int position = ftell(bookFile);
	bookInfo.index = position;
	Book book;
	book.exist = 1;
	book.flag = 1;

	for (int i = 0; i < num; i++) {
		book.callnumber = bookInfo.ISBN * N + i;
		if (i == num - 1) {
			book.next = -1;
		}
		else {
			book.next = position + BOOK_SIZE;
		}
		fwrite(&book, BOOK_SIZE, 1, bookFile);
		position = ftell(bookFile);
	}
	fclose(bookFile);

	FILE *bookInfoFile = fopen("bookinfo", "ab+");
	fseek(bookInfoFile, 0, SEEK_END);
	fwrite(&bookInfo, BOOKINFO_SIZE, 1, bookInfoFile);
	fclose(bookInfoFile);
	cout << "ͼ��������" << endl;
}

void addNewBooks() {
	char buffer[500];
	BookInfo bookInfo;
	cout << "������������";
	cin >> buffer;
	if (strlen(buffer) > 30) {
		cout << "����̫�����޷��洢" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(bookInfo.name, buffer);
	cout << "���������ߣ�";
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "����̫�����޷��洢" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(bookInfo.writer, buffer);
	cout << "����������磺";
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "����̫�����޷��洢" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(bookInfo.press, buffer);
	cout << "������۸�";
	cin >> bookInfo.price;
	if (cin.fail()) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	
	cin.clear();
	cin.sync();

	cout << "������ISBN(���8λ)��";
	cin >> bookInfo.ISBN;
	if (cin.fail() || bookInfo.ISBN > 20000000) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	cout << "������¼�뱾��(���100��)��";
	int num;
	cin >> num;
	if (cin.fail() || num > 100) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	bookInfo.exist = 1;

	cout << "��Ϣ¼����ϣ���ȷ��:" << endl;
	cout << "ȷ��¼�룺1   ��������¼�룺0" << endl;
	cout << "��ѡ��:";
	int isSave;
	cin >> isSave;
	if (cin.fail() || (isSave != 0 && isSave != 1)) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	long position = searchByISBN(bookInfo.ISBN, bookInfo);
	if (position != -1) {
		cout << "�����Ѵ��ڣ����ʧ��" << endl;
		return;
	}
	if (isSave == 1) {
		save(bookInfo, num);
	}
}

void addBooksByBatch(int k) {
	for (int i = 1; i < k; i++) {
		BookInfo bookInfo;
		sprintf(bookInfo.name, "%d", i);
		sprintf(bookInfo.writer, "%d", i);
		sprintf(bookInfo.press, "%d", i);
		bookInfo.ISBN = i;
		bookInfo.price = 20;
		bookInfo.exist = 1;
		save(bookInfo, 5);
	}
}

//ͨ��position��λͼ��ͼ��,����ֵΪ�ɽ�����
int locationBook(BookInfo bookInfo) {
	long position = bookInfo.index;

	FILE *bookFile = fopen("book", "rb");
	fseek(bookFile, position, SEEK_SET);
	Book book;
	int bookCount = 0;
	int borrowQuantity = 0;
	fread(&book, BOOK_SIZE, 1, bookFile);
	while (book.next != -1) {
		if (book.exist) {
			bookCount++;
			if (book.flag == 1) {
				borrowQuantity += 1;
			}
		}
		fseek(bookFile, book.next, SEEK_SET);
		fread(&book, BOOK_SIZE, 1, bookFile);
	}

	if (book.next == -1 && book.exist) {
		bookCount++;
		if (book.flag == 1) {
			borrowQuantity += 1;
		}
	}

	fclose(bookFile);

	cout << "ͼ����Ϣ��" << endl;
	cout << "������" << bookInfo.name << endl;
	cout << "���ߣ�" << bookInfo.writer << endl;
	cout << "�����磺" << bookInfo.press << endl;
	cout << "ISBN��" << bookInfo.ISBN << endl;
	cout << "�۸�" << bookInfo.price << endl;
	cout << "��棺" << bookCount << endl;
	cout << "�ɽ�������" << borrowQuantity << endl;

	return borrowQuantity;
}

//���ؿɽ�����
int listBookInfo(BookInfo bookInfo) {

	cout << "ͼ����Ϣ��" << endl;
	cout << "������" << bookInfo.name << endl;
	cout << "���ߣ�" << bookInfo.writer << endl;
	cout << "�����磺" << bookInfo.press << endl;
	cout << "ISBN��" << bookInfo.ISBN << endl;
	cout << "�۸�" << bookInfo.price << endl;

	long position = bookInfo.index;
	FILE *bookFile = fopen("book", "rb");
	fseek(bookFile, position, SEEK_SET);
	Book book;
	int bookCount = 0;
	int borrowQuantity = 0;
	fread(&book, BOOK_SIZE, 1, bookFile);
	while (book.next != -1) {
		if (book.exist == 1) {
			cout << "����ţ�" << book.callnumber << " ";
			bookCount++;
			cout << "�Ƿ����� ";
			if (book.flag == 1) {
				borrowQuantity += 1;
				cout << "�ڹ�" << endl;
			}
			else {
				cout << "���" << endl;
			}
		}
		fseek(bookFile, book.next, SEEK_SET);
		fread(&book, BOOK_SIZE, 1, bookFile);
	}

	if (book.next == -1 && book.exist == 1) {
		cout << "����ţ�" << book.callnumber << " ";
		bookCount++;
		cout << "�Ƿ����� ";
		if (book.flag == 1) {
			borrowQuantity += 1;
			cout << "�ڹ�" << endl;
		}
		else {
			cout << "���" << endl;
		}
	}
	fclose(bookFile);

	cout << "��棺" << bookCount << endl;
	cout << "�ɽ�������" << borrowQuantity << endl << endl;

	return borrowQuantity;
}


//����ֵΪ��һ�����λ��
long searchByBookName(char *name, bool isAccurate, vector<BookInfo> &bookInfoArr) {
	long position = 0;
	if (isAccurate) {
		FILE *bookInfoFile = fopen("bookinfo", "rb");
		bool isFind = false;
		BookInfo bookInfos[BLOCK_SIZE];
		BookInfo bookInfo;
		fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
		int i = 0;
		while (!feof(bookInfoFile) && !isFind) {
			for (i = 0; i < BLOCK_SIZE; i++) {
				if (bookInfos[i].exist == 1 && strcmp(name, bookInfos[i].name) == 0) {
					isFind = true;
					position = bookInfos[i].index;
					bookInfo.ISBN = bookInfos[i].ISBN;
					bookInfo.price = bookInfos[i].price;
					bookInfo.index = position;
					bookInfo.exist = 1;
					strcpy(bookInfo.name, bookInfos[i].name);
					strcpy(bookInfo.press, bookInfos[i].press);
					strcpy(bookInfo.writer, bookInfos[i].writer);
					break;
				}
			}
			fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
		}
		
		if (isFind) {
//locationBook(bookInfo);
//listBookInfo(bookInfo);
			bookInfoArr.push_back(bookInfo);
			return (ftell(bookInfoFile) - BOOKINFO_SIZE - BOOKINFO_SIZE * (BLOCK_SIZE -i));
		}
		else {
			cout << "�Բ��𣬸��鲻����" << endl;
			return -1;
		}

		fclose(bookInfoFile);

	}
	//ģ������
	else {
		FILE *bookInfoFile = fopen("bookinfo", "rb");
		bool isFind = false;
		BookInfo bookInfos[BLOCK_SIZE];
		BookInfo bookInfo;
		fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
		while (!feof(bookInfoFile)) {
			for (int i = 0; i < BLOCK_SIZE; i++) {
				if (bookInfos[i].exist == 1 && string(bookInfos[i].name).find(name) != std::string::npos){
					isFind = true;
					position = bookInfos[i].index;
					bookInfo.ISBN = bookInfos[i].ISBN;
					bookInfo.price = bookInfos[i].price;
					bookInfo.index = position;
					bookInfo.exist = 1;
					strcpy(bookInfo.name, bookInfos[i].name);
					strcpy(bookInfo.press, bookInfos[i].press);
					strcpy(bookInfo.writer, bookInfos[i].writer);
					bookInfoArr.push_back(bookInfo);
				}
			}
			fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
		}
		fclose(bookInfoFile);

		if (!isFind) {
			cout << "�Բ��𣬸��鲻����" << endl;
		}
	}

	return -1;
}

void searchByWriter(char *name, vector<BookInfo> &bookInfoArr) {
	long position = 0;
	FILE *bookInfoFile = fopen("bookinfo", "rb");
	BookInfo bookInfos[BLOCK_SIZE];
	BookInfo bookInfo;
	fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
	while (!feof(bookInfoFile)) {
		for (int i = 0; i < BLOCK_SIZE; i++) {
			if (bookInfos[i].exist == 1 && strcmp(name, bookInfos[i].writer) == 0) {
				position = bookInfos[i].index;
				bookInfo.ISBN = bookInfos[i].ISBN;
				bookInfo.price = bookInfos[i].price;
				bookInfo.index = position;
				bookInfo.exist = 1;
				strcpy(bookInfo.name, bookInfos[i].name);
				strcpy(bookInfo.press, bookInfos[i].press);
				strcpy(bookInfo.writer, bookInfos[i].writer);
				bookInfoArr.push_back(bookInfo);
				break;
			}
		}
		fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
	}
	fclose(bookInfoFile);

	if (bookInfoArr.size() == 0) {
		cout << "û���ҵ������ߵ���" << endl;
	}
}

void searchBook(char *name, int mode, bool isAccurate, vector<BookInfo> &bookInfoArr) {
	//mode = 0 ���������ң�mode = 1 �����߲���
	// isAccurate = true ��׼���ң�isAccurate = false ģ����ѯ
	if (mode == 0) {
		searchByBookName(name, isAccurate, bookInfoArr);
	}
	else if (mode == 1){
		searchByWriter(name, bookInfoArr);
	}
}

void search(int mode, bool accurate, vector<BookInfo> &bookInfoArr) {
	cout << "�����룺";
	char name[500];
	cin >> name;
	if (strlen(name) > 30) {
		cout << "����̫�����쳣�˳�" << endl;
		return;
		//exit(-1);
	}
	searchBook(name, mode, accurate, bookInfoArr);
}

void updateBookAfterReturn(long callnumber) {
	long ISBN = callnumber / N;
	BookInfo bookInfo;
	searchByISBN(ISBN, bookInfo);
	if (bookInfo.ISBN == ISBN) {
		long location = 0;
		bool isFind = false;
		long position = bookInfo.index;

		FILE *bookFile = fopen("book", "rb+");
		fseek(bookFile, position, SEEK_SET);
		Book book;
		fread(&book, BOOK_SIZE, 1, bookFile);
		while (book.next != -1 && !isFind) {
			if (book.exist == 1 && book.callnumber == callnumber) {
				isFind = true;
				location = ftell(bookFile) - BOOK_SIZE;
				break;
			}
			fseek(bookFile, book.next, SEEK_SET);
			fread(&book, BOOK_SIZE, 1, bookFile);
		}

		if (book.next == -1 && !isFind && book.exist == 1) {
			if (book.callnumber == callnumber) {
				isFind = true;
				location = ftell(bookFile) - BOOK_SIZE;
			}
		}

		if (isFind) {
			book.flag = 1;
			fseek(bookFile, location, SEEK_SET);
			fwrite(&book, BOOK_SIZE, 1, bookFile);
		}

		fclose(bookFile);
	}
}

long updateBookAfterBorrow(BookInfo bookInfo) {
	long callnumber = 0;
	long index = bookInfo.index;

	FILE *bookFile = fopen("book", "rb+");
	fseek(bookFile, index, SEEK_SET);
	Book tempBook;
	fread(&tempBook, BOOK_SIZE, 1, bookFile);
	while (tempBook.next != -1) {
		if (tempBook.exist == 1 && tempBook.flag == 1) {
			index = ftell(bookFile) - BOOK_SIZE;
			break;
		}
		fread(&tempBook, BOOK_SIZE, 1, bookFile);
	}

	tempBook.flag = 0;
	fseek(bookFile, index, SEEK_SET);
	fwrite(&tempBook, BOOK_SIZE, 1, bookFile);
	fclose(bookFile);

	callnumber = tempBook.callnumber;
    cout << "���ĳɹ�" <<endl;
	return callnumber;
}

void updateBookInfo() {
	cout << "������ISBN��";
	long ISBN;
	cin >> ISBN;
	if (cin.fail() || ISBN > 20000000) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	BookInfo bookInfo;
	long position = searchByISBN(ISBN, bookInfo);
	if (position != -1) {
		printBookInfo(bookInfo);
		cout << "��ѡ����Ҫ���ĵ���Ŀ��";
		int a;
		cin >> a;
		if (a) {
			cin.clear();
			cin.sync();
			char buffer[500];
			switch (a) {
			case 1:
				cout << "������������";
				cin >> buffer;
				if (strlen(buffer) > 30) {
					cout << "����̫�����쳣�˳�" << endl;
					cin.clear();
					cin.sync();
					return;
				}
				strcpy(bookInfo.name, buffer);
				break;
			case 2:
				cout << "���������ߣ�";
				cin >> buffer;
				if (strlen(buffer) > 20) {
					cout << "����̫�����쳣�˳�" << endl;
					cin.clear();
					cin.sync();
					return;
				}
				strcpy(bookInfo.writer, buffer);
				break;
			case 3:
				cout << "����������磺";
				cin >> buffer;
				if (strlen(buffer) > 20) {
					cout << "����̫�����쳣�˳�" << endl;
					cin.clear();
					cin.sync();
					return;
				}
				strcpy(bookInfo.press, buffer);
				break;
			case 4:
				cout << "������۸�";
				cin >> bookInfo.price;
				if (cin.fail()) {
					cin.clear();
					cin.sync();
					cout << "���������쳣�˳�" << endl;
				}
				break;
			default:
				cout << "��������" << endl;
				break;
			}
			cin.clear();
			cin.sync();
		} else if (cin.fail()) {
			cout << "���������쳣�˳�" << endl;
			cin.clear();
			cin.sync();
			return;
		}
		
		//��д��һ���������ļ����������д
		FILE *bookInfoFile = fopen("bookinfo", "rb+");
		fseek(bookInfoFile, position, SEEK_SET);
		fwrite(&bookInfo, BOOKINFO_SIZE, 1, bookInfoFile);
		fclose(bookInfoFile);

		cout << "���³ɹ�" << endl;
	}
}


//����Ѿ����ڵ���
void addOldBooks() {
	cout << "���������ISBN��";
	long ISBN;
	cin >> ISBN;
	if (cin.fail() || ISBN > 20000000) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	BookInfo bookInfo;
	searchByISBN(ISBN, bookInfo);

	if (bookInfo.ISBN == ISBN) {
		long position = bookInfo.index;
		printBookInfo(bookInfo);
		//Ѱ��ԭ�ȵ����һ����
		FILE *bookFile = fopen("book", "rb+");
		fseek(bookFile, 0, SEEK_END);
		long location = ftell(bookFile);
		Book book;
		fseek(bookFile, position, SEEK_SET);
		fread(&book, BOOK_SIZE, 1, bookFile);
		while (book.next != -1) {
			fread(&book, BOOK_SIZE, 1, bookFile);
		}
		if (book.next == -1) {

			int curCount = book.callnumber - ISBN * N + 1;
			cout << "��������ӵ�������";
			int cnt;
			cin >> cnt;
			if (cin.fail() || (cnt+curCount) > 100) {
				cout << "���������쳣�˳�" << endl;
				cin.clear();
				cin.sync();
				return;
			}
			cin.clear();
			cin.sync();

			book.next = location;
			fseek(bookFile, (-BOOK_SIZE), SEEK_CUR);
			fwrite(&book, BOOK_SIZE, 1, bookFile);
			fseek(bookFile, 0, SEEK_END);
			location = ftell(bookFile);
			book.exist = 1;
			book.flag = 1;
			for (int i = 0; i < cnt; i++) {
				book.callnumber += 1;
				if (i == cnt - 1) {
					book.next = -1;
				}
				else {
					book.next = location + BOOK_SIZE;
				}
				fwrite(&book, BOOK_SIZE, 1, bookFile);
				location = ftell(bookFile);
			}
			fclose(bookFile);
			cout << "��ӳɹ�" << endl;
		}
		else {
			cout << "���ҳ���" << endl;
		}
	}
}

void listAllBooks() {
	FILE *bookInfoFile = fopen("bookinfo", "rb");
	BookInfo bookInfos[BLOCK_SIZE];
	BookInfo bookInfo;
	fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
	while (!feof(bookInfoFile)) {
		for (int i = 0; i < BLOCK_SIZE; i++) {
			if (bookInfos[i].exist == 1) {
				bookInfo.ISBN = bookInfos[i].ISBN;
				bookInfo.price = bookInfos[i].price;
				bookInfo.exist = bookInfos[i].exist;
				bookInfo.index = bookInfos[i].index;
				strcpy(bookInfo.name, bookInfos[i].name);
				strcpy(bookInfo.press, bookInfos[i].press);
				strcpy(bookInfo.writer, bookInfos[i].writer);
				listBookInfo(bookInfo);
			}
			
		}
		fread(bookInfos, BOOKINFO_SIZE, BLOCK_SIZE, bookInfoFile);
	}
}

//ͨ�������ɾ��ͼ��
void deleteByCallNumber() {
	cout << "����������Ҫɾ��ͼ��������:";
	long callnumber;
	cin >> callnumber;
	if (cin.fail() || callnumber > 2000000000) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	long ISBN = callnumber / N;
	BookInfo bookInfo;
	searchByISBN(ISBN, bookInfo);
	if (bookInfo.ISBN == ISBN) {

		long location = 0;
		bool isFind = false;

		cout << "ͼ����Ϣ��" << endl;
		cout << "������" << bookInfo.name << endl;
		cout << "���ߣ�" << bookInfo.writer << endl;
		cout << "�����磺" << bookInfo.press << endl;
		cout << "ISBN��" << bookInfo.ISBN << endl;
		cout << "�۸�" << bookInfo.price << endl;
		long position = bookInfo.index;
		FILE *bookFile = fopen("book", "rb+");
		fseek(bookFile, position, SEEK_SET);
		Book book;
		fread(&book, BOOK_SIZE, 1, bookFile);
		while (book.next != -1 && !isFind) {
			if (book.exist == 1 && book.callnumber == callnumber) {
				isFind = true;
				location = ftell(bookFile) - BOOK_SIZE;
				break;
			}
			fseek(bookFile, book.next, SEEK_SET);
			fread(&book, BOOK_SIZE, 1, bookFile);
		}

		if (book.next == -1 && !isFind && book.exist == 1) {
			if (book.callnumber == callnumber) {
				isFind = true;
				location = ftell(bookFile) - BOOK_SIZE;
			}
		}

		if (isFind) {
			cout << "����ţ�" << book.callnumber << endl;
			cout << "ȷ��ɾ����1���˳���0" << endl;
			cout << "��ѡ��";
			int a;
			cin >> a;
			if (cin.fail()) {
				cout << "���������쳣�˳�" << endl;
				cin.clear();
				cin.sync();
				return;
			} else if (a == 1) {
				if (book.flag) {
					book.exist = 0;
					fseek(bookFile, location, SEEK_SET);
					fwrite(&book, BOOK_SIZE, 1, bookFile);
					cout << "ɾ���ɹ�" << endl;
				}
				else {
					cout << "���鱻�����������ɾ��" << endl;
				}
				cin.clear();
				cin.sync();
			}
		}

		fclose(bookFile);
	}
}

//ͨ������ɾ��ͼ��
void deleteByBookName() {
	cout << "������������";
	char buffer[500];
	char name[30];
	cin >> buffer;
	if (strlen(buffer) > 30) {
		cout << "����̫�����쳣�˳�" << endl;
		return;
	}
	strcpy(name, buffer);
	vector<BookInfo> bookInfos;
	long position = searchByBookName(name, true, bookInfos);
	if (position != -1) {
		BookInfo bookInfo = bookInfos[0];
listBookInfo(bookInfo);
		cout << "����������Ҫɾ��ͼ��������:";
		long callnumber;
		cin >> callnumber;
		if (cin.fail() || callnumber > 2000000000) {
			cout << "���������쳣�˳�" << endl;
			cin.clear();
			cin.sync();
			return;
		}
		cin.clear();
		cin.sync();

		long position = bookInfo.index;
		FILE *bookFile = fopen("book", "rb+");
		fseek(bookFile, position, SEEK_SET);
		Book book;
		bool isFind = false;
		long location = 0;
		fread(&book, BOOK_SIZE, 1, bookFile);
		while (book.next != -1 && !isFind) {
			if (book.exist == 1 && book.callnumber == callnumber) {
				isFind = true;
				location = ftell(bookFile) - BOOK_SIZE;
				break;
			}
			fread(&book, BOOK_SIZE, 1, bookFile);
		}

		if (book.next != -1 && !isFind && book.exist == 1) {
			if (book.callnumber == callnumber) {
				isFind = true;
				location = ftell(bookFile) - BOOK_SIZE;
			}
		}

		if (isFind) {
			cout << "ȷ��ɾ����1���˳���0" << endl;
			cout << "��ѡ��";
			int a;
			cin >> a;
			if (cin.fail() || (a != 0 && a != 1)) {
				cout << "���������쳣�˳�" << endl;
				cin.clear();
				cin.sync();
				return;
			}
			cin.clear();
			cin.sync();

			if (a == 1) {
				if (book.flag) {
					book.exist = 0;
					fseek(bookFile, location, SEEK_SET);
					fwrite(&book, BOOK_SIZE, 1, bookFile);
					cout << "ɾ���ɹ�" << endl;
				}
				else {
					cout << "���鱻�����������ɾ��" << endl;
				}
			}
		}

		fclose(bookFile);
	}
}

void deleteAllBooks(BookInfo bookInfo) {
}

bool can(long position) {

	FILE *bookFile = fopen("book", "rb");
	fseek(bookFile, position, SEEK_SET);
	Book book;
	fread(&book, BOOK_SIZE, 1, bookFile);
	while (book.next != -1) {
		if (book.exist == 1 && book.flag == 0) {
			return false;
			break;
		}
		fseek(bookFile, book.next, SEEK_SET);
		fread(&book, BOOK_SIZE, 1, bookFile);
	}

	return true;
}

//ɾ��һ��ͼ��
void deleteBookInfo() {
	cout << "������������";
	char buffer[500];
	char name[30];
	cin >> buffer;
	if (strlen(buffer) > 30) {
		cout << "����̫�����쳣�˳�" << endl;
		return;
	}
	strcpy(name, buffer);
	vector<BookInfo> bookInfos;
	long position = searchByBookName(name, true, bookInfos);
	if (position != -1) {
		BookInfo bookInfo = bookInfos[0];
		listBookInfo(bookInfo);
		if (!can(bookInfo.index)) {
			cout << "���鱻�����������ɾ��" << endl;
			return;
		}
		cout << "ȷ��ɾ����1���˳���0" << endl;
		cout << "��ѡ��";
		int a;
		cin >> a;
		if (cin.fail() || (a != 0 && a != 1)) {
			cout << "���������쳣�˳�" << endl;
			cin.clear();
			cin.sync();
			return;
		}
		cin.clear();
		cin.sync();

		if (a == 1) {
			deleteAllBooks(bookInfo);
			bookInfo.exist = 0;
			FILE *bookInfoFile = fopen("bookinfo", "rb+");
			fseek(bookInfoFile, position, SEEK_SET);
			fwrite(&bookInfo, BOOKINFO_SIZE, 1, bookInfoFile);
			fclose(bookInfoFile);
			cout << "ɾ���ɹ�" << endl;
		}
	}
}

void deleteBook() {
	cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "                |	         1: ɾ��һ��ͼ��              |" << endl;
	cout << "                |	         2: ͨ�������ɾ��һ����      |" << endl;
	cout << "                |	         3: ͨ������ɾ��һ����        |" << endl;
	cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "��ѡ��ɾ����ʽ��";

	int a;
	cin >> a;
	if (a >= 1 && a <= 3) {
		cin.clear();
		cin.sync();

		switch (a) {
		case 1:
			deleteBookInfo();
			break;
		case 2:
			deleteByCallNumber();
			break;
		case 3:
			deleteByBookName();
			break;
		default:
			cout << "��������" << endl;
			break;
		}
		cin.clear();
		cin.sync();
	}
	else if (cin.fail()) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	else {
		cout << "���������쳣�˳�" << endl;
	}
	a = -1;
}

#endif // BOOKDAO_H_INCLUDED
