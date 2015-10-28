#ifndef COREAEWK
#define COREAEWK
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePlan.hpp"
#include "CoreSdwk.hpp"
#include "CoreSdmq.hpp"
#include "CoreSdmg.hpp"
#include "PrtoSeda.pb.h"

namespace orion{

class CoreAewk : CoreSdwk {
	CorePlan* cVplanner;
	CoreSdmg* cVsedamanager;
	CoreSdmq<PrtoSeda>* cVmessagequeue;
	
public:
  __fastcall CoreAewk();  
	__fastcall ~CoreAewk ();

	void init (void* cPlanner, void* cSedamanager);
	void work_ () ;
	void setqq (void* cMessagequeue) {cVmessagequeue = (CoreSdmq<PrtoSeda>*) cMessagequeue;};

	static bool ppllm (	CoreEnpo::GOSSIPLOC* cGossipelement, 
											PrtoMktr* cPrtomerkeltreesource, PrtoMktr* cPrtomerkeltreedestination,
											AnsiString sSendernodeid, PrtoLmtb* cMutable,
											AnsiString sKeystart, int iLimitsize);
	static bool ppllr (	CoreEnpo::GOSSIPLOC* cGossipelement, PrtoMktr* cPrtomerkeltreesource, 
											PrtoLmtb* cMutable);
};

};

#endif