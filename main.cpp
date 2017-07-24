#include "XWtree.h"
#include <stdio.h>

int main()
{
	Tree fileSystem=NULL;
	Choice option;  //ChoicΪö�ٱ�������XWtree.h

	//Ĭ�ϴ��ļ��м���������ʧ��˵���ļ������ڣ����ʼ����
	if(!Load(&fileSystem))
		Initialize(&fileSystem);

	//���������һ��
	PrintPreOrder(fileSystem, 0);

	//���ϻ�ȡ��ִ���û���ѡ����
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
	
	//�˳�����ʱ�����洢���ļ���
	Store(fileSystem);
	
	return 0;
}