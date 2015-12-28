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
	void printFullyear() const { cout << year << separator << setfill('0') << setw(2) << month << separator << setfill('0') << setw(2) << day; }		 // 以YYYY-MM-DD的形式打印，2011-01-08
	void printStandaryear() const;	 //以YY-MM-DD的形式打印，比如11-01-08
	int fullYearTo(const Date& date) const;
	int getDayOfYear(int, int, int) const; //计算当前日期是今年的多少天
	int getLeftDaysYear() const;// getLeftDaysYear：计算当前日期距本年结束还有几天
	int operator>(Date& date) const;
	int operator-(const Date& date) const;
	Date add();
	void init();

private:
	int year, month, day;
	char separator;
	static  int daysPerMonth[12]; // 声明静态常变量，每月的天数
	// 在.cpp文件中定义并初始化
	int checkDay(int day) const;	   // 根据年和月，判断参数日期是否合法。
	// 如果合法，返回day，否则返回 1。
	int isLeapyear(int year) const; // 判断参数年是否是闰年。

};
#endif
