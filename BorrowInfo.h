#ifndef BORROWINFO_H_INCLUDED
#define BORROWINFO_H_INCLUDED

typedef struct {
	int id;
	long callnumber;
	//1表示归还，0表示没有归还
	int isReturn;
	//借书时间
	int year1;
	int month1;
	int day1;
	//续借次数
	int time;
	//应还时间
	int year2;
	int month2;
	int day2;
	long index; //下一条借阅记录的位置
}BorrowInfo;

const int BORROWINFO_SIZE = sizeof(BorrowInfo);

#endif // BORROWINFO_H_INCLUDED
