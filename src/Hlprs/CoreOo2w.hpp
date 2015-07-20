#ifndef COREOO2W
#define COREOO2W
#include "CoreOo2t.hpp"
#include "CoreChls.hpp" 
#include "CoreClps.hpp" 


namespace orion {

template<typename T, int iVleafdim = 32>
class CoreOo2w {

public:
	 
	typedef CoreChls<T> OO2LEAFLOC;
	typedef CoreClps<T> OO2POS;

private:

	OO2LEAFLOC cVfirst;
	OO2LEAFLOC cVlast;	
	CoreOo2t<OO2LEAFLOC> cVindextree;
	CoreSmph cVmutex;
	int iVcount;

	void __fastcall init ();
	OO2LEAFLOC* __fastcall insrt (AnsiString sKey);
	T* __fastcall bnset (OO2LEAFLOC* cLeaf, AnsiString* sKey);
	bool __fastcall bnuns (OO2LEAFLOC* cLeaf, AnsiString* sKey, bool bDeletable, AnsiString* sKeydeleterb);
	void __fastcall satur (CoreList<T>* cList);
	void __fastcall under (OO2LEAFLOC* cLeaf, bool bDeletable, AnsiString* sKeydeleterb);

public:
	
	__fastcall CoreOo2w ();
  __fastcall ~CoreOo2w ();

	T* __fastcall bnset (AnsiString sKey); 
	T* __fastcall bnget (AnsiString sKey);
	OO2POS __fastcall bnsrc (AnsiString* sKey);
	OO2POS __fastcall bnsrc (AnsiString* sKey, int& iCompare);
	OO2POS __fastcall bnsdl (AnsiString* sKey, bool& bDelete, AnsiString& sDelete);
	T* __fastcall get__ (OO2POS cPosition);
	T* __fastcall set__ (OO2POS cPosition = -1, bool bInsert = true, AnsiString* sOrderkey = NULL);
	bool __fastcall bnuns (AnsiString sKey);
	bool __fastcall unset (OO2POS cPosition, bool bDeletable = false, AnsiString* sKeydeleterb = NULL);
	int __fastcall count ();
	void __fastcall clear ();

	OO2POS __fastcall first ();
	OO2POS __fastcall last_ ();

	int __fastcall lncnt ();
	OO2POS __fastcall ln2ps (int iLinearizedelement);

	AnsiString __fastcall prnta (bool bReadableleaves = true);

};

};


#endif