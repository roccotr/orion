#include "CoreSdmg.hpp"
#include "CoreButl.hpp"
#include "TmplList.cpp"
#include "TmplSdmg.cpp"
#include "TmplSdmq.cpp"
#include "TmplSdel.cpp"

using namespace google;

//#define iCdisablestages
//#undef iCdisableprintlog

	const int CoreSdmg::iCidlems = 10;

  __fastcall CoreSdmg::CoreSdmg() {
		
		bVactive = false;
		
	}

  __fastcall CoreSdmg::~CoreSdmg () {

	}

	/*INIT*/
	void __fastcall CoreSdmg::init_ (PTR_VOID cPlanner) {

		cVplanner = cPlanner;
	}

	/*START*/
	bool __fastcall CoreSdmg::start () {

#ifndef iCdisablestages
		SEDALOC* cVsedaloc;


		if (!bVactive) {
			for (int iV = 0; iV < cVsedastages.count (); iV++) {
				cVsedaloc = cVsedastages.get__ (iV);
				for (int iVinner = 0; iVinner < cVsedaloc->cVworkers.count (); iVinner++) 
					cVsedaloc->cVworkers.get__ (iVinner)->cVsedaelement->start ();
#ifndef iCdisableprintlog
				printf ("%s [%i worker/s started]\n", CoreButl::lpad_ (cVsedaloc->sVindex, '.', 25).c_str (), cVsedaloc->cVworkers.count ());
#endif

				LOG(INFO) << "S running: " << cVsedaloc->sVindex.c_str ();
			}
			bVactive = true;
			return true;
		}


#endif

		return false;
	}

	/*STOP*/
	bool __fastcall CoreSdmg::stop_ () {

#ifndef iCdisablestages
		SEDALOC* cVsedaloc;

		if (bVactive) {
			for (int iV = 0; iV < cVsedastages.count (); iV++) {
				cVsedaqueuesmutex.locks ();
				cVsedaloc = cVsedastages.get__ (iV);
				cVsedaloc->bVvalid = false;
				cVsedaqueuesmutex.relse ();
				while (cVsedaloc->cVmessagequeue.count () > 0) Sleep (iCidlems);

				LOG(INFO) << "S stopping:" << cVsedaloc->sVindex.c_str ();

				for (int iVinner = 0; iVinner < cVsedaloc->cVworkers.count (); iVinner++) 
					cVsedaloc->cVworkers.get__ (iVinner)->cVsedaelement->stop_ ();

#ifndef iCdisableprintlog
				printf ("%s [%i worker/s stopped]\n", CoreButl::lpad_ (cVsedaloc->sVindex, '.', 25).c_str (), cVsedaloc->cVworkers.count ());
#endif

				LOG(INFO) << "S ends:    " << cVsedaloc->sVindex.c_str ();
				FlushLogFiles(GLOG_INFO);
			}
			bVactive = false;
			return true;
		}

#endif

		return false;
	}

	/*PUSH Stage message*/
	bool __fastcall CoreSdmg::pushs (AnsiString sName, PrtoSeda& cQueuemessage) {
		SEDALOC* cVsedaloc;
		bool bVreturn;

		bVreturn = false;
		cVsedastagesmutex.locks ();
		cVsedaloc = cVsedastages.bnget (sName);
		if (cVsedaloc != NULL && cVsedaloc->bVvalid) 
			bVreturn = cVsedaloc->cVmessagequeue.push_ (cQueuemessage);
		cVsedastagesmutex.relse ();
		return bVreturn;
	}


	/*NEW custom QUeue*/
	bool __fastcall CoreSdmg::newqu (AnsiString sName) {
		SEDALOC* cVsedaloc;

		cVsedaqueuesmutex.locks ();
		cVsedaloc = cVsedaqueues.bnset (sName);
		cVsedaloc->cVmessagequeue.init (sName);
		cVsedaqueuesmutex.relse ();
		return true;
	}

	/*DELete custom QUeue*/
	bool __fastcall CoreSdmg::delqu (AnsiString sName) {
		bool bVreturn;

		bVreturn = false;
		cVsedaqueuesmutex.locks ();
		bVreturn = cVsedaqueues.bnuns (sName);
		cVsedaqueuesmutex.relse ();
		return bVreturn;
	}

	/*PuSH custom QUeue message*/
	bool __fastcall CoreSdmg::pshqu (AnsiString sName, PrtoSeda& cQueuemessage) {
		SEDALOC* cVsedaloc;
		bool bVreturn;

		bVreturn = false;
		cVsedaqueuesmutex.locks ();
		cVsedaloc = cVsedaqueues.bnget (sName);
		if (cVsedaloc != NULL && cVsedaloc->bVvalid) 
			bVreturn = cVsedaloc->cVmessagequeue.push_ (cQueuemessage);
		cVsedaqueuesmutex.relse ();
		return bVreturn;
	}

	/*POP custom QUeue message*/
	bool __fastcall CoreSdmg::popqu (AnsiString sName, PrtoSeda& cQueuemessage, int iWatchdogms) {
		SEDALOC* cVsedaloc;
		CoreSdmq<PrtoSeda>* cVmessagequeue;

		cVsedaqueuesmutex.locks ();
		cVsedaloc = cVsedaqueues.bnget (sName);
		if (cVsedaloc != NULL) {
			cVmessagequeue = &cVsedaloc->cVmessagequeue;
			cVsedaqueuesmutex.relse ();
			if (cVmessagequeue->pop__ (cQueuemessage)) return true;
			else {
				cVmessagequeue->wait_ (iWatchdogms);
				return cVmessagequeue->pop__ (cQueuemessage);
			}
		}		
		cVsedaqueuesmutex.relse ();
		return false;
	}