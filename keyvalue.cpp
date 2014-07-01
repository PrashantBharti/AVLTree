#include "basicdef.h"
#include "keyvalue.h"
using namespace keyvaluedef;
/***********************************************/
KEYVALUE::KEYVALUE(void)
: iKey(0)
, iValue(0)
{
}
/***********************************************/
KEYVALUE::~KEYVALUE(void)
{
}
/***********************************************/
/*static*/ KEYVALUE* KEYVALUE::Create(int iKey_, int iValue_)
{
	KEYVALUE* keyvalue = new KEYVALUE();
	int iRet = keyvalue->Intialize(iKey_, iValue_);
	if(!iRet)
		delete keyvalue, keyvalue = nullptr;
	return keyvalue;
}
/***********************************************/
/*static*/ void KEYVALUE::Destroy(KEYVALUE** ptrkeyvalue)
{
	delete *ptrkeyvalue;
	*ptrkeyvalue = nullptr;
}
/***********************************************/
int KEYVALUE::Intialize(int iKey_, int iValue_)
{
	if(IS_NULL(this))
		return FALSE;

	iKey = iKey_;
	iValue = iValue_;

	return TRUE;
}
/***********************************************/
