#include "CoreRlsw.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

using namespace orion;

  __fastcall CoreRlsw::CoreRlsw() : CoreSdwk () {

	}

  __fastcall CoreRlsw::~CoreRlsw () {

	}

	/*INIT*/
	void __fastcall CoreRlsw::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

  /*WORK*/
	void CoreRlsw::work_ () {
		PrtoSeda cVmessage;

		while (cVmessagequeue->pop__ (cVmessage)) {
			cVplanner->swtch ();
		}		
	}