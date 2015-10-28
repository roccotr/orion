#ifndef CORERRWR
#define CORERRWR
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePlan.hpp"
#include "CoreSdwk.hpp"
#include "CoreSdmq.hpp"
#include "CoreSdmg.hpp"
#include "PrtoSeda.pb.h"

namespace orion {

class CoreRrwr : CoreSdwk {

	static const int iCtimeoutsedacallbacksms;

	typedef struct CoreQplc {
		AnsiString sVindex;

		int iVpointerloc;
	} QRESPOINTERLOC;

	typedef struct CoreGelc {
		AnsiString sVindex;

		int iVpointerloc;
	} GOELPOINTERLOC;

	PTR_PLANNER cVplanner;
	CoreSdmq<PrtoSeda>* cVmessagequeue;
	

public:
  __fastcall CoreRrwr();  
	__fastcall ~CoreRrwr ();

	void init (PTR_VOID cPlanner);
	void work_ () ;
	void setqq (void* cMessagequeue) {cVmessagequeue = (CoreSdmq<PrtoSeda>*) cMessagequeue;};
	static PrtoQres* getrs (RepeatedPtrField<PrtoQres>* cQueryresult, bool& bFullmatch);
	static bool getrs (RepeatedPtrField<PrtoQres>* cQueryresult, PrtoLkey* cKey, PrtoLval* cValue);
	static bool rrepr (	PTR_SEDAMANAGER cSedamanager, PTR_GOSSIPER cGossiper,
											PrtoSeda* cQueryresultandrepair, PrtoLval* cValue, int iTimeoutsedacallbacksms);
};

};

#endif