#include "CoreTbsv.hpp"
#include "CoreHutl.hpp"
#include "CoreButl.hpp"
#include "TmplList.cpp"
#include "TmplFsys.cpp"
#include "CorePlan.hpp"
#include "CoreAutl.hpp"

using namespace google;
using namespace orion;

//#define iCdebugmutator
//#define iCdebugquery

	const int CoreTbsv::iCmaxmemtabledim = 100000000;
	const int CoreTbsv::iCmaxmemtablesize = 2000 * 1024 * 1024;
	const char CoreTbsv::bCmaindelimiter = '/';
	const char CoreTbsv::bCmajordelimiter = '|';
	const char CoreTbsv::bCqualifierdelimiter = ':';
	const int CoreTbsv::iCredotimestampsize = sizeof (int64vcl) + sizeof (int);
	const AnsiString CoreTbsv::sCchar0 = CoreButl::chr2s (0);
	const AnsiString CoreTbsv::sCchar1 = CoreButl::chr2s (1);
	const AnsiString CoreTbsv::sCcleanredotimestamp = CoreButl::lpadt (sCchar0, sCchar0, iCredotimestampsize);
	const AnsiString CoreTbsv::sCfastkeyupsertdelimiters = 
		(AnsiString) bCmaindelimiter + (AnsiString) bCqualifierdelimiter + (AnsiString) bCmajordelimiter + (char) bCupsert;
	const AnsiString CoreTbsv::sCfastkeydeletedelimiters = 
		(AnsiString) bCmaindelimiter + (AnsiString) bCqualifierdelimiter + (AnsiString) bCmajordelimiter + (char) bCdelete;


  __fastcall CoreTbsv::CoreTbsv () {

		sVtable =  "";
		cVreverse = boost::shared_ptr<CoreRvrs> (new CoreRvrs ());
	}

  __fastcall CoreTbsv::~CoreTbsv () {

		LOG(INFO) << "T stopping: " << sVtable.c_str ();
		FlushLogFiles(GLOG_INFO);
		mnicp (!(bVvolatiletable || bVmemorytable));
		cVserializer.reset ();
		LOG(INFO) << "Tablet ends:" << sVtable.c_str ();
		FlushLogFiles(GLOG_INFO);
	}

	/*INIT*/
  void __fastcall CoreTbsv::init_	(	PTR_POOL cPool, PTR_SEDAMANAGER cSedamanager,
																		AnsiString sNamespace, AnsiString sTable,
																		AnsiString sDatapath, AnsiString sTabletspath,
																		AnsiString sSystemkey, int iGracetime,
																		PTR_TABLETSERVER cTabletsystem, bool bResetlocal) {
		AnsiString sVbuildpath;
		AnsiString sVxmlfilename;

		sVnamespace = sNamespace;
		sVbuildpath = sDatapath + sNamespace + "/";
		if (!CoreHutl::ispvl (sVbuildpath)) CoreHutl::crdir (sVbuildpath);
		cVpool = cPool;
		cVsedamanager = cSedamanager;
		sVpath = sVbuildpath;
		sVtable = sVfilename = sTable;
		iVgracetimestamp = int64vcl ((double) iGracetime * CoreButl::dCtmstlfactor);
		cVserializer = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));		
		sVsystemkey = sSystemkey;
		cVtabletsystem = cTabletsystem;
		bVtabletsystem = cVtabletsystem.use_count () == 0;
		iVmemtabledim = iCmaxmemtabledim;
		iVmemtablesize = iCmaxmemtablesize;		
		iVmemtablechunk = iCmaxmemtabledim;
		bVmemorytable = bVvolatiletable = bVfastkey = false;
		bVabort = false;
		iVpartitiontype = CorePart::iCnopartition;
		sVredotimestamp = sCcleanredotimestamp;
		iVsstablecounter = 0;
		loada (bResetlocal);
		if (cVactivememloc == NULL) newmt (true, sVredotimestamp);
		if (bVtabletsystem) mincp (true);

		LOG(INFO) <<		"Tablet: " << sVtable.c_str () << 
										", path: " << sVbuildpath.c_str () <<
										", init";
		FlushLogFiles(GLOG_INFO);
	}

	/*SET StoraGe*/
	void __fastcall CoreTbsv::setsg (	PrtoLstr* cStorage) {

		if (cStorage != NULL) {
			cVdbopmutex.locks ();
			if (cStorage->ivmemtabledim () > 0) iVmemtabledim = cStorage->ivmemtabledim ();
			if (cStorage->ivmemtablesize () > 0) iVmemtablesize = cStorage->ivmemtablesize ();
			iVmemtablechunk = iVmemtabledim;
			if (cStorage->ivmemtablechunk () > 0) iVmemtablechunk = cStorage->ivmemtablechunk ();
			bVmemorytable = cStorage->bvmemorytable ();
			bVvolatiletable = cStorage->bvvolatiletable ();
			bVfastkey = cStorage->bvfastkey ();
			cVactivememloc->cVmemtable->setmx (iVmemtabledim, iVmemtablesize, iVmemtablechunk);
			iVpartitiontype = (CorePart::iCpartitiontype) cStorage->ivpartitiontype ();
			cVdbopmutex.relse ();

			LOG(INFO) <<	"Tablet: " << sVtable.c_str () << 
										", dim: " << iVmemtabledim <<
										", size: " << iVmemtablesize <<
										", chunk: " << iVmemtablechunk <<
										", type: " << iVpartitiontype <<
										", memory: " << bVmemorytable <<
										", volatile: " << bVvolatiletable <<
										", storage";
			FlushLogFiles(GLOG_INFO);
		}
	}

  /*MUTaToR*/
	bool __fastcall CoreTbsv::muttr (PrtoLkey* cKey, PrtoLval* cValue, AnsiString sRedotimestamp) {
		AnsiString sVkey;
		bool bVreturn;
		bool bVput;
		std::string sVtimestamp;
		int iVmainindexlength;
		PrtoSeda cVmessage;


		//printf ("1debug %s\n", fkey_ (cKey, iVmainindexlength).debug ().c_str ());
		//printf ("2debug %s\n", key__ (cVreverse, cKey, iVmainindexlength).debug ().c_str ());

		if (bVfastkey ) sVkey = fkey_ (cKey, iVmainindexlength);
		else sVkey = key__ ( cVreverse, cKey, iVmainindexlength);
		
#ifdef iCdebugmutator
		printf ("L %s-%s\n", sVtable.c_str (),((AnsiString) cKey->svmain ()).prntb ().c_str ());
#endif
		
		bVput = true;
		cVdbopmutex.locks ();
		if (cKey->ivstate () == DELTMB) {			
			sVtimestamp = CoreButl::i642s (cVreverse->descl (CoreButl::tmstl ())).to_string ();				
			cValue->Clear ();
			cValue->set_svopaquevalue (sVtimestamp);
			cValue->set_ivtimestamp (cKey->ivtimestamp ());
			bVput = !(bVvolatiletable && cVmemtablepool.count () == 1);
		}
		if (cVactivememloc->cVmemtable->full_ ()) {
			if (bVvolatiletable) {
				cVdbopmutex.relse ();
				return false;
			} else if (!cVactivememloc->bVneedcompaction) {
				cVmessage.set_svnamespace (sVnamespace.to_string ());
				cVmessage.set_svtable (sVtable.to_string ());
				cVactivememloc->bVneedcompaction = true;
				cVsedamanager->pushs (CorePlan::sCflushwriter, cVmessage);	
			}
		} else if (cVactivememloc->cVmemtable->chunk ()) {
			cVmessage.set_svnamespace (sVnamespace.to_string ());
			cVmessage.set_svtable (sVtable.to_string ());
			cVsedamanager->pushs (CorePlan::sCmemtablepartial, cVmessage);
		}
		if (sRedotimestamp != NULL) cVactivememloc->sVredotimestamp = Max (cVactivememloc->sVredotimestamp, sRedotimestamp);
		else cVactivememloc->sVredotimestamp = CoreTbsv::sCcleanredotimestamp;
		if (bVput) {
			bVreturn = cVactivememloc->cVmemtable->put__ (sVkey, iVmainindexlength, cValue);
		} else bVreturn = cVactivememloc->cVmemtable->delte (sVkey, iVmainindexlength);
		cVdbopmutex.relse ();
		return bVreturn;
	}

	/*IS ToMBstone*/
	bool __fastcall CoreTbsv::istmb (	PTR_REVERSE cReverse, AnsiString sKey) {
		PrtoLkey cVkey;

		key__ (cReverse, sKey, &cVkey);
		return cVkey.ivstate () == DELTMB;
	}

	/*EXpired ToMBstone*/
	bool __fastcall CoreTbsv::extmb (	PTR_REVERSE cReverse, AnsiString sOpaquevalue, int64vcl iGracetimestamp) {
		int64vcl iVtombstonetimestamp;

		iVtombstonetimestamp = cReverse->rdesl (*((int64vcl*) sOpaquevalue.c_str ()));
		return (CoreButl::tmstl () - iGracetimestamp >= iVtombstonetimestamp);
	}

	/*GET Table Counter*/
	int __fastcall CoreTbsv::gettc () {

		return cVmemtablepool.count () + cVsstablepool.count ();
	}

	/*GET Table Element*/
	bool __fastcall CoreTbsv::gette ( int iElement, CoreTble::PTR& cTable) {
		PTR_MEMTABLELOC cVmemtableloc;
		PTR_SSTABLELOC cVsstableloc;
		int iVmemtablepoolcount;

		iVmemtablepoolcount = cVmemtablepool.count ();
		if (iElement < iVmemtablepoolcount) {
			cVmemtableloc = cVmemtablepool.get__ (iElement)->cVptr;
			cTable = cVmemtableloc->cVmemtable;
			return cVmemtableloc->bVvalid;
		} else {
			cVsstableloc = cVsstablepool.get__ (iElement - iVmemtablepoolcount)->cVptr;
			cTable = cVsstableloc->cVsstable;
			return cVsstableloc->bVvalid;
		}
	}

	/*No STATe*/
	AnsiString __fastcall CoreTbsv::nstat (AnsiString sKey) {

		return sKey.SubString (0, sKey.Length () - 2);
	}

	/*QUERY*/
	bool __fastcall CoreTbsv::query (PrtoLkey* cKey, PrtoLval* cValue, CoreTble::iCquerytype iQuerytype) {
		int iVdummy;
		bool bViteratorupsert;
		bool bVreturniteratorupsert;
		AnsiString sViteratorkey;
		AnsiString sVbuiltkey;
		AnsiString sVdiscardkey;
		AnsiString sVdiscardkey2;
		AnsiString sVcompareiteratorkey;
		AnsiString sVcompletekey;
		AnsiString sVcandidatekey;
		AnsiString sVcandidatecompletekey;
		AnsiString sVcomparableiteratorkey;
		AnsiString sVsavediteratorkey;
		AnsiString sVreturnkey;
		CoreTble::PTR cVtable;
		PrtoLval cVcandidatevalue;
		
		cValue->Clear ();		
		sVcandidatekey = NULL;

#ifdef iCdebugquery
		printf (">>>>>\n");
#endif

		switch (iQuerytype) {

			case CoreTble::iCexactquery:
				sVbuiltkey = key__ ( cVreverse, cKey, iVdummy, false);
				//sVbuiltkey = fkey_ (cKey, iVdummy);
				//printf ("qui %s %i\n", sVbuiltkey.c_str (), bVfastkey);

				cVdbopmutex.locks ();
				for (int iV = 0; iV < gettc (); iV++) {
					if (!gette (iV, cVtable)) continue;
					sViteratorkey = sVbuiltkey;
					if (	cVtable->get__ (sViteratorkey, &cVcandidatevalue) && 
								(sViteratorkey.SubString (0, sVbuiltkey.Length ()) == sVbuiltkey)) {


						if (sVcandidatekey == NULL || sVcandidatekey > sViteratorkey) {
							sVcandidatekey = sViteratorkey;
							cValue->CopyFrom (cVcandidatevalue);

#ifdef iCdebugquery
							printf ("%s %i-> %s %s\n", cVtable->getid ().c_str (), iV, sVcandidatekey.prntb ().c_str (), sVcandidatekey.debug ().c_str ());
#endif							

							cKey->set_ivtimestamp (cValue->ivtimestamp ());
						}
						continue;
					} 
					
#ifdef iCdebugquery					
					else printf ("! %s %i %s\n", cVtable->getid ().c_str (), iV, sViteratorkey.prntb ().c_str ());
#endif

				}
				cVdbopmutex.relse ();
				return (sVcandidatekey != NULL && sVcandidatekey.c_str () [sVcandidatekey.Length () - 1] == (char) bCupsert);
				break;		
			case CoreTble::iCrangequery:	

				sVbuiltkey = key__ ( cVreverse, cKey, iVdummy, false);

				sVdiscardkey2 = "";
				cVdbopmutex.locks ();

				do {
					sVreturnkey = NULL;
					for (int iV = 0; iV < gettc (); iV++) {
						if (!gette (iV, cVtable)) continue;
						sViteratorkey = sVbuiltkey;					
						sVdiscardkey = "";
						while (true) {
							if (!cVtable->get__ (sViteratorkey, &cVcandidatevalue)) break;					
							sVcompareiteratorkey = getmj (sViteratorkey);
							sVcomparableiteratorkey = sViteratorkey;
							bViteratorupsert = sViteratorkey.c_str () [sViteratorkey.Length () - 1] == (char) bCupsert;
							sViteratorkey += sCchar1;
							if (sVcompareiteratorkey == sVdiscardkey || sVcompareiteratorkey == sVdiscardkey2) continue;
							if (sVcandidatekey == NULL || sVcandidatekey > sVcomparableiteratorkey) {
								sVsavediteratorkey = sViteratorkey;
								sVcandidatekey = sVcomparableiteratorkey;
								sVreturnkey = sVcompareiteratorkey;
								bVreturniteratorupsert = bViteratorupsert ;
								cValue->CopyFrom (cVcandidatevalue);
								cKey->set_ivtimestamp (cValue->ivtimestamp ());
							}
							break;
						}
					}

					sVbuiltkey = sVsavediteratorkey;
					//sVbuiltkey = sViteratorkey; 
					
					sVdiscardkey2 = sVreturnkey;
					//sVdiscardkey2 = sVcompareiteratorkey;
					
					sVcandidatekey = NULL;

				} while (sVreturnkey != NULL && !bVreturniteratorupsert);

				cVdbopmutex.relse ();
				if (sVreturnkey != NULL && bVreturniteratorupsert) {
					cKey->set_svmain (getmn (sVreturnkey).to_string ());
					return true;
				} else return false;
				break;			

			case CoreTble::iCtokenquery:				
				sVbuiltkey = key__ ( cVreverse, cKey, iVdummy, false);		
				cVdbopmutex.locks ();
				do {
					sVreturnkey = NULL;
					for (int iV = 0; iV < gettc (); iV++) {
						if (!gette (iV, cVtable)) continue;
						sVdiscardkey = sViteratorkey = sVbuiltkey;					
						while (true) {
							if (!cVtable->get__ (sViteratorkey, &cVcandidatevalue)) break;
							sVcompareiteratorkey = getmj (sViteratorkey);
							sVcomparableiteratorkey = sViteratorkey;
							bViteratorupsert = sViteratorkey.c_str () [sViteratorkey.Length () - 1] == (char) bCupsert;
							sViteratorkey += sCchar1;
							if (sVcompareiteratorkey == sVdiscardkey) continue;
							if (sVcandidatekey == NULL || sVcandidatekey > sVcomparableiteratorkey) {
								sVcandidatekey = sVcomparableiteratorkey;
								sVreturnkey = sVcompareiteratorkey;
								bVreturniteratorupsert = bViteratorupsert ;
								cValue->CopyFrom (cVcandidatevalue);
								cKey->set_ivtimestamp (cValue->ivtimestamp ());
							}
							break;
						}
					}
					 
					sVbuiltkey = sVreturnkey;
					//sVbuiltkey = sVcompareiteratorkey;

					sVcandidatekey = NULL;
				} while (sVreturnkey != NULL && !bVreturniteratorupsert);
				cVdbopmutex.relse ();
				if (sVreturnkey != NULL && bVreturniteratorupsert) {
					cKey->set_svmain (getmn (sVreturnkey).to_string ());
					return true;
				} else return false;
				break;

			case CoreTble::iCtkslwquery:
				sVbuiltkey = key__ ( cVreverse, cKey, iVdummy, true);
				cVdbopmutex.locks ();
				for (int iV = 0; iV < gettc (); iV++) {
					if (!gette (iV, cVtable)) continue;
					sVdiscardkey = nstat (sVbuiltkey);
					sViteratorkey = sVbuiltkey;					
					while (true) {
						if (!cVtable->get__ (sViteratorkey, &cVcandidatevalue)) break;
						sVcompareiteratorkey = nstat (sViteratorkey);
						sVcompletekey = sViteratorkey;
						sViteratorkey += sCchar0;
						if (sVcompareiteratorkey == sVdiscardkey) continue;
						if (sVcandidatekey == NULL || sVcandidatekey > sVcompareiteratorkey) {
							sVcandidatekey = sVcompareiteratorkey;
							sVcandidatecompletekey = sVcompletekey;
							cValue->CopyFrom (cVcandidatevalue);
						}
						break;
					}
				}
				cVdbopmutex.relse ();
				if (sVcandidatekey != NULL) {
					key__ (cVreverse, sVcandidatecompletekey, cKey);
					return true;
				} else return false;

		}		
		return false;
	}

	/*GET Next Sstable filename*/
	AnsiString __fastcall CoreTbsv::getns () {
		AnsiString sVreturn;

		sVreturn = sVfilename + CoreSstb::sCversiondelimiter + (AnsiString) iVsstablecounter;
		iVsstablecounter++;
		return sVreturn.UpperCase ();		
	}

	/*MINor Compaction Internal*/
	bool __fastcall CoreTbsv::minci (bool bForcenew) {
		CoreSstb::PTR cVsstable;
		PTR_MEMTABLELOC cVmemtableloc;
		PTR_SSTABLELOC cVsstableloc;

		LOG(INFO) << "Minor comp:" << sVtable.c_str () << ", is memorytable: " << (bVmemorytable ? "true" : "false");
		FlushLogFiles(GLOG_INFO);

		if (bForcenew) newmt (true, sVredotimestamp);
		if (bVvolatiletable) return true;
		cVdbopmutex.locks ();
		for (int iV = 0; iV < cVmemtablepool.count (); iV++) {
			cVmemtableloc = cVmemtablepool.get__ (iV)->cVptr;
			if (cVmemtableloc->bVvalid && cVactivememloc != cVmemtableloc && 
					(cVmemtableloc->bVneedcompaction || bVtabletsystem)) {
				cVsstableloc = cVsstablepool.set__ ()->cVptr;
				cVsstableloc->sVredotimestamp = cVmemtableloc->sVredotimestamp;
				sVredotimestamp = Max (sVredotimestamp, cVsstableloc->sVredotimestamp);
				cVsstableloc->iVcompactionlevel = 0;
				cVsstable = cVsstableloc->cVsstable;
				cVsstable->init (cVpool);
				cVsstable->setmd (sVpath, getns ());
				cVdbopmutex.relse ();

				cVsstable->saved (cVmemtableloc->cVmemtable, 
					bVmemorytable ? CoreHsrx::iCmemorysstable : CoreHsrx::iCsstable, 
					cVsstableloc->sVredotimestamp);

				cVdbopmutex.locks ();
				cVsstableloc->cVlinkedmemtableloc = cVmemtableloc;
				cVsstableloc->bVvalid = !bVmemorytable;
				cVsstableloc->bVvalidasmemorytable = cVmemtableloc->bVvalid = bVmemorytable;
				if (!cVsstableloc->bVvalidasmemorytable) cVmemtableloc->bVdeadmanwalking = true;
			}
		}	
		cVdbopmutex.relse ();
		return rcyci ();
	}

	/*MINor ComPaction*/
	bool __fastcall CoreTbsv::mincp (bool bForcenew) {
		bool bVreturn;
		PrtoSeda cVmessage;
		
		cVcompactionmutex.locks ();
		if (cVactivememloc != NULL && cVactivememloc->cVmemtable->getsd () > 0) {
			bVreturn = minci (bForcenew);
			if (bVtabletsystem) {
				cVmessage.set_svnamespace (sVnamespace.to_string ());
				cVmessage.set_svtable (sVtable.to_string ());
				cVmessage.set_ivcompactiontype (MERGE);
				cVsedamanager->pushs (CorePlan::sCmemtablepostflusher, cVmessage);
			}
		} else bVreturn = false;
		cVcompactionmutex.relse ();
		return bVreturn;
	}

	/*MiNI Compaction Internal*/
	bool __fastcall CoreTbsv::mnici (bool bNotmemory) {
		CoreSstb::PTR cVsstable;
		PTR_SSTABLELOC cVsstableloc;
		CoreHsrx::iCsstabletypes iVtype;


		LOG(INFO) << "Mini comp: " << sVtable.c_str () << ", is memorytable: " <<  (bVmemorytable ? "true" : "false");
		FlushLogFiles(GLOG_INFO);

		if (bVvolatiletable) return true;
		cVdbopmutex.locks ();
		for (int iV = 0; iV < cVsstablepool.count (); iV++) {
			cVsstableloc = cVsstablepool.get__ (iV)->cVptr;
			if (cVsstableloc->cVlinkedmemtableloc == cVactivememloc) invss (cVsstableloc, NULL, false);
		}
		cVsstableloc = cVsstablepool.set__ ()->cVptr;
		cVsstableloc->sVredotimestamp = cVactivememloc->sVredotimestamp;
		sVredotimestamp = Max (sVredotimestamp, cVsstableloc->sVredotimestamp);
		cVsstableloc->iVcompactionlevel = 0;
		cVsstable = cVsstableloc->cVsstable;
		cVsstable->init (cVpool);
		cVsstable->setmd (sVpath, getns ());

		if (bNotmemory) {
			cVsstableloc->bVvalid = true;
			cVsstableloc->bVvalidasmemorytable = false;
			iVtype = CoreHsrx::iCsstable;
		} else {
			cVsstableloc->bVvalid = false;
			cVsstableloc->bVvalidasmemorytable = true;
			cVsstableloc->cVlinkedmemtableloc = cVactivememloc;
			iVtype = CoreHsrx::iCpartialmemtable;
		}

		cVsstable->saved (cVactivememloc->cVmemtable, iVtype, cVsstableloc->sVredotimestamp);
		cVdbopmutex.relse ();
		return rcyci (true, false);
	}

	/*MiNI ComPaction*/
	bool __fastcall CoreTbsv::mnicp (bool bNotmemory) {
		bool bVreturn;
		PrtoSeda cVmessage;

		cVcompactionmutex.locks ();
		if (cVactivememloc == NULL || cVactivememloc->cVmemtable->getsd () == 0 || bVabort) bVreturn = false;
		else bVreturn = mnici (bNotmemory);
		cVcompactionmutex.relse ();
		return bVreturn;
	}

	/*INValidate SStable*/
	bool __fastcall CoreTbsv::invss (PTR_SSTABLELOC cSstableloc, AnsiString sRedotimestamp, bool bAlsolinkedmemtable) {

		cSstableloc->bVdeadmanwalking = true;
		cSstableloc->bVvalid = false;
		if (cSstableloc->bVvalidasmemorytable) {
			cSstableloc->bVvalidasmemorytable = false;
			if (bAlsolinkedmemtable) {
				if (cSstableloc->cVlinkedmemtableloc != NULL) cSstableloc->cVlinkedmemtableloc->bVvalid = false;
				if (cVactivememloc == cSstableloc->cVlinkedmemtableloc) newmi (true, sRedotimestamp); 
			}
		} 		
		return true;
	}

	/*MAJor Compaction Internal*/
	bool __fastcall CoreTbsv::majci (	PTR_SSTABLELOC cS1, PTR_SSTABLELOC cS2, PTR_SSTABLELOC cSresult, int iCompactionlevel, 
																		bool bMemorytable) {
		bool bVreturn;
		AnsiString sVmaxredotimestamp;


		cSresult->cVsstable->setmd (sVpath, getns ());

		LOG(INFO) << "Major comp:" << cSresult->cVsstable->getfn ().c_str() << " = " << 
								 cS1->cVsstable->getfn ().c_str() << " + " << cS2->cVsstable->getfn ().c_str() <<
								 ", comp lev: " << (iCompactionlevel + 1);
		FlushLogFiles(GLOG_INFO);

		sVmaxredotimestamp = Max (cS1->sVredotimestamp, cS2->sVredotimestamp);
		bVreturn = cSresult->cVsstable->majcp (cS1->cVsstable, cS2->cVsstable, sVmaxredotimestamp);
		cSresult->iVcompactionlevel = iCompactionlevel + 1;
		
		LOG(INFO) << "Major comp:" << cSresult->cVsstable->getfn ().c_str() << 
									", result: " << (bVreturn ? "Ok" : "Ko");
		FlushLogFiles(GLOG_INFO);

		cVdbopmutex.locks ();
		if (bMemorytable) bVreturn &= sstme (cSresult, sVredotimestamp);				
		if (bVreturn) {
			cSresult->bVvalid = !bMemorytable;
			cSresult->bVvalidasmemorytable = bMemorytable;
			invss (cS1, sVmaxredotimestamp, true);
			invss (cS2, sVmaxredotimestamp, true);
		}
		cVdbopmutex.relse ();		
		return bVreturn;
	}

	/*ReCYCle Internal*/
	bool __fastcall CoreTbsv::rcyci (bool bSstables, bool bMemtables) {
		PTR_SSTABLELOC cVsstableloc;
		PTR_MEMTABLELOC cVmemtableloc;

		cVdbopmutex.locks ();
		if (bSstables)
			for (int iV = cVsstablepool.count () - 1; iV >= 0; iV--) {
				cVsstableloc = cVsstablepool.get__ (iV)->cVptr;
				if (cVsstableloc->bVdeadmanwalking) {
					cVsstableloc->cVsstable->delfs ();	
					cVsstablepool.unset (iV);
				}
			}
		if (bMemtables)
			for (int iV = cVmemtablepool.count () - 1; iV >= 0; iV--) {
				cVmemtableloc = cVmemtablepool.get__ (iV)->cVptr;
				if (cVmemtableloc->bVdeadmanwalking) cVmemtablepool.unset (iV);					
			}
		cVdbopmutex.relse ();
		dumps ();			
		return true;
	}

	/*CoMpaction Level CheCK*/
	bool __fastcall CoreTbsv::cmlck (PTR_SSTABLELOC cSstableloc, int iCompactionlevel, bool bExact) {
		bool bVvalidcheck;
		bool bVcompactioncheck;
		bool bVshortsize;

		bVvalidcheck = cSstableloc->bVvalid || (cSstableloc->bVvalidasmemorytable && bVmemorytable);
		bVcompactioncheck = (bExact && cSstableloc->iVcompactionlevel == iCompactionlevel) ||
											  (!bExact && cSstableloc->iVcompactionlevel >= iCompactionlevel);
		bVshortsize = cSstableloc->cVsstable->getsz () < CoreSstb::iVmaxcompactionsize;
		return bVvalidcheck && bVcompactioncheck && bVshortsize;
	}

	/*MAJor Compaction Internal*/
	bool __fastcall CoreTbsv::majci (bool& bUplevels,int iCompactionlevel, bool bQuick, bool bUplevel) {
		AnsiString sVtemp;
		bool bVresult;
		bool bVfound;
		bool bVmemorytable;
		bool bVcheck;
		int iV;
		int iVcompactions;
		int iVinner;
		int iVcompactionlevel;
		PTR_SSTABLELOC cVsstableloc;
		PTR_SSTABLELOC cVsstablelocinner;
		PTR_SSTABLELOC cVsstablelocresult;
		CoreList<TABLELOC> cVtablelist;

		iVcompactionlevel = bUplevel ? iCompactionlevel - 1 : iCompactionlevel;
		bVresult = false;
		bUplevels = false;
		iV = 0;		
		iVcompactions = 0;

		cVdbopmutex.locks ();
		while (iV < cVsstablepool.count ()) {
			cVsstableloc = cVsstablepool.get__ (iV)->cVptr;
			bVmemorytable = cVsstableloc->bVvalidasmemorytable;
			bVcheck = cmlck (cVsstableloc, iCompactionlevel);	
			sVtemp = cVsstableloc->cVsstable->getfn ();
			if (	bVcheck && !cVtablelist.bnget (sVtemp)) {

				cVtablelist.bnset (sVtemp);
				iVinner = 0;
				bVfound = false;
				while (iVinner < cVsstablepool.count ()) {
					cVsstablelocinner = cVsstablepool.get__ (iVinner)->cVptr;
					bVcheck = cmlck (cVsstablelocinner, iCompactionlevel);

					sVtemp = cVsstablelocinner->cVsstable->getfn ();
					bVfound = bVcheck && !cVtablelist.bnget (sVtemp);
					if (bVfound) {
						cVtablelist.bnset (sVtemp);
						cVsstablelocresult = cVsstablepool.set__ ()->cVptr;
						cVsstablelocresult->cVsstable->init (cVpool);
						cVdbopmutex.relse ();
						bVresult = majci (cVsstableloc, cVsstablelocinner, cVsstablelocresult, iVcompactionlevel, bVmemorytable);
						cVdbopmutex.locks ();

						if (bVresult) iVcompactions++;
						break;
					} else {
						if (cmlck (cVsstablelocinner, iCompactionlevel, false)) bUplevels = true;
					}
					iVinner++;
				} 
			}
			iV++;
			if (bQuick && bVresult) break;
		}		
		cVdbopmutex.relse ();
		if (iVcompactions > 0) return rcyci (true, true);
		return false;
	}

	/*MAJor ComPaction*/
	bool __fastcall CoreTbsv::majcp (bool& bUplevels,int iCompactionlevel, bool bQuick, bool bUplevel) {
		bool bVreturn;

		cVcompactionmutex.locks ();
		bVreturn = majci (bUplevels, iCompactionlevel, bQuick, bUplevel);
		cVcompactionmutex.relse ();
		return bVreturn;
	}

	/*SHRiNK*/
	bool __fastcall CoreTbsv::shrnk (bool bSoft) {
		PTR_SSTABLELOC cVsstablelocsource;
		PTR_SSTABLELOC cVsstablelocdestination;
		int iVcount;
		bool bVmemorytable;
		bool bVreturn;
		bool bVpartialresult;
		CoreSstb::PTR cVsstable;

		bVreturn = true;
		cVdbopmutex.locks ();
		iVcount = cVsstablepool.count ();
		for (int iV = 0; iV < iVcount; iV++) {
			cVsstablelocsource = cVsstablepool.get__ (iV)->cVptr;
			if (	cVsstablelocsource->bVdeadmanwalking) continue;
			bVmemorytable = cVsstablelocsource->bVvalidasmemorytable;
			cVsstablelocdestination = cVsstablepool.set__ ()->cVptr;
			cVsstable = cVsstablelocdestination->cVsstable;
			cVsstable->init (cVpool);
			cVsstable->setmd (sVpath, getns ());
			bVpartialresult = cVsstable->shrnk (	cVsstablelocsource->cVsstable, cVsstablelocsource->sVredotimestamp, 
																						bSoft, iVgracetimestamp);
			if (!bVpartialresult) {
				cVsstablelocdestination->bVdeadmanwalking = true;
			} else {
				if (bVmemorytable) bVpartialresult &= sstme (cVsstablelocdestination, sVredotimestamp);				
				if (bVpartialresult) {
					cVsstablelocdestination->bVvalid = true;
					cVsstablelocdestination->bVvalidasmemorytable = bVmemorytable;
					invss (cVsstablelocsource, sVredotimestamp, true);
				}
			}
			LOG(INFO) << "Shrnk comp:" << cVsstablelocdestination->cVsstable->getfn ().c_str() << 
									", result: " << (bVpartialresult ? "Ok" : "Ko");
			FlushLogFiles(GLOG_INFO);
			
			bVreturn &= bVpartialresult;
		}
		cVdbopmutex.relse ();
		rcyci (true, false);
		return bVreturn;
	}

	/*NEW Memtable Internal*/
	CoreTbsv::PTR_MEMTABLELOC __fastcall CoreTbsv::newmi (bool bActivememloc, AnsiString sRedotimestamp) {
		PTR_MEMTABLELOC cVreturn;

		cVreturn = cVmemtablepool.set__ ()->cVptr;		
		cVreturn->bVneedcompaction = false;
		cVreturn->sVredotimestamp = sRedotimestamp;
		cVreturn->cVmemtable->init (	cVpool, (AnsiString) (cVmemtablepool.count ()  -1), 
																iVmemtabledim, iVmemtablesize, iVmemtablechunk);
		cVreturn->bVvalid = true;
		if (bActivememloc) cVactivememloc = cVreturn;
		return cVreturn;
	}

	/*NEW MemTable*/
	CoreTbsv::PTR_MEMTABLELOC __fastcall CoreTbsv::newmt (bool bActivememloc, AnsiString sRedotimestamp) {
		PTR_MEMTABLELOC cVreturn;

		cVdbopmutex.locks ();
		cVreturn = newmi (bActivememloc, sRedotimestamp);
		cVdbopmutex.relse ();
		return cVreturn;
	}

	/*SStable To Memtable*/
	bool __fastcall CoreTbsv::sstme (PTR_SSTABLELOC cSstableloc, AnsiString sRedotimestamp, bool bActivememtable) {
		PTR_MEMTABLELOC cVmemtableloc;

		cVmemtableloc = newmi (bActivememtable, sRedotimestamp);
		cVmemtableloc->bVvalid = cSstableloc->cVsstable->tomem (cVmemtableloc->cVmemtable);
		if (cVmemtableloc->bVvalid) {
			cSstableloc->bVvalidasmemorytable = true;
			cSstableloc->bVvalid = false;
			cSstableloc->cVlinkedmemtableloc = cVmemtableloc;

			LOG(INFO) <<	"To memtab: " << cSstableloc->cVsstable->getfn ().c_str () << 
										", sstable count: " << cSstableloc->cVsstable->getsc () <<
										", memtable count: " << (long) (cSstableloc->cVlinkedmemtableloc != NULL ? 
											cSstableloc->cVlinkedmemtableloc->cVmemtable->getsc () : 0L);
			LOG(INFO) << "Minor comp:" << sVtable.c_str () << ", is memorytable: " << (bVmemorytable ? "true" : "false");
			FlushLogFiles(GLOG_INFO);

			return true;
		} else {

			LOG(INFO) <<	"To memtab: FATAL ERROR " << cSstableloc->cVsstable->getfn ().c_str () << 
										", sstable count: " << cSstableloc->cVsstable->getsc ();
			FlushLogFiles(GLOG_INFO);

			return false;
		}
	}

	/*LOAD All*/
	bool __fastcall CoreTbsv::loada (bool bResetlocal) {
		CoreSstb::PTR cVsstable;
		PTR_SSTABLELOC cVsstableloc;
		ORDEREDSSTABLELOC* cVorderedsstableloc;
		bool bVreturn;
		bool bVnotsstable;
		bool bVsystemtracked;
		int iVsstableorderingcounter;
		CoreHsrx::iCsstabletypes iVsstabletype;
		AnsiString sVtempfilename;
		AnsiString sVtempfilenamenoext;
		CoreList<AnsiString> cVfilenames;
		CoreList<TABLELOC> cVsystemsstable;
		CoreList<ORDEREDSSTABLELOC> cVorderedsstablepool;

		LOG(INFO) <<		"Tablet: " << sVtable.c_str () << 
										", load all starts";
		FlushLogFiles(GLOG_INFO);
		bVreturn = false;
		iVsstablecounter = 0;
		bVsystemtracked = rests (&cVsystemsstable);
		CoreHutl::readr (sVpath, sVfilename + (AnsiString) "*." + CoreSstb::sCdataext, cVfilenames);
		for (int iV = 0; iV < cVfilenames.count (); iV++) {		
			sVtempfilename = *cVfilenames.get__ (iV);
			if (CoreButl::left_ (sVtempfilename, CoreSstb::sCversiondelimiter) != sVfilename) continue;
			sVtempfilenamenoext = CoreButl::left_ (sVtempfilename, ".");
			if	(!bResetlocal && (bVsystemtracked && cVsystemsstable.bnget (sVtempfilenamenoext) == NULL)) continue;
			iVsstableorderingcounter = CoreButl::left_ (	CoreButl::right (*cVfilenames.get__ (iV), 
																										CoreSstb::sCversiondelimiter), ".").ToInt ();
			iVsstablecounter = Max (iVsstablecounter, iVsstableorderingcounter);
			cVorderedsstableloc = cVorderedsstablepool.bnset (CoreButl::lpad_ ((AnsiString) iVsstableorderingcounter, '0', 10));
			cVorderedsstableloc->sVtempfilenamenoext = sVtempfilenamenoext;
		}
		cVsstablepool.clear ();	
		iVsstablecounter++;
		sVredotimestamp = sCcleanredotimestamp;
		for (int iV = 0; iV < cVorderedsstablepool.count (); iV++) {
			cVorderedsstableloc = cVorderedsstablepool.get__ (iV);
			bVreturn = true;
			cVsstableloc = cVsstablepool.set__ ()->cVptr;
			cVsstable = cVsstableloc->cVsstable;
			cVsstable->init (cVpool);
			cVsstable->setmd (sVpath, cVorderedsstableloc->sVtempfilenamenoext);
			bVreturn &= cVsstable->loadd (iVsstabletype, cVsstableloc->sVredotimestamp);	
			cVsstableloc->iVsstabletype = iVsstabletype;
			bVnotsstable = iVsstabletype != CoreHsrx::iCsstable;						
			sVredotimestamp = Max (sVredotimestamp, 
				CoreButl::lpadt (cVsstableloc->sVredotimestamp, sCchar0, iCredotimestampsize));
			if (bVnotsstable) bVreturn &= sstme (cVsstableloc, sVredotimestamp, iVsstabletype == CoreHsrx::iCpartialmemtable);
			else cVsstableloc->bVvalid = true;	
		}
		if (bResetlocal) dumps ();

		LOG(INFO) <<		"Tablet: " << sVtable.c_str () << 
										", load all ends";
		FlushLogFiles(GLOG_INFO);

		return bVreturn;
	}

	/*GET Committed Timestamp*/
	AnsiString __fastcall CoreTbsv::getcr () {
		AnsiString sVreturn;

		cVdbopmutex.locks ();
		sVreturn = sVredotimestamp;
		cVdbopmutex.relse ();
		return sVreturn;
	}

	/*KEY Internal*/
	AnsiString __fastcall CoreTbsv::key_i (	PTR_REVERSE cReverse,
																					AnsiString sMain, int& iMainindexlength, 
																					AnsiString sAccessgroup, 
																					AnsiString sQualifier, 
																					bool bAttributes,
																					iCstatetype iState, 
																					int64vcl iTimestamp) {
		AnsiString sVreturn;
		int64vcl iVtimestamp;

		sVreturn = sMain;
		if (sAccessgroup != NULL) {
			sVreturn += (AnsiString) bCmaindelimiter + sAccessgroup;
			if (sQualifier != NULL) sVreturn += (AnsiString) bCqualifierdelimiter + sQualifier;
		}
		iMainindexlength = sVreturn.Length ();
		if (bAttributes) {
			iVtimestamp = cReverse->descl (iTimestamp);
			sVreturn +=  (AnsiString) bCmajordelimiter + CoreButl::i642s (iVtimestamp);
			switch (iState) {
				case UPSERT: sVreturn += (char) bCupsert; break;
				case DELTMB: sVreturn += (char) bCdelete; break;
			}
		}
		return sVreturn;
	}

	/*Fast KEY*/
	AnsiString __fastcall CoreTbsv::fkey_ (PrtoLkey* cKey, int& iMainindexlength) {
		AnsiString sVkey = cKey->svmain ();
		iMainindexlength = sVkey.Length () + 2;
		sVkey += cKey->ivstate () == UPSERT ? sCfastkeyupsertdelimiters: sCfastkeydeletedelimiters;
		return sVkey;
	};

	/*KEY*/
	AnsiString __fastcall CoreTbsv::key__ (	PTR_REVERSE cReverse, PrtoLkey* cKey, int& iMainindexlength, bool bAttributes) {

		/*
		printf ("1\n");
		printf ("valeva %s\n", cKey->svmain ().c_str ());

		printf ("%i %i", cKey->svaccessgroup ().length (), cKey->svqualifier ().length ());
		printf ("valeva %s - %s - %s \n", cKey->svmain ().c_str (), cKey->svaccessgroup ().c_str (), cKey->svqualifier ().c_str ());
		*/

		AnsiString sVreturn = key_i (	cReverse, cKey->svmain (), iMainindexlength, cKey->svaccessgroup (), cKey->svqualifier (), 
										bAttributes, cKey->ivstate (), cKey->ivtimestamp ());	
		
		
		//printf ("2 %s\n", sVreturn.c_str ());
		return sVreturn;

	}

	/*KEY*/
	void __fastcall CoreTbsv::key__ (	PTR_REVERSE cReverse, AnsiString sKey, PrtoLkey* cKey) {
		int iVlength;
		int iVpos;
		int iVtemp;
		char* bVbuffer;
		
		cKey->Clear ();
		iVlength = sKey.Length ();
		bVbuffer = sKey.c_str ();
		cKey->set_ivstate (bVbuffer [iVlength - 1] == (char) bCupsert ? UPSERT : DELTMB);
		iVpos= iVlength - 1 - sizeof (int64vcl);
		cKey->set_ivtimestamp (cReverse->rdesl (*((int64vcl*) &bVbuffer [iVpos])));
		iVpos -= 2;
		iVtemp = CoreButl::rvpos (bVbuffer, bCqualifierdelimiter, iVpos);
		if (iVtemp >= 0 && iVtemp != iVpos) {
			cKey->set_svqualifier (AnsiString (&bVbuffer [iVtemp + 1], iVpos- iVtemp + 1).to_string ());
			iVpos = iVtemp;
		}
		iVpos--;
		iVtemp = CoreButl::rvpos (bVbuffer, bCmaindelimiter, iVpos);
		if (iVtemp >= 0 && iVtemp != iVpos) {
			cKey->set_svaccessgroup (AnsiString (&bVbuffer [iVtemp + 1], iVpos- iVtemp + 1).to_string ());
			iVpos = iVtemp;
		}	
		cKey->set_svmain (AnsiString (bVbuffer, iVpos + 1).to_string ());
	}

	/*GET MaJor from key shipped with attributes*/
	AnsiString __fastcall CoreTbsv::getmj ( AnsiString sKey) {

		return sKey.SubString (0, sKey.Length () - 2 - sizeof (int64vcl));
	}

	/*GET MaiN from major string*/
	AnsiString __fastcall CoreTbsv::getmn ( AnsiString sKey) {
		int iVmaindelimiter;

		iVmaindelimiter = CoreButl::revap (sKey, bCmaindelimiter);
		if (iVmaindelimiter != 0) return sKey.SubString (0, iVmaindelimiter - 1);
		else return sKey;
	}

	/*DUMP System*/
	bool __fastcall CoreTbsv::dumps () {
		PTR_SSTABLELOC cVsstableloc;
		PrtoSsls cVsystemsstablelist;
		PrtoLkey cVkey;
		PrtoLval cVvalue;

		if (!bVtabletsystem) {
			for (int iV = 0; iV < cVsstablepool.count (); iV++) {
				cVsstableloc = cVsstablepool.get__ (iV)->cVptr;
				if (cVsstableloc->bVvalid || cVsstableloc->bVvalidasmemorytable) 
					cVsystemsstablelist.add_svlistindex (cVsstableloc->cVsstable->getfn ().to_string ());				
			}
			CoreAutl::zrkey (&cVkey);
			cVkey.set_svmain (sVsystemkey.to_string ());
			cVkey.set_ivstate (UPSERT);
			cVkey.set_ivtimestamp (CoreButl::tmstl ());
			cVvalue.set_svopaquevalue (cVsystemsstablelist.SerializeAsString ());
			cVvalue.set_ivtimestamp (cVkey.ivtimestamp ());
			cVtabletsystem->muttr (&cVkey, &cVvalue, NULL);
			cVtabletsystem->mincp (true);

			for (int iV = 0; iV < cVsystemsstablelist.svlistindex_size (); iV++) {
				LOG(INFO) <<		"L save: " << sVsystemkey.c_str () << 
												", add: " << cVsystemsstablelist.mutable_svlistindex (iV)->c_str () <<
												", timestamp: " << cVvalue.ivtimestamp ();
				FlushLogFiles(GLOG_INFO);
			}

			return true;
		}
		return false;
	}

	/*RESTore System*/
	bool __fastcall CoreTbsv::rests (CoreList<CoreTbsv::TABLELOC>* cSystemsstable) {
		PrtoLkey cVkey;
		PrtoLval cVvalue;
		PrtoSsls cVsystemsstablelist;

		if (!bVtabletsystem) {
			cVkey.set_svmain (sVsystemkey.to_string ());
			if (cVtabletsystem->query (&cVkey, &cVvalue, CoreTble::iCexactquery)) {
				cVsystemsstablelist.ParseFromString (cVvalue.svopaquevalue ());
				for (int iV = 0; iV < cVsystemsstablelist.svlistindex_size (); iV++) {
					cSystemsstable->bnset (cVsystemsstablelist.svlistindex (iV));

					LOG(INFO) <<		"L load: " << cVkey.svmain ().c_str () << 
													", add: " << cVsystemsstablelist.mutable_svlistindex (iV)->c_str () <<
													", timestamp: " << cVvalue.ivtimestamp ();
					FlushLogFiles(GLOG_INFO);

				}
			}
			return true;
		}
		return false;
	}

	/*GET Tablet Info*/
	bool __fastcall CoreTbsv::getti (RepeatedPtrField<PrtoTlel>* cTabletinfo, bool bDiagnostic) {
		unsigned int iVsize;
		unsigned int iVmapped;
		PrtoTlel* cVtabletelement;
		PTR_MEMTABLELOC cVmemtableloc;
		PTR_SSTABLELOC cVsstableloc;

		cTabletinfo->Clear ();
		cVcompactionmutex.locks ();
		iVsize = 0;

		for (int iV = 0; iV < cVmemtablepool.count (); iV++) {
			cVmemtableloc = cVmemtablepool.get__ (iV)->cVptr;
			if (!cVmemtableloc->bVvalid) continue;
			if (bDiagnostic) cVmemtableloc->cVmemtable->clear ();
			cVtabletelement = cTabletinfo->Add ();
			cVtabletelement->set_ivtablettype (MEMORYTABLETYPE);
			cVtabletelement->set_svname ((sVtable + "-" + cVmemtableloc->cVmemtable->getid ()).to_string ());
			cVtabletelement->set_bvvalid (cVmemtableloc->bVvalid);
			cVtabletelement->set_bvvalidasmemorytable (bVvolatiletable);
			cVtabletelement->set_bvneedcompaction (cVmemtableloc->bVneedcompaction);
			cVtabletelement->set_ivcompactionlevel (0);
			cVtabletelement->set_svmemorytable ("");
			iVsize = cVmemtableloc->cVmemtable->getsd ();
			cVtabletelement->set_ivsize (iVsize);
			cVtabletelement->set_ivres (cVmemtableloc->cVmemtable->getrs ());
			iVmapped = cVmemtableloc->cVmemtable->getma ();
			cVtabletelement->set_ivmapped (iVmapped);
			cVtabletelement->set_ivsize (iVmapped);
			cVtabletelement->set_ivcount (cVmemtableloc->cVmemtable->getsc ());
			cVtabletelement->set_svredotimestamp (cVmemtableloc->sVredotimestamp.debug (false).to_string ());
			cVtabletelement->set_ivsstabletype (-1);
		}
		for (int iV = 0; iV < cVsstablepool.count (); iV++) {
			cVsstableloc = cVsstablepool.get__ (iV)->cVptr;
			if (!cVsstableloc->bVvalid && !cVsstableloc->bVvalidasmemorytable) continue;
			cVtabletelement = cTabletinfo->Add ();				
			cVtabletelement->set_ivtablettype(SSTABLETYPE);
			cVtabletelement->set_svname ((cVsstableloc->cVsstable->getfn ()).to_string ());
			cVtabletelement->set_bvvalid (cVsstableloc->bVvalid);
			cVtabletelement->set_bvvalidasmemorytable (cVsstableloc->bVvalidasmemorytable);
			cVtabletelement->set_bvneedcompaction (false);
			cVtabletelement->set_ivsstabletype (cVsstableloc->iVsstabletype);
			cVtabletelement->set_ivcompactionlevel (cVsstableloc->iVcompactionlevel);
			cVtabletelement->set_svmemorytable (((AnsiString)
				(cVsstableloc->cVlinkedmemtableloc != NULL ? 
				sVtable + "-" + cVmemtableloc->cVmemtable->getid () : "-")).to_string ());
			cVtabletelement->set_ivcount (cVsstableloc->cVsstable->getsc ());
			cVtabletelement->set_ivres (cVsstableloc->cVsstable->getrs ());
			cVtabletelement->set_ivmapped (cVsstableloc->cVsstable->getma ());
			cVtabletelement->set_ivsize (cVsstableloc->cVsstable->getsz ());
			cVtabletelement->set_svredotimestamp (cVsstableloc->sVredotimestamp.debug (false).to_string ());
		}
		
		cVcompactionmutex.relse ();
		return true;
	}

	/*SET ABort*/
	void __fastcall CoreTbsv::setab (bool bAbort) {

		bVabort = bAbort;
	}

	/*GET working SiZe*/
	int64vcl __fastcall CoreTbsv::getsz () {
		PTR_MEMTABLELOC cVmemtableloc;
		PTR_SSTABLELOC cVsstableloc;
		int64vcl iVsize;

		cVcompactionmutex.locks ();
		iVsize = 0;
		for (int iV = 0; iV < cVmemtablepool.count (); iV++) {
			cVmemtableloc = cVmemtablepool.get__ (iV)->cVptr;
			if (!cVmemtableloc->bVvalid) continue;
			iVsize += cVmemtableloc->cVmemtable->getma ();
		}
		for (int iV = 0; iV < cVsstablepool.count (); iV++) {
			cVsstableloc = cVsstablepool.get__ (iV)->cVptr;
			if (!cVsstableloc->bVvalid) continue;
			iVsize += cVsstableloc->cVsstable->getsz ();
		}
		cVcompactionmutex.relse ();
		return iVsize;
	}

	/*CLEAR*/
	void __fastcall CoreTbsv::clear () {
		PTR_MEMTABLELOC cVmemtableloc;
		PTR_SSTABLELOC cVsstableloc;

		cVcompactionmutex.locks ();
		for (int iV = 0; iV < cVmemtablepool.count (); iV++) {
			cVmemtableloc = cVmemtablepool.get__ (iV)->cVptr;
			cVmemtableloc->bVvalid = false;
		}
		for (int iV = 0; iV < cVsstablepool.count (); iV++) {
			cVsstableloc = cVsstablepool.get__ (iV)->cVptr;
			cVsstableloc->bVvalidasmemorytable = cVsstableloc->bVvalid = false;
		}
		newmt (true, CoreTbsv::sCcleanredotimestamp);
		rcyci (true, true);
		cVcompactionmutex.relse ();	
	}

	/*CLEAN*/
	bool __fastcall CoreTbsv::clean () {

		cVdbopmutex.locks ();
		for (int iV = cVsstablepool.count () - 1; iV >= 0; iV--) cVsstablepool.get__ (iV)->cVptr->bVdeadmanwalking = true;
		for (int iV = cVmemtablepool.count () - 1; iV >= 0; iV--) cVmemtablepool.get__ (iV)->cVptr->bVdeadmanwalking = true; 
		newmi (true, sVredotimestamp);
		cVdbopmutex.relse ();
		rcyci ();
		dumps ();
		return true;
	}

	/*Write Archive LOG*/
	bool __fastcall CoreTbsv::walog () {

		return !bVvolatiletable;
	}

	/*Write Archive LOG*/
	bool __fastcall CoreTbsv::rpair () {

		cVmemtablepool.clear ();
		cVsstablepool.clear ();
		loada (true);
		if (cVactivememloc == NULL) newmt (true, sVredotimestamp);
		return true;
	}

	/*Force COMPaction*/
	bool __fastcall CoreTbsv::fcomp () {
		bool bVdummy;

		while (majcp (bVdummy, 0));
		return true;
	}

	AnsiString __fastcall CoreTbsv::debug () {
		AnsiString sVreturn;
		PTR_MEMTABLELOC cVmemtableloc;

		sVreturn = "";
		cVdbopmutex.locks ();
		for (int iV = cVmemtablepool.count () - 1; iV >= 0; iV--) {
			cVmemtableloc = cVmemtablepool.get__ (iV)->cVptr;
			sVreturn += cVmemtableloc->cVmemtable->debug ();
		}
		cVdbopmutex.relse ();
		return sVreturn;
	}

