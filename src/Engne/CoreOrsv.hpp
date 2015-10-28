#ifndef COREORSV
#define COREORSV
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreThth.hpp"
#include "CoreSckt.hpp"
#include "CoreList.hpp"

namespace orion {

class CoreOrsv : TThread {

	typedef struct CoreMetl {
		PTR_SERVERWORKER cVserverworker;

	} SERVERWORKERLOC;

	boost::shared_ptr<CoreSckt> cVsocket;
	CoreList<SERVERWORKERLOC> cVserverworkers;
	bool bVaborted;
	bool bVsharedmemory;
	int iVconnectioncounter;
	void* cVthriftserver;
	PTR_POOL cVpool;
	AnsiString sVsharedmemoryprefix;

public:
  __fastcall CoreOrsv (	void* cThriftserver, AnsiString sHost, int iPort, int iListeners, 
												AnsiString sSharedmemoryprefix);
  __fastcall ~CoreOrsv ();

	void __fastcall Execute ();
	void __fastcall abort (); 
	bool __fastcall isabr ();

};

};

#endif