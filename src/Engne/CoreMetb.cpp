#include "CoreMetb.hpp"
#include "TmplList.cpp"
#include "TmplFsys.cpp"
#include "TmplClps.cpp"

using namespace orion;

  __fastcall CoreMetb::CoreMetb () {

		
  }

  __fastcall CoreMetb::CoreMetb (PTR_POOL cPool, AnsiString sId, int iMaxindex, int iMaxsize, int iChunksize) {
		
		init (cPool, sId, iMaxindex, iMaxsize, iChunksize);
  }

  __fastcall CoreMetb::~CoreMetb () {

		clear ();
  }

	/*INIT*/
  void __fastcall CoreMetb::init (PTR_POOL cPool, AnsiString sId, int iMaxindex, int iMaxsize, int iChunksize) {
		
		iVcurrentsize = 0;
		cVpool = cPool;
		sVid = sId;
		cVsortedindex = boost::shared_ptr<BASE_CUSTOMSORTEDINDEX> (new BASE_CUSTOMSORTEDINDEX (cVpool));
		cVserializer = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));
		cVdatalist.clear ();
		cVsortedindex->clear ();
		setmx (iMaxindex, iMaxsize, iChunksize);
		iVcachegetkey = CUSTOMSORTEDINDEXPOSNULL;
  }

	/*SET MaX*/
	void __fastcall CoreMetb::setmx (int iMaxindex, int iMaxsize, int iChunksize) {

		cVmutex.locks ();
		iVmaxindex = iMaxindex;
		iVmaxsize = iMaxsize;
		iVchunksize = iChunksize;
		cVmutex.relse ();
	}

	/*PUT*/
	bool __fastcall CoreMetb::put__ (AnsiString sKey, int iMainindexlength, unsigned int iValue, void* vValue) {
		bool bVrecycle;
		unsigned int iVrecycle;
		DATALOC* cVdataloc;

		if (cVsortedindex->addop (sKey, cVdatalist.count (), iMainindexlength, bVrecycle, iVrecycle)) {
			if (!bVrecycle) cVdataloc = cVdatalist.set__ ();
			else {
				iVcurrentsize -= clear (iVrecycle);
				cVdataloc = cVdatalist.get__ (iVrecycle);
			}			
			cVdataloc->iVsize = iValue;
			if (iValue > 0) {
				cVdataloc->vVdata = cVpool->mallc ( CorePool::iCchar, iValue, "METB");
				memcpy (cVdataloc->vVdata, vValue, iValue);
				iVcurrentsize += iValue;
			}
		}	
		return true;
	}

	/*DELeTE*/
	bool __fastcall CoreMetb::delte (AnsiString sKey, int iMainindexlength) {
		unsigned int iVrecycle;

		cVmutex.locks ();
		while (cVsortedindex->delop (sKey, iMainindexlength, iVrecycle)) iVcurrentsize -= clear (iVrecycle);
		cVmutex.relse ();
		return true;
	}
	
	/*PUT*/
	bool __fastcall CoreMetb::put__ (AnsiString sKey, int iMainindexlength, PrtoLval* cValue) {
		bool bVreturn;

		cVmutex.locks ();
		cVserializer->srlze<PrtoLval> (cValue);	
		bVreturn = put__ (sKey, iMainindexlength, cVserializer->bsize (), cVserializer->baddr ());
		cVmutex.relse ();
		return bVreturn;
	}

	/*GET*/
	bool __fastcall CoreMetb::get__ (AnsiString& sKey, PrtoLval* cValue) {
		CUSTOMSORTEDINDEXPOS iVstartindex;
		CUSTOMSORTEDINDEXPOS iVupperboundindex;
		AnsiString sVcursor;
		DATALOC* cVdataloc;
		
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
				cVdataloc = cVdatalist.get__ (cVsortedindex->getix (iVstartindex));
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
	unsigned int __fastcall CoreMetb::clear (int iV) {
		DATALOC* cVdataloc;
		unsigned int iVreturn;

		iVcachegetkey = CUSTOMSORTEDINDEXPOSNULL;
		if (iV < cVdatalist.count ()) {
			cVdataloc = cVdatalist.get__ (iV);			
			iVreturn = cVdataloc->iVsize;
			if (iVreturn > 0) {
				cVpool->free_ (cVdataloc->vVdata);
				cVdataloc->iVsize = 0;
			}
			return iVreturn;
		}
		return 0;
	}

	/*CLEAR*/
	void __fastcall CoreMetb::clear () {

		for (int iV = 0; iV < cVdatalist.count (); iV++) clear (iV);
		cVdatalist.clear ();
		cVsortedindex->clear ();
		iVcurrentsize = 0;
	}

	/*DEBUG*/
	AnsiString __fastcall CoreMetb::debug () {
		
		return cVsortedindex->debug ();
	}

	/*GET Size Data*/
	unsigned int __fastcall CoreMetb::getsd () {
			
		return iVcurrentsize;
	}

	/*GET ReS*/
	unsigned int __fastcall CoreMetb::getrs () {
		unsigned int iVreturn;

		cVmutex.locks ();
		iVreturn = cVsortedindex->getsz ();
		cVmutex.relse ();
		return iVreturn;
	}

	/*GET MApped*/
	unsigned int __fastcall CoreMetb::getma () {

		return getrs () + iVcurrentsize;
	}


	/*GET Sorted index Count */
	int __fastcall CoreMetb::getsc () {
			
		return cVsortedindex->count ();
	}

	/*GET StiX*/
	__fastcall PTR_CUSTOMSORTEDINDEX CoreMetb::getsx () {
		
		return cVsortedindex;
	}

	/*is FULL*/
	bool __fastcall CoreMetb::full_ () {

		return (iVcurrentsize >= iVmaxsize || cVsortedindex->count () >= iVmaxindex);
	}

	/*is CHUNK*/
	bool __fastcall CoreMetb::chunk () {

		iVchunk = (iVchunk + 1) % iVchunksize;
		return (iVchunk == 0);
	}

	/*GET DaTa*/
	bool __fastcall CoreMetb::getdt (	CUSTOMSORTEDINDEXPOS iLoc, AnsiString& sKey, int& iMainindexlength,
																		unsigned int& iValue, void** vValue) {
		DATALOC* cVdataloc;
		CoreStib::SORTEDINDEXLOC* cVindexloc;

		cVindexloc = cVsortedindex->getil (iLoc);
		sKey = cVindexloc->sVindex;
		iMainindexlength = cVindexloc->iVmainindexlength;
		cVdataloc = cVdatalist.get__ (cVindexloc->iVindexed);
		iValue = cVdataloc->iVsize;
		*vValue = cVdataloc->vVdata;
		return true;
	}

	/*GET ID*/
	AnsiString __fastcall CoreMetb::getid () {

		return sVid;
	}

