#ifndef COREFLQU
#define COREFLQU
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
extern "C" {
 #include "liblfds611.h"
}

using namespace orion;

template<typename T, int iVdim = 10000>
class CoreFlqu {

  const static long int iCwaitprecision;

	TEvent cVeventfull;
	TEvent cVeventempty;
	struct lfds611_queue_state* cVqs;
	bool bVterminated;

	static void delte (void* cUserdata, void* cDummy);

public:
  __fastcall CoreFlqu	();
  __fastcall ~CoreFlqu ();

	bool __fastcall push_ (T* cElement);
	bool __fastcall pop__ (T** cElement);
	void __fastcall abort ();

	bool __fastcall wait_ (int iWatchdogms);
	void __fastcall signl ();
};


#endif
