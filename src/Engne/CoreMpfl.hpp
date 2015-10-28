#ifndef COREMPFL
#define COREMPFL
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePlan.hpp"
#include "CoreSdwk.hpp"
#include "CoreSdmq.hpp"
#include "CoreSdmg.hpp"
#include "PrtoSeda.pb.h"

namespace orion {

class CoreMpfl : CoreSdwk {
	PTR_PLANNER cVplanner;
	CoreSdmq<PrtoSeda>* cVmessagequeue;
	
public:
  __fastcall CoreMpfl ();  
	__fastcall ~CoreMpfl ();

	void init (PTR_VOID cPlanner);
	void work_ () ;
	void setqq (void* cMessagequeue) {cVmessagequeue = (CoreSdmq<PrtoSeda>*) cMessagequeue;};
};

};

#endif