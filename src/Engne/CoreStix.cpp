#include "CoreStix.hpp"
#include "TmplList.cpp"
#include "TmplFsys.cpp"

using namespace orion;

  __fastcall CoreStix::CoreStix (PTR_POOL cPool) {
		
  }

  __fastcall CoreStix::~CoreStix () {

  }

	/*ADD OPtimized*/
	bool __fastcall CoreStix::addop (	AnsiString sKey, int64vcl iIndexed, short int iMainindexlength,
																		bool& bRecycle, unsigned int& iRecycle) {
		SORTEDINDEXLOC* cVsortedindexloc;
		int iVinsposition;
		int iVprvposition;
		int iVlistcount;
		int iVcomparelength;
		bool bVwaste;

		bRecycle = false;
		iVinsposition = bnsrc (sKey);	
		iVprvposition = iVinsposition - 1;
		iVlistcount = cVlist.count ();
		iVcomparelength = iMainindexlength > 0 ? iMainindexlength - 1: sKey.Length ();
		bVwaste = iVprvposition >= 0 && iVprvposition < iVlistcount &&
							cVlist.get__ (iVprvposition)->sVindex.strcmp (sKey, iVcomparelength) == 0;
		if (bVwaste) return false;
		bRecycle = iVinsposition >= 0 && iVinsposition < iVlistcount &&
							 cVlist.get__ (iVinsposition)->sVindex.strcmp (sKey, iVcomparelength) == 0;
		if (bRecycle) {
			cVsortedindexloc = cVlist.get__ (iVinsposition);				
			iRecycle = cVsortedindexloc->iVindexed;
			set__ (cVsortedindexloc, sKey, iRecycle, iMainindexlength);
		} else {
			cVsortedindexloc = cVlist.set__ (iVinsposition);			
			set__ (cVsortedindexloc, sKey, iIndexed, iMainindexlength);
		}						
		return true;
	}

	/*DELete OPtimized*/
	bool __fastcall CoreStix::delop (	AnsiString sKey, short int iMainindexlength, unsigned int& iRecycle) {
		SORTEDINDEXLOC* cVsortedindexloc;
		AnsiString sVkey;
		int iVinsposition;
		int iVlistcount;
		int iVcomparelength;
		bool bVfound;
		
		iVcomparelength = iMainindexlength > 0 ? iMainindexlength - 1: sKey.Length ();
		sVkey = sKey.SubString (1, iVcomparelength);
		iVinsposition = bnsrc (sVkey);	
		iVlistcount = cVlist.count ();		
		bVfound = iVinsposition >= 0 && iVinsposition < iVlistcount &&
							 cVlist.get__ (iVinsposition)->sVindex.strcmp (sKey, iVcomparelength) == 0;
		if (bVfound) {
			cVsortedindexloc = cVlist.get__ (iVinsposition);				
			iRecycle = cVsortedindexloc->iVindexed;
			cVlist.unset (iVinsposition);
			return true;
		}	else return false;
	}


	bool __fastcall CoreStix::add__ (AnsiString sKey, int64vcl iIndexed, short int iMainindexlength) {
		SORTEDINDEXLOC* cVsortedindexloc;
		int iVinsertposition;

		iVinsertposition = bnsrc (sKey);
		if (!(iVinsertposition -1 > 0 && 
					iVinsertposition - 1 < cVlist.count () &&
					cVlist.get__ (iVinsertposition- 1)->sVindex == sKey)) {
			cVsortedindexloc = cVlist.set__ (iVinsertposition);
			set__ (cVsortedindexloc, sKey, iIndexed, iMainindexlength);
			return true;
		} else return false;
	}

	/*ADD Not Sorted*/
	bool __fastcall CoreStix::addns (AnsiString sKey, int64vcl iIndexed, short int iMainindexlength) {
		SORTEDINDEXLOC* cVsortedindexloc;

		cVsortedindexloc = cVlist.set__ ();
		set__ (cVsortedindexloc, sKey, iIndexed, iMainindexlength);
		return true;
	}

	/*BiNary SeaRCh*/
	int __fastcall CoreStix::bnsrc (AnsiString sKey) {

		return cVlist.bnsrc (sKey);
	}

	/*GET*/
	AnsiString __fastcall CoreStix::get__ (int iPosition, bool bOnlymain) {
		SORTEDINDEXLOC* cVsortedindexloc;
		
		cVsortedindexloc = cVlist.get__ (iPosition);
		if (!bOnlymain || cVsortedindexloc->iVmainindexlength <= 0) return cVsortedindexloc->sVindex;
		else return cVsortedindexloc->sVindex.SubString (0, cVsortedindexloc->iVmainindexlength);	
	}

	/*GET IndeXed*/
	int64vcl __fastcall CoreStix::getix (int iPosition) {

		return cVlist.get__ (iPosition)->iVindexed;
	}

	/*GET sorted Index Loc*/
	CoreStix::SORTEDINDEXLOC* __fastcall CoreStix::getil (int iPosition) {

		return cVlist.get__ (iPosition);
	}

	void __fastcall CoreStix::setil (SORTEDINDEXLOC* cSortedindexloc, int iPosition) {
		SORTEDINDEXLOC* cVsortedindexloc;

		cVsortedindexloc = cVlist.set__ (iPosition);
		cVsortedindexloc->sVindex = cSortedindexloc->sVindex;
		cVsortedindexloc->iVindexed = cSortedindexloc->iVindexed;
		cVsortedindexloc->iVmainindexlength = cSortedindexloc->iVmainindexlength;
	}

	/*SET IndeXed*/
	void __fastcall CoreStix::setix (int iPosition, int64vcl iIndexed) {

		cVlist.get__ (iPosition)->iVindexed = iIndexed;
	}

	/*COUNT*/
	int __fastcall CoreStix::count () {

		return cVlist.count ();
	}

	/*GET SiZe*/
	unsigned int __fastcall CoreStix::getsz () {
		unsigned int iVreturn;

		iVreturn = 0;
		for (int iV = 0; iV < cVlist.count (); iV++) 
			iVreturn += cVlist.get__ (iV)->sVindex.Length () + sizeof (int) * 2;
		return iVreturn;
	}

	/*DEBUG*/
	AnsiString __fastcall CoreStix::debug () {
		AnsiString sVreturn;

		sVreturn = (AnsiString) cVlist.count () + bCcarriagereturn;
		for (int iV = 0; iV < cVlist.count (); iV++) {
			sVreturn += (AnsiString) iV + " - " + (AnsiString) cVlist.get__ (iV)->sVindex.c_str () + " - " + 
									" - " + (AnsiString) cVlist.get__ (iV)->iVmainindexlength + " - " + 
									(AnsiString) (int) cVlist.get__ (iV)->iVindexed + " - " + bCcarriagereturn;
			sVreturn += (AnsiString) " x " + cVlist.get__ (iV)->sVindex.debug () + bCcarriagereturn;
		}
		return sVreturn;
	}

	/*CLEAR*/
	void __fastcall CoreStix::clear () {
		
		cVlist.clear ();
	}

	/*TO CoNTent*/
	void __fastcall CoreStix::tocnt (PrtoIndx* cVproto) {
		PrtoStix* cVserializedstixloc;
		SORTEDINDEXLOC* cVsortedindexloc;

		for (int iV = 0; iV < cVlist.count (); iV++)  {
			cVsortedindexloc = cVlist.get__ (iV);
			cVserializedstixloc = cVproto->add_cvstix ();
			cVserializedstixloc->set_svindex (cVsortedindexloc->sVindex.to_string ());
			cVserializedstixloc->set_ivindexed32 (0);
			cVserializedstixloc->set_ivindexed64 (cVsortedindexloc->iVindexed);
			cVserializedstixloc->set_ivmainindexlength (cVsortedindexloc->iVmainindexlength);
		}
	}

	/*FroM CoNTent*/
	void __fastcall CoreStix::fmcnt (PrtoIndx* cVproto) {
		PrtoStix* cVserializedstixloc;
		SORTEDINDEXLOC* cVsortedindexloc;

		clear ();
		for (int iV = 0; iV < cVproto->cvstix_size (); iV++) {
			cVserializedstixloc = cVproto->mutable_cvstix (iV);
			cVsortedindexloc = cVlist.set__ ();
			cVsortedindexloc->sVindex = *cVserializedstixloc->mutable_svindex ();
			if (cVserializedstixloc->has_ivmainindexlength ())
				cVsortedindexloc->iVmainindexlength = cVserializedstixloc->ivmainindexlength ();
			else cVsortedindexloc->iVmainindexlength = -1;
			cVsortedindexloc->iVindexed = 
				cVserializedstixloc->has_ivindexed64 () ? 
				cVserializedstixloc->ivindexed64 () : cVserializedstixloc->ivindexed32 ();
		}
	}

	/*FIRST*/
	int CoreStix::first () {

		return 0;
	}

	/*LAST*/
	int CoreStix::last_ () {

		return cVlist.count ();
	}

	/*DELete DuPlicates*/
	void __fastcall CoreStix::deldp () {
		AnsiString sVnetindex;
		AnsiString sVprevious;
		AnsiString sVcurrent;
		CoreStix::SORTEDINDEXLOC* cVsortedindexloc;

		sVprevious = "";
		for (int iV = 0; iV < cVlist.count (); iV++)  {
			cVsortedindexloc = cVlist.get__ (iV);
			if (cVsortedindexloc->iVmainindexlength < 0) continue;
			sVcurrent = getmn (cVsortedindexloc);
			if (sVcurrent == sVprevious) cVsortedindexloc->sVindex = NULL;
			sVprevious = sVcurrent;
		}
		for (int iV = cVlist.count () - 1; iV >= 0; iV--)  
			if (cVlist.get__ (iV)->sVindex == NULL) cVlist.unset (iV);		
	}

