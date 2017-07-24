#ifndef XWTREE
#define XWTREE

//两个常用宏，分别用于文件（夹）名最大长度、路径最大长度
#define NAMESIZE 50
#define PATHSIZE 2000

//在内存中的树的结点定义
typedef struct treeNode{
	bool IsFile;  
	char name[NAMESIZE];  
	struct treeNode *son; 
	struct treeNode *brother;
}treeNode;
typedef treeNode* Tree;

//将树存入文件时的结点定义
typedef struct fileNode {
	bool IsFile;
	char name[NAMESIZE];
	size_t father;
}fileNode;






//用于判断用户所选操作的枚举类型
typedef enum Choice{
	INSERT,PREORDER,POSTORDER,QUIT
}Choice;

//main中会使用到的函数
bool Initialize(Tree *t); //第一次使用程序时需要的初始化函数
Choice GetChoice();    //获取并返回用户所选操作的函数
void Store(Tree t);    //程序结束时将树t存入文件的函数
void PrintPreOrder(Tree t, int layer);    //先序遍历树
void PrintPostOrder(Tree t, int layer);   //后序遍历树
bool Load(Tree *t);    //将树从文件中加载至内存
void Insert(Tree t);  //在树中新增结点
void Delete(Tree t); //删除树中的某结点（若该结点为文件夹，则删除其下所有文件）


#endif