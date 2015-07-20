#include "CoreTutl.hpp"
#include "CoreAewk.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

using namespace orion;

  __fastcall CoreAewk::CoreAewk() : CoreSdwk () {}

  __fastcall CoreAewk::~CoreAewk () {}

	/*INIT*/
	void __fastcall CoreAewk::init (void* cPlanner, void* cSedamanager) {

		cVplanner = (CorePlan*) cPlanner;
		cVsedamanager = (CoreSdmg*) cSedamanager;
	}

  /*WORK*/
	void CoreAewk::work_ () {
		PrtoSeda cVmessage;

		while (cVmessagequeue->pop__ (cVmessage)) {

		}		
	}

	/*Push PuLL Match*/
	bool CoreAewk::ppllm (	CoreEnpo::GOSSIPLOC* cGossipelement, 
													PrtoMktr* cPrtomerkeltreesource, PrtoMktr* cPrtomerkeltreedestination,
													AnsiString sSendernodeid, PrtoLmtb* cMutable,
													AnsiString sKeystart, int iLimitsize) {
		ThrfComm cVcommand;
		ThrfCort cVcommandresult;
		ThrfAenp* cVantientrophy;

		cVcommand.iVcommand = iEreservedkeyword::ANTIENTROPHY;
		cVcommand.iVsubcommand = iEreservedkeyword::PPULLMATCH;
		cVantientrophy = &cVcommand.cVantientrophy;
		cVantientrophy->sVnodeid = sSendernodeid.to_string ();
		CoreTutl::cpmut (cMutable, &cVantientrophy->cVmutable);
		cVantientrophy->sVkeystart = sKeystart.to_string ();
		cVantientrophy->iVlimitsize = iLimitsize;
		cVantientrophy->sVopaquedata = cPrtomerkeltreesource->SerializeAsString ();
		CoreTutl::sendc (CoreEnpo::getcf (cGossipelement), cVcommand, cVcommandresult);		
		cPrtomerkeltreedestination->Clear ();
		cPrtomerkeltreedestination->ParseFromString (cVcommandresult.cVreturnantientrophy.sVopaquedata);
		return cVcommandresult.bVreturn;
	}


	/*Push PuLL Repair*/
	bool CoreAewk::ppllr (	CoreEnpo::GOSSIPLOC* cGossipelement, PrtoMktr* cPrtomerkeltreesource, 
													PrtoLmtb* cMutable) {
		ThrfComm cVcommand;
		ThrfCort cVcommandresult;
		ThrfAenp* cVantientrophy;

		cVcommand.iVcommand = iEreservedkeyword::ANTIENTROPHY;
		cVcommand.iVsubcommand = iEreservedkeyword::PPULLREPAIR;
		cVantientrophy = &cVcommand.cVantientrophy;
		CoreTutl::cpmut (cMutable, &cVantientrophy->cVmutable);
		cVantientrophy->sVopaquedata = cPrtomerkeltreesource->SerializeAsString ();
		CoreTutl::sendc (CoreEnpo::getcf (cGossipelement), cVcommand, cVcommandresult);		
		return cVcommandresult.bVreturn;
	}




