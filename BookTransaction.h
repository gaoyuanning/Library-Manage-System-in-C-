#ifndef BOOKTRANSACTION_H_INCLUDED
#define BOOKTRANSACTION_H_INCLUDED

//借阅者可以更改自己的密码，但是必须通过密码验证。

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

//搜寻借阅记录时一次性读取的数目
const int SIZE = 1;

void coverBorrowInfo(BorrowInfo borrowInfo, long position) {
	FILE *borrowInfoFile = fopen("borrowinfo", "rb+");
	fseek(borrowInfoFile, position, SEEK_SET);
	fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
	fclose(borrowInfoFile);
}

//返回在文件中的位置
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
	
	//表示没有借阅记录
	return -1;
}

//定位某个人最后一条借阅记录的位置
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

//借书操作，更新图书信息
void updateBookInfo_Borrow(int id, int callnumber, Date date) {
	//根据不同的人员身份计算不同的应还日期
	Date *pdate2 = new Date();
	int k = calIdLen(id);
	switch (k) {
	case 6:
		pdate2 = new Date(((date.add()).add()).add()); //老师可以借阅3个月
		break;
	case 7:
		pdate2 = new Date((date.add()).add()); //研究生可以借阅2个月
		break;
	case 8:
		pdate2 = new Date(date.add()); //本科生可以借阅1个月
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
	//找到该读者第一条借阅记录的位置
	long position = findBorrowInfo(borrowInfo);
	if (position != -1) {
		BorrowInfo tempBorrowInfo;
		//找到该读者最后一条借阅记录的位置,temp用于存放最后一条记录的信息
		position = locateLastBorrowInfo(position, tempBorrowInfo);
		//最后一条记录的index指向新存入的记录
		tempBorrowInfo.index = location;
		coverBorrowInfo(tempBorrowInfo, position);
	}
	fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
	fclose(borrowInfoFile);

}

//更新借阅信息 mode = 0 代表借书，mode = 1 代表还书  mode = 2 代表续借
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
		cout << "更新图书模式出错" << endl;
		break;
	}
}

//检查有无超期图书,超期返回值为真
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
		cout << "对不起，登录失败！" << endl;
		return false;
	}
}

//检查有无超越本数
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
			cout << "信息有误！" << endl;
			return true;
			break;
		}
		return false;
	}
	else {
		cout << "对不起，登录失败！" << endl;
		return false;
	}
}

//真正的借书功能实现
//借书之前检查有无超期图书，是否达到最大借阅本数，两者通过后才能借书，，，，，，待实现功能
//检查有无超期图书就是用文件中存储的两个日期做差，老师，研究生，本科生不同
//最大借阅本数老师，研究生，本科生不同

void borrowBook(BookInfo bookInfo) {
	int sum = 0;
	if (checkBookOutDate(sum)) 
	{
		cout << "您有超期图书，请先归还图书" << endl;
		return;
	}
	if (checkOutMaxBorrowSum(sum)) {
		cout << "对不起，您已达到最大借阅本数" << endl;
		return;
	}
	/*
	if (0) {
		cout << "您有超期图书，请先归还图书" << endl;
		return;
	}
	else if (0) {
		cout << "对不起，您已达到最大借阅本数" << endl;
		return;
	}*/
	else {
		cout << endl;
		int borrowQuantity = locationBook(bookInfo);
		cout << endl;
		if (borrowQuantity > 1) {
			cout << "确认借阅:1, 放弃本次借阅：0" << endl;
			int a;
			cout << "请选择：";
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
				long callnumber = updateBookAfterBorrow(bookInfo);
				updateBorrowInfo(id, callnumber, date, 0);
			}
		}
		else {
			cout << "对不起，该书只剩一本为保存本，不允许外借" << endl;
		}
	}
}

void listBookInfoArr(vector<BookInfo> bookInfoArr) {
	if (bookInfoArr.size() > 0) {
		for (int i = 0; i < bookInfoArr.size(); i++) {
			BookInfo bookInfo = bookInfoArr[i];
			cout << i + 1 << "： " << bookInfo.name << endl;
		}

		cout << endl <<"请选择您要借阅的图书序号:";
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
			cout << "非法输入，请重新输入" << endl;
			cin.clear();
			cin.sync();
		}
		else {
			cout << "输入有误" << endl;
		}

	}
}

//总的借书模块
void borrow() {
	int mode;
	bool accurate;
	vector<BookInfo> bookInfoArr;

	bool quit = false;
	while (!quit) {
		cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "                |	         1、按书名精确查找	      |" << endl;
		cout << "                |	         2、按书名模糊查找	      |" << endl;
		cout << "                |	         3、按作者查找		      |" << endl;
		cout << "                |	         0、退出	              |" << endl;
		cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "请选择：";
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
			cout << "输入有误，异常退出" << endl;
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
	cout << "借书日期：";
	dateBorrow.printFullyear();
	cout << endl;
	cout << "应还日期：";
	dateReturn.printFullyear();
	cout << endl;
	int days = dateReturn - date;
	cout << "借阅状态：";
	if (borrowInfo.isReturn == 1) {
		cout << "已还" << endl;
	}
	else {
		cout << "未还" << endl;
		if (days < 0) {
			cout << "该图书已超期！" << endl;
		}
	}
}

//测试使用
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

//posArr记录每本记录的位置
bool queryBorrowHistory(vector<long> &posArr) {
	bool haveBorrowBook = false;
	if (checkLog()) {

		Reader reader;
		initReader(reader);
		cout << endl;
		cout << "学工号：" << reader.id << endl;
		cout << "姓名：" << reader.name << endl;
		cout << "专业：" << reader.major << endl;
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
				cout << "借阅信息" << i++ << ":" << endl;
				printBorrowInfo(borrowInfo, date);
				cout << endl;
				fseek(borrowInfoFile, borrowInfo.index, SEEK_SET);
				fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
			}
			cout << "借阅信息" << i++ << ":" << endl;
			printBorrowInfo(borrowInfo, date);
		}
		else {
			haveBorrowBook = false;
			cout << "您没有借阅记录" << endl;
		}
		fclose(borrowInfoFile);
	}
	return haveBorrowBook;
}

void borrowHistory() {
	vector<long> posArr;
	queryBorrowHistory(posArr);
}

//续借图书，续借之前检查有无超期未还的图书
void renewBook() {
	int sum = 0;
	if (checkBookOutDate(sum)) {
		cout << "您有超期图书，请先归还图书" << endl;
		return;
	}
	else if (checkOutMaxBorrowSum(sum)) {
		cout << "对不起，您已达到最大借阅本数" << endl;
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
		cout << "请选择您要续借的图书:";
		int a;
		cin >> a;
		if (cin.fail()) {
			cout << "输入有误，异常退出" << endl;
			cin.clear();
			cin.sync();
			return;
		}
		cin.clear();
		cin.sync();

		int length = posArr.size();
		if (a >= 1 && a <= length) {
			long position = posArr[a - 1];
			//检查续借次数，不同类型的续借次数不一样，老师2次，学生1次
			FILE *borrowInfoFile = fopen("borrowinfo", "rb+");
			BorrowInfo borrowInfo;
			fseek(borrowInfoFile, position, SEEK_SET);
			fread(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
			if (!borrowInfo.isReturn) {
				int time = borrowInfo.time;
				switch (k) {
				case 6:
					if (time >= 2) { //老师已经续借了两次了
						cout << "对不起，您已经续借本书两次，不允许续借了" << endl;
					}
					else {
						Date tempDate = date.add();
						borrowInfo.year2 = tempDate.getYear();
						borrowInfo.month2 = tempDate.getMonth();
						borrowInfo.day2 = tempDate.getDay();
						borrowInfo.time = time + 1;
						fseek(borrowInfoFile, position, SEEK_SET);
						fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
						cout << "续借成功" << endl;
					}
					break;
				case 7:
					if (time >= 1) { //研究生已经续借了一次了
						cout << "对不起，您已经续借本书一次，不允许续借了" << endl;
					}
					else {
						Date tempDate = date.add();
						borrowInfo.year2 = tempDate.getYear();
						borrowInfo.month2 = tempDate.getMonth();
						borrowInfo.day2 = tempDate.getDay();
						borrowInfo.time = time + 1;
						fseek(borrowInfoFile, position, SEEK_SET);
						fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
						cout << "续借成功" << endl;
					}
					break;
				case 8:
					if (time >= 1) { //本科生已经续借了一次了
						cout << "对不起，您已经续借本书一次，不允许续借了" << endl;
					}
					else {
						Date tempDate = date.add();
						borrowInfo.year2 = tempDate.getYear();
						borrowInfo.month2 = tempDate.getMonth();
						borrowInfo.day2 = tempDate.getDay();
						borrowInfo.time = time + 1;
						fseek(borrowInfoFile, position, SEEK_SET);
						fwrite(&borrowInfo, BORROWINFO_SIZE, 1, borrowInfoFile);
						cout << "续借成功" << endl;
					}
					break;
				default:
					cout << "信息有误！" << endl;
					break;
				}
			}
			else {
				cout << "该书已经归还！" << endl;
			}
			fclose(borrowInfoFile);
		}
		else {
			cout << "输入有误！" << endl;
		}
	}
}

//归还图书
void returnBook() {
	vector<long> posArr;
	if (!queryBorrowHistory(posArr)) {
		return;
	}
	cout << "请选择您要归还的图书:";
	int a;
	cin >> a;
	if (cin.fail()) {
		cout << "输入有误，异常退出" << endl;
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
			cout << "归还成功" << endl;
		}
		else {
			cout << "该书已经归还！" << endl;
		}
		fclose(borrowInfoFile);
	}
	else {
		cout << "输入有误！" << endl;
	}
}

void addBook() {
	bool quit = false;
	while (!quit) {
		cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "                |	         1、添加新书		      |" << endl;
		cout << "                |	         2、添加旧书		      |" << endl;
		cout << "                |	         0、退出	              |" << endl;
		cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "请选择：";
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
				cout << "输入有误" << endl;
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
	}
}

#endif
