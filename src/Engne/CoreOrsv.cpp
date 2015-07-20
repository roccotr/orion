#include "CoreOrsv.hpp"
#include "TmplList.cpp"

  __fastcall CoreOrsv::CoreOrsv(	void* cThriftserver, 
																	AnsiString sHost, int iPort, int iListeners,
																	AnsiString sSharedmemoryprefix)  : TThread (true) {
		bool bVsuccess;

		cVpool = boost::shared_ptr<CorePool> (new CorePool ());
		cVthriftserver = cThriftserver;
		cVsocket = boost::shared_ptr<CoreSckt> (new CoreSckt (CoreSckt::trsla (sHost)));
		bVsuccess = cVsocket->open_ (iPort);
		
		bVsharedmemory = CoreSckt::trsla (sHost) == CoreSckt::iCsocketpipe;
		bVsharedmemory = false;
		
		iVconnectioncounter = 0;
		if (bVsuccess) bVsuccess = cVsocket->bind_ ();
		if (bVsuccess) bVsuccess = cVsocket->listn (iListeners);
		bVaborted = false;
		sVsharedmemoryprefix = sSharedmemoryprefix;
		Resume ();
	}

  __fastcall CoreOrsv::~CoreOrsv () {

	}

  /*EXECUTE*/
  void __fastcall CoreOrsv::Execute () {
		CoreSckt* cVsocketclient;

		while (!Terminated) { 
			cVsocketclient = cVsocket->accpt ();
			if (bVaborted) break;
			if (cVsocketclient == NULL) continue;
			iVconnectioncounter++;
			cVserverworkers.set__ ()->cVserverworker = boost::shared_ptr<CoreThth> (
				new CoreThth (cVpool, cVsocketclient, cVthriftserver, bVsharedmemory, sVsharedmemoryprefix, iVconnectioncounter));
		}	
	}

	/*ABORT*/
	void __fastcall CoreOrsv::abort () {
		if (!bVaborted) {
			cVsocket->cls ();
			bVaborted = true;
		}
	}

	/*IS ABoRt*/
	bool __fastcall CoreOrsv::isabr () {
		return bVaborted;
	}		
