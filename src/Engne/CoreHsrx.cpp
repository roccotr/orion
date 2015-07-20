#include "CoreHsrx.hpp"
#include "TmplList.cpp"
#include "TmplFsys.cpp"
#include "CoreMmh3.hpp"
#include "TmplClps.cpp"
#include <math.h>

using namespace orion;

	const int CoreHsrx::iCcollisionfactor = 5;
	const int CoreHsrx::iCshorterindexes = -1;
	const AnsiString CoreHsrx::sCindexmagic = "ORNI";

  __fastcall CoreHsrx::CoreHsrx (PTR_POOL cPool) {
		
		cVpool = cPool;
		cVsortedradix = boost::shared_ptr<CoreStix> (new CoreStix (cVpool));
  }

  __fastcall CoreHsrx::~CoreHsrx () {

  }

	/*GET Common Prefix*/
	AnsiString __fastcall CoreHsrx::getcp (AnsiString sKey1, AnsiString sKey2) {
		int iVcommonlength;
		int iV;
		char* bVkey1;
		char* bVkey2;

		bVkey1 = sKey1.c_str ();
		bVkey2 = sKey2.c_str ();
		iVcommonlength = Min (sKey1.Length (),sKey2.Length ()) ;
		for (iV = 0; iV < iVcommonlength; iV++) {
			if (bVkey1 [iV] != bVkey2 [iV]) break;
		}
		if (sKey1.Length () > 0) return sKey1.SubString (0, iV);
		else return sKey1;
	}

	/*BUILD*/
	bool __fastcall  CoreHsrx::build (PTR_CUSTOMSORTEDINDEX cSortedindex, bool bKeepindexed, bool bCreatecommonprefix) {
		AnsiString sVcurrentindex;
		AnsiString sVcurrentradix;
		AnsiString sVpreviousindex;
		int iVlinkedindex;
		//unsigned int iVhash;
		//unsigned int iVbucket;
		CoreStib::SORTEDINDEXLOC* cVsortedindexloc;
		PTR_SORTEDINDEX cVbackupstix (new CoreStix (cVpool));

		cVsortedradix->clear ();
		for (CUSTOMSORTEDINDEXPOS iV = cSortedindex->first (); iV < cSortedindex->last_ (); iV++) {
			cVsortedradix->setil (cSortedindex->getil (iV));
			cVbackupstix ->setil (cSortedindex->getil (iV));
		}

		if (bCreatecommonprefix) {
			sVpreviousindex = "";		
			for (int iV = 0; iV < cVbackupstix->count (); iV++) {			
				sVcurrentindex = cVbackupstix->get__ (iV, true);
				sVcurrentradix = getcp (sVpreviousindex, sVcurrentindex);		
				if (sVcurrentradix != "") cVsortedradix->add__ (sVcurrentradix, 0, iCshorterindexes);
				sVpreviousindex = sVcurrentindex;
			}
		}

		iVlinkedindex = 0;
		for (int iV = 0; iV < cVsortedradix->count (); iV++) {
			sVcurrentradix = cVsortedradix->get__ (iV);			
			for (int iVinner = iVlinkedindex; iVinner < cVbackupstix->count (); iVinner++) {
				sVcurrentindex = cVbackupstix->get__ (iVinner);
				iVlinkedindex = iVinner;
				if (sVcurrentindex >= sVcurrentradix) break;
			}
			cVsortedradix->setix (iV, iVlinkedindex);
		}		

		cVsortedradix->deldp ();
		if (bKeepindexed) {
			for (int iV = 0; iV < cVsortedradix->count (); iV++) {
				cVsortedindexloc = cVsortedradix->getil (iV);
				cVsortedindexloc->iVindexed = cVbackupstix->getix (cVsortedindexloc->iVindexed);
			}
		}

		return true;
	}

	void __fastcall CoreHsrx::clear () {

		cVsortedradix->clear ();
	}


	/*HASH function*/
	unsigned int __fastcall CoreHsrx::hash_ (AnsiString sKey) {
		unsigned int iVhash;

		MurmurHash3_x86_32 ( (const void *) sKey.c_str (), sKey.Length (), 0, &iVhash);
		return iVhash;
	}

	/*QUERY*/
	int64vcl __fastcall CoreHsrx::query (	AnsiString sKey) {

		return cVsortedradix->getix (Max (cVsortedradix->bnsrc (sKey) - 1, 0));
	}

	/*ReMaPpiNG*/
	bool __fastcall CoreHsrx::rmpng (PTR_REMAPPINGLIST cIndexremapping) {
		int iV;
		CoreStib::SORTEDINDEXLOC* cVsortedindexloc;

		for (iV = 0; iV < cVsortedradix->count (); iV++)  {
			cVsortedindexloc = cVsortedradix->getil (iV);
			cVsortedindexloc->iVindexed = cIndexremapping->get__ (cVsortedindexloc->iVindexed)->iVindexed;
		}
		return true;
	}

	/*TO CoNTent*/
	bool __fastcall CoreHsrx::tocnt (PrtoIndx* cVproto, iCsstabletypes iSstabletype, AnsiString sRedotimestamp) {
		//PrtoHash* cVserializedhashloc;

		cVsortedradix->tocnt (cVproto);
		cVproto->set_ivsstabletype ((iCsstabletype) iSstabletype);
		cVproto->set_svredotimestamp (sRedotimestamp.to_string ());
		return true;
	}

	/*TO CoNTent*/
	bool __fastcall CoreHsrx::tocnt (PTR_FILESYSTEM cFilesystem, iCsstabletypes iSstabletype, AnsiString sRedotimestamp) {
		PrtoIndx cVserializedindx;

		tocnt (&cVserializedindx, iSstabletype, sRedotimestamp);						
		cFilesystem->srlze<PrtoIndx> (&cVserializedindx);
		return true;
	}

	/*TO Record IO*/
	bool __fastcall CoreHsrx::torio (PTR_FILESYSTEM cFilesystem, iCsstabletypes iSstabletype, AnsiString sRedotimestamp) {
		PrtoIndx cVserializedindx;
		PrtoIatm cVatom;
		PrtoIndx cVheader;
		//PrtoHash* cVserializedhashloc;

		cVheader.set_ivsstabletype ((iCsstabletype) iSstabletype);
		cVheader.set_svredotimestamp (sRedotimestamp.to_string ());
		cFilesystem->sctds<PrtoIndx> (&cVheader, true, true);
		cVsortedradix->tocnt (&cVserializedindx);	
		for (int iV = 0; iV < cVserializedindx.cvstix_size (); iV++) {
			cVatom.Clear ();
			cVatom.add_cvstix ()->CopyFrom (cVserializedindx.cvstix (iV));
			cFilesystem->sctds<PrtoIatm> (&cVatom, true, true);
		}
		return true;
	}


	/*FroM CoNTent*/
	bool __fastcall CoreHsrx::fmcnt (PrtoIndx* cVproto, iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp) {
		//PrtoHash* cVserializedhashloc;

		cVsortedradix->fmcnt (cVproto);
		iSstabletype = (iCsstabletypes) cVproto->ivsstabletype ();
		sRedotimestamp = cVproto->svredotimestamp ();
		return true;
	}

	/*FroM CoNTent*/
	bool __fastcall CoreHsrx::fmcnt (PTR_FILESYSTEM cFilesystem, iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp) {
		PrtoIndx cVserializedindx;
		
		cFilesystem->parse<PrtoIndx> (&cVserializedindx); 
		fmcnt (&cVserializedindx, iSstabletype, sRedotimestamp);
		return true;
	}

	/*FroM Record IO*/
	bool __fastcall CoreHsrx::fmrio (PTR_FILESYSTEM cFilesystem, iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp) {
		PrtoIndx cVserializedindx;
		PrtoIatm cVatom;
		//PrtoHash* cVserializedhashloc;
		bool bVparsed;
		int64vcl iVsize;
	
		iVsize = cFilesystem->fsize ();
		cFilesystem->pcfds<PrtoIndx> (&cVserializedindx, bVparsed, -1, true); 
		if (!bVparsed) return false;
		iSstabletype = (iCsstabletypes) cVserializedindx.ivsstabletype ();
		sRedotimestamp = cVserializedindx.svredotimestamp ();
		while (cFilesystem->pcfds<PrtoIatm> (&cVatom, bVparsed, -1, true) && bVparsed) {
			for (int iV = 0; iV < cVatom.cvstix_size (); iV++) 
				cVserializedindx.add_cvstix ()->CopyFrom (cVatom.cvstix (iV));
			cVatom.Clear ();
			if (iVsize == cFilesystem->getpo ()) break;
		};
		cVsortedradix->fmcnt (&cVserializedindx);
		return true;
	}

	/*TO CoNTenT*/
	bool __fastcall CoreHsrx::tocnt (AnsiString sFilename, iCsstabletypes iSstabletype, AnsiString sRedotimestamp, int64vcl& iSize, int iFormat) {
		PTR_FILESYSTEM cVfsys (new CoreFsys (cVpool));
		
		switch (iFormat) {
			case 0:
				tocnt (cVfsys, iSstabletype, sRedotimestamp);
				cVfsys->owrte (sFilename);
				iSize = cVfsys->bsize ();
				return true;
			case 1:
				INDEXHEADER cVheader (1, sCindexmagic);
				cVfsys->begin (0, sFilename, false);
				cVfsys->setmb (&cVheader, sizeof (cVheader), false);
				cVfsys->wchnk (sizeof (cVheader));
				torio (cVfsys, iSstabletype, sRedotimestamp);
				iSize = cVfsys->fsize ();
				cVfsys->end__ ();	
				return true;
		}
		return false;
	}

	/*FroM CoNTent*/
	bool __fastcall CoreHsrx::fmcnt (AnsiString sFilename, iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp, int64vcl& iSize) {
		PTR_FILESYSTEM cVfsys (new CoreFsys (cVpool));
		INDEXHEADER cVheader;
		int iVheadersize = sizeof (cVheader);
		bool bVrecordio;
		bool bVreturn;

		if (!cVfsys->begin (0, sFilename)) return false;
		iSize = cVfsys->fsize ();
		if (iSize > (int64vcl) iVheadersize) {
			if (!cVfsys->rchnk (&iVheadersize, false, false)) return false;
			memcpy (&cVheader, cVfsys->baddr (), iVheadersize);
			bVrecordio = strncmp (cVheader.bCmagic, sCindexmagic.c_str (), sCindexmagic.Length ()) == 0;
		} 
		if (bVrecordio) {
			bVreturn = fmrio (cVfsys, iSstabletype, sRedotimestamp);	
			cVfsys->end__ ();	
		} else {
			cVfsys->end__ ();
			cVfsys = PTR_FILESYSTEM (new CoreFsys (cVpool));
			bVreturn = cVfsys->read_ (sFilename);
			if (bVreturn) bVreturn = fmcnt (cVfsys, iSstabletype, sRedotimestamp);
		}		
		return bVreturn;
	}

	/*GET Sorted indeX*/
	PTR_SORTEDINDEX __fastcall CoreHsrx::getsx () {

		return cVsortedradix;
	}

	/*DEBUG*/
	AnsiString __fastcall CoreHsrx::debug () {
		AnsiString sVreturn;
		int iV;
		CoreStib::SORTEDINDEXLOC* cVsortedindexloc;

		sVreturn = "";
		sVreturn += (AnsiString) "indexes=" + (AnsiString) (int) cVsortedradix->count () + (AnsiString) "\n";
		for (iV = 0; iV < cVsortedradix->count (); iV++)  {
			cVsortedindexloc = cVsortedradix->getil (iV);
			sVreturn += cVsortedindexloc->sVindex.prntb () + 
									" " + (AnsiString) (int) cVsortedindexloc->iVindexed + 
									" " + (AnsiString) (int) cVsortedindexloc->iVmainindexlength + 
									(AnsiString) "\n";
		}
		return sVreturn;
	}


	/*GET Size Count*/
	unsigned int __fastcall CoreHsrx::getsc () {

		return cVsortedradix->count ();
	}

	/*GET SiZe*/
	unsigned int __fastcall CoreHsrx::getsz () {

		return cVsortedradix->getsz ();
	}
