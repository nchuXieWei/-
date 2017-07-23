#include "XWtree.h"
#include <stdio.h>

int main()
{
	
	Tree t=NULL;
	Choice temp;

	
	
	if(!Load(&t))
		Initialize(&t);

	PrintPreOrder(t, 0);

	while((temp=GetChoice())!=QUIT)
	{
		switch(temp)
		{
			case INSERT:Insert(t);break;
			case PREORDER:PrintPreOrder(t,0);break;
			case POSTORDER:PrintPostOrder(t,0);break;
		}
	}
	Store(t);
	
	
	return 0;
}