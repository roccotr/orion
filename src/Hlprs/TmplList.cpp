#include "CoreList.hpp"

using namespace orion;

	template<typename T>
  __fastcall CoreList<T>::CoreList () {
		
  }

	template<typename T>
  __fastcall CoreList<T>::~CoreList () {
		
		clear ();
  }

	/*SET*/
	template<typename T>
	T* __fastcall CoreList<T>::set__(int iPosition, bool bInsert) {
		T* cVelement;

		cVmutex.locks ();
		if (iPosition >= cVl.Count) iPosition = -1;
		cVelement = new T;
		if (bInsert) cVl.Add (cVelement, iPosition);
		else cVl.Items [iPosition] = cVelement;
		cVmutex.relse ();
		return cVelement;
	}

	/*GET*/
	template<typename T>
	T* __fastcall CoreList<T>::get__(int iPosition) {
		T* cVreturn;

		cVmutex.locks ();
		if (iPosition >= 0 && iPosition < cVl.Count)	cVreturn = (T*) cVl.Items [iPosition];
		else cVreturn = NULL;;
		cVmutex.relse ();
		return cVreturn;
	}

	/*UNSET*/
	template<typename T>
	bool __fastcall CoreList<T>::unset(int iPosition) {
		bool bVreturn;

		cVmutex.locks ();
		if (iPosition >= 0 && iPosition < cVl.Count) {
			delete (T*) cVl.Items [iPosition];
			cVl.Delete (iPosition);
			bVreturn = true;
		} else bVreturn = false;
		cVmutex.relse ();
		return bVreturn;
	}

	/*COUNT*/
	template<typename T>
	int __fastcall CoreList<T>::count () {
		
		return cVl.Count;
	}

	/*CLEAR*/
	template<typename T>
	void __fastcall CoreList<T>::clear() {

		cVmutex.locks ();
		for (int iV = 0; iV < cVl.Count; iV++) 
			delete (T*) cVl.Items [iV];
		cVl.Clear ();
		cVmutex.relse ();
	}

	/*ALLoCate Void*/
	template<typename T>
	void __fastcall CoreList<T>::allcv (int iCapacity) {
		
		clear ();
		cVmutex.locks ();
		cVl.Capacity = iCapacity;
		for (int iV = 0; iV < iCapacity; iV++) cVl.Add (NULL, iV);
		cVmutex.relse ();
	}

	/*BiNary SeaRCh*/
	template<typename T>
	int __fastcall CoreList<T>::bnsrc (AnsiString sKey) {

		int iVreturn;
		int iVmin;
		int iVmax;
		int iVtest;
		bool bVgreaterequal;
		AnsiString sVtest;
		T* cVtemp;

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

	/*BiNary SeaRCh*/
	template<typename T>
	int __fastcall CoreList<T>::bnsrc (AnsiString* sKey, int& iComparison) {

		int iVreturn;
		int iVmin;
		int iVmax;
		int iVtest;
		bool bVgreaterequal;
		AnsiString sVtest;
		T* cVtemp;

		iComparison = -1;
		iVmin = 0;
		iVmax = count ();
		if (iVmax > 0)
			while (true) {
				iVtest = (iVmin + iVmax) >> 1;
				cVtemp = get__ (iVtest);
				if (cVtemp == NULL) return - 1;
				sVtest = cVtemp->sVindex;
				iComparison = sKey->strcmp (sVtest);
				bVgreaterequal = iComparison >= 0;
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
	template<typename T>
	T* __fastcall CoreList<T>::bnset (AnsiString sKey) {
		int iVinsertposition;
		T* cVreturn;

		iVinsertposition = bnsrc (sKey);
		cVreturn = set__ (iVinsertposition);
		cVreturn->sVindex = sKey;
		return cVreturn;
	}

	/*BiNary SeaRCh*/
	template<typename T>
	T* __fastcall CoreList<T>::bnget (AnsiString sKey) {
		int iVposition;
		T* cVreturn;

		iVposition = bnsrc (sKey) - 1;
		if (iVposition < 0 || iVposition >= count ()) return NULL;
		else {
			cVreturn = get__ (iVposition);
			if (cVreturn == NULL) return NULL;
			if (cVreturn->sVindex == sKey) return cVreturn;
			else return NULL;
		}
	}

	/*BiNary UNSet*/
	template<typename T>
	bool __fastcall CoreList<T>::bnuns (AnsiString sKey) {
		int iVposition;
		T* cVreturn;

		iVposition = bnsrc (sKey) - 1;
		if (!(iVposition < 0 || iVposition >= count ())) {
			cVreturn = get__ (iVposition);
			if (cVreturn == NULL) return false;
			if (cVreturn->sVindex == sKey) {
				unset (iVposition);
				return true;
			}
		}
		return false;
	}

	/*SPLIT*/
	template<typename T>
	void __fastcall CoreList<T>::split (CoreList* cDestination, int iBegin) {
		int iVcount;
		int iV;
		int iVnew;

		cVmutex.locks ();
		cDestination->cVmutex.locks ();
		iVcount = cVl.Count - iBegin;
		cDestination->cVl.Capacity = iVcount;
		for (iV = 0; iV < iVcount; iV++) cDestination->cVl.Add (NULL, iV);
		for (iV = iBegin, iVnew = 0; iVnew < iVcount; iV++, iVnew++)  cDestination->cVl [iVnew] = cVl [iV];
		for (iV = cVl.Count - 1; iV >= iBegin; iV--) cVl.Delete (iV);
		cDestination->cVmutex.relse ();
		cVmutex.relse ();
	}

	/*DEBUG*/
	template<typename T>
	AnsiString __fastcall CoreList<T>::debug () {
		AnsiString sVreturn;

		sVreturn = "";
		for (int iV = 0; iV < count (); iV++) 
			sVreturn += (AnsiString) iV + (AnsiString) ":  " + get__ (iV)->sVindex.prntb () + "\n";
		return sVreturn;
	}

