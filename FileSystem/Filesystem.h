#ifndef __FILESYSTEM_H_
#define __FILESYSTEM_H_
#include<iostream>
#include <string>
#define NAME 30
#define PATH 1000
using namespace std;

class File {  //文件类
public:
	char name[NAME];
	string context;  //内容
	File* next;
	File() { next = 0; }
	File(const char* nam) { strcpy(name, nam); next = 0; }
};
class Directory {  //目录类
public:
	char name[NAME];
	Directory* parent;
	File* filechild;
	Directory* dirchild;
	Directory* next;
	Directory() { parent = dirchild  = next = 0; filechild = 0; }
	Directory(const char* nam) { strcpy(name, nam); parent = dirchild = next = 0; filechild = 0;}
};
class FileSystem {
private:
	Directory* root; //根目录
	Directory* currentdir;  //当前目录
	char path[PATH];  //当前路径
public:
	FileSystem();
	~FileSystem();
	int name_is_right(const char* name);
	File* findFile(const char* name, Directory* dir = 0);  //查找文件 dir默认为0也就是当前目录
	Directory* findDir(const char* name, Directory* dir = 0); //查找目录
	Directory* path_is_right(const char* path);//路径是否正确 
	int createFile(const char* name);  //创建文件
	int createDir(const char* name);  //创建目录
	int enterDir(const char* name);   //进入目录
	int returnDir(); //回到上一个目录
	int rename(const char* name, const char* newname);  //重命名
	void showDirContext();
	int remove(const char* name);  //删除
	void deleteDir(Directory* dir);   //删除目录
	Directory* copyDir(Directory* dir); //复制目录
	char* showpath();
	int move(const char* name,const char* path);  //移动
	int cpoy(const char* str1,const char* str2);  //复制
};



#endif // __FILESYSTEM_H_
