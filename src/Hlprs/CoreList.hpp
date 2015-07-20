#ifndef CORELIST
#define CORELIST
#include "CoreVcl_.hpp"
#include "CoreSmph.hpp"

namespace orion {

template<typename T>
class CoreList
{
private:
	TList cVl;
	CoreSmph cVmutex;

public:
  __fastcall CoreList ();
  __fastcall ~CoreList ();
	T* __fastcall set__ (int iPosition = -1, bool bInsert = true);
	T* __fastcall bnset (AnsiString sKey);
	T* __fastcall get__ (int iPosition);
	T* __fastcall bnget (AnsiString sKey);
	bool __fastcall bnuns (AnsiString sKey);
	bool __fastcall unset (int iPosition);
	int __fastcall count ();
	void __fastcall clear ();
	int __fastcall bnsrc (AnsiString sKey);
	int __fastcall bnsrc (AnsiString* sKey, int& iComparison);

	void __fastcall allcv (int iCapacity);
	void __fastcall split (CoreList* cDestination, int iBegin);
	AnsiString __fastcall debug ();
};

};


#endif