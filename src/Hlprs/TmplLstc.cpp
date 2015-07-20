#include "CoreLstc.hpp"

	template<typename T, int iVcachecapacity>
  __fastcall CoreLstc<T, iVcachecapacity>::CoreLstc () : CoreList<T> () {
		
  }

	template<typename T, int iVcachecapacity> 
  __fastcall CoreLstc<T, iVcachecapacity>::~CoreLstc () {
	}
  
	/*BiNary SET*/
	template<typename T, int iVcachecapacity>
	T* __fastcall CoreLstc<T, iVcachecapacity>::set__ (int iPosition, bool bInsert) {

		cVcache.clear ();
		return CoreList<T>::set__ (iPosition, bInsert);
	}

	/*BiNary SET*/
	template<typename T, int iVcachecapacity>
	T* __fastcall CoreLstc<T, iVcachecapacity>::bnset (AnsiString sKey) {

		cVcache.clear ();
		return CoreList<T>::bnset (sKey);
	}

	/*BiNary GET*/
	template<typename T, int iVcachecapacity>
	T* __fastcall CoreLstc<T, iVcachecapacity>::bnget (AnsiString sKey) {
		T* cVreturn;

		if (cVcache.get__ (&sKey, cVreturn)) return cVreturn;
		cVreturn = CoreList<T>::bnget (sKey);
		if (cVreturn != NULL) cVcache.set__ (cVreturn); 
		return cVreturn;
	}

	/*BiNary UNSet*/
	template<typename T, int iVcachecapacity>
	bool __fastcall CoreLstc<T, iVcachecapacity>::bnuns (AnsiString sKey) {

		cVcache.clear ();
		return CoreList<T>::bnuns (sKey);
	}

	/*UNSET*/
	template<typename T, int iVcachecapacity>
	bool __fastcall CoreLstc<T, iVcachecapacity>::unset (int iPosition) {

		cVcache.clear ();
		return CoreList<T>::unset (iPosition); 
	}

	/*CLEAR*/
	template<typename T, int iVcachecapacity>
	void __fastcall CoreLstc<T, iVcachecapacity>::clear () { 

		cVcache.clear ();
		CoreList<T>::clear ();
	}

	/*DEBUG*/
	template<typename T, int iVcachecapacity>  
	AnsiString __fastcall CoreLstc<T, iVcachecapacity>::debug () {

		return cVcache.debug ();
	}
