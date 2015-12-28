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
const int N = 100;//有ISBN得到索书号的乘的基数大小

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
		cout << "图书馆不存在该类书" << endl;
		return -1;
	}
}


void save(BookInfo bookInfo, int num) { //num表示书的本数
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
	cout << "图书添加完毕" << endl;
}

void addNewBooks() {
	char buffer[500];
	BookInfo bookInfo;
	cout << "请输入书名：";
	cin >> buffer;
	if (strlen(buffer) > 30) {
		cout << "名字太长，无法存储" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(bookInfo.name, buffer);
	cout << "请输入作者：";
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "名字太长，无法存储" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(bookInfo.writer, buffer);
	cout << "请输入出版社：";
	cin >> buffer;
	if (strlen(buffer) > 20) {
		cout << "名字太长，无法存储" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	strcpy(bookInfo.press, buffer);
	cout << "请输入价格：";
	cin >> bookInfo.price;
	if (cin.fail()) {
		cout << "输入有误，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	
	cin.clear();
	cin.sync();

	cout << "请输入ISBN(最大8位)：";
	cin >> bookInfo.ISBN;
	if (cin.fail() || bookInfo.ISBN > 20000000) {
		cout << "输入有误，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	cout << "请输入录入本数(最大100本)：";
	int num;
	cin >> num;
	if (cin.fail() || num > 100) {
		cout << "输入有误，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	bookInfo.exist = 1;

	cout << "信息录入完毕，请确认:" << endl;
	cout << "确认录入：1   放弃本次录入：0" << endl;
	cout << "请选择:";
	int isSave;
	cin >> isSave;
	if (cin.fail() || (isSave != 0 && isSave != 1)) {
		cout << "输入有误，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	long position = searchByISBN(bookInfo.ISBN, bookInfo);
	if (position != -1) {
		cout << "该书已存在，添加失败" << endl;
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

//通过position定位图书图书,返回值为可借数量
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

	cout << "图书信息：" << endl;
	cout << "书名：" << bookInfo.name << endl;
	cout << "作者：" << bookInfo.writer << endl;
	cout << "出版社：" << bookInfo.press << endl;
	cout << "ISBN：" << bookInfo.ISBN << endl;
	cout << "价格：" << bookInfo.price << endl;
	cout << "库存：" << bookCount << endl;
	cout << "可借数量：" << borrowQuantity << endl;

	return borrowQuantity;
}

//返回可借数量
int listBookInfo(BookInfo bookInfo) {

	cout << "图书信息：" << endl;
	cout << "书名：" << bookInfo.name << endl;
	cout << "作者：" << bookInfo.writer << endl;
	cout << "出版社：" << bookInfo.press << endl;
	cout << "ISBN：" << bookInfo.ISBN << endl;
	cout << "价格：" << bookInfo.price << endl;

	long position = bookInfo.index;
	FILE *bookFile = fopen("book", "rb");
	fseek(bookFile, position, SEEK_SET);
	Book book;
	int bookCount = 0;
	int borrowQuantity = 0;
	fread(&book, BOOK_SIZE, 1, bookFile);
	while (book.next != -1) {
		if (book.exist == 1) {
			cout << "索书号：" << book.callnumber << " ";
			bookCount++;
			cout << "是否借出： ";
			if (book.flag == 1) {
				borrowQuantity += 1;
				cout << "在馆" << endl;
			}
			else {
				cout << "借出" << endl;
			}
		}
		fseek(bookFile, book.next, SEEK_SET);
		fread(&book, BOOK_SIZE, 1, bookFile);
	}

	if (book.next == -1 && book.exist == 1) {
		cout << "索书号：" << book.callnumber << " ";
		bookCount++;
		cout << "是否借出： ";
		if (book.flag == 1) {
			borrowQuantity += 1;
			cout << "在馆" << endl;
		}
		else {
			cout << "借出" << endl;
		}
	}
	fclose(bookFile);

	cout << "库存：" << bookCount << endl;
	cout << "可借数量：" << borrowQuantity << endl << endl;

	return borrowQuantity;
}


//返回值为第一本书的位置
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
			cout << "对不起，该书不存在" << endl;
			return -1;
		}

		fclose(bookInfoFile);

	}
	//模糊搜索
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
			cout << "对不起，该书不存在" << endl;
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
		cout << "没有找到该作者的书" << endl;
	}
}

void searchBook(char *name, int mode, bool isAccurate, vector<BookInfo> &bookInfoArr) {
	//mode = 0 按书名查找；mode = 1 按作者查找
	// isAccurate = true 精准查找；isAccurate = false 模糊查询
	if (mode == 0) {
		searchByBookName(name, isAccurate, bookInfoArr);
	}
	else if (mode == 1){
		searchByWriter(name, bookInfoArr);
	}
}

void search(int mode, bool accurate, vector<BookInfo> &bookInfoArr) {
	cout << "请输入：";
	char name[500];
	cin >> name;
	if (strlen(name) > 30) {
		cout << "名字太长，异常退出" << endl;
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
    cout << "借阅成功" <<endl;
	return callnumber;
}

void updateBookInfo() {
	cout << "请输入ISBN：";
	long ISBN;
	cin >> ISBN;
	if (cin.fail() || ISBN > 20000000) {
		cout << "输入有误，异常退出" << endl;
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
		cout << "请选择您要更改的条目：";
		int a;
		cin >> a;
		if (a) {
			cin.clear();
			cin.sync();
			char buffer[500];
			switch (a) {
			case 1:
				cout << "请输入书名：";
				cin >> buffer;
				if (strlen(buffer) > 30) {
					cout << "输入太长，异常退出" << endl;
					cin.clear();
					cin.sync();
					return;
				}
				strcpy(bookInfo.name, buffer);
				break;
			case 2:
				cout << "请输入作者：";
				cin >> buffer;
				if (strlen(buffer) > 20) {
					cout << "输入太长，异常退出" << endl;
					cin.clear();
					cin.sync();
					return;
				}
				strcpy(bookInfo.writer, buffer);
				break;
			case 3:
				cout << "请输入出版社：";
				cin >> buffer;
				if (strlen(buffer) > 20) {
					cout << "输入太长，异常退出" << endl;
					cin.clear();
					cin.sync();
					return;
				}
				strcpy(bookInfo.press, buffer);
				break;
			case 4:
				cout << "请输入价格：";
				cin >> bookInfo.price;
				if (cin.fail()) {
					cin.clear();
					cin.sync();
					cout << "输入有误，异常退出" << endl;
				}
				break;
			default:
				cout << "输入有误！" << endl;
				break;
			}
			cin.clear();
			cin.sync();
		} else if (cin.fail()) {
			cout << "输入有误，异常退出" << endl;
			cin.clear();
			cin.sync();
			return;
		}
		
		//读写打开一个二进制文件，允许读和写
		FILE *bookInfoFile = fopen("bookinfo", "rb+");
		fseek(bookInfoFile, position, SEEK_SET);
		fwrite(&bookInfo, BOOKINFO_SIZE, 1, bookInfoFile);
		fclose(bookInfoFile);

		cout << "更新成功" << endl;
	}
}


//添加已经存在的书
void addOldBooks() {
	cout << "请输入书的ISBN：";
	long ISBN;
	cin >> ISBN;
	if (cin.fail() || ISBN > 20000000) {
		cout << "输入有误，异常退出" << endl;
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
		//寻找原先的最后一本书
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
			cout << "请输入添加的数量：";
			int cnt;
			cin >> cnt;
			if (cin.fail() || (cnt+curCount) > 100) {
				cout << "输入有误，异常退出" << endl;
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
			cout << "添加成功" << endl;
		}
		else {
			cout << "查找出错" << endl;
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

//通过索书号删除图书
void deleteByCallNumber() {
	cout << "请输入您所要删除图书的索书号:";
	long callnumber;
	cin >> callnumber;
	if (cin.fail() || callnumber > 2000000000) {
		cout << "输入有误，异常退出" << endl;
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

		cout << "图书信息：" << endl;
		cout << "书名：" << bookInfo.name << endl;
		cout << "作者：" << bookInfo.writer << endl;
		cout << "出版社：" << bookInfo.press << endl;
		cout << "ISBN：" << bookInfo.ISBN << endl;
		cout << "价格：" << bookInfo.price << endl;
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
			cout << "索书号：" << book.callnumber << endl;
			cout << "确认删除：1，退出：0" << endl;
			cout << "请选择：";
			int a;
			cin >> a;
			if (cin.fail()) {
				cout << "输入有误，异常退出" << endl;
				cin.clear();
				cin.sync();
				return;
			} else if (a == 1) {
				if (book.flag) {
					book.exist = 0;
					fseek(bookFile, location, SEEK_SET);
					fwrite(&book, BOOK_SIZE, 1, bookFile);
					cout << "删除成功" << endl;
				}
				else {
					cout << "该书被借出，不允许删除" << endl;
				}
				cin.clear();
				cin.sync();
			}
		}

		fclose(bookFile);
	}
}

//通过书名删除图书
void deleteByBookName() {
	cout << "请输入书名：";
	char buffer[500];
	char name[30];
	cin >> buffer;
	if (strlen(buffer) > 30) {
		cout << "名字太长，异常退出" << endl;
		return;
	}
	strcpy(name, buffer);
	vector<BookInfo> bookInfos;
	long position = searchByBookName(name, true, bookInfos);
	if (position != -1) {
		BookInfo bookInfo = bookInfos[0];
listBookInfo(bookInfo);
		cout << "请输入您所要删除图书的索书号:";
		long callnumber;
		cin >> callnumber;
		if (cin.fail() || callnumber > 2000000000) {
			cout << "输入有误，异常退出" << endl;
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
			cout << "确认删除：1，退出：0" << endl;
			cout << "请选择：";
			int a;
			cin >> a;
			if (cin.fail() || (a != 0 && a != 1)) {
				cout << "输入有误，异常退出" << endl;
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
					cout << "删除成功" << endl;
				}
				else {
					cout << "该书被借出，不允许删除" << endl;
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

//删除一类图书
void deleteBookInfo() {
	cout << "请输入书名：";
	char buffer[500];
	char name[30];
	cin >> buffer;
	if (strlen(buffer) > 30) {
		cout << "名字太长，异常退出" << endl;
		return;
	}
	strcpy(name, buffer);
	vector<BookInfo> bookInfos;
	long position = searchByBookName(name, true, bookInfos);
	if (position != -1) {
		BookInfo bookInfo = bookInfos[0];
		listBookInfo(bookInfo);
		if (!can(bookInfo.index)) {
			cout << "该书被借出，不允许删除" << endl;
			return;
		}
		cout << "确认删除：1，退出：0" << endl;
		cout << "请选择：";
		int a;
		cin >> a;
		if (cin.fail() || (a != 0 && a != 1)) {
			cout << "输入有误，异常退出" << endl;
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
			cout << "删除成功" << endl;
		}
	}
}

void deleteBook() {
	cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "                |	         1: 删除一类图书              |" << endl;
	cout << "                |	         2: 通过索书号删除一本书      |" << endl;
	cout << "                |	         3: 通过书名删除一本书        |" << endl;
	cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "请选择删除方式：";

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
			cout << "输入有误！" << endl;
			break;
		}
		cin.clear();
		cin.sync();
	}
	else if (cin.fail()) {
		cout << "输入有误，异常退出" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	else {
		cout << "输入有误，异常退出" << endl;
	}
	a = -1;
}

#endif // BOOKDAO_H_INCLUDED
