#include "CoreLiss.hpp"

	template<typename T, typename C>
  __fastcall CoreLiss<T, C>::CoreLiss () {

  }


	template<typename T, typename C>
  __fastcall CoreLiss<T, C>::~CoreLiss () {
		
		clear ();
  }

	template<typename T, typename C>
	T __fastcall CoreLiss<T, C>::set__(int iPosition, bool bInsert) {
		LOC* cVelement;

		cVmutex.locks ();
		if (iPosition >= cVl.Count) iPosition = -1;
		cVelement = new LOC;
		if (bInsert) cVl.Add (cVelement, iPosition);
		else cVl.Items [iPosition] = cVelement;
		cVmutex.relse ();
		return cVelement->cVloc;
	}

	/*GET*/
	template<typename T, typename C>
	T __fastcall CoreLiss<T, C>::get__(int iPosition) {
		T cVreturn;

		cVmutex.locks ();
		if (iPosition >= 0 && iPosition < cVl.Count)	cVreturn = ((LOC*) cVl.Items [iPosition])->cVloc;
		cVmutex.relse ();
		return cVreturn;
	}

	/*UNSET*/
	template<typename T, typename C>
	void __fastcall CoreLiss<T, C>::unset(int iPosition) {

		cVmutex.locks ();
		if (iPosition >= 0 && iPosition < cVl.Count) {
			delete (LOC*) cVl.Items [iPosition];
			cVl.Delete (iPosition);
		}
		cVmutex.relse ();
	}

	/*COUNT*/
	template<typename T, typename C>
	int __fastcall CoreLiss<T, C>::count () {
		
		return cVl.Count;
	}

	/*CLEAR*/
	template<typename T, typename C>
	void __fastcall CoreLiss<T, C>::clear() {

		cVmutex.locks ();
		for (int iV = 0; iV < cVl.Count; iV++) 
			delete (LOC*) cVl.Items [iV];
		cVl.Clear ();
		cVmutex.relse ();
	}

	/*ALLoCate Void*/
	template<typename T, typename C>
	void __fastcall CoreLiss<T, C>::allcv (int iCapacity) {
		
		clear ();
		cVmutex.locks ();
		cVl.Capacity = iCapacity;
		for (int iV = 0; iV < iCapacity; iV++) cVl.Add (NULL, iV);
		cVmutex.relse ();
	}

	/*BiNary SeaRCh*/
	template<typename T, typename C>
	int __fastcall CoreLiss<T, C>::bnsrc (AnsiString sKey) {

		int iVreturn;
		int iVmin;
		int iVmax;
		int iVtest;
		bool bVgreaterequal;
		AnsiString sVtest;
		T cVtemp;

		iVmin = 0;
		iVmax = count ();
		if (iVmax > 0)
			while (true) {
				iVtest = (iVmin + iVmax) >> 1;
				cVtemp = get__ (iVtest);
				if (cVtemp == NULL) return - 1;
				sVtest = cVtemp->sVindex;
				bVgreaterequal = (sKey >= sVtest);
				if (!bVgreaterequal) iVmax = iVtest;
				else iVmin = iVtest;
				if (iVmin == iVmax) {
					iVreturn = iVtest;
					break;
				}
				if (iVmin == iVmax -1 && bVgreaterequal) {
					iVreturn = iVtest + 1;
					break;
				}
			}
		else iVreturn = -1;
		return iVreturn;
	}

	/*BiNary SET*/
	template<typename T, typename C>
	T __fastcall CoreLiss<T, C>::bnset (AnsiString sKey) {
		int iVinsertposition;
		T cVreturn;

		iVinsertposition = bnsrc (sKey);
		cVreturn = set__ (iVinsertposition);
		cVreturn->sVindex = sKey;
		return cVreturn;
	}

	/*BiNary SeaRCh*/
	template<typename T, typename C>
	T __fastcall CoreLiss<T, C>::bnget (AnsiString sKey) {
		int iVposition;
		T cVreturn;
		T cVnull;

		iVposition = bnsrc (sKey) - 1;
		if (iVposition < 0 || iVposition >= count ()) return cVnull;
		else {
			cVreturn = get__ (iVposition);
			if (cVreturn.use_count () == 0) return cVnull;
			if (cVreturn->sVindex == sKey) return cVreturn;
			else return cVnull;
		}
	}

	/*BiNary UNSet*/
	template<typename T, typename C>
	bool __fastcall CoreLiss<T, C>::bnuns (AnsiString sKey) {
		int iVposition;
		T cVreturn;

		iVposition = bnsrc (sKey) - 1;
		if (!(iVposition < 0 || iVposition >= count ())) {
			cVreturn = get__ (iVposition);
			if (cVreturn.use_count () == 0) return false;
			if (cVreturn->sVindex == sKey) {
				unset (iVposition);
				return true;
			}
		}
		return false;
	}

	/*DEBUG*/
	template<typename T, typename C>
	AnsiString __fastcall CoreLiss<T, C>::debug () {
		AnsiString sVreturn;

		sVreturn = "";
		for (int iV = 0; iV < count (); iV++) 
			sVreturn += (AnsiString) iV + (AnsiString) ":  " + get__ (iV)->sVindex.prntb () + "\n";
		return sVreturn;
	}

