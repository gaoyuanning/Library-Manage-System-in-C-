#ifndef BOOKTRANSACTION_H_INCLUDED
#define BOOKTRANSACTION_H_INCLUDED

//�����߿��Ը����Լ������룬���Ǳ���ͨ��������֤��

#include "BookDAO.h"
#include "ReaderDAO.h"
#include "Reader.h"
#include "Log.h"
#include "Date.h"
#include "BorrowInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;

//��Ѱ���ļ�¼ʱһ���Զ�ȡ����Ŀ
const int SIZE = 1;

void coverBorrowInfo(BorrowInfo borrowInfo, long position) {
	FILE *borrowInfoFile = fopen("borrowinfo", "rb+");
	fseek(borrowInfoFile, position, SEEK_SET);
	fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
	fclose(borrowInfoFile);
}

//�������ļ��е�λ��
long findBorrowInfo(BorrowInfo borrowInfo) {

	FILE *borrowInfoFile = fopen("borrowinfo", "rb");
	fseek(borrowInfoFile, 0, SEEK_SET);
	BorrowInfo tempBorrowInfos[SIZE];
	fread(tempBorrowInfos, BORROWINFO_SIZE, SIZE, borrowInfoFile);
	int i = 0;
	while (!feof(borrowInfoFile)) {
		for (i = 0; i < SIZE; i++) {
			if (borrowInfo.id == tempBorrowInfos[i].id) {
				return (ftell(borrowInfoFile) - BORROWINFO_SIZE * (SIZE - i));
			}
		}
		fread(tempBorrowInfos, BORROWINFO_SIZE, SIZE, borrowInfoFile);
	}

	for (int i = 0; i < SIZE; i++) {
		if (borrowInfo.id == tempBorrowInfos[i].id) {
			return (ftell(borrowInfoFile) - BORROWINFO_SIZE * (SIZE - i));
		}
	}

	fclose(borrowInfoFile);
	
	//��ʾû�н��ļ�¼
	return -1;
}

//��λĳ�������һ�����ļ�¼��λ��
long locateLastBorrowInfo(long position, BorrowInfo &borrowInfo) {

		FILE *borrowInfoFile = fopen("borrowinfo", "rb");
		fseek(borrowInfoFile, position, SEEK_SET);
		fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
		while (borrowInfo.index != -1) {
			fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
		}

		position = ftell(borrowInfoFile) - BORROWINFO_SIZE;
		fclose(borrowInfoFile);

		return position;
}

//�������������ͼ����Ϣ
void updateBookInfo_Borrow(int id, int callnumber, Date date) {
	//���ݲ�ͬ����Ա��ݼ��㲻ͬ��Ӧ������
	Date *pdate2 = new Date();
	int k = calIdLen(id);
	switch (k) {
	case 6:
		pdate2 = new Date(((date.add()).add()).add()); //��ʦ���Խ���3����
		break;
	case 7:
		pdate2 = new Date((date.add()).add()); //�о������Խ���2����
		break;
	case 8:
		pdate2 = new Date(date.add()); //���������Խ���1����
		break;
	default:
		break;
	}
	
	BorrowInfo borrowInfo;
	borrowInfo.id = id;
	borrowInfo.callnumber = callnumber;
	borrowInfo.isReturn = 0;
	borrowInfo.year1 = date.getYear();
	borrowInfo.month1 = date.getMonth();
	borrowInfo.day1 = date.getDay();
	borrowInfo.year2 = pdate2->getYear();
	borrowInfo.month2 = pdate2->getMonth();
	borrowInfo.day2 = pdate2->getDay();
	borrowInfo.index = -1;
	borrowInfo.time = 0;

	FILE *borrowInfoFile = fopen("borrowinfo", "ab+");
	fseek(borrowInfoFile, 0, SEEK_END);
	long location = ftell(borrowInfoFile);
	//�ҵ��ö��ߵ�һ�����ļ�¼��λ��
	long position = findBorrowInfo(borrowInfo);
	if (position != -1) {
		BorrowInfo tempBorrowInfo;
		//�ҵ��ö������һ�����ļ�¼��λ��,temp���ڴ�����һ����¼����Ϣ
		position = locateLastBorrowInfo(position, tempBorrowInfo);
		//���һ����¼��indexָ���´���ļ�¼
		tempBorrowInfo.index = location;
		coverBorrowInfo(tempBorrowInfo, position);
	}
	fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
	fclose(borrowInfoFile);

}

//���½�����Ϣ mode = 0 ������飬mode = 1 ������  mode = 2 ��������
void updateBorrowInfo(int id, long callnumber, Date date, int mode) {
	switch (mode) {
	case 0:
		updateBookInfo_Borrow(id, callnumber, date);
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		cout << "����ͼ��ģʽ����" << endl;
		break;
	}
}

//������޳���ͼ��,���ڷ���ֵΪ��
bool checkBookOutDate(int &sum) {
	sum = 0;
	bool outDate = false;
	if (checkLog()) {
		FILE *borrowInfoFile = fopen("borrowinfo", "rb");
		BorrowInfo borrowInfo;
		borrowInfo.id = id;
		long position = findBorrowInfo(borrowInfo);
		if (position != -1) {
			fseek(borrowInfoFile, position, SEEK_SET);
			fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
			while (borrowInfo.index != -1) {
				Date dateReturn(borrowInfo.year2, borrowInfo.month2, borrowInfo.day2);
				int days = dateReturn - date;
				if (borrowInfo.isReturn == 0 && days < 0) {
					outDate = true;
				}
				if (borrowInfo.isReturn == 0) {
					sum += 1;
				}
				fseek(borrowInfoFile, borrowInfo.index, SEEK_SET);
				fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
			}
			Date dateReturn(borrowInfo.year2, borrowInfo.month2, borrowInfo.day2);
			int days = dateReturn - date;
			if (borrowInfo.isReturn == 0 && days < 0) {
				outDate = true;
			}
			if (borrowInfo.isReturn == 0) {
				sum += 1;
			}
		}
		fclose(borrowInfoFile);

		return outDate;
	}
	else {
		cout << "�Բ��𣬵�¼ʧ�ܣ�" << endl;
		return false;
	}
}

//������޳�Խ����
bool checkOutMaxBorrowSum(int sum) {
	if (checkLog()) {
		int k = calIdLen(id);
		switch (k) {
		case 6:
			if (sum >= TEACHER_BORROW_SIZE) {
				return true;
			}
			break;
		case 7:
			if (sum >= GRADUATE_BORROW_SIZE) {
				return true;
			}
			break;
		case 8:
			if (sum >= UNDERGRA_BORROW_SIZE) {
				return true;
			}
			break;
		default:
			cout << "��Ϣ����" << endl;
			return true;
			break;
		}
		return false;
	}
	else {
		cout << "�Բ��𣬵�¼ʧ�ܣ�" << endl;
		return false;
	}
}

//�����Ľ��鹦��ʵ��
//����֮ǰ������޳���ͼ�飬�Ƿ�ﵽ�����ı���������ͨ������ܽ��飬������������ʵ�ֹ���
//������޳���ͼ��������ļ��д洢���������������ʦ���о�������������ͬ
//�����ı�����ʦ���о�������������ͬ

void borrowBook(BookInfo bookInfo) {
	int sum = 0;
	if (checkBookOutDate(sum)) 
	{
		cout << "���г���ͼ�飬���ȹ黹ͼ��" << endl;
		return;
	}
	if (checkOutMaxBorrowSum(sum)) {
		cout << "�Բ������Ѵﵽ�����ı���" << endl;
		return;
	}
	/*
	if (0) {
		cout << "���г���ͼ�飬���ȹ黹ͼ��" << endl;
		return;
	}
	else if (0) {
		cout << "�Բ������Ѵﵽ�����ı���" << endl;
		return;
	}*/
	else {
		cout << endl;
		int borrowQuantity = locationBook(bookInfo);
		cout << endl;
		if (borrowQuantity > 1) {
			cout << "ȷ�Ͻ���:1, �������ν��ģ�0" << endl;
			int a;
			cout << "��ѡ��";
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
				long callnumber = updateBookAfterBorrow(bookInfo);
				updateBorrowInfo(id, callnumber, date, 0);
			}
		}
		else {
			cout << "�Բ��𣬸���ֻʣһ��Ϊ���汾�����������" << endl;
		}
	}
}

void listBookInfoArr(vector<BookInfo> bookInfoArr) {
	if (bookInfoArr.size() > 0) {
		for (int i = 0; i < bookInfoArr.size(); i++) {
			BookInfo bookInfo = bookInfoArr[i];
			cout << i + 1 << "�� " << bookInfo.name << endl;
		}

		cout << endl <<"��ѡ����Ҫ���ĵ�ͼ�����:";
		int k;
		cin >> k;
		if (k >= 1 && k <= bookInfoArr.size()) {
			BookInfo bookInfo = bookInfoArr[k - 1];
			//listBookInfo(bookInfo);
			if (checkLog()) {
				borrowBook(bookInfo);
			}
			cin.clear();
			cin.sync();
		}
		else if (cin.fail()) {
			cout << "�Ƿ����룬����������" << endl;
			cin.clear();
			cin.sync();
		}
		else {
			cout << "��������" << endl;
		}

	}
}

//�ܵĽ���ģ��
void borrow() {
	int mode;
	bool accurate;
	vector<BookInfo> bookInfoArr;

	bool quit = false;
	while (!quit) {
		cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "                |	         1����������ȷ����	      |" << endl;
		cout << "                |	         2��������ģ������	      |" << endl;
		cout << "                |	         3�������߲���		      |" << endl;
		cout << "                |	         0���˳�	              |" << endl;
		cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "��ѡ��";
		int k;
		cin >> k;
		if (k >= 0 && k <= 3) {
			cin.clear();
			cin.sync();

			switch (k) {
			case 1:
				mode = 0;
				accurate = true;
				search(mode, accurate, bookInfoArr);
				break;
			case 2:
				mode = 0;
				accurate = false;
				search(mode, accurate, bookInfoArr);
				break;
			case 3:
				mode = 1;
				accurate = true;
				search(mode, accurate, bookInfoArr);
				break;
			case 0:
				quit = true;
				break;
			default:
				break;
			}
			cin.clear();
			cin.sync();
		} else if (cin.fail()) {
			cout << "���������쳣�˳�" << endl;
			cin.clear();
			cin.sync();
		}
		k = -1;
		listBookInfoArr(bookInfoArr);
		bookInfoArr.clear();
	}
}

void printBorrowInfo(BorrowInfo borrowInfo, Date date) {
	int ISBN = borrowInfo.callnumber / N;
	BookInfo bookInfo;
	searchByISBN(ISBN, bookInfo);
	printBookInfo(bookInfo);
	Date dateBorrow(borrowInfo.year1, borrowInfo.month1, borrowInfo.day1);
	Date dateReturn(borrowInfo.year2, borrowInfo.month2, borrowInfo.day2);
	cout << "�������ڣ�";
	dateBorrow.printFullyear();
	cout << endl;
	cout << "Ӧ�����ڣ�";
	dateReturn.printFullyear();
	cout << endl;
	int days = dateReturn - date;
	cout << "����״̬��";
	if (borrowInfo.isReturn == 1) {
		cout << "�ѻ�" << endl;
	}
	else {
		cout << "δ��" << endl;
		if (days < 0) {
			cout << "��ͼ���ѳ��ڣ�" << endl;
		}
	}
}

//����ʹ��
void listAllBorrowInfo() {
	FILE *borrowInfoFile = fopen("borrowinfo", "rb");
	BorrowInfo borrowInfo;
	fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
	while (!feof(borrowInfoFile)) {
		cout << borrowInfo.id << " " << borrowInfo.callnumber << 
			" " << borrowInfo.index << endl;
		fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
	}
	fclose(borrowInfoFile);
}

//posArr��¼ÿ����¼��λ��
bool queryBorrowHistory(vector<long> &posArr) {
	bool haveBorrowBook = false;
	if (checkLog()) {

		Reader reader;
		initReader(reader);
		cout << endl;
		cout << "ѧ���ţ�" << reader.id << endl;
		cout << "������" << reader.name << endl;
		cout << "רҵ��" << reader.major << endl;
		cout << endl;
		FILE *borrowInfoFile = fopen("borrowinfo", "rb");
		BorrowInfo borrowInfo;
		borrowInfo.id = id;
		long position = findBorrowInfo(borrowInfo);

		int i = 1;
		if (position != -1) {
			haveBorrowBook = true;
			posArr.push_back(position);
			fseek(borrowInfoFile, position, SEEK_SET);
			fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
			while (borrowInfo.index != -1) {
				posArr.push_back(borrowInfo.index);
				cout << "������Ϣ" << i++ << ":" << endl;
				printBorrowInfo(borrowInfo, date);
				cout << endl;
				fseek(borrowInfoFile, borrowInfo.index, SEEK_SET);
				fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
			}
			cout << "������Ϣ" << i++ << ":" << endl;
			printBorrowInfo(borrowInfo, date);
		}
		else {
			haveBorrowBook = false;
			cout << "��û�н��ļ�¼" << endl;
		}
		fclose(borrowInfoFile);
	}
	return haveBorrowBook;
}

void borrowHistory() {
	vector<long> posArr;
	queryBorrowHistory(posArr);
}

//����ͼ�飬����֮ǰ������޳���δ����ͼ��
void renewBook() {
	int sum = 0;
	if (checkBookOutDate(sum)) {
		cout << "���г���ͼ�飬���ȹ黹ͼ��" << endl;
		return;
	}
	else if (checkOutMaxBorrowSum(sum)) {
		cout << "�Բ������Ѵﵽ�����ı���" << endl;
		return;
	}
	else {
		int k = 0;
		if (checkLog()) {
			k = calIdLen(id);
		}

		vector<long> posArr;
		if (!queryBorrowHistory(posArr)) {
			return;

		}
		cout << "��ѡ����Ҫ�����ͼ��:";
		int a;
		cin >> a;
		if (cin.fail()) {
			cout << "���������쳣�˳�" << endl;
			cin.clear();
			cin.sync();
			return;
		}
		cin.clear();
		cin.sync();

		int length = posArr.size();
		if (a >= 1 && a <= length) {
			long position = posArr[a - 1];
			//��������������ͬ���͵����������һ������ʦ2�Σ�ѧ��1��
			FILE *borrowInfoFile = fopen("borrowinfo", "rb+");
			BorrowInfo borrowInfo;
			fseek(borrowInfoFile, position, SEEK_SET);
			fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
			if (!borrowInfo.isReturn) {
				int time = borrowInfo.time;
				switch (k) {
				case 6:
					if (time >= 2) { //��ʦ�Ѿ�������������
						cout << "�Բ������Ѿ����豾�����Σ�������������" << endl;
					}
					else {
						Date tempDate = date.add();
						borrowInfo.year2 = tempDate.getYear();
						borrowInfo.month2 = tempDate.getMonth();
						borrowInfo.day2 = tempDate.getDay();
						borrowInfo.time = time + 1;
						fseek(borrowInfoFile, position, SEEK_SET);
						fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
						cout << "����ɹ�" << endl;
					}
					break;
				case 7:
					if (time >= 1) { //�о����Ѿ�������һ����
						cout << "�Բ������Ѿ����豾��һ�Σ�������������" << endl;
					}
					else {
						Date tempDate = date.add();
						borrowInfo.year2 = tempDate.getYear();
						borrowInfo.month2 = tempDate.getMonth();
						borrowInfo.day2 = tempDate.getDay();
						borrowInfo.time = time + 1;
						fseek(borrowInfoFile, position, SEEK_SET);
						fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
						cout << "����ɹ�" << endl;
					}
					break;
				case 8:
					if (time >= 1) { //�������Ѿ�������һ����
						cout << "�Բ������Ѿ����豾��һ�Σ�������������" << endl;
					}
					else {
						Date tempDate = date.add();
						borrowInfo.year2 = tempDate.getYear();
						borrowInfo.month2 = tempDate.getMonth();
						borrowInfo.day2 = tempDate.getDay();
						borrowInfo.time = time + 1;
						fseek(borrowInfoFile, position, SEEK_SET);
						fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
						cout << "����ɹ�" << endl;
					}
					break;
				default:
					cout << "��Ϣ����" << endl;
					break;
				}
			}
			else {
				cout << "�����Ѿ��黹��" << endl;
			}
			fclose(borrowInfoFile);
		}
		else {
			cout << "��������" << endl;
		}
	}
}

//�黹ͼ��
void returnBook() {
	vector<long> posArr;
	if (!queryBorrowHistory(posArr)) {
		return;
	}
	cout << "��ѡ����Ҫ�黹��ͼ��:";
	int a;
	cin >> a;
	if (cin.fail()) {
		cout << "���������쳣�˳�" << endl;
		cin.clear();
		cin.sync();
		return;
	}
	cin.clear();
	cin.sync();

	int length = posArr.size();
	if (a >= 1 && a <= length) {
		long position = posArr[a - 1];
		FILE *borrowInfoFile = fopen("borrowinfo", "rb+");
		BorrowInfo borrowInfo;
		fseek(borrowInfoFile, position, SEEK_SET);
		fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
		if (!borrowInfo.isReturn) {	
			borrowInfo.isReturn = 1;
			fseek(borrowInfoFile, position, SEEK_SET);
			fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
			updateBookAfterReturn(borrowInfo.callnumber);
			cout << "�黹�ɹ�" << endl;
		}
		else {
			cout << "�����Ѿ��黹��" << endl;
		}
		fclose(borrowInfoFile);
	}
	else {
		cout << "��������" << endl;
	}
}

void addBook() {
	bool quit = false;
	while (!quit) {
		cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "                |	         1���������		      |" << endl;
		cout << "                |	         2����Ӿ���		      |" << endl;
		cout << "                |	         0���˳�	              |" << endl;
		cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "��ѡ��";
		int k;
		cin >> k;
		if (k >= 0 && k <= 2) {
			cin.clear();
			cin.sync();

			switch (k) {
			case 1:
				addNewBooks();
				break;
			case 2:
				addOldBooks();
				break;
			case 0:
				quit = true;
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
		}
		k = -1;
	}
}

#endif
