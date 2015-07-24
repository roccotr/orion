#ifndef CORESDMG
#define CORESDMG
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreSdmq.hpp"
#include "CoreSdwk.hpp"
#include "CoreSdel.hpp"
#include "CoreSmph.hpp"
#include "PrtoSeda.pb.h"

namespace orion {

using namespace com::tomting::orion; 

class CoreSdmg {
	const static int iCidlems;

	typedef boost::shared_ptr<CoreSdel<PrtoSeda> > PTR_SEDAELEMENT;

	typedef struct CoreSdwl {
		AnsiString sVindex;

		PTR_SEDAWORKER cVworker;
		PTR_SEDAELEMENT cVsedaelement;

	} SEDAWORKERLOC;

	typedef struct CoreSdil {
		bool bVvalid;

		AnsiString sVindex;
		CoreSdmq<PrtoSeda> cVmessagequeue;
		CoreList<SEDAWORKERLOC> cVworkers;	

		CoreSdil () {
			bVvalid = true;
		}
	} SEDALOC;

	bool bVactive;
	PTR_VOID cVplanner;
	CoreSmph cVsedastagesmutex;
	CoreSmph cVsedaqueuesmutex;
	CoreList<SEDALOC> cVsedastages;
	CoreList<SEDALOC> cVsedaqueues;

public:

  __fastcall CoreSdmg();
  __fastcall ~CoreSdmg ();

	void __fastcall init_ (PTR_VOID cPlanner);
	
	template<class T>
	void __fastcall init_ (AnsiString sName, int iPollingtimems, int iWorkers, bool bForceworker = false);

	bool __fastcall start ();
	bool __fastcall stop_ ();
	bool __fastcall pushs (AnsiString sName, PrtoSeda& cQueuemessage);

	bool __fastcall newqu (AnsiString sName);
	bool __fastcall delqu (AnsiString sName);
	bool __fastcall pshqu (AnsiString sName, PrtoSeda& cQueuemessage);
	bool __fastcall popqu (AnsiString sName, PrtoSeda& cQueuemessage, int iWatchdogms);

};

typedef boost::shared_ptr<CoreSdmg> PTR_SEDAMANAGER;

};

#endif