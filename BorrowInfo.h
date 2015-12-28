#ifndef BORROWINFO_H_INCLUDED
#define BORROWINFO_H_INCLUDED

typedef struct {
	int id;
	long callnumber;
	//1��ʾ�黹��0��ʾû�й黹
	int isReturn;
	//����ʱ��
	int year1;
	int month1;
	int day1;
	//�������
	int time;
	//Ӧ��ʱ��
	int year2;
	int month2;
	int day2;
	long index; //��һ�����ļ�¼��λ��
}BorrowInfo;

const int BORROWINFO_SIZE = sizeof(BorrowInfo);

#endif // BORROWINFO_H_INCLUDED
