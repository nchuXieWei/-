#include "XWtree.h"
#include <stdio.h>

int main()
{
	Tree fileSystem=NULL;
	Choice option;  //Choic为枚举变量，见XWtree.h

	//默认从文件中加载树，若失败说明文件不存在，则初始化树
	if(!Load(&fileSystem))
		Initialize(&fileSystem);

	//先序输出树一次
	PrintPreOrder(fileSystem, 0);

	//不断获取、执行用户所选操作
	while((option=GetChoice())!=QUIT)
	{
		switch(option)
		{
			case INSERT:Insert(fileSystem);break;
			case DELETE:DeleteNode(fileSystem);break;
			case PREORDER:PrintPreOrder(fileSystem,0);break;
			case POSTORDER:PrintPostOrder(fileSystem,0);break;
		}
	}
	
	//退出程序时将树存储至文件中
	Store(fileSystem);
	
	return 0;
}