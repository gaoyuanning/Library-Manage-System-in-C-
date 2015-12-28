#include "Log.h"
#include "Book.h"
#include "BookDAO.h"
#include "Reader.h"
#include "BookTransaction.h"
#include <iostream>
#include <string>
using namespace std;

void userMenu() {
	if (checkLog()) {
		bool quit = false;
		while (!quit) {
			cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "                |	         1������		      |" << endl;
			cout << "                |	         2������		      |" << endl;
			cout << "                |	         3������		      |" << endl;
			cout << "                |	         4��������ʷ		      |" << endl;
			cout << "                |	         5����������		      |" << endl;
			cout << "                |	         0���˳�	              |" << endl;
			cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "��ѡ��";
			int k;
			cin >> k;
			if (k >= 0 && k <= 5) {
				cin.clear();
				cin.sync();

				switch (k) {
				case 1:
					borrow();
					break;
				case 2:
					returnBook();
					break;
				case 3:
					renewBook();
					break;
				case 4:
					borrowHistory();
					break;
				case 5:
					changePasswd();
					break;
				case 0:
					quit = true;
					break;
				default:
					break;
				}
				cin.clear();
				cin.sync();
			}
			else if (cin.fail()) {
				cout << "������������������" << endl;
				cin.clear();
				cin.sync();
			}
			k = -1;
		}
	}
}

void admMenu() {
	if (checkLog2()) {
		//if (true){
		bool quit = false;
		while (!quit) {
			cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "                |	         1�����ͼ��		      |" << endl;
			cout << "                |	         2��ɾ��ͼ��		      |" << endl;
			cout << "                |	         3������ͼ����Ϣ              |" << endl;
			cout << "                |	         4������û�		      |" << endl;
			cout << "                |	         5��ɾ���û�		      |" << endl;
			cout << "                |	         6�������û���Ϣ	      |" << endl;
			cout << "                |	         7����ӹ���Ա	              |" << endl;
			cout << "                |	         0���˳�	              |" << endl;
			cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "��ѡ��";
			int k;
			cin >> k;
			if (k >= 0 && k <= 7) {
				cin.clear();
				cin.sync();

				switch (k) {
				case 1:
					addBook();
					break;
				case 2:
					deleteBook();
					break;
				case 3:
					updateBookInfo();
					break;
				case 4:
					addReader();
					break;
				case 5:
					deleteReader();
					break;
				case 6:
					updateReaderInfo();
					break;
				case 7:
					addAdm();
					break;
				case 0:
					quit = true;
					admLogged = false;
					break;
				default:
					break;
				}
				cin.clear();
				cin.sync();
			}
			else if (cin.fail()) {
				cout << "������������������" << endl;
				cin.clear();
				cin.sync();
			}
			k = -1;
		}
	}
}

void displayMenu() {
	date.init();
	bool quit = false;
	cout << endl << endl << "*****************************��ӭ����ͼ��ݹ���ϵͳ*****************************" << endl;
	while (!quit) {
		isLogged = false;	
		cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "                |	         1������ͼ��		      |" << endl;
		cout << "                |	         2�����˵�¼		      |" << endl;
		cout << "                |	         3����̨��¼		      |" << endl;
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
				borrow();
				break;
			case 2:
				userMenu();
				break;
			case 3:
				admMenu();
				break;
			case 0:
				quit = true;
				break;
			default:
				break;
			}
			cin.clear();
			cin.sync();
		}
		else if (cin.fail()) {
			cout << "������������������" << endl;
			cin.clear();
			cin.sync();
		}
		else {
			cout << "������������������" << endl;
		}
		k = -1;
	}
}