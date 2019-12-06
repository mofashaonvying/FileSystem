#include<iostream>
#include"Filesystem.h"
using namespace std;
int main()
{
	FileSystem f;
	f.createDir("1");
	f.createFile("1.c");
	f.showDirContext();
	f.enterDir("1");
	f.createDir("2");
	f.createFile("2.c");
	f.showDirContext();
	cout << f.showpath() << endl;
	f.remove("2");
	f.showDirContext();
	cout << f.showpath() << endl;
	cout << f.path_is_right("root/1")->name << endl;
	f.createFile("123.c");
	f.showDirContext();
	f.rename("123.c", "123456.c");
	f.showDirContext();
	cout << endl;
	f.createFile("123.c");
	f.showDirContext();
	f.move("123.c", "root/1");
	f.enterDir("root");
	cout << f.showpath() << endl;
	f.showDirContext();

	f.enterDir("1");
	cout << f.showpath() << endl;
	f.returnDir();
	cout << f.showpath() << endl;
	system("pause");
	return 0;
}