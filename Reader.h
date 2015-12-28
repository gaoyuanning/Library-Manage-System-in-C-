#ifndef READER_H_INCLUDED
#define READER_H_INCLUDED

#include <iostream>

using namespace std;

//六位表示老师，七位表示研究生，八位表示本科生
//续借次数，老师2次，学生1次,且续借时延期1个月
//借阅本数，老师8本，研究生6本，本科生4本
//老师第一次借阅还书期限为3个月，研究生为2个月，本科生为1个月

const int TEACHER_BORROW_SIZE = 8;
const int GRADUATE_BORROW_SIZE = 6;
const int UNDERGRA_BORROW_SIZE = 4;

typedef struct {
	int id;
	char password[15];//密码
	int isExist; //1代表存在，0代表删除
	char name[20];
	char major[20];
}Reader;

const int READER_SIZE = sizeof(Reader);

void printReaderInfo(Reader reader) {
	cout << "id:" << reader.id << endl; 
	cout << "姓名:" << reader.name << endl;
	cout << "专业:" << reader.major << endl;
	//cout << "密码:" << reader.password << endl;
	cout << endl;
}
#endif // READER_H_INCLUDED
