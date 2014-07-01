#include "basicdef.h"
#include "keyvalue.h"
#include "node.h"
#include "avltree.h"
using namespace avltreedef;
/***********************************************/
AVLTREE::AVLTREE(void)
: nodeRoot(nullptr)
{
}
/***********************************************/
AVLTREE::~AVLTREE(void)
{
	NODE::Destroy(&nodeRoot);
}
/***********************************************/
/*static*/ AVLTREE* AVLTREE::Create(void)
{
	AVLTREE* avltree = new AVLTREE();
	int iRet = avltree->Intialize();
	if(!iRet)
		delete avltree, avltree = nullptr;
	return avltree;
}
/***********************************************/
/*static*/ void AVLTREE::Destroy(AVLTREE** ptravltree)
{
	delete *ptravltree;
	*ptravltree = nullptr;
}
/***********************************************/
int AVLTREE::Intialize(void)
{
	return IS_NULL(this) ? FALSE : TRUE;
}
/***********************************************/
int AVLTREE::Insert(int iKey, int iValue)
{
	if(IS_NULL(this))
		return FALSE;

	//Insert root if tree is empty
	if(IsEmpty())
	{
		nodeRoot = NODE::Create(iKey, iValue);
		return TRUE;
	}

	//Get parent node
	NODE* nodeParent = nodeRoot;
	while (!IS_NULL(nodeParent))
	{
		if(nodeParent->KeyValue()->Key() == iKey)
			return FALSE;

		if(nodeParent->KeyValue()->Key() > iKey && nodeParent->HasLeftTree())
			nodeParent = nodeParent->LeftTree();
		else if(nodeParent->KeyValue()->Key() < iKey && nodeParent->HasRightTree())
			nodeParent = nodeParent->RightTree();
		else
			break;
	}

	//Prepare child and insert to parent
	if(IS_NULL(nodeParent))
		return FALSE;
	NODE* nodeChild = NODE::Create(iKey, iValue);
	if(iKey < nodeParent->KeyValue()->Key())
		nodeParent->SetLeftTree(nodeChild);
	else if(iKey > nodeParent->KeyValue()->Key())
		nodeParent->SetRightTree(nodeChild);
	else
	{
		NODE::Destroy(&nodeChild);
		return FALSE;
	}

	//Now we have to calculate set new details for parent and balance all grand parents
	nodeChild->SetParent(nodeParent);
	nodeParent->ReCalculateSize();
	nodeParent->ReCalculateHeight();
	NODE* nodeGrandParent = nodeParent->Parent();
	while(!IS_NULL(nodeGrandParent))
	{
		//Calculate new size
		nodeGrandParent->ReCalculateSize();

		//Balance if required and calculate new height
		if(!nodeGrandParent->IsBalanced())
		{
			nodeGrandParent->ReBalance();
			nodeGrandParent = nodeGrandParent->Parent();
			if(IS_NULL(nodeGrandParent->Parent()))
				nodeRoot = nodeGrandParent;
		}
		nodeGrandParent->ReCalculateHeight();
		nodeGrandParent = nodeGrandParent->Parent();
	}

	return TRUE;
}
/***********************************************/
int AVLTREE::Delete(int iKey)
{
	if(IS_NULL(this))
		return FALSE;

	if(IsEmpty())
		return FALSE;

	NODE* nodeToDelete = SearchInternal(iKey);
	if(IS_NULL(nodeToDelete))
		return FALSE;

	while(!nodeToDelete->IsLeaf())
	{
		NODE* nodeNext = MinInternal(nodeToDelete->RightTree());
		if(IS_NULL(nodeNext))
			nodeNext = MaxInternal(nodeToDelete->LeftTree());

		nodeToDelete->SetKey(nodeNext->KeyValue()->Key());
		nodeToDelete->SetValue(nodeNext->KeyValue()->Value());

		nodeToDelete = nodeNext;
	}

	NODE* nodeParent = nodeToDelete->Parent();
	bool bIsLeftNode = nodeParent->LeftTree() == nodeToDelete;
	NODE::Destroy(&nodeToDelete);
	
	if(IS_NULL(nodeParent))
	{
		nodeRoot = nullptr;
		return TRUE;
	}

	if(bIsLeftNode)
		nodeParent->SetLeftTree(nullptr);
	else
		nodeParent->SetRightTree(nullptr);

	while(!IS_NULL(nodeParent))
	{
		nodeParent->ReCalculateSize();

		if(!nodeParent->IsBalanced())
		{
			nodeParent->ReBalance();
			nodeParent = nodeParent->Parent();
			if(IS_NULL(nodeParent->Parent()))
				nodeRoot = nodeParent;
		}
		nodeParent->ReCalculateHeight();
		nodeParent = nodeParent->Parent();
	}

	return TRUE;
}
/***********************************************/
NODE* AVLTREE::MinInternal(NODE* nodeRoot_) const
{
	while(nodeRoot_->HasLeftTree())
		nodeRoot_ = nodeRoot_->LeftTree();
	return nodeRoot_;
}
/***********************************************/
const KEYVALUE* AVLTREE::MinKeyValue(void) const
{
	if(IS_NULL(this) || IsEmpty())
		return nullptr;

	const NODE* node = MinInternal(nodeRoot);

	return node->KeyValue();
}
/***********************************************/
NODE* AVLTREE::MaxInternal(NODE* nodeRoot_) const
{

	while(nodeRoot_->HasRightTree())
		nodeRoot_ = nodeRoot_->RightTree();
	return nodeRoot_;
}
/***********************************************/
const KEYVALUE* AVLTREE::MaxKeyValue(void) const
{
	if(IS_NULL(this) || IsEmpty())
		return nullptr;

	const NODE* node = MaxInternal(nodeRoot);

	return node->KeyValue();
}
/***********************************************/
NODE* AVLTREE::SearchInternal(int iKey) const
{
	NODE* node = nodeRoot;
	while (!IS_NULL(node))
	{
		if(node->KeyValue()->Key() == iKey)
			break;
		else if(node->KeyValue()->Key() > iKey)
			node = node->LeftTree();
		else if(node->KeyValue()->Key() < iKey)
			node = node->RightTree();
	}

	return node;
}
/***********************************************/
const KEYVALUE* AVLTREE::Search(int iKey) const
{
	if(IS_NULL(this) || IsEmpty())
		return nullptr;

	const NODE* node = SearchInternal(iKey);

	return node->KeyValue();
}
/***********************************************/
const KEYVALUE* AVLTREE::Successor(int iKey) const
{
	if(IS_NULL(this) || IsEmpty())
		return nullptr;

	NODE* node = SearchInternal(iKey);
	const NODE* nodeMinRight = MinInternal(node->RightTree());

	return IS_NULL(nodeMinRight) && node->IsLeftChild() ? node->Parent()->KeyValue() : nullptr;
}
/***********************************************/
const KEYVALUE* AVLTREE::Predecessor(int iKey) const
{
	if(IS_NULL(this) || IsEmpty())
		return nullptr;

	NODE* node = SearchInternal(iKey);
	const NODE* nodeMaxRight = MaxInternal(node->LeftTree());

	return IS_NULL(nodeMaxRight) && node->IsRightChild() ? node->Parent()->KeyValue() : nullptr;
}
/***********************************************/
const NODE* AVLTREE::SelectInternal(const NODE* nodeRoot_, int iOrder) const
{
	if(nodeRoot_->Size() < iOrder)
		return nullptr;

	if(nodeRoot_->HasLeftTree() && nodeRoot_->LeftTree()->Size() >= iOrder)
		return SelectInternal(nodeRoot_->LeftTree(), iOrder);
	if(nodeRoot->LeftTree()->Size()+1 == iOrder)
		return nodeRoot_;
	if(nodeRoot->HasRightTree() && nodeRoot->RightTree()->Size() >= (iOrder - nodeRoot->LeftTree()->Size()-1))
		return SelectInternal(nodeRoot->RightTree(), iOrder - nodeRoot->LeftTree()->Size()-1);

	return nullptr;
}
/***********************************************/
const KEYVALUE* AVLTREE::Select(int iOrder) const
{
	if(IS_NULL(this) || iOrder < 1 || IsEmpty())
		return nullptr;

	const NODE* node = SelectInternal(nodeRoot, iOrder);

	return node->KeyValue();
}
/***********************************************/
int AVLTREE::InOrderDetails(const NODE* nodeRoot_, const KEYVALUE* arrPtrKeyValue[], size_t szSize, size_t szCurrentSize) const
{
	size_t szPreviousSize = szCurrentSize;
	if(nodeRoot_->HasLeftTree() && szCurrentSize < szSize)
		szCurrentSize += InOrderDetails(nodeRoot_->LeftTree(), arrPtrKeyValue, szSize, szCurrentSize);
	if(szCurrentSize < szSize)
		arrPtrKeyValue[szCurrentSize++] = nodeRoot_->KeyValue();
	if(nodeRoot_->HasRightTree() && szCurrentSize < szSize)
		szCurrentSize += InOrderDetails(nodeRoot_->RightTree(), arrPtrKeyValue, szSize, szCurrentSize);

	return szCurrentSize-szPreviousSize;
}
/***********************************************/
int AVLTREE::InOrderDetails(const KEYVALUE* arrPtrKeyValue[], size_t szSize) const
{
	if(IS_NULL(this) || IsEmpty() || IS_NULL(arrPtrKeyValue) || szSize == 0)
		return FALSE;

	int iRet = InOrderDetails(nodeRoot, arrPtrKeyValue, szSize, 0);

	return iRet ? TRUE : FALSE;
}
/***********************************************/
int AVLTREE::PreOrderDetails(const NODE* nodeRoot_, const KEYVALUE* arrPtrKeyValue[], size_t szSize, size_t szCurrentSize) const
{
	size_t szPreviousSize = szCurrentSize;
	if(szCurrentSize < szSize)
		arrPtrKeyValue[szCurrentSize++] = nodeRoot_->KeyValue();
	if(szCurrentSize < szSize && nodeRoot_->HasLeftTree())
		szCurrentSize += PreOrderDetails(nodeRoot_->LeftTree(), arrPtrKeyValue, szSize, szCurrentSize);
	if(szCurrentSize < szSize && nodeRoot_->HasRightTree())
		szCurrentSize += PreOrderDetails(nodeRoot_->RightTree(), arrPtrKeyValue, szSize, szCurrentSize);

	return szCurrentSize-szPreviousSize;
}
/***********************************************/
int AVLTREE::PreOrderDetails(const KEYVALUE* arrPtrKeyValue[], size_t szSize) const
{
	if(IS_NULL(this) || IsEmpty() || IS_NULL(arrPtrKeyValue) || szSize == 0)
		return FALSE;

	int iRet = PreOrderDetails(nodeRoot, arrPtrKeyValue, szSize, 0);

	return iRet ? TRUE : FALSE;
}
/***********************************************/
int AVLTREE::PostOrderDetails(const NODE* nodeRoot_, const KEYVALUE* arrPtrKeyValue[], size_t szSize, size_t szCurrentSize) const
{
	size_t szPreviousSize = szCurrentSize;
	if(szCurrentSize < szSize && nodeRoot_->HasLeftTree())
		szCurrentSize += PostOrderDetails(nodeRoot_->LeftTree(), arrPtrKeyValue, szSize, szCurrentSize);
	if(szCurrentSize < szSize && nodeRoot_->HasRightTree())
		szCurrentSize += PostOrderDetails(nodeRoot_->RightTree(), arrPtrKeyValue, szSize, szCurrentSize);
	if(szCurrentSize < szSize)
		arrPtrKeyValue[szCurrentSize++] = nodeRoot_->KeyValue();

	return szCurrentSize-szPreviousSize;
}
/***********************************************/
int AVLTREE::PostOrderDetails(const KEYVALUE* arrPtrKeyValue[], size_t szSize) const
{
	if(IS_NULL(this) || IsEmpty() || IS_NULL(arrPtrKeyValue) || szSize == 0)
		return FALSE;

	int iRet = PostOrderDetails(nodeRoot, arrPtrKeyValue, szSize, 0);

	return iRet ? TRUE : FALSE;
}
/***********************************************/