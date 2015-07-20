#include "CorePlan.hpp"
#include "CoreXml_.hpp"
#include "DataXml_.hpp"
#include "CoreHutl.hpp"
#include "CoreAenp.hpp"
#include "TmplList.cpp"
#include "TmplLiss.cpp"
#include "TmplCche.cpp"
#include "TmplLssc.cpp"
#include "md5.h"

#include "CoreFlwr.hpp"
#include "CoreMpfl.hpp"
#include "CoreMrfl.hpp"
#include "CoreRlsw.hpp"
#include "CoreRmwr.hpp"
#include "CoreRmrd.hpp"
#include "CoreGowr.hpp"
#include "CoreHhwr.hpp"
#include "CoreRrwr.hpp"
#include "CoreObwr.hpp"
#include "CoreRlrc.hpp"
#include "CoreTutl.hpp"
#include "CorePutl.hpp"
#include "CoreAutl.hpp"

#include "TmplSdmg.cpp"
#include "TmplSdmq.cpp"
#include "TmplSdel.cpp"

using namespace google;

//#define iCdebugqryl2
//#define iCdebugstatement
//#define iCdebugvalidation
//#define iCcompactiontemporaryfix
//#define iCdebugmutator
//#define iCnoworkers


//#define QRYL2 qryl2
#define QRYL2 qrl2t

	const AnsiString CorePlan::sCdefaultaccessgroup = "DEFAULT";
	const AnsiString CorePlan::sCindexseparator =  CoreButl::chr2s (0);
	const double CorePlan::dCmovingaveragedimdelay = 20;
	const AnsiString CorePlan::sCpromptupdates = "Updates: ";

	const AnsiString CorePlan::sCflushwriter = "FlushWriter";
	const AnsiString CorePlan::sCmemtablepostflusher = "MemtablePostFlusher";
	const AnsiString CorePlan::sCredologswitcher = "RedologSwitcher";
	const AnsiString CorePlan::sCgossipworker = "Gossiper";
	const AnsiString CorePlan::sCremotewriter = "RemoteWriter";
	const AnsiString CorePlan::sCremotereader = "RemoteReader";
	const AnsiString CorePlan::sChhworker = "HintedHandoff";
	const AnsiString CorePlan::sCmemtablepartial = "MemtablePartialFlusher";
	const AnsiString CorePlan::sCredologrecycler = "RedologRecycler";


  __fastcall CorePlan::CorePlan(	PTR_POOL cPool, CoreEnpo::NODEDATA& cNodedata) {
		LOG(WARNING) << "Plan starts";

		cVreverse = boost::shared_ptr<CoreRvrs> (new CoreRvrs ());
		cVparser = boost::shared_ptr<CorePars> (new CorePars ());
		cVglobalstats.iVstartuptime = CoreButl::tmstl ();
		cVpool = cPool;
		cVnodedata = cNodedata; 
		cVnodedata.sVdatapath = cVnodedata.sVdatapath != NULL ? cVnodedata.sVdatapath : cVnodedata.sVtabletpath;
		cVnodedata.sVredologpath = cVnodedata.sVredologpath != NULL ? cVnodedata.sVredologpath : cVnodedata.sVtabletpath;

#ifndef iCdisableprintlog
		printf ("Datapath: %s\n", cVnodedata.sVdatapath.c_str ());
		printf ("Redologpath: %s\n", cVnodedata.sVredologpath.c_str ());
#endif

		LOG(WARNING) << "Data path: " << cVnodedata.sVdatapath.c_str ();
		LOG(WARNING) << "Redo path: " << cVnodedata.sVredologpath.c_str ();
		FlushLogFiles(GLOG_WARNING);

		cVnodedata.iVmaxcompactionlevel = Max (cVnodedata.iVmaxcompactionlevel, 0);
	}

  __fastcall CorePlan::~CorePlan () {
		
		LOG(WARNING) << "Plan ends";
		FlushLogFiles(GLOG_WARNING);
	}

  void __fastcall CorePlan::init_ ( PTR_VOID cThis, CoreEnpo::iCstategossipnode iState, bool bResetlocal) {
		int iV;
		AnsiString sVtemp;

		cVsedamanager = boost::shared_ptr<CoreSdmg> (new CoreSdmg ());
		cVsedamanager->init_ (cThis);

#ifndef iCnoworkers
		cVsedamanager->init_<CoreFlwr> (sCflushwriter, 1000, 1);
		cVsedamanager->init_<CoreMpfl> (sCmemtablepostflusher, 1000, 1);
		cVsedamanager->init_<CoreMrfl> (sCmemtablepartial, 1000, 1);
		cVsedamanager->init_<CoreRlsw> (sCredologswitcher, 1000, 1);
		cVsedamanager->init_<CoreGowr> (sCgossipworker, 1000, 1, true);
		cVsedamanager->init_<CoreHhwr> (sChhworker, 1000, 1, true);
		cVsedamanager->init_<CoreRrwr> (CorePeer::sCrrworker, 100, 1);
		cVsedamanager->init_<CoreObwr> (CoreGoss::sCobjectmanager, 1000, 1);
		cVsedamanager->init_<CoreRlrc> (sCredologrecycler, 15000, 1, true);
#endif

		cVpeer = boost::shared_ptr<CorePeer> (new CorePeer (cVpool, cVsedamanager, cVnodedata, iState, bResetlocal));

		for (iV = 0; iV < cVnodedata.iVconcurrentwrites; iV++) {
			sVtemp = sCremotewriter + (AnsiString) iV;
			cVpeer->addwr (sVtemp);
			cVsedamanager->init_<CoreRmwr> (sVtemp, 1000, 1, true);
		}
		for (iV = 0; iV < cVnodedata.iVconcurrentreads; iV++) {
			sVtemp = sCremotereader + (AnsiString) iV;
			cVpeer->addrd (sVtemp);
			cVsedamanager->init_<CoreRmrd> (sVtemp, 1000, 1, true);
		}

		cVsedamanager->start ();

		cVredolog = cVpeer->getal ();
		cVpartitioner = cVpeer->getpt ();
		cVgossiper = cVpeer->getgo ();
		cVgossiper->init_ (cVsedamanager);
		cVgossiper->rgold ();
		
		LOG(INFO) << "Plan running";
		FlushLogFiles(GLOG_WARNING);
		FlushLogFiles(GLOG_INFO);
	}

	/*END*/
	void __fastcall CorePlan::end__ () {

		LOG(INFO) << "Plan stopping";
		cVsedamanager->stop_ ();
		LOG(INFO) << "Seda manager stopped";
		cVtablepool.clear ();
		LOG(INFO) << "Tabletpool ends";
		cVsedamanager.reset ();
		cVredolog.reset ();
		cVpartitioner.reset ();
		cVgossiper.reset ();
		cVparser.reset ();
		cVpeer.reset ();

		LOG(INFO) << "Peer ends";
		FlushLogFiles(GLOG_INFO);

		cVpool.reset ();
	}

  /*INSeRT*/
	bool __fastcall CorePlan::insrt (PrtoLmtb* cMutable, PrtoPval* cInputrow) {
		AnsiString sVvalue;
		PrtoLkey cVkey;
		PrtoLval cVvaluecolumnoutput;
		PrtoPvlc* cVvaluecolumninput;

		CoreAutl::zrkey (&cVkey);
		cVkey.set_svmain (cInputrow->mutable_cvvaluecells (0)->svvalue ());
		for (int iVinner = 1; iVinner < cInputrow->mutable_cvvaluecells ()->size (); iVinner++) {
			cVvaluecolumninput = cInputrow->mutable_cvvaluecells (iVinner);
			cVkey.set_svaccessgroup (cVvaluecolumninput->svcolumn ());
			cVkey.set_ivtimestamp (CoreButl::tmstl ());
			cVvaluecolumnoutput.set_svopaquevalue (cVvaluecolumninput->svvalue ());
			cVvaluecolumnoutput.set_ivtimestamp (cVkey.ivtimestamp ());
			muttr (cMutable, &cVkey, &cVvaluecolumnoutput, CorePart::iCnopartition, NULL, NULL, false, NULL);
		}
		return true;
	}

	/*MUTaToR*/
	bool __fastcall CorePlan::muttr (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, 
																		CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
																		AnsiString sKeyforendpoint, bool bLocalfilter, CoreAlog::TABLETLOC** cCachedtabletloc) {

#ifdef iCdebugmutator
		printf ("R %s-%s\n", cMutable->svtable ().c_str (), ((AnsiString) cKey->svmain ()).prntb ().c_str ());
#endif

		return cVpeer->muttr (MUTATOR, cMutable, cKey, cValue, 
													iPartitiontype, cPartitionloc, sKeyforendpoint, bLocalfilter, cCachedtabletloc);
	}

	/*QUERY*/
	bool __fastcall CorePlan::query (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, CoreTble::iCquerytype iQuerytype, 
																		CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc, 
																		CoreAlog::TABLETLOC* cCachedtabletloc, CoreSmph* cLockedmutex) {

		return cVpeer->query (cMutable, cKey, cValue, iQuerytype, iPartitiontype, cPartitionloc, cCachedtabletloc, cLockedmutex);
	}

	/*LoCal QueRY*/
	bool __fastcall CorePlan::lcqry (PrtoLqry* cQuery, PrtoLval* cValueresult, bool bGlobalmutex) {
		bool bVreturn;
		bool bVindex;
		PTR_TABLELOC cVtableloc;
		CorePart::iCpartitiontype iVpartitiontype;
		CorePart::PARTITIONLOC* cVpartitionloc;
		CoreSmph* cVmutex;
		PrtoLmtb cVmutable;

		if (bGlobalmutex || CoreAlog::sCsystemnamespace == cQuery->mutable_cvmutable ()->svnamespace ()) {
			cVpartitionloc = NULL;
			iVpartitiontype = CorePart::iCnopartition;
			cVmutex = &cVglobalmutex;
		} else {
			cVmutable.CopyFrom (*cQuery->mutable_cvmutable ());
			bVindex = CoreAlog::ix2tb (&cVmutable);
			cVglobalmutex.locks ();			
			cVtableloc = gettl (&cVmutable);
			cVglobalmutex.relse ();
			if (cVtableloc == NULL  || !cVtableloc->bVvalid) return false;
			cVpartitionloc = &cVtableloc->cVpartitionloc;
			iVpartitiontype = bVindex ? cVpartitionloc->cVpartitionindex.iVpartitiontype : cVpartitionloc->cVpartition.iVpartitiontype;
			cVmutex = &cVtableloc->cVlocalmutex;
		}
		cVmutex->locks ();
		bVreturn = cVpeer->query (	cQuery->mutable_cvmutable (), cQuery->mutable_cvkey (), 
			cValueresult, (CoreTble::iCquerytype) cQuery->ivquery (), 
			iVpartitiontype, cVpartitionloc, NULL, cVmutex, true);
		if (bVreturn && cQuery->bvdigest ()) cValueresult->set_svopaquevalue (md5 (cValueresult->svopaquevalue ()));
		cVmutex->relse ();
		return bVreturn;
	}

	/*LauNCH*/
	bool __fastcall CorePlan::lnch_ (PrtoLmtb* cMutable, POINTERFUNCPROTOMUTABLE cPointerfunc) {
		bool bVreturn;
		bool bVpartialresult;
		AnsiString sVindex;
		CoreList<AnsiString> cVfilenames;
		PrtoLmtb cVmutable;

		if (cMutable != NULL) {
			if (CoreAlog::index (cMutable)) return false;
			return (this->*cPointerfunc)(cMutable);
		} else {
			bVreturn = true;
			CoreHutl::readr (cVnodedata.sVtabletpath, (AnsiString) "*." + CoreXml_::sCxmlext, cVfilenames);
			for (int iV = 0; iV < cVfilenames.count (); iV++) {
				sVindex = CoreButl::left_ (*cVfilenames.get__ (iV), ".");
				cVmutable.set_svnamespace (CoreButl::left_ (sVindex, CoreAlog::sCtabletdelimiter).to_string ());
				cVmutable.set_svtable (CoreButl::right (sVindex, CoreAlog::sCtabletdelimiter).to_string ());
				bVpartialresult = (this->*cPointerfunc) (&cVmutable);
				bVreturn &= bVpartialresult;
			}
			return bVreturn;
		}
	}

	/*TOUCH*/
	bool __fastcall CorePlan::touch (PrtoLmtb* cMutable) {			
		PTR_TABLELOC cVtableloc;

		if (CoreAlog::index (cMutable)) return false;
		cVtableloc = gettl (cMutable);
		return cVtableloc != NULL && cVtableloc->bVvalid;
	}

	/*RePaiR*/
	bool __fastcall CorePlan::rpair (PrtoLmtb* cMutable) {			
		PTR_TABLELOC cVtableloc;

		if (CoreAlog::index (cMutable)) return false;
		cVtableloc = gettl (cMutable, true);
		if (cVtableloc != NULL && cVtableloc->bVvalid) {
			gettb (cMutable)->rpair ();
			return true;
		} else return false;
	}

	/*GET MaSk*/
	bool __fastcall CorePlan::getms (	int&iMask, RepeatedPtrField<PrtoL2cl>* cRepeatedselectsource, PTR_TABLELOC cTableloc, 
																		CoreList<COLUMNPOINTERLOC>* cColumnpointerpool) {
		PrtoL2cl* cVcolumninput;
		COLUMNLOC* cVcolumnloc;

		iMask = 0;
		for (int iV = 0; iV < cRepeatedselectsource->size (); iV++) {			
			cVcolumninput = cRepeatedselectsource->Mutable (iV);
			cVcolumnloc = getcl (cVcolumninput->svcolumn (), cTableloc);
			if (cVcolumnloc == NULL) continue;
			cColumnpointerpool->set__ ()->cVcolumnpointer = cVcolumnloc;
			iMask |= 1 << cVcolumnloc->iVaccessgroup;
		}		
		return true;
	}

	/*QueRY for Access Groups*/
	bool __fastcall CorePlan::qryag (	PrtoLkey* cBasekey, PrtoLmtb* cMutable, int iMask, CoreList<ACCESSGROUPLOC>* cAccessgrouppool, 
																		CoreTble::iCquerytype iQuerytype, STATSLOC* cStatsloc, 
																		CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
																		CoreAlog::TABLETLOC* cCachedtabletloc, PTR_TABLELOC cTableloc) {
		bool bVreturn;
		ACCESSGROUPLOC* cVaccessgrouploc;
		PrtoLval cVvalue;

		bVreturn = true;
		for (int iV = 0; iV < cAccessgrouppool->count (); iV++) 
			if (iMask & 1 << iV) {
				cVaccessgrouploc = cAccessgrouppool->get__ (iV);				
				cBasekey->set_svaccessgroup (cVaccessgrouploc->sVindex.to_string ());
				if (!query (cMutable, cBasekey, &cVvalue, iQuerytype, iPartitiontype, cPartitionloc, cCachedtabletloc, 
					cTableloc != NULL ? &cTableloc->cVlocalmutex : NULL)) {
					bVreturn = false;
					break;
				}
				cVaccessgrouploc->cVaccessobject.ParseFromString (cVvalue.svopaquevalue ());
				cStatsloc->iVsize += cVvalue.svopaquevalue ().length ();
			}
		return bVreturn;
	}

	/*FiLL Key Slice*/
	bool __fastcall CorePlan::fllks ( PrtoL2qr* cQuery, PrtoL2mr* cDmlresult,
																		PrtoLkey* cKey, CoreList<COLUMNPOINTERLOC>* cColumnpointerpool,
																		CoreList<ACCESSGROUPLOC>* cAccessgrouppool, int iCount, bool bOnlysecondary) {
		PrtoL2ks* cVkeyslice;
		COLUMNLOC* cVcolumnloc;
		bool bVreturn;

#ifdef iCdebugqryl2
		printf ("ins %s\n", ((AnsiString) cKey->svmain ()).prntb ().c_str ()); 
#endif

		cVkeyslice = cDmlresult->mutable_cvkeyslice ()->Add ();
		bVreturn = cDmlresult->cvkeyslice_size () == iCount;
		cVkeyslice->mutable_cvkey ()->CopyFrom (*cKey);
		if (bOnlysecondary) return bVreturn;
		cVkeyslice->mutable_cvcolumns ()->CopyFrom (*cQuery->mutable_cvselect ());
		for (int iV = 0; iV < cVkeyslice->cvcolumns_size (); iV++) {
			cVcolumnloc = cColumnpointerpool->get__ (iV)->cVcolumnpointer;
			cVkeyslice->mutable_cvcolumns (iV)->CopyFrom (
					*cAccessgrouppool->get__ (cVcolumnloc->iVaccessgroup)->
					cVaccessobject.mutable_cvcolumns (cVcolumnloc->iVaccessobjectitem));
		}
		return bVreturn;
	}

	/*QUERY*/
	bool __fastcall CorePlan::qryl1 (PrtoL2qr* cQuery, PrtoL2mr* cDmlresult, bool bOnlysecondary, PTR_TABLELOC cTablelocpointer, bool bLocal) {
		int iVquerysize;
		int iVmask;
		int iVcount;
		bool bVinclusive;
		bool bVkeyend;
		bool bVeof;
		bool bVwrap;
		AnsiString sVaccessgroup;
		AnsiString sVkeyend;
		AnsiString sVkeycurrent;
		PrtoLval cVvalue;
		PrtoLkey* cVkey;
		PrtoLmtb* cVmutable;
		PTR_TABLELOC cVtableloc;
		CoreTble::iCquerytype iVquerytype;
		CoreTble::iCquerytype iVoriginalquerytype;
		CorePart::PARTITIONLOC* cVpartitionloc;
		CorePart::iCpartitiontype iVpartitiontype;

		bVeof = false;
		cDmlresult->mutable_cvkeyslice ()->Clear ();
		iVquerysize = cQuery->cvselect_size ();
		cVmutable = cQuery->mutable_cvmutable ();
		cVkey = cQuery->mutable_cvkey_start ();		
		
		cVtableloc = cTablelocpointer;		
		cVpartitionloc = &cVtableloc->cVpartitionloc;
		iVpartitiontype = bLocal ? CorePart::iCnopartition : CorePart::getpt (cVpartitionloc, CoreAlog::index (cVmutable));
		
		switch (cQuery->ivquery ()) {
			case EXACTQUERY:
				iVoriginalquerytype = CoreTble::iCexactquery;
				bVkeyend = false;
				iVcount = 1;
				break;
			case RANGEQUERY:
				iVoriginalquerytype = CoreTble::iCrangequery;
				bVinclusive = true;
				bVwrap = false;
				sVkeyend = cQuery->mutable_cvkey_end ()->svmain ();
				bVkeyend = sVkeyend != "";
				iVcount = cQuery->ivcount ();
				break;
			case TOKENQUERY:
			case TKSLWQUERY:
				iVoriginalquerytype = (CoreTble::iCquerytype) cQuery->ivquery ();
				bVinclusive = false;
				sVkeyend = cQuery->mutable_cvkey_end ()->svmain ();
				bVkeyend = sVkeyend != "";
				bVwrap = bVkeyend && sVkeyend <= cQuery->mutable_cvkey_start ()->svmain ();
				iVcount = cQuery->ivcount ();
				break;
		}
		iVquerytype = iVoriginalquerytype;
		cVtableloc->cVcolumnpointerpoolql1.clear ();
		getms (iVmask, cQuery->mutable_cvselect (), cVtableloc, &cVtableloc->cVcolumnpointerpoolql1);
		while (iVcount > 0) {
			bVeof = !qryag (cVkey, cVmutable, iVmask, &cVtableloc->cVaccessgrouppool, iVquerytype, 
				&cVtableloc->cVstatsloc [iCreadstats], iVpartitiontype, cVpartitionloc, cTablelocpointer->cVcachedlocaltablet, cTablelocpointer);			
			if (bVeof) {	
				if (bVkeyend && bVwrap) {
					cVkey->set_svmain ("");
					bVwrap = false;
					bVeof = false;
					continue;
				} else break;
			}
			if (bVkeyend) {
				sVkeycurrent = cVkey->svmain ();
				if (sVkeycurrent > sVkeyend && !bVwrap) break;
				if (!bVinclusive && sVkeycurrent == sVkeyend) break;
			}
			fllks (cQuery, cDmlresult, cVkey, &cVtableloc->cVcolumnpointerpoolql1, &cVtableloc->cVaccessgrouppool, -1, bOnlysecondary);
			iVquerytype = CoreTble::iCtokenquery;
			iVcount--;
		}

		return cDmlresult->cvkeyslice_size () != 0;
	}

	/*GET Index Loc*/
	bool __fastcall CorePlan::getil (	PTR_TABLELOC cTableloc, PrtoL2wh* cWhere, INDEXLOC** cReturnindexloc, 
																		CoreTble::iCquerytype& iQuerytype) {
		AnsiString sVcolumn1;
		AnsiString sVcolumn2;
		bool bVtemp;
		bool bVmatcheddirection;
		int iVmatchedconditions;
		int iVtomatchconditions;
		int iVmatchedorders;
		int iVtomatchorders;
		int iVwhereconditions;
		int iVcurrentcolumn;
		PrtoL2cl* cVinputcolumn;
		PrtoL2or* cVinputorderby;
		INDEXCOLUMNLOC* cVindexcolumnloc;

		iQuerytype = CoreTble::iCrangequery;
		for (int iV = 0; iV < cTableloc->cVindexpool.count (); iV++) {
			*cReturnindexloc = cTableloc->cVindexpool.get__ (iV);
			iVmatchedconditions = 0;
			iVtomatchconditions = (*cReturnindexloc)->cVindexcolumnpool.count ();
			iVwhereconditions = cWhere->cvcondition_size ();
			if (iVwhereconditions > iVtomatchconditions) continue;
			for (int iVinner = 0; iVinner < iVtomatchconditions; iVinner++) {
				cVindexcolumnloc = (*cReturnindexloc)->cVindexcolumnpool.get__ (iVinner);
				sVcolumn1 = cVindexcolumnloc->cVcolumnloc->sVindex;			
				if (iVwhereconditions > iVinner) {
					cVinputcolumn = cWhere->mutable_cvcondition (iVinner);
					sVcolumn2 = cVinputcolumn->svcolumn ();		
					switch (cVinputcolumn->ivconditiontype ()) {
						case GECOND: bVmatcheddirection = cVindexcolumnloc->bVasc; break;
						case GCOND: 
							bVmatcheddirection = cVindexcolumnloc->bVasc; 
							iQuerytype = CoreTble::iCtokenquery;
							break;
						case LECOND: bVmatcheddirection = !cVindexcolumnloc->bVasc; break;
						case LCOND: 
							bVmatcheddirection = !cVindexcolumnloc->bVasc; 
							iQuerytype = CoreTble::iCtokenquery;							
							break;
						default: bVmatcheddirection = true; break;
					}
					if (!bVmatcheddirection || sVcolumn1 != sVcolumn2) break;
				} else break;
				iVmatchedconditions++;
			}
			if (iVmatchedconditions == iVtomatchconditions) {
				iVcurrentcolumn = 0;
				iVmatchedorders = 0;
				iVtomatchorders = cWhere->cvorderby_size ();
				for (int iVinner = 0; iVinner < iVtomatchorders; iVinner++) {
					cVinputorderby = cWhere->mutable_cvorderby (iVinner);
					sVcolumn2 = cVinputorderby->svcolumn ();
					do {
						cVindexcolumnloc = (*cReturnindexloc)->cVindexcolumnpool.get__ (iVcurrentcolumn);
						sVcolumn1 =  cVindexcolumnloc->cVcolumnloc->sVindex;
						iVcurrentcolumn++;
						bVtemp = sVcolumn2 == sVcolumn1;
					} while (!bVtemp  && iVcurrentcolumn < (*cReturnindexloc)->cVindexcolumnpool.count ());					
					if (bVtemp && cVindexcolumnloc->bVasc == (cVinputorderby->ivordertype () == ASC)) iVmatchedorders++;
					else break;
				}
				if (iVmatchedorders == iVtomatchorders) return true;
			}
		}
		return false;
	}

	/*VALIDation*/
	bool __fastcall CorePlan::valid ( AnsiString sReferencefullindex, AnsiString sReturnedfullindex, 
																		PrtoL2iv* cIndexvalue, POINTERKEYLOC* cPointerkeyloc, PrtoL2wh* cWhere, 
																		bool& bForkeyslice) {
		bool bVreturn;
		int iVindexposition;
		int iVindexlength;
		AnsiString sVreturnedindex;
		AnsiString sVreferenceindex;
		PrtoL2iv* cVindexvaluereference;

		bVreturn = true;
		bForkeyslice = true;
		sVreturnedindex = sReturnedfullindex.SubString (0, cIndexvalue->ivkeydelimiters (0));
		cVindexvaluereference = &cPointerkeyloc->cVkeyloc->cVindexvalue;
		sVreferenceindex = sReferencefullindex.SubString (0, cVindexvaluereference->ivkeydelimiters (0)); 
		if (sVreturnedindex != sVreferenceindex) return false;
		for (int iV = 1; iV < cIndexvalue->ivkeydelimiters_size (); iV++) {	
			iVindexposition = cIndexvalue->ivkeydelimiters (iV - 1) + 2;
			iVindexlength = cIndexvalue->ivkeydelimiters (iV) + 1 - iVindexposition; 
			sVreturnedindex = sReturnedfullindex.SubString (iVindexposition, iVindexlength);
			cVindexvaluereference = &cPointerkeyloc->cVkeyloc->cVindexvalue;
			iVindexposition = cVindexvaluereference->ivkeydelimiters (iV - 1) + 2;
			iVindexlength = cVindexvaluereference->ivkeydelimiters (iV) + 1 - iVindexposition; 
			sVreferenceindex = sReferencefullindex.SubString (iVindexposition, iVindexlength);

			switch (cWhere->cvcondition (iV - 1).ivconditiontype ()) {
				case ECOND: bVreturn &= sVreturnedindex == sVreferenceindex; break;
				case LECOND:
				case GECOND: 
					bVreturn &= sVreturnedindex >= sVreferenceindex; 
					break;
				case LCOND:
				case GCOND: 
					bVreturn &= sVreturnedindex >= sVreferenceindex; 
					bForkeyslice &= sVreturnedindex > sVreferenceindex; 	
					break;
				default: bVreturn &= false; break;
			}		

#ifdef iCdebugvalidation
			printf ("%i ret %s > ref %s, result %i, %i, condition %i\n", 
				iV,
				sVreturnedindex.prntb ().c_str (), 
				sVreferenceindex.prntb ().c_str (),  
				bVreturn, bForkeyslice, sVreturnedindex > sVreferenceindex);
#endif

		}


#ifdef iCdebugvalidation
		printf ("ret %s > ref %s, result %i, %i\n", 
			sReturnedfullindex.prntb ().c_str (), 
			sReferencefullindex.prntb ().c_str (),  
			bVreturn, bForkeyslice);

#endif
		return bVreturn;
	}

	/*GET Sorted Keys*/
	bool __fastcall CorePlan::getsk (	CoreList<KEYLOC>* cKeys, CoreList<POINTERKEYLOC>* cSortedkeys, 
																		RepeatedPtrField<PrtoL2cl>* cRepeatedcondition) {
		KEYLOC* cVkeyloc;

		for (int iV = 0; iV < cKeys->count (); iV++) {
			cVkeyloc = cKeys->get__ (iV);
			cSortedkeys->bnset (
				cVkeyloc->sVmain.SubString (0, cVkeyloc->cVindexvalue.ivkeydelimiters (cRepeatedcondition->size ()))
			)->cVkeyloc = cVkeyloc;
		}
		return true;
	}

	/*QUERY*/
	bool __fastcall CorePlan::qryl2 (	PrtoL2qr* cQuery, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer,
																		bool bTeleport, bool bIndexloc, bool bIndexkey, 
																		INDEXLOC* cIndexloc, CoreTble::iCquerytype iQuerytype,
																		CoreList<POINTERKEYLOC>* cSortedkeys) {
		AnsiString sVreturnedfullindex;
		AnsiString sVreferencefullindex;
		AnsiString sViteratorfullindex;
		AnsiString sVfirstordercolumn;
		int iVmask;
		int iVcount;
		int iVsortedkeyscount;
		int iVsortedkeyreference;
		int iVdelimitertruncation;
		bool bVorderbyquery;
		bool bVforkeyslice;
		bool bVonlysecondary;
		bool bVsupercache;
		bool bVindexloc;
		bool bVindexkey;
		PrtoLmtb cVmutableitable;
		PrtoL2iv cVindexvalue;
		PrtoLkey cVkey;
		PrtoLval cVvalue;
		PrtoLmtb* cVmutable;
		PrtoL2wh* cVwhere;
		PrtoLkey cVkeyitable;
		PrtoLval cVvalueitable;
		PTR_TABLELOC cVtableloc;
		INDEXLOC* cVindexloc;
		POINTERKEYLOC* cVpointerkeyloc;
		ORDERBYKEYLOC* cVorderbykeyloc;
		CoreList<POINTERKEYLOC>* cVpointersortedkeys;
		CoreTble::iCquerytype iVquerytype;
		CorePart::PARTITIONLOC* cVpartitionloc;
		CorePart::iCpartitiontype iVtablepartitiontype;
		CorePart::iCpartitiontype iVitablepartitiontype;

#ifdef iCdebugqryl2
		printf ("query L2");
#endif

		iVcount = cQuery->ivcount ();
		cDmlresult->mutable_cvkeyslice ()->Clear ();
		cVmutable = cQuery->mutable_cvmutable ();
		cVwhere = cQuery->mutable_cvwhere ();
		bVorderbyquery = cVwhere->cvorderby_size () > 0;
		bVonlysecondary = cQuery->bvonlysecondary ();
		
		cVtableloc = cTablelocpointer;
		cVpartitionloc = &cVtableloc->cVpartitionloc;
		iVtablepartitiontype = CorePart::getpt (cVpartitionloc, false);
		iVitablepartitiontype = CorePart::getpt (cVpartitionloc, true);

		cVmutableitable.CopyFrom (*cVmutable);
		CoreAlog::tb2ix (&cVmutableitable);
		cVtableloc->cVcolumnpointerpoolql2.clear ();
		getms (iVmask, cQuery->mutable_cvselect (), cVtableloc, &cVtableloc->cVcolumnpointerpoolql2);
		if (bTeleport) {
			cVindexloc = cIndexloc;
			bVindexloc = bIndexloc;
			iVquerytype = iQuerytype;
		
		} else bVindexloc = getil (cVtableloc, cVwhere, &cVindexloc, iVquerytype);
		if (bVorderbyquery) cVtableloc->cVorderbykeysql2.clear ();
		if (bVindexloc) {

#ifdef iCdebugqryl2
			printf ("index: %s\n", cVindexloc->sVindex.c_str ());
#endif
			if (bTeleport) {
				bVindexkey = bIndexkey;
				cVpointersortedkeys = cSortedkeys;
			} else {
				cVtableloc->cVstatickeysql2.clear ();
				bVindexkey = getik (cVindexloc, cVwhere->mutable_cvcondition (), false, 0, UPSERT, &cVtableloc->cVstatickeysql2, false, NULL);
				cVtableloc->cVstaticsortedkeysql2.clear ();
				cVpointersortedkeys = &cVtableloc->cVstaticsortedkeysql2;
				if (bVindexkey) getsk (&cVtableloc->cVstatickeysql2, cVpointersortedkeys, cVwhere->mutable_cvcondition ());
			}
			if (bVindexkey) {
				sVreturnedfullindex = "";
				iVsortedkeyscount = cVpointersortedkeys->count ();
				bVorderbyquery &= iVsortedkeyscount > 1;
				if (bVorderbyquery) {					
					sVfirstordercolumn = cVwhere->mutable_cvorderby (0)->svcolumn ();
					iVdelimitertruncation = 0;
					for (int iV = 0; iV < cVindexloc->cVindexcolumnpool.count (); iV++) 
						if (cVindexloc->cVindexcolumnpool.get__ (iV)->cVcolumnloc->sVindex == sVfirstordercolumn) 
							iVdelimitertruncation = iV;
				}
				bVsupercache = false;
				for (int iV = 0; iV < iVsortedkeyscount; iV++) {					
					cVpointerkeyloc = cVpointersortedkeys->get__ (iV);
					sVreferencefullindex = cVpointerkeyloc->sVindex;	

#ifdef iCdebugqryl2
					printf ("\nref %s\n", sVreferencefullindex.prntb ().c_str ());
					printf ("ret %s\n", sVreturnedfullindex.prntb ().c_str ());
					printf ("ju? %i\n", sVreferencefullindex < sVreturnedfullindex);
#endif

					if (bVsupercache && sVreferencefullindex > sVreturnedfullindex) bVsupercache = false;
					if (!bVsupercache) {
						if (sVreferencefullindex < sVreturnedfullindex) continue;								
						cVkeyitable.set_svmain ((sVreferencefullindex + CoreButl::chr2s (0, 1)).to_string ());	
					};

#ifdef iCdebugqryl2
					printf ("qr? %i\n", !bVsupercache);
#endif

					if (bVsupercache || query (	&cVmutableitable, &cVkeyitable, &cVvalueitable, iVquerytype, 
																			iVitablepartitiontype, cVpartitionloc, cTablelocpointer->cVcachedlocalindextablet, 
																			&cTablelocpointer->cVlocalmutex)) {		
						do {
							if (bVsupercache) bVsupercache = false;
							else {
								cVindexvalue.ParseFromString (cVvalueitable.svopaquevalue ());
								sViteratorfullindex = sVreturnedfullindex = cVkeyitable.svmain (); 
								sVreturnedfullindex = sVreturnedfullindex.SubString (0, 
									cVindexvalue.ivkeydelimiters (cVindexvalue.ivkeydelimiters_size () - 1)); 
							}

#ifdef iCdebugqryl2
							printf ("ok? %s\n", sVreturnedfullindex.prntb ().c_str ()); 
#endif

							if (valid (	sVreferencefullindex, sVreturnedfullindex, &cVindexvalue, 
													cVpointerkeyloc, cVwhere, bVforkeyslice)) {		

#ifdef iCdebugqryl2
								printf ("ok! \n"); 
#endif

								if (bVforkeyslice) {
									cVkey.set_svmain (cVindexvalue.svoriginalkey ());
									if (bVorderbyquery) {			
										cVorderbykeyloc = cVtableloc->cVorderbykeysql2.bnset (
											CoreButl::sbstr (sVreturnedfullindex, cVindexvalue.ivkeydelimiters (iVdelimitertruncation) + 1));										
										cVorderbykeyloc->sVreferencefullindex = sVreferencefullindex;
										cVorderbykeyloc->sVoriginalindex = cVindexvalue.svoriginalkey ();

#ifdef iCdebugqryl2
										printf ("o-> *%s* *%s*, k-> *%s*\n", 
											cVorderbykeyloc->sVoriginalindex.prntb ().c_str (),
											sVreturnedfullindex.prntb ().c_str (),
											CoreButl::sbstr (sVreturnedfullindex, 
												cVindexvalue.ivkeydelimiters (iVdelimitertruncation) + 1).prntb ().c_str ()
											); 
#endif

										cVorderbykeyloc->sViteratorindex = sViteratorfullindex; 
										cVorderbykeyloc->iVsortedkeyreference = iV;
										break;
									} else {
										if (bVonlysecondary || qryag (&cVkey, cVmutable, iVmask, &cVtableloc->cVaccessgrouppool, 
																									CoreTble::iCexactquery, &cVtableloc->cVstatsloc [iCreadstats],
																									iVtablepartitiontype, cVpartitionloc, cVtableloc->cVcachedlocaltablet, cVtableloc)) {
											if (fllks (	cQuery, cDmlresult, &cVkey, &cVtableloc->cVcolumnpointerpoolql2, 
																	&cVtableloc->cVaccessgrouppool, iVcount, bVonlysecondary)) 																		
																		return true;											
										}
									}
								}
							} else {
								bVsupercache = true;

#ifdef iCdebugqryl2
								printf ("sup\n");
#endif

								break;
							}
						} while (query (	&cVmutableitable, &cVkeyitable, &cVvalueitable, CoreTble::iCtokenquery, 
							iVitablepartitiontype, cVpartitionloc, cVtableloc->cVcachedlocalindextablet, &cVtableloc->cVlocalmutex));
					}
				}
				if (bVorderbyquery) {

#ifdef iCdebugqryl2
					printf ("ord ->>>>>>>>>>>>>>>>\n"); 
#endif

					while (cVtableloc->cVorderbykeysql2.count () > 0) {
						cVorderbykeyloc = cVtableloc->cVorderbykeysql2.get__ (0);				
						cVkey.set_svmain (cVorderbykeyloc->sVoriginalindex.to_string ());
						sVreferencefullindex = cVorderbykeyloc->sVreferencefullindex;
						iVsortedkeyreference = cVorderbykeyloc->iVsortedkeyreference;
						cVpointerkeyloc = cVpointersortedkeys->get__ (iVsortedkeyreference);
						cVkeyitable.set_svmain (cVorderbykeyloc->sViteratorindex.to_string ());						
						cVtableloc->cVorderbykeysql2.unset (0);
						if (bVonlysecondary || qryag (&cVkey, cVmutable, iVmask, &cVtableloc->cVaccessgrouppool, 
																					CoreTble::iCexactquery, &cVtableloc->cVstatsloc [iCreadstats], 
																					iVtablepartitiontype, cVpartitionloc, cVtableloc->cVcachedlocaltablet, cVtableloc)) {
							if (fllks (	cQuery, cDmlresult, &cVkey, &cVtableloc->cVcolumnpointerpoolql2, 
													&cVtableloc->cVaccessgrouppool, iVcount, bVonlysecondary)) 						
							return true;
						}							
						if (query (	&cVmutableitable, &cVkeyitable, &cVvalueitable, CoreTble::iCtokenquery, 
							iVitablepartitiontype, cVpartitionloc, cVtableloc->cVcachedlocalindextablet, &cTablelocpointer->cVlocalmutex)) {
							cVindexvalue.ParseFromString (cVvalueitable.svopaquevalue ());
							sVreturnedfullindex = cVkeyitable.svmain (); 

#ifdef iCdebugqryl2
							printf ("\nref %s\n", sVreferencefullindex.prntb ().c_str ());
							printf ("ok2? %s\n", sVreturnedfullindex.prntb ().c_str ()); 
#endif

							if (valid (	sVreferencefullindex, sVreturnedfullindex, &cVindexvalue, 
													cVpointerkeyloc, cVwhere, bVforkeyslice)) {

#ifdef iCdebugqryl2
								printf ("ok2! \n"); 
#endif

								if (bVforkeyslice) {									
									cVorderbykeyloc = cVtableloc->cVorderbykeysql2.bnset (
											CoreButl::sbstr (sVreturnedfullindex, cVindexvalue.ivkeydelimiters (iVdelimitertruncation) + 1));	
									cVorderbykeyloc->sVreferencefullindex = sVreferencefullindex;
									cVorderbykeyloc->sVoriginalindex = cVindexvalue.svoriginalkey ();

#ifdef iCdebugqryl2
									printf ("o2-> *%s* *%s*\n", 
											cVorderbykeyloc->sVoriginalindex.prntb ().c_str (),
											sVreturnedfullindex.prntb ().c_str ()); 
#endif

									cVorderbykeyloc->sViteratorindex = sVreturnedfullindex;
									cVorderbykeyloc->iVsortedkeyreference = iVsortedkeyreference;
								}
							}
						}
					}					
				}
			}
		} else {
			PrtoL2cl* cVconditioncolumn;
			PrtoL2or* cVorderbycolumn;
			bool bVasc;

			if (cQuery->bvfulltablescan ()) return false;
			printf ("MISSING INDEX - %s ", cVtableloc->sVindex.c_str ());
			for (int iV = 0; iV < cVwhere->cvcondition_size (); iV++) {
				cVconditioncolumn = cVwhere->mutable_cvcondition (iV);
				switch (cVconditioncolumn->ivconditiontype ()) {
					case LCOND:
					case LECOND:
						bVasc = false;
					default:
						bVasc = true;
				}
				printf ("- %s %i -", cVconditioncolumn->svcolumn ().c_str (), bVasc); 
			}
			for (int iV = 0; iV < cVwhere->cvorderby_size (); iV++) {
				cVorderbycolumn = cVwhere->mutable_cvorderby (iV);
				printf ("- %s %i -", cVorderbycolumn ->svcolumn ().c_str (), cVorderbycolumn->ivordertype () == ASC); 
			}
			printf ("\n");
		}
		return true;
	}

	/*QueRy L2 Teleported*/
	bool __fastcall CorePlan::qrl2t (PrtoL2qr* cQuery, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer) {
		bool bVindexloc;
		bool bVindexkey;
		PrtoLmtb cVmutableitable;
		PrtoL2wh* cVwhere;
		PTR_TABLELOC cVtableloc;
		INDEXLOC* cVindexloc;
		CoreList<KEYLOC> cVkeys;
		CoreList<POINTERKEYLOC> cVsortedkeys;
		CoreTble::iCquerytype iVquerytype;
		CoreEnpo::GOSSIPLOC cVgossiploc;
		bool bVreturn;

		cVwhere = cQuery->mutable_cvwhere ();
		cVtableloc = cTablelocpointer;
		bVindexloc = getil (cVtableloc, cVwhere, &cVindexloc, iVquerytype); 
		bVindexkey = false;
		if (bVindexloc) {
			bVindexkey = getik (cVindexloc, cVwhere->mutable_cvcondition (), false, 0, UPSERT, &cVkeys, false, NULL);
			if (bVindexkey) {
				getsk (&cVkeys, &cVsortedkeys, cVwhere->mutable_cvcondition ());
				if ((!cQuery->bvdisableteleport ()) && (cVsortedkeys.count () > 0)) {
					cVmutableitable.CopyFrom (*cQuery->mutable_cvmutable ());
					CoreAlog::tb2ix (&cVmutableitable);
					if (cVpeer->tlprt (	&cVmutableitable, (cVsortedkeys.get__ (0)->sVindex + CoreButl::chr2s (0, 1)),
						cVtableloc->cVpartitionloc.cVpartitionindex.iVpartitiontype, &cVtableloc->cVpartitionloc, cVgossiploc)) {
						cTablelocpointer->cVlocalmutex.relse ();
						bVreturn = CoreTutl::sendq (cVgossiploc.cVconnectionfactory, cQuery, true, cDmlresult);
						cTablelocpointer->cVlocalmutex.locks ();
						return bVreturn;
					}
				}
			}
		}
		return qryl2 (	cQuery, cDmlresult, cTablelocpointer, true, bVindexloc, bVindexkey, 
										cVindexloc, iVquerytype, &cVsortedkeys);
	}

	/*QUERY*/
	bool __fastcall CorePlan::query (PrtoL2qr* cQuery, PrtoL2mr* cDmlresult) {
		bool bVreturn;
		TDateTime tVtimestampstart;
		PTR_TABLELOC cVtableloc;

		tVtimestampstart = __Time ();
		cVglobalmutex.locks ();
		cVtableloc = gettl (cQuery->mutable_cvmutable ());
		cVglobalmutex.relse ();
		if (cVtableloc == NULL  || !cVtableloc->bVvalid) return false;
		cVtableloc->cVlocalmutex.locks ();
		if (cQuery->has_cvwhere ()) bVreturn = QRYL2 (cQuery, cDmlresult, cVtableloc);
		else bVreturn = qryl1 (cQuery, cDmlresult, false, cVtableloc);
		if (bVreturn) stats (cVtableloc, tVtimestampstart, iCreadstats, 1);
		cVtableloc->cVlocalmutex.relse ();
		return bVreturn;
	}

	/*GET PeeR*/
	PTR_PEER __fastcall CorePlan::getpr() {

		return cVpeer;
	}

	/*GET Seda Manager*/
	PTR_SEDAMANAGER __fastcall CorePlan::getsm () {

		return cVsedamanager;
	}

	/*LOAD Table*/
	bool __fastcall CorePlan::loadt (PrtoLmtb* cMutable, PTR_TABLELOC cTableloc, bool bRepair, AnsiString sXml) {
		CoreXml_ cVdocument;
		AnsiString sVcontent;
		AnsiString sVxmlfilename;
		AnsiString sVtabletfilename;
		AnsiString sVcolumnname;
		AnsiString sVaccessgroupname;
		AnsiString sVindexname;
		AnsiString sVcolumntype;
		int iV;
		int iVtabletype;
		int iVindextype;
		int iVaccessgroup;
		bool bVindex;
		bool bVloadfromdb;
		CoreNode* cVtablenode;
		CoreNode* cVcolumnnode;
		CoreNode* cVaccessgroupnode;
		CoreNode* cVindexnode;
		CoreNode* cVstoragenode;
		TList* cVtablechildrennodes;
		TList* cVaccessgroupchildrennodes;
		TList* cVcolumnchildrennodes;
		COLUMNLOC* cVcolumnloc;
		ACCESSGROUPLOC* cVaccessgrouploc;
		ACCESSGROUPLOC* cVprevaccessgrouploc;
		INDEXLOC* cVindexloc;
		INDEXCOLUMNLOC* cVindexcolumnloc;
		INDEXPOINTERLOC* cVindexpointerloc;
		PrtoLmtb cVmutableitable;
		PrtoTabl cVtablet;
		
		bVindex = false;		
		sVxmlfilename = getxn (cMutable);
		sVtabletfilename = gettn (cMutable);		
	
		LOG(INFO) << "Table Ok?: " << sVtabletfilename.c_str ();		
		FlushLogFiles(GLOG_INFO);
		if (ldtbl (sVtabletfilename, &cVtablet)) {
			sVcontent = cVtablet.svxml ();
			cVpartitioner->pbtpp (cVtablet.mutable_cvpartitionedelements (), 
															&cTableloc->cVpartitionloc.cVpartition, 
															cTableloc->cVpartitionloc.iVcustomreplicationfactor);
			cVpartitioner->pbtpp (cVtablet.mutable_cvpartitionedindexelements (), 
															&cTableloc->cVpartitionloc.cVpartitionindex, 
															cTableloc->cVpartitionloc.iVcustomreplicationfactor);
			bVloadfromdb = true;
		} else {
			sVcontent = sXml == NULL ? CoreButl::getff (cVpool, sVxmlfilename) : sXml;
			bVloadfromdb = false;		
		}
		if (sVcontent != "") {
			cTableloc->bVvalid = true;
			cTableloc->cVstorage.set_ivmemtabledim (-1);
			cTableloc->cVstorage.set_ivmemtablesize (-1);
			cTableloc->cVstorage.set_ivmemtablechunk (-1);
			cTableloc->cVaccessgrouppool.set__ ()->sVindex = sCdefaultaccessgroup;
			cVdocument.getfc (sVcontent);
			cVtablenode = cVdocument.getrt ()->getcd (DataXml_::sCtagtable_);
			cVtablechildrennodes = cVtablenode->getcn ();
			for (iV = 0; iV < cVtablechildrennodes->Count; iV++) {
				cVcolumnnode = (CoreNode*) cVtablechildrennodes->Items [iV];
				if (cVcolumnnode->gettx () == DataXml_::sCtagcolumn) {
					sVcolumnname = cVcolumnnode->getls (DataXml_::sCtagname__);
					sVcolumntype = cVcolumnnode->getls (DataXml_::sCtagcoltyp);
					cVcolumnloc = getcl (sVcolumnname, cTableloc);
					if (cVcolumnloc == NULL) continue;
					cVcolumnloc->iVtype = CoreButl::getci (sVcolumntype, DataXml_::sCvalcoltps, DataXml_::iCvalcoltps);
				}
			}
			for (iV = 0; iV < cVtablechildrennodes->Count; iV++) {
				cVaccessgroupnode = (CoreNode*) cVtablechildrennodes->Items [iV];
				if (cVaccessgroupnode->gettx () == DataXml_::sCtagaccsgr) {
					sVaccessgroupname = cVaccessgroupnode->getls (DataXml_::sCtagname__); 	
					cVaccessgrouploc = getal (sVaccessgroupname, cTableloc);
					cVcolumnnode = cVaccessgroupnode->getcd (DataXml_::sCtagcolumn);
					cVaccessgroupchildrennodes = cVcolumnnode->getcn ();
					iVaccessgroup = getai (cVaccessgrouploc, cTableloc);
					for (int iVinner = 0; iVinner < cVaccessgroupchildrennodes->Count; iVinner++) {
						cVcolumnnode = (CoreNode*) cVaccessgroupchildrennodes->Items [iVinner];
						if (cVcolumnnode->gettx () == DataXml_::sCtagname__) {
							sVcolumnname = cVcolumnnode->getcd ()->gettx ();
							cVcolumnloc = getcl (sVcolumnname, cTableloc);
							if (cVcolumnloc == NULL) continue;
							cVcolumnloc->iVaccessgroup = iVaccessgroup;
							cVaccessgrouploc->cVaccessobject.add_cvcolumns ();
							cVcolumnloc->iVaccessobjectitem = cVaccessgrouploc->cVaccessobject.cvcolumns_size () - 1;
						}
					}
				}
			}
			cVaccessgrouploc = cTableloc->cVaccessgrouppool.get__ (0);
			for (iV = 0; iV < cTableloc->cVcolumnpool.count (); iV++) {
				cVcolumnloc = cTableloc->cVcolumnpool.get__ (iV);
				if (cVcolumnloc->iVaccessgroup == 0) {
					cVaccessgrouploc->cVaccessobject.add_cvcolumns ()->set_svcolumn (cVcolumnloc->sVindex.to_string ());
					cVcolumnloc->iVaccessobjectitem = cVaccessgrouploc->cVaccessobject.cvcolumns_size () - 1;
				}
			}
			for (iV = 0; iV < cVtablechildrennodes->Count; iV++) {
				cVindexnode = (CoreNode*) cVtablechildrennodes->Items [iV];
				if (cVindexnode->gettx () == DataXml_::sCtagindex_) {
					sVindexname = cVindexnode->getls (DataXml_::sCtagname__); 
					cVindexloc = getil (sVindexname, cTableloc);
					if (cVindexloc == NULL) continue;
					cVcolumnchildrennodes = cVindexnode->getcn ();
					for (int iVinner = 0; iVinner < cVcolumnchildrennodes->Count; iVinner++) {
						cVcolumnnode = (CoreNode*) cVcolumnchildrennodes->Items [iVinner];
						cVprevaccessgrouploc = NULL;
						if (cVcolumnnode->gettx () == DataXml_::sCtagcolumn) {
							sVcolumnname = cVcolumnnode->getls (DataXml_::sCtagname__);
							cVcolumnloc = getcl (sVcolumnname, cTableloc);
							if (cVcolumnloc == NULL) continue;
							bVindex = true;
							cVindexcolumnloc = cVindexloc->cVindexcolumnpool.set__ ();
							cVindexcolumnloc->cVcolumnloc = cVcolumnloc;
							cVindexcolumnloc->bVasc = cVcolumnnode->getls (DataXml_::sCtagorder_) == DataXml_::sCvalascend;
							cVaccessgrouploc = cTableloc->cVaccessgrouppool.get__ (cVcolumnloc->iVaccessgroup);
							if (cVprevaccessgrouploc != NULL && cVaccessgrouploc != cVprevaccessgrouploc) return false;
							cVprevaccessgrouploc = cVaccessgrouploc;
						}
					}
					cVindexpointerloc = cVaccessgrouploc->cVindexpointerpool.set__ ();
					cVindexpointerloc->cVindexloc = cVindexloc;
				}
			}

			for (iV = 0; iV < cVtablechildrennodes->Count; iV++) {
				cVstoragenode = (CoreNode*) cVtablechildrennodes->Items [iV];
				if (cVstoragenode->gettx () == DataXml_::sCtagstorge) {
					cTableloc->cVstorage.set_ivmemtabledim (cVstoragenode->getli (DataXml_::sCtagmemdim, true, 0));
					cTableloc->cVstorage.set_ivmemtablesize (cVstoragenode->getli (DataXml_::sCtagmemsze, true, 0));
					cTableloc->cVstorage.set_ivmemtablechunk (cVstoragenode->getli (DataXml_::sCtagmemchk, true, 0));
					iVtabletype = cVstoragenode->getli (DataXml_::sCtagmemtab, true, 
						cVstoragenode->getli (DataXml_::sCtagtbtype, true, 0));
					iVindextype = cVstoragenode->getli (DataXml_::sCtagtitype, true, 0);
					cTableloc->cVstorage.set_bvmemorytable (iVtabletype == iCmemorytable || iVtabletype == iCvolatiletable);
					cTableloc->cVstorage.set_bvvolatiletable (iVtabletype == iCvolatiletable);
					cTableloc->cVstorage.set_bvfastkey (false);
					cTableloc->bVlocalindex = iVindextype == iClocalindex;
					cTableloc->cVstorage.set_bvlocalindex (cTableloc->bVlocalindex);
					cTableloc->cVstorage.set_svshardingfrom (cVstoragenode->getls (DataXml_::sCtagtbshfm, true, "").to_string ());
					cTableloc->cVstorage.set_svshardingto (cVstoragenode->getls (DataXml_::sCtagtbshto, true, "").to_string ());	
					cTableloc->cVstorage.set_svshardingindexfrom (cVstoragenode->getls (DataXml_::sCtagtishfm, true, "").to_string ());
					cTableloc->cVstorage.set_svshardingindexto (cVstoragenode->getls (DataXml_::sCtagtishto, true, "").to_string ());	
					cTableloc->iVpartitiontype = (iCpartitiontype) cVstoragenode->getli (DataXml_::sCtagtbpaty, true, iCnopartition);
					cTableloc->cVstorage.set_ivpartitiontype ((com::tomting::orion::iCpartitiontype) cTableloc->iVpartitiontype);
					cTableloc->cVpartitionloc.iVcustomreplicationfactor = cVstoragenode->getli (DataXml_::sCtagrepfac, true, -1);
					cTableloc->cVstorage.set_ivreplicationfactor (cTableloc->cVpartitionloc.iVcustomreplicationfactor);
				} else {
					cTableloc->cVstorage.set_bvmemorytable (true);
					cTableloc->cVstorage.set_bvvolatiletable (true);
				}
			} 
			if (cTableloc->cVaccessgrouppool.count () > sizeof (int) * 8) return false;
			cTableloc->cVstorage.set_bvrepair (bRepair);
			setsg (cMutable, cTableloc);
			if (bVindex) {
				getim (cMutable, cVmutableitable);
				setsg (&cVmutableitable, cTableloc);
			}
			if (!bVloadfromdb) cVpartitioner->setpd (&cTableloc->cVpartitionloc, NULL, NULL);
			cTableloc->cVpartitionloc.cVpartitionindex.iVpartitiontype = 
				getpt ((iCpartitiontype) cTableloc->cVstorage.ivpartitiontype (), true);
			cTableloc->cVpartitionloc.cVpartition.iVpartitiontype = 
				getpt ((iCpartitiontype) cTableloc->cVstorage.ivpartitiontype (), false);			
			cTableloc->cVcachedlocaltablet = cVredolog->gettb (cMutable, false, false);
			cTableloc->cVcachedlocalindextablet = cVredolog->gettb (&cVmutableitable, false, false);

			LOG(INFO) <<	"Xml cont:  " << sVcontent.c_str (); 
			LOG(INFO) <<	"Table Ok:  " << cTableloc->sVindex.c_str () << 
										", columns: " << cTableloc->cVcolumnpool.count () <<
										", indexes: " << cTableloc->cVindexpool.count () << 
										", access grp: " << 	cTableloc->cVaccessgrouppool.count () <<
										", load from db: " << (bVloadfromdb ? "true" : "false") <<
										", partition type: " << cTableloc->cVstorage.ivpartitiontype () <<
										", memory table: " << (cTableloc->cVstorage.bvmemorytable () ? "true" : "false") <<
										", volatile table: " << (cTableloc->cVstorage.bvvolatiletable () ? "true" : "false") <<
										", local index: " << (cTableloc->cVstorage.bvlocalindex () ? "true" : "false");
			FlushLogFiles(GLOG_INFO);
			return true;
		}
		LOG(WARNING) << "Table " << sVtabletfilename.c_str ();		
		FlushLogFiles(GLOG_WARNING);
		return false;
	}

	/*SAVE Table*/
	bool __fastcall CorePlan::savet (PrtoL2ct* cCreatetable) {
		AnsiString sVcolumnname;
		CoreXml_ cVdocument;
		CoreNode* cVnode;
		PrtoL2cl* cVcolumn;
		PrtoLmtb* cVmutable;
		CoreNode* cVtempcolumnname;
		CoreNode* cVtempcolumntype;
		CoreNode* cVtempcolumnnode;
		CoreNode* cVtempstorage;

		cVnode = cVdocument.getft (DataXml_::sCtagtable_);	
		for (int iV = 0; iV < cCreatetable->cvcolumns_size (); iV++) {
			cVcolumn = cCreatetable->mutable_cvcolumns (iV);
			sVcolumnname = cVcolumn->svcolumn ();
			cVtempcolumnname = CoreNode::newnd (new CoreNode (sVcolumnname.UpperCase ()), DataXml_::sCtagname__);
			cVtempcolumntype = CoreNode::newnd (new CoreNode (DataXml_::sCvalcoltps [cVcolumn->ivtype ()]), DataXml_::sCtagcoltyp);
			cVtempcolumnnode = new CoreNode (DataXml_::sCtagcolumn); 
			cVtempcolumnnode->addcd (cVtempcolumnname);
			cVtempcolumnnode->addcd (cVtempcolumntype);
			cVnode->addcd (cVtempcolumnnode);
		}
		cVtempstorage = new CoreNode (DataXml_::sCtagstorge);
		if (cCreatetable->has_ivtabletype ()) 
			cVtempstorage->upglf (DataXml_::sCtagtbtype, new CoreNode (cCreatetable->ivtabletype ()));
		if (cCreatetable->has_ivindextype ()) 
			cVtempstorage->upglf (DataXml_::sCtagtitype, new CoreNode (cCreatetable->ivindextype ()));			
		if (cCreatetable->has_ivpartitiontype ()) 
			cVtempstorage->upglf (DataXml_::sCtagtbpaty, new CoreNode (cCreatetable->ivpartitiontype ()));			
		if (cCreatetable->has_ivreplicationfactor ()) 
			cVtempstorage->upglf (DataXml_::sCtagrepfac, new CoreNode (cCreatetable->ivreplicationfactor ()));				
		if (cCreatetable->has_ivmemtabledim ()) 
			cVtempstorage->upglf (DataXml_::sCtagmemdim, new CoreNode (cCreatetable->ivmemtabledim ()));				

		cVnode->addcd (cVtempstorage);
		cVmutable = cCreatetable->mutable_cvmutable ();
		if (CoreButl::settf (cVpool, getxn (cVmutable), cVdocument.settc (true, true))) 
			return true;
		else return false;
	}

	/*GET Xml Name*/
	AnsiString __fastcall CorePlan::getxn (AnsiString sBasename) {
		
		return	CoreButl::ffpen (cVnodedata.sVtabletpath + sBasename  + "." + CoreXml_::sCxmlext);
	}

	/*GET Tablet Name*/
	AnsiString __fastcall CorePlan::gettn (PrtoLmtb* cMutable) {
		
		return	((AnsiString) cMutable->svnamespace () + CoreAlog::sCtabletdelimiter + 
						(AnsiString) cMutable->svtable ()).UpperCase ();
	}

	/*GET Xml Name*/
	AnsiString __fastcall CorePlan::getxn (PrtoLmtb* cMutable) {
		return getxn (gettn (cMutable));
	}

	/*CReaTe TaBle*/
	bool __fastcall CorePlan::crttb (PrtoL2ct* cCreatetable, PrtoL2st* cStatement) {
		bool bVreturn = false;
		PrtoL2ct cVcreatetable;
		PrtoL2cl* cVcolumninput;

		if (cCreatetable != NULL) cVcreatetable.CopyFrom (*cCreatetable);
		else {
			cVcreatetable.mutable_cvmutable ()->CopyFrom (*cStatement->mutable_cvmutable ());
			cVcreatetable.mutable_cvcolumns ()->CopyFrom (cStatement->cvcolumns ());
			for (int iV = 0; iV < cVcreatetable.cvcolumns_size (); iV++) {
				cVcolumninput = cVcreatetable.mutable_cvcolumns (iV);
				cVcolumninput->set_ivtype (cVcolumninput->mutable_cvvalue ()->ivtype ());
			}
		}
		cVglobalmutex.locks ();
		PTR_TABLELOC cVtableloc = gettl (cVcreatetable.mutable_cvmutable ());
		if (cVtableloc == NULL) bVreturn = savet (&cVcreatetable);
		cVglobalmutex.relse (); 
		return bVreturn;
	}

	/*CReaTe TaBle*/
	bool __fastcall CorePlan::crttb (PrtoLmtb* cMutable) {

		return setsg (cMutable);
	}

	/*SET StoraGe*/
	bool __fastcall CorePlan::setsg (PrtoLmtb* cMutable, PTR_TABLELOC cTableloc) {
		PrtoLstr* cVstorage;
		PrtoLmtb cVmutable;
		PrtoLkey cVkey;
		PrtoLval cVvalue;
		
		cVmutable.CopyFrom (*cMutable);
		cVstorage = cVmutable.mutable_cvstorage ();
		cVstorage->CopyFrom (cTableloc->cVstorage);
		return cVpeer->muttr (STORAGE, &cVmutable, &cVkey, &cVvalue, CorePart::iCnopartition, NULL, NULL, false, NULL);
	}

	/*SET StoraGe*/
	bool __fastcall CorePlan::setsg (PrtoLmtb* cMutable) {
		PrtoLkey cVkey;
		PrtoLval cVvalue;
		
		return cVpeer->muttr (STORAGE, cMutable, &cVkey, &cVvalue, CorePart::iCnopartition, NULL, NULL, false, NULL);
	}

	/*GET Table Loc*/
	CorePlan::PTR_TABLELOC __fastcall CorePlan::gettl (PrtoLmtb* cMutable, bool bRepair, AnsiString sXml) {
		AnsiString sVindex;
		PTR_TABLELOC cVtableloc;
		PTR_TABLELOC cVnulltableloc;

		sVindex = CoreAlog::gettk (cMutable);
		cVtableloc = cVtablepool.bnget (sVindex);
		if (cVtableloc == NULL) {
			cVtableloc = cVtablepool.bnset (sVindex);
			if (loadt (cMutable, cVtableloc, bRepair, sXml)) return cVtableloc;
			else {
				cVtablepool.bnuns (sVindex);
				return cVnulltableloc;
			}
		}
		return cVtableloc;
	}

	/*GET Table Loc*/
	CorePlan::PTR_TABLELOC __fastcall CorePlan::gettl (AnsiString sNextindex) {
		PTR_TABLELOC cVtableloc;
		int iVtemp;

		cVglobalmutex.locks ();
		iVtemp = cVtablepool.bnsrc (sNextindex + char (1));
		if (iVtemp < cVtablepool.count ()) cVtableloc = cVtablepool.get__ (iVtemp);
		cVglobalmutex.relse ();
		return cVtableloc;
	}

	/*GET Column Loc*/
	CorePlan::COLUMNLOC* __fastcall CorePlan::getcl (AnsiString sColumnname, PTR_TABLELOC cTableloc) {
		AnsiString sVcolumnname;
		COLUMNLOC* cVcolumnloc;

		if (sColumnname == "") return NULL;
		sVcolumnname = sColumnname.UpperCase ().Trim ();
		cVcolumnloc = cTableloc->cVcolumnpool.bnget (sVcolumnname);
		if (cVcolumnloc == NULL) return cTableloc->cVcolumnpool.bnset (sVcolumnname);
		else return cVcolumnloc;
	}

	/*GET Accessgroup Loc*/
	CorePlan::ACCESSGROUPLOC* __fastcall CorePlan::getal (AnsiString sAccessgroupname, PTR_TABLELOC cTableloc) {
		AnsiString sVaccessgroupname;
		ACCESSGROUPLOC* cVaccessgrouploc;

		sVaccessgroupname = sAccessgroupname.UpperCase ().Trim ();
		cVaccessgrouploc = cTableloc->cVaccessgrouppool.bnget (sVaccessgroupname);
		if (cVaccessgrouploc == NULL) return cTableloc->cVaccessgrouppool.bnset (sVaccessgroupname);
		else return cVaccessgrouploc;
	}

	/*GET Index Loc*/
	CorePlan::INDEXLOC* __fastcall CorePlan::getil (AnsiString sIndexname, PTR_TABLELOC cTableloc) {
		AnsiString sVindexname;
		INDEXLOC* cVindexloc;

		if (sIndexname == "") return NULL;
		sVindexname = sIndexname.UpperCase ().Trim ();
		cVindexloc = cTableloc->cVindexpool.bnget (sVindexname);
		if (cVindexloc == NULL) return cTableloc->cVindexpool.bnset (sVindexname);
		else return cVindexloc;
	}

	/*GET Accessgroup Item*/
	int __fastcall CorePlan::getai (ACCESSGROUPLOC* cAccessgroup, PTR_TABLELOC cTableloc) {
		
		for (int iV = 0; iV < cTableloc->cVaccessgrouppool.count (); iV++) 
			if (cTableloc->cVaccessgrouppool.get__ (iV) == cAccessgroup) return iV;
		return -1;
	}

	/*GET Index Mutable*/
	bool __fastcall CorePlan::getim (PrtoLmtb* cMutable, PrtoLmtb& cMutableitable) {

		cMutableitable.CopyFrom (*cMutable);
		CoreAlog::tb2ix (&cMutableitable);
		return true;
	}

	/*STATement Internal*/
	bool __fastcall CorePlan::stati (	PrtoL2st* cStatement, PTR_TABLELOC cTablelocpointer, bool bOptimizerewriteindexes, 
																		bool bUpdateonlyindexes, bool bLocalfilter) {
		int iV;
		int iVinner;
		int iVmask;
		int iVupdateinner;
		int iVaccessgroupcount;
		bool bVindexinsert;
		bool bVdeletestatement;
		bool bVindexupdate;
		AnsiString sVqualifier;
		AnsiString sVcolumn;
		PTR_TABLELOC cVtableloc;
		PrtoLmtb* cVmutable;
		PrtoLkey* cVkey;
		PrtoL2cl* cVcolumninput;
		PrtoL2cl* cVcolumnoutput;
		PrtoLkey cVupdatekey;
		PrtoLval cVvalue;
		PrtoLval cVupdatevalue;
		PrtoLval cVitablevalue;
		PrtoLmtb cVmutableitable;
		PrtoL2qr cVdeletequery;
		PrtoL2mr cVdeletedmlresult;
		COLUMNLOC* cVcolumnloc;
		ACCESSGROUPLOC* cVaccessgrouploc;
		INDEXLOC* cVindexloc;
		KEYLOC* cVkeyloc;
		POINTERKEYLOC* cVpointerkeyloc;
		CorePart::PARTITIONLOC* cVpartitionloc;
		CorePart::iCpartitiontype iVtablepartitiontype;
		CorePart::iCpartitiontype iVitablepartitiontype;

		iVmask = 0;
		cVmutable = cStatement->mutable_cvmutable ();
		cVkey = cStatement->mutable_cvkey ();
		bVdeletestatement = cVkey->ivstate () == DELTMB;
		cVtableloc = cTablelocpointer;
		cVpartitionloc = &cVtableloc->cVpartitionloc;
		iVtablepartitiontype = CorePart::getpt (cVpartitionloc, false);
		iVitablepartitiontype = CorePart::getpt (cVpartitionloc, true);
		bVindexinsert = cVtableloc->cVindexpool.count () > 0;
		bVindexupdate = bVindexinsert && !bVdeletestatement && !bUpdateonlyindexes;
		if (bVindexinsert) getim (cVmutable, cVmutableitable);
		if (bVdeletestatement) {
			cVtableloc->cVcolumnsstm.clear ();
			cVdeletequery.mutable_cvmutable ()->CopyFrom (*cVmutable );
			cVdeletequery.mutable_cvkey_start ()->CopyFrom (*cVkey);
			for (iV = 0; iV < cVtableloc->cVindexpool.count (); iV++) {
				cVindexloc = cVtableloc->cVindexpool.get__ (iV);
				for (iVinner = 0; iVinner < cVindexloc->cVindexcolumnpool.count (); iVinner++) {
					sVcolumn = cVindexloc->cVindexcolumnpool.get__ (iVinner)->cVcolumnloc->sVindex;
					if (cVtableloc->cVcolumnsstm.bnget (sVcolumn) == NULL) {
						cVtableloc->cVcolumnsstm.bnset (sVcolumn);
						cVdeletequery.mutable_cvselect ()->Add ()->set_svcolumn (sVcolumn.to_string ());
					}
				}			
			}			
			cVdeletequery.set_ivquery (EXACTQUERY);
			if (!qryl1 (&cVdeletequery, &cVdeletedmlresult, false, cVtableloc)) return false;			
			cStatement->mutable_cvcolumns ()->CopyFrom (*cVdeletedmlresult.mutable_cvkeyslice (0)->mutable_cvcolumns ());
		}
		iVaccessgroupcount = cVtableloc->cVaccessgrouppool.count ();
		for (iV = 0; iV < cStatement->cvcolumns_size (); iV++) {
			cVcolumninput = cStatement->mutable_cvcolumns (iV);
			cVcolumnloc = getcl (cVcolumninput->svcolumn (), cVtableloc);
			if (cVcolumnloc == NULL) continue;
			cVaccessgrouploc = cVtableloc->cVaccessgrouppool.get__ (cVcolumnloc->iVaccessgroup);
			cVcolumnoutput = cVaccessgrouploc->cVaccessobject.mutable_cvcolumns (cVcolumnloc->iVaccessobjectitem);
			cVcolumnoutput->CopyFrom (*cVcolumninput);
			iVmask |= 1 << cVcolumnloc->iVaccessgroup;
		}
		for (iV = 0; iV < iVaccessgroupcount; iV++) {
			if ((iVmask & (1 << iV)) || bVdeletestatement) {
				cVaccessgrouploc = cVtableloc->cVaccessgrouppool.get__ (iV);	
				cVaccessgrouploc->bVupdateaccessobject = false;
				cVkey->set_svaccessgroup (cVaccessgrouploc->sVindex.to_string ());				
				if (bVindexupdate) {
					cVupdatekey.CopyFrom (*cVkey);
					if (cVpeer->query (cVmutable, &cVupdatekey, &cVupdatevalue, CoreTble::iCexactquery, iVtablepartitiontype, cVpartitionloc, NULL, 
														&cTablelocpointer->cVlocalmutex)) {
						cVaccessgrouploc->cVupdateaccessobject.CopyFrom (cVaccessgrouploc->cVaccessobject);		
						cVaccessgrouploc->cVupdateaccessobject.ParseFromString (cVupdatevalue.svopaquevalue ());
						cVaccessgrouploc->iVupdatetimestamp = cVupdatevalue.ivtimestamp ();
						cVaccessgrouploc->bVupdateaccessobject = true;	
					}
				}
				cVvalue.set_svopaquevalue (cVaccessgrouploc->cVaccessobject.SerializeAsString ());	
				cVtableloc->cVstatsloc [iCwritestats].iVsize += cVvalue.svopaquevalue ().length ();
				cVvalue.set_ivtimestamp (cVkey->ivtimestamp ());
				if (!bUpdateonlyindexes) {
					muttr (	cVmutable, cVkey, &cVvalue, iVtablepartitiontype, &cTablelocpointer->cVpartitionloc, 
																					NULL, bLocalfilter, &cTablelocpointer->cVcachedlocaltablet); 
				}
				if (bVindexinsert) {
					for (iVinner = 0; iVinner < cVaccessgrouploc->cVindexpointerpool.count (); iVinner++) {		
						cVtableloc->cVkeysstm.clear ();		
						getik (	cVaccessgrouploc->cVindexpointerpool.get__ (iVinner)->cVindexloc, 
							cVaccessgrouploc->cVaccessobject.mutable_cvcolumns (), true, 
							cVkey->ivtimestamp (), cVkey->ivstate (), &cVtableloc->cVkeysstm, true, cVkey->svmain ());												
						if (cVaccessgrouploc->bVupdateaccessobject) {
							cVtableloc->cVpointerkeysstm.clear ();
							for (iVupdateinner = 0; iVupdateinner < cVtableloc->cVkeysstm.count (); iVupdateinner++) {
								cVkeyloc = cVtableloc->cVkeysstm.get__ (iVupdateinner);
								cVtableloc->cVpointerkeysstm.bnset (cVkeyloc->sVmain)->cVkeyloc = cVkeyloc;
							}
							getik (	cVaccessgrouploc->cVindexpointerpool.get__ (iVinner)->cVindexloc, 
								cVaccessgrouploc->cVupdateaccessobject.mutable_cvcolumns (), true, 																							
								cVaccessgrouploc->iVupdatetimestamp, DELTMB, &cVtableloc->cVkeysstm, true, cVkey->svmain ());		
							for (;iVupdateinner < cVtableloc->cVkeysstm.count (); iVupdateinner++) {
								cVkeyloc = cVtableloc->cVkeysstm.get__ (iVupdateinner);
								cVpointerkeyloc = cVtableloc->cVpointerkeysstm.bnget (cVkeyloc->sVmain);
								if (cVpointerkeyloc != NULL && bOptimizerewriteindexes) cVpointerkeyloc->cVkeyloc->bVvalid = cVkeyloc->bVvalid = false;
							}							
						}
						for (int iVindex = 0; iVindex < cVtableloc->cVkeysstm.count (); iVindex++) {
							cVkeyloc = cVtableloc->cVkeysstm.get__ (iVindex);
							if (!cVkeyloc->bVvalid) continue;							
							cVkeyloc->cVindexvalue.set_svoriginalkey (cVkey->svmain ());
							cVitablevalue.set_svopaquevalue (cVkeyloc->cVindexvalue.SerializeAsString ());				
							cVitablevalue.set_ivtimestamp (cVkey->ivtimestamp ());		
							muttr (&cVmutableitable, &cVkeyloc->cVkey, &cVitablevalue, iVitablepartitiontype, &cTablelocpointer->cVpartitionloc, 
								cTablelocpointer->bVlocalindex ? (AnsiString) cVkey->svmain () : NULL, bLocalfilter, 
								&cTablelocpointer->cVcachedlocalindextablet);		

#ifdef iCdebugstatement		
							printf ("%s %lld %i\n", ((AnsiString) cVkeyloc->cVkey.svmain ()).prntb ().c_str (), 
								cVkeyloc->cVkey.ivtimestamp (),
								cVkeyloc->cVkey.ivstate ());
#endif

						}
					}
				}				
			}
		}
		return true;
	}

	/*STATeMent*/
	bool __fastcall CorePlan::statm (	PrtoL2st* cStatement, 
																		bool bOptimizerewriteindexes, 
																		bool bUpdateonlyindexes, 
																		bool bLocalfilter,
																		PTR_TABLELOC* cTablelocpointer) {
		bool bVreturn;
		TDateTime tVtimestampstart;
		PTR_TABLELOC cVtableloc;
		
		tVtimestampstart = __Time ();
		if (cTablelocpointer != NULL && (*cTablelocpointer) != NULL) cVtableloc = boost::shared_ptr<TABLELOC> (*cTablelocpointer);
		else {
			cVglobalmutex.locks ();
			cVtableloc = gettl (cStatement->mutable_cvmutable ());
			cVglobalmutex.relse ();
			if (cVtableloc == NULL) {
				if (!crttb (NULL, cStatement)) return false;
				cVglobalmutex.locks ();
				cVtableloc = gettl (cStatement->mutable_cvmutable ());
				cVglobalmutex.relse ();
				if (cVtableloc == NULL) return false;
			}
			if (cTablelocpointer != NULL) *cTablelocpointer = cVtableloc;
		}
		switch (cStatement->ivopcode ()) {
			case MUTATOR: 			
				cVtableloc->cVlocalmutex.locks ();
				bVreturn = stati (cStatement, cVtableloc, bOptimizerewriteindexes, bUpdateonlyindexes, bLocalfilter); 
				if (bVreturn) stats (cVtableloc, tVtimestampstart, iCwritestats, 1);
				cVtableloc->cVlocalmutex.relse ();
				break;
			case REPAIR_: 
				cVtableloc->cVlocalmutex.locks ();
				bVreturn = rpaii (cStatement); 
				cVtableloc->cVlocalmutex.relse ();
				break; 
			default: bVreturn = false;
		}		
		return bVreturn;
	}

	/*LoCal STateMent*/
	bool __fastcall CorePlan::lcstm (PrtoLstm* cStatement, bool bNomutex, CoreAlog::TABLETLOC** cTabletlocpointer) {
		bool bVreturn;
		PTR_TABLELOC cVtableloc;
		CoreSmph* cVmutex;
		PrtoLmtb cVmutable;	

		if (bNomutex) {
			return cVpeer->muttr (cStatement, cTabletlocpointer);
		} else if (CoreAlog::sCsystemnamespace == cStatement->mutable_cvmutable ()->svnamespace ()) {
			cVmutex = &cVglobalmutex;
		} else {
			cVmutable.CopyFrom (*cStatement->mutable_cvmutable ());
			CoreAlog::ix2tb (&cVmutable);
			cVglobalmutex.locks ();		
			cVtableloc = gettl (&cVmutable);
			cVglobalmutex.relse ();
			if (cVtableloc == NULL  || !cVtableloc->bVvalid) return false;
			cVmutex = &cVtableloc->cVlocalmutex;
		}
		cVmutex->locks ();
		bVreturn = cVpeer->muttr (cStatement->ivopcode (), cStatement->mutable_cvmutable (), cStatement->mutable_cvkey (),
			cStatement->mutable_cvvalue (), CorePart::iCnopartition, NULL, NULL, false, cTabletlocpointer);
		cVmutex->relse ();
		return bVreturn;
	}

	/*RePAIr Internal*/
	bool __fastcall CorePlan::rpaii (PrtoL2st* cStatement) {
		bool bVreturn;
		bool bVfirsttime;
		PrtoLval cVvalue;
		PrtoLkey cVkey;
		PrtoL2iv cVindexvalue;
		PrtoLmtb cVmutable;
		AnsiString sVkey;
		AnsiString sVreferencetofix;
		AnsiString sVforwardkey;
		AnsiString sVfirsttime = "";
		PTR_TABLELOC cVtableloc;
		CorePart::PARTITIONLOC* cVpartitionloc;
		CorePart::iCpartitiontype iVpartitiontype;

		bVreturn = true;
		sVreferencetofix = cStatement->mutable_cvkey ()->svmain ();
		cVmutable.CopyFrom (*cStatement->mutable_cvmutable ());
		cVglobalmutex.locks ();
		cVtableloc = gettl (&cVmutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL  || !cVtableloc->bVvalid) return false;
		CoreAlog::tb2ix (&cVmutable);
		cVkey.set_svmain (CorePars::sCminansistring.to_string ());
		bVfirsttime = true;
		cVpartitionloc = &cVtableloc->cVpartitionloc;
		iVpartitiontype = CorePart::getpt (cVpartitionloc, CoreAlog::index (&cVmutable));
		while (query (&cVmutable, &cVkey, &cVvalue, CoreTble::iCtokenquery, iVpartitiontype, cVpartitionloc, NULL, NULL) && 
			cVkey.svmain () != sVfirsttime.to_string ()) {			
			sVkey = cVkey.svmain ();
			sVforwardkey = sVkey.prntb ();
			cVindexvalue.ParseFromString (cVvalue.svopaquevalue ());
			if ((AnsiString) cVindexvalue.svoriginalkey () == sVreferencetofix) {
				
#ifdef iCdebugstatement		
				printf ("%s -> %s\n", sVforwardkey.c_str (), cVindexvalue.svoriginalkey ().c_str ());
#endif

				cVkey.set_ivstate (DELTMB);
				bVreturn &= muttr (&cVmutable, &cVkey, &cVvalue, iVpartitiontype, cVpartitionloc, NULL, false, NULL);		
			}
			if (bVfirsttime) sVfirsttime = cVkey.svmain ();
			bVfirsttime = false;
		}
		return bVreturn;
	}

	/*ReBuiLD Indexes*/
	bool __fastcall CorePlan::rbldi (PrtoLmtb* cMutable) {
		bool bVreturn;
		bool bVfromempty;
		AnsiString sVmainkey;
		AnsiString sVprevmainkey;
		PrtoLval cVvalue;
		PrtoLkey cVkey;
		PrtoLkey* cVresultkey;
		PrtoLmtb cVmutable;
		PrtoL2qr cVquery;
		PrtoL2mr cVdmlresult;
		PrtoL2st cVstatement;
		PrtoL2ks* cVkeyslice;
		PTR_TABLELOC cVtableloc;
		CorePart::PARTITIONLOC* cVpartitionloc;
		CorePart::iCpartitiontype iVpartitiontype;

		bVreturn = true;
		bVfromempty = true;
		cVmutable.CopyFrom (*cMutable);
		cVglobalmutex.locks ();
		cVtableloc = gettl (&cVmutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL  || !cVtableloc->bVvalid) return true;
		if (cVtableloc->cVindexpool.count () == 0) return true;
		CoreAlog::tb2ix (&cVmutable);
		cVkey.set_svmain (CorePars::sCminansistring.to_string ());
		cVpartitionloc = &cVtableloc->cVpartitionloc;
		iVpartitiontype = CorePart::getpt (cVpartitionloc, CoreAlog::index (&cVmutable));
		while (query (&cVmutable, &cVkey, &cVvalue, CoreTble::iCtkslwquery, iVpartitiontype, cVpartitionloc, NULL, NULL) && 
			cVkey.svmain () != CorePars::sCminansistring.to_string ()) {			
			cVkey.set_ivstate (DELTMB);		
			bVreturn &= muttr (&cVmutable, &cVkey, &cVvalue, iVpartitiontype, cVpartitionloc, NULL, true, NULL);		
			bVfromempty = false;
		}
		cVquery.mutable_cvmutable ()->CopyFrom (*cMutable);
		cVquery.set_ivquery (TKSLWQUERY);
		cVquery.mutable_cvkey_start ()->set_svmain (CorePars::sCminansistring.to_string ());
		cVquery.set_ivcount (1);
		cVstatement.mutable_cvmutable ()->CopyFrom (*cMutable);
		cVstatement.set_ivopcode (MUTATOR);
		for (int iV = 0; iV < cVtableloc->cVcolumnpool.count (); iV++) 
				cVquery.mutable_cvselect ()->Add ()->set_svcolumn (cVtableloc->cVcolumnpool.get__ (iV)->sVindex.to_string ());
		sVprevmainkey = CorePars::sCminansistring;
		while (query (&cVquery, &cVdmlresult) && cVdmlresult.cvkeyslice_size () > 0) {
			cVkeyslice = cVdmlresult.mutable_cvkeyslice (0);
			cVresultkey = cVkeyslice->mutable_cvkey ();
			sVmainkey = cVresultkey->svmain ();
			if (cVresultkey->ivstate () != UPSERT || sVmainkey == sVprevmainkey) continue;
			sVprevmainkey = sVmainkey;
			cVstatement.mutable_cvkey ()->CopyFrom (*cVresultkey );
			cVstatement.mutable_cvcolumns ()->CopyFrom (*cVkeyslice->mutable_cvcolumns ());
			if (!bVfromempty) cVstatement.mutable_cvkey ()->set_ivtimestamp (cVstatement.mutable_cvkey ()->ivtimestamp () + 1);
			bVreturn &= statm (&cVstatement, bVfromempty, bVfromempty, true);
			cVquery.mutable_cvkey_start ()->CopyFrom (*cVresultkey);
		}
		return bVreturn;
	}

	/*CLEAN*/
	bool __fastcall CorePlan::clean (PrtoLmtb* cMutable) {
		PrtoLmtb cVmutable;
		PTR_TABLETSERVER cVtabletserver;

		cVmutable.CopyFrom (*cMutable);
		CoreAlog::tb2ix (&cVmutable);
		cVtabletserver = cVpeer->gettb (&cVmutable);
		if (cVtabletserver.use_count () == 0) return false;
		return cVtabletserver->clean ();
	}

	/*PURGE*/
	bool __fastcall CorePlan::purge (PrtoLmtb* cMutable) {
		CoreAenp cVantientrophy (this);
		PTR_TABLELOC cVtableloc;

		cVglobalmutex.locks ();
		cVtableloc = gettl (cMutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return false;
		return cVantientrophy.purge (cMutable, &cVtableloc->cVpartitionloc);
	}

	/*TRUNCate*/
	bool __fastcall CorePlan::trunc (PrtoLmtb* cMutable) {
		PrtoLmtb cVmutable;
		PTR_TABLETSERVER cVtabletserver;

		cVmutable.CopyFrom (*cMutable);
		cVtabletserver = cVpeer->gettb (&cVmutable);
		if (cVtabletserver.use_count () == 0) return false;
		if (cVtabletserver->clean ()) {
			CoreAlog::tb2ix (&cVmutable);
			cVtabletserver = cVpeer->gettb (&cVmutable);
			if (cVtabletserver.use_count () == 0) return false;
			return cVtabletserver->clean ();
		} else return false;
	}

	/*SHRiNk Internal*/
	bool __fastcall CorePlan::shrni (PrtoLmtb* cMutable, bool bSoft) {
		PrtoLmtb cVmutable;
		PTR_TABLETSERVER cVtabletserver;

		cVmutable.CopyFrom (*cMutable);
		cVtabletserver = cVpeer->gettb (&cVmutable);
		if (cVtabletserver.use_count () == 0) return true;
		if (!cVtabletserver->shrnk (bSoft)) return false;
		CoreAlog::tb2ix (&cVmutable);
		cVtabletserver = cVpeer->gettb (&cVmutable);
		if (cVtabletserver.use_count () == 0) return true;
		return cVtabletserver->shrnk (bSoft);
	}

	/*SHRiNK*/
	bool __fastcall CorePlan::shrnk (PrtoLmtb* cMutable) {

		return shrni (cMutable, false);
	}

	/*SOfr SHRink*/
	bool __fastcall CorePlan::soshr (PrtoLmtb* cMutable) {

		return shrni (cMutable, true);
	}

	/*SPLIT*/
	bool __fastcall CorePlan::split (PrtoLmtb* cMutable) {
		PTR_TABLELOC cVtableloc;
		PrtoLmtb cVmutable;

		cVmutable.CopyFrom (*cMutable);
		cVglobalmutex.locks ();
		cVtableloc = gettl (&cVmutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return true;
		if (!cVtableloc->cVpartitionloc.bVmanualpartition) {
			if (cVtableloc->cVpartitionloc.cVpartition.iVpartitiontype != CorePart::iCcustompartition)
				getpt (&cVmutable, false, cVtableloc->cVpartitionloc.cVpartition.sVjoincustompartition);
			if (cVtableloc->cVpartitionloc.cVpartitionindex.iVpartitiontype != CorePart::iCcustompartition)
				getpt (&cVmutable, true, cVtableloc->cVpartitionloc.cVpartitionindex.sVjoincustompartition);
		}
		return true;		
	}

	/*FReeZe SPlit*/
	bool __fastcall CorePlan::frzsp (PrtoLmtb* cMutable) {
		PTR_TABLELOC cVtableloc;
		PrtoLmtb cVmutable;
		CoreEnpo::GOSSIPLOC cVgossiploc;
		CorePart::PARTITIONLOC* cVpartitionloc;

		cVmutable.CopyFrom (*cMutable);
		cVglobalmutex.locks ();
		cVtableloc = gettl (&cVmutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return true;
		cVgossiper->getge (NULL, cVgossiploc, 1);
		cVpartitionloc = &cVtableloc->cVpartitionloc;

		LOG(INFO) <<	"Freeze to: " << cVgossiploc.sVindex.c_str () << 
									", table: " << cVmutable.svtable ().c_str () <<
									", bound: " << cVpartitionloc->cVpartition.sVjoincustompartition.prntb ().c_str () <<
									", index bound: " << cVpartitionloc->cVpartitionindex.sVjoincustompartition.prntb ().c_str ();
		FlushLogFiles(GLOG_INFO);

		cVpartitioner->setpd (cVpartitionloc, cVgossiploc.sVindex, cVgossiploc.sVtoken,
			cVpartitionloc->cVpartition.sVjoincustompartition, 
			cVpartitionloc->cVpartitionindex.sVjoincustompartition);
		return sttbl (gettn (&cVmutable), cVtableloc);
	}

	/*ReBuild PARtition*/
	bool __fastcall CorePlan::rbpar (PrtoLmtb* cMutable) {
		PTR_TABLELOC cVtableloc;
		PrtoLmtb cVmutable;
		CoreEnpo::GOSSIPLOC cVgossiploc;

		cVmutable.CopyFrom (*cMutable);
		cVglobalmutex.locks ();
		cVtableloc = gettl (&cVmutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return true;
	
		cVglobalmutex.locks ();
		cVtableloc->cVpartitionloc.cVpartition.sVupperbound = 
			cVpartitioner->rebpp (&cVtableloc->cVpartitionloc.cVpartition.cVpartitionpool);		
		cVtableloc->cVpartitionloc.cVpartitionindex.sVupperbound = 
			cVpartitioner->rebpp (&cVtableloc->cVpartitionloc.cVpartitionindex.cVpartitionpool);
		cVglobalmutex.relse ();
		return sttbl (gettn (&cVmutable), cVtableloc);
	}

	/*GET Index Key*/
	bool __fastcall CorePlan::getik (	INDEXLOC* cIndexloc, RepeatedPtrField<PrtoL2cl>* cRepeatedvaluesource, 
																		bool bAccessobjectitem, int64vcl iTimestamp, iCstatetype iState, CoreList<KEYLOC>* cReturnkeys,
																		bool bIndexsequence, AnsiString sUniquekey) {
		int iVcyclecount;
		int iVrepeatedfieldselection;
		int iVqualifier;
		AnsiString sVqualifier;
		AnsiString sVcommonroot;
		INDEXCOLUMNLOC* cVindexcolumnloc;
		COLUMNLOC* cVcolumnloc;
		PrtoL2cv* cVcolumnvalue;
		PrtoL2iv cVindexvalue;
		KEYLOC* cVkeyloc;
		int iVswitchtype;
		int iVreturnkeysstart;

		sVqualifier = cIndexloc->sVindex;
		iVqualifier = sVqualifier.Length (); 

		iVreturnkeysstart = cReturnkeys->count ();
		cVkeyloc = cReturnkeys->set__ ();
		cVkeyloc->cVindexvalue.add_ivkeydelimiters (iVqualifier);
		for (int iV = 0; iV < cIndexloc->cVindexcolumnpool.count (); iV++) {
			cVindexcolumnloc = cIndexloc->cVindexcolumnpool.get__ (iV);
			cVcolumnloc = cVindexcolumnloc->cVcolumnloc;
			iVrepeatedfieldselection = bAccessobjectitem ? cVcolumnloc->iVaccessobjectitem : iV;
			cVcolumnvalue = cRepeatedvaluesource->Mutable (iVrepeatedfieldselection)->mutable_cvvalue ();				
			iVcyclecount = cReturnkeys->count ();
			for (int iVcycle = iVreturnkeysstart; iVcycle < iVcyclecount; iVcycle++) {
				cVkeyloc = cReturnkeys->get__ (iVcycle);
				cVkeyloc->sVmain += sCindexseparator;
				if (cVcolumnvalue->has_ivtype ()) iVswitchtype = cVcolumnvalue->ivtype ();
				else iVswitchtype = cVcolumnloc->iVtype;
				switch (iVswitchtype) {

					case STRINGTYPE:
						if (cVindexcolumnloc->bVasc) cVkeyloc->sVmain += cVcolumnvalue->svvalue ();
						else cVkeyloc->sVmain += CoreButl::desca (cVcolumnvalue->svvalue ());						
						cVkeyloc->cVindexvalue.add_ivkeydelimiters (iVqualifier + cVkeyloc->sVmain.Length ());
						break;
					case LSTRNGTYPE:
						sVcommonroot = cVkeyloc->sVmain;
						cVindexvalue.CopyFrom (cVkeyloc->cVindexvalue);
						for (int iVinner = 0; iVinner < cVcolumnvalue->svlistvalue_size (); iVinner++) {						
							if (iVinner > 0) {
								cVkeyloc = cReturnkeys->set__ ();
								cVkeyloc->sVmain = sVcommonroot;
								cVkeyloc->cVindexvalue.CopyFrom (cVindexvalue);
							}							
							if (cVindexcolumnloc->bVasc) cVkeyloc->sVmain += *cVcolumnvalue->mutable_svlistvalue (iVinner);
							else cVkeyloc->sVmain += CoreButl::desca (*cVcolumnvalue->mutable_svlistvalue(iVinner));
							cVkeyloc->cVindexvalue.add_ivkeydelimiters (iVqualifier + cVkeyloc->sVmain.Length ());
						}
						break;					
					case INTEGRTYPE:
						if (cVindexcolumnloc->bVasc) cVkeyloc->sVmain += CoreButl::int2s (
							cVreverse->asc_i (cVcolumnvalue->ivvalue ()));													
						else cVkeyloc->sVmain += CoreButl::int2s (cVreverse->desci (cVcolumnvalue->ivvalue ()));						
						cVkeyloc->cVindexvalue.add_ivkeydelimiters (iVqualifier + cVkeyloc->sVmain.Length ());
						break;
					case LINTGRTYPE:
						sVcommonroot = cVkeyloc->sVmain;
						cVindexvalue.CopyFrom (cVkeyloc->cVindexvalue);
						for (int iVinner = 0; iVinner < cVcolumnvalue->ivlistvalue_size (); iVinner++) {						
							if (iVinner > 0) {
								cVkeyloc = cReturnkeys->set__ ();
								cVkeyloc->sVmain = sVcommonroot;
								cVkeyloc->cVindexvalue.CopyFrom (cVindexvalue);
							}							
							if (cVindexcolumnloc->bVasc) cVkeyloc->sVmain += CoreButl::int2s (
								cVreverse->asc_i (cVcolumnvalue->ivlistvalue (iVinner)));								
							else cVkeyloc->sVmain += CoreButl::int2s (cVreverse->desci (cVcolumnvalue->ivlistvalue (iVinner)));
							cVkeyloc->cVindexvalue.add_ivkeydelimiters (iVqualifier + cVkeyloc->sVmain.Length ());
						}
						break;
					case DOUBLETYPE:
						if (cVindexcolumnloc->bVasc) cVkeyloc->sVmain += CoreButl::dbl2s (
							cVreverse->asc_d (cVcolumnvalue->dvvalue ()));													
						else cVkeyloc->sVmain += CoreButl::desca (CoreButl::dbl2s (cVcolumnvalue->dvvalue ()));						
						cVkeyloc->cVindexvalue.add_ivkeydelimiters (iVqualifier + cVkeyloc->sVmain.Length ());
						break;
					case LDOUBLTYPE:
						sVcommonroot = cVkeyloc->sVmain;
						cVindexvalue.CopyFrom (cVkeyloc->cVindexvalue);
						for (int iVinner = 0; iVinner < cVcolumnvalue->dvlistvalue_size (); iVinner++) {						
							if (iVinner > 0) {
								cVkeyloc = cReturnkeys->set__ ();
								cVkeyloc->sVmain = sVcommonroot;
								cVkeyloc->cVindexvalue.CopyFrom (cVindexvalue);
							}							
							if (cVindexcolumnloc->bVasc) cVkeyloc->sVmain += CoreButl::dbl2s (
								cVreverse->asc_d (cVcolumnvalue->dvlistvalue (iVinner)));								
							else cVkeyloc->sVmain += CoreButl::desca (CoreButl::dbl2s (cVreverse->asc_d (cVcolumnvalue->dvlistvalue (iVinner))));
							cVkeyloc->cVindexvalue.add_ivkeydelimiters (iVqualifier + cVkeyloc->sVmain.Length ());
						}
						break;
					case BOOLN_TYPE:
						if (cVindexcolumnloc->bVasc) cVkeyloc->sVmain += CoreButl::bol2s (cVcolumnvalue->bvvalue ());													
						else cVkeyloc->sVmain += CoreButl::desca (CoreButl::bol2s (cVcolumnvalue->bvvalue ()));						
						cVkeyloc->cVindexvalue.add_ivkeydelimiters (iVqualifier + cVkeyloc->sVmain.Length ());
						break;

				}
			}
		}
		for (int iV = iVreturnkeysstart; iV < cReturnkeys->count (); iV++) {
			cVkeyloc = cReturnkeys->get__ (iV);
			cVkeyloc->sVmain = sVqualifier + cVkeyloc->sVmain;
			if (!bIndexsequence) cVkeyloc->cVkey.set_svmain (cVkeyloc->sVmain.to_string ());
			else cVkeyloc->cVkey.set_svmain ((cVkeyloc->sVmain + sCindexseparator + sUniquekey).to_string ());									
			cVkeyloc->cVkey.set_ivstate (iState);
			cVkeyloc->cVkey.set_ivtimestamp (iTimestamp);
		}
		return true;
	}

	/*SeLeCT Internal*/
	bool __fastcall CorePlan::slcti (PrtoL2mr* cDmlresult, PrtoL2qr* cQuery, PTR_TABLELOC cTablelocpointer, bool bLocal, bool bOnlysecondary) {
		PTR_TABLELOC cVtableloc;
		COLUMNLOC* cVcolumnloc;
		PrtoL2or* cVorderby;
		
		cVtableloc = cTablelocpointer;
		if (cQuery->mutable_cvwhere ()->cvcondition_size () > 0) return QRYL2 (cQuery, cDmlresult, cVtableloc);
		else {
			if (cQuery->mutable_cvwhere ()->cvorderby_size () > 0) {
				cVorderby = cQuery->mutable_cvwhere ()->mutable_cvorderby (0);
				cVcolumnloc = getcl (cVorderby->svcolumn (), cVtableloc);
				if (CorePars::bfkec (	cVorderby, cQuery->mutable_cvwhere ()->add_cvcondition (), cVcolumnloc)) 
					return QRYL2 (cQuery, cDmlresult, cVtableloc);
			} else {
				cQuery->mutable_cvkey_start ()->set_svmain (CorePars::sCminansistring.to_string ());
				cQuery->mutable_cvkey_end ()->set_svmain (CorePars::sCminansistring.to_string ());
				cQuery->set_ivquery (TOKENQUERY);
				return qryl1 (cQuery, cDmlresult, bOnlysecondary, cVtableloc, bLocal);
			}
		}		
		return false;
	}

	/*ChecK STRing*/
	int __fastcall CorePlan::ckstr (AnsiString sString1, AnsiString sString2) {

		return sString1.strcmp (sString2);
	}

	/*ChecK INT*/
	int __fastcall CorePlan::ckint (int iInt1, int iInt2) {

		if (iInt1 == iInt2) return 0;
		else return iInt1 > iInt2 ? 1 : -1;
	}

	/*ChecK DouBLe*/
	int __fastcall CorePlan::ckdbl (double dDouble1, double dDouble2) {

		if (dDouble1 == dDouble2) return 0;
		else return dDouble1 > dDouble2 ? 1 : -1;
	}

	/*ChecK BOoLean*/
	int __fastcall CorePlan::ckbol (bool bBool1, bool bBool2) {

		if (bBool1 == bBool2) return 0;
		else return bBool1 > bBool2 ? 1 : -1;
	}

	/*ChecK CoMPare*/
	bool __fastcall CorePlan::ckcmp (iCconditiontype iConditiontype, int iCompared) {

		switch (iConditiontype) {
			case ECOND: return iCompared == 0;
			case GCOND: return iCompared > 0;
			case GECOND: return iCompared >= 0;
			case LCOND: return iCompared < 0;
			case LECOND: return iCompared <= 0;
		}
		return false;
	}

	/*ChecK RAW*/
	bool __fastcall CorePlan::ckraw (PrtoL2cl* cCondition, PrtoL2cl* cObject) {

		switch (cObject->ivtype ()) {
			case STRINGTYPE:
				return ckcmp (cCondition->ivconditiontype (), 
					ckstr (cObject->mutable_cvvalue ()->svvalue (), cCondition->mutable_cvvalue ()->svvalue ()));
			case LSTRNGTYPE:	
				for (int iV = 0; iV < cObject->mutable_cvvalue ()->svlistvalue_size (); iV++) {
					if (ckcmp (cCondition->ivconditiontype (), 
						ckstr (cObject->mutable_cvvalue ()->svlistvalue (iV), cCondition->mutable_cvvalue ()->svlistvalue (0))))
						return true;
				}
				return false;
			case INTEGRTYPE:
				return ckcmp (cCondition->ivconditiontype (), 
					ckint (cObject->mutable_cvvalue ()->ivvalue (), cCondition->mutable_cvvalue ()->ivvalue ()));
			case LINTGRTYPE:	
				for (int iV = 0; iV < cObject->mutable_cvvalue ()->ivlistvalue_size (); iV++) {
					if (ckcmp (cCondition->ivconditiontype (), 
						ckint (cObject->mutable_cvvalue ()->ivlistvalue (iV), cCondition->mutable_cvvalue ()->ivlistvalue (0))))
						return true;
				}
				return false;
			case DOUBLETYPE:
				return ckcmp (cCondition->ivconditiontype (), 
					ckdbl (cObject->mutable_cvvalue ()->dvvalue (), cCondition->mutable_cvvalue ()->dvvalue ()));
			case LDOUBLTYPE:	
				for (int iV = 0; iV < cObject->mutable_cvvalue ()->dvlistvalue_size (); iV++) {
					if (ckcmp (cCondition->ivconditiontype (), 
						ckdbl (cObject->mutable_cvvalue ()->dvlistvalue (iV), cCondition->mutable_cvvalue ()->dvlistvalue (0))))
						return true;
				}
			case BOOLN_TYPE:
				return ckcmp (cCondition->ivconditiontype (), 
					ckbol (cObject->mutable_cvvalue ()->bvvalue (), cCondition->mutable_cvvalue ()->bvvalue ()));
			default:
				return false;
		}
	}

	/*SeLeCt Full Internal*/
	bool __fastcall CorePlan::slcfi (PrtoL2mr* cDmlresult, PrtoL2qr* cQuery, PTR_TABLELOC cTablelocpointer, bool bLocal, bool bOnlysecondary) {
		PrtoL2qr cVl1query;
		PrtoL2mr cVl1result;
		int iVcount;
		int iVfield;
		bool bVfound;
		AnsiString sVkey;
		AnsiString sVprevmainkey;
		CoreList<int> cVposition;
		PrtoL2ks* cVkeyslice;
		PrtoL2wh* cVwhere;
		PrtoLkey* cVresultkey;

		iVcount = cQuery->ivcount ();
		cVl1query.CopyFrom (*cQuery);

		cVl1query.mutable_cvkey_start ()->set_svmain (CorePars::sCminansistring.to_string ());
		cVl1query.mutable_cvkey_end ()->set_svmain (CorePars::sCminansistring.to_string ());
		cVl1query.set_ivquery (TKSLWQUERY);
		cVl1query.set_ivcount (1);
		cVwhere = cVl1query.mutable_cvwhere ();
		for (int iV = 0; iV < cVwhere->cvcondition_size (); iV++) {
			for (int iVinner = 0; iVinner < cVl1query.cvselect_size (); iVinner++) {
				if (cVl1query.mutable_cvselect (iVinner)->svcolumn () == 
					cVl1query.mutable_cvwhere ()->mutable_cvcondition (iV)->svcolumn ()) {
					*cVposition.set__ () = iVinner;
					bVfound = true;
					break;
				} 
			}
			if (!bVfound) return false;
		}
		sVprevmainkey = CorePars::sCminansistring;
		while (qryl1 (&cVl1query, &cVl1result, bOnlysecondary, cTablelocpointer, bLocal)) {
			cVkeyslice = cVl1result.mutable_cvkeyslice (0);
			cVresultkey = cVkeyslice->mutable_cvkey ();
			sVkey = cVresultkey->svmain ();

			/*
			printf ("valore %s %s ed è un upsert %i %lld \n", sVprevmainkey.c_str (), sVkey.c_str (), cVresultkey->ivstate () == UPSERT,
				cVresultkey->ivtimestamp ());*/

			if (sVkey == sVprevmainkey) continue;
			sVprevmainkey = sVkey;
			if (cVresultkey->ivstate () != UPSERT) continue;
			cVl1query.mutable_cvkey_start ()->set_svmain (sVkey.to_string ());
			bVfound = true;
			for (int iV = 0; iV < cVwhere->cvcondition_size (); iV++) {
				iVfield = *cVposition.get__ (iV);
				if (iVfield >= cVkeyslice->cvcolumns_size ()) return false;
				if (!ckraw (cVwhere->mutable_cvcondition (iV), cVkeyslice->mutable_cvcolumns (iVfield))) {
					bVfound = false;
					break;
				}
			}
			if (bVfound) {
				cDmlresult->add_cvkeyslice ()->CopyFrom (*cVkeyslice);
				iVcount--;
				if (iVcount == 0) break;
			} 
			cVl1result.Clear ();
		}
		return true;
	}

	/*INSerRt Internal*/
	bool __fastcall CorePlan::insri (PrtoL2st* cStatement, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer) {
		PrtoL2st cVstatement;
		int iVinsertcolumns;
		int iVselectcolumns;

		iVinsertcolumns = cStatement->cvcolumns_size ();		
		for (int iV = 0; iV < cDmlresult->cvkeyslice_size (); iV++) {
			cVstatement.CopyFrom (*cStatement);
			iVselectcolumns = cDmlresult->mutable_cvkeyslice (iV)->cvcolumns_size ();
			if (iVinsertcolumns != iVselectcolumns) return false;
			cVstatement.mutable_cvkey ()->CopyFrom (cDmlresult->mutable_cvkeyslice (iV)->cvkey ());
			cVstatement.mutable_cvkey ()->set_ivtimestamp (cVstatement.ivtimestamp ());
			for (int iVinner = 0; iVinner < iVselectcolumns; iVinner++) {
				cVstatement.mutable_cvcolumns (iVinner)->mutable_cvvalue ()->CopyFrom (
					cDmlresult->mutable_cvkeyslice (iV)->mutable_cvcolumns (iVinner)->cvvalue ());
			}
			stati (&cVstatement, cTablelocpointer, true, false,false);
		}
		return true;
	}

	/*UPDaTe Internal*/
	bool __fastcall CorePlan::updti (PrtoL2st* cStatement, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer) {
		PrtoL2st cVstatement;
		PrtoL2st cVmaster;
		CoreList<UPDATELOC> cVupdates;
		PrtoL2ks* cVkeyslice;
		PrtoL2cl* cVcolumn;
		AnsiString sVtemp;

		for (int iV = 0; iV < cStatement->cvcolumns_size (); iV++) {
			cVupdates.bnset (((AnsiString) cStatement->mutable_cvcolumns (iV)->svcolumn ()).UpperCase ())->
				cVprotoupdate.CopyFrom (*(cStatement->mutable_cvcolumns (iV)->mutable_cvvalue ()));
		}
		cVmaster.CopyFrom (*cStatement);
		cVmaster.mutable_cvcolumns ()->Clear ();
		for (int iV = 0; iV < cDmlresult->cvkeyslice_size (); iV++) {
			cVkeyslice = cDmlresult->mutable_cvkeyslice (iV);
			cVstatement.CopyFrom (cVmaster);
			cVstatement.mutable_cvkey ()->CopyFrom (cVkeyslice ->cvkey ());
			cVstatement.mutable_cvkey ()->set_ivtimestamp (cVstatement.ivtimestamp ());
			for (int iVinner = 0; iVinner < cVkeyslice->cvcolumns_size (); iVinner++) {
				sVtemp = cVkeyslice->mutable_cvcolumns (iVinner)->svcolumn ();
				UPDATELOC* cVupdate = cVupdates.bnget (sVtemp.UpperCase ());
				cVcolumn = cVkeyslice->mutable_cvcolumns (iVinner);
				if (cVupdate != NULL) {
					cVcolumn->mutable_cvvalue ()->CopyFrom (cVupdate->cVprotoupdate);
					cVcolumn->set_ivtype (cVupdate->cVprotoupdate.ivtype ());
				}
				cVstatement.add_cvcolumns ()->CopyFrom (*cVcolumn);
			}
			stati (&cVstatement, cTablelocpointer, true, false, false);
		}
		return true;
	}

	/*DELeTe Internal*/
	bool __fastcall CorePlan::delti (PrtoL2st* cStatement, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer) {
		PrtoL2st cVstatement;
		PrtoL2st cVmaster;
		PrtoL2ks* cVkeyslice;
		PrtoL2cl* cVcolumn;
		AnsiString sVtemp;

		cVmaster.CopyFrom (*cStatement);
		cVmaster.mutable_cvcolumns ()->Clear ();
		for (int iV = 0; iV < cDmlresult->cvkeyslice_size (); iV++) {
			cVkeyslice = cDmlresult->mutable_cvkeyslice (iV);
			cVstatement.CopyFrom (cVmaster);
			cVstatement.mutable_cvkey ()->CopyFrom (cVkeyslice ->cvkey ());
			//cVstatement.mutable_cvkey ()->set_ivtimestamp (cVstatement.ivtimestamp ());
			cVstatement.mutable_cvkey ()->set_ivstate (DELTMB);
			for (int iVinner = 0; iVinner < cVkeyslice->cvcolumns_size (); iVinner++) {
				sVtemp = cVkeyslice->mutable_cvcolumns (iVinner)->svcolumn ();
				cVcolumn = cVkeyslice->mutable_cvcolumns (iVinner);
				cVstatement.add_cvcolumns ()->CopyFrom (*cVcolumn);
			}
			stati (&cVstatement, cTablelocpointer, true, false, false);
		}
		return true;
	}

	/*STATS*/
	void __fastcall CorePlan::stats (PTR_TABLELOC cTableloc, TDateTime tTimestampstart, iCstatstype iStatstype, int iCount) {
		TDateTime tVtimestampdiff;
		STATSLOC* cVstatsloc;

		cVstatsloc = &cTableloc->cVstatsloc [iStatstype];
		cVstatsloc->iVcount += iCount;
		tVtimestampdiff = __Time () - tTimestampstart;
		cVstatsloc->dVmeandelay  = CoreButl::movav (cVstatsloc->dVmeandelay, tVtimestampdiff, dCmovingaveragedimdelay);
		cVstatsloc->dVworstdelay = Max (cVstatsloc->dVworstdelay, tVtimestampdiff);
		cVstatsloc->dVtotaldelay += tVtimestampdiff;
	}

	/*SELeCT*/
	bool __fastcall CorePlan::selct (	PrtoL2mr* cDmlresult, AnsiString sSqlstring, PrtoL2os* cOsql, bool bLocal) {
		CorePars::PARSESESSION cVparsesession;		
		PrtoL2qr cVquery;
		PTR_TABLELOC cVtableloc;
		TDateTime tVtimestampstart;
		bool bVreturn;

		tVtimestampstart = __Time ();
		cVparser->delps (sSqlstring, &cVparsesession, CorePars::iCselect);
		cVparser->frmps (cOsql->svnamespace (), &cVparsesession, &cVquery);
		cVglobalmutex.locks ();
		cVtableloc = gettl (cVquery.mutable_cvmutable ());
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return false;
		cVparser->selps (&cVparsesession, &cVquery, cVtableloc);
		if (!cVparser->whrps (&cVparsesession, &cVquery, cVtableloc, cOsql->mutable_cvinputbindings ())) return false;
		if (!cVparser->ordps (&cVparsesession, &cVquery)) return false;
		if (!cVparser->limps (&cVparsesession, &cVquery)) return false;
		if (cVquery.cvselect_size () == 0) 
			for (int iV = 0; iV < cVtableloc->cVcolumnpool.count (); iV++) 
				cVquery.mutable_cvselect ()->Add ()->set_svcolumn (cVtableloc->cVcolumnpool.get__ (iV)->sVindex.to_string ());
		cVquery.set_bvonlysecondary (cOsql->bvonlysecondary ());
		cVquery.set_bvdisableteleport (cOsql->bvdisableteleport ());
		cVquery.set_bvfulltablescan (cOsql->bvfulltablescan ());
		cVtableloc->cVlocalmutex.locks ();

#ifdef iCdebugqryl2
		printf ("OSQL Query: %s\n", cOsql->svosqlstring ().c_str ());
		printf ("OSQL Parameters: %i\n", cOsql->cvinputbindings_size ());
#endif

		bVreturn = slcti (cDmlresult, &cVquery, cVtableloc, bLocal, cOsql->bvonlysecondary ());

		if (!bVreturn && cOsql->bvfulltablescan ()) 
			bVreturn = slcfi (cDmlresult, &cVquery, cVtableloc, bLocal, cOsql->bvonlysecondary ());

#ifdef iCdebugqryl2
			printf ("OSQL Results: %i\n", cDmlresult->cvkeyslice_size ());
#endif

		if (bVreturn) stats (cVtableloc, tVtimestampstart, iCreadstats, 1);
		cVtableloc->cVlocalmutex.relse ();
		return bVreturn;
	}

	/*INSeRT*/
	bool __fastcall CorePlan::insrt (	AnsiString sSqlstring, PrtoL2mr* cDmlresult, PrtoL2os* cOsql) {
		AnsiString sVtemp;
		AnsiString sVcommand;
		CorePars::PARSESESSION cVparsesession;	
		PrtoL2st cVstatement;
		PrtoL2mr cVdmlresult;
		bool bVreturn;
		PTR_TABLELOC cVtableloc;
		TDateTime tVtimestampstart;
		COLUMNLOC* cVcolumnloc;

		tVtimestampstart = __Time ();
		cVparser->delps (cOsql->svosqlstring (), &cVparsesession, CorePars::iCinsert);
		cVparser->stbps (cOsql->svnamespace (), cOsql->ivtimestamp (), &cVparsesession, &cVstatement, CorePars::sCkeywordinto);
		cVparser->insps (&cVparsesession, &cVstatement);
		cVglobalmutex.locks ();
		cVtableloc = gettl (cVstatement.mutable_cvmutable ());
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return false;
		sVtemp = cVparser->getps (&cVparsesession, CorePars::sCkeywordbracketclose);
		sVtemp = sVtemp.Trim();
		sVcommand = CoreButl::left2 (sVtemp, " ").UpperCase ();
		if (sVcommand == CorePars::sCkeywordselect) {
			sVtemp = CorePars::sCkeywordselect + " " + sVtemp;
			bVreturn = selct (&cVdmlresult, sVtemp, cOsql, false);
			if (bVreturn) {
				cVtableloc->cVlocalmutex.locks ();
				for (int iV = 0; iV < cVstatement.cvcolumns_size (); iV++) {
					sVtemp = cVstatement.mutable_cvcolumns (iV)->svcolumn ();
					cVcolumnloc = cVtableloc->cVcolumnpool.bnget (sVtemp.UpperCase ());
					if (cVcolumnloc == NULL) return false;
					else cVstatement.mutable_cvcolumns (iV)->set_ivtype ((iCcolumntype) cVcolumnloc->iVtype);
					
				}
				bVreturn = insri (&cVstatement, &cVdmlresult, cVtableloc);
				setrs (sCpromptupdates, cDmlresult, cVdmlresult.cvkeyslice_size ());	
				if (bVreturn) stats (cVtableloc, tVtimestampstart, iCwritestats, cVdmlresult.cvkeyslice_size ());
				cVtableloc->cVlocalmutex.relse ();
			}
		} else {
			cVparser->delps (sVtemp, &cVparsesession, CorePars::iCinsertvalues);
			cVparser->valps (&cVparsesession, &cVstatement, cVtableloc);
			tVtimestampstart = __Time ();
			cVtableloc->cVlocalmutex.locks ();
			bVreturn = stati (&cVstatement, cVtableloc, true, false, false);
			setrs (sCpromptupdates, cDmlresult, 1);	
			if (bVreturn) stats (cVtableloc, tVtimestampstart, iCwritestats, 1);
			cVtableloc->cVlocalmutex.relse ();
		}
		return bVreturn;
	}

	/*SET ReSult*/
	void __fastcall CorePlan::setrs (AnsiString cPrompt, PrtoL2mr* cResult, int iCount) {
		PrtoL2cl* cVcolumn;
		PrtoL2cv* cVvalue;

		cVcolumn = cResult->add_cvkeyslice ()->add_cvcolumns ();
		cVcolumn->set_ivtype (INTEGRTYPE);
		cVvalue = cVcolumn->mutable_cvvalue ();
		cVvalue->set_ivtype (INTEGRTYPE);
		cVvalue->set_ivvalue (iCount);
	}

	/*UPDaTE*/
	bool __fastcall CorePlan::updte (PrtoL2mr* cDmlresult, AnsiString sSqlstring, PrtoL2os* cOsql) {
		AnsiString sVcommand;
		AnsiString sVtemp;
		TDateTime tVtimestampstart;
		CorePars::PARSESESSION cVparsesession;	
		PrtoL2st cVstatement;
		PrtoL2mr cVdmlresult;
		PTR_TABLELOC cVtableloc;
		PrtoL2os cVqueryosql;
		bool bVreturn;

		tVtimestampstart = __Time ();
		cVparser->delps (cOsql->svosqlstring (), &cVparsesession, CorePars::iCupdate);
		cVparser->stbps (cOsql->svnamespace (), cOsql->ivtimestamp (), &cVparsesession, &cVstatement, CorePars::sCkeywordupdate);
		cVglobalmutex.locks ();
		cVtableloc = gettl (cVstatement.mutable_cvmutable ());
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return false;
		if (!cVparser->setps (&cVparsesession, &cVstatement, cVtableloc, cOsql->mutable_cvinputbindings ())) return false;
		sVcommand = CorePars::sCkeywordselect + " " + CorePars::sCkeywordfrom + " " +
			cVstatement.mutable_cvmutable ()->svtable () + " ";
		sVtemp = CorePars::getps (&cVparsesession, CorePars::sCkeywordwhere);
		if (sVtemp != NULL) sVcommand += CorePars::sCkeywordwhere + " " + sVtemp + " ";
		sVtemp = CorePars::getps (&cVparsesession, CorePars::sCkeywordlimit);
		if (sVtemp != NULL) sVcommand += CorePars::sCkeywordlimit + " " + sVtemp;
		cVqueryosql.CopyFrom (*cOsql);
		bVreturn = selct (&cVdmlresult, sVcommand, &cVqueryosql, false);
		if (bVreturn) {
			cVtableloc->cVlocalmutex.locks ();
			bVreturn = updti (&cVstatement, &cVdmlresult, cVtableloc);
			setrs (sCpromptupdates, cDmlresult, cVdmlresult.cvkeyslice_size ());
			if (bVreturn) stats (cVtableloc, tVtimestampstart, iCwritestats, cDmlresult->cvkeyslice_size ());
			cVtableloc->cVlocalmutex.relse ();
			return true;
		}
		return false;
	}

	/*DELeTE*/
	bool __fastcall CorePlan::delte (PrtoL2mr* cDmlresult, AnsiString sSqlstring, PrtoL2os* cOsql) {
		AnsiString sVcommand;
		AnsiString sVtemp;
		TDateTime tVtimestampstart;
		CorePars::PARSESESSION cVparsesession;	
		PrtoL2st cVstatement;
		PrtoL2mr cVdmlresult;
		PTR_TABLELOC cVtableloc;
		PrtoL2os cVqueryosql;
		bool bVreturn;

		tVtimestampstart = __Time ();
		cVparser->delps (cOsql->svosqlstring (), &cVparsesession, CorePars::iCdelete);
		cVparser->stbps (cOsql->svnamespace (), cOsql->ivtimestamp (), &cVparsesession, &cVstatement, CorePars::sCkeywordfrom);
		cVglobalmutex.locks ();
		cVtableloc = gettl (cVstatement.mutable_cvmutable ());
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return false;

		if (!cVparser->setps (&cVparsesession, &cVstatement, cVtableloc, cOsql->mutable_cvinputbindings ())) return false;
		sVcommand = CorePars::sCkeywordselect + " " + CorePars::sCkeywordfrom + " " +
			cVstatement.mutable_cvmutable ()->svtable () + " ";
		sVtemp = CorePars::getps (&cVparsesession, CorePars::sCkeywordwhere);
		if (sVtemp != NULL) sVcommand += CorePars::sCkeywordwhere + " " + sVtemp + " ";
		sVtemp = CorePars::getps (&cVparsesession, CorePars::sCkeywordlimit);
		if (sVtemp != NULL) sVcommand += CorePars::sCkeywordlimit + " " + sVtemp;
		cVqueryosql.CopyFrom (*cOsql);
		bVreturn = selct (&cVdmlresult, sVcommand, &cVqueryosql, false);
		if (bVreturn) {
			cVtableloc->cVlocalmutex.locks ();
			bVreturn = delti (&cVstatement, &cVdmlresult, cVtableloc);
			setrs (sCpromptupdates, cDmlresult, cVdmlresult.cvkeyslice_size ());		
			if (bVreturn) stats (cVtableloc, tVtimestampstart, iCwritestats, cDmlresult->cvkeyslice_size ());
			cVtableloc->cVlocalmutex.relse ();
			return true;
		}
		return false;
	}

	/*OSQL*/
	bool __fastcall CorePlan::osql_ (PrtoL2os* cOsql, PrtoL2mr* cDmlresult) {
		bool bVreturn;
		AnsiString sVosqlstring;
		AnsiString sVcommand;

		sVosqlstring = cOsql->svosqlstring ();
		sVosqlstring = sVosqlstring.Trim ();
		sVcommand = CoreButl::left2 (sVosqlstring, " ").UpperCase ();
		if (sVcommand == CorePars::sCkeywordselect) {
			bVreturn = selct (cDmlresult, cOsql->svosqlstring (), cOsql, false);
		} else if (sVcommand == CorePars::sCkeywordinsert) {
			bVreturn = insrt (cOsql->svosqlstring (), cDmlresult, cOsql);
		} else if (sVcommand == CorePars::sCkeywordlocsel) {
			bVreturn = selct (cDmlresult, cOsql->svosqlstring (), cOsql, true);
		} else if (sVcommand == CorePars::sCkeywordupdate) {
			bVreturn = updte (cDmlresult, cOsql->svosqlstring (), cOsql);
		} else if (sVcommand == CorePars::sCkeyworddelete) {
			bVreturn = delte (cDmlresult, cOsql->svosqlstring (), cOsql);
		}
		return bVreturn;
	}

	/*DEBuG Index*/
	void __fastcall CorePlan::debgi (	PrtoLmtb* cMutable, AnsiString sKey, AnsiString sReversekey,
																		CoreTble::iCquerytype iVquerytype) {		
		bool bVfirsttime;
		PrtoLval cVvalue;
		PrtoLkey cVkey;
		PrtoL2iv cVindexvalue;
		PrtoLmtb cVmutable;
		AnsiString sVkey;
		AnsiString sVforwardkey;
		AnsiString sVreversekey;
		AnsiString sVfirsttime = "";
		char bVtemp;

		cVmutable.CopyFrom (*cMutable);
		CoreAlog::tb2ix (&cVmutable);
		cVkey.set_svmain (CorePars::sCminansistring.to_string ());
		bVfirsttime = true;
		while (query (&cVmutable, &cVkey, &cVvalue, iVquerytype, CorePart::iCnopartition, NULL, NULL, NULL) && 
			cVkey.svmain () != sVfirsttime.to_string ()) {
			
			sVkey = cVkey.svmain ();
			sVforwardkey = sVkey.prntb ();
			sVreversekey = CoreButl::desca (sVkey).prntb ();
			if (sVkey.SubString (1, sKey.Length ()) == sKey) {
				cVindexvalue.ParseFromString (cVvalue.svopaquevalue ());
				if (sReversekey == NULL || sReversekey == (AnsiString) cVindexvalue.svoriginalkey ()) {
					printf ("  "); 
					for (int iV = 0; iV < sVforwardkey.Length (); iV++) {
						bVtemp = sVreversekey.c_str () [iV];
						if (bVtemp == '#') bVtemp = sVforwardkey.c_str () [iV];
						printf ("%c", bVtemp);
					}
					printf (" -> %s, %lld\n", cVindexvalue.svoriginalkey ().c_str (), cVvalue.ivtimestamp ());
				}
			}
			if (bVfirsttime) sVfirsttime = cVkey.svmain ();
			bVfirsttime = false;
		}
	}

	/*COMPAction*/
	bool __fastcall CorePlan::compa (	PrtoLmtb* cMutable, CoreTbsv::iCcompactions iCompaction, 
																		int iCompactionlevel) {
		AnsiString sVtemp;
		AnsiString sVtable;
		bool bVreturn;

#ifdef iCcompactiontemporaryfix
		PrtoLmtb cVmutable;
		CoreSmph* cVcompactionmutex;

		cVmutable.CopyFrom (*cMutable);
		cVmutable.set_svtable (CoreButl::rplce (cVmutable.svtable (), CoreAlog::sCitableprefix, "").to_string ());
		cVglobalmutex.locks ();
		cVcompactionmutex = 
			(AnsiString) cVmutable.svnamespace () == CoreAlog::sCsystemnamespace ? &cVglobalmutex : &gettl (&cVmutable)->cVlocalmutex;
		cVglobalmutex.relse ();
#endif

		switch (iCompaction) {
			case MAJOR: sVtemp = "MAJOR"; break;
			case MINOR: sVtemp = "MINOR"; break;
			case MERGE: sVtemp = "MERGE"; break;
			case FLUSH: sVtemp = "FLUSH"; break;
			default: return false;
		}
		LOG(INFO) << ">Comp Ini:" << cMutable->svtable ().c_str () << ", comp lev: " << 
								iCompactionlevel << ", type: " << sVtemp.c_str ();
		FlushLogFiles(GLOG_INFO);

#ifdef iCcompactiontemporaryfix
		cVcompactionmutex->locks ();
#endif 

		bVreturn =  cVpeer->compa (cMutable, iCompaction, iCompactionlevel, cVnodedata.iVmaxcompactionlevel);

#ifdef iCcompactiontemporaryfix
		cVcompactionmutex->relse ();
#endif

		LOG(INFO) << "<Comp End:" << cMutable->svtable ().c_str () << ", comp lev: " << 
								iCompactionlevel << ", type: " << sVtemp.c_str ();
		FlushLogFiles(GLOG_INFO);

		return bVreturn;
	}
	
	/*SWiTCH*/
	bool __fastcall CorePlan::swtch () {

		return cVpeer->swtch ();
	}

	/*ReCYCleR*/
	bool __fastcall CorePlan::rcycr () {
		PrtoLmtb cVmutable;
		PrtoSeda cVmessage;

		cVglobalmutex.locks ();
		for (int iV = 0; iV < cVtablepool.count (); iV++) {
			CoreAlog::settk (cVtablepool.get__ (iV)->sVindex, &cVmutable);
			cVmessage.set_svnamespace (cVmutable.svnamespace ());
			cVmessage.set_svtable (cVmutable.svtable ());
			cVmessage.set_ivcompactiontype (MAJOR);
			cVsedamanager->pushs (CorePlan::sCmemtablepostflusher, cVmessage);
		}
		cVglobalmutex.relse ();		
		return true;
	}

	/*GOSSiP*/
	bool __fastcall CorePlan::gossp (PrtoGoss* cGossip, PrtoGosr* cGossipresult) {
		bool bVreturn;
		bool bVreloadtables;

		bVreturn = cVpeer->getgo ()->gossp (cGossip, cGossipresult, bVreloadtables);
		if (bVreloadtables) lnch_ (NULL, &CorePlan::rltbl);
		return bVreturn;
	}
	
	/*GET working SiZe*/
	int64vcl __fastcall CorePlan::getsz () {

		return cVredolog->getsz ();  
	}

	/*GET Gossip Pool*/
	void __fastcall CorePlan::getgp (PrtoGoss* cGossipresult, bool bDetails) {
		AnsiString sVlocalnodeid;
		PrtoGoel* cVgossipelement;
		CoreEnpo::GOSSIPLOC cVtemp; 

		cVgossiper->getgp (cGossipresult); 
		if (bDetails) {

			/*
			printf ("\n\ngossiper\n %s\n", cVpeer->getgo ()->debug ().c_str ());
			printf ("\n\nring\n %s\n", cVpeer->getep ()->debug ().c_str ());*/

			sVlocalnodeid = cVpeer->getep ()->getni ();
			for (int iV = 0; iV < cGossipresult->cvgossipelement_size (); iV++) {
				cVgossipelement = cGossipresult->mutable_cvgossipelement (iV);
				if (cVgossipelement->svnodeid () == sVlocalnodeid.to_string ()) {
					cVgossipelement->set_ivsize (getsz ());
				} else {
					cVtemp.sVaddress = cGossipresult->cvgossipelement (iV).svaddress ();
					cVtemp.iVport = cGossipresult->cvgossipelement (iV).ivport ();
					cVtemp.cVconnectionfactory->init_ (cVtemp.sVaddress, cVtemp.iVport);		
					cVgossipelement->set_ivsize (CorePutl::size_ (cVtemp.cVconnectionfactory));
				}
			}
		}
	}

	/*GET Gossip Pool*/
	void __fastcall CorePlan::getgp (PrtoGoel* cGossipresult) {
		cVgossiper->getgp (cGossipresult);
	}

	/*JOIN*/
	bool __fastcall CorePlan::join_ (AnsiString sAddress, int iPort, AnsiString sNewtoken, bool bAlign) {

		if (cVpeer->joina (sAddress, iPort, sNewtoken)) {
			lnch_ (NULL, &CorePlan::rltbl);
			if (cVpeer->joinb (bAlign)) {
				CorePlan::lnch_ (NULL, &CorePlan::rltbl);
				return true;
			}
		}
		return false;
	}

	/*REBouND*/
	bool __fastcall CorePlan::rebnd (AnsiString sAddress, int iPort) {

		if (cVpeer->rebnd (sAddress, iPort)) {
			lnch_ (NULL, &CorePlan::rltbl);
			return true;
		}
		return false;
	}
	
	/*LEAVE*/
	bool __fastcall CorePlan::leave (bool bAlign) {
		CoreAenp cVantientrophy (this);
		PrtoLmtb cVmutable;

		lnch_ (NULL, &CorePlan::rltbl);
		if (cVpeer->leave (&cVantientrophy, bAlign)) {
			CorePlan::lnch_ (NULL, &CorePlan::rltbl);
			lnch_ (NULL, &CorePlan::trunc);
			cVmutable.set_svnamespace (CoreAlog::sCsystemnamespace.to_string ());
			cVmutable.set_svtable (CoreAlog::sCglobalsystemtable.to_string ());
			lnch_ (&cVmutable, &CorePlan::trunc);
			cVredolog->kill_ ();
			return true;
		}
		return false;
	}

	/*SET ABort*/
	void __fastcall CorePlan::setab (bool bAbort) {

		cVpeer->setab (bAbort);
	}

	/*GET Next node TOken*/
	AnsiString __fastcall CorePlan::getnt () {

		return cVpeer->getgo ()->getnt ();
	}

	/*UPdate TaBLet*/
	bool __fastcall CorePlan::uptbl (AnsiString sTabletfilename, iCstatetype iCstate, PrtoTabl* cTablet) {
		PrtoLkey cVkey;
		PrtoLmtb cVmutable;
		PrtoLval cVvalue;

		cVkey.set_svmain (sTabletfilename.to_string ());
		cVkey.set_ivtimestamp (CoreButl::tmstl ());
		cVkey.set_ivstate (iCstate);
		cVmutable.set_svnamespace (CoreAlog::sCsystemnamespace.to_string ());
		cVmutable.set_svtable (CoreAlog::sCglobalsystemtable.to_string ());
		if (cTablet != NULL) cVvalue.set_svopaquevalue (cTablet->SerializeAsString ());
		cVvalue.set_ivtimestamp (cVkey.ivtimestamp ());
		return cVpeer->muttr (MUTATOR, &cVmutable, &cVkey, &cVvalue, CorePart::iCrandompartition, NULL, NULL, false, NULL);
	}

	/*DeLete TaBLe*/
	bool __fastcall CorePlan::dltbl (PrtoLmtb* cMutable) {
		PTR_TABLELOC cVtableloc;

		cVglobalmutex.locks ();
		cVtableloc = gettl (cMutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return true;
		return uptbl (gettn (cMutable), DELTMB, NULL);	
	}

	/*STore TaBLe*/
	bool __fastcall CorePlan::sttbl (AnsiString sTablefilename, PTR_TABLELOC cTableloc, AnsiString sXml) {
		AnsiString sVcontent;
		CoreXml_ cVdocument;
		PrtoTabl cVtablet;

		if (sXml != "") sVcontent = sXml;
		else sVcontent = CoreButl::getff (cVpool, CoreButl::ffpen (cVnodedata.sVtabletpath + sTablefilename + "." + CoreXml_::sCxmlext));
		if (sVcontent != "") {
			cVdocument.getfc (sVcontent );
			cVtablet.set_svxml (cVdocument.settc (false).to_string ());
			if (cTableloc != NULL) {
				CorePart::pptpb (&cTableloc->cVpartitionloc.cVpartition.cVpartitionpool, cVtablet.mutable_cvpartitionedelements ());
				CorePart::pptpb (&cTableloc->cVpartitionloc.cVpartitionindex.cVpartitionpool, cVtablet.mutable_cvpartitionedindexelements ());
			}
			return uptbl (sTablefilename, UPSERT, &cVtablet);
		}
		return false;
	}

	/*STore TaBLe*/
	bool __fastcall CorePlan::sttbl (PrtoLmtb* cMutable, AnsiString sXml) {
		PTR_TABLELOC cVtableloc;

		cVglobalmutex.locks ();
		cVtableloc = gettl (cMutable, false, sXml);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return true;
		return sttbl (gettn (cMutable), cVtableloc, sXml);
	}

	/*STore TaBles*/
	bool __fastcall CorePlan::sttbs () {
		bool bVreturn;
		CoreList<AnsiString> cVfilenames;
		PTR_SORTEDINDEX cVsortedfilenames = boost::shared_ptr<CoreStix> (new CoreStix (cVpool));
		PTR_TABLELOC cVtablelocnull;
		
		CoreHutl::readr (cVnodedata.sVtabletpath, (AnsiString) "*." + CoreXml_::sCxmlext, cVfilenames);
		for (int iV = 0; iV < cVfilenames.count (); iV++) cVsortedfilenames->add__ (*cVfilenames.get__ (iV));
		bVreturn = true;
		for (int iV = 0; iV < cVsortedfilenames->count (); iV++) 
			bVreturn &= sttbl (CoreButl::left_ (cVsortedfilenames->get__ (iV), "."), cVtablelocnull);	
		return bVreturn;
	}

	/*LoaD TaBLe*/
	bool __fastcall CorePlan::ldtbl (AnsiString sTabletfilename, PrtoTabl* cTable) {
		PrtoLkey cVkey;
		PrtoLmtb cVmutable;
		PrtoLval cVvalue;

		cVkey.set_svmain (sTabletfilename.to_string ());
		cVmutable.set_svnamespace (CoreAlog::sCsystemnamespace.to_string ());
		cVmutable.set_svtable (CoreAlog::sCglobalsystemtable.to_string ());
		if (cVpeer->query (&cVmutable, &cVkey, &cVvalue, CoreTble::iCexactquery, CorePart::iCrandompartition, NULL, NULL, NULL)) {
			cTable->ParseFromString (cVvalue.svopaquevalue ());
			return true;
		}
		return false;
	}

	/*LoaD TaBLe*/
	bool __fastcall CorePlan::ldtbl (PrtoLmtb* cMutable, PrtoTabl* cTable) {

		return ldtbl (gettn (cMutable), cTable);
	}

	/*ReLoad TaBLe*/
	bool __fastcall CorePlan::rltbl (PrtoLmtb* cMutable) {
		PrtoTabl cVtable;
		PTR_TABLELOC cVtableloc;

		if (ldtbl (cMutable, &cVtable)) {
			cVglobalmutex.locks ();
			cVtableloc = gettl (cMutable);
			cVglobalmutex.relse ();
			if (cVtableloc == NULL || !cVtableloc->bVvalid) return false;
			cVglobalmutex.locks ();
			cVpartitioner->pbtpp (cVtable.mutable_cvpartitionedelements (), 
														&cVtableloc->cVpartitionloc.cVpartition,
														cVtableloc->cVpartitionloc.iVcustomreplicationfactor);
			cVpartitioner->pbtpp (cVtable.mutable_cvpartitionedindexelements (), 
														&cVtableloc->cVpartitionloc.cVpartitionindex,
														cVtableloc->cVpartitionloc.iVcustomreplicationfactor);
			cVglobalmutex.relse ();
			return true;
		}
		return false;
	}

	/*Force COMPaction*/
	bool __fastcall CorePlan::fcomp (PrtoLmtb* cMutable) {
		bool bVreturn;
		PTR_TABLELOC cVtableloc;
		PTR_TABLETSERVER cVtabletserver;

		cVglobalmutex.locks ();
		cVtableloc = gettl (cMutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return true;
		cVtableloc->cVlocalmutex.locks ();
		cVtabletserver = cVpeer->gettb (cMutable);
		if (cVtabletserver.use_count () == 0) return false;
		bVreturn = cVtabletserver->fcomp ();
		cVtableloc->cVlocalmutex.relse ();
		return bVreturn;
	}

	/*debug ANaLYZe*/
	bool __fastcall CorePlan::anlyz (PrtoLmtb* cMutable, int64vcl iTimestamp) {
		AnsiString sVprevmainkey;
		AnsiString sVkey;
		PTR_TABLELOC cVtableloc;
		PrtoL2qr cVl1query;
		PrtoL2mr cVl1result;
		PrtoLkey* cVresultkey;
		PrtoL2ks* cVkeyslice;

		cVglobalmutex.locks ();
		cVtableloc = gettl (cMutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return true;
		cVl1query.mutable_cvkey_start ()->set_svmain (CorePars::sCminansistring.to_string ());
		cVl1query.mutable_cvkey_end ()->set_svmain (CorePars::sCminansistring.to_string ());
		cVl1query.mutable_cvmutable ()->CopyFrom (*cMutable);
		cVl1query.mutable_cvselect ()->Add ()->set_svcolumn (cVtableloc->cVcolumnpool.get__ (0)->sVindex.to_string ());
		cVl1query.set_ivquery (TKSLWQUERY);
		cVl1query.set_ivcount (1);
		cVl1query.set_bvonlysecondary (true);
		sVprevmainkey = CorePars::sCminansistring;

		cVtableloc->cVlocalmutex.locks ();
		while (qryl1 (&cVl1query, &cVl1result, true, cVtableloc, false)) {
			cVkeyslice = cVl1result.mutable_cvkeyslice (0);
			cVresultkey = cVkeyslice->mutable_cvkey ();
			sVkey = cVresultkey->svmain ();
			
			if (sVkey == sVprevmainkey) printf ("R %s\n", sVkey.c_str ());
			else if (cVresultkey->ivstate () != UPSERT) printf ("D %s\n", sVkey.c_str ());
			else printf ("I %s\n", sVkey.c_str ());

			//if (cVresultkey->ivtimestamp () <= iTimestamp) printf ("%s\n", sVkey.c_str ());
			//if (cVresultkey->ivstate () != UPSERT || sVkey == sVprevmainkey) continue;

			
			sVprevmainkey = sVkey;
			cVl1query.mutable_cvkey_start ()->set_svmain (sVkey.to_string ());
		}
		cVtableloc->cVlocalmutex.relse ();
		return true;
	}

	/*CLONE*/
	bool __fastcall CorePlan::clone (PrtoLmtb* cMutablesource, PrtoLmtb* cMutabledestination, int64vcl iTimestamp, int64vcl& iVreturn) {
		PrtoL2st cVstatement;
		AnsiString sVprevmainkey;
		AnsiString sVkey;
		PTR_TABLELOC cVtablelocsource;
		PTR_TABLELOC cVtablelocdestination;
		PrtoL2qr cVl1query;
		PrtoL2mr cVl1result;
		PrtoLkey* cVresultkey;
		PrtoL2ks* cVkeyslice;
		bool bVreturn;
	
		iVreturn = 0;
		cVglobalmutex.locks ();
		cVtablelocsource = gettl (cMutablesource);
		cVtablelocdestination = gettl (cMutabledestination);
		cVglobalmutex.relse ();
		if (cVtablelocsource == NULL || !cVtablelocsource->bVvalid) return false;
		if (cVtablelocdestination == NULL || !cVtablelocdestination->bVvalid) return false;
		cVstatement.mutable_cvmutable ()->CopyFrom (*cMutabledestination);
		cVstatement.set_ivtimestamp (iTimestamp);
		cVl1query.mutable_cvkey_start ()->set_svmain (CorePars::sCminansistring.to_string ());
		cVl1query.mutable_cvkey_end ()->set_svmain (CorePars::sCminansistring.to_string ());
		cVl1query.mutable_cvmutable ()->CopyFrom (*cMutablesource);
		for (int iV = 0; iV < cVtablelocsource->cVcolumnpool.count (); iV++) 
			cVl1query.mutable_cvselect ()->Add ()->set_svcolumn (cVtablelocsource->cVcolumnpool.get__ (iV)->sVindex.to_string ());	
		cVl1query.set_ivquery (TKSLWQUERY);
		cVl1query.set_ivcount (1);
		sVprevmainkey = CorePars::sCminansistring;
		bVreturn = false;
		cVtablelocsource->cVlocalmutex.locks ();
		while (qryl1 (&cVl1query, &cVl1result, false, cVtablelocsource, false)) {
			cVkeyslice = cVl1result.mutable_cvkeyslice (0);
			cVresultkey = cVkeyslice->mutable_cvkey ();
			sVkey = cVresultkey->svmain ();
			if (sVkey != sVprevmainkey && cVresultkey->ivstate () == UPSERT) {
				iVreturn++;
				cVtablelocdestination->cVlocalmutex.locks ();
				cVstatement.mutable_cvkey ()->CopyFrom (cVkeyslice->cvkey ());
				cVstatement.mutable_cvkey ()->set_ivtimestamp (cVstatement.ivtimestamp ());				
				cVstatement.mutable_cvcolumns ()->CopyFrom (cVkeyslice->cvcolumns ());
				stati (&cVstatement, cVtablelocdestination, true, false,false);
				cVtablelocdestination->cVlocalmutex.relse ();
				bVreturn = true;
			}
			sVprevmainkey = sVkey;
			cVl1query.mutable_cvkey_start ()->set_svmain (sVkey.to_string ());
		}
		cVtablelocsource->cVlocalmutex.relse ();
		return bVreturn;
	}


	/*InFo TaBlet*/
	bool __fastcall CorePlan::iftbl (PrtoLmtb* cMutable, PrtoTabl* cTablet, bool bDiagnostic) {
		PTR_TABLELOC cVtableloc;
		CorePart::PARTITIONLOC* cVpartitionloc;

		if (!touch (cMutable)) return false;
		cVtableloc = gettl (cMutable);
		cVpartitionloc = &cVtableloc->cVpartitionloc;
		if (cVpartitionloc->cVpartition.iVpartitiontype == CorePart::iCcustompartition) 
			CorePart::pptpb (&cVpartitionloc->cVpartition.cVpartitionpool, cTablet->mutable_cvpartitionedelements ());
		if (cVpartitionloc->cVpartitionindex.iVpartitiontype == CorePart::iCcustompartition) 
			CorePart::pptpb (&cVpartitionloc->cVpartitionindex.cVpartitionpool, cTablet->mutable_cvpartitionedindexelements ());
		cTablet->set_svpartitiontype (getpl (cVtableloc->iVpartitiontype).to_string ());
		return cVpeer->iftbl (gettn (cMutable), cTablet, bDiagnostic);
	}

	/*STATS*/
	bool __fastcall CorePlan::stats (iCstatstype iStatstype, PrtoLmtb* cMutable, PrtoStel* cStatselement) {
		PTR_TABLELOC cVtableloc;
		STATSLOC cVstatsloc;

		if (iStatstype != CorePlan::iCreadstats && iStatstype != CorePlan::iCwritestats) return false;
		if (cMutable->svtable () == "") {
			for (int iV = 0; iV < cVtablepool.count (); iV++) {
				cVtableloc = cVtablepool.get__ (iV);
				cVstatsloc.iVcount += cVtableloc->cVstatsloc [iStatstype].iVcount;
				cVstatsloc.dVmeandelay += cVtableloc->cVstatsloc [iStatstype].dVmeandelay;
				cVstatsloc.dVworstdelay += cVtableloc->cVstatsloc [iStatstype].dVworstdelay;
				cVstatsloc.iVsize += cVtableloc->cVstatsloc [iStatstype].iVsize;
			}
			cStatselement->set_ivcount (cVstatsloc.iVcount);
			cStatselement->set_ivmeandelay (CoreButl::tmstp (cVstatsloc.dVmeandelay) / Max (cVtablepool.count (), 1));
			cStatselement->set_ivworstdelay (CoreButl::tmstp (cVstatsloc.dVworstdelay) / Max (cVtablepool.count (), 1));
			cStatselement->set_ivsize (cVstatsloc.iVsize);
			cStatselement->set_ivelapsed (CoreButl::tmstl () - cVglobalstats.iVstartuptime);
			return true;
		} else {
			cVtableloc = gettl (cMutable);
			if (cVtableloc != NULL && cVtableloc->bVvalid) {
				cStatselement->set_ivcount (cVtableloc->cVstatsloc [iStatstype].iVcount);
				cStatselement->set_ivmeandelay (CoreButl::tmstp (cVtableloc->cVstatsloc [iStatstype].dVmeandelay));
				cStatselement->set_ivworstdelay (CoreButl::tmstp (cVtableloc->cVstatsloc [iStatstype].dVworstdelay));
				cStatselement->set_ivsize (cVtableloc->cVstatsloc [iStatstype].iVsize);
				return true;
			} 
			return false;
		}
	}

	/*GET TaBlet server*/
	PTR_TABLETSERVER CorePlan::gettb (PrtoLmtb* cMutable) {

		return cVpeer->gettb (cMutable);
	}

	/*Push Pull MatCH*/
	bool __fastcall CorePlan::ppmch (PrtoAenp* cAntientrophy) {
		CoreAenp cVantientrophy (this);

		touch (cAntientrophy->mutable_cvmutable ());
		return cVantientrophy.ppmch (cAntientrophy);
	}

	/*Push Pull REPair*/
	bool __fastcall CorePlan::pprep (PrtoAenp* cAntientrophy) {
		CoreAenp cVantientrophy (this);

		touch (cAntientrophy->mutable_cvmutable ());
		return cVantientrophy.pprep (cAntientrophy);
	}

	/*PULL*/
	bool __fastcall CorePlan::pulls () {

		return cVpeer->pull_ ();
	}

	/*PULL*/
	bool __fastcall CorePlan::pulld (AnsiString sNodeid) {
		CoreAenp cVantientrophy (this);

		lnch_ (NULL, &CorePlan::touch);
		return cVantientrophy.rpair (sNodeid,  true, true);
	}

	/*GET ParTition*/
	double __fastcall CorePlan::getpt (PrtoLmtb* cMutable, bool bIndex, AnsiString& sPartition, double dPartition ) {
		AnsiString sVreturn;
		PrtoLmtb cVmutable;
		PrtoLkey cVkey;
		PrtoLval cVvalue;
		double dVsize;
		double dVtotalsize;
		double dVtargetsize;

		cVmutable.CopyFrom (*cMutable);
		if (bIndex) CoreAlog::tb2ix (&cVmutable); 
		cVkey.set_svmain (CorePars::sCminansistring.to_string ());
		dVtargetsize = dVtotalsize = 0;
		while (query (&cVmutable, &cVkey, &cVvalue, CoreTble::iCtkslwquery, CorePart::iCnopartition, NULL, NULL, NULL) && 
			cVkey.svmain () != CorePars::sCminansistring.to_string ()) {			
			dVtotalsize += cVvalue.ByteSize ();
			if (sPartition != "" && dVtargetsize == 0 && sPartition < (AnsiString) cVkey.svmain ()) dVtargetsize = dVtotalsize;
		}
		dVsize = dVtargetsize;
		if (dVtargetsize == 0) {
			dVtargetsize = dVtotalsize * dPartition;
			cVkey.set_svmain (CorePars::sCminansistring.to_string ());
			dVsize = 0;
			while (query (&cVmutable, &cVkey, &cVvalue, CoreTble::iCtkslwquery, CorePart::iCnopartition, NULL, NULL, NULL) && 
				cVkey.svmain () != CorePars::sCminansistring.to_string () && dVsize < dVtargetsize) {			
				dVsize += cVvalue.ByteSize ();
			}
			sPartition = cVkey.svmain ();
		} 
		return dVsize / dVtotalsize;
	}

	/*manual SPLIT*/
	bool __fastcall CorePlan::split (PrtoLmtb* cMutable, AnsiString sPartition, AnsiString sPartitionindex) {
		PTR_TABLELOC cVtableloc;

		cVglobalmutex.locks ();
		cVtableloc = gettl (cMutable);
		cVglobalmutex.relse ();
		if (cVtableloc == NULL || !cVtableloc->bVvalid) return false;
		cVtableloc->cVpartitionloc.cVpartition.sVjoincustompartition = sPartition;
		cVtableloc->cVpartitionloc.cVpartitionindex.sVjoincustompartition = sPartitionindex;
		cVtableloc->cVpartitionloc.bVmanualpartition = true;
		return true;
	}

	/*SET Seda Manager*/
	bool __fastcall CorePlan::setsm (bool bActive) {

		if (!bActive) return cVsedamanager->stop_ ();
		else return cVsedamanager->start ();
	}

	/*TOP*/
	bool __fastcall CorePlan::top__ (PrtoTop_& cVtop) {
		PTR_TABLELOC cVtableloc;
		STATSLOC cVstatsloc;
		PrtoTope* cVtopelement;
		PrtoLmtb cVmutable;
		CoreAlog::TABLETLOC* cVtabletloc;
		int64vcl iVreadcount;
		int64vcl iVwritecount;

		for (int iV = 0; iV < cVtablepool.count (); iV++) {
			cVtableloc = cVtablepool.get__ (iV);
			cVtableloc->cVlocalmutex.locks ();
			cVtopelement = cVtop.add_cvtopelement ();
			cVtopelement->set_svtablet (cVtableloc->sVindex.to_string ());
			cVtopelement->set_ivreadtime (CoreButl::tmstp (cVtableloc->cVstatsloc [CorePlan::iCreadstats].dVtotaldelay));
			cVtopelement->set_ivwritetime (CoreButl::tmstp (cVtableloc->cVstatsloc [CorePlan::iCwritestats].dVtotaldelay));
			cVtopelement->set_ivreadcountl2 (cVtableloc->cVstatsloc [CorePlan::iCreadstats].iVcount);
			cVtopelement->set_ivwritecountl2 (cVtableloc->cVstatsloc [CorePlan::iCwritestats].iVcount);
			iVreadcount = iVwritecount = 0;
			CoreAlog::settk (cVtableloc->sVindex, &cVmutable);
			for (int iV = 0; iV < 2; iV++) {
				cVtabletloc = cVredolog->gettb (&cVmutable, false, false);
				if (cVtabletloc == NULL) continue;
				iVreadcount += cVtabletloc->iVreadcount;
				iVwritecount += cVtabletloc->iVwritecount;
				CoreAlog::tb2ix (&cVmutable);
			}
			cVtopelement->set_ivreadcountl1 (iVreadcount);
			cVtopelement->set_ivwritecountl1 (iVwritecount);
			cVtableloc->cVlocalmutex.relse ();
		}
		cVtop.set_ivtimestamp (CoreButl::tmstl ());
		return true;
	}

	/*GET Partition Type*/
	CorePart::iCpartitiontype CorePlan::getpt (iCpartitiontype iTablepartitiontype, bool bIndex) {

		switch (iTablepartitiontype) {
			case iCnopartition: return CorePart::iCnopartition;
			case iCrandompartition: return CorePart::iCrandompartition;
			case iCbyteorderpartition: return CorePart::iCbyteorderpartition;
			case iCcustompartition: return CorePart::iCcustompartition;
			case iCbalancedcustompartition: return bIndex ? CorePart::iCcustompartition : CorePart::iCrandompartition;
			default:
				return CorePart::iCnopartition;
		}
	}

	/*GET Partition Label*/
	AnsiString CorePlan::getpl (iCpartitiontype iPartitiontype) {

		switch (iPartitiontype) { 
			case iCrandompartition: return "RANDOM PARTITION";
			case iCbyteorderpartition: return "BYTE ORDER PARTITION";
			case iCcustompartition: return "CUSTOM PARTITION";
			case iCbalancedcustompartition: return "BALANCED CUSTOM PARTITION";
			default: return "NO PARTITION";
		}	
	}