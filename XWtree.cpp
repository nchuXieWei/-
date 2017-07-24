#include "XWtree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//非main中使用的子函数
int StoreToArray(fileNode *a, Tree t, int father, int ths);
int Count(Tree t);
void FreeTree(Tree t);
Tree FindDirectory(Tree t, const char *insertPath, int layer, int total);
void getNodeName(char *des, const char *source, int layer);
void FreeTree(Tree t);




//用于得到path中指定层数的结点名，层数从0开始
//如path为root/xxx/yyy/时，若layer为2则des将被写为yyy，若layer为0则des将被写为root
//path必须为‘/’结尾
void getNodeName(char *des, const char *path, int layer)
{
	int srcStart = 0;

	for (int i = 0;i != layer;)
	{
		if (path[srcStart++] == '/')
			i++;
	}

	int srcEnd = srcStart;

	while (path[srcEnd] != '/')
		srcEnd++;

	strncpy_s(des, 50, &path[srcStart], srcEnd - srcStart);
	des[srcEnd - srcStart] = '\0';
}




//寻找参数Path中某文件夹结点的地址，参数target用于指明寻找路径中的哪一层文件夹，层数从0开始
//如Path为root/xxx/yyy/时，如果target为2则返回yyy的地址，target为0则返回root的地址
//参数layer调用者一般设为0，用于函数递归实现
//若不存在目标文件夹（目标结点为文件或target大于path中的层数），则返回NULL
Tree FindDirectory(Tree t,const char *path,int layer,int target)
{
	char name[NAMESIZE];
	getNodeName(name,path,layer);
	//若当前层次的结点名字与path中对应层次的名字相同，且当前层次结点是文件夹，则继续向下寻找，否则返回NULL
	if(!strcmp(name,t->name)&&!t->IsFile)
	{
		if(layer==target)
			return t;
		getNodeName(name,path,layer+1);
		Tree temp=t->son;
		while(temp!=NULL)
		{
			if(!temp->IsFile&&!strcmp(temp->name,name))
				return FindDirectory(temp,path,layer+1,target);
			else
				temp=temp->brother;
		}
		return NULL;
	}
	else return NULL;
}




//初始化树t，使其为文件夹且名为root
bool Initialize(Tree *t)
{	
	*t=(Tree)malloc(sizeof(treeNode));
	(*t)->IsFile=false;
	strcpy_s((*t)->name,NAMESIZE,"root");
	(*t)->son=NULL;
	(*t)->brother=NULL;
	return true;
}




//提示用户如何操作并获取用户所选的操作
Choice GetChoice()
{
	int choice;
	printf("\n请选择你要的操作：\n");
	printf("0.新增文件（夹）   1.删除文件（夹）    2.先序遍历系统   3.后序遍历系统    4.退出程序\n");
	scanf_s("%d",&choice);
	
	switch(choice)
	{
		case 0:return INSERT;
		case 1:return DELETE;
		case 2:return PREORDER;
		case 3:return POSTORDER;
		default:return QUIT;
	}
}



//在t中插入新结点
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
	
	Tree directory=FindDirectory(t,insertPath,0,n-1);
	
	if(directory==NULL)
	{
		printf("路径错误！");
		return;
	}
	
	printf("请选择你要新增的类型：0.文件夹   1.文件\n");
	int nodeType;
	scanf_s("%d",&nodeType);

	Tree newNode=(Tree)malloc(sizeof(treeNode));
	switch(nodeType)
	{
		case 0:newNode->IsFile=false;break;
		case 1:newNode->IsFile=true;break;
		default:printf("不存在该选项");return;
	}
	
	printf("请输入文件（夹）名：\n");
	char name[NAMESIZE];
	scanf_s("%s",name,NAMESIZE);
	newNode->son = newNode->brother = NULL;
	strcpy_s(newNode->name,50,name);
	
	if(directory->son!=NULL)
	{
		newNode->brother=directory->son;
		directory->son=newNode;
	}
	else
		directory->son=newNode;
	
	printf("插入成功\n\n");
}


//先序输出树
//调用者将参数layer设为0，该参数用于函数递归实现及打印
void PrintPreOrder(Tree t,int layer)
{
	if(t==NULL)
		return;

	//根据当前结点在树中的层决定打印多少个制表符
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



//后序输出树
//调用者将参数layer设为0，该参数用于函数递归实现及打印
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
	
	//根据当前结点在树中的层决定打印多少个制表符
	for(int i=0;i<layer;++i)
		putchar('\t');
	printf("%s\n",t->name);

}




//统计树中有多少个结点
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



//释放树中的每一个结点
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




//退出程序时将树存入同级目录下的文件FileSystem.xw中
void Store(Tree t)
{
	FILE *fp;
	fopen_s(&fp, "FileSystem.xw", "wb+");

	//统计树中有多少结点并将该信息写入文件中，用于加载文件时快速确定重建的树需要多少结点
	int size = Count(t);
	fwrite(&size, sizeof(int), 1, fp);

	//根据结点的数量开辟一个等大小的fileNode型数组，将树中所有结点通过先序顺序存入数组，然后将该数组写入文件
	//数组中每个结点均记录了该结点在原树中的father在数组中的下标
	fileNode *file = (fileNode*)malloc(sizeof(fileNode)*size);
	StoreToArray(file, t, -1,0);

	fwrite(file, sizeof(fileNode), size, fp);
	fclose(fp);

	free(file);
	FreeTree(t);
}



//将树通过先序顺序存入数组file中
//参数father即结点在原树中的father在数组中的下标，调用者设其为-1
//参数ths即当前结点在数组中的下标
//参数father和ths都是为了函数的递归实现
int StoreToArray(fileNode *file, Tree node,int father,int ths)
{
	if (node == NULL)
		return ths;

	file[ths].father = father;
	file[ths].IsFile = node->IsFile;
	strcpy_s(file[ths].name, NAMESIZE, node->name);

	Tree temp = node->son;
	//nextNode即下一个存储到数组的结点的下标，因为函数递归实现所以nextNode必不可少
	int nextNode = ths+1;
	while (temp != NULL)
	{
		nextNode = StoreToArray(file, temp, ths, nextNode);
		temp = temp->brother;
	}

	return nextNode;
}


//将存储在文件中的树加载进内存并重构为树
bool Load(Tree *t)
{
	FILE *fp;
	if (fopen_s(&fp, "FileSystem.xw", "rb"))
		return false;

	//获取文件中的size，以确定树有多少结点，并将文件中的树存入file数组中
	int size = 0;
	fread(&size, sizeof(int), 1, fp);
	fileNode *file = (fileNode*)malloc(sizeof(fileNode)*size);
	fread(file, sizeof(fileNode), size, fp);
	fclose(fp);

	//nodeInformation存储各结点存入内存后的位置
	Tree *nodeInformation = (Tree *)malloc(sizeof(Tree)*size);
	Tree temp = NULL;
	for (int i = 0;i < size;++i)
	{
		temp = (Tree)malloc(sizeof(treeNode));
		temp->IsFile = file[i].IsFile;
		strcpy_s(temp->name, NAMESIZE, file[i].name);
		temp->son = temp->brother = NULL;
		nodeInformation[i] = temp;
	}

	//根据nodeInformation存储的各结点位置，以及file中存储的各结点father下标，重构树
	int index = 0;
	for (int i = 1;i < size;++i)
	{
		index = file[i].father;
		if (nodeInformation[index]->son == NULL)
			nodeInformation[index]->son = nodeInformation[i];
		else
		{
			temp = nodeInformation[index]->son;
			while (temp->brother != NULL)
				temp = temp->brother;
			temp->brother = nodeInformation[i];
		}
	}

	*t = nodeInformation[0];
	
	free(file);
	free(nodeInformation);
	return true;
}


//删除树中的某结点，若该结点为文件夹类型，则其下所有文件都会被删除
void DeleteNode(Tree t)
{
	char deletePath[PATHSIZE];
	printf("请输入要删除的文件（夹）的路径，格式如此：root/xxx/xxx（注意，删除文件夹将导致其下所有文件删除）\n");
	scanf_s("%s", deletePath, PATHSIZE);
	strcat_s(deletePath, PATHSIZE, "/");

	int n = 0;  //n为inserPath中'/'的个数，即“结点个数”
	int i = 0;
	while (deletePath[i] != '\0')
		if (deletePath[i++] == '/')
			n++;


	Tree directory = FindDirectory(t, deletePath, 0, n - 2);
	char name[NAMESIZE] = "\0";
	getNodeName(name, deletePath, n - 1);
	if (directory == NULL)
	{
		printf("路径错误！");
		return;
	}

	
	Tree deleteNode_leftBrother = NULL;
	Tree deleteNode = directory->son;
	while (deleteNode != NULL)
	{
		if (!strcmp(deleteNode->name, name))
			break;
		else
		{
			deleteNode_leftBrother = deleteNode;
			deleteNode = deleteNode->brother;
		}
	}

	if (deleteNode == NULL)
	{
		printf("不存在该目标！\n");
		return;
	}
	if (deleteNode_leftBrother != NULL)
			deleteNode_leftBrother->brother = deleteNode->brother;
	else if (deleteNode->brother != NULL)
		directory->son = deleteNode->brother;
	else
		directory->son = NULL;

	FreeTree(deleteNode);
	printf("删除成功\n\n");
}

