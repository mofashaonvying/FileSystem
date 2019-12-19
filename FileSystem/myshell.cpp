#include<iostream>
#include"Filesystem.h"
using namespace std;
int main()
{
	string username, password,file,psd,txt;
	ifstream infile;
	ofstream outfile;
	char word[100];
	int choice;
	do {
		FileSystem f;
		cout << "***文件管理系统***" << endl;
		cout << "1.登录" << endl;
		cout << "2.注册" << endl;
		cout << "0.退出" << endl;
		cout << "请输入序号选择: ";
		cin >> choice;
		if (choice == 0)break;
		if(choice !=1 && choice != 2)
		{
			cout << "输入的序号错误\n";
			cout << "请按任意键继续...";
			cin.get();
			system("cls");
			continue;
		}
		if (choice == 1)
		{
			do {
				cout << "请输入用户名：";
				cin >> username;
				cout << "请输入密码：";
				cin >> password;
				file = username + ".txt";
				infile.open(file, ios::in);
				if (!infile)cout << "用户名不存在" << endl;
			} while (!infile);
			infile >> psd;
			while (password != psd)
			{
				cout << "密码错误" << endl;
				cout << "请重新输入密码:";
				cin >> password;
			}
			cout << "登录成功" << endl;

			infile.getline(word, 100, '\n');
			while (true)
			{
				infile.getline(word, 100, '\n');
				f.getOption(word, txt, 2);
				//对write的操作
				int n = 0, len = strlen(word), i;
				char option[10], var1[45];
				while (word[n] == ' ')n++;
				if (word[n] == 'w')
				{
					for (i = 0; n < len; n++, i++)
					{
						if (word[n] == ' ')break;
						option[i] = word[n];
					}
					option[i] = '\0';

					while (word[n] == ' ')n++;
					for (i = 0; n < len; n++, i++)
					{
						if (word[n] == ' ')break;
						var1[i] = word[n];
					}
					var1[i] = '\0';
					if (strcmp(option, "w") == 0)
					{
						File* p = f.findFile(var1);
						char ch;
						string str = "";
						while ((ch = infile.get()) != '$')
							str += ch;
						f.write(p, str);
						infile.get();
					}
				}
				if (word[0] == '\0')break;
			}
			infile.close();
		}
		if (choice == 2)
		{
			while(true)
			{
				cout << "请输入用户名：";
				cin >> username;
				cout << "请输入密码：";
				cin >> password;
				file = username + ".txt";
				outfile.open(file, ios::in);
				if (!outfile)
				{
					outfile.open(file, ios::out);
					cout << "注册成功" << endl;
					outfile << password << endl;
					outfile.close();
					break;
					
				}
				else
				{
					cout << "该用户名已存在" << endl;
					outfile.close();
				}
			}
		}

		outfile.open(file, ios::app);
		while (cin.getline(word, 100, '\n'))
		{
			if (word[0] == '\0')
			{
				cout << "\n" << f.showpath() << ">";
				continue;
			}
			int t;
			txt = "";
			t = f.getOption(word, txt);
			if (t == -1)break;
			switch (t)
			{
			case 0:cout << "命令错误" << endl;
				break;
			case 2:cout << "该目录下文件或目录不存在" << endl;
				break;
			case 3:cout << "该路径下已有同名文件" << endl;
				break;
			case 4:cout << "路径不正确" << endl;
				break;
			case 5:cout << "命名错误，不能包含非法字符/、\\、:、\" 空格、? " << endl;
				break;
			case 6:cout << "新文件名已存在" << endl;
				break;
			case 7:cout << "已经是根目录了 " << endl;
				break;
			default:break;
			}
			if (t == 1)outfile << word << endl;
			if (!txt.empty())outfile << txt << '$' << endl;
			cout << "\n" << f.showpath() << ">";
		}
		outfile.close();
		cout << "已登出...";
		cin.get();
		system("cls");
	} while (choice);

	cout << "请按任意键继续...";
	cin.get();
	return 0;
}