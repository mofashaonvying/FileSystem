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
	for (int i = 0; i < strlen(name); i++)   //Ŀ¼�����ļ������ܰ���/��\��:��" �ո� ?
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
	if (findFile(name))return 0;     //��ͬ���ļ�����0
	if (findDir(name))return 0;     //��ͬ��Ŀ¼����0
	File* file = new File(name);
	file->next = currentdir->filechild;
	currentdir->filechild = file;
	return 1;
}

int FileSystem::createDir(const char* name)
{
	if (findDir(name))return 0;     //��ͬ��Ŀ¼����0
	if (findFile(name))return 0;     //��ͬ���ļ�����0
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

int FileSystem::rename(const char* name, const char*newname)//0.�ļ������� -1.���ļ����Ѵ���
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

int FileSystem::move(const char* name, const char* path) // 0��ʾ��Ŀ¼��û�и����ֵ��ļ� -1��ʾ·������ȷ -2��ʾ��·��������ͬ���ļ�
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

int FileSystem::copy(const char* name, const char* str)// -1 �������� 0 ������ -2 ��Ŀ¼���ļ���Ŀ¼�Ѵ��� -3 ·������ȷ
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

int FileSystem::write(File* fp,string txt)//д�ļ�
{
	fp->context = txt;
	return 0;
}

int FileSystem::getOption(char*word,string&txt ,int type) //���ļ����߼��̶������� 1.Ϊ���� 2.Ϊ�ļ�
{												//����ֵ 0.������� 1������ȷ 2.Ϊ�������� 3.��Ŀ¼���ļ���Ŀ¼������ 4.��·��������ͬ���ļ� 5 ·������ȷ
	int n = 0, len = strlen(word), i;          //6 ���ļ����Ѵ��� 7 �Ѿ��Ǹ�Ŀ¼�� -1 �˳�����
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

	if (n < len)return 0;  //������������

	//����
	if (strcmp(var1, "?") == 0 && type == 1)
	{
		if (strlen(var2) != 0) return 0;
		if (strcmp(option, "mv") == 0)
		{
			cout << "  mv      �ƶ��ļ���Ŀ¼" << endl;
			cout << "    mv [file] [dir]      �ƶ���Ŀ¼�µ��ļ�����Ŀ¼�µ�Ŀ¼��" << endl;
			cout << "    mv [file] [path]     �ƶ���Ŀ¼�µ��ļ���ָ��·��" << endl;
			cout << "    mv [dir]  [dir]      �ƶ���Ŀ¼�µ�Ŀ¼����Ŀ¼�µ�Ŀ¼��" << endl;
			cout << "    mv [dir]  [path]     �ƶ���Ŀ¼�µ�Ŀ¼��ָ��·��" << endl;
		}
		else if (strcmp(option, "rnm") == 0)
		{
			cout << "  rnm     �������ļ���Ŀ¼" << endl;
			cout << "    rnm [dir]  [newname]  ����Ŀ¼�µ�Ŀ¼����Ϊ�µ�Ŀ¼��" << endl;
			cout << "    rnm [file] [newname]  ����Ŀ¼�µ��ļ�����Ϊ�µ��ļ���" << endl;
		}
		else if (strcmp(option, "cp") == 0)
		{
			cout << "  cp      �����ļ���Ŀ¼" << endl;
			cout << "    cp  [file] [newname]  �ڸ�Ŀ¼�¸����ļ��������ļ����Ѵ����򸲸�" << endl;
			cout << "    cp  [file] [path]     �����ļ���ָ��·��" << endl;
			cout << "    cp  [dir] [path]      ����Ŀ¼��ָ��·��" << endl;
		}
		else if (strcmp(option, "cd") == 0)
		{
			cout << "  cd    ����Ŀ¼�򷵻���һ��" << endl;
			cout << "    cd [dir]   �����Ŀ¼�µ�Ŀ¼" << endl;
			cout << "    cd [path]  �����·����Ŀ¼" << endl;
			cout << "    cd..       ������һ��Ŀ¼" << endl;
		}
		else if (strcmp(option, "mkdir") == 0)
			cout << "  mkdir [dirname]  ����Ŀ¼" << endl;
		else if (strcmp(option, "touch") == 0)
			cout << "  touch [filename] �����ļ�" << endl;
		else if (strcmp(option, "ls") == 0)
			cout << "  ls    �г���Ŀ¼�����е��ļ���Ŀ¼" << endl;
		else if (strcmp(option, "rm") == 0)
		{
			cout << "  rm    ɾ���ļ���Ŀ¼" << endl;
			cout << "    rm [file]   ɾ���ļ�" << endl;
			cout << "    rm [dir]    ɾ��Ŀ¼" << endl;
		}
		else if (strcmp(option, "r") == 0)
			cout << "  r [file]      ��ȡ�ļ�" << endl;
		else if (strcmp(option, "w") == 0)
			cout << "  w [file]      д�ļ�" << endl;
		else
			return 0;
		return 8;
	}

	//����������
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


	//һ��������

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

	//�޲ε�

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
		cout << "�й���ĳ���������ϸ��Ϣ��������[������]+��" << endl;
		cout << "  ls                                �г���Ŀ¼�����е��ļ���Ŀ¼" << endl;
		cout << "  cd    [dir/path]/..               ����Ŀ¼�򷵻���һ��" << endl;
		cout << "  touch [filename]                  �����ļ�" << endl;
		cout << "  mkdir [dirname]                   ����Ŀ¼" << endl;
		cout << "  r     [file]                      ��ȡ�ļ�" << endl;
		cout << "  w     [file]                      д�ļ�" << endl;
		cout << "  rnm   [file/dir] [newname]        �������ļ���Ŀ¼" << endl;
		cout << "  cp    [file/dir] [newname/path]   �����ļ���Ŀ¼" << endl;
		cout << "  mv    [file/dir] [dir/path]       �ƶ��ļ���Ŀ¼" << endl;
		cout << "  rm    [file/dir]                  ɾ���ļ���Ŀ¼" << endl;
		cout << "  exit                              �˳�" << endl;
		return 8;
	}

	return 0;
}

