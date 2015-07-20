#ifndef CORELISS
#define CORELISS
#include <boost/shared_ptr.hpp>
#include "CoreVcl_.hpp"
#include "CoreSmph.hpp"

using namespace orion;

template<typename T, typename C>
class CoreLiss {
private:
	TList cVl;
	CoreSmph cVmutex;

	typedef struct CoreLilc {

		T cVloc;
		CoreLilc () {
			cVloc = T (new C);
		}
	} LOC;

public:
  __fastcall CoreLiss ();
  __fastcall ~CoreLiss ();
	T __fastcall set__ (int iPosition = -1, bool bInsert = true);
	T __fastcall bnset (AnsiString sKey);
	T __fastcall get__ (int iPosition);
	T __fastcall bnget (AnsiString sKey);
	bool __fastcall bnuns (AnsiString sKey);
	void __fastcall unset (int iPosition);
	int __fastcall count ();
	void __fastcall clear ();
	void __fastcall allcv (int iCapacity);
	int __fastcall bnsrc (AnsiString sKey);

	AnsiString __fastcall debug ();
};


#endif