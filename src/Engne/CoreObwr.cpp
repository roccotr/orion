#include "CoreObwr.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

  __fastcall CoreObwr::CoreObwr() : CoreSdwk () {

	}

  __fastcall CoreObwr::~CoreObwr () {

	}

	/*INIT*/
	void __fastcall CoreObwr::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

  /*WORK*/
	void CoreObwr::work_ () {
		PrtoSeda cVmessage;

		while (cVmessagequeue->pop__ (cVmessage)) {
			cVplanner->lnch_ (NULL, &CorePlan::rltbl);
		}		
	}