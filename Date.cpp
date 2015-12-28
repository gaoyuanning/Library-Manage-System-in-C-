#include<iostream>
#include"Date.h"
using namespace std;

int Date::daysPerMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

Date::Date(const Date& date){
	year = date.getYear(); month = date.getMonth(); day = date.getDay(); separator = '-';
}

void Date::setDate(Date date)
{
	this->year = date.getYear();
	this->month = date.getMonth();
	this->day = date.getDay();
}

void Date::printStandaryear() const{
	int m;
	m = year - year / 100 * 100;
	cout << setfill('0') << setw(2) << m << separator << setfill('0') << setw(2) << month << separator << setfill('0') << setw(2) << day;
}


int Date::fullYearTo(const Date& date) const
{
	int n;

	n = date.getYear() - year - 1;
	return n;
}

int Date::getDayOfYear(int year, int month, int day) const
{
	int a[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int sum = 0, i;
	if (isLeapyear(year))
		a[1] = 29;
	for (i = 0; i < month - 1; i++)
		sum = sum + a[i];
	sum = sum + day;
	return sum;
}

int Date::operator-(const Date& date) const
{
	int i, sum = 0;
	int year2 = date.getYear(); int month2 = date.getMonth(); int day2 = date.getDay();
	int year1, month1, day1;
	year1 = year; month1 = month; day1 = day;

	if (year1 == year2) sum = getDayOfYear(year1, month1, day1) - getDayOfYear(year2, month2, day2);
	else if (year1 < year2)
	{
		for (i = year1 + 1; i < year2; i++)
		{
			if (isLeapyear(i)) sum = sum + 366;
			else sum = sum + 365;
		}
		sum = sum + getDayOfYear(year2, month2, day2) + 365 - getDayOfYear(year1, month1, day1);
		if (isLeapyear(year1))
			sum = sum + 1;
		sum = -sum;
	}
	else
	{
		for (i = year2 + 1; i < year1; i++)
		{
			if (isLeapyear(i)) sum = sum + 366;
			else sum = sum + 365;
		}
		sum = sum + getDayOfYear(year1, month1, day1) + 365 - getDayOfYear(year2, month2, day2);
		if (isLeapyear(year2))
			sum = sum + 1;
	}
	return sum;
}

int Date::checkDay(int day) const{
	if (isLeapyear(this->year)) daysPerMonth[1] = 29;
	if (daysPerMonth[month - 1] < day || day <= 0) day = 1;
	return day;
}

int Date::isLeapyear(int year) const{
	int a;
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) a = 1;
	else a = 0;
	return a;
}

int Date::operator>(Date& date) const{
	int a;
	int year2 = date.getYear(); int month2 = date.getMonth(); int day2 = date.getDay();
	int year1 = year; int month1 = month; int day1 = day;
	if (year1 > year2) a = 1;
	else if (year1<year2) a = 0;
	else
	{
		if (month1>month2) a = 1;
		else if (month1<month2) a = 0;
		else
		{
			if (day1>day2) a = 1;
			else a = 0;
		}
	}

	return a;
}

Date Date::add()
{
	int mm, yy, dd;
	mm = month;
	yy = year;
	dd = day;

	if (mm == 12)
	{
		mm = 1;
		yy = yy + 1;
	}
	else mm = mm + 1;

	Date pdate = Date(yy, mm, dd);

	return pdate;

}

void Date::init() {
	cout << "请输入今日日期(yyyy-mm-dd)：" << endl;
	int a[13] = { -1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int year1 = 0, month1 = 0, day1 = 0;
	bool isYear = false;
	bool isMonth = false;
	bool isDay = false;
	while (!isYear) {
		cout << "年：";
		cin >> year1;
		if (year1) {
			isYear = true;
			cin.clear();
			cin.sync();
		}
		else if (cin.fail() || year1 > 2000000000) {
			cout << "输入有误，请重新输入" << endl;
			cin.clear();
			cin.sync();
		}
	}
	
	cin.clear();
	cin.sync();

	while (!isMonth) {
		cout << "月： ";
		cin >> month1;
		if (month1 >=1 && month1 <= 12) {
			isMonth = true;
			cin.clear();
			cin.sync();
		}
		else if (cin.fail()) {
			cout << "输入有误，请重新输入" << endl;
			cin.clear();
			cin.sync();
		}
		else {
			cout << "输入有误，请重新输入" << endl;
		}
	}
	cin.clear();
	cin.sync();

	while (!isDay) {
		cout << "日：";
		cin >> day1;
		int maxDay = a[month1];
		if (isLeapyear(year1) && month1 == 2) {
			maxDay += 1;
		}
		if (day1 >= 1 && day1 <= maxDay) {
			isDay = true;
			cin.clear();
			cin.sync();
		}
		else if (cin.fail()) {
			cout << "输入有误，请重新输入" << endl;
			cin.clear();
			cin.sync();
		}
		else {
			cout << "输入有误，请重新输入" << endl;
		}
	}

	this->year = year1;
	this->month = month1;
	this->day = day1;
}