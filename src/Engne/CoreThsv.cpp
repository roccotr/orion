#include "CoreThsv.hpp"
#include "CoreHutl.hpp"
#include "CoreXml_.hpp"
#include "CoreTutl.hpp"
#include "TmplList.cpp"

#include <glog/logging.h>
#include <gflags/gflags.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using boost::shared_ptr;
using namespace orion;

//#define iCdebugcalls
//#define iCdisablestatement


	const int CoreThsv::iCwaitshutdownms = 10;

  DEFINE_string(path, "Orion/", "Orion path");
  DEFINE_string(configuration, "", "Configuration file");

  __fastcall CoreThsv::CoreThsv() {

		iVcriticalregion = 0;
		bVstarted = false;
		bVoffline = false;
		strtp ();
	}

  __fastcall CoreThsv::~CoreThsv () {

		shtdw ();
	}

	/*STaRTuP*/
	bool __fastcall CoreThsv::strtp (CoreEnpo::iCstategossipnode iState, bool bResetlocal) {
		AnsiString sVpath;
		AnsiString sVcodename;
		AnsiString sVcommand;
		AnsiString sVparameter;
		AnsiString sVconfigurationfile;

		cVshutdownstartupmutex.locks ();
		if (!bVstarted) {
			cVpool = boost::shared_ptr<CorePool> (new CorePool ());
			CoreButl::splpt (CoreButl::left_ (CoreHutl::appnm (GetModuleHandle (NULL)), "."), sVpath, sVcodename);
			sVconfigurationfile = sVcodename + "." + CoreXml_::sCxmlext;

			if (FLAGS_path != "")  sVpath = FLAGS_path;
			if (FLAGS_configuration != "") sVconfigurationfile = FLAGS_configuration;

			cVorion = boost::shared_ptr<CoreOrn_> (new CoreOrn_ (cVpool, sVpath, sVconfigurationfile, iState, bResetlocal));
			cVplanner = cVorion->getpl ();
			bVstarted = true;
		}
		bVsuspendcriticalregion = false;
		cVshutdownstartupmutex.relse ();
		return bVstarted;
	}
	
	/*SHuTDoWn*/
	bool __fastcall CoreThsv::shtdw (bool bAbort) {

		cVshutdownstartupmutex.locks ();
		bVsuspendcriticalregion = true;
		while (!bAbort && iVcriticalregion > 0 && !bVoffline) Sleep (iCwaitshutdownms);
		if (bVstarted) {
			if (bAbort) cVplanner->setab (true);
			cVplanner.reset ();
			cVorion.reset ();
			cVpool.reset ();
			bVstarted = false;
		}
		cVshutdownstartupmutex.relse ();
		return !bVstarted;
	}

	/*ENTer Critical Region*/
	bool __fastcall CoreThsv::entcr () {
		bool bVreturn;

		bVreturn = !bVsuspendcriticalregion;
		if (bVreturn) __InterlockedIncrement (&iVcriticalregion);
		return bVreturn;
	}

	/*LeaVE Critical Region*/
	void __fastcall CoreThsv::lvecr () {

		__InterlockedDecrement (&iVcriticalregion);
	}


  /*PING*/
  bool CoreThsv::ping() {
		
		return true;
	}

  /*CREATE table*/
  bool CoreThsv::create(const ThrfL2ct& cCreateTable) {
		PrtoL2ct cVprotocreatetable;
		PrtoL2cl* cVprotocolumn;
		const ThrfL2cl* cVthriftcolumn;
		bool bVreturn;

		if (!entcr () || bVoffline) return false;
		CoreTutl::cpmut (&cCreateTable.cVmutable, cVprotocreatetable.mutable_cvmutable ());
		for (unsigned int iV = 0; iV < cCreateTable.cVcolumns.size (); iV++) {
			cVprotocolumn = cVprotocreatetable.mutable_cvcolumns ()->Add ();
			cVthriftcolumn = &cCreateTable.cVcolumns [iV];
			cVprotocolumn->set_ivtype ((iCcolumntype) cVthriftcolumn->iVtype);
			cVprotocolumn->set_svcolumn (cVthriftcolumn->sVcolumn);			
		}
		bVreturn = cVplanner->crttb (&cVprotocreatetable);
		lvecr ();
		return bVreturn;
	}

  /*STATEMENT*/
  bool CoreThsv::statement(const ThrfL2st& cStatement) {

#ifdef iCdisablestatement

		//printf ("(%i)", CoreXutl::getpn ());
		return true;
#endif

		PrtoL2st cVprotostatement;
		bool bVreturn;
		
#ifdef iCdebugcalls
		printf ("thrift statement inizio %s *%s*\n", 			
			cStatement.cVmutable.sVtable.c_str (),
			((AnsiString) cStatement.cVkey.sVmain).prntb ().c_str ());
#endif

		if (!entcr () || bVoffline) return false;
		CoreTutl::statm (cStatement, cVprotostatement); 

#ifdef iCdebugcalls
		printf ("thrift statement fine\n");
#endif

		bVreturn = cVplanner->statm (&cVprotostatement);
		lvecr ();
		return bVreturn;
  }

  /*QUERY*/
	void CoreThsv::query(ThrfL2qb& bReturn, const ThrfL2qr& cQuery) {
		PrtoL2qr cVprotoquery;
		PrtoL2mr cVdmlresult;

		//return;

#ifdef iCdebugcalls
		printf ("inizio query thrift\n");
#endif

		if (!entcr () || bVoffline) return;
		CoreTutl::query (cQuery, cVprotoquery);
		bReturn.bVreturn = cVplanner->query (&cVprotoquery, &cVdmlresult);
		bReturn.cKeyslices.clear ();
		if (bReturn.bVreturn) CoreTutl::wrtrs (&cVdmlresult, bReturn.cKeyslices);
		lvecr ();
  }

	/*OSQL*/
	void CoreThsv::osql(std::vector<ThrfL2ks>& _return, const ThrfL2os& cQuery) {
		PrtoL2mr cVdmlresult;
		PrtoL2os cVprotoosql;
		
#ifdef iCdebugcalls
		printf ("inizio osql thrift\n");
#endif

		if (!entcr () || bVoffline) return;
		CoreTutl::osql_ (cQuery, cVprotoosql);
		_return.clear ();
		if (cVplanner->osql_ (&cVprotoosql, &cVdmlresult)) CoreTutl::wrtrs (&cVdmlresult, _return);
		
#ifdef iCdebugcalls
		printf ("termino osql thrift, risultato %i\n", _return.size ());
		for (unsigned int iV = 0; iV < _return.size (); iV++) {
			ThrfL2ks row = _return [iV ];
			printf ("key %s %i\n", row.cVkey.sVmain.c_str (), row.cVcolumns.size ());
			for (unsigned int iVinner = 0; iVinner < row.cVcolumns.size (); iVinner++) {
				printf ("colonna %i %s\n", iVinner, row.cVcolumns [iVinner].sVcolumn.c_str ());
			}			
		}
#endif

		lvecr ();
	}

	/*GOSSiP*/
	void CoreThsv::gossp (ThrfGoss& _return, const ThrfGoss& cGossip, const bool bAll, const bool bDetails) {
		PrtoGoss cVprotogossip;
		PrtoGosr cVprotogossipresult;
		ThrfGoel cVthriftvoidgossipelement;

		if (!entcr () || bVoffline) return;
		for (unsigned int iV = 0; iV < cGossip.cVgossipelement.size (); iV++) 
			CoreTutl::cpgoe (&cGossip.cVgossipelement [iV], cVprotogossip.add_cvgossipelement ());				
		if (!bAll && cVplanner->gossp (&cVprotogossip, &cVprotogossipresult)) {
			CoreTutl::cpgoe (cVprotogossipresult.mutable_cvgossipelement (), &cVthriftvoidgossipelement);
			_return.cVgossipelement.push_back (cVthriftvoidgossipelement);
		} else {
			cVplanner->getgp (&cVprotogossip, bDetails);
			CoreTutl::cpgos (&cVprotogossip, &_return);
		}
		lvecr ();
	}

	/*GET NodeData*/
	void CoreThsv::getnd (CoreEnpo::NODEDATA& cNodedata) {

		cVorion->getnd (cNodedata);
	}

	/*COMMAND*/
  void CoreThsv::command(ThrfCort& _return, const ThrfComm& cCommand) {
		PrtoGoel cVgossipelement;
		PrtoTabl cVtable;
		PrtoAenp cVantientrophy;
		ThrfComm cVnewcommand;
		PrtoLmtb cVmutable;
		PrtoLmtb cVmutabledestination;
		PrtoLmtb* cVmutablepointer;
		PrtoStel cVstats;
		PrtoTop_ cVtop;

		_return.sVreturnmessage = "";
		_return.bVreturn = false;
		switch (cCommand.iVcommand) {
			case iEreservedkeyword::STARTUP:
				switch (cCommand.iVsubcommand) {
					case iEreservedkeyword::RESETLOCAL:
						_return.bVreturn = strtp (CoreEnpo::iCrunning, true);
						break;
					default:
						_return.bVreturn = strtp ();
						break;
				}
				break;
			case iEreservedkeyword::SHUTDOWN:
				switch (cCommand.iVsubcommand) {
					case iEreservedkeyword::ABORT:
						_return.bVreturn = shtdw (true);
						break;
					case iEreservedkeyword::NORMAL:
						_return.bVreturn = true;
						_return.bVreturn &= shtdw (false);
						_return.bVreturn &= strtp ();
						_return.bVreturn &= shtdw (false);
						break;
					case iEreservedkeyword::KILL:
						_return.bVreturn = shtdw (false);
						event_base_loopexit(cVserver->getUserEventBase (), 0);
						cVserver->stop ();
						break;
					default:
						break;
				}
				break;
			case iEreservedkeyword::VERS:
				_return.sVreturnmessage = ((AnsiString) iCversion + (AnsiString) " " + CoreButl::getsvr ()).to_string ();
				_return.bVreturn = true;
				break;
			case iEreservedkeyword::NETWORK:
				switch (cCommand.iVsubcommand) {
					case iEreservedkeyword::REBOUND:
						cVplanner->getgp (&cVgossipelement);
						cVnewcommand.iVport = cVgossipelement.ivport ();
						cVnewcommand.sVaddress = cVgossipelement.svaddress ();
						cVnewcommand.iVcommand = iEreservedkeyword::NETWORK;
						cVnewcommand.iVsubcommand = iEreservedkeyword::REJOIN;
						CoreTutl::sendc (cCommand.sVaddress, cCommand.iVport, cVnewcommand, _return);
						break;
					case iEreservedkeyword::ADDFAST:
					case iEreservedkeyword::ADD:
						cVplanner->getgp (&cVgossipelement);
						cVnewcommand.sVnewtoken = cVplanner->getnt ().to_string ();
						cVnewcommand.iVport = cVgossipelement.ivport ();
						cVnewcommand.sVaddress = cVgossipelement.svaddress ();
						cVnewcommand.iVcommand = iEreservedkeyword::NETWORK;
						cVnewcommand.iVsubcommand = 
							cCommand.iVsubcommand == iEreservedkeyword::ADD ? iEreservedkeyword::JOIN : iEreservedkeyword::JOINFAST;
						CoreTutl::sendc (cCommand.sVaddress, cCommand.iVport, cVnewcommand, _return);
						break;
					case iEreservedkeyword::REMOVEFAST:
					case iEreservedkeyword::REMOVE:
						cVnewcommand.iVcommand = iEreservedkeyword::NETWORK;
						cVnewcommand.iVsubcommand = 
							cCommand.iVsubcommand == iEreservedkeyword::REMOVE ? iEreservedkeyword::LEAVE : iEreservedkeyword::LEAVEFAST;
						CoreTutl::sendc (cCommand.sVaddress, cCommand.iVport, cVnewcommand, _return);
						break;
					case iEreservedkeyword::REJOIN:
						_return.bVreturn = cVplanner->rebnd (cCommand.sVaddress, cCommand.iVport);
						break;
					case iEreservedkeyword::JOIN:
						_return.bVreturn = cVplanner->join_ (cCommand.sVaddress, cCommand.iVport, cCommand.sVnewtoken, true);
						break;
					case iEreservedkeyword::JOINFAST:
						_return.bVreturn = cVplanner->join_ (cCommand.sVaddress, cCommand.iVport, cCommand.sVnewtoken, false);
						break;
					case iEreservedkeyword::LEAVE:
						_return.bVreturn = cVplanner->leave (true);
						break;
					case iEreservedkeyword::LEAVEFAST:
						_return.bVreturn = cVplanner->leave (false);
						break;
					case iEreservedkeyword::STARTTHRIFT:	
						bVoffline = false;
						_return.bVreturn = true;
						break;
					case iEreservedkeyword::STOPTHRIFT:		
						bVoffline = true;
						_return.bVreturn = true;
						break;
					case iEreservedkeyword::DISCONNECT:		
						event_base_loopexit(cVserver->getUserEventBase (), 0);
						cVserver->stop ();
						break;
					case iEreservedkeyword::STOPSEDA:
						_return.bVreturn = cVplanner->setsm (false);
						break;
					case iEreservedkeyword::STARTSEDA:
						_return.bVreturn = cVplanner->setsm (true);
						break;
				}
				break;
			case iEreservedkeyword::TOP:
				_return.bVreturn = cVplanner->top__ (cVtop);
				CoreTutl::cptop (&cVtop, &_return.cVreturntop);
				break;
			case iEreservedkeyword::STATS:
				CoreTutl::cpmut (&cCommand.cVmutable, &cVmutable);
				_return.bVreturn = cVplanner->stats (
					cCommand.iVsubcommand == iEreservedkeyword::WRITE ? CorePlan::iCwritestats : CorePlan::iCreadstats,
					&cVmutable, &cVstats);
				CoreTutl::cpsta (&cVstats, &_return.cVreturnstats);
				break;
			case iEreservedkeyword::SIZE:
				_return.bVreturn = true;
				_return.cVreturnstats.iVsize = cVplanner->getsz ();
				break;
			case iEreservedkeyword::STATSTHRIFT:
				_return.bVreturn = true;
				_return.cVreturnthriftstats.iVconnections = cVserver->getNumConnections ();
				_return.cVreturnthriftstats.iVidleconnections = cVserver->getNumIdleConnections ();
				_return.cVreturnthriftstats.iVmaxconnections = cVserver->getMaxConnections ();
				_return.cVreturnthriftstats.iVprocessors = cVserver->getNumActiveProcessors ();
				_return.cVreturnthriftstats.iVmaxprocessors = cVserver->getMaxActiveProcessors ();
				break;
			case iEreservedkeyword::TABLE:
				if (cCommand.cVmutable.sVtable != "") {
					CoreTutl::cpmut (&cCommand.cVmutable, &cVmutable);
					cVmutablepointer = &cVmutable;
				} else cVmutablepointer = NULL;
				switch (cCommand.iVsubcommand) {
					case iEreservedkeyword::TOUCH:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::touch);
						break;
					case iEreservedkeyword::REPAIR:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::rpair);
						break;
					case iEreservedkeyword::CLEAN:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::clean);
						break;
					case iEreservedkeyword::TRUNCATE:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::trunc);
						break;
					case iEreservedkeyword::SHRINK:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::shrnk);
						break;
					case iEreservedkeyword::SOFTSHRINK:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::soshr);
						break;
					case iEreservedkeyword::PURGE:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::purge);
						break;
					case iEreservedkeyword::REBUILD:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::rbldi);
						break;
					case iEreservedkeyword::SPLIT:
						if (cCommand.sVnewtoken != "") 
							_return.bVreturn = cVplanner->split (cVmutablepointer, cCommand.sVnewtoken, cCommand.sVnewtoken);
						else _return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::split);
						break;
					case iEreservedkeyword::FREEZE:
						cVplanner->lnch_ (cVmutablepointer, &CorePlan::split);
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::frzsp);
						break;
					case iEreservedkeyword::REBUILDPART:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::rbpar);
						break;
					case iEreservedkeyword::COMPACTION:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::fcomp);
						break;
					case iEreservedkeyword::STORE:
						if (cVmutablepointer == NULL) _return.bVreturn = cVplanner->sttbs ();
						else _return.bVreturn = cVplanner->sttbl (cVmutablepointer, cCommand.sVnewtoken);
						break;
					case iEreservedkeyword::LOAD:
						_return.bVreturn = cVplanner->ldtbl (cVmutablepointer, &cVtable);
						CoreTutl::cptbl (&cVtable, &_return.cVreturntable);
						break;
					case iEreservedkeyword::RELOAD:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::rltbl);
						break;
					case iEreservedkeyword::INFO:
						_return.bVreturn = cVplanner->iftbl (cVmutablepointer, &cVtable);
						CoreTutl::cptbl (&cVtable, &_return.cVreturntable);
						break;
					case iEreservedkeyword::FORGET:
						_return.bVreturn = cVplanner->lnch_ (cVmutablepointer, &CorePlan::dltbl);
						break;
					case iEreservedkeyword::CLONE:
						CoreTutl::cpmut (&cCommand.cVmutabledestination, &cVmutabledestination);
						_return.bVreturn = cVplanner->clone (cVmutablepointer, &cVmutabledestination, cCommand.iVtimestamp, _return.iVreturn);
						break;
				}
				break;
			case iEreservedkeyword::ANTIENTROPHY:
				switch (cCommand.iVsubcommand) {
					case iEreservedkeyword::PULLALL:
						_return.bVreturn = cVplanner->pulld (cCommand.sVnodeid);
						break;
					case iEreservedkeyword::PPULLMATCH:
						CoreTutl::cpaen (&cCommand.cVantientrophy, &cVantientrophy);
						_return.bVreturn = cVplanner->ppmch (&cVantientrophy);
						CoreTutl::cpaen (&cVantientrophy, &_return.cVreturnantientrophy);
						break;
					case iEreservedkeyword::PPULLREPAIR:
						CoreTutl::cpaen (&cCommand.cVantientrophy, &cVantientrophy);
						_return.bVreturn = cVplanner->pprep (&cVantientrophy);
						CoreTutl::cpaen (&cVantientrophy, &_return.cVreturnantientrophy);
						break;
				}
				break;
		}
	}

	/*LOCAL STATEMENT*/
	bool CoreThsv::localstatement(const std::vector<ThrfLstm>& cMutations) {
		PrtoLstm cVstatement;

		if (!entcr () || bVoffline) return false;
		for (unsigned int iV = 0; iV < cMutations.size (); iV++) {
			CoreTutl::cpstm (&cMutations [iV], &cVstatement);
			cVplanner->lcstm (&cVstatement);
		}
		lvecr ();
		return true;
	}

	/*LOCALQUERY*/
  void CoreThsv::localquery(std::vector<ThrfL1ks>& _return, const std::vector<ThrfLqry>& cQueries) {
		PrtoLqry cVquery;
		PrtoLval cVprotovalue;
		ThrfL1ks cVthriftkeyslice;

		if (!entcr () || bVoffline) return;
		for (unsigned int iV = 0; iV < cQueries.size (); iV++) {
			CoreTutl::cpqry (&cQueries [iV], &cVquery);
			cVthriftkeyslice.bVresult = cVplanner->lcqry (&cVquery, &cVprotovalue);
			CoreTutl::cpkey (&cVquery.cvkey (), &cVthriftkeyslice.cVkey);
			CoreTutl::cpval (&cVprotovalue, &cVthriftkeyslice.cVvalue);
			_return.push_back (cVthriftkeyslice);
		}
		lvecr ();
	}

	/*PROTOBATCH*/
	void CoreThsv::protobatch(std::vector<std::string> & _return,const std::vector<std::string> & sOpaque) {
		PrtoSrvc cVprotoservice;
		PrtoSrvr cVresult;

		if (!entcr () || bVoffline) return;
		for (unsigned int iV = 0; iV < sOpaque.size (); iV++) {
			cVprotoservice.ParseFromString (sOpaque [iV]);
			switch (cVprotoservice.ivservicetype ()) {
				case (STATEMENT):
					cVresult.Clear ();
					cVresult.set_bvreturn ( cVplanner->statm (cVprotoservice.mutable_cvstatement ()));
					_return.push_back (cVresult.SerializeAsString ());
					break;
				case (QUERY):
					cVresult.Clear ();
					cVresult.set_bvreturn ( cVplanner->query (cVprotoservice.mutable_cvquery (), cVresult.mutable_cvdmlresult ()));
					_return.push_back (cVresult.SerializeAsString ());
					break;
				case (OSQL):
					cVresult.Clear ();
					cVresult.set_bvreturn ( cVplanner->osql_ (cVprotoservice.mutable_cvosql (), cVresult.mutable_cvdmlresult ()));
					_return.push_back (cVresult.SerializeAsString ());
					break;
			}
		}
		lvecr ();
	}

	/*THRIFTBATCH*/
	void CoreThsv::thriftbatch (ThrfSrvr* _return, const ThrfSrvc* cService) {
			
		switch (cService->iVservicetype) {
			case iEservicetype::STATEMENT:
				_return->bVreturn = statement (cService->cVstatement);
				break;
			case iEservicetype::QUERY:
				query(_return->cVqueryresult, cService->cVquery);
				break;
			case iEservicetype::OSQL:
				osql (_return->cVdmlresult, cService->cVosql);
				break;
		}
	}

	/*THRIFTBATCH*/
	void CoreThsv::thriftbatch (std::vector<ThrfSrvr> & _return,const std::vector<ThrfSrvc> & cService) {
		ThrfSrvr cVthriftvoidresult;	

		for (unsigned int iV = 0; iV < cService.size (); iV++) {
			_return.push_back (cVthriftvoidresult);
			thriftbatch (&_return [iV], &cService [iV]);
		}
	}

	/*PROTOBATCH*/
	void CoreThsv::protobatch (PrtoSrvr* _return, PrtoSrvc* cService, CorePlan::PTR_TABLELOC* cTablelocpointer) {

		switch (cService->ivservicetype ()) {
			case STATEMENT:
				if (!((cTablelocpointer != NULL) &&  (*cTablelocpointer != NULL) && 
						(*cTablelocpointer)->sVindex == CoreAlog::gettk (cService->mutable_cvstatement ()->mutable_cvmutable ()))) {
							(*cTablelocpointer).reset ();
				}
				_return->set_bvreturn (cVplanner->statm (cService->mutable_cvstatement (), true, false, false, cTablelocpointer));
				break;
			case QUERY:
				_return->set_bvreturn (cVplanner->query (cService->mutable_cvquery (), _return->mutable_cvdmlresult ()));
				break;
			case OSQL:
				_return->set_bvreturn (cVplanner->osql_ (cService->mutable_cvosql (), _return->mutable_cvdmlresult ()));
		}
	}

	/*PROTOBATCH*/
	void CoreThsv::protobatch (RepeatedPtrField<PrtoSrvr>* _return, RepeatedPtrField<PrtoSrvc>* cRepeatedservice) {
		PrtoSrvr* cVresult;
		CorePlan::PTR_TABLELOC cVtablecache;
		
		for (int iV = 0; iV < cRepeatedservice->size (); iV++) {
			cVresult = _return->Add ();
			protobatch (cVresult, cRepeatedservice->Mutable (iV), &cVtablecache);	
		}
	}

	/*SET EventBase*/
	void CoreThsv::seteb (TNonblockingServer* cServer) {
		cVserver = cServer;
	}

	/*ALIVE*/
	bool CoreThsv::alive () {

		return bVstarted;
	}
