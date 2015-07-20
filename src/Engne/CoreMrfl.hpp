#ifndef COREMRFL
#define COREMRFL
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePlan.hpp"
#include "CoreSdwk.hpp"
#include "CoreSdmq.hpp"
#include "CoreSdmg.hpp"
#include "PrtoSeda.pb.h"

namespace orion {

class CoreMrfl : CoreSdwk {
	PTR_PLANNER cVplanner;
	CoreSdmq<PrtoSeda>* cVmessagequeue;
	
public:
  __fastcall CoreMrfl ();  
	__fastcall ~CoreMrfl ();

	void init (PTR_VOID cPlanner);
	void work_ () ;
	void setqq (void* cMessagequeue) {cVmessagequeue = (CoreSdmq<PrtoSeda>*) cMessagequeue;};
};

};

#endif