#include "CoreCche.hpp"

	template<typename T, int iVcapacity>
  __fastcall CoreCche<T, iVcapacity>::CoreCche () {
		
		cVl.Capacity = iVcapacity;
		for (int iV = 0; iV < iVcapacity; iV++) cVl.Add (new CACHELOC);
		cVfirst = (CACHELOC*) cVl [0];
		for (int iV = 0; iV < iVcapacity - 1; iV++) {
			CACHELOC* cVthis = (CACHELOC*) cVl [iV];
			CACHELOC* cVfollowing = (CACHELOC*) cVl [iV + 1];
			cVthis->cVnext = cVfollowing;
			cVfollowing->cVprev = cVthis;
		}
		cVlast = (CACHELOC*) cVl [iVcapacity - 1];
  }

	template<typename T, int iVcapacity>
  __fastcall CoreCche<T, iVcapacity>::~CoreCche () {
		
		for (int iV = 0; iV < iVcapacity; iV++) delete (CACHELOC*) cVl [iV];
		cVl.Clear ();
  }

	/*CLEAR*/
	template<typename T, int iVcapacity>
	void __fastcall CoreCche<T, iVcapacity>::clear () {

		for (int iV = 0; iV < iVcapacity; iV++) ((CACHELOC*) cVl [iV])->bVvalid = false;
	}

	/*SET*/
	template<typename T, int iVcapacity>
  void __fastcall CoreCche<T, iVcapacity>::set__ (T cElement) {

		cVfirst->cVprev = cVlast;
		cVlast->cVnext = cVfirst;
		cVfirst = cVlast;
		cVlast = cVlast->cVprev;		
		cVlast->cVnext = NULL; 
		cVfirst->cVloc = cElement;
		cVfirst->bVvalid = true;
	}

	/*GET*/
	template<typename T, int iVcapacity>
  bool __fastcall CoreCche<T, iVcapacity>::get__ (AnsiString* sKey, T& cReturn) {
		CACHELOC* cVcursor;

		cVcursor = cVfirst;
		while (cVcursor != NULL && cVcursor->bVvalid)  {
			if (cVcursor->cVloc->sVindex == *sKey) {
				if (cVfirst != cVcursor) {
					cVcursor->cVprev->cVnext = cVcursor->cVnext;
					if (cVlast != cVcursor) cVcursor->cVnext->cVprev = cVcursor->cVprev;
					else cVlast = cVcursor->cVprev;
					cVcursor->cVnext = cVfirst;
					cVfirst->cVprev = cVcursor;
					cVfirst = cVcursor;
					
				}
				cReturn = cVcursor->cVloc; 
				return true;
			}
			cVcursor = cVcursor->cVnext;
		};
		return false;
	}

	/*DEBUG*/
	template<typename T, int iVcapacity>
  AnsiString __fastcall CoreCche<T, iVcapacity>::debug () {
		CACHELOC* cVcursor;
		AnsiString sVreturn;
		int iV;

		iV = 0;
		sVreturn = ""; 
		cVcursor = cVfirst; 
		while (cVcursor != NULL && cVcursor->bVvalid)  {
			sVreturn += (AnsiString) iV + (AnsiString) ": " + cVcursor->cVloc->sVindex + bCcarriagereturn;
			cVcursor = cVcursor->cVnext;
			iV++; 
		};
		if (cVfirst->bVvalid) sVreturn += (AnsiString) "F=" + cVfirst->cVloc->sVindex.c_str () + bCcarriagereturn; 
		if (cVlast->bVvalid) sVreturn += (AnsiString) "L=" + cVlast->cVloc->sVindex.c_str () + bCcarriagereturn;
		return sVreturn;
	}