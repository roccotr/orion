#ifndef CORECWRK
#define CORECWRK

#include <glog/logging.h>

#include "CoreCocf.hpp"
#include "CoreFlqu.hpp"

#define iCmaxworksize 100

class CoreCwrk : TThread {
public:

	typedef struct CoreCjob {
		PrtoSrvc cVservice;
		PrtoSrvr cVserviceresult;
		CoreSmph cVwaitforresult;
		bool bVoptimistic;

		/*INITIALIZE*/
		CoreCjob (PrtoSrvc* cService, bool bOptimistic) {

			bVoptimistic = bOptimistic;
			if (!bVoptimistic) cVwaitforresult.locks ();
			cVservice.CopyFrom (*cService);
		}
	} CLIENTJOBLOC;

private:
	const static int iCqueuepollingtime;

	PTR_ORIONCONNECTIONFACTORY cVconnectionfactory;
	int iVbulksize;
	CoreFlqu<CLIENTJOBLOC, iCmaxworksize> cVqueue;

public:
  __fastcall CoreCwrk	();
  __fastcall ~CoreCwrk ();

	void __fastcall init_ (PTR_ORIONCONNECTIONFACTORY cConnectionfactory, int iBulksize);
	bool __fastcall addjb (CLIENTJOBLOC* cJob);
	void __fastcall abort ();
	void Execute (); 
};


#endif