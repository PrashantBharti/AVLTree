#ifndef __avl_tree_h__
#define __avl_tree_h__
/***********************************************/
namespace avltreedef
{
/***********************************************/
using namespace keyvaluedef;
using namespace nodedef;
/***********************************************/
class AVLTREE
{
public:
	static AVLTREE* Create(void);
	static void Destroy(AVLTREE** ptravltree);

private:
	AVLTREE(void);
	~AVLTREE(void);
	int Intialize(void);

public:
	int Insert(int iKey, int iValue);
	int Delete(int iKey);

public:
	bool IsEmpty(void) const {return IS_NULL(this) ? false : IS_NULL(nodeRoot);}
	int Size(void) const {return IS_NULL(this) ? FALSE : nodeRoot->Size();}
	const KEYVALUE* MaxKeyValue(void) const;
	const KEYVALUE* MinKeyValue(void) const;
	const KEYVALUE* Successor(int iKey) const;
	const KEYVALUE* Predecessor(int iKey) const;
	const KEYVALUE* Select(int iOrder) const;
	const KEYVALUE* Search(int iKey) const;

public:
	int InOrderDetails(const KEYVALUE* arrPtrKeyValue[], size_t szSize) const;
	int PreOrderDetails(const KEYVALUE* arrPtrKeyValue[], size_t szSize) const;
	int PostOrderDetails(const KEYVALUE* arrPtrKeyValue[], size_t szSize) const;

private:
	NODE* SearchInternal(int iKey) const;
	NODE* MaxInternal(NODE* nodeRoot_) const;
	NODE* MinInternal(NODE* nodeRoot_) const;
	const NODE* SelectInternal(const NODE* nodeRoot_, int iOrder) const;
	int InOrderDetails(const NODE* nodeRoot_, const KEYVALUE* arrPtrKeyValue[], size_t szSize, size_t szCurrentSize) const;
	int PreOrderDetails(const NODE* nodeRoot_, const KEYVALUE* arrPtrKeyValue[], size_t szSize, size_t szCurrentSize) const;
	int PostOrderDetails(const NODE* nodeRoot_, const KEYVALUE* arrPtrKeyValue[], size_t szSize, size_t szCurrentSize) const;

private:
	NODE* nodeRoot;
};
/***********************************************/
}
/***********************************************/
#endif //__avl_tree_h__