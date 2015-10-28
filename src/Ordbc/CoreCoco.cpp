#include "CoreCoco.hpp"
#include "TmplShst.cpp"
#include "TmplTsrl.cpp"

  __fastcall CoreCoco::CoreCoco () {
		
		bVconnection = false;
	}

  __fastcall CoreCoco::~CoreCoco () {

		clse_ ();
	}


  bool __fastcall CoreCoco::open_ (PTR_POOL cPool, AnsiString sAddress, int iPort, AnsiString sProtocol) {
		AnsiString sSharedmemoryname;

		if (!bVconnection) {
			cVpool = cPool;
			cVsocket = boost::shared_ptr<CoreSckt> (
				new CoreSckt (CoreSckt::trsla (sAddress)));
			bVconnection = cVsocket->open_ (0);
			if (bVconnection) bVconnection = cVsocket->cnnct (iPort, sAddress);
			if (bVconnection) { 
				cVsocket->msend (sProtocol);
				bVconnection = cVsocket->mrcve (sSharedmemoryname);
				if (bVconnection && (sSharedmemoryname != "")) {
					cVsharedsend = boost::shared_ptr<CoreShst<ThrfBsrc> > (new CoreShst<ThrfBsrc> (false, sSharedmemoryname + "_SVC", true));
					cVsharedreceive = boost::shared_ptr<CoreShst<ThrfBsrr> > (new CoreShst<ThrfBsrr> (false, sSharedmemoryname + "_RES", true));
					bVsharedmemory = true;
				} else bVsharedmemory = false;
			}
			cVfilesystem = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));
		}
		return bVconnection;
	}

	/*CLoSE*/
  bool __fastcall CoreCoco::clse_ () {
		ThrfBsrc cVservice;

		if (bVconnection) {
			if (bVsharedmemory) {
				cVservice.iVbulkservicetype = iEbulkservicetype::PING;
				cVsharedsend->push_ (cVservice);
			}
			cVsocket->clse_ ();
			bVconnection= false;
		}
		return false;
	}

  /*RUN*/
	void __fastcall CoreCoco::run__ (ThrfBsrr* cReturn, ThrfBsrc* cService) {
		int iVdim;
		
		if (bVconnection) {
			if (bVsharedmemory) {
				cVsharedsend->pushs (*cService);
				*cReturn = cVsharedreceive->pop_s ();
			} else {
				boost::shared_ptr<CoreTsrl> cVserializer (new CoreTsrl ());
				boost::shared_ptr<CoreTsrl> cVdeserializer (new CoreTsrl ());
				iVdim = cVserializer->srlze<ThrfBsrc> (*cService, false);
				cVsocket->send_ ((char*) cVserializer->tobuf (), iVdim);
				cVsocket->lflsh ();
				iVdim = cVsocket->recve ();
				if (iVdim == 0) return;
				cVsocket->recve ((char*) cVdeserializer->frbub (iVdim), iVdim);
				cVdeserializer->frbue (iVdim);
				cVdeserializer->parse<ThrfBsrr> (*cReturn);
			}
		}
	}

  /*RUN*/
	void __fastcall CoreCoco::runpb (PrtoBsrr* cReturn, PrtoBsrc* cService) { 
		int iVdim;
		
		if (bVconnection) {
			if (bVsharedmemory) { 
				iVdim = cService->ByteSize ();
				cService->SerializeWithCachedSizesToArray ((google::protobuf::uint8*) cVsharedsend->pshbb (iVdim));
				cVsharedsend->pshbe ();
				iVdim = cVsharedreceive->popbb ();
				cReturn->ParseFromArray (cVsharedreceive->popbe (), iVdim);
			} else {
				std::string sVservice = cService->SerializeAsString ();
				cVsocket->send_ ((char*) sVservice.data (), sVservice.length ());
				cVsocket->lflsh ();
				iVdim = cVsocket->recve ();
				if (iVdim == 0) return;
				cVfilesystem->setbf (iVdim);
				cVsocket->recve ((char*) cVfilesystem->baddr (), iVdim);
				cReturn->ParseFromArray (cVfilesystem->baddr (), iVdim);
			}
		}
	}


	/*VALID*/
	bool __fastcall CoreCoco::valid () {

		return bVconnection;
	}