#include"Filesystem.h"

FileSystem::FileSystem()
{
	root = new Directory("root");
	strcpy(path, "root");
	currentdir = root;
}

FileSystem::~FileSystem()
{
	deleteDir(root);
}

int FileSystem::name_is_right(const char* name)
{
	for (int i = 0; i < strlen(name); i++)   //目录名或文件名不能包含/、\、:、"
	{
		if (name[i] == '/'||name[i]=='\\'|| name[i] == ':'|| name[i] == '"')
			return 0;
	}
	return 1;
}

File* FileSystem::findFile(const char* name, Directory* dir)
{
	if (dir == 0)dir = currentdir;
	File* p = dir->filechild;
	while (p)
	{
		if (strcmp(p->name, name) == 0)
			return p;
		p = p->next;
	}
	return 0;
}

Directory* FileSystem::findDir(const char* name, Directory* dir)
{
	if (dir == 0)dir = currentdir;
	Directory* p = dir->dirchild;
	while (p)
	{
		if (strcmp(p->name, name) == 0)
			return p;
		p = p->next;
	}
	return 0;
}

Directory* FileSystem::path_is_right(const char* path)
{
	if (path[0] == '/')return 0;
	Directory* p;
	char nam[30];
	int num = 0;
	for (int i = 0,j = 0; i <= strlen(path); i++,j++)
	{
		if (path[i] != '/'&&i!= strlen(path))
		{
			nam[j] = path[i];
		}
		else if(path[i] == '/'||i== strlen(path))
		{
			nam[j++] = '\0';
			num++;
			if (num == 1)
			{
				if (strcmp(root->name, nam) == 0)p = root;
				else
				{
					p = findDir(nam);
					if (p == 0)return 0;
				}
			}
			else
			{
				p = findDir(nam, p);
				if (p == 0)return 0;
			}
			j = -1;
		}
	}
	return  p;
}

int FileSystem::createFile(const char* name)
{
	if (findFile(name))return 0;     //有同名文件返回0
	if (findDir(name))return 0;     //有同名目录返回0
	File* file = new File(name);
	file->next = currentdir->filechild;
	currentdir->filechild = file;
	return 1;
}

int FileSystem::createDir(const char* name)
{
	if (findDir(name))return 0;     //有同名目录返回0
	if (findFile(name))return 0;     //有同名文件返回0
	Directory* dir = new Directory(name);
	dir->next = currentdir->dirchild;
	currentdir->dirchild = dir;
	dir->parent = currentdir;
	return 1;
}

int FileSystem::enterDir(const char* name)
{
	Directory* p = findDir(name);
	if (p != 0) {
		strcat(path, "/");
		strcat(path, name);
		currentdir = p;
		return 1;
	}
	p = path_is_right(name);
	if (p != 0)
	{
		strcpy(path, name);
		currentdir = p;
		return 1;
	}
	return 0;
}

int FileSystem::returnDir()
{
	if (currentdir->parent != 0)
	{
		path[strlen(path) - strlen(currentdir->name)-1] = '\0';
		currentdir = currentdir->parent;
		return 1;
	}
	return 0;
}

int FileSystem::rename(const char* name, const char*newname)//0.文件不存在 -1.新文件名已存在
{
	File* fp= findFile(name);
	Directory* dp = findDir(name);
	if (fp == 0 && dp == 0)return 0;
	if (fp!=0) {
		if (findFile(newname))return -1;
		strcpy(fp->name, newname);
		return 1;
	}
	if (dp != 0) {
		if (findDir(newname))return -1;
		strcpy(dp->name, newname);
		return 1;
	}
}

void FileSystem::showDirContext()
{
	File* filep = currentdir->filechild;
	Directory* dirp = currentdir->dirchild;
	cout << "file:";
	while (filep) {
		cout << filep->name << " ";
		filep = filep->next;
	}
	cout << "\ndirectory:";
	while (dirp) {
		cout << dirp->name << " ";
		dirp = dirp->next;
	}
	cout << endl;
}

int FileSystem::remove(const char* name)
{
	File* fp = findFile(name);
	if (fp != 0) {
		File* ptr = currentdir->filechild;
		while (ptr) {
			if (ptr == fp)
			{
				currentdir->filechild = fp->next;
				delete fp;
				return 1;
			}
			if (ptr->next == fp)
			{
				ptr->next = fp->next;
				delete fp;
				return 1;
			}
			ptr = ptr->next;
		}
	}

	Directory* dp = findDir(name);
	if (dp != 0) {
		Directory* ptr = currentdir->dirchild;
		while (ptr) {
			if (ptr == dp)
			{
				currentdir->dirchild = dp->next;
				deleteDir(dp);
				return 1;
			}
			if (ptr->next == dp)
			{
				ptr->next = dp->next;
				deleteDir(dp);
				return 1;
			}
			ptr = ptr->next;
		}
	}
	return 0;
}

void FileSystem::deleteDir(Directory* dir)
{
	if (dir->dirchild == 0 && dir->filechild == 0)delete dir;
	else
	{
		File* filep = dir->filechild;
		Directory* dirp = dir->dirchild;
		while (filep) {
			File* p = filep;
			filep = filep->next;
			delete p;
		}
		dir->filechild = 0;
		while (dirp) {
			Directory* p = dirp;
			dirp = dirp->next;
			deleteDir(p);
		}
		dir->dirchild = 0;
		deleteDir(dir);
	}
}

Directory* FileSystem::copyDir(Directory* dir)
{
	Directory* newdir = new Directory(dir->name);
	if (dir->dirchild == 0 && dir->filechild == 0)return newdir;

	File* fp = dir->filechild;
	File* newfp = new File(fp->name);
	newfp->context = fp->context;
	newdir->filechild = newfp;
	fp = fp->next;
	while (fp)
	{
		newfp->next = new File(fp->name);
		newfp = newfp->next;
		newfp->context = fp->context;
		fp = fp->next;
	}

	Directory* dp = dir->dirchild;
	Directory* newdp = new Directory(dp->name);
	newdir->dirchild = newdp;
	newdp->parent = newdir;
	dp = dp->next;
	while (dp)
	{
		newdp->next = copyDir(dp);
		newdp = newdp->next;
		newdp->parent = newdir;
		dp = dp->next;
	}
	return newdir;
}


char* FileSystem::showpath()
{
	return path;
}

int FileSystem::move(const char* name, const char* path) // 0表示该目录下没有该名字的文件 -1表示路径不正确 -2表示该路径下已有同名文件
{
	File* fp = findFile(name);
	Directory* dp = findDir(name);
	if (fp == 0 && dp == 0)return 0;
	Directory* p = path_is_right(path);
	if (findFile(name, p) != 0 && findDir(name) != 0)return  -2;
	if (p == 0)return -1;
	if (fp != 0)
	{
		File* ptr = currentdir->filechild;
		while (ptr) {
			if (ptr == fp)
			{
				currentdir->filechild = fp->next;
				fp->next = p->filechild;
				p->filechild = fp;
				return 1;
			}
			if (ptr->next == fp)
			{
				ptr->next = fp->next;
				fp->next = p->filechild;
				p->filechild = fp;
				return 1;
			}
			ptr = ptr->next;
		}
	}
	if (dp != 0)
	{
		Directory* ptr = currentdir->dirchild;
		while (ptr) {
			if (ptr == dp)
			{
				currentdir->dirchild = dp->next;
				dp->next = p->dirchild;
				p->dirchild = dp;
				return 1;
			}
			if (ptr->next == dp)
			{
				ptr->next = dp->next;
				dp->next = p->dirchild;
				p->dirchild = dp;
				return 1;
			}
			ptr = ptr->next;
		}
	}
}

int FileSystem::cpoy(const char* str1, const char* str2)
{
	
	return 0;
}




