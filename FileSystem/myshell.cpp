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
		cout << "***�ļ�����ϵͳ***" << endl;
		cout << "1.��¼" << endl;
		cout << "2.ע��" << endl;
		cout << "0.�˳�" << endl;
		cout << "���������ѡ��: ";
		cin >> choice;
		if (choice == 0)break;
		if(choice !=1 && choice != 2)
		{
			cout << "�������Ŵ���\n";
			cout << "�밴���������...";
			cin.get();
			system("cls");
			continue;
		}
		if (choice == 1)
		{
			do {
				cout << "�������û�����";
				cin >> username;
				cout << "���������룺";
				cin >> password;
				file = username + ".txt";
				infile.open(file, ios::in);
				if (!infile)cout << "�û���������" << endl;
			} while (!infile);
			infile >> psd;
			while (password != psd)
			{
				cout << "�������" << endl;
				cout << "��������������:";
				cin >> password;
			}
			cout << "��¼�ɹ�" << endl;

			infile.getline(word, 100, '\n');
			while (true)
			{
				infile.getline(word, 100, '\n');
				f.getOption(word, txt, 2);
				//��write�Ĳ���
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
				cout << "�������û�����";
				cin >> username;
				cout << "���������룺";
				cin >> password;
				file = username + ".txt";
				outfile.open(file, ios::in);
				if (!outfile)
				{
					outfile.open(file, ios::out);
					cout << "ע��ɹ�" << endl;
					outfile << password << endl;
					outfile.close();
					break;
					
				}
				else
				{
					cout << "���û����Ѵ���" << endl;
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
			case 0:cout << "�������" << endl;
				break;
			case 2:cout << "��Ŀ¼���ļ���Ŀ¼������" << endl;
				break;
			case 3:cout << "��·��������ͬ���ļ�" << endl;
				break;
			case 4:cout << "·������ȷ" << endl;
				break;
			case 5:cout << "�������󣬲��ܰ����Ƿ��ַ�/��\\��:��\" �ո�? " << endl;
				break;
			case 6:cout << "���ļ����Ѵ���" << endl;
				break;
			case 7:cout << "�Ѿ��Ǹ�Ŀ¼�� " << endl;
				break;
			default:break;
			}
			if (t == 1)outfile << word << endl;
			if (!txt.empty())outfile << txt << '$' << endl;
			cout << "\n" << f.showpath() << ">";
		}
		outfile.close();
		cout << "�ѵǳ�...";
		cin.get();
		system("cls");
	} while (choice);

	cout << "�밴���������...";
	cin.get();
	return 0;
}