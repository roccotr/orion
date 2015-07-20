#include "CorePeer.hpp"
#include "TmplList.cpp"
#include "CoreRrwr.hpp"
#include "CoreGowr.hpp"
#include "CoreAewk.hpp"
#include "CorePutl.hpp"
#include "CoreAenp.hpp"
#include "math.h"

using namespace google;
using namespace orion;

//#define iCdebugwriter
//#define iCdebugreader 


	const int CorePeer::iCtimeoutsedacallbacksms = 500;
	const int CorePeer::iCsleepgossiperms = 1000;
	const AnsiString CorePeer::sCrrworker = "ReadRepair";

  __fastcall CorePeer::CorePeer(PTR_POOL cPool, PTR_SEDAMANAGER cSedamanager, CoreEnpo::NODEDATA& cNodedata,
																CoreEnpo::iCstategossipnode iState, bool bResetlocal) {
		PrtoGoel cVgossipelement;

		cVreverse = boost::shared_ptr<CoreRvrs> (new CoreRvrs ());
		cVpool = cPool;
		cVsedamanager = cSedamanager;
		cVredolog = boost::shared_ptr<CoreAlog> (
			new CoreAlog (	cVpool, cVsedamanager, cNodedata.sVredologpath, cNodedata.sVdatapath, 
											cNodedata.sVtabletpath, cNodedata.iVredologdim, bResetlocal, cNodedata.iVgracetime));
		iVticket = 0;
		cVgossiper = boost::shared_ptr<CoreGoss> (new CoreGoss ());
		cVpartitioner = boost::shared_ptr<CorePart> (new CorePart ());
		cVendpoint = boost::shared_ptr<CoreEnpo> (new CoreEnpo ());
		cVgossiper->init_ (cVendpoint, cVredolog);
		cVhintedhandoff = boost::shared_ptr<CoreHiha> (new CoreHiha (cVredolog, cVgossiper));
		cVgossipelement.set_svnodeid (cNodedata.sVnodeid.to_string ());
		cVgossipelement.set_svdatacenterid (cNodedata.sVdatacenterid.to_string ());
		cVgossipelement.set_svaddress (cNodedata.sVthriftaddress.to_string ());
		cVgossipelement.set_ivport (cNodedata.iVthriftport);
		cNodedata.sVtoken = cVgossiper->addgl (&cVgossipelement, true).cVgossiploc->sVtoken;
		cVgossiper->state (iState);
		cVendpoint->init_ (cNodedata);	
		cVpartitioner->init_ (cVendpoint);
		sVthisnodeid = cVendpoint->getni ();
	}

  __fastcall CorePeer::~CorePeer () {

		cVhintedhandoff.reset ();
		cVendpoint.reset ();
		cVpartitioner.reset ();
		cVgossiper.reset ();
		cVredolog.reset ();
	}

	/*GET TIcket*/
	AnsiString CorePeer::getti () {
		AnsiString sVreturn;

		cVmutexticket.locks ();
		sVreturn = CoreButl::i642s (iVticket++);
		cVmutexticket.relse ();
		return sVreturn;
	}

	/*MUTaToR*/
	bool __fastcall CorePeer::muttr (	iCopcodetype iOpcodetype, PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, 
																		CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
																		AnsiString sKeyforendpoint, bool bLocalfilter, CoreAlog::TABLETLOC** cCachedtabletloc) {
		PrtoLstm cVstatement;

#ifdef iCdebugwriter
		printf ("WRITER %s.%s <- %s\n", 
			cMutable->svnamespace ().c_str (), 
			cMutable->svtable ().c_str (),
			((AnsiString) cKey->svmain ()).prntb ().c_str ());
#endif


		cVstatement.set_ivopcode (iOpcodetype);
		(cVstatement.mutable_cvmutable ())->CopyFrom (*cMutable);
		(cVstatement.mutable_cvkey ())->CopyFrom (*cKey);
		(cVstatement.mutable_cvvalue ())->CopyFrom (*cValue);

		if (iPartitiontype != CorePart::iCnopartition) {
			bool bVcallback;
			bool bVreturn;
			bool bVdummy;
			AnsiString sVticket;
			PrtoSeda cVmessageresult;
			PrtoGoel* cVgossipelement;
			CoreEnpo::POINTERGOSSIPLOC* cVpointergossiploc;
			CoreEnpo::GOSSIPLOC* cVgossiploc;
			CoreEnpo::REPLICATIONDATA cVreplicationdata;

			bVreturn = true;	
			cVendpoint->getrp (CoreEnpo::iCwrite, cVpartitioner->desow (iPartitiontype, 
												sKeyforendpoint == NULL ? (AnsiString) cKey->svmain () : sKeyforendpoint,
												CoreAlog::index (cVstatement.mutable_cvmutable ()), cPartitionloc), cVreplicationdata, bVdummy, 0,
												cPartitionloc != NULL ? cPartitionloc->iVcustomreplicationfactor : -1);		

			bVcallback = cVreplicationdata.iVremotereplicas > 0 && !bLocalfilter;
			if (bVcallback) {
				sVticket = getti ();
				cVsedamanager->newqu (sVticket);
			}
			for (int iV = 0; iV < cVreplicationdata.cVpointergossippool.count (); iV++) {
				cVpointergossiploc = cVreplicationdata.cVpointergossippool.get__ (iV);
				cVgossiploc = cVendpoint->getrg (cVpointergossiploc->iVelement);
				
#ifdef iCdebugwriter
				printf ("WRITER %s %i - %s\n", cVgossiploc->sVtoken.c_str (), cVgossiploc->bVlocal, cVgossiploc->sVwriter.c_str ());
#endif				

				if (cVgossiploc->bVlocal) bVreturn &= cVredolog->statm (&cVstatement, cCachedtabletloc);
				else if (!bLocalfilter) {
					PrtoSeda cVmessage;

					cVmessage.mutable_cvstatement ()->CopyFrom (cVstatement);
					cVgossipelement = cVmessage.mutable_cvgossipelement ();
					cVgossipelement->set_svnodeid (cVgossiploc->sVindex.to_string ());
					cVgossipelement->set_svdatacenterid (cVgossiploc->sVdatacenterid.to_string ());
					cVgossipelement->set_svaddress (cVgossiploc->sVaddress.to_string ());
					cVgossipelement->set_ivport (cVgossiploc->iVport);
					if (cVpointergossiploc->bVcallback) cVmessage.set_svcallback (sVticket.to_string ());
					else cVmessage.set_svcallback ("");
					cVsedamanager->pushs (cVgossiploc->sVwriter, cVmessage);	
				}
			}
			if (bVcallback) {
				do {
					if (cVsedamanager->popqu (sVticket, cVmessageresult, iCtimeoutsedacallbacksms)) cVreplicationdata.iVremotereplicas--;
					else {
						bVreturn = false;
						break;
					}
				} while (cVreplicationdata.iVremotereplicas > 0);
				cVsedamanager->delqu (sVticket);
			}
			return bVreturn;
		} else {
			bool bVreturn = cVredolog->statm (&cVstatement, cCachedtabletloc);
			return bVreturn;
		}
	}

	/*MUTaToR*/
	bool __fastcall CorePeer::muttr (	PrtoLstm* cStatement, CoreAlog::TABLETLOC** cCachedtabletloc) {

		return cVredolog->statm (cStatement, cCachedtabletloc);
	}

	/*ReMote QueRY*/
	bool __fastcall CorePeer::rmqry (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, CoreTble::iCquerytype iQuerytype, 
																		CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
																		bool& bNodeoffset, int iNodeoffset, CoreAlog::TABLETLOC* cCachedtabletloc, CoreSmph* cLockedmutex) {
		bool bVcallback;
		bool bVreturn;
		bool bVindex;
		bool bVrangequery;
		int iVreadconsistencylevel;
		AnsiString sVtemp;
		AnsiString sVticket;
		AnsiString sVnextbound;
		AnsiString sVprevbound;
		PrtoLqry* cVquery;
		PrtoSeda cVmessageresult;
		PrtoGoel* cVgossipelement;
		CoreEnpo::REPLICATIONDATA cVreplicationdata;
		CoreEnpo::POINTERGOSSIPLOC* cVpointergossiploc;
		CoreEnpo::GOSSIPLOC* cVgossiploc;
		PrtoSeda cVqueryresultandrepair;
		PrtoQres* cVqueryresult;
		PrtoLkey cVkeybackup;

#ifdef iCdebugreader
		printf ("READER %s.%s <- %s\n", 
			cMutable->svnamespace ().c_str (), 
			cMutable->svtable ().c_str (),
			((AnsiString) cKey->svmain ()).prntb ().c_str ());
#endif

		bVreturn = true;
		bVrangequery = iQuerytype != CoreTble::iCexactquery;
		bVindex = CoreAlog::index (cMutable);
		cVkeybackup.CopyFrom (*cKey);
		iVreadconsistencylevel = cVendpoint->getrc ();
		cVendpoint->getrp (	CoreEnpo::iCread, 
												cVpartitioner->desow (iPartitiontype, cKey->svmain (), bVindex, cPartitionloc, bVrangequery), 
												cVreplicationdata, bNodeoffset, iNodeoffset,
												cPartitionloc != NULL ? cPartitionloc->iVcustomreplicationfactor : -1);	

#ifdef iCdebugreader
						printf ("\nK (%i -> %i): %s\n", iNodeoffset, bNodeoffset, ((AnsiString) cKey->svmain ()).prntb ().c_str ());
#endif


		bVcallback = cVreplicationdata.iVremotereplicas > 0;
		if (bVcallback) {
			sVticket = getti ();
			cVsedamanager->newqu (sVticket);
		}
		for (int iV = 0; iV < cVreplicationdata.cVpointergossippool.count (); iV++) {
			cVpointergossiploc = cVreplicationdata.cVpointergossippool.get__ (iV);
			cVgossiploc = cVendpoint->getrg (cVpointergossiploc->iVelement);
					
#ifdef iCdebugreader
			printf ("READER %s %i\n", cVgossiploc->sVindex.c_str (), cVgossiploc->bVlocal);
#endif				

			if (cVgossiploc->bVlocal) {		
				cVqueryresult = cVqueryresultandrepair.add_cvlistqueryresult ();
				cVqueryresult->set_svnodeid (cVgossiploc->sVindex.to_string ());
				cVqueryresult->mutable_cvkey ()->CopyFrom (cVkeybackup);

				cVqueryresult->set_bvresult (
					cVredolog->query (cMutable, cVqueryresult->mutable_cvkey (), cVqueryresult->mutable_cvvalue (), iQuerytype, cCachedtabletloc));
				
					if (bVreturn && !(cVreplicationdata.bVnotsorted && bVcallback)) {
						bVreturn &= cVqueryresult->bvresult ();
						cValue->CopyFrom (cVqueryresult->cvvalue ());					
						sVnextbound =  cPartitionloc == NULL ? NULL : bVindex ? 
							cPartitionloc->cVpartitionindex.sVupperbound : 
							cPartitionloc->cVpartition.sVupperbound;

						if (sVnextbound != NULL && sVnextbound != "" && 
							(AnsiString) cVkeybackup.svmain () <= sVnextbound &&
							(AnsiString) cVqueryresult->cvkey ().svmain () >= sVnextbound) 
								bVreturn = false;
						else {
							cKey->CopyFrom (cVqueryresult->cvkey ());

						
						}
					}

			} else {
				PrtoSeda cVmessage;

				cVquery = cVmessage.mutable_cvquery ();
				cVquery->mutable_cvmutable ()->CopyFrom (*cMutable);
				cVquery->mutable_cvkey ()->CopyFrom (cVkeybackup);
				cVquery->set_ivquery ((iCquerytype) iQuerytype);
				cVgossipelement = cVmessage.mutable_cvgossipelement ();
				cVgossipelement->set_svnodeid (cVgossiploc->sVindex.to_string ());
				cVgossipelement->set_svdatacenterid (cVgossiploc->sVdatacenterid.to_string ());
				cVgossipelement->set_svaddress (cVgossiploc->sVaddress.to_string ());
				cVgossipelement->set_ivport (cVgossiploc->iVport);
				cVgossipelement->set_svreader (cVgossiploc->sVreader.to_string ());
				cVgossipelement->set_svwriter (cVgossiploc->sVwriter.to_string ());

				cVqueryresultandrepair.add_cvlistgossipelement ()->CopyFrom (*cVgossipelement);
				if (cVpointergossiploc->bVcallback) cVmessage.set_svcallback (sVticket.to_string ());
				else cVmessage.set_svcallback ("");
				cVsedamanager->pushs (cVgossiploc->sVreader, cVmessage);	
			}
		}

		if (bVcallback) {
			if (cLockedmutex != NULL) cLockedmutex->relse ();

			do {
				if (cVsedamanager->popqu (sVticket, cVmessageresult, iCtimeoutsedacallbacksms)) {
					cVqueryresult = cVqueryresultandrepair.add_cvlistqueryresult ();
					cVqueryresult->CopyFrom (*cVmessageresult.mutable_cvlistqueryresult (0));
					cVreplicationdata.iVremotereplicas--;

#ifdef iCdebugreader
					printf ("READER ->%s %i\n", cVqueryresult->svnodeid ().c_str (), cVqueryresult->bvresult ());
#endif		

				} else {					
					if (!cVreplicationdata.bVnotsorted) bVreturn = false;
					break;
				}
			} while (cVreplicationdata.iVremotereplicas > 0);
			if (cLockedmutex != NULL) cLockedmutex->locks ();
			cVsedamanager->delqu (sVticket);
			if (bVreturn) {

				if (!cVreplicationdata.bVnotsorted)
					bVreturn = CoreRrwr::getrs (cVqueryresultandrepair.mutable_cvlistqueryresult (), cKey, cValue);
				else {
					CoreList<POINTERRESLOC> cVpointerrespool;

					for (int iV = 0; iV < cVqueryresultandrepair.cvlistqueryresult_size (); iV++) {
						cVqueryresult = cVqueryresultandrepair.mutable_cvlistqueryresult (iV);
						if (cVqueryresult->bvresult ())
							cVpointerrespool.bnset (cVqueryresult->cvkey ().svmain ())->cVqueryresult = cVqueryresult;
					}
					if (cVpointerrespool.count () > 0) {
						cVqueryresult  = cVpointerrespool.get__ (0)->cVqueryresult;
						bVreturn = cVqueryresult->bvresult ();
						cKey->CopyFrom (cVqueryresult->cvkey ());
						cValue->CopyFrom (cVqueryresult->cvvalue ());
					} else bVreturn = false;
				}

#ifdef iCdebugreader
				printf ("READER -> (%s) %lld %i\n", cKey->svmain ().c_str (), cKey->ivtimestamp (), bVreturn); 
#endif				

				if (!bVrangequery && (!bVreturn || cVendpoint->getrr ())) {
					cVquery = cVqueryresultandrepair.mutable_cvquery ();
					cVquery->mutable_cvmutable ()->CopyFrom (*cMutable);
					cVquery->mutable_cvkey ()->CopyFrom (*cKey);
					cVquery->set_ivquery ((iCquerytype) iQuerytype);
					cVqueryresultandrepair.set_svcallback (getti ().to_string ());
					if (iVreadconsistencylevel != CoreEnpo::iCall) 
						cVsedamanager->pushs (sCrrworker, cVqueryresultandrepair);
					else bVreturn = 
						CoreRrwr::rrepr (	cVsedamanager, cVgossiper, &cVqueryresultandrepair, cValue, iCtimeoutsedacallbacksms);
				}
			}				
		}

		return bVreturn;		 																	  
	}

	/*QUERY*/
	bool __fastcall CorePeer::query (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, CoreTble::iCquerytype iQuerytype, 
																		CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc, 
																		CoreAlog::TABLETLOC* cCachedtabletloc, CoreSmph* cLockedmutex, bool bLocal) {
		bool bVreturn;
		int iVnodeoffset;
		bool bVnodeoffset;
		AnsiString sVnextbound;
		PrtoLkey cVkeybackup;

		if (iPartitiontype != CorePart::iCnopartition && !bLocal) {
			switch (iQuerytype) {
			case CoreTble::iCexactquery:
						return rmqry (cMutable, cKey, cValue, iQuerytype, iPartitiontype, cPartitionloc, bVnodeoffset, 0, cCachedtabletloc, 
													cLockedmutex);
						break;
				default:

#ifdef iCdebugreader
						printf ("\n\nK: %s\n", ((AnsiString) cKey->svmain ()).prntb ().c_str ());
#endif

						iVnodeoffset = 0;
						bVnodeoffset = bVreturn = false;
						while (!bVnodeoffset && !(bVreturn = rmqry (	cMutable, cKey, cValue, iQuerytype, 
																				iPartitiontype, cPartitionloc, 
																				bVnodeoffset, iVnodeoffset, cCachedtabletloc, cLockedmutex))) {
							iVnodeoffset++;
							bVreturn = false;						
						}

						return bVreturn;
						break;
			}
		} else {
			if (iPartitiontype != CorePart::iCnopartition) {
				cVkeybackup.CopyFrom (*cKey);
				bVreturn = cVredolog->query (cMutable, cKey, cValue, iQuerytype, cCachedtabletloc);
				sVnextbound =  cPartitionloc == NULL ? NULL : CoreAlog::index (cMutable) ? 
					cPartitionloc->cVpartitionindex.sVupperbound : 
					cPartitionloc->cVpartition.sVupperbound;
				if (sVnextbound != NULL && sVnextbound != "" && 
						(AnsiString) cVkeybackup.svmain () <= sVnextbound &&
						(AnsiString) cKey->svmain () >= sVnextbound) bVreturn = false;
				return bVreturn;
			} else return cVredolog->query (cMutable, cKey, cValue, iQuerytype, cCachedtabletloc);
		}
	}

	/*TeLePoRT*/
	bool __fastcall CorePeer::tlprt ( PrtoLmtb* cMutable, AnsiString sMain, 
																		CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
																		CoreEnpo::GOSSIPLOC& cGossiploc) {
		bool bVlocal;

		cVendpoint->getsg (	cVpartitioner->desow (iPartitiontype, sMain, 
												CoreAlog::index (cMutable), cPartitionloc), bVlocal, cGossiploc);	
		return !bVlocal;
	}

	/*GET ArchiveLog*/
	PTR_REDOLOG __fastcall CorePeer::getal () {

		return cVredolog;
	}

	/*COMPAction*/
	bool __fastcall CorePeer::compa (	PrtoLmtb* cMutable, CoreTbsv::iCcompactions iCompaction, 
																		int iCompactionlevel, int iMaxcompactionlevel) {

		return cVredolog->compa (cMutable, iCompaction, iCompactionlevel, iMaxcompactionlevel);
	}

	/*SWiTCH*/
	bool __fastcall CorePeer::swtch () {

		return cVredolog->swtch ();
	}


	/*JOIN part A*/
	bool __fastcall CorePeer::joina (AnsiString sAddress, int iPort, AnsiString sNewtoken) {
		CoreEnpo::GOSSIPLOC cVgossipelement;

		if (cVgossiper->check (sAddress, iPort)) return false;
		if (sNewtoken != "") cVgossiper->newtk (sNewtoken);

		cVgossiper->state (CoreEnpo::iCjoining);
		LOG(INFO) << "Joining:   " << sAddress.c_str () << ":" << iPort;	
		FlushLogFiles(GLOG_INFO);		
		
		cVgossipelement.sVaddress = sAddress;
		cVgossipelement.iVport = iPort;
		cVgossipelement.cVconnectionfactory->init_ (sAddress, iPort);
		if (CoreGowr::gossr (cVgossiper, &cVgossipelement)) {
			do {
				Sleep (iCsleepgossiperms);
			} while (!cVendpoint->joind ());
			
			cVgossiper->state (CoreEnpo::iCloading);
			LOG(INFO) << "Loading";	
			FlushLogFiles(GLOG_INFO);			
			
			cVgossiper->getge (NULL, cVgossipelement, -1);
			CoreGowr::gossr (cVgossiper, &cVgossipelement);
			CorePutl::frze_ (cVgossipelement.cVconnectionfactory);
			return true;
		}
		return false;
	}

	/*JOIN part B*/
	bool __fastcall CorePeer::joinb (bool bAlign) {
			
		if (bAlign) pull_ ();

		Sleep (iCsleepgossiperms);	
		cVgossiper->state (CoreEnpo::iCrunning);
		LOG(INFO) << "Running    ";	
		FlushLogFiles(GLOG_INFO);

		return true;
	}

	/*REBouND*/
	bool __fastcall CorePeer::rebnd (AnsiString sAddress, int iPort) {
		CoreEnpo::GOSSIPLOC cVgossipelement;

		cVgossiper->chnge (cVendpoint->getnd ()->sVthriftaddress, cVendpoint->getnd ()->iVthriftport);
		cVgossiper->state (CoreEnpo::iCrunning);
		LOG(INFO) << "Running    ";	
		FlushLogFiles(GLOG_INFO);
		
		cVgossipelement.sVaddress = sAddress;
		cVgossipelement.iVport = iPort;
		cVgossipelement.cVconnectionfactory->init_ (sAddress, iPort);
		if (CoreGowr::gossr (cVgossiper, &cVgossipelement)) {
			do {
				Sleep (iCsleepgossiperms);
			} while (!cVendpoint->joind ());
			return true;
		}
		return false;
	}
	
	/*LEAVE*/
	bool __fastcall CorePeer::leave (void* cAntientrophy, bool bAlign) {
		CoreEnpo::GOSSIPLOC cVgossipelement;
		bool bVreturn;
		CoreAenp* cVantientrophy;

		bVreturn = true;
		cVgossiper->getge (NULL, cVgossipelement, -1);
		cVendpoint->ustrg (cVendpoint->gettk ());

		cVgossiper->state (CoreEnpo::iCleaving);
		LOG(INFO) << "Leaving   ";	
		FlushLogFiles(GLOG_INFO);

		CoreGowr::gossr (cVgossiper, &cVgossipelement);
		CorePutl::rbpar (cVgossipelement.cVconnectionfactory);
		
		cVantientrophy = (CoreAenp*) cAntientrophy;
		if (bAlign) bVreturn &= cVantientrophy->rpair (NULL, true, true, NULL, &cVgossipelement);
		
		Sleep (iCsleepgossiperms);	
		cVgossiper->state (CoreEnpo::iCstopped);
		LOG(INFO) << "Stopped   ";	
		FlushLogFiles(GLOG_INFO);
		
		CoreGowr::gossr (cVgossiper, &cVgossipelement);

		return bVreturn;
	}

	/*SET ABort*/
	void __fastcall CorePeer::setab (bool bAbort) {

		cVredolog->setab (bAbort);
	}

	/*ADD WRiter*/
	void __fastcall CorePeer::addwr (AnsiString sWriter) {
		
		cVgossiper->addwr (sWriter);
	}

	/*ADD ReaDer*/
	void __fastcall CorePeer::addrd (AnsiString sReader) {

		cVgossiper->addrd (sReader);
	}

	/*PUT Hinted Handoff*/
	bool __fastcall CorePeer::puthh (AnsiString sNode, PrtoLsts* cStatements) {
		
		return cVhintedhandoff->put__ (sNode, cStatements);
	}

	/*PRoCess Hinted Handoff*/
	void __fastcall CorePeer::prchh () {

		cVhintedhandoff->prcss ();
	}

	/*InFo TaBlet*/
	bool __fastcall CorePeer::iftbl (AnsiString sTabletfilename, PrtoTabl* cTablet, bool bDiagnostic) {

		return cVredolog->iftbl (sTabletfilename, cTablet, bDiagnostic);
	}

	/*GET TaBlet server*/
	PTR_TABLETSERVER __fastcall CorePeer::gettb (PrtoLmtb* cMutable) {

		return cVredolog->gettb (cMutable);
	}

	/*GET ParTitioner*/
	PTR_PARTITIONER CorePeer::getpt () {

		return cVpartitioner;
	}
	
	/*GET EndPoint*/
	PTR_ENDPOINT CorePeer::getep () {

		return cVendpoint;
	}

	/*GET NodeId*/
	AnsiString __fastcall CorePeer::getni () {

		return sVthisnodeid;
	}

	/*PULL*/
	bool __fastcall CorePeer::pull_ () {
		CoreEnpo::RINGGOSSIPLOC cVreplicationdata;
		CoreList<CoreEnpo::RINGGOSSIPLOC> cVringgossippoolresult;
		CoreEnpo::RINGGOSSIPLOC* cVringgossiploc;
		bool bVreturn;

		bVreturn = true;
		cVendpoint->getpr (cVringgossippoolresult); 
		for (int iV = 0; iV < cVringgossippoolresult.count (); iV++) {
			cVringgossiploc = cVringgossippoolresult.get__ (iV);
			if (cVringgossiploc->cVgossiploc->bVlocal) return false;
			LOG(INFO) << "Pulling:   " << cVringgossiploc->cVgossiploc->sVindex.c_str ();	
			FlushLogFiles(GLOG_INFO);

			bVreturn &= CorePutl::pull_ (	cVringgossiploc->cVgossiploc->cVconnectionfactory,
																		sVthisnodeid.to_string ());
			
			//if (bVreturn) bVreturn &= CorePutl::purge (	cVringgossiploc->cVgossiploc->cVconnectionfactory); 
		}
		return bVreturn;
	}

	/*GET GOssiper*/
	PTR_GOSSIPER __fastcall CorePeer::getgo () {

		return cVgossiper;
	}