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
	for (int i = 0; i < strlen(name); i++)   //目录名或文件名不能包含/、\、:、" 空格 ?
	{
		if (name[i] == '/'||name[i]=='\\'|| name[i] == ':'|| name[i] == '"' || name[i] == ' ' || name[i] == '?')
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
	File* nfp = findFile(newname);
	Directory* ndp = findDir(newname);
	if (fp == 0 && dp == 0)return 0;
	if (nfp != 0 || ndp != 0)return -1;
	if (fp!=0) {
		if (nfp)return -1;
		strcpy(fp->name, newname);
		return 1;
	}
	if (dp != 0) {
		if (ndp)return -1;
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
		//deleteDir(dir);
		delete dir;
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
	Directory* newdp = copyDir(dp);
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
	if (p == 0)return -1;
	if (findFile(name, p) != 0 || findDir(name,p) != 0)return  -2;
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
				dp->parent = p;
				return 1;
			}
			if (ptr->next == dp)
			{
				ptr->next = dp->next;
				dp->next = p->dirchild;
				p->dirchild = dp;
				dp->parent = p;
				return 1;
			}
			ptr = ptr->next;
		}
	}
}

int FileSystem::copy(const char* name, const char* str)// -1 命名错误 0 不存在 -2 该目录下文件或目录已存在 -3 路径不正确
{
	File* fp = findFile(name);
	Directory* dp = findDir(name);
	Directory* pp = path_is_right(str);
	if (fp == 0 && dp == 0)return 0;
	if (fp)
	{
		File* newfp = new File(fp->name);
		newfp->context = fp->context;
		if (pp)
		{
			if (findFile(name, pp))return -2;
			newfp->next = pp->filechild;
			pp->filechild = newfp;
			return 1;
		}
		else
		{
			if (strchr(str, '/')) return -3;
			if (!name_is_right(str))return -1;
			File* p = findFile(str);
			if (p == 0)
			{
				strcpy(newfp->name, str);
				newfp->next = currentdir->filechild;
				currentdir->filechild = newfp;
				return 1;
			}
			else
			{
				p->context = newfp->context;
				delete newfp;
				return 1;
			}
		}
	}
	if (dp)
	{
		Directory* newdp = copyDir(dp);
		if (pp)
		{
			if (findDir(name, pp))return -2;
			newdp->next = pp->dirchild;
			pp->dirchild = newdp;
			newdp->parent = pp;
			return 1;
		}
		return -3;
	}
	return 0;
}

string FileSystem::read(File* fp)
{
	return fp->context;
}

int FileSystem::write(File* fp,string txt)//写文件
{
	fp->context = txt;
	return 0;
}

int FileSystem::getOption(char*word,string&txt ,int type) //从文件或者键盘读入命令 1.为键盘 2.为文件
{												//返回值 0.命令错误 1命令正确 2.为命名错误 3.该目录下文件或目录不存在 4.该路径下已有同名文件 5 路径不正确
	int n = 0, len = strlen(word), i;          //6 新文件名已存在 7 已经是根目录了 -1 退出程序
	char option[10], var1[45], var2[45];
	while (word[n] == ' ')n++;
	for (i=0; n < len; n++, i++)
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

	while (word[n] == ' ')n++;
	for (i = 0; n < len; n++, i++)
	{
		if (word[n] == ' ')break;
		var2[i] = word[n];
	}
	var2[i] = '\0';

	if (n < len)return 0;  //大于两个参数

	//帮助
	if (strcmp(var1, "?") == 0 && type == 1)
	{
		if (strlen(var2) != 0) return 0;
		if (strcmp(option, "mv") == 0)
		{
			cout << "  mv      移动文件或目录" << endl;
			cout << "    mv [file] [dir]      移动该目录下的文件到该目录下的目录中" << endl;
			cout << "    mv [file] [path]     移动该目录下的文件到指定路径" << endl;
			cout << "    mv [dir]  [dir]      移动该目录下的目录到该目录下的目录中" << endl;
			cout << "    mv [dir]  [path]     移动该目录下的目录到指定路径" << endl;
		}
		else if (strcmp(option, "rnm") == 0)
		{
			cout << "  rnm     重命名文件或目录" << endl;
			cout << "    rnm [dir]  [newname]  将该目录下的目录名改为新的目录名" << endl;
			cout << "    rnm [file] [newname]  将该目录下的文件名改为新的文件名" << endl;
		}
		else if (strcmp(option, "cp") == 0)
		{
			cout << "  cp      复制文件或目录" << endl;
			cout << "    cp  [file] [newname]  在该目录下复制文件，若新文件名已存在则覆盖" << endl;
			cout << "    cp  [file] [path]     复制文件到指定路径" << endl;
			cout << "    cp  [dir] [path]      复制目录到指定路径" << endl;
		}
		else if (strcmp(option, "cd") == 0)
		{
			cout << "  cd    进入目录或返回上一级" << endl;
			cout << "    cd [dir]   进入该目录下的目录" << endl;
			cout << "    cd [path]  进入给路径的目录" << endl;
			cout << "    cd..       返回上一级目录" << endl;
		}
		else if (strcmp(option, "mkdir") == 0)
			cout << "  mkdir [dirname]  创建目录" << endl;
		else if (strcmp(option, "touch") == 0)
			cout << "  touch [filename] 创建文件" << endl;
		else if (strcmp(option, "ls") == 0)
			cout << "  ls    列出该目录下所有的文件和目录" << endl;
		else if (strcmp(option, "rm") == 0)
		{
			cout << "  rm    删除文件或目录" << endl;
			cout << "    rm [file]   删除文件" << endl;
			cout << "    rm [dir]    删除目录" << endl;
		}
		else if (strcmp(option, "r") == 0)
			cout << "  r [file]      读取文件" << endl;
		else if (strcmp(option, "w") == 0)
			cout << "  w [file]      写文件" << endl;
		else
			return 0;
		return 8;
	}

	//两个参数的
	if (strcmp(option, "mv") == 0)
	{
		int t = move(var1, var2);
		if (t == 0)return 2;
		if (t == -1)return 4;
		if (t == -2)return 3;
		return 1;
	}
	if (strcmp(option, "rnm") == 0)
	{
		int t = rename(var1, var2);
		if (t == 0)return 2;
		if (t == -1)return 6;
		return 1;
	}
	if (strcmp(option, "cp") == 0)
	{
		int t = copy(var1, var2);
		if (t == 0)return 2;
		if (t == -1)return 5;
		if (t == -2)return 3;
		if (t == -3)return 4;
		return 1;
	}


	//一个参数的

	if (strlen(var2) != 0) return 0;
	if (strcmp(option, "cd") == 0)
	{
		if (enterDir(var1) == 0)
			return 4;
		return 1;
	}
	if (strcmp(option, "mkdir") == 0)
	{
		if (name_is_right(var1))
		{
			if (createDir(var1) == 0)
				return 3;
		}
		else
			return 5;
		return 1;
	}
	if (strcmp(option, "touch") == 0)
	{
		if (name_is_right(var1))
		{
			if (createFile(var1) == 0)
				return 3;
		}
		else
			return 5;
		return 1;
	}
	if (strcmp(option, "rm") == 0)
	{
		int t = remove(var1);
		if (t == 0)return 2;
		return 1;
	}
	if (strcmp(option, "r") == 0 && type == 1)
	{
		File* p = findFile(var1);
		if (p == 0)
			return 2;
		else
			cout << read(p) << endl;
		return 8;
	}
	if (strcmp(option, "w") == 0 && type == 1)
	{
		File* p = findFile(var1);
		if (p == 0)
			return 2;
		char ch;
		string str = "";
		while ((ch = getchar()) != '$')
			str += ch;
		txt = str;
		write(p, str);
		while ((ch = getchar()) != '\n') {}
		return 1;
	}

	//无参的

	if (strlen(var1) != 0) return 0;
	if (strcmp(option, "ls") == 0 && type == 1)
	{
		showDirContext();
		return 8;
	}
	if (strcmp(option, "cd..") == 0)
	{
		if (currentdir == root)return 7;
		returnDir();
		return 1;
	}
	if (strcmp(option, "exit") == 0)
	{
		return -1;
	}
	if (strcmp(option, "help") == 0&&type==1) 
	{
		cout << "有关于某个命令的详细信息，请输入[操作符]+？" << endl;
		cout << "  ls                                列出该目录下所有的文件和目录" << endl;
		cout << "  cd    [dir/path]/..               进入目录或返回上一级" << endl;
		cout << "  touch [filename]                  创建文件" << endl;
		cout << "  mkdir [dirname]                   创建目录" << endl;
		cout << "  r     [file]                      读取文件" << endl;
		cout << "  w     [file]                      写文件" << endl;
		cout << "  rnm   [file/dir] [newname]        重命名文件或目录" << endl;
		cout << "  cp    [file/dir] [newname/path]   复制文件或目录" << endl;
		cout << "  mv    [file/dir] [dir/path]       移动文件或目录" << endl;
		cout << "  rm    [file/dir]                  删除文件或目录" << endl;
		cout << "  exit                              退出" << endl;
		return 8;
	}

	return 0;
}

