#ifndef CORERMWR
#define CORERMWR
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePlan.hpp"
#include "CoreSdwk.hpp"
#include "CoreSdmq.hpp"
#include "CoreSdmg.hpp"
#include "PrtoSeda.pb.h"

namespace orion {

class CoreRmwr : CoreSdwk {
	PTR_PLANNER cVplanner;
	CoreSdmq<PrtoSeda>* cVmessagequeue;
	
	bool sendt (void* cGossipelement, void* cMutations);

public:
  __fastcall CoreRmwr();  
	__fastcall ~CoreRmwr ();

	void init (PTR_VOID cPlanner);
	void work_ () ;
	void setqq (void* cMessagequeue) {cVmessagequeue = (CoreSdmq<PrtoSeda>*) cMessagequeue;};
};

};

#endif