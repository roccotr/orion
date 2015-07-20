#ifndef CORECCHE
#define CORECCHE
#include "CoreVcl_.hpp"

using namespace orion;


template<typename T, int iVcapacity>
class CoreCche
{
private:
  TList cVl;

	typedef struct CoreChlc {
		bool bVvalid;
		T cVloc;
		CoreChlc* cVprev;
		CoreChlc* cVnext;

		/*INITIALIZE*/
		CoreChlc () {
			bVvalid = false;
			cVprev = NULL;
			cVnext = NULL;
		}
	} CACHELOC;

	CACHELOC* cVfirst;
	CACHELOC* cVlast;
public:
  __fastcall CoreCche ();
  __fastcall ~CoreCche ();
  void __fastcall set__ (T cElement);
  bool __fastcall get__ (AnsiString* sKey, T& cReturn);
  void __fastcall clear ();
  AnsiString __fastcall debug ();
};


#endif