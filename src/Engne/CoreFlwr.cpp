#include "CoreFlwr.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

using namespace orion;

  __fastcall CoreFlwr::CoreFlwr() : CoreSdwk () {}

  __fastcall CoreFlwr::~CoreFlwr () {}

	/*INIT*/
	void __fastcall CoreFlwr::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

  /*WORK*/
	void CoreFlwr::work_ () {
		PrtoSeda cVmessage;
		PrtoLmtb cVmutable;
		PTR_SEDAMANAGER cVsedamanager;

		cVsedamanager = cVplanner->getsm ();
		while (cVmessagequeue->pop__ (cVmessage)) {
			cVmutable.set_svnamespace (cVmessage.svnamespace ());
			cVmutable.set_svtable (cVmessage.svtable ());			
			cVplanner->compa (&cVmutable, CoreTbsv::iCflush);
			cVmessage.set_ivcompactiontype (MAJOR);
			cVsedamanager->pushs (CorePlan::sCmemtablepostflusher, cVmessage);
			cVmessage.Clear ();
			cVsedamanager->pushs (CorePlan::sCredologswitcher, cVmessage);
		}		
	}