#include "CoreSdmg.hpp"

using namespace orion;

  /*INIT*/
	template<class T>
	void __fastcall CoreSdmg::init_ (AnsiString sName, int iPollingtimems, int iWorkers, bool bForceworker) {
		SEDALOC* cVsedastageloc;
		SEDAWORKERLOC* cVsedaworkerloc;
		PTR_SEDAWORKER cVsedaworker;

		cVsedastageloc = cVsedastages.bnset (sName);
		cVsedastageloc->cVmessagequeue.init (sName);
		for (int iV = 0; iV < iWorkers; iV++) {
			cVsedaworkerloc = cVsedastageloc->cVworkers.set__ (); 
			cVsedaworkerloc->sVindex = sName + "_" + (AnsiString)  iV;
			cVsedaworker = cVsedaworkerloc->cVworker = boost::shared_ptr<CoreSdwk> ((CoreSdwk*) new T ());
			cVsedaworker->init (cVplanner);
			cVsedaworkerloc->cVsedaelement = boost::shared_ptr<CoreSdel<PrtoSeda> > (
				new CoreSdel<PrtoSeda> (&cVsedastageloc->cVmessagequeue, cVsedaworkerloc->sVindex, iPollingtimems, cVsedaworker, bForceworker));
		}
	}