#ifndef __FILESYSTEM_H_
#define __FILESYSTEM_H_
#include<iostream>
#include<fstream>
#include <string.h>
#define NAME 30
#define PATH 1000
using namespace std;

class File {  //文件类
public:
	char name[NAME]; //文件名
	string context;  //内容
	File* next;   //指向下一个文件节点
	File() { next = 0; }
	File(const char* nam) { strcpy(name, nam); next = 0; }
};
class Directory {  //目录类
public:
	char name[NAME];  //目录名
	Directory* parent; //指向父目录
	File* filechild;  //指向文件链表的头节点
	Directory* dirchild;  //指向链表节点的头节点
	Directory* next;  //指向下一个目录节点
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
	int name_is_right(const char* name);   //判断文件名目录名是否正确
	File* findFile(const char* name, Directory* dir = 0);  //查找文件 dir默认为0也就是当前目录
	Directory* findDir(const char* name, Directory* dir = 0); //查找目录
	Directory* path_is_right(const char* path);//路径是否正确 
	int createFile(const char* name);  //创建文件
	int createDir(const char* name);  //创建目录
	int enterDir(const char* name);   //进入目录
	int returnDir(); //回到上一个目录
	int rename(const char* name, const char* newname);  //重命名
	void showDirContext();  //显示目录和文件
	int remove(const char* name);  //删除
	void deleteDir(Directory* dir);   //删除目录
	Directory* copyDir(Directory* dir); //复制目录
	char* showpath();   //返回当前路径
	int move(const char* name,const char* path);  //移动
	int copy(const char* name,const char* str);  //复制
	string read(File*fp);      //读文件
	int write(File*fp,string txt);   //写文件
	int getOption(char*word,string&txt,int type=1);//从文件或者键盘读入命令 1.为键盘 2.为文件
};

#endif // __FILESYSTEM_H_
