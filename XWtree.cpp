#include "XWtree.h"
#include <string.h>
#include <stdlib.h>

static bool Loaded = false;

Tree FindDirectory(Tree t,const char *Path,int layer,int total)
{
	char name[NAMESIZE];
	getNodeName(name,Path,layer);
	if(!strcmp(name,t->name)&&!t->IsFile)
	{
		if(layer==(total-1))
			return t;
		getNodeName(name,Path,layer+1);
		Tree temp=t->son;
		while(temp!=NULL)
		{
			if(!temp->IsFile&&!strcmp(temp->name,name))
			{
				return FindDirectory(temp,Path,layer+1,total);
			}
			else
				temp=temp->brother;
		}
		return NULL;
	}
	else
	{
		return NULL;
	}	
}

void getNodeName(char *des,const char *path,int layer)
{
	int srcStart=0;
	
	for(int i=0;i!=layer;)
	{
		if(path[srcStart++]=='/')
			i++;
	}
	
	
	int srcEnd=srcStart;
	
	while(path[srcEnd]!='/')
		srcEnd++;
	
	strncpy_s(des,50,&path[srcStart],srcEnd-srcStart);
	des[srcEnd-srcStart]='\0';
	
}

bool Initialize(Tree *t)
{	
	*t=(Tree)malloc(sizeof(treeNode));
	
	(*t)->IsFile=false;
	strcpy_s((*t)->name,NAMESIZE,"root");
	(*t)->son=NULL;
	(*t)->brother=NULL;
	
	return true;
}

Choice GetChoice()
{
	int choice;
	printf("\n请选择你要的操作：\n");
	printf("0.新增文件（夹）   1.先序遍历系统   2.后序遍历系统    3.退出程序\n");
	
	scanf_s("%d",&choice);
	while(getchar()!='\n');
	switch(choice)
	{
		case 0:return INSERT;
		case 1:return PREORDER;
		case 2:return POSTORDER;
		default:return QUIT;
	}
}

void Insert(Tree t)
{
	char insertPath[PATHSIZE];
	printf("请输入要新增文件（夹）的路径，格式如此：root/xxx/xxx/\n");
	scanf_s("%s", insertPath, PATHSIZE);
	
	int n=0;  //n为inserPath中'/'的个数，即“结点个数”
	int i=0;
	while(insertPath[i]!='\0')
		if(insertPath[i++]=='/')
			n++;
	
	
	Tree directory=FindDirectory(t,insertPath,0,n);
	
	if(directory==NULL)
	{
		printf("路径错误！");
		return;
	}
	
	printf("请选择你要新增的类型：0.文件夹   1.文件\n");
	int temp;
	scanf_s("%d",&temp);
	while(getchar()!='\n');
	
	Tree newNode=(Tree)malloc(sizeof(treeNode));
	switch(temp)
	{
		case 0:newNode->IsFile=false;break;
		case 1:newNode->IsFile=true;break;
		default:printf("不存在该选项");return;
	}
	
	printf("请输入文件（夹）名：\n");
	char name[NAMESIZE];
	scanf_s("%s",name,NAMESIZE);
	newNode->son=newNode->brother=NULL;
	strcpy_s(newNode->name,50,name);
	
	if(directory->son!=NULL)
	{
		newNode->brother=directory->son;
		directory->son=newNode;
	}
	else
	{
		directory->son=newNode;
	}
	
	printf("插入成功\n");
}

void PrintPreOrder(Tree t,int layer)
{

	if(t==NULL)
		return;

	for(int i=0;i<layer;++i)
		putchar('\t');
	printf("%s\n",t->name);
	
	Tree temp=t->son;
	while(temp!=NULL)
	{
		PrintPreOrder(temp,layer+1);
		temp=temp->brother;
	}
}

void PrintPostOrder(Tree t,int layer)
{
	if(t==NULL)
		return;
	
	
	Tree temp=t->son;
	while(temp!=NULL)
	{
		PrintPostOrder(temp,layer+1);
		temp=temp->brother;
	}
	
	
	for(int i=0;i<layer;++i)
		putchar('\t');
	printf("%s\n",t->name);

}

int Count(Tree t)
{
	int total = 0;
	if (t == NULL)
		return total;

	total++;
	Tree temp = t->son;
	while (temp != NULL)
	{
		total += Count(temp);
		temp = temp->brother;
	}

	return total;
}


void FreeTree(Tree t)
{
	if(t==NULL)
		return;
	
	Tree temp = NULL;
	Tree t_son=t->son;
	while(t_son!=NULL)
	{
		temp = t_son->brother;
		FreeTree(t_son);
		t_son=temp;
	}
	
	free(t);
}

void Store(Tree t)
{
	FILE *fp;
	fopen_s(&fp, "FileSystem.xw", "wb+");
	int size = Count(t);
	fwrite(&size, sizeof(int), 1, fp);
	fileNode *file = (fileNode*)malloc(sizeof(fileNode)*size);
	StoreToArray(file, t, -1,0);

	fwrite(file, sizeof(fileNode), size, fp);

	fclose(fp);
	free(file);
	if (Loaded)
		free(t);
	else
		FreeTree(t);
}

int StoreToArray(fileNode *file, Tree t,int father,int ths)
{
	if (t == NULL)
		return ths;

	file[ths].father = father;
	file[ths].IsFile = t->IsFile;
	strcpy_s(file[ths].name, NAMESIZE, t->name);

	Tree temp = t->son;
	int i = ths+1;
	while (temp != NULL)
	{
		i = StoreToArray(file, temp, ths, i);
		temp = temp->brother;
	}

	return i;
}



bool Load(Tree *t)
{
	FILE *fp;
	if (fopen_s(&fp, "FileSystem.xw", "rb"))
		return false;

	int size = 0;
	fread(&size, sizeof(int), 1, fp);
	fileNode *file = (fileNode*)malloc(sizeof(fileNode)*size);
	treeNode *data = (treeNode *)malloc(sizeof(treeNode)*size);
	fread(file, sizeof(fileNode), size, fp);
	fclose(fp);

	for (int i = 0;i < size;++i)
	{
		data[i].IsFile = file[i].IsFile;
		strcpy_s(data[i].name, NAMESIZE, file[i].name);
		data[i].son = data[i].brother = NULL;
	}

	Tree temp = NULL;
	for (int i = 0;i < size;++i)
	{
		for (int j = i + 1;j < size;++j)
		{
			if (file[j].father == i)
			{
				if (data[i].son == NULL)
				{
					data[i].son = &data[j];
				}
				else
				{
					temp = data[i].son;
					while (temp->brother != NULL)
						temp = temp->brother;
					temp->brother = &data[j];
				}
			}
		}
	}
	free(file);
	*t = data;
	Loaded = true;
	return true;
}



