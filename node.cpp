#include <math.h>
#include "basicdef.h"
#include "keyvalue.h"
#include "node.h"
using namespace nodedef;
/***********************************************/
NODE::NODE(void)
: iHeight(0)
, iSize(0)
, keyvalue(nullptr)
, nodeParent(nullptr)
, nodeLeftTree(nullptr)
, nodeRightTree(nullptr)
{
}
/***********************************************/
NODE::~NODE(void)
{
	delete nodeLeftTree;
	delete nodeRightTree;
	KEYVALUE::Destroy(&keyvalue);
}
/***********************************************/
/*static*/ NODE* NODE::Create(int iKey, int iValue)
{
	NODE* node = new NODE();
	int iRet = node->Intialze(iKey, iValue);
	if(!iRet)
		delete node, node = nullptr;
	return node;
}
/***********************************************/
/*static*/ void NODE::Destroy(NODE** ptrnode)
{
	delete *ptrnode;
	*ptrnode = nullptr;
}
/***********************************************/
int NODE::Intialze(int iKey, int iValue)
{
	if(IS_NULL(this))
		return FALSE;

	iHeight = 1;
	iSize = 1;
	keyvalue = keyvaluedef::KEYVALUE::Create(iKey, iValue);
	
	return IS_NULL(keyvalue) ? FALSE : TRUE;
}
/***********************************************/
bool NODE::IsLeftChild(void) const
{
	if(IS_NULL(this))
		return false;

	return nodeParent->LeftTree() == this;
}
/***********************************************/
bool NODE::IsRightChild(void) const
{
	if(IS_NULL(this))
		return false;

	return nodeParent->RightTree() == this;
}
/***********************************************/
bool NODE::IsBalanced(void) const
{
	if(IS_NULL(this))
		return false;

	return abs(nodeLeftTree->Height()-nodeRightTree->Height()) <= 1 ? true : false;
}
/***********************************************/
void NODE::ReCalculateHeight(void)
{
	if(!IS_NULL(this))
	{
		int iHeightMax = nodeLeftTree->Height() > nodeRightTree->Height() ? nodeLeftTree->Height() : nodeRightTree->Height();
		iHeight = iHeightMax+1;
	}
}
/***********************************************/
void NODE::ReCalculateSize(void)
{
	if(!IS_NULL(this))
		iSize = nodeLeftTree->Size() + nodeRightTree->Size() + 1;
}
/***********************************************/
void NODE::ReBalance(void)
{
	if(IS_NULL(this) || IsBalanced())
		return;

	if(nodeLeftTree->Height() > nodeRightTree->Height()+1)
	{
		NODE* nodeLeftSubTree = nodeLeftTree;
		if(nodeLeftSubTree->LeftTree()->Height() > nodeLeftSubTree->RightTree()->Height())
		{
			//Single Rotation
			nodeLeftTree->SetParent(this->Parent());
			this->SetParent(nodeLeftSubTree);
			nodeLeftTree = nodeLeftSubTree->RightTree();
			nodeLeftSubTree->SetRightTree(this);

			this->ReCalculateHeight();
			this->ReCalculateSize();
			nodeLeftSubTree->ReCalculateHeight();
			nodeLeftSubTree->ReCalculateSize();
		}
		else if(nodeLeftSubTree->LeftTree()->Height() < nodeLeftSubTree->RightTree()->Height())
		{
			//Double Rotation
			NODE* nodeLeftSubRightTree = nodeLeftSubTree->RightTree();

			nodeLeftSubRightTree->SetParent(nodeLeftSubTree->Parent());
			nodeLeftSubTree->SetParent(nodeLeftSubRightTree);
			nodeLeftSubTree->SetRightTree(nodeLeftSubRightTree->LeftTree());
			nodeLeftSubRightTree->SetLeftTree(nodeLeftSubTree);

			nodeLeftSubRightTree->SetParent(this->Parent());
			this->SetParent(nodeLeftSubRightTree);
			nodeLeftTree = nodeLeftSubRightTree->RightTree();
			nodeLeftSubRightTree->SetRightTree(this);

			this->ReCalculateHeight();
			this->ReCalculateSize();
			nodeLeftSubTree->ReCalculateHeight();
			nodeLeftSubTree->ReCalculateSize();
			nodeLeftSubRightTree->ReCalculateHeight();
			nodeLeftSubRightTree->ReCalculateSize();
		}
	}
	else if(nodeRightTree->Height() > nodeLeftTree->Height()+1)
	{
		NODE* nodeRightSubTree = nodeRightTree;
		if(nodeRightSubTree->RightTree()->Height() > nodeRightSubTree->LeftTree()->Height())
		{
			//Single Rotation
			nodeRightSubTree->SetParent(this->Parent());
			this->SetParent(nodeRightSubTree);
			nodeRightTree = nodeRightSubTree->LeftTree();
			nodeRightSubTree->SetLeftTree(this);

			this->ReCalculateHeight();
			this->ReCalculateSize();
			nodeRightSubTree->ReCalculateHeight();
			nodeRightSubTree->ReCalculateSize();
		}
		else if(nodeRightSubTree->RightTree()->Height() < nodeRightSubTree->LeftTree()->Height())
		{
			//Double Rotation
			NODE* nodeRightSubLeftTree = nodeRightSubTree->LeftTree();

			nodeRightSubLeftTree->SetParent(nodeRightSubTree->Parent());
			nodeRightSubTree->SetParent(nodeRightSubLeftTree);
			nodeRightSubTree->SetLeftTree(nodeRightSubLeftTree->RightTree());
			nodeRightSubLeftTree->SetRightTree(nodeRightSubTree);

			nodeRightSubLeftTree->SetParent(this->Parent());
			this->SetParent(nodeRightSubLeftTree);
			nodeRightTree = nodeRightSubLeftTree->LeftTree();
			nodeRightSubLeftTree->SetLeftTree(this);

			this->ReCalculateHeight();
			this->ReCalculateSize();
			nodeRightSubTree->ReCalculateHeight();
			nodeRightSubTree->ReCalculateSize();
			nodeRightSubLeftTree->ReCalculateHeight();
			nodeRightSubLeftTree->ReCalculateSize();
		}
	}
	else
		return;
}
/***********************************************/