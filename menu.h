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
			cout << "                |	         1、借书		      |" << endl;
			cout << "                |	         2、还书		      |" << endl;
			cout << "                |	         3、续借		      |" << endl;
			cout << "                |	         4、借阅历史		      |" << endl;
			cout << "                |	         5、更改密码		      |" << endl;
			cout << "                |	         0、退出	              |" << endl;
			cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "请选择：";
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
				cout << "输入有误，请重新输入" << endl;
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
			cout << "                |	         1、添加图书		      |" << endl;
			cout << "                |	         2、删除图书		      |" << endl;
			cout << "                |	         3、更改图书信息              |" << endl;
			cout << "                |	         4、添加用户		      |" << endl;
			cout << "                |	         5、删除用户		      |" << endl;
			cout << "                |	         6、更改用户信息	      |" << endl;
			cout << "                |	         7、添加管理员	              |" << endl;
			cout << "                |	         0、退出	              |" << endl;
			cout << "	         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "请选择：";
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
				cout << "输入有误，请重新输入" << endl;
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
	cout << endl << endl << "*****************************欢迎进入图书馆管理系统*****************************" << endl;
	while (!quit) {
		isLogged = false;	
		cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "                |	         1、查找图书		      |" << endl;
		cout << "                |	         2、个人登录		      |" << endl;
		cout << "                |	         3、后台登录		      |" << endl;
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
			cout << "输入有误，请重新输入" << endl;
			cin.clear();
			cin.sync();
		}
		else {
			cout << "输入有误，请重新输入" << endl;
		}
		k = -1;
	}
}