#include "CoreSstb.hpp"
#include "CoreTbsv.hpp"
#include "TmplFsys.cpp"
#include "TmplList.cpp"
#include "TmplClps.cpp"

using namespace google;
using namespace orion;

//#define iCdebugsstb
#define iCdebugmake

	const unsigned int CoreSstb::iVmaxcompactionsize = 2147483648u;
	const AnsiString CoreSstb::sCindxext = "IDX";
	const AnsiString CoreSstb::sCdataext = "DBF";
	const AnsiString CoreSstb::sCversiondelimiter = "_";

  __fastcall CoreSstb::CoreSstb () {
		
	}

  __fastcall CoreSstb::CoreSstb (PTR_POOL cPool) {
		
		init (cPool);
	}

  __fastcall CoreSstb::~CoreSstb () {
  }

	/*INIT*/
  void __fastcall CoreSstb::init (PTR_POOL cPool) {
		
		sVpublicfilename = NULL;
		cVpool = cPool;
		cVserializer = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));
		cVhsrx = boost::shared_ptr<CoreHsrx> (new CoreHsrx (cVpool));
		cVfsysrandomread = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));
		iVsizedbf = 0;
		iVsizeidx = 0;
		iVcachegetkey = -1;
	}

	/*SET MetaData*/
	void __fastcall CoreSstb::setmd (AnsiString sPath, AnsiString sFilename) {

		sVpublicpath = sPath;
		sVpublicfilename = sFilename;
	}

	/*GET Filename Data*/
	AnsiString __fastcall CoreSstb::getfd (AnsiString sPath, AnsiString sFilename) {

		return sPath + (sFilename + "." + sCdataext).UpperCase ();
	}

	/*GET Filename Indexes*/
	AnsiString __fastcall CoreSstb::getfi (AnsiString sPath, AnsiString sFilename) {

		return sPath + (sFilename + "." + sCindxext).UpperCase ();
	}

	/*SAVE Data*/
	bool __fastcall CoreSstb::saved (	BASE_CUSTOMMEMTABLE::PTR cMemtable, CoreHsrx::iCsstabletypes iSstabletype, AnsiString sRedotimestamp) {
		AnsiString sVkey;
		unsigned int iVdatasize;
		int iVmainindexlength;
		void* vVdata;
		PrtoData cVdata;
		CoreFsys cVfsys (cVpool);
		PTR_REMAPPINGLIST cVindexremapping (new CoreList<CoreHsrx::REMAPPINGINDEXLOC> ());
		if (!cVfsys.begin (-1, getfd (sVpublicpath, sVpublicfilename), false)) return false;
		cVmutex.locks ();

#ifdef iCdebugsstb
		printf ("saved >>> %s %i\n", sVpublicfilename.c_str (), cMemtable->getsc ());
#endif

		for (CUSTOMSORTEDINDEXPOS iV = cMemtable->getsx ()->first (); iV < cMemtable->getsx ()->last_ (); iV++) {
			cMemtable->getdt (iV, sVkey, iVmainindexlength, iVdatasize, &vVdata);
			cVdata.Clear ();
			cVdata.set_svkey (sVkey.to_string ());
			cVdata.set_ivmainkeylength (iVmainindexlength);
			cVdata.set_svopaquevalue ((char*) vVdata, iVdatasize);
			cVindexremapping->set__ ()->iVindexed = cVfsys.getpo (); 
			if (!(&cVfsys)->sctds<PrtoData> (&cVdata)) break;		
		}
		cVfsys.end__ ();

		savei (cMemtable->getsx (), cVindexremapping, false, iSstabletype, sRedotimestamp);
		setrr ();

#ifdef iCdebugsstb
		printf ("saved <<< %s\n", sVpublicfilename.c_str ());
#endif

		cVmutex.relse ();
		return true;
	}

	/*SET Random Read*/
	void __fastcall CoreSstb::setrr () {

		cVfsysrandomread->begin (-1, getfd (sVpublicpath, sVpublicfilename));
		iVsizedbf = cVfsysrandomread->fsize ();
	}

	/*SAVE Index*/
	bool __fastcall CoreSstb::savei (	PTR_CUSTOMSORTEDINDEX cStix, PTR_REMAPPINGLIST cIndexremapping, 
																		bool bKeepindexed, CoreHsrx::iCsstabletypes iSstabletype, AnsiString sRedotimestamp) {
		AnsiString sVindexfilename;
		bool bVsuccess;
		int64vcl iVfilesize;

#ifdef iCdebugsstb
		printf ("savei >>> %s\n", sVpublicfilename.c_str ());
#endif

		cVhsrx->clear ();

#ifdef iCdebugsstb
		printf ("cleared >>> %s\n", sVpublicfilename.c_str ());
		printf ("elementi >>> %i\n", cStix->count ());
#endif

		cVhsrx->build (cStix, bKeepindexed);		

#ifdef iCdebugsstb
		printf ("built >>> %s\n", sVpublicfilename.c_str ());
#endif

		if (cIndexremapping.use_count () > 0) cVhsrx->rmpng (cIndexremapping);

#ifdef iCdebugsstb
		printf ("remapped >>> %s\n", sVpublicfilename.c_str ());
#endif

		sVindexfilename = getfi (sVpublicpath, sVpublicfilename);
		bVsuccess = cVhsrx->tocnt (sVindexfilename, iSstabletype, sRedotimestamp, iVfilesize);

#ifdef iCdebugsstb
		printf ("written >>> %s\n", sVpublicfilename.c_str ());
#endif

		LOG_IF(INFO, bVsuccess) <<	"Sst save:  " << sVindexfilename.c_str () << 
																", count: " << cVhsrx->getsc () <<
																", timestamp: " << sRedotimestamp.debug (false).c_str ();	
		FlushLogFiles(GLOG_INFO);

#ifdef iCdebugsstb
		printf ("savei <<< %s\n", sVpublicfilename.c_str ());
#endif

		return bVsuccess;
	}

	/*LOAD Index*/
	bool __fastcall CoreSstb::loadi (CoreHsrx::iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp) {
		int64vcl iVreadsize;
		AnsiString sVindexfilename;

		cVhsrx->clear ();
		sVindexfilename = getfi (sVpublicpath, sVpublicfilename);
		
		if (cVhsrx->fmcnt (sVindexfilename, iSstabletype, sRedotimestamp, iVreadsize)) {
			iVsizeidx = iVreadsize;
			setrr ();		
			LOG(INFO) <<	"Sst load:  " << sVindexfilename.c_str () << 
										", count: " <<cVhsrx->getsc () <<
										", timestamp: " << sRedotimestamp.debug (false).c_str ();	
			FlushLogFiles(GLOG_INFO);
		} else {
			LOG(WARNING) <<	"Sst load:  " << sVindexfilename.c_str ();	
			FlushLogFiles(GLOG_WARNING);								
		}
		return true;
	}

	/*LOAD Data*/
	bool __fastcall CoreSstb::loadd (CoreHsrx::iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp) {
		bool bVreturn;

		cVmutex.locks ();
		bVreturn = loadi (iSstabletype, sRedotimestamp);
		cVmutex.relse ();
		return bVreturn;
	}

	/*TO MEMtable*/
	bool __fastcall CoreSstb::tomem (BASE_CUSTOMMEMTABLE::PTR cMemtable) {
		bool bVparsed;
		bool bVreturn;
		PTR_SORTEDINDEX cVremappedindex;
		CoreStib::SORTEDINDEXLOC* cVsortedindexloc;
		PrtoData cVdata;

		bVreturn = true;
		cVmutex.locks ();
		cVremappedindex = cVhsrx->getsx ();
		for (int iV = 0; iV < cVremappedindex->count (); iV++) {
			cVsortedindexloc = cVremappedindex->getil (iV);
			if (cVsortedindexloc->iVmainindexlength != CoreHsrx::iCshorterindexes) {
				if (!pcfds (&cVdata, bVparsed, cVsortedindexloc->iVindexed)) {
					bVreturn = false;
					break;	
				}
				cMemtable->put__ (	cVsortedindexloc->sVindex, cVsortedindexloc->iVmainindexlength,
														cVdata.mutable_svopaquevalue ()->length (), 
														(void*) cVdata.mutable_svopaquevalue ()->c_str ());
			}
		}
		cVmutex.relse ();
		return bVreturn;
	}

	/*GET Filesystem Random read*/
	PTR_FILESYSTEM __fastcall CoreSstb::getfr () {

		return cVfsysrandomread;
	}

	/*MAJor ComPaction*/
	bool __fastcall CoreSstb::majcp (CoreSstb::PTR cS1, CoreSstb::PTR cS2, AnsiString sRedotimestamp) {
		bool bVparsed;
		AnsiString sVredotimestamp;
		PTR_CUSTOMSORTEDINDEX cVnewstix = boost::shared_ptr<BASE_CUSTOMSORTEDINDEX> (new BASE_CUSTOMSORTEDINDEX (cVpool));
		CoreSstb::PTR cVsstables [2];
		PTR_SORTEDINDEX cVremappedindexes [2];
		PTR_FILESYSTEM cVappendwrite = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));
		AnsiString sVkey [2];
		int iVpointer [2];
		int iVnextscanning;
		int iVscanning;
		int iVcounter;
		boolean bVflush;
		CoreStib::SORTEDINDEXLOC* cVsortedindexloc;
		PrtoData cVreaddata;	
		PTR_REMAPPINGLIST cVdummy;

		if (!cVappendwrite->begin (-1, getfd (sVpublicpath, sVpublicfilename), false)) return false;
		cVmutex.locks ();
		cS1->locks ();
		cS2->locks ();

#ifdef iCdebugsstb
		printf ("majcp >>> %s\n", sVpublicfilename.c_str ());
#endif

		iVcounter = 0;
		cVremappedindexes [0] = cS1->cVhsrx->getsx ();
		cVremappedindexes [1] = cS2->cVhsrx->getsx ();
		cVsstables [0] = cS1;
		cVsstables [1] = cS2;
		iVpointer [0] = iVpointer [1] = 0;
		sVkey [1] = cVremappedindexes [1]->get__ (iVpointer [1]);

		iVscanning = 0;
		bVflush = false;
		while (iVpointer [iVscanning] < cVremappedindexes [iVscanning]->count ()) {
			cVsortedindexloc = cVremappedindexes [iVscanning]->getil (iVpointer [iVscanning]);
			if (cVsortedindexloc->iVmainindexlength == CoreHsrx::iCshorterindexes) {				
				iVpointer [iVscanning]++;
				continue;
			}
			sVkey [iVscanning] = cVsortedindexloc->sVindex;
			if (!bVflush) iVnextscanning = sVkey [0] <= sVkey [1] ? 0 : 1;			
			else iVnextscanning = iVscanning;
			if (iVscanning == iVnextscanning) {
				cVsstables [iVscanning]->pcfds (&cVreaddata, bVparsed, cVsortedindexloc->iVindexed);
				cVnewstix->addns (sVkey [iVscanning], cVappendwrite->getpo (), cVsortedindexloc->iVmainindexlength);
				cVappendwrite->sctds<PrtoData> (&cVreaddata);							
				iVpointer [iVscanning]++;
				if (iVpointer [iVscanning] == cVremappedindexes [iVscanning]->count ()) {
					iVnextscanning = 1 - iVscanning;
					bVflush = true;
				}
			}
			iVscanning = iVnextscanning; 
		}
		savei (cVnewstix, cVdummy, true, CoreHsrx::iCsstable, sRedotimestamp);
		setrr ();

#ifdef iCdebugsstb
		printf ("majcp <<< %s\n", sVpublicfilename.c_str ());
#endif

		cS2->relse ();
		cS1->relse ();
		cVmutex.relse ();
		return true;
	}

	/*SHRiNK*/
	bool __fastcall CoreSstb::shrnk (CoreSstb::PTR cSource, AnsiString sRedotimestamp, bool bSoft, int64vcl iGracetimestamp) {
		bool bVparsed;
		int iVpointer;
		int iVlength;
		PTR_SORTEDINDEX cVremappedindex;
		AnsiString sVkey;
		AnsiString sVmain;
		AnsiString sVlastmain;
		CoreStib::SORTEDINDEXLOC* cVsortedindexloc;
		PTR_REVERSE cVreverse = boost::shared_ptr<CoreRvrs> (new CoreRvrs ());
		PTR_FILESYSTEM cVappendwrite = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));
		PTR_CUSTOMSORTEDINDEX cVnewstix = boost::shared_ptr<BASE_CUSTOMSORTEDINDEX> (new BASE_CUSTOMSORTEDINDEX (cVpool));
		PrtoData cVreaddata;	
		PTR_REMAPPINGLIST cVdummy;

		iVpointer = 0;
		sVlastmain = "";
		cVremappedindex = cSource->cVhsrx->getsx ();
		if (!cVappendwrite->begin (-1, getfd (sVpublicpath, sVpublicfilename), false)) return false;
		cVmutex.locks ();
		cSource->locks ();
		while (iVpointer  < cVremappedindex->count ()) {
			cVsortedindexloc = cVremappedindex->getil (iVpointer);
			if (cVsortedindexloc->iVmainindexlength == CoreHsrx::iCshorterindexes) {
				iVpointer++;
				continue;
			}
			iVlength = cVsortedindexloc->iVmainindexlength > 0 ? cVsortedindexloc->iVmainindexlength - 1: sVkey.Length ();
			sVkey = cVsortedindexloc->sVindex;
			sVmain  = sVkey.SubString (1, iVlength);
			
			if (sVmain != sVlastmain)  {
				if (CoreTbsv::istmb (cVreverse, sVkey)) {
					if (bSoft) {
						cSource->pcfds (&cVreaddata, bVparsed, cVsortedindexloc->iVindexed);
						if (bVparsed && !CoreTbsv::extmb (cVreverse, cVreaddata.svopaquevalue (), iGracetimestamp)) {
							cVnewstix->addns (sVkey, cVappendwrite->getpo (), cVsortedindexloc->iVmainindexlength);
							cVappendwrite->sctds<PrtoData> (&cVreaddata);								
						}
					}
				} else {
					cSource->pcfds (&cVreaddata, bVparsed, cVsortedindexloc->iVindexed);
					if (bVparsed) {
						cVnewstix->addns (sVkey, cVappendwrite->getpo (), cVsortedindexloc->iVmainindexlength);
						cVappendwrite->sctds<PrtoData> (&cVreaddata);		
					}
				}
			}
			sVlastmain = sVmain;
			iVpointer++;
		}
		savei (cVnewstix, cVdummy, true, CoreHsrx::iCsstable, sRedotimestamp); 
		setrr ();
		cSource->relse ();
		cVmutex.relse ();
		return true;
	}

	/*GET*/
	bool __fastcall CoreSstb::get__ (AnsiString& sKey, PrtoLval* cValue) {
		int64vcl iVstartindex;
		bool bVparsed;
		AnsiString sVcursor;
		PrtoData cVdata;			

		cVmutex.locks ();
		if (	iVcachegetkey >= 0 && 
					sVcachegetkeyinput <= sKey && sVcachegetkeyoutput >= sKey) {
			iVstartindex = iVcachegetkey;
		} else {
			iVcachegetkey = iVstartindex = cVhsrx->query (sKey);
			sVcachegetkeyoutput = NULL;
		}
		sVcachegetkeyinput = sKey;
		while (pcfds (&cVdata, bVparsed, iVstartindex)) {	
			sVcursor = cVdata.svkey ();
			if (sVcursor >= sKey) {
				sVcachegetkeyoutput = sKey = sVcursor;
				cVserializer->setmb (	(void*) cVdata.mutable_svopaquevalue ()->c_str (), 
															cVdata.mutable_svopaquevalue ()->length ());
				cVserializer->parse<PrtoLval> (cValue);
				cVmutex.relse ();
				return true;
			}
			iVstartindex = -1;
		}
		cVmutex.relse ();
		return false;
	}

	/*GET ID*/
	AnsiString __fastcall CoreSstb::getid () {

		return getfn ();
	}

	/*DELete FileS*/
	bool __fastcall CoreSstb::delfs () {

		cVfsysrandomread.reset ();
		CoreXutl::delfl (getfd (sVpublicpath, sVpublicfilename).c_str ());
		CoreXutl::delfl (getfi (sVpublicpath, sVpublicfilename).c_str ());
		return true;
	}

	/*LOCKS*/
	void __fastcall CoreSstb::locks () {
		
		cVmutex.locks ();
	}
	
	/*RELSE*/
	void __fastcall CoreSstb::relse () {

		cVmutex.relse ();
	}


	/*DEBUG*/
	AnsiString __fastcall CoreSstb::debug (PrtoData* cData) {
		int iVmainkeylength;
		AnsiString sVreturn;
		AnsiString sVkey = cData->svkey ();
		PrtoLval cVvaluecolumn;

		sVreturn =  sVkey;
		iVmainkeylength = cData->ivmainkeylength ();
		if (iVmainkeylength > 0)
			sVreturn += (AnsiString) " -> " + (AnsiString) iVmainkeylength;
		cVvaluecolumn.ParseFromArray (cData->mutable_svopaquevalue ()->c_str (), cData->mutable_svopaquevalue ()->length ());
		sVreturn += (AnsiString) " ->\n" + (AnsiString) cVvaluecolumn.svopaquevalue ();
		return sVreturn;
	}

	AnsiString __fastcall CoreSstb::getfn () {

		return sVpublicfilename;
	}

	/*GET Size Count*/
	unsigned int __fastcall CoreSstb::getsc () {
		unsigned int iVreturn;

		cVmutex.locks ();
		iVreturn = cVhsrx->getsc ();
		cVmutex.relse ();
		return iVreturn;
	}

	/*GET ReS*/
	unsigned int __fastcall CoreSstb::getrs () {
		unsigned int iVreturn;

		cVmutex.locks ();
		iVreturn = cVhsrx->getsz ();
		cVmutex.relse ();
		return iVreturn;
	}

	/*GET SiZe*/
	unsigned int __fastcall CoreSstb::getma () {

		return iVsizeidx;
	}

	/*GET SiZe*/
	unsigned int __fastcall CoreSstb::getsz () {

		return iVsizeidx + iVsizedbf;
	}

	/*Parse Chunk From DiSk mutexed*/
	bool CoreSstb::pcfds (PrtoData* cProtodata, bool &bParsed, int64vcl iPosition) {
		bool bVreturn;

		cVmutexpcfds.locks ();
		bVreturn = cVfsysrandomread->pcfds<PrtoData> (cProtodata, bParsed, iPosition);
		cVmutexpcfds.relse ();
		return bVreturn;
	}

	/*CHecK IndeX*/
	bool __fastcall CoreSstb::chkix (AnsiString sFilename, AnsiString& sRedotimestamp, CoreHsrx::iCsstabletypes& iSstabletype) {
		bool bVreturn;
		PTR_POOL cVpool (new CorePool ());
		
		CoreHsrx cVhsrxindex (cVpool);
		int64vcl iVfilesize;

	
		bVreturn = cVhsrxindex.fmcnt (sFilename + CoreSstb::sCindxext, iSstabletype, sRedotimestamp, iVfilesize);		
		return bVreturn;
	}

	/*CHKDsK*/
	bool __fastcall CoreSstb::chkdk (AnsiString sPath, AnsiString sFilename) {
		AnsiString sVfilename = sPath + sFilename + ".";
		bool bVreturn;
		bool bVparse;
		PrtoData cVelement;
		PTR_POOL cVpool (new CorePool ());
		PTR_FILESYSTEM cVfiledata (new CoreFsys (cVpool));
		CoreHsrx cVhsrxindex (cVpool);
		AnsiString sVredotimestamp;
		CoreHsrx::iCsstabletypes iVsstabletype;
		PTR_SORTEDINDEX cVsortedindex;
		CoreStib::SORTEDINDEXLOC* cVsortedindexloc;
		int64vcl iVfilesize;

		bVreturn = cVhsrxindex.fmcnt (sVfilename + CoreSstb::sCindxext, iVsstabletype, sVredotimestamp, iVfilesize);
		if (bVreturn ) {
			cVsortedindex = cVhsrxindex.getsx ();
			if (!cVfiledata->begin (-1, sVfilename + CoreSstb::sCdataext)) {
				printf ("ERROR\n");
				return false;
			}
			for (int iV = 0; iV < cVsortedindex->count (); iV++) {
				cVsortedindexloc = cVsortedindex->getil (iV);

#ifdef iCdebugmake
				printf ("CHECK   %s %lld -> ", 
					cVsortedindexloc->sVindex.prntb ().c_str (),
					cVsortedindexloc->iVindexed);
#endif

				cVfiledata->pcfds<PrtoData> (&cVelement, bVparse, cVsortedindexloc->iVindexed);	
				if (bVparse && cVelement.svkey () == cVsortedindexloc->sVindex.to_string ()) {
					printf ("OK\n");
				} else {
					printf ("ERROR\n");
					printf ("%s <> %s\n", 
						((AnsiString) cVelement.svkey ()).prntb ().c_str (), cVsortedindexloc->sVindex.prntb ().c_str ());
					return false;
				}
			}
			cVfiledata->end__ ();
		} else {
			printf ("ERROR\n");
			return false;
		}
		//cVfileidx->end__ ();
		return bVreturn;
	}

	/*MAKE*/
	bool __fastcall CoreSstb::make_ (AnsiString sPath, AnsiString sFilenameold, AnsiString sFilenamenew) {

		AnsiString sVfilenameold = sPath + sFilenameold + ".";
		AnsiString sVfilenametemp = sPath + "_" + sFilenameold + ".";
		AnsiString sVfilenamenew = sPath + sFilenamenew + ".";
		PrtoData cVelement;
		PTR_POOL cVpool (new CorePool ());
		PTR_FILESYSTEM cVfileold (new CoreFsys (cVpool));
		PTR_FILESYSTEM cVfiletemp (new CoreFsys (cVpool));
		PTR_FILESYSTEM cVfilenew (new CoreFsys (cVpool));
		PTR_CUSTOMSORTEDINDEX cVstixindex (new BASE_CUSTOMSORTEDINDEX (cVpool));
		PTR_SORTEDINDEX cVpurestixindex (new CoreStix (cVpool));
		CoreHsrx cVhsrxindex (cVpool);
		CoreHsrx::iCsstabletypes iVsstabletype;
		AnsiString sVredotimestamp;
		bool bVvalid;
		bool bVparse;
		bool bVinsert;
		CUSTOMSORTEDINDEXPOS iVposition;
		int64vcl iVsize;
		int64vcl iVpos;
		int64vcl iVfilesize;

		chkix (sVfilenameold, sVredotimestamp, iVsstabletype);
		cVfileold = PTR_FILESYSTEM (new CoreFsys (cVpool));
		bVvalid = cVfileold->begin (-1, sVfilenameold + CoreSstb::sCdataext);
		cVfiletemp->begin (-1, sVfilenametemp + CoreSstb::sCdataext, false);
		cVfileold->setpo (0);
		if (bVvalid) {
			bVparse = true;
			iVsize = cVfileold->fsize ();
			iVpos = 0;
			while (iVpos < iVsize) {
				iVpos = cVfileold->getpo ();
				cVfileold->pcfds<PrtoData> (&cVelement, bVparse, -1);	
				if (bVparse) {
					iVposition = cVstixindex->bnsrc (cVelement.svkey ()) - 1;
					bVinsert = !(	iVposition >= 0 && iVposition < cVstixindex->last_ () && 
												cVstixindex->get__ (iVposition) == cVelement.svkey ());
					if (bVinsert ) {
						cVstixindex->add__ (cVelement.svkey (), iVpos, cVelement.ivmainkeylength ());

#ifdef iCdebugmake
						printf ("READ    %s - %i, %lld\n", 
							((AnsiString) cVelement.svkey ()).prntb ().c_str (), 
							cVelement.ivmainkeylength (), iVpos);
#endif

						cVfiletemp->sctds<PrtoData> (&cVelement);
					}
				} else break;
				iVpos = cVfileold->getpo ();
			}
		}
		cVfileold->end__ ();
		cVfiletemp->end__ ();	

		cVfilenew = PTR_FILESYSTEM (new CoreFsys (cVpool));
		cVfiletemp = PTR_FILESYSTEM (new CoreFsys (cVpool));
		cVfilenew->begin (-1, sVfilenamenew + CoreSstb::sCdataext, false);
		bVvalid = cVfiletemp->begin (-1, sVfilenameold + CoreSstb::sCdataext);
		if (bVvalid)
			for (CUSTOMSORTEDINDEXPOS iV = cVstixindex->first (); iV < cVstixindex->last_ (); iV++) {

#ifdef iCdebugmake
				printf ("REWRITE %s %lld ", 
					cVstixindex->getil (iV)->sVindex.prntb ().c_str (),
					cVstixindex->getil (iV)->iVindexed);
#endif

				cVfiletemp->pcfds<PrtoData> (&cVelement, bVparse, cVstixindex->getil (iV)->iVindexed);	
				iVpos = cVfilenew->getpo ();
				if (bVparse) {
					cVfilenew->sctds<PrtoData> (&cVelement);
					cVstixindex->getil (iV)->iVindexed = iVpos;
					printf (" -> %lld\n", cVstixindex->getil (iV)->iVindexed);
				} else {
					printf ("ERROR\n");
					return false;
				}
			}
		cVfilenew->end__ ();
		cVfiletemp->end__ ();
		cVhsrxindex.clear ();
		cVhsrxindex.build (cVstixindex, true);
		cVhsrxindex.tocnt (sVfilenamenew + CoreSstb::sCindxext, iVsstabletype, sVredotimestamp, iVfilesize); 
		CoreXutl::delfl ((sVfilenametemp + CoreSstb::sCdataext).c_str ());
		return true;
	}

	/*DEBUG*/
	bool __fastcall CoreSstb::debug (AnsiString sPath, AnsiString sFilename) {
		AnsiString sVfilename = sPath + sFilename + ".";
		AnsiString sVfilenametemp = sPath + "_" + sFilename + ".";
		PrtoData cVelement;
		PTR_POOL cVpool (new CorePool ());
		PTR_FILESYSTEM cVfileold (new CoreFsys (cVpool));
		PTR_CUSTOMSORTEDINDEX cVstixindex (new BASE_CUSTOMSORTEDINDEX (cVpool));
		PTR_SORTEDINDEX cVpurestixindex (new CoreStix (cVpool));
		CoreHsrx cVhsrxindex (cVpool);
		CoreHsrx::iCsstabletypes iVsstabletype;
		AnsiString sVredotimestamp;
		bool bVvalid;
		bool bVparse;
		bool bVinsert;
		CUSTOMSORTEDINDEXPOS iVposition;
		int64vcl iVsize;
		int64vcl iVpos;

		chkix (sVfilename, sVredotimestamp, iVsstabletype);
		cVfileold = PTR_FILESYSTEM (new CoreFsys (cVpool));
		bVvalid = cVfileold->begin (-1, sVfilename + CoreSstb::sCdataext);
		cVfileold->setpo (0);
		if (bVvalid) {
			bVparse = true;
			iVsize = cVfileold->fsize ();
			iVpos = 0;
			while (iVpos < iVsize) {
				iVpos = cVfileold->getpo ();
				cVfileold->pcfds<PrtoData> (&cVelement, bVparse, -1);	
				if (bVparse) {
					iVposition = cVstixindex->bnsrc (cVelement.svkey ()) - 1;
					bVinsert = !(	iVposition >= 0 && iVposition < cVstixindex->last_ () && 
												cVstixindex->get__ (iVposition) == cVelement.svkey ());
					if (bVinsert ) {

#ifdef iCdebugmake
						printf ("READ    %s - %i, %lld\n", 
							((AnsiString) cVelement.svkey ()).prntb ().c_str (), 
							cVelement.ivmainkeylength (), iVpos);
#endif
						Sleep (100);
					}
				} else break;
				iVpos = cVfileold->getpo ();
			}
		}
		cVfileold->end__ ();

		return true;
	}

	/*DeBuG IndeX*/
	bool __fastcall CoreSstb::dbgix (PTR_POOL cPool, AnsiString sPath, AnsiString sFilename) {
		CoreHsrx cVhashradix (cPool);
		CoreHsrx::iCsstabletypes iVtabletype;
		AnsiString sVredolog;
		int64vcl iVsize;

		cVhashradix.fmcnt (sPath + sFilename + "." + CoreSstb::sCindxext, iVtabletype, sVredolog, iVsize);
		printf ("%s\n", cVhashradix.debug ().c_str ());
		return true;
	}

