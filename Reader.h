#ifndef READER_H_INCLUDED
#define READER_H_INCLUDED

#include <iostream>

using namespace std;

//��λ��ʾ��ʦ����λ��ʾ�о�������λ��ʾ������
//�����������ʦ2�Σ�ѧ��1��,������ʱ����1����
//���ı�������ʦ8�����о���6����������4��
//��ʦ��һ�ν��Ļ�������Ϊ3���£��о���Ϊ2���£�������Ϊ1����

const int TEACHER_BORROW_SIZE = 8;
const int GRADUATE_BORROW_SIZE = 6;
const int UNDERGRA_BORROW_SIZE = 4;

typedef struct {
	int id;
	char password[15];//����
	int isExist; //1������ڣ�0����ɾ��
	char name[20];
	char major[20];
}Reader;

const int READER_SIZE = sizeof(Reader);

void printReaderInfo(Reader reader) {
	cout << "id:" << reader.id << endl; 
	cout << "����:" << reader.name << endl;
	cout << "רҵ:" << reader.major << endl;
	//cout << "����:" << reader.password << endl;
	cout << endl;
}
#endif // READER_H_INCLUDED
