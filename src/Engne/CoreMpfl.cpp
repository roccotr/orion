#include "CoreMpfl.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

//#define iCdisablepostflusher

  __fastcall CoreMpfl::CoreMpfl() : CoreSdwk () {
		
	}

  __fastcall CoreMpfl::~CoreMpfl () {

	}

	/*INIT*/
	void __fastcall CoreMpfl::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

  /*WORK*/
	void CoreMpfl::work_ () {
		PrtoSeda cVmessage;
		PrtoLmtb cVmutable;

		while (cVmessagequeue->pop__ (cVmessage)) {
			cVmutable.set_svnamespace (cVmessage.svnamespace ());
			cVmutable.set_svtable (cVmessage.svtable ());	

#ifndef iCdisablepostflusher

			cVplanner->compa (&cVmutable, (CoreTbsv::iCcompactions) cVmessage.ivcompactiontype ());
#endif

		}	
	}