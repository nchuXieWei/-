#include "XWtree.h"
#include <stdio.h>

int main()
{
	Tree fileSystem=NULL;
	Choice option;

	//默认从文件中加载树，若失败说明文件不存在，则初始化树
	if(!Load(&fileSystem))
		Initialize(&fileSystem);

	PrintPreOrder(fileSystem, 0);

	while((option=GetChoice())!=QUIT)
	{
		switch(option)
		{
			case INSERT:Insert(fileSystem);break;
			case PREORDER:PrintPreOrder(fileSystem,0);break;
			case POSTORDER:PrintPostOrder(fileSystem,0);break;
		}
	}
	Store(fileSystem);
	
	return 0;
}