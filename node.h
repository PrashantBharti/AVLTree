#ifndef __node_h__
#define __node_h__
/***********************************************/
namespace nodedef
{
/***********************************************/
using namespace keyvaluedef;
/***********************************************/
//NODE
//Handles a tree node
/***********************************************/
class NODE
{
public:
	static NODE* Create(int iKey, int iValue);
	static void Destroy(NODE** ptrnode);

private:
	NODE(void);
	~NODE(void);
	int Intialze(int iKey, int iValue);

public:
	int Height(void) const {return IS_NULL(this) ? FALSE : iHeight;}
	int Size(void) const {return IS_NULL(this) ? FALSE : iSize;}
	const KEYVALUE* KeyValue(void) const {return IS_NULL(this) ? nullptr : keyvalue;}
	const NODE* LeftTree(void) const {return IS_NULL(this) ? nullptr : nodeLeftTree;}
	NODE* LeftTree(void) {return IS_NULL(this) ? nullptr : nodeLeftTree;}
	const NODE* RightTree(void) const {return IS_NULL(this) ? nullptr : nodeRightTree;}
	NODE* RightTree(void) {return IS_NULL(this) ? nullptr : nodeRightTree;}
	NODE* Parent(void) const {return IS_NULL(this) ? nullptr : nodeParent;}
	bool IsLeaf(void) const {return IS_NULL(this) ? false : IS_NULL(nodeLeftTree) && IS_NULL(nodeRightTree);}
	bool IsLeftChild(void) const;
	bool IsRightChild(void) const;
	bool HasLeftTree(void) const {return IS_NULL(this) ? false : !IS_NULL(nodeLeftTree);}
	bool HasRightTree(void) const {return IS_NULL(this) ? false : !IS_NULL(nodeRightTree);}
	bool IsBalanced(void) const;

public:
	void ReCalculateHeight(void);
	void ReCalculateSize(void);
	void ReBalance(void);
	void SetValue(int iValue) {if(!IS_NULL(this)) keyvalue->SetValue(iValue);}
	void SetKey(int iKey) {if(!IS_NULL(this)) keyvalue->SetKey(iKey);}
	void SetLeftTree(NODE* nodeLeftChild) {if(!IS_NULL(this)) nodeLeftTree = nodeLeftChild;}
	void SetRightTree(NODE* nodeRightChild) {if(!IS_NULL(this)) nodeRightTree = nodeRightChild;}
	void SetParent(NODE* nodeParent_) {if(!IS_NULL(this)) nodeParent = nodeParent_;}

private:
	int iHeight;
	int iSize;
	NODE* nodeLeftTree;
	NODE* nodeRightTree;
	NODE* nodeParent;
	KEYVALUE* keyvalue;
};
/***********************************************/
}
#endif //__node_h__