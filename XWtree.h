#include <stdio.h>
#ifndef XWTREE
#define XWTREE
#define NAMESIZE 50
#define PATHSIZE 2000


typedef struct treeNode{
	bool IsFile;  
	char name[NAMESIZE];  
	struct treeNode *son; 
	struct treeNode *brother;
}treeNode;

typedef struct fileNode {
	bool IsFile;
	char name[NAMESIZE];
	size_t father;
}fileNode;

typedef treeNode* Tree;

typedef enum Choice{
	INSERT,PREORDER,POSTORDER,QUIT
}Choice;



bool Load(Tree *t);
int StoreToArray(fileNode *a, Tree t,int father,int ths);
int Count(Tree t);
void Store(Tree t);
bool Initialize(Tree *t);
Choice GetChoice();
void Insert(Tree t);
void PrintPreOrder(Tree t,int layer);
void PrintPostOrder(Tree t,int layer);
void FreeTree(Tree t);
Tree FindDirectory(Tree t,const char *insertPath,int layer,int total);
void getNodeName(char *des,const char *source,int layer);
void FreeTree(Tree t);

#endif