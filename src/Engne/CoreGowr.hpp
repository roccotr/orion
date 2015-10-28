#ifndef COREGOWR
#define COREGOWR
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePlan.hpp"
#include "CoreSdwk.hpp"
#include "CoreSdmq.hpp"
#include "CoreSdmg.hpp"
#include "PrtoSeda.pb.h"

namespace orion {

class CoreGowr : CoreSdwk {
	static const double dVtimeconst;

	PTR_PLANNER cVplanner;
	CoreSdmq<PrtoSeda>* cVmessagequeue;

public:
  __fastcall CoreGowr();  
	__fastcall ~CoreGowr ();

	void init (PTR_VOID cPlanner);
	void work_ () ;
	void setqq (void* cMessagequeue) {cVmessagequeue = (CoreSdmq<PrtoSeda>*) cMessagequeue;};
	static bool gossr (PTR_GOSSIPER cGossiper, CoreEnpo::GOSSIPLOC* cGossiploc);
};

};

#endif