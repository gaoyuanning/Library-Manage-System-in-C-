#ifndef BOOK_H_INCLUDED
#define BOOK_H_INCLUDED

#include <iostream>
using namespace std;

typedef struct {
	char name[30];
	char press[20];
	char writer[20];
	float price;
	long ISBN; //最大为8位
	int exist; //1表示存在，0表示不存在
	long index; // 第一本书的位置
}BookInfo;

typedef struct {
	long callnumber;
	int flag;   //标志是否可借，1表示可借，0表示已借出
	int exist; //1表示存在，0表示不存在
	long next; //下一本书的位置
}Book;

void printBookInfo(BookInfo bookInfo) {
	cout << "1:书名 " << bookInfo.name << endl;
	cout << "2: 作者 " << bookInfo.writer << endl;
	cout << "3: 出版社 " << bookInfo.press << endl;
	cout << "4: 价格 " << bookInfo.price << endl;
}

#endif // BOOK_H_INCLUDED
