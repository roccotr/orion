#ifndef CORELSTC
#define CORELSTC
#include "CoreList.hpp"
#include "CoreCche.hpp"

using namespace orion;

template<typename T, int iVcachecapacity>
class CoreLstc : public CoreList<T> {
private:
	CoreCche<T*, iVcachecapacity> cVcache;

public:
  __fastcall CoreLstc ();
  __fastcall ~CoreLstc ();

	T* __fastcall set__ (int iPosition = -1, bool bInsert = true);
	T* __fastcall bnset (AnsiString sKey);
	T* __fastcall bnget (AnsiString sKey);
	bool __fastcall bnuns (AnsiString sKey);
	bool __fastcall unset (int iPosition);
	void __fastcall clear ();

	AnsiString __fastcall debug ();


};


#endif