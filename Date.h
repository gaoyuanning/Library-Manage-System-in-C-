#ifndef DATE_H
#define DATE_H
#include<iostream>
#include<iomanip>
using namespace std;

class Date{
public:
	//Date();
	Date(int year = 1990, int month = 1, int day = 1) :year(year), month(month), day(day){ separator = '-'; this->day = checkDay(this->day); }
	Date(const Date& date);
	void setDate(Date date);
	void setYear(int year1){ year = year1; }
	int getYear() const { return year; }
	void setMonth(int month1){ month = month1; }
	int getMonth() const { return month; }
	void setDay(int day1){ day = day1; }
	int getDay() const { return day; }
	void setSeparator(char separator1)  { separator = separator1; }
	char getSeparator()const{ return separator; }
	void printFullyear() const { cout << year << separator << setfill('0') << setw(2) << month << separator << setfill('0') << setw(2) << day; }		 // ��YYYY-MM-DD����ʽ��ӡ��2011-01-08
	void printStandaryear() const;	 //��YY-MM-DD����ʽ��ӡ������11-01-08
	int fullYearTo(const Date& date) const;
	int getDayOfYear(int, int, int) const; //���㵱ǰ�����ǽ���Ķ�����
	int getLeftDaysYear() const;// getLeftDaysYear�����㵱ǰ���ھ౾��������м���
	int operator>(Date& date) const;
	int operator-(const Date& date) const;
	Date add();
	void init();

private:
	int year, month, day;
	char separator;
	static  int daysPerMonth[12]; // ������̬��������ÿ�µ�����
	// ��.cpp�ļ��ж��岢��ʼ��
	int checkDay(int day) const;	   // ��������£��жϲ��������Ƿ�Ϸ���
	// ����Ϸ�������day�����򷵻� 1��
	int isLeapyear(int year) const; // �жϲ������Ƿ������ꡣ

};
#endif
