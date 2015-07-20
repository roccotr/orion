#include "CoreMrfl.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

//#define iCdisablepartialflusher

  __fastcall CoreMrfl::CoreMrfl() : CoreSdwk () {
		
	}

  __fastcall CoreMrfl::~CoreMrfl () {

	}

	/*INIT*/
	void __fastcall CoreMrfl::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

  /*WORK*/
	void CoreMrfl::work_ () {
		PrtoSeda cVmessage;
		PrtoLmtb cVmutable;

		while (cVmessagequeue->pop__ (cVmessage)) {
			cVmutable.set_svnamespace (cVmessage.svnamespace ());
			cVmutable.set_svtable (cVmessage.svtable ());	

#ifndef iCdisablepartialflusher
			
			cVplanner->compa (&cVmutable, CoreTbsv::iCminor);
#endif

		}	
	}