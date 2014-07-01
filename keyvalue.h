#ifndef __key_value_h__
#define __key_value_h__
/***********************************************/
namespace keyvaluedef
{
/***********************************************/
//KEYVALUE
//Stores a key <-> value pair 
/***********************************************/
class KEYVALUE
{
public:
	static KEYVALUE* Create(int iKey_, int iValue_);
	static void Destroy(KEYVALUE** ptrkeyvalue);

private:
	KEYVALUE(void);
	~KEYVALUE(void);
	int Intialize(int iKey_, int iValue_);

public:
	int Key(void) const {return IS_NULL(this) ? FALSE : iKey;}
	int Value(void) const {return IS_NULL(this) ? FALSE : iValue;}

public:
	void SetValue(int iValue_) {if(!IS_NULL(this)) iValue = iValue_;}
	void SetKey(int iKey_) {if(!IS_NULL(this)) iKey = iKey_;}

private:
	int iKey;
	int iValue;
};
/***********************************************/
}
/***********************************************/
#endif // __key_value_h__