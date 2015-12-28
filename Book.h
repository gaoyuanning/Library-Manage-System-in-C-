#ifndef BOOK_H_INCLUDED
#define BOOK_H_INCLUDED

#include <iostream>
using namespace std;

typedef struct {
	char name[30];
	char press[20];
	char writer[20];
	float price;
	long ISBN; //���Ϊ8λ
	int exist; //1��ʾ���ڣ�0��ʾ������
	long index; // ��һ�����λ��
}BookInfo;

typedef struct {
	long callnumber;
	int flag;   //��־�Ƿ�ɽ裬1��ʾ�ɽ裬0��ʾ�ѽ��
	int exist; //1��ʾ���ڣ�0��ʾ������
	long next; //��һ�����λ��
}Book;

void printBookInfo(BookInfo bookInfo) {
	cout << "1:���� " << bookInfo.name << endl;
	cout << "2: ���� " << bookInfo.writer << endl;
	cout << "3: ������ " << bookInfo.press << endl;
	cout << "4: �۸� " << bookInfo.price << endl;
}

#endif // BOOK_H_INCLUDED
