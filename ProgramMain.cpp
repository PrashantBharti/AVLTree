#include <cstddef>
#include "basicdef.h"
#include "keyvalue.h"
#include "node.h"
#include "avltree.h"
using namespace avltreedef;
/************************************************/
int main(int argc, char *argv[])
{
	const KEYVALUE* arrPtrKeyValue[10];

	AVLTREE* avltree = AVLTREE::Create();
	avltree->Insert(7, 7);
	avltree->Insert(2, 2);
	avltree->Insert(4, 4);
	avltree->Insert(6, 6);
	avltree->InOrderDetails(arrPtrKeyValue, sizeof(arrPtrKeyValue)/sizeof(arrPtrKeyValue[0]));
	avltree->Delete(4);
	avltree->InOrderDetails(arrPtrKeyValue, sizeof(arrPtrKeyValue)/sizeof(arrPtrKeyValue[0]));
	AVLTREE::Destroy(&avltree);
	return 1;
}
/************************************************/