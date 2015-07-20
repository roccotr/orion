#include "CoreRlrc.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

using namespace orion;

  __fastcall CoreRlrc::CoreRlrc() : CoreSdwk () {

	}

  __fastcall CoreRlrc::~CoreRlrc () {

	}

	/*INIT*/
	void __fastcall CoreRlrc::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

  /*WORK*/
	void CoreRlrc::work_ () {
		
		cVplanner->rcycr ();
	}