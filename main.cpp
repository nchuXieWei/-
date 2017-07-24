#include "XWtree.h"
#include <stdio.h>

int main()
{
	Tree fileSystem=NULL;
	Choice option;

	//Ĭ�ϴ��ļ��м���������ʧ��˵���ļ������ڣ����ʼ����
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