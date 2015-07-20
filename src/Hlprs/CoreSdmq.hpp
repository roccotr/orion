#ifndef CORESDMQ
#define CORESDMQ
#include "CoreVcl_.hpp"
#include "CoreSmph.hpp"
#include "CoreList.hpp"

namespace orion {

template<typename T>
class CoreSdmq {
	AnsiString sVname;
	CoreSmph cVmutex;
	CoreList<T> cVqueue;
	TEvent* cVevent;

public:
  __fastcall CoreSdmq();
  __fastcall ~CoreSdmq ();

	void __fastcall init (AnsiString sName);

	bool __fastcall push_ (const T& cMessage);
	bool __fastcall pop__ (T& cMessage);
	int __fastcall count ();

	bool __fastcall wait_ (int iWatchdogms);
	void __fastcall signl ();

};

};

#endif