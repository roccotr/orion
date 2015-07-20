#ifndef CORESDEL
#define CORESDEL
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreSdmq.hpp"
#include "CoreSdwk.hpp"
#include "CoreSmph.hpp"

namespace orion {

template<typename T>
class CoreSdel : TThread {

	const static int iCidlems;

	AnsiString sVname;
	int iVpollingtimems;
	bool bVforceworker;
	bool bVrunning;
	CoreSmph cVmutex;
	CoreSdmq<T>* cVmessagequeue;
	PTR_SEDAWORKER cVworker;

public:
  __fastcall CoreSdel(	CoreSdmq<T>* cMessagequeue, AnsiString sName, int iPollingtimems, 
												PTR_SEDAWORKER cWorker, bool bForceworker = false);
  __fastcall ~CoreSdel ();

  void Terminate ();
  void Execute ();
	void start ();
	void stop_ ();
};

};

#endif
