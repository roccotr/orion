#include "CoreCwrk.hpp"
#include "TmplFlqu.cpp"
#include "TmplList.cpp"

//#define iCdebugworker

	const int CoreCwrk::iCqueuepollingtime = 100;

  __fastcall CoreCwrk::CoreCwrk () : TThread (true) {

	}

  __fastcall CoreCwrk::~CoreCwrk () {

		abort ();
	}

	void __fastcall CoreCwrk::init_ (PTR_ORIONCONNECTIONFACTORY cConnectionfactory, int iBulksize) {

		cVconnectionfactory = cConnectionfactory;
		iVbulksize = iBulksize;
		Resume ();
	}

  /*ADD JoB*/
	bool __fastcall CoreCwrk::addjb (CLIENTJOBLOC* cJob) {

		return cVqueue.push_ (cJob);
	}

	/*EXECUTE*/
	void CoreCwrk::Execute () {
		int iVbulkdim;
		int iVbulkcount;

		while (!Terminated) {
			PrtoBsrc cVbulkservice;
			PrtoBsrr cVbulkserviceresult;
			CoreList<CLIENTJOBLOC*> cVbulk;
			CLIENTJOBLOC* cVjobloc;
			PTR_ORIONCONNECTION cVorionconnection;

			cVbulkservice.Clear ();
			cVbulkserviceresult.Clear ();

			if (cVqueue.pop__ (&cVjobloc)) {
				*cVbulk.set__ () =	cVjobloc;
				cVbulkservice.add_cvbulkproto ()->CopyFrom (cVjobloc->cVservice);

#ifdef iCdebugworker
				printf ("%s\n", cVjobloc->cVservice.mutable_cvstatement ()->mutable_cvkey ()->svmain ().c_str ());
#endif

			} else {
				cVqueue.wait_ (iCqueuepollingtime);
				continue;
			}
			iVbulkdim = iVbulksize;
			while (iVbulkdim-- > 0 && cVqueue.pop__ (&cVjobloc)) {

#ifdef iCdebugworker
				printf ("%s\n", cVjobloc->cVservice.mutable_cvstatement ()->mutable_cvkey ()->svmain ().c_str ());
#endif

				*cVbulk.set__ () =	cVjobloc;
				cVbulkservice.add_cvbulkproto ()->CopyFrom (cVjobloc->cVservice);
			}
			iVbulkcount = cVbulk.count ();
			if (iVbulkcount > 0) {
				cVorionconnection = cVconnectionfactory->chout();
				cVbulkservice.set_ivbulkservicetype (BULKPROTO);
				cVorionconnection->runpb (&cVbulkserviceresult, &cVbulkservice);

				/*
				cVbulkserviceresult.Clear ();
				for (int iVouter = 0; iVouter < cVbulkservice.cvbulkproto_size (); iVouter++) {
					PrtoSrvr* cVres = cVbulkserviceresult.add_cvbulkproto ();
					for (int iV = 0; iV < 700; iV++) cVres->set_bvreturn (iV != rand ());
				}*/

				cVconnectionfactory->chin_ (cVorionconnection);
			}
			for (int iV = 0; iV < iVbulkcount; iV++) {
				cVjobloc = *cVbulk.get__ (iV);
				if (cVjobloc->bVoptimistic) {
					delete cVjobloc;
				} else {
					cVjobloc->cVserviceresult.CopyFrom (cVbulkserviceresult.cvbulkproto (iV));
					cVjobloc->cVwaitforresult.relse ();
				}
			}
			cVqueue.wait_ (iCqueuepollingtime);
		}
	}

	/*ABORT*/
	void __fastcall CoreCwrk::abort () {

		Terminate ();
	}
