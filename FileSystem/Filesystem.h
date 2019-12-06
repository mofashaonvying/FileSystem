#ifndef __FILESYSTEM_H_
#define __FILESYSTEM_H_
#include<iostream>
#include <string>
#define NAME 30
#define PATH 1000
using namespace std;

class File {  //�ļ���
public:
	char name[NAME];
	string context;  //����
	File* next;
	File() { next = 0; }
	File(const char* nam) { strcpy(name, nam); next = 0; }
};
class Directory {  //Ŀ¼��
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
	Directory* root; //��Ŀ¼
	Directory* currentdir;  //��ǰĿ¼
	char path[PATH];  //��ǰ·��
public:
	FileSystem();
	~FileSystem();
	int name_is_right(const char* name);
	File* findFile(const char* name, Directory* dir = 0);  //�����ļ� dirĬ��Ϊ0Ҳ���ǵ�ǰĿ¼
	Directory* findDir(const char* name, Directory* dir = 0); //����Ŀ¼
	Directory* path_is_right(const char* path);//·���Ƿ���ȷ 
	int createFile(const char* name);  //�����ļ�
	int createDir(const char* name);  //����Ŀ¼
	int enterDir(const char* name);   //����Ŀ¼
	int returnDir(); //�ص���һ��Ŀ¼
	int rename(const char* name, const char* newname);  //������
	void showDirContext();
	int remove(const char* name);  //ɾ��
	void deleteDir(Directory* dir);   //ɾ��Ŀ¼
	Directory* copyDir(Directory* dir); //����Ŀ¼
	char* showpath();
	int move(const char* name,const char* path);  //�ƶ�
	int cpoy(const char* str1,const char* str2);  //����
};



#endif // __FILESYSTEM_H_
