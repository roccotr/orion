#include "CoreAlog.hpp"
#include "CoreHutl.hpp"
#include "TmplList.cpp"
#include "TmplCche.cpp"
#include "TmplLstc.cpp"
#include "TmplFsys.cpp" 

#include <glog/logging.h>
using namespace google;
using namespace orion;

//#define iCdebugtimestamp
//#define iCdebuglog
//#define iCdebugtable "PIMTINGCHANNELGEOSTEM"
//#define iCdebugindex "_peppemas_it_peppemastestchannel"
//#define iCdebugstartindex "4"
//#define iCdebuglocalstatement


	const AnsiString CoreAlog::sCredologprefix = "REDO";
	const AnsiString CoreAlog::sCredologdelimiter = "_";
	const AnsiString CoreAlog::sCredologext = "LOG";
	const AnsiString CoreAlog::sCsystemnamespace = "SYSTEM";
	const AnsiString CoreAlog::sClocalsystemtable = "LOCAL";
	const AnsiString CoreAlog::sCglobalsystemtable = "GLOBAL";
	const AnsiString CoreAlog::sCsystemlogictable = "RESERVED";
	const AnsiString CoreAlog::sCitableprefix = "%";
	const AnsiString CoreAlog::sCtabletdelimiter = "_";
	const AnsiString CoreAlog::sCtablekeydelimiter = "/";
	const int CoreAlog::iCmaxactivelogdim = 10;

  __fastcall CoreAlog::CoreAlog(	PTR_POOL cPool, PTR_SEDAMANAGER cSedamanager,
																	AnsiString sRedologspath, AnsiString sDatapath, AnsiString sTabletspath, 
																	int iRedologdim, bool bResetlocal, int iGracetime) {
		cVpool = cPool;
		cVsedamanager = cSedamanager;
		sVredologspath = sRedologspath;
		sVdatapath = sDatapath;
		sVtabletspath = sTabletspath;
		iVredologdim = Max (iRedologdim, iCmaxactivelogdim);
		iVgracetime = iGracetime;
		cVreverse = boost::shared_ptr<CoreRvrs> (new CoreRvrs ());
		bVabort = false;
		bVresetlocal = bResetlocal;
		open_ ();
	}

  __fastcall CoreAlog::~CoreAlog () {

		close ();
	}

	/*OPEN*/
	void __fastcall CoreAlog::open_ () {
		PrtoLmtb cVsystemmutable;

		cVredologwriter = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));
		cVactiveredologfile = NULL;		
		cVsystemmutable.set_svnamespace (sCsystemnamespace.to_string ());
		cVsystemmutable.set_svtable (sClocalsystemtable.to_string ());		
		cVtabletsystemloc = gettb (&cVsystemmutable, true);
		iVredologcounter = 0;

#ifndef iCdisableprintlog
		loada (true);
#else
		loada (false);
#endif

		if (bVresetlocal) cVtabletsystemloc->cVtablet->clear ();

		LOG(INFO) << "Redo ready";
		FlushLogFiles(GLOG_INFO);
	}
	
	/*CLOSE*/
	void __fastcall CoreAlog::close () {
		
		LOG(INFO) << "Redo stopping";
		FlushLogFiles(GLOG_INFO);
		if (!bVabort) rcycl ();
		for (int iV = cVtabletpool.count () - 1; iV >= 0; iV--) {
			if (!cVtabletpool.get__ (iV)->bVsystem) cVtabletpool.unset (iV);		
		}
	}

	/*KILL*/
	void __fastcall CoreAlog::kill_ () {
		REDOLOGFILELOC* cVredologfileloc;

		newri (true);
		cVredologfilepoolmutex.locks ();
		for (int iV = 0; iV < cVredologfilepool.count (); iV++) {
			cVredologfileloc = cVredologfilepool.get__ (iV);
			if (cVredologfileloc != cVactiveredologfile) remve (cVredologfileloc);
		}
		cVredologfilepoolmutex.relse ();
	}

	/*ReCYCLe*/
	void __fastcall CoreAlog::rcycl () {
		bool bVremovelogfile;
		REDOLOGFILELOC* cVredologfileloc;
		REDOLOGTABLET* cVredologtablet;

		cVredologfilepoolmutex.locks ();
		for (int iV = 0; iV < cVredologfilepool.count (); iV++) {
			cVredologfileloc = cVredologfilepool.get__ (iV);
			if (!cVredologfileloc->bVvalid && cVredologfileloc == cVactiveredologfile) continue;
			bVremovelogfile = true;
			for (int iVinner = cVredologfileloc->cVredologtabletpool.count () - 1; iVinner >= 0; iVinner--) {
				cVredologtablet = cVredologfileloc->cVredologtabletpool.get__	(iVinner);
				bVremovelogfile &= cVredologtablet->sVtimestamp <= cVredologtablet->cVtabletserver->getcr ();
			}
			if (bVremovelogfile) remve (cVredologfileloc);
		}
		if (cVactiveredologfile == NULL) newri (true);				
		cVredologfilepoolmutex.relse ();
	}

  /*STATeMent*/
	bool __fastcall CoreAlog::statm (PrtoLstm* cStatement, CoreAlog::TABLETLOC** cCachedtabletloc, AnsiString sRedotimestamp) {
		AnsiString sVredotimestamp;
		TABLETLOC* cVtabletloc;
		
		switch (cStatement->ivopcode ()) {
			case MUTATOR:

#ifdef iCdebuglocalstatement
				printf ("%s_%s (%s-%i)\n", 
					cStatement->mutable_cvmutable ()->svnamespace ().c_str (),
					cStatement->mutable_cvmutable ()->svtable ().c_str (),
					((AnsiString) cStatement->mutable_cvkey ()->svmain ()).prntb ().c_str (),
					cStatement->mutable_cvkey ()->ivstate () == UPSERT);
#endif

#ifdef iCdebugtimestamp
				printf ("%i %lld %lld - %s %s \n", 					
					cStatement->mutable_cvkey ()->ivstate () == UPSERT,
					cStatement->mutable_cvkey ()->ivtimestamp (),
					cStatement->mutable_cvvalue ()->ivtimestamp (),
					cStatement->mutable_cvmutable ()->svtable ().c_str (),
					((AnsiString) cStatement->mutable_cvkey ()->svmain ()).prntb ().c_str ());
					 
#endif
				if (cCachedtabletloc != NULL && *cCachedtabletloc != NULL) cVtabletloc = *cCachedtabletloc;
				else {
					cVtabletloc = gettb (cStatement->mutable_cvmutable (), false);
					if (cCachedtabletloc != NULL) *cCachedtabletloc = cVtabletloc;
				}
				if (sRedotimestamp == NULL && cVtabletloc->cVtablet->walog ()) wrtrl (cStatement, cVtabletloc, sVredotimestamp);
				else sVredotimestamp = sRedotimestamp;
				cVtabletloc->iVwritecount++;
				return cVtabletloc->cVtablet->muttr (cStatement->mutable_cvkey (), cStatement->mutable_cvvalue (), sVredotimestamp);
			case STORAGE:
				return setsg (	cStatement->mutable_cvmutable ());		
			default:
				return false;
		}
	}

	/*QUERY*/
	bool __fastcall CoreAlog::query (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, 
																		CoreTble::iCquerytype iQuerytype, CoreAlog::TABLETLOC* cCachedtabletloc) {
		TABLETLOC* cVtabletloc;

		cVtabletloc = cCachedtabletloc != NULL ? cCachedtabletloc : gettb (cMutable, false);
		cVtabletloc->iVreadcount++;
		return cVtabletloc->cVtablet->query (cKey, cValue, iQuerytype);
	}

	/*GET TaBlet  server*/
	CoreAlog::TABLETLOC* __fastcall CoreAlog::gettb (PrtoLmtb* cMutable, bool bTabletsystem, bool bCreate, bool bRepair) {
		AnsiString sVstixkey;
		AnsiString sVnamespace;
		AnsiString sVtable;
		TABLETLOC* cVtabletloc;
		PTR_TABLETSERVER cVtabletnull;

		sVstixkey = gettk (cMutable, sVnamespace, sVtable);
		if (sVnamespace == sCsystemnamespace && sVtable == sCsystemlogictable) return cVtabletsystemloc;
		cVtabletpoolmutex.locks ();
		cVtabletloc = cVtabletpool.bnget (sVstixkey);
		if (cVtabletloc == NULL && bCreate) {
			cVtabletloc = cVtabletpool.bnset (sVstixkey);
			cVtabletloc->bVsystem = bTabletsystem;
			cVtabletloc->cVtablet->init_ (cVpool, cVsedamanager, sVnamespace, sVtable, sVdatapath, sVtabletspath, 
				sVstixkey, iVgracetime,
				bTabletsystem ? cVtabletnull : cVtabletsystemloc->cVtablet,
				bTabletsystem ? false : bRepair);		
			cVtabletpoolmutex.relse ();
			return cVtabletloc;
		}
		cVtabletpoolmutex.relse ();
		return cVtabletloc;
	}

	/*SET StoraGe*/
	bool __fastcall CoreAlog::setsg (PrtoLmtb* cMutable) {
		TABLETLOC* cVtabletloc;

		cVtabletloc = gettb (cMutable, false, true, cMutable->mutable_cvstorage ()->bvrepair ());
		cVtabletpoolmutex.locks ();
		cVtabletloc->cVtablet->setsg (cMutable->mutable_cvstorage ());
		cVtabletpoolmutex.relse ();
		return true;
	}

	/*GET log IndeX*/
	AnsiString __fastcall CoreAlog::getix (PTR_REVERSE cReverse, int& iCounter) {
		AnsiString sVreturn;

		sVreturn = 
			CoreButl::lpad_ (	CoreButl::i642s (cReverse->asc_l (CoreButl::tmstl ())), 
												CoreTbsv::sCchar0, sizeof (int64vcl));
		iCounter++;
		if (iCounter < 0) {
			iCounter = 0;
			Sleep (10);
		}
		sVreturn += CoreButl::int2s (cReverse->asc_i (iCounter));
		return sVreturn;
	}

	/*GET log Reverse indeX*/
	void __fastcall CoreAlog::getrx (PTR_REVERSE cReverse, AnsiString sIndex, int64vcl& iTimestamp, int& iCounter) {
			AnsiString sVindex;
			int iVbasepos;

			sVindex = CoreButl::lpad_ (sIndex, CoreTbsv::sCchar0, CoreTbsv::iCredotimestampsize);
			iVbasepos = sVindex.Length () - CoreTbsv::iCredotimestampsize;
			iTimestamp = cReverse->asc_l (*(int64vcl*) &(sVindex.c_str () [iVbasepos ]));
			iCounter = cReverse->asc_i (*(int*) &(sVindex.c_str () [iVbasepos  + sizeof (int64vcl)]));
	}

	/*LOG Reverse indeX*/
	AnsiString __fastcall CoreAlog::logrx (PTR_REVERSE cReverse, AnsiString sIndex) {
		int iVcounter;
		int64vcl iVtimestamp;
		time_t cVnowtime;
		struct tm *cVnowtm;
		char bVtmbuf[64];
		double dVseconds;

		CoreAlog::getrx (cReverse, sIndex, iVtimestamp, iVcounter);
		dVseconds = ((double) iVtimestamp) / CoreButl::dCtmstlfactor;
		cVnowtime = floor (dVseconds);
		cVnowtm = localtime(&cVnowtime);
		strftime(bVtmbuf, sizeof bVtmbuf, "%Y-%m-%d %H:%M:%S", cVnowtm);
		return	(AnsiString) bVtmbuf + (AnsiString) "." + 
						(AnsiString) int ((dVseconds - (double) cVnowtime) * CoreButl::dCtmstpfactor) + (AnsiString) "-" +
						(AnsiString) iVcounter;
	}

	/*LOG Reverse index Sybase format*/
	AnsiString __fastcall CoreAlog::logrs (PTR_REVERSE cReverse, AnsiString sIndex) {
		int iVcounter;
		int64vcl iVtimestamp;
		time_t cVnowtime;
		struct tm *cVnowtm;
		char bVtmbuf[64];
		double dVseconds;

		CoreAlog::getrx (cReverse, sIndex, iVtimestamp, iVcounter);
		dVseconds = ((double) iVtimestamp) / CoreButl::dCtmstlfactor;
		cVnowtime = floor (dVseconds);
		cVnowtm = localtime(&cVnowtime);
		strftime(bVtmbuf, sizeof bVtmbuf, "%Y%m%d%H%M%S", cVnowtm);
		return	(AnsiString) bVtmbuf + 
						(AnsiString) int ((dVseconds - (double) cVnowtime) * 10000000.0);
	}

	/*GET log IndeX*/
	AnsiString __fastcall CoreAlog::getix () {
		AnsiString sVreturn;

		cVredoixmutex.locks ();
		sVreturn = getix (cVreverse, iVindexcount);
		cVredoixmutex.relse ();
		return sVreturn;
	}

	/*NEW Redolog Internal*/
	CoreAlog::REDOLOGFILELOC* __fastcall CoreAlog::newri (bool bActive) {
		REDOLOGFILELOC* cVredologfile;

		cVredologfile = cVredologfilepool.set__ ();
		cVredologfile->sVfilename = sVredologspath + newrn ();
		if (bActive) {
			cVactiveredologfile = cVredologfile;
			cVredologwriter->begin (-1, cVactiveredologfile->sVfilename, false);
			iVactivelogdim = 0;

			LOG(INFO) << "Redo new:  " << cVactiveredologfile->sVfilename.c_str ();
			FlushLogFiles(GLOG_INFO);
		}
		return cVredologfile;
	}

	/*NEW RedoLog*/
	CoreAlog::REDOLOGFILELOC* __fastcall CoreAlog::newrl (bool bActive) {
		REDOLOGFILELOC* cVreturn;

		cVredologwritermutex.locks ();
		cVredologfilepoolmutex.locks ();
		cVreturn = newri (bActive);
		cVredologfilepoolmutex.relse ();	
		cVredologwritermutex.relse ();
		return cVreturn;
	}

	/*NEW Redolog Name*/
	AnsiString __fastcall CoreAlog::newrn () {

		return sCredologprefix + sCredologdelimiter + CoreButl::ci64_ (iVredologcounter++, false) + "." + sCredologext;
	}

	/*WRiTe RedoLog*/
	bool __fastcall CoreAlog::wrtrl (PrtoLstm* cStatement, TABLETLOC* cTabletloc, AnsiString& sRedotimestamp) {
		bool bVreturn;
		PrtoLlog cVredologitem;
		REDOLOGTABLET* cVredologtablet;

		cVredologtablet = getrt (cVactiveredologfile, cTabletloc);
		sRedotimestamp = cVredologtablet->sVtimestamp = getix ();
		cVredologitem.set_svtimestamp (cVredologtablet->sVtimestamp.to_string ());
		cVredologitem.mutable_cvstatement ()->CopyFrom (*cStatement);
		cVredologwritermutex.locks ();
		bVreturn = cVredologwriter->sctds<PrtoLlog> (&cVredologitem, true, true);	
		iVactivelogdim++;
		cVredologwritermutex.relse ();
		return bVreturn;
	}

	/*GET Redolog Tablet*/
	CoreAlog::REDOLOGTABLET* CoreAlog::getrt (REDOLOGFILELOC* cRedologfileloc, TABLETLOC* cTabletloc) {
		REDOLOGTABLET* cVreturn;

		cVredologfilepoolmutex.locks ();
		cVreturn = cRedologfileloc->cVredologtabletpool.bnget (cTabletloc->sVindex);
		if (cVreturn == NULL) {
			cVreturn = cRedologfileloc->cVredologtabletpool.bnset (cTabletloc->sVindex);
			cVreturn->sVtimestamp = CoreTbsv::sCcleanredotimestamp;
			cVreturn->cVtabletserver = cTabletloc->cVtablet;
		}
		cVredologfilepoolmutex.relse ();
		return cVreturn;
	}

	/*REMoVE*/
	void __fastcall CoreAlog::remve (REDOLOGFILELOC* cRedologfileloc) {

		if (cRedologfileloc == cVactiveredologfile) {
			cVredologwriter->end__ ();
			cVactiveredologfile = NULL;
		}
		cRedologfileloc->bVvalid = false;
		CoreXutl::delfl (cRedologfileloc->sVfilename.c_str ());

		LOG(INFO) << "Redo del:  " << cRedologfileloc->sVfilename.c_str ();
		FlushLogFiles(GLOG_INFO);
	}

	/*ReCoVeRY*/
	bool __fastcall CoreAlog::rcvry (bool bPrintf) {

		if (bPrintf) printf ("\n");
		for (int iV = 0; iV < cVredologfilepool.count (); iV++)  
			rcvry (cVredologfilepool.get__ (iV), bPrintf);	
		return false;
	}

	/*ReCoVeRY*/
	bool __fastcall CoreAlog::rcvry (REDOLOGFILELOC* cRedologfileloc, bool bPrintf) {
		int iVrecovered;
		int iVtemp;
		bool bVparsed;
		AnsiString sVrowtimestamp;
		AnsiString sVpath;
		AnsiString sVfilename;
		CoreFsys cVfsys (cVpool);
		PrtoLlog cVredologitem;
		REDOLOGTABLET* cVredologtablet;
		TABLETLOC* cVtabletloc;
	
		iVrecovered = 0;
		LOG(INFO) <<	"Redo recov:" << cRedologfileloc->sVfilename.c_str ();
		FlushLogFiles(GLOG_INFO);

		for (int iV = 0; iV < cVtabletpool.count (); iV++) cVtabletpool.get__ (iV)->iVrecyclecounter = 0;		
		cVfsys.begin (-1, cRedologfileloc->sVfilename);
		while (cVfsys.pcfds<PrtoLlog> (&cVredologitem, bVparsed)) {
			if (bVparsed) {
				cVtabletloc = gettb (cVredologitem.mutable_cvstatement ()->mutable_cvmutable (), false);			
				cVredologtablet = getrt (cRedologfileloc, cVtabletloc);

#ifdef iCdebuglog
				if (cVredologtablet->sVindex.AnsiPos (iCdebugtable) > 0) {
					AnsiString sVkeymain = cVredologitem.mutable_cvstatement ()->mutable_cvkey ()->svmain ();
					if (sVkeymain.prntb ().AnsiPos (iCdebugindex) > 0 && sVkeymain.prntb ().AnsiPos (iCdebugstartindex) == 1) {
						bool bVinsert= cVredologitem.mutable_cvstatement ()->mutable_cvkey ()->ivstate () == UPSERT;
						int64vcl iVtime = cVredologitem.mutable_cvstatement ()->mutable_cvkey ()->ivtimestamp ();
						printf ("%s %s i=%i t=%lld\n", cVredologtablet->sVindex.c_str (), sVkeymain.prntb ().c_str (), bVinsert, iVtime);
					}
				}
#endif

				sVrowtimestamp = CoreButl::lpadt (cVredologitem.svtimestamp(), CoreTbsv::sCchar0, CoreTbsv::iCredotimestampsize);

#ifdef iCdebuglog
				printf ("leggo %s, (%s) timestamp %s > %s\n", 
					cVtabletloc->sVindex.c_str (), 
					CoreAlog::logrx (&cVreverse, cVredologtablet->sVtimestamp).c_str (), 
					sVrowtimestamp.debug (false).c_str (), 
					CoreButl::lpadt (cVtabletloc->cVtablet.getcr (), sCredotimestamppad, iCredotimestampsize).debug (false).c_str ()); 
#endif

				if (sVrowtimestamp > cVtabletloc->cVtablet->getcr ())  {
					statm (cVredologitem.mutable_cvstatement (), NULL, sVrowtimestamp);		
					cVtabletloc->iVrecyclecounter++;
					iVrecovered++;
				}
			}
		}
		CoreButl::splpt (cRedologfileloc->sVfilename, sVpath, sVfilename);
		if (iVrecovered > 0) {
			if (bPrintf) printf ("%s: %i\n", sVfilename.c_str (), iVrecovered);			
			for (int iV = 0; iV < cVtabletpool.count (); iV++) {
				iVtemp = cVtabletpool.get__ (iV)->iVrecyclecounter;
				LOG_IF(INFO, iVtemp > 0) <<	
					"Redo load: " << sVfilename.c_str () << 
					", " << cVtabletpool.get__ (iV)->sVindex.c_str () << 
					": " << iVtemp;
			}	
			FlushLogFiles(GLOG_INFO);
		}
		return true;
	}

	/*LOAD All*/
	bool __fastcall CoreAlog::loada (bool bPrintf) {
		int64vcl iVredologorderingcounter;
		AnsiString sVtempfilename;
		AnsiString sVtempfilenamenoext;
		CoreList<AnsiString> cVfilenames;
		CoreList<ORDEREDREDOLOGFILELOC> cVorderedredologfilepool;
		ORDEREDREDOLOGFILELOC* cVorderedredologfileloc;
		REDOLOGFILELOC* cVredologfileloc;
		
		CoreHutl::readr (sVredologspath, sCredologprefix + (AnsiString) "*." + sCredologext, cVfilenames);
		for (int iV = 0; iV < cVfilenames.count (); iV++) {		
			sVtempfilename = *cVfilenames.get__ (iV);
			if (CoreButl::left_ (sVtempfilename, CoreSstb::sCversiondelimiter) != sCredologprefix) continue;
			sVtempfilenamenoext = CoreButl::left_ (sVtempfilename, ".");
			iVredologorderingcounter = std::stoull(	CoreButl::left_ (	CoreButl::right (*cVfilenames.get__ (iV), 
																							CoreAlog::sCredologdelimiter), ".").to_string ());
			iVredologcounter = Max (iVredologcounter, iVredologorderingcounter);
			cVorderedredologfileloc = 
				cVorderedredologfilepool.bnset (CoreButl::lpad_ (CoreButl::ci64_ (iVredologorderingcounter), '0', 20));
			cVorderedredologfileloc->iVredologorderingcounter = iVredologorderingcounter;
			cVorderedredologfileloc->sVtempfilename = sVtempfilename;
		}
		iVredologcounter++;
		cVredologfileloc = newri (true);
		for (int iV = 0; iV < cVorderedredologfilepool.count (); iV++) {
			cVorderedredologfileloc = cVorderedredologfilepool.get__ (iV);			
			if (bPrintf) printf (".");
			load_ (sVredologspath + cVorderedredologfileloc->sVtempfilename);	
		}
		rcvry (bPrintf);
		return true;
	}

	/*LOAD*/
	bool __fastcall CoreAlog::load_ (AnsiString sFilename) {
		bool bVparsed;
		CoreFsys cVfsys (cVpool);
		PrtoLlog cVredologitem;
		REDOLOGTABLET* cVredologtablet;
		TABLETLOC* cVtabletloc;
		REDOLOGFILELOC* cVredologfileloc;

		cVredologfileloc = cVredologfilepool.set__ ();
		cVredologfileloc->bVvalid = true;
		cVredologfileloc->sVfilename = sFilename;
		cVfsys.begin (-1, cVredologfileloc->sVfilename);
		while (cVfsys.pcfds<PrtoLlog> (&cVredologitem, bVparsed)) {
			if (bVparsed) {
				cVtabletloc = gettb (cVredologitem.mutable_cvstatement ()->mutable_cvmutable (), false);
				cVredologtablet = getrt (cVredologfileloc, cVtabletloc);
				cVredologtablet->sVtimestamp = 
					CoreButl::lpadt (cVredologitem.svtimestamp (), CoreTbsv::sCchar0, CoreTbsv::iCredotimestampsize);			
			}
		}

		LOG(INFO) <<	"Redo load: " << cVredologfileloc->sVfilename.c_str ();

		for (int iV = 0; iV < cVredologfileloc->cVredologtabletpool.count (); iV++) {
			cVredologtablet = cVredologfileloc->cVredologtabletpool.get__ (iV);
			LOG(INFO) <<	"           " << cVredologtablet->sVindex.c_str () <<
										", redo timestamp: " << CoreAlog::logrx (cVreverse, cVredologtablet->sVtimestamp).c_str () <<
										", tablet timestamp: " << CoreAlog::logrx (cVreverse, cVredologtablet->cVtabletserver->getcr ()).c_str ();
			LOG(INFO) <<	"           " << cVredologtablet->sVindex.c_str () <<
										", redo timestamp: " << cVredologtablet->sVtimestamp.debug (false).c_str () << 										
										", tablet timestamp: " << cVredologtablet->cVtabletserver->getcr ().debug (false).c_str (); 												
		}
		FlushLogFiles(GLOG_INFO);
		return true;
	}

	/*DEBUG*/
	AnsiString __fastcall CoreAlog::debug () {
		AnsiString sVreturn;

		sVreturn = "";
		for (int iV = 0; iV < cVredologfilepool.count (); iV++) 
			sVreturn += (AnsiString) cVredologfilepool.get__ (iV)->sVfilename + 
									(AnsiString) "\n";
		return sVreturn;
	}

	/*GET Table Key*/
	AnsiString __fastcall CoreAlog::gettk (PrtoLmtb* cMutable, AnsiString& sNamespace, AnsiString& sTable) {

		sNamespace = cMutable->svnamespace ();
		sTable = cMutable->svtable ();
		return sNamespace + sCtablekeydelimiter + sTable;
	}

	/*SET Table Key*/
	void __fastcall CoreAlog::settk (AnsiString sIndex, PrtoLmtb* cMutable) {
		int iVdelimiter;
		
		iVdelimiter = sIndex.AnsiPos (sCtablekeydelimiter);
		cMutable->set_svnamespace (sIndex.SubString (1, iVdelimiter - 1).to_string ());
		cMutable->set_svtable (CoreButl::sbstr (sIndex, iVdelimiter + 1).to_string ());
	}

	/*GET Table Key*/
	AnsiString __fastcall CoreAlog::gettk (PrtoLmtb* cMutable) {
		AnsiString sVnamespace;
		AnsiString sVtable;

		return gettk (cMutable, sVnamespace, sVtable);
	}

	/*COMPAction*/
	bool __fastcall CoreAlog::compa (	PrtoLmtb* cMutable, CoreTbsv::iCcompactions iCompaction, 
																		int iCompactionlevel, int iMaxcompactionlevel) {
		int iVcompactionlevel;
		bool bVuplevels;
		TABLETLOC* cVtabletloc;

		cVtabletloc = gettb (cMutable, false);
		if (cVtabletloc != NULL) {
			switch (iCompaction) {
				case CoreTbsv::iCmajor:
					iVcompactionlevel = iCompactionlevel;
					while (cVtabletloc->cVtablet->majcp (bVuplevels, iVcompactionlevel) && iVcompactionlevel < iMaxcompactionlevel) {
						if (!bVuplevels) break;
						iVcompactionlevel++;
					}
					while (cVtabletloc->cVtablet->majcp (bVuplevels, iVcompactionlevel));
					break;
				case CoreTbsv::iCminor:
					cVtabletloc->cVtablet->mincp (false);
					break;
				case CoreTbsv::iCflush:
					cVtabletloc->cVtablet->mincp (true);
					break;
				case CoreTbsv::iCmerge:
					while (cVtabletloc->cVtablet->majcp (bVuplevels, iCompactionlevel, false, true));
					break;
			}
		}
		return true;
	}

	/*SET ABort*/
	void __fastcall CoreAlog::setab (bool bAbort) {

		bVabort = bAbort;
		for (int iV = 0; iV < cVtabletpool.count (); iV++)
			cVtabletpool.get__ (iV)->cVtablet->setab (bAbort);
	}

	/*SWiTCH*/
	bool __fastcall CoreAlog::swtch () {

		if (iVactivelogdim > iVredologdim) {
			newrl ();
			rcycl ();
		}
		return true;
	}

	/*InFo TaBlet*/
	bool __fastcall CoreAlog::iftbl (AnsiString sTabletfilename, PrtoTabl* cTablet, bool bDiagnostic) {
		bool bVreturn;
		PrtoLmtb cVmutable;
		int iVdelimiter;
		AnsiString sVnamespace;
		AnsiString sVtable;
		TABLETLOC* cVtablet;

		bVreturn = false;
		iVdelimiter = sTabletfilename.AnsiPos (CoreAlog::sCtabletdelimiter);
		if (iVdelimiter > 1) {
			sVnamespace = sTabletfilename.SubString (1, iVdelimiter - 1);
			sVtable= CoreButl::sbstr (sTabletfilename, iVdelimiter + 1);
			cVmutable.set_svnamespace (sVnamespace.UpperCase ().to_string ());
			cVmutable.set_svtable (sVtable.UpperCase ().to_string ());
			cVtablet = gettb (&cVmutable, false, false);
			if (cVtablet != NULL && cVtablet->cVtablet->getti (cTablet->mutable_cvtabletelement ())) bVreturn = true;
			sVtable = sCitableprefix + sVtable;
			cVmutable.set_svtable (sVtable.UpperCase ().to_string ());
			cVtablet = gettb (&cVmutable, false, false);
			if (cVtablet != NULL && cVtablet->cVtablet->getti (cTablet->mutable_cvindextabletelement (), bDiagnostic)) bVreturn = true;
		}
		return bVreturn;
	}

	/*GET TaBlet server*/
	PTR_TABLETSERVER __fastcall CoreAlog::gettb (PrtoLmtb* cMutable) {
		TABLETLOC* cVtabletloc;
		PTR_TABLETSERVER cVdummy;
		
		cVtabletloc = gettb (cMutable, false);
		if (cVtabletloc != NULL) return cVtabletloc->cVtablet;
		else return cVdummy;
	}

	/*is INDEX*/
	bool __fastcall CoreAlog::index (PrtoLmtb* cMutable) {
		AnsiString sVtable;
		
		sVtable = cMutable->svtable ();
		return (sVtable.Pos (CoreAlog::sCitableprefix) == 1);
	}

	/*IndeX 2 Table*/
	bool __fastcall CoreAlog::ix2tb (PrtoLmtb* cMutable) {

		if (index (cMutable)) {cMutable->set_svtable (
			CoreButl::sbstr ((AnsiString) cMutable->svtable (), 1+CoreAlog::sCitableprefix.Length ()).to_string ());
			return true;
		} else return false;
	}

	/*Table 2 index*/
	void __fastcall CoreAlog::tb2ix (PrtoLmtb* cMutable) {

		if (!index (cMutable)) cMutable->set_svtable (CoreAlog::sCitableprefix.to_string () + cMutable->svtable ());
	}

	/*GET working SiZe*/
	int64vcl __fastcall CoreAlog::getsz () {
		int64vcl iVsize;

		iVsize = 0;
		cVtabletpoolmutex.locks ();
		for (int iV = 0; iV < cVtabletpool.count (); iV++) iVsize += cVtabletpool.get__ (iV)->cVtablet->getsz ();
		cVtabletpoolmutex.relse ();
		return iVsize;
	}