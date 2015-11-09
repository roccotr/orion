#include "CoreCacp.hpp"
#include "CoreAlog.hpp"
#include "TmplList.cpp"
#include "TmplLstc.cpp"
#include "TmplCche.cpp"

using namespace orion;

  __fastcall CoreCacp::CoreCacp (PTR_ARIESCONNECTION cAriesconnection) {

		cVariesconnection = cAriesconnection;
	}

  __fastcall CoreCacp::~CoreCacp () {
	}

  /*RUN*/
	bool __fastcall CoreCacp::run__ (PrtoSrvr& cReturn, PrtoSrvc& cService, bool bOptimistic) {
		bool bVreturn;
		PrtoLmtb* cVmutable;
		CoreCaco::BROKERLOC* cVbrokerloc;

		switch (cService.ivservicetype ()) {
			case STATEMENT: cVmutable = cService.mutable_cvstatement ()->mutable_cvmutable (); break;
			case OSQL: cVmutable = NULL; break;
			case QUERY: cVmutable = cService.mutable_cvquery ()->mutable_cvmutable (); break;
		}
		if (cVmutable != NULL) {
			AnsiString sVindex = CoreAlog::gettk (cVmutable);
			cVbrokerloc = cVprivatebrokerpool.bnget (sVindex);
			if (cVbrokerloc == NULL) {
				cVbrokerloc = cVprivatebrokerpool.bnset (sVindex);
				*cVbrokerloc= cVariesconnection->getbl (sVindex);
			}
			bVreturn = cVariesconnection->run__ (cReturn, cService, bOptimistic, cVbrokerloc->cVworker);
		} else bVreturn = cVariesconnection->run__ (cReturn, cService, bOptimistic);
		return bVreturn;
	}
