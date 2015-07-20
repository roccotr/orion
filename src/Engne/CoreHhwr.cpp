#include "CoreHhwr.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

using namespace orion;

  __fastcall CoreHhwr::CoreHhwr() : CoreSdwk () {}

  __fastcall CoreHhwr::~CoreHhwr () {}

	/*INIT*/
	void __fastcall CoreHhwr::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

  /*WORK*/
	void CoreHhwr::work_ () {
		
		cVplanner->getpr ()->prchh ();
	}