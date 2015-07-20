#ifndef CORERLSW
#define CORERLSW
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePlan.hpp"
#include "CoreSdwk.hpp"
#include "CoreSdmq.hpp"
#include "CoreSdmg.hpp"
#include "PrtoSeda.pb.h"

namespace orion {

class CoreRlsw : CoreSdwk {
	PTR_PLANNER cVplanner;
	CoreSdmq<PrtoSeda>* cVmessagequeue;
	
public:
  __fastcall CoreRlsw();  
	__fastcall ~CoreRlsw ();

	void init (PTR_VOID cPlanner);
	void work_ () ;
	void setqq (void* cMessagequeue) {cVmessagequeue = (CoreSdmq<PrtoSeda>*) cMessagequeue;};
};

};

#endif