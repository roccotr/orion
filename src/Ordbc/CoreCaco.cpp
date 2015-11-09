#include "CoreCaco.hpp"
#include "TmplList.cpp"


  __fastcall CoreCaco::CoreCaco (PTR_ORIONCONNECTIONFACTORY cOrionconnectionfactory, int iWorkers, int iBulksize) {

		cVorionconnectionfactory = cOrionconnectionfactory;
		iVbulksize = iBulksize;
		iVworkers = iWorkers;
		iVsimplerule = iVsubcounter = iVrefcount = iVassignworker =0;
	}

  __fastcall CoreCaco::~CoreCaco () {
	}

	/*ADD ReFerence*/
	bool __fastcall CoreCaco::addrf () {

		cVmutex.locks ();
		iVrefcount++;
		if (iVrefcount == 1)
			for (int iV = 0; iV < iVworkers; iV++)
				cVworkerpool.set__ ()->init_ (cVorionconnectionfactory, iVbulksize);
		cVmutex.relse ();
		return true;
	}

	/*SUB ReFerence*/
	bool __fastcall CoreCaco::subrf () {

		cVmutex.locks ();
		iVrefcount--;
		if (iVrefcount == 0)
			for (int iV = iVworkers - 1; iV >= 0; iV--)
				cVworkerpool.unset (iV);
		cVmutex.relse ();
		return true;
	}

  /*ADD JoB*/
	bool __fastcall CoreCaco::addjb (CoreCwrk::CLIENTJOBLOC* cJob, int iWorker) {
		int iVindex;
		bool bVreturn;

		cVmutex.locks ();
		if (iVworkers > 0) {
			if (iWorker >= 0) iVindex = iWorker % iVworkers;
			else {
				iVsubcounter = ++iVsubcounter % iVbulksize;
				if (iVsubcounter == 0) iVindex = ++iVsimplerule % iVworkers;
				else iVindex = iVsimplerule % iVworkers;
			}
			bVreturn = cVworkerpool.get__ (iVindex)->addjb (cJob);
		} else bVreturn = false;
		cVmutex.relse ();
		return bVreturn;
	}

	/*VALID*/
	bool __fastcall CoreCaco::valid () {
		bool bVreturn;

		cVmutex.locks ();
		bVreturn = iVrefcount > 0;
		cVmutex.relse ();
		return bVreturn;
	}

	/*RUN*/
	bool __fastcall CoreCaco::run__ (PrtoSrvr& cReturn, PrtoSrvc& cService, bool bOptimistic, int iWorker) {

		CoreCwrk::CLIENTJOBLOC* cVjob = new CoreCwrk::CLIENTJOBLOC (&cService, bOptimistic);
		if (addjb (cVjob, iWorker)) {
			if (!bOptimistic) {
				cVjob->cVwaitforresult.locks ();
				cReturn = cVjob->cVserviceresult;
				delete cVjob;
			}
			return true;
		} else return false;
	}

	/*GET Broker Loc*/
	CoreCaco::BROKERLOC CoreCaco::getbl (AnsiString sIndex) {
		BROKERLOC* cVbrokerloc;

		cVmutex.locks ();
		cVbrokerloc = cVbrokerpool.bnget (sIndex);
		if (cVbrokerloc  == NULL) {
			cVbrokerloc  = cVbrokerpool.bnset (sIndex);
			cVbrokerloc->iVworker = iVassignworker;
			cVbrokerloc->cVworker = cVworkerpool.get__ (iVassignworker);
			iVassignworker = (iVassignworker + 1) % iVworkers;
		}
		cVmutex.relse ();
		return *cVbrokerloc;
	}

	/*RUN*/
	bool __fastcall CoreCaco::run__ (PrtoSrvr& cReturn, PrtoSrvc& cService, bool bOptimistic, CoreCwrk* cWorker) {

		CoreCwrk::CLIENTJOBLOC* cVjob = new CoreCwrk::CLIENTJOBLOC (&cService, bOptimistic);
		if (cWorker->addjb (cVjob)) {
			if (!bOptimistic) {
				cVjob->cVwaitforresult.locks ();
				cReturn = cVjob->cVserviceresult;
				delete cVjob;
			}
			return true;
		} else return false;
	}
