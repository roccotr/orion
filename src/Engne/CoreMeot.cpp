#include "CoreMeot.hpp"
#include "TmplList.cpp"
#include "TmplFsys.cpp"
#include "TmplClps.cpp"

using namespace orion;

  __fastcall CoreMeot::CoreMeot () {
  }

  __fastcall CoreMeot::CoreMeot (PTR_POOL cPool, AnsiString sId, int iMaxindex, int iMaxsize, int iChunksize) {
		
		init (cPool, sId, iMaxindex, iMaxsize, iChunksize);
  }

  __fastcall CoreMeot::~CoreMeot () {

		clear ();
  }

	/*INIT*/
  void __fastcall CoreMeot::init (PTR_POOL cPool, AnsiString sId, int iMaxindex, int iMaxsize, int iChunksize) {
		
		cVpool = cPool;
		sVid = sId;
		cVsortedindex = boost::shared_ptr<BASE_CUSTOMSORTEDINDEX> (new BASE_CUSTOMSORTEDINDEX (cVpool));
		cVserializer = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));
		cVsortedindex->clear ();
		setmx (iMaxindex, iMaxsize, iChunksize);
		iVcachegetkey = CUSTOMSORTEDINDEXPOSNULL;
  }

	/*SET MaX*/
	void __fastcall CoreMeot::setmx (int iMaxindex, int iMaxsize, int iChunksize) {

		cVmutex.locks ();
		iVmaxindex = iMaxindex;
		iVmaxsize = iMaxsize;
		iVchunksize = iChunksize;
		cVmutex.relse ();
	}

	/*PUT*/
	bool __fastcall CoreMeot::put__ (AnsiString sKey, int iMainindexlength, unsigned int iValue, void* vValue) {
		CoreStib::DATALOC cVdataloc (iValue, vValue);

		return cVsortedindex->addo2 (sKey, iMainindexlength, &cVdataloc);
	}

	/*DELeTE*/
	bool __fastcall CoreMeot::delte (AnsiString sKey, int iMainindexlength) {
		bool bVreturn;
		bool bVsearchallkey;

		cVmutex.locks ();
		bVreturn = false;
		bVsearchallkey = false;
		while (cVsortedindex->delo2 (sKey, iMainindexlength, bVsearchallkey)) bVreturn = true;
		iVcachegetkey = CUSTOMSORTEDINDEXPOSNULL;
		cVmutex.relse ();
		return bVreturn;
	}
	
	/*PUT*/
	bool __fastcall CoreMeot::put__ (AnsiString sKey, int iMainindexlength, PrtoLval* cValue) {
		bool bVreturn;

		cVmutex.locks ();
		cVserializer->srlze<PrtoLval> (cValue);	
		bVreturn = put__ (sKey, iMainindexlength, cVserializer->bsize (), cVserializer->baddr ());
		iVcachegetkey = CUSTOMSORTEDINDEXPOSNULL;
		cVmutex.relse ();
		return bVreturn;
	}

	/*GET*/
	bool __fastcall CoreMeot::get__ (AnsiString& sKey, PrtoLval* cValue) {
		CUSTOMSORTEDINDEXPOS iVstartindex;
		CUSTOMSORTEDINDEXPOS iVupperboundindex;
		AnsiString sVcursor;
		CoreStib::DATALOC* cVdataloc;
		
		cVmutex.locks ();
		if (cVsortedindex->count () == 0) { 
			cVmutex.relse ();
			return false;
		}
		iVupperboundindex = cVsortedindex->last_ ();

		if (	iVcachegetkey >= 0 && iVcachegetkey < iVupperboundindex &&
					sVcachegetkeyinput <= sKey && sVcachegetkeyoutput >= sKey) {
			iVstartindex = iVcachegetkey;   
		} else {
			iVcachegetkey = iVstartindex = cVsortedindex->bnsrc (sKey); 
			sVcachegetkeyoutput = NULL;
		}
		sVcachegetkeyinput = sKey;
		while (iVstartindex < cVsortedindex->last_ ()) { 
			sVcursor = cVsortedindex->get__ (iVstartindex);		  			
			if (sVcursor >= sKey) {
				iVcachegetkey = iVstartindex;
				sVcachegetkeyoutput = sKey = sVcursor;
				cVdataloc = &cVsortedindex->getil (iVstartindex)->cVdataloc;
				cVserializer->setmb (cVdataloc->vVdata, cVdataloc->iVsize);
				cVserializer->parse<PrtoLval> (cValue);
				cVmutex.relse ();
				return true;
			}
			iVstartindex++;
			if (!(iVstartindex < iVupperboundindex)) break;
		}

		cVmutex.relse ();
		return false;
	}

	/*CLEAR*/
	void __fastcall CoreMeot::clear () {

		cVsortedindex->clear ();
	}

	/*DEBUG*/
	AnsiString __fastcall CoreMeot::debug () {
		
		return cVsortedindex->debug () + (AnsiString) (int) getsd () + (AnsiString) "\n";
	}

	/*GET Size Data*/
	unsigned int __fastcall CoreMeot::getsd () {
		unsigned int iVreturn;

		cVmutex.locks ();
		iVreturn = cVsortedindex->getsd ();
		cVmutex.relse ();
		return iVreturn;
	}

	/*GET ReS*/
	unsigned int __fastcall CoreMeot::getrs () {
		unsigned int iVreturn;

		cVmutex.locks ();
		iVreturn = cVsortedindex->getsz ();
		cVmutex.relse ();
		return iVreturn;
	}

	/*GET MApped*/
	unsigned int __fastcall CoreMeot::getma () {

		return getrs () + getsd ();
	}

	/*GET Sorted index Count */
	int __fastcall CoreMeot::getsc () {
			
		return cVsortedindex->count ();
	}

	/*GET StiX*/
	__fastcall PTR_CUSTOMSORTEDINDEX CoreMeot::getsx () {
		
		return cVsortedindex;
	}

	/*is FULL*/
	bool __fastcall CoreMeot::full_ () {

		return (cVsortedindex->getsd ()  >= iVmaxsize || cVsortedindex->count () >= iVmaxindex);
	}

	/*is CHUNK*/
	bool __fastcall CoreMeot::chunk () {

		iVchunk = (iVchunk + 1) % iVchunksize;
		return (iVchunk == 0);
	}

	/*GET DaTa*/
	bool __fastcall CoreMeot::getdt (	CUSTOMSORTEDINDEXPOS iLoc, AnsiString& sKey, int& iMainindexlength,
																		unsigned int& iValue, void** vValue) {
		CoreStib::DATALOC* cVdataloc;
		CoreStib::SORTEDINDEXLOC* cVindexloc;

		cVindexloc = cVsortedindex->getil (iLoc);
		sKey = cVindexloc->sVindex;
		iMainindexlength = cVindexloc->iVmainindexlength;
		cVdataloc = &cVindexloc->cVdataloc;
		iValue = cVdataloc->iVsize;
		*vValue = cVdataloc->vVdata;
		return true;
	}

	/*GET ID*/
	AnsiString __fastcall CoreMeot::getid () {

		return sVid;
	}

