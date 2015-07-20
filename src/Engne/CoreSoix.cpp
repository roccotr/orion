#include "CoreSoix.hpp"
#include "TmplList.cpp"
#include "TmplOo2w.cpp"
#include "TmplOo2t.cpp"
#include "TmplClps.cpp"
#include "TmplFsys.cpp"

using namespace orion;

//#define iCdebugdelete

  __fastcall CoreSoix::CoreSoix (PTR_POOL cPool) {
		
		cVpool = cPool;
		iVcurrentsize = 0;
  }

  __fastcall CoreSoix::~CoreSoix () { 
		 
		clear ();
  }

	/*DEALLoc*/
	void CoreSoix::deall (SORTEDINDEXLOC* cSortedindexloc) {
		CoreStib::DATALOC* cVdataloc;
		
		cVdataloc = &cSortedindexloc->cVdataloc;
		if (cVdataloc->iVsize > 0) {			
			iVcurrentsize -= cVdataloc->iVsize;
			if (cVdataloc->vVdata != NULL) cVpool->free_ (cVdataloc->vVdata);
			cVdataloc->vVdata = NULL;
			cVdataloc->iVsize = 0;
		}
	}

	/*ALLOC*/
	void CoreSoix::alloc (SORTEDINDEXLOC* cSortedindexloc, DATALOC* cDataloc) {
		CoreStib::DATALOC* cVdataloc;
		
		cVdataloc = &cSortedindexloc->cVdataloc;
		deall (cSortedindexloc);
		if (cDataloc->iVsize > 0) {
			cVdataloc->vVdata = cVpool->mallc ( CorePool::iCchar, cDataloc->iVsize, "METB");
			memcpy (cVdataloc->vVdata, cDataloc->vVdata, cDataloc->iVsize);
			cVdataloc->iVsize = cDataloc->iVsize;
			iVcurrentsize += cDataloc->iVsize;
		}
	}

	/*BiNary SeaRCh*/ 
	CoreSoix::OO2POSITION __fastcall CoreSoix::bnsrc (AnsiString sKey) {

		return cVlist.bnsrc (&sKey);
	}


	/*ADD*/
	bool __fastcall CoreSoix::add__ (AnsiString sKey, int64vcl iIndexed, short int iMainindexlength) {
		SORTEDINDEXLOC* cVsortedindexloc;
		int iVcompare;

		OO2POSITION iVinsertposition = cVlist.bnsrc (&sKey, iVcompare);
		if (iVcompare != 0) {
			cVsortedindexloc = cVlist.set__ (iVinsertposition, true, &sKey);
			set__ (cVsortedindexloc, iIndexed, iMainindexlength);
			return true;
		} else {
			return false;
		}
	}

	/*ADD OPtimized*/
	bool __fastcall CoreSoix::addop (	AnsiString sKey, int64vcl iIndexed, short int iMainindexlength,
																		bool& bRecycle, unsigned int& iRecycle) {
		SORTEDINDEXLOC* cVsortedindexloc;
		int iVcomparelength;
		int iVcompare;

		bRecycle = false;
		OO2POSITION iVinsposition = cVlist.bnsrc (&sKey, iVcompare);	
		if (iVcompare == 0) return false;
		iVcomparelength = iMainindexlength > 0 ? iMainindexlength - 1: sKey.Length ();
		if (iVinsposition >= 0 && iVinsposition < cVlist.last_ ()) {
			cVsortedindexloc = cVlist.get__ (iVinsposition);
			bRecycle = cVsortedindexloc != NULL && cVsortedindexloc->sVindex.strcmp (sKey, iVcomparelength) == 0;
		}
		if (bRecycle) {			
			iRecycle = cVsortedindexloc->iVindexed;
			set__ (cVsortedindexloc, sKey, iRecycle, iMainindexlength);
		} else {
			cVsortedindexloc = cVlist.set__ (iVinsposition, true, &sKey);			
			set__ (cVsortedindexloc, iIndexed, iMainindexlength);
		}			
		return true;
	}

	/*ADD O2*/
	bool __fastcall CoreSoix::addo2 (	AnsiString sKey, short int iMainindexlength, DATALOC* cDataloc) {
		SORTEDINDEXLOC* cVsortedindexloc;
		int iVcomparelength;
		int iVcompare;
		bool bVrecycle;

		bVrecycle = false;
		OO2POSITION iVinsposition = cVlist.bnsrc (&sKey, iVcompare);	
		if (iVcompare == 0) return false;
		iVcomparelength = iMainindexlength > 0 ? iMainindexlength - 1: sKey.Length ();
		if (iVinsposition >= 0 && iVinsposition < cVlist.last_ ()) {
			cVsortedindexloc = cVlist.get__ (iVinsposition);
			bVrecycle = cVsortedindexloc != NULL && cVsortedindexloc->sVindex.strcmp (sKey, iVcomparelength) == 0;
		}
		if (!bVrecycle) {
			cVsortedindexloc = cVlist.set__ (iVinsposition, true, &sKey);		
		} else {
			cVsortedindexloc->sVindex = sKey;
		}
		cVsortedindexloc->iVmainindexlength = iMainindexlength;
		alloc (cVsortedindexloc, cDataloc);
		return true;
	}

	/*ADD Not Sorted*/
	bool __fastcall CoreSoix::addns (AnsiString sKey, int64vcl iIndexed, short int iMainindexlength) {
		SORTEDINDEXLOC* cVsortedindexloc;

		cVsortedindexloc = cVlist.set__ (OO2POSITIONNULL, true, &sKey);
		set__ (cVsortedindexloc, iIndexed, iMainindexlength);
		return true;
	}

	/*GET*/
	AnsiString __fastcall CoreSoix::get__ (OO2POSITION iPosition, bool bOnlymain) {
		SORTEDINDEXLOC* cVsortedindexloc;
		bool bComplete;
		
		cVsortedindexloc = cVlist.get__ (iPosition);
		if (cVsortedindexloc == NULL) return NULL;
		bComplete = !bOnlymain || cVsortedindexloc->iVmainindexlength <= 0;
		if (bComplete) return cVsortedindexloc->sVindex;
		else return cVsortedindexloc->sVindex.SubString (0, cVsortedindexloc->iVmainindexlength);	
	}

	/*DELete OPtimized*/
	bool __fastcall CoreSoix::delop (	AnsiString sKey, short int iMainindexlength, unsigned int& iRecycle) {
		SORTEDINDEXLOC* cVsortedindexloc;
		AnsiString sVdelete;
		int iVcomparelength;
		bool bVfound;
		bool bVdelete;
		
		bVfound = false;
		OO2POSITION iVinsposition = cVlist.bnsdl (&sKey, bVdelete, sVdelete) - 1;	
		if (iVinsposition >= 0 && iVinsposition < cVlist.last_ ()) {  
			cVsortedindexloc = cVlist.get__ (iVinsposition);
			iVcomparelength = iMainindexlength > 0 ? iMainindexlength - 1: sKey.Length ();	
			bVfound = cVsortedindexloc != NULL && cVsortedindexloc->sVindex.strcmp (sKey, iVcomparelength) == 0;
		}
		if (bVfound) {	
			iRecycle = cVsortedindexloc->iVindexed;
			cVlist.unset (iVinsposition, bVdelete, &sVdelete);
			return true;
		}	else return false;
	}

	/*DELete O2*/
	bool __fastcall CoreSoix::delo2 (	AnsiString& sKey, short int iMainindexlength, bool& bSearchallkey) {
		SORTEDINDEXLOC* cVsortedindexloc;
		AnsiString sVdelete;
		AnsiString sVkey;
		int iVcomparelength;
		int iVcomparelength2;
		bool bVfound;
		bool bVdelete;
		bool bVreturn;

		bVfound = false; 
		bVreturn = false;
		iVcomparelength = iMainindexlength > 0 ? iMainindexlength - 1: sKey.Length ();
		sVkey = bSearchallkey ? sKey : sKey.SubString (0, iVcomparelength);

#ifdef iCdebugdelete
		printf ("-> %s\n", sVkey.prntb ().c_str ());  
#endif

		OO2POSITION iVinsposition = cVlist.bnsdl (&sVkey, bVdelete, sVdelete); 

#ifdef iCdebugdelete
		printf ("%i %i\n", iVinsposition.cVpointer, iVinsposition.iVposition); 
#endif

		if (iVinsposition >= 0 && iVinsposition < cVlist.last_ ()) {
			cVsortedindexloc = cVlist.get__ (iVinsposition);
			bVfound = cVsortedindexloc != NULL && cVsortedindexloc->sVindex.strcmp (sVkey, iVcomparelength) == 0;
			
#ifdef iCdebugdelete
			if (cVsortedindexloc != NULL) printf ("%s == %s %i\n", cVsortedindexloc->sVindex.prntb ().c_str (), sVkey.prntb ().c_str (), iVcomparelength);
#endif

		}

		if (bVfound) {

#ifdef iCdebugdelete
			printf ("%s\n", cVlist.prnta (false).c_str ());
			printf ("%s\n", cVsortedindexloc->sVindex.prntb ().c_str ());
			printf ("%s\n", cVlist.prnta (false).c_str ()); 
#endif
			
			deall (cVsortedindexloc);
			cVlist.unset (iVinsposition, bVdelete, &sVdelete);
			bVreturn = true;

		}	else {
			iVinsposition.rpair (true);
			bSearchallkey = true;


			cVsortedindexloc = cVlist.get__ (iVinsposition);
			if (cVsortedindexloc != NULL) {
				sKey = cVsortedindexloc->sVindex;
				iVcomparelength2 = iMainindexlength > 0 ? iMainindexlength - 1: sKey.Length ();

#ifdef iCdebugdelete
				printf ("%s %i\n", sKey.prntb ().c_str (),
					iVcomparelength2 == iVcomparelength && sKey.strcmp (sVkey, iVcomparelength) == 0);
				printf ("%s\n", iVinsposition.cVpointer->cVlist.debug ().c_str ());
#endif

				bVreturn = iVcomparelength2 == iVcomparelength && sKey.strcmp (sVkey, iVcomparelength) == 0;
			}

		}

		return bVreturn;
	}

	/*DELete DuPlicates*/
	void __fastcall CoreSoix::deldp () {
		AnsiString sVnetindex;
		AnsiString sVprevious;
		AnsiString sVcurrent;
		CoreSoix::SORTEDINDEXLOC* cVsortedindexloc;

		sVprevious = "";
		for (OO2POSITION iV = cVlist.first (); iV < cVlist.last_ (); iV++)  {
			cVsortedindexloc = cVlist.get__ (iV);
			if (cVsortedindexloc->iVmainindexlength < 0) continue;
			sVcurrent = getmn (cVsortedindexloc);
			if (sVcurrent == sVprevious) cVsortedindexloc->sVindex = NULL;
			sVprevious = sVcurrent;
		}
		for (OO2POSITION iV = cVlist.last_ () - 1; iV >= cVlist.first (); iV--)  
			if (cVlist.get__ (iV)->sVindex == NULL) {
				deall (cVlist.get__ (iV));
				cVlist.unset (iV);
			}
	}

	/*GET IndeXed*/
	int64vcl __fastcall CoreSoix::getix (OO2POSITION iPosition) {

		return cVlist.get__ (iPosition)->iVindexed;
	}

	/*GET sorted Index Loc*/
	CoreSoix::SORTEDINDEXLOC* __fastcall CoreSoix::getil (OO2POSITION iPosition) {

		return cVlist.get__ (iPosition);
	}

	/*COUNT*/
	int __fastcall CoreSoix::count () {

		return cVlist.count ();
	}

	/*GET SiZe*/
	unsigned int __fastcall CoreSoix::getsz () {
		unsigned int iVreturn;

		iVreturn = 0;
		for (OO2POSITION iV = cVlist.first (); iV < cVlist.last_ (); iV++) 
			iVreturn += cVlist.get__ (iV)->sVindex.Length () + sizeof (int) * 2;
		return iVreturn;
	}

	/*GET Size Data*/
	unsigned int __fastcall CoreSoix::getsd () {

		return iVcurrentsize;
	}

	/*CLEAR*/
	void __fastcall CoreSoix::clear () {

		for (OO2POSITION iV = cVlist.first (); iV < cVlist.last_ (); iV++)  deall (cVlist.get__ (iV));
		cVlist.clear ();
	}

	/*TO CoNTent*/
	void __fastcall CoreSoix::tocnt (PrtoIndx* cVproto) {
		PrtoStix* cVserializedstixloc;
		SORTEDINDEXLOC* cVsortedindexloc;

		for (OO2POSITION iV = cVlist.first (); iV < cVlist.last_ (); iV++)  {
			cVsortedindexloc = cVlist.get__ (iV);
			cVserializedstixloc = cVproto->add_cvstix ();
			cVserializedstixloc->set_svindex (cVsortedindexloc->sVindex.to_string ());
			cVserializedstixloc->set_ivindexed32 (0);
			cVserializedstixloc->set_ivindexed64 (cVsortedindexloc->iVindexed);
			cVserializedstixloc->set_ivmainindexlength (cVsortedindexloc->iVmainindexlength);
		}
	}

	/*FroM CoNTent*/
	void __fastcall CoreSoix::fmcnt (PrtoIndx* cVproto) { 
		AnsiString sVkey;
		PrtoStix* cVserializedstixloc;
		SORTEDINDEXLOC* cVsortedindexloc;

		clear ();
		for (int iV = 0; iV < cVproto->cvstix_size (); iV++) {
			cVserializedstixloc = cVproto->mutable_cvstix (iV);
			sVkey = *cVserializedstixloc->mutable_svindex ();
			cVsortedindexloc = cVlist.set__ (OO2POSITIONNULL, true, &sVkey);
			if (cVserializedstixloc->has_ivmainindexlength ())
				cVsortedindexloc->iVmainindexlength = cVserializedstixloc->ivmainindexlength ();
			else cVsortedindexloc->iVmainindexlength = -1;
			cVsortedindexloc->iVindexed = 
				cVserializedstixloc->has_ivindexed64 () ? 
				cVserializedstixloc->ivindexed64 () : cVserializedstixloc->ivindexed32 ();
		}
	}

	/*FIRST*/
	CoreSoix::OO2POSITION CoreSoix::first () {

		return cVlist.first ();
	}

	/*LAST*/
	CoreSoix::OO2POSITION CoreSoix::last_ () {

		return cVlist.last_ ();
	}

	/*DEBUG*/
	AnsiString __fastcall CoreSoix::debug () { 
		AnsiString sVreturn;
		int iVcycle;

		sVreturn = (AnsiString) cVlist.count () + bCcarriagereturn;  
		iVcycle = 0;
		for (OO2POSITION iV = cVlist.first (); iV < cVlist.last_ (); iV++) { 
			if (cVlist.get__ (iV) == NULL) continue;
			sVreturn += (AnsiString) iVcycle++ + 
									" - " + " (" + (AnsiString) iV.iVposition + ")" + 
									" - " + (AnsiString) cVlist.get__ (iV)->sVindex.prntb ().c_str () + " - " + 
									" - " + (AnsiString) cVlist.get__ (iV)->iVmainindexlength + " - " + 
									(AnsiString) (int) cVlist.get__ (iV)->iVindexed + " - " + bCcarriagereturn;
			//sVreturn += (AnsiString) " x " + cVlist.get__ (iV)->sVindex.debug () + bCcarriagereturn;
		}
		//sVreturn += cVlist.prnta (false) + bCcarriagereturn;
		return sVreturn;
	}

